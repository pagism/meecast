/* vim: set sw=4 ts=4 et: */
/* This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
*/
/*******************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "weather-main.h"
#include "weather-home.h"
/*******************************************************************************/
int
main(int argc, char *argv[]){
    GtkWidget		*OMWeather = NULL;
    osso_context_t	*osso_context;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif
    osso_context = osso_initialize(PACKAGE, VERSION, FALSE, NULL);
    if(!osso_context){
	    g_error("osso_initialize failed\n");
	    return 1;
    }
    gtk_init(&argc, &argv);

    OMWeather = create_omweather();
    if(OMWeather){
        app->osso = osso_context;
	    gtk_widget_show(OMWeather);
	    gtk_main();
	    gtk_widget_destroy(OMWeather);
    }
    osso_deinitialize(osso_context);
    return 0;
}
/*******************************************************************************/
gboolean
main_widget_button_key_press_cb (GtkWidget   *widget,
                                        GdkEventKey *event,
                                        gpointer     user_data)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (event->keyval == GDK_F6){
    /* The "Full screen" hardware key has been pressed */
        if (app->fullscreen == GDK_WINDOW_STATE_FULLSCREEN)
            gtk_window_unfullscreen (GTK_WINDOW(user_data));
        else
            gtk_window_fullscreen (GTK_WINDOW(user_data));
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

    return FALSE;
}
/*******************************************************************************/
gboolean
main_window_state_event_cb(GtkWidget   *widget,
                                        GdkEventWindowState *event,
                                        gpointer     user_data)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->fullscreen =  (event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN);
    return FALSE;
}
/*******************************************************************************/
GtkWidget*
create_omweather(void){
    GtkWidget	*main_widget = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_widget), PACKAGE);
    gtk_window_set_default_size(GTK_WINDOW(main_widget), 640, 480);
    if(!omweather_init_OS2009(main_widget))
	return NULL;
/* signals */
    g_signal_connect((gpointer)main_widget, "destroy_event",
			G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect((gpointer)main_widget, "delete_event",
			G_CALLBACK (gtk_main_quit), NULL);
    gtk_widget_show_all(main_widget);
/* Connect to keypress */
    g_signal_connect (main_widget, "key_press_event",
                      G_CALLBACK (main_widget_button_key_press_cb),
                      main_widget);
/* For fullscreen/ unfullscreen mode */
    g_signal_connect (main_widget, "window_state_event",
                      G_CALLBACK (main_window_state_event_cb),
                      NULL);

    return main_widget;
}/*******************************************************************************/
