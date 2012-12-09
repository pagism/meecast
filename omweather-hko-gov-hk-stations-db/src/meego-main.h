/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of omweather-foreca-com-source - MeeCast
 *
 * Copyright (C) 2012 Vlad Vasilyeu
 * 	for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _main_h
#define _main_h 1
/*******************************************************************************/
//#include <gtk/gtk.h>
#ifdef GLIB
    #include <glib.h>
#endif
#ifdef QT
    #include <QHash>
#endif
#include <libxml/parser.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <stdlib.h>
#include <locale.h>
#include <sys/stat.h>

/*******************************************************************************/
int source_init(void);
void source_destroy(void);
#ifdef GLIB
    void free_fields(gpointer key, gpointer val, gpointer user_data);
    gint get_station_weather_data(const gchar *station_id_with_path, GHashTable *data, gboolean get_detail_data);
#endif
#ifdef  QT
    QString hash_hko_icon_table_find(QHash<QString, QString> *hash, char *key);
#endif
//gint parse_xml_detail_data(const gchar *station_id, xmlNode *root_node, GHashTable *data);
int parse_and_write_xml_data(const char *station_id, htmlDocPtr doc, const char *result_file);

/*******************************************************************************/
#endif
