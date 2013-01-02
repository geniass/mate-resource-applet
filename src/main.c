/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main applet factory
 *
 *        Version:  1.0
 *        Created:  02/01/2013 17:04:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ari Croock (ac), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <mate-panel-applet.h>
#include <string.h>
#include "resource-applet.h"

static gboolean resource_applet_fill(MatePanelApplet* applet,
                                    const gchar *iid,
                                    gpointer data){
    ResourceApplet *resource_applet;
    gboolean retval = FALSE;

    if(strcmp(iid, "ResourceApplet") == 0){
        resource_applet = g_new0(ResourceApplet, 1);
        resource_applet->applet = applet;
        resource_applet_init(resource_applet);
        retval = TRUE;
    }
    return retval;
}

MATE_PANEL_APPLET_OUT_PROCESS_FACTORY ("ResourceAppletFactory",
                                        PANEL_TYPE_APPLET,
                                        "ResourceApplet",
                                        resource_applet_fill,
                                        NULL);
