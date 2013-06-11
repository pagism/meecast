//* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen
 *
 * Copyright (C) 2012 - 2013 Vlad Vasilyeu
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


#ifndef CONFIGTIZEN_H
#define CONFIGTIZEN_H

#include "../core/core.h"

class ConfigTizen : public Core::Config

{
private:
    int _screen_width;
    int _screen_height;
protected:
    static ConfigTizen* _self;
    static int _refcount;
    virtual ~ConfigTizen();
    ConfigTizen();
    ConfigTizen(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd");

public:
    static ConfigTizen* Instance();
    static ConfigTizen* Instance(const std::string& filename, const std::string& schema_filename = "/usr/" + schemaPath + "config.xsd");
    void set_screen_width(int width);
    int  get_screen_width();
    void set_screen_height(int height);
    int  get_screen_height();
    void saveStation1(String city_id, String city_name, String region,
                      String country, String source, String source_id, bool gps=false, double latitude = 0.0, double longitude = 0.0);

};

#endif // CONFIGTIZEN_H
