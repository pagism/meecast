# 
# Do not Edit! Generated by:
# spectacle version 0.18
# 
# >> macros
%define wantmeegopanel 0
%define all_x86 i386 i586 i686 %{ix86}
%define all_arm %{arm}
# << macros
 
Name:       com.meecast
Summary:    Weather for Meego
Version:    0.4.1
Release:    1
Group:      Utility
License:    GPLv2.1
URL:        https://garage.maemo.org/projects/omweather/
Source0:    %{name}-%{version}.tar.bz2
#Temporary
#Requires:       libmeegotouch-devel
BuildRequires:  pkgconfig(QtCore) >= 4.7.0
BuildRequires:  pkgconfig(libcurl)
BuildRequires:  pkgconfig(sqlite3)
BuildRequires:  pkgconfig(gconf-2.0)
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(libxml-2.0)
%if %{wantmeegopanel}
BuildRequires:  pkgconfig(meego-panel)
BuildRequires:  pkgconfig(mutter-plugins)
%endif
BuildRequires:  gettext
BuildRequires:  qt-qmake
BuildRequires:  libqt-devel
#BuildRequires:  libmeegotouch
BuildRequires:  libmeegotouch-devel
BuildRequires:  desktop-file-utils


%description
Weather Forecast on Meego.



%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
#export PATH=/usr/lib/qt4/bin:$PATH
%if %{wantmeegopanel}
qmake PREFIX=%{_prefix} CONFIG+=meegopanel CONFIG+=UXpanel -r
%else
qmake PREFIX=%{_prefix} CONFIG+=UXpanel -r
%endif
make
# << build pre



# >> build post
# << build post
%install
rm -rf %{buildroot}
# >> install pre
# << install pre

# >> install post
make INSTALL_ROOT=%{buildroot} install
%if %{wantmeegopanel}
ln -s /opt/com.meecast.omweather/share/icons  %{buildroot}/opt/com.meecast.omweather/share/meego-panel-omweather/theme/icons
%endif
rm %{buildroot}/opt/com.meecast.omweather/lib/libomweather-core.a
# << install post
#desktop-file-install --delete-original       \
#  --dir %{buildroot}%{_datadir}/applications             \
#   %{buildroot}%{_datadir}/applications/*.desktop



%post
# >> post
/sbin/ldconfig
# << post

%postun
# >> postun
/sbin/ldconfig
# << postun


%files
%defattr(-,root,root,-)
/opt/com.meecast.omweather
/opt/com.meecast.omweather/bin
/opt/com.meecast.omweather/bin/omweather-settings
#%if %{wantmeegopanel}
#/opt/com.meecast.omweather/bin/omweather-qml
#/opt/com.meecast.omweather/bin/omweather-settouch
#%endif
/opt/com.meecast.omweather/share
/opt/com.meecast.omweather/lib
#/usr/share/locale
#/usr/lib/omweather/weathercom
#/usr/share/omweather/copyright_icons/weather.com.png
#/usr/share/omweather/db/weather.com.db
#/usr/share/omweather/sources/weather.com.xml
# >> files
/usr/lib/meegotouch/applicationextensions/libevents-meecast.so
%if  %{wantmeegopanel}
/usr/share/omweather-settings
/opt/com.meecast.omweather/libexec
/opt/com.meecast.omweather/libexec/meego-panel-omweather
#/usr/share/meego-panel-omweather
/usr/share/mutter-netbook/panels/meego-panel-omweather.desktop
/etc/xdg
/usr/share/dbus-1/services
%{_datadir}/applications/*.desktop
%else 
%{_datadir}/applications/*.desktop
#/usr/share/pixmaps
%endif
/usr/share
%changelog
* Fri Aug 05 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.24
  * began support MeeGo 1.2.0.90 on Tablet UX
* Mon Aug 01 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.23
  * began support Tabblet UX
* Thu Jul 26 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.22
  * fixed desktop file
* Sun Jul 03 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.21
  * Adapted for Meego 1.2
  * Fixed probem with night information
  * Fixed current view
  * Fixed segmentation fault in application mode
* Thu Apr 14 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.20
  * Fixed error with autostart
  * Fixed small memory leak
* Thu Apr 06 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.19
  * Adapted to Intel AppUP
* Thu Apr 04 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.18
  * Adapted to Intel AppUP
* Thu Mar 26 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.17
  * Added automatic updating intervals
  * Added automatically updating on connection
  * Added iconset Grazank's
  * Fixed temperature units
  * Added wind speed units
* Thu Mar 22 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.16
  * Fixed problem in iconstes switching
* Thu Mar 22 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.15
  * Fixed problem with button icon "refresh" for qml
  * Associeted icon 49 as icon "na"
  * Fixed problem with destroing of clutter objects
* Mon Mar 21 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.14
  * Added switching between iconstes to settings 
  * Disabled animation
* Sun Mar 20 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.13
  * New design by Wazd (Andrew Zhilin) for Meego-panel
  * fixed problem with libtouch setting application
  * Added new iconset MeCast
* Thu Mar 12 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.12
  * changed signals in qml
  * cosmetic changes in qt touch
  * switched to new settings(qml) and renamed qml settings bin file
  * fixed segmantation fault in meego-mpl
* Thu Mar 10 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.11
  * Began adding temeperature day
  * Added new source of weather forecast gismeteo.ru
  * Redesigned qml window
  * Began added MeegoTouch setting  
* Sat Mar 5 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.10
  * Fixed spelling mistake
  * Changed default icon to N/A in meego panel
* Sat Feb 26 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.9
  * Fixed problem with Temperature unit
  * Improvements in config and QML window
* Mon Feb 21 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.8
  * Fixed problem with time for forecast.
  * Fixed QML forms
  * Added Night box for detail window
* Fri Feb 19 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.7
  * Added transaltion for wind and forecast's description
* Fri Feb 19 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.6
  * Added Russian translate to omweather-settings
* Fri Feb 18 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.5
  * Added animation for meego panel
* Wed Feb 16 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.4
  * Added icon to desktop. Added translation to QML.
  * Added button 'close' to qml
* Tue Feb 15 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.3 
  * Updated QML version of Omweather
* Mon Feb 14 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.2
  * Updated QML version of Omweather
* Sun Feb 13 2011  Vlad Vasilyeu <vasvlad@gmail.com> 0.3.1
  * Added QML version of Omweather
* Wed Feb 9 2011 Vlad Vasilyeu <vasvlad@gmail.com> 0.3
  * Initial version
# << files


