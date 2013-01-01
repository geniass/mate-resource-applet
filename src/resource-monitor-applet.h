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

#ifndef RESOURCE_MONITOR_APPLET_H
#define RESOURCE_MONITOR_APPLET_H

#include <gtk/gtk.h>
#include <mate-panel-applet.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

struct _ResourceMonitorApplet {
        /* The actual applet */
        MatePanelApplet *applet;
        gint size;
        
        GtkWidget *table; 
};


void resource_monitor_applet_init(ResourceMonitorApplet *applet);

#endif /* RESOURCE_MONITOR_APPLET_H */
