/*
 * =====================================================================================
 *
 *       Filename:  resource-monitor-applet.c
 *
 *    Description:  MATE Applet to monitor free RAM, CPU usage and maybe other
 *    stuff
 *
 *        Version:  1.0
 *        Created:  31/12/2012 18:50:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ari Croock (ac), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>
#include <mate-panel-applet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <assert.h>

#include <sys/sysinfo.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include "resource-applet.h"


/* Determines if to continue the timer or not */
static gboolean continue_timer = FALSE;

/* Determines if the timer has started */
static gboolean start_timer = FALSE;

/* Display seconds expired */
static int sec_expired = 0;

const char* freeram = " used of ";
const char* percent_used_ram = "% used";
const char* mb = " MB";


/* callbacks for panel menu 
TODO: implement these*/
static void prefs_cb(GtkAction *action,
        gpointer *data) {}

static void about_cb(GtkAction *action,
        gpointer data) {}

static void help_cb(GtkAction *action, 
        gpointer data) {}

static void destroy_cb(GtkWidget *widget, gpointer data) {
    ResourceApplet *resource_applet;
    resource_applet = (ResourceApplet *) data;
    /* Destroy all components of the applet, then the applet itself */

    gtk_widget_destroy(GTK_WIDGET(resource_applet->applet));

    g_free(resource_applet);
    return;
}

static void change_background_cb(MatePanelApplet *applet, 
        MatePanelAppletBackgroundType type,
        GdkColor *color, 
        GdkPixmap *pixmap, 
        gpointer *data) {
    GtkRcStyle *rc_style;
    GtkStyle *style;

    g_debug("change-background occurred");

    /* reset style */
    gtk_widget_set_style(GTK_WIDGET(applet), NULL);
    rc_style = gtk_rc_style_new();
    gtk_widget_modify_style(GTK_WIDGET(applet), rc_style);
    gtk_rc_style_unref(rc_style);

    switch(type) {
        case PANEL_COLOR_BACKGROUND:
            gtk_widget_modify_bg(GTK_WIDGET(applet),
                    GTK_STATE_NORMAL, color);
            break;

        case PANEL_PIXMAP_BACKGROUND:
            style = gtk_style_copy(GTK_WIDGET(applet)->style);
            if (style->bg_pixmap[GTK_STATE_NORMAL]) {
                g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);
            }
            style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
            gtk_widget_set_style(GTK_WIDGET(applet), style);
            g_object_unref(style);
            break;

        case PANEL_NO_BACKGROUND:
            /* fall through */
        default:
            break;
    }
}

static void change_orient_cb (MatePanelApplet *applet, 
        MatePanelAppletOrient orient, 
        gpointer data) {}

static void size_allocate_cb(MatePanelApplet *applet, 
        GtkAllocation *allocation, 
        gpointer data) {
    ResourceApplet *resource_applet;
    MatePanelAppletOrient orient;

    g_debug("size-allocate occurred");

    resource_applet = (ResourceApplet *) data;
    orient = mate_panel_applet_get_orient(resource_applet->applet);

    if ((orient == MATE_PANEL_APPLET_ORIENT_LEFT) || 
            (orient == MATE_PANEL_APPLET_ORIENT_RIGHT)) {
        if(resource_applet->size == allocation->width)
            return;

        resource_applet->size = allocation->width;

    } else{
        if(resource_applet->size == allocation->height)
            return;
        resource_applet->size = allocation->height;

        /* TODO: call all the necessary *_changed functions */
    }
}

static void style_set_cb(GtkWidget *widget,
        GtkStyle *old_style,
        gpointer data) {}

/*static const GtkActionEntry sensors_applet_menu_actions[] = {
  { "Preferences", GTK_STOCK_PROPERTIES, N_("_Preferences"),
  NULL, NULL,
  G_CALLBACK(prefs_cb) },
  { "Help", GTK_STOCK_HELP, N_("_Help"),
  NULL, NULL,
  G_CALLBACK(help_cb) },
  { "About", GTK_STOCK_ABOUT, N_("_About"),
  NULL, NULL,
  G_CALLBACK(about_cb) }
  };*/

static gboolean
_label_update(gpointer data)
{
    GtkLabel *label = (GtkLabel*)data;

    struct sysinfo system;
    sysinfo(&system);

    double totalram_mb = (double) ((double)(system.totalram * system.mem_unit) / (1024 * 1024));
    double freeram_mb = (double) ((double) (system.freeram * system.mem_unit) / (1024 * 1024));
    double percent_used = ((totalram_mb - freeram_mb) * 100) / totalram_mb;
    unsigned int mem_unit = system.mem_unit;
    printf("mem_unit: %u", mem_unit);

    const int totalram_len = snprintf(NULL, 0, "%.0f", totalram_mb); /* work out length of buffer */
    assert(totalram_len > 0);
    /* C99 */
    char buffer_totalram[totalram_len + 1];
    double_to_string(totalram_mb, totalram_len, buffer_totalram);

    const int freeram_len = snprintf(NULL, 0, "%.0f", freeram_mb); /* work out length of buffer */
    assert(freeram_len > 0);
    /* C99 */
    char buffer_freeram[freeram_len + 1];
    double_to_string(freeram_mb, freeram_len, buffer_freeram);

    const int percent_len = snprintf(NULL, 0, "%.0f", percent_used); /* work out length of buffer */
    assert(percent_len > 0);
    /* C99 */
    char buffer_percent_used[percent_len + 1];
    double_to_string(percent_used, percent_len, buffer_percent_used);


    char output[strlen(freeram) + strlen(mb) + totalram_len + freeram_len + percent_len + strlen(mb) + strlen(percent_used_ram) + 3 + 1];
strcpy(output, buffer_freeram);
strcat(output, mb);
    strcat(output, freeram);
    strcat(output, buffer_totalram);
        strcat(output, " MB | ");
    strcat(output, buffer_percent_used);
    strcat(output, percent_used_ram);
    gtk_label_set_label(label, output);

    return continue_timer;
}

static void
_start_timer (GtkWidget *button, gpointer data)
{
    (void)button;/*Avoid compiler warnings*/
    GtkWidget *label = data;
    if(!start_timer)
    {
        g_timeout_add_seconds(1, _label_update, label);
        start_timer = TRUE;
        continue_timer = TRUE;
    }
}


void double_to_string(double val, int len, char* buffer) {
    int c = snprintf(buffer, len + 1, "%.0f", val);
    assert(buffer[len] == '\0');
    assert(c == len);
}


void resource_applet_init(ResourceApplet* resource_applet){
    g_assert(resource_applet);
    g_assert(resource_applet->applet);

    resource_applet->size = 50;

    mate_panel_applet_set_flags(resource_applet->applet, MATE_PANEL_APPLET_EXPAND_MINOR);

    g_signal_connect(resource_applet->applet, "destroy", G_CALLBACK(destroy_cb), resource_applet);

    struct sysinfo system;
    sysinfo(&system);

    double totalram_mb = ((double)(system.totalram * system.mem_unit));
    double freeram_mb = (double) ((double) (system.freeram * system.mem_unit));
    double percent_used = ((totalram_mb - freeram_mb) * 100) / totalram_mb;
        
    //int len = snprintf(NULL, 0, "%.3f", totalram_mb); /* work out length of buffer */
    //assert(len > 0);
    /* C99 */
    //char buffer[len + 1];
    //double_to_string(totalram_mb, len, buffer);
    
/* don't know how to reuse this */

    int len = snprintf(NULL, 0, "%.0f", freeram_mb); /* work out length of buffer */
    assert(len > 0);
    /* C99 */
    char buffer[len + 1];
    double_to_string(freeram_mb, len, buffer);


    char output[10 + len + 3 + 1];
    strcpy(output, "Free RAM: ");
    strcat(output, buffer);
    strcat(output, " MB");
    GtkWidget* label = gtk_label_new(output);

    //int len = snprintf(NULL, 0, "%.3f", percent_used); /* work out length of buffer */
    //assert(len > 0);
    /* C99 */
    //char buffer[len + 1];
    //double_to_string(percent_used, len, buffer);


    //char output[10 + len + 3 + 1];
    //strcpy(output, "Free RAM: ");
    //strcat(output, buffer);
    //strcat(output, " MB");
    //GtkWidget* label = gtk_label_new(output);
    

    _start_timer(NULL, label);
    g_timeout_add_seconds(1, _label_update, label);
    continue_timer = TRUE;
    start_timer = TRUE;


    gtk_container_add(GTK_CONTAINER(resource_applet->applet), label);
    gtk_widget_show_all(GTK_WIDGET(resource_applet->applet));

}
