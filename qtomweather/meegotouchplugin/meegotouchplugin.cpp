#include <MButton>
#include <MContainer>
#include "meegotouchplugin.h"
#include <MLibrary>
#include <QDeclarativeComponent>
#include <QDeclarativeView>


M_LIBRARY
Q_EXPORT_PLUGIN2(weatherextension, WeatherApplicationExtension)

WeatherApplicationExtension::WeatherApplicationExtension() : box(0)
{
}

WeatherApplicationExtension::~WeatherApplicationExtension()
{
    delete box;
}

void WeatherApplicationExtension::weatherExtensionSpecificOperation()
{
    // do something specific to the demo extension interface
}

bool WeatherApplicationExtension::initialize(const QString &)
{
   QGraphicsObject* mWidget;

   QDeclarativeView* view = new QDeclarativeView();
   view->setSource(QUrl::fromLocalFile("/opt/com.meecast.omweather/share/omweather/qml/applet.qml"));
   box = new MWidget();
   mWidget = qobject_cast<QGraphicsObject*>(view->rootObject());
   mlWidget->setParentItem(box);

   return true;
}

MWidget *WeatherApplicationExtension::widget()
{
    return box;
}

