/*
 * =====================================================================================
 *
 *       Filename:  resource-monitor-applet.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/12/2012 19:23:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ari Croock (ac), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef RESOURCE_APPLET_H
#define RESOURCE_APPLET_H

#include <gtk/gtk.h>
#include <mate-panel-applet.h>

typedef struct ResourceApplet ResourceApplet;
struct ResourceApplet {
        /* The actual applet */
        MatePanelApplet *applet;
        gint size;
        
        GtkWidget *table; 
};


void resource_applet_init(ResourceApplet *applet);

#endif /* RESOURCE_APPLET_H */
