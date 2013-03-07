/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2013 Vlad Vasilyeu
 * Copyright (C) 2010-2011 Tanya Makova
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "controller.h"
ConfigQml *
create_and_fill_config(){
    ConfigQml *config;

    std::cerr<<"Create Config class: " << std::endl;
    try{
        config = ConfigQml::Instance(Core::AbstractConfig::getConfigPath()+
                                       "config.xml",
                                       Core::AbstractConfig::prefix+
                                       Core::AbstractConfig::schemaPath+
                                       "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  ConfigQml::Instance();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        config =  ConfigQml::Instance();
    }
    //std::cerr<<"End of creating Config class: " <<std::endl;
    config->saveConfig();
    std::cerr<<"End of creating Config class" <<std::endl;

    return config;
}


Controller::Controller(const std::string& qml_filename ) : QObject()
{
//  _qview = new QDeclarativeView();
  _qview  = QmlDocument::create(qml_filename.c_str());
  _dp = NULL;
  this->active_frame = new ActiveFrame();
  _active_frame_view = this->active_frame->qml();
  this->load_config();
  this->load_data();
  active_frame->StartFrame();
  this->update();

}

Controller::~Controller()
{
  if (_dp) 
      _dp->DeleteInstance();

}

QmlDocument* 
Controller::qview()
{
    return _qview;
}

QmlDocument*
Controller::active_frame_qview()
{
    return _active_frame_view;
}

void
Controller::active_frame_qview(QmlDocument *active_frame_view){
	_active_frame_view = active_frame_view;
}

void
Controller::load_data()
{
  time_t current_day;
  time_t current_hour;
  struct tm *tm = NULL;
  struct tm time_tm1;
  struct tm time_tm2;

  DataItem *forecast_data = NULL;
  Core::Data *temp_data = NULL;
  int i = 0;
  int timezone = 0;
  int localtimezone = 0;
  
  /* std::cerr<<" Controller::load_data()"<<std::endl; */

  std::string mapfilename(Core::AbstractConfig::getCachePath());
  _dp->DeleteInstance(); 
         
  if (_config->current_station_id() != INT_MAX && _config->stationsList().size() > 0 &&
        _config->stationsList().at(_config->current_station_id())){
          try{
                _dp =  Core::DataParser::Instance(_config->stationsList().at(_config->current_station_id())->fileName(), Core::AbstractConfig::prefix+Core::AbstractConfig::schemaPath+"data.xsd");
            }
            catch(const std::string &str){
                std::cerr<<"Error in DataParser class: "<< str <<std::endl;
                _dp = Core::DataParser::Instance();
            //    return;
            }
            catch(const char *str){
                std::cerr<<"Error in DataParser class: "<< str <<std::endl;
                _dp = Core::DataParser::Instance();
           //     return;
            }
            mapfilename += _config->stationsList().at(_config->current_station_id())->sourceName().c_str();
            mapfilename += "_";
            mapfilename += _config->stationsList().at(_config->current_station_id())->id().c_str();
            mapfilename += "_map_";
            mapfilename += "%s.png";
  }

 
    
  _model = new DataModelQt(new DataItem, qApp);
  _current = new DataModelQt(new DataItem, qApp);
  _night_model = new DataModelQt(new DataItem, qApp);
  _current_night = new DataModelQt(new DataItem, qApp);
  _hours_model = new DataModelQt(new DataItem, qApp);

  if (_dp){
    fprintf (stderr, "timezone!!!!! %i\n",_dp->timezone());
    timezone = _dp->timezone();
  }

  /* Set localtimezone */

  current_day = time(NULL);
  gmtime_r(&current_day, &time_tm1);
  localtime_r(&current_day, &time_tm2);
  localtimezone = (mktime(&time_tm2) - mktime(&time_tm1))/3600; 
  /* set current day */ 
  current_day = time(NULL);
  fprintf(stderr,"Current time %i\n", current_day);

  //tm = localtime(&current_day);
  tm = gmtime(&current_day);
  current_day = mktime(tm)  ; /* today 00:00:00 */
  tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
//  tm->tm_isdst = 1;
  tm->tm_isdst = 0;
//  current_day = mktime(tm) - 3600*timezone ; /* today 00:00:00 */
  current_day = mktime(tm) - 3600*timezone + 3600*localtimezone ; /* today 00:00:00 */
      fprintf(stderr,"Current day %li\n", current_day);
  /* fill current day */
  if  (_dp != NULL && (temp_data = _dp->data().GetDataForTime(time(NULL)))) {
      forecast_data = new DataItem(temp_data);

      if (forecast_data->Text() != "")
        forecast_data->Text(_(forecast_data->Text().c_str()));
      if (_dp->data().GetSunRiseForTime(current_day + 15 * 3600) > 0)
        forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 15 * 3600) + 3600*timezone -3600*localtimezone);
      if (_dp->data().GetSunSetForTime(current_day + 15 * 3600) >0)
        forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 15 * 3600) + 3600*timezone - 3600*localtimezone);
      forecast_data->LastUpdate(_dp->LastUpdate());
      forecast_data->temperatureunit = _config->temperatureunit();
      forecast_data->windunit = _config->windspeedunit();
      forecast_data->pressureunit = _config->pressureunit();
      forecast_data->visibleunit = _config->visibleunit();

      /* Add map */
      forecast_data->MapPattern(mapfilename);

      _current->appendRow(forecast_data);
      /* Preparing time for updateing */
      uint result_time = 0;
      if (_config->UpdatePeriod() != INT_MAX){
          if ((time(NULL) - _dp->LastUpdate()) > _config->UpdatePeriod())
              result_time = time(NULL) + 10;
          else
              if (_dp->LastUpdate() + _config->UpdatePeriod() < temp_data->EndTime())
                 result_time = _dp->LastUpdate() + _config->UpdatePeriod();  
              else
                 result_time = temp_data->EndTime();
      }else
          result_time = temp_data->EndTime();

      QDateTime t;
      t.setTime_t(_dp->LastUpdate());
      QString description = forecast_data->Text().c_str();

  }

  /* fill current night */
  if  (_dp != NULL && (temp_data = _dp->data().GetDataForTime(current_day + 3*3600))) {
      forecast_data = new DataItem(temp_data);

      if (forecast_data->Text() != "")
        forecast_data->Text(_(forecast_data->Text().c_str()));

      if (_dp->data().GetSunRiseForTime(current_day + 3 * 3600) >0)
        forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 3 * 3600) + 3600*timezone - 3600*localtimezone);
      if (_dp->data().GetSunSetForTime(current_day + 3 * 3600) >0)
        forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 3 * 3600) + 3600*timezone - 3600*localtimezone);
      forecast_data->LastUpdate(_dp->LastUpdate());
      forecast_data->temperatureunit = _config->temperatureunit();
      forecast_data->windunit = _config->windspeedunit();
      forecast_data->pressureunit = _config->pressureunit();
      forecast_data->visibleunit = _config->visibleunit();
      /* Add map */
      forecast_data->MapPattern(mapfilename);

      _current_night->appendRow(forecast_data);
      /*MeecastIf* dbusclient = new MeecastIf("com.meecast.applet", "/com/meecast/applet", QDBusConnection::sessionBus(), 0);
      dbusclient->SetCurrentData( _config->stationname(), forecast_data->temperature(),
                                  forecast_data->temperature_high(), forecast_data->temperature_low(),
                                  (_config->iconspath() + "/" + _config->iconset() + "/" + forecast_data->icon()), forecast_data->Data::EndTime(), forecast_data->current());
      */
  }

  /* set next day */
  //i = 3600*24;
  i = 1; /* plus 1 second  */ 
  /* fill other days */
  while  (_dp != NULL && ((temp_data = _dp->data().GetDataForTime(current_day + 15 * 3600  + i )) || (i < 7*3600*24))) {
      if (temp_data){
          forecast_data = new DataItem(temp_data);

          if (forecast_data->Text() != "")
            forecast_data->Text(_(forecast_data->Text().c_str()));

          if (_dp->data().GetSunRiseForTime(current_day + 15 * 3600 + i) > 0)
            forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 15 * 3600  + i) + 3600*timezone - 3600*localtimezone);
          if (_dp->data().GetSunSetForTime(current_day + 15 * 3600 + i) > 0)
            forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 15 * 3600  + i) + 3600*timezone - 3600*localtimezone);
          forecast_data->LastUpdate(_dp->LastUpdate());
          forecast_data->temperatureunit = _config->temperatureunit();
          forecast_data->windunit = _config->windspeedunit();
          forecast_data->pressureunit = _config->pressureunit();
          forecast_data->visibleunit = _config->visibleunit();
          if (i == 0)
            forecast_data->MapPattern(mapfilename);
          else
            forecast_data->MapPattern(std::string(""));
          _model->appendRow(forecast_data);
      }
      i = i + 3600*24;
  }
  /* set next night */
//  i = 3600*24;
  i = 1;
  /* fill other nights */
  while  (_dp != NULL && ((temp_data = _dp->data().GetDataForTime( current_day + 3 * 3600  + i)) || (i < 7*3600*24))) {
      if (temp_data){
          forecast_data = new DataItem(temp_data);

          if (forecast_data->Text() != "")
            forecast_data->Text(_(forecast_data->Text().c_str()));

          if (_dp->data().GetSunRiseForTime(current_day + 3 * 3600 + i) > 0)
            forecast_data->SunRiseTime(_dp->data().GetSunRiseForTime(current_day + 3 * 3600  + i) + 3600*timezone - 3600*localtimezone);
          if (_dp->data().GetSunSetForTime(current_day + 3 * 3600 + i) > 0)
            forecast_data->SunSetTime(_dp->data().GetSunSetForTime(current_day + 3 * 3600  + i) + 3600*timezone - 3600*localtimezone);
          forecast_data->LastUpdate(_dp->LastUpdate());
          forecast_data->temperatureunit = _config->temperatureunit();
          forecast_data->windunit = _config->windspeedunit();
          forecast_data->pressureunit = _config->pressureunit();
          forecast_data->visibleunit = _config->visibleunit();
          if (i == 0)
            forecast_data->MapPattern(mapfilename);
          else
            forecast_data->MapPattern("");
          _night_model->appendRow(forecast_data);
      }
      i = i + 3600*24;
  }
  /* set current hour */
  current_hour = time(NULL);
  tm = gmtime(&current_hour);
  tm->tm_sec = 0; tm->tm_min = 1; 
  tm->tm_isdst = 1;
  current_hour = mktime(tm); 
  i =0;
  
  /* fill hours */
  while  (_dp != NULL && i<5*24*3600) {
    if (temp_data = _dp->data().GetDataForTime(current_hour + i, true)){
        if (temp_data->StartTime() + 60 == current_hour + i){
            forecast_data = new DataItem(temp_data);
            if (forecast_data->Text() != "")
                forecast_data->Text(_(forecast_data->Text().c_str()));
            forecast_data->temperatureunit = _config->temperatureunit();
            forecast_data->windunit = _config->windspeedunit();
            forecast_data->pressureunit = _config->pressureunit();
            _hours_model->appendRow(forecast_data);
        }
    }
    i = i + 3600;
  }

  _qview->setContextProperty("Current", _current);
  _qview->setContextProperty("Current_night", _current_night);
  _qview->setContextProperty("Forecast_model", _model);
  _qview->setContextProperty("Forecast_night_model", _night_model);
  _qview->setContextProperty("Forecast_hours_model", _hours_model);

  _active_frame_view->setContextProperty("Current", _current);


  /* models for station selection */
  SelectModel* source_model = new SelectModel(qApp);
  CountryModel* country_model = new CountryModel(qApp);
  RegionModel* region_model = new RegionModel(qApp);
  CityModel* city_model = new CityModel(qApp);
  //country_model->populate("gismeteo.ru");
  QStringList sources = _config->Sources();
  for (int j=0; j<sources.size(); j++){
      QString str = sources.at(j);
      source_model->addData(new SelectData(str, "", str.left(1)));
      //qDebug() << countries.at(j) << str.left(1);
  }
  _qview->setContextProperty("source_model", source_model);
  _qview->setContextProperty("country_model", country_model);
  _qview->setContextProperty("region_model", region_model);
  _qview->setContextProperty("city_model", city_model);
  
}

void
Controller::load_config()
{
   std::cout<<"Load"<<std::endl;
  _config = create_and_fill_config();   
  _qview->setContextProperty("Config", _config);
  _active_frame_view->setContextProperty("Config", _config);
  _active_frame_view->setContextProperty("ActiveFrame", active_frame);

}
void
Controller::reload_config()
{
  std::cout<<"Reload";
  _config->ReLoadConfig();
  delete _model;
  this->load_data();
}

ConfigQml*
Controller::config()
{
   return _config;
}

void
Controller::update() {
    QTimer::singleShot(3600*1000, this, SLOT(update()));
 	_config->refreshconfig();
}
