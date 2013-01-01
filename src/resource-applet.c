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
#include <glib/gprintf.h>
#include <gio/gio.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include "resource-applet.h"


/* callbacks for panel menu 
TODO: implement these*/
static void prefs_cb(GtkAction *action,
        gpointer *data) {}

static void about_cb(GtkAction *action,
        gpointer data) {}

static void help_cb(GtkAction *action, 
        gpointer data) {}

static void destroy_cb(GtkWidget *widget, gpointer data) {
    ResourceMonitorApplet *resource_applet;
    resource_applet = (ResourceMonitorApplet *) data;
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
    ResourceMonitorApplet *resource_applet;
    MatePanelAppletOrient orient;

    g_debug("size-allocate occurred");

    resource_applet = (ResourceMonitorApplet *) data;
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


void resource_monitor_applet_init(ResourceMonitorApplet* resource_applet){
    g_assert(resource_applet);
    g_assert(resource_applet->applet);

    resource_applet->size = 50;

    mate_panel_applet_set_flags(resource_applet->applet, MATE_PANEL_APPLET_EXPAND_MINOR);

    g_signal_connect(resource_applet->applet, "destroy", G_CALLBACK(destroy_cb), resource_applet);

    GtkWidget* label = gtk_label_new("Blah because fuck you that's how");

    gtk_container_add(GTK_CONTAINER(resource_applet->applet), label);
    gtk_widget_show_all(GTK_WIDGET(resource_applet->applet));

}
