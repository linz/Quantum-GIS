/***************************************************************************
                          qgsworldmapimporter.cpp 
 Import tool for various worldmap analysis output files
 Functions:
   
                             -------------------
    begin                : Jan 21, 2004
    copyright            : (C) 2004 by Tim Sutton
    email                : tim@linfiniti.com
  
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 /*  $Id$ */

// includes

#include "../../src/qgisapp.h"
#include "../../src/qgsmaplayer.h"
#include "../../src/qgsrasterlayer.h"
#include "qgsworldmapimporter.h"


#include <qtoolbar.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qlineedit.h>
#include <qaction.h>
#include <qapplication.h>
#include <qcursor.h>

//non qt includes
#include <iostream>
#include <qgsworldmapimportergui.h>

// xpm for creating the toolbar icon
 #include "icon_wmi.xpm"
// 
static const char *pluginVersion = "0.1";
/**
* Constructor for the plugin. The plugin is passed a pointer to the main app
* and an interface object that provides access to exposed functions in QGIS.
* @param qgis Pointer to the QGIS main window
* @param _qI Pointer to the QGIS interface object
*/
QgsWorldMapImporter::QgsWorldMapImporter(QgisApp * theQGisApp, QgisIface * theQgisInterFace):
                                qgisMainWindowPointer(theQGisApp), qGisInterface(theQgisInterFace)
{
  /** Initialize the plugin and set the required attributes */
    pluginNameQString = "WorldMap Import";
    pluginVersionQString = "Version 0.1";
    pluginDescriptionQString = "Importer for WorldMap model output files.";

}

QgsWorldMapImporter::~QgsWorldMapImporter()
{

}

/* Following functions return name, description, version, and type for the plugin */
QString QgsWorldMapImporter::name()
{
    return pluginNameQString;
}

QString QgsWorldMapImporter::version()
{
    return pluginVersionQString;

}

QString QgsWorldMapImporter::description()
{
    return pluginDescriptionQString;

}

int QgsWorldMapImporter::type()
{
    return QgisPlugin::UI;
}

/*
* Initialize the GUI interface for the plugin 
*/
void QgsWorldMapImporter::initGui()
{
    // add a menu with 2 items
    QPopupMenu *pluginMenu = new QPopupMenu(qgisMainWindowPointer);

    pluginMenu->insertItem(QIconSet(icon_wmi),"&WorldMap Results Importer", this, SLOT(run()));

    menuBarPointer = ((QMainWindow *) qgisMainWindowPointer)->menuBar();

    menuIdInt = qGisInterface->addMenu("&Biodiversity", pluginMenu);
     // Create the action for tool
    QAction *myQActionPointer = new QAction("WorldMap Importer", QIconSet(icon_wmi), "&Wmi",0, this, "run");
    // Connect the action to the run
    connect(myQActionPointer, SIGNAL(activated()), this, SLOT(run()));
    // Add the toolbar
    toolBarPointer = new QToolBar((QMainWindow *) qgisMainWindowPointer, "Biodiversity");
    toolBarPointer->setLabel("WorldMap Importer");
    // Add the zoom previous tool to the toolbar
    myQActionPointer->addTo(toolBarPointer);
    

}

// Slot called when the buffer menu item is activated
void QgsWorldMapImporter::run()
{
  QgsWorldMapImporterGui *myQgsWorldMapImporterGui=new QgsWorldMapImporterGui();
  //listen for when the layer has been made so we can draw it
  connect(myQgsWorldMapImporterGui, SIGNAL(drawLayer(QString)), this, SLOT(drawLayer(QString)));
  myQgsWorldMapImporterGui->show();
}
  //!draw a raster layer in the qui - intended to respond to signal sent by diolog when it as finished creating
  //layer
  void QgsWorldMapImporter::drawLayer(QString theQString)
{
  std::cout << "DrawLayer slot called with " << theQString << std::endl;
  qGisInterface->addRasterLayer(theQString);
}
// Unload the plugin by cleaning up the GUI
void QgsWorldMapImporter::unload()
{
    // remove the GUI
    menuBarPointer->removeItem(menuIdInt);
    delete toolBarPointer;
}
/** 
* Required extern functions needed  for every plugin 
* These functions can be called prior to creating an instance
* of the plugin class
*/
// Class factory to return a new instance of the plugin class
extern "C" QgisPlugin * classFactory(QgisApp * theQGisAppPointer, QgisIface * theQgisInterfacePointer)
{
    return new QgsWorldMapImporter(theQGisAppPointer, theQgisInterfacePointer);
}

// Return the name of the plugin - note that we do not user class members as
// the class may not yet be insantiated when this method is called.
extern "C" QString name()
{
    return QString("WorldMap Importer");
}

// Return the description
extern "C" QString description()
{
    return QString("This is a plugin to convert WorldMap model outputs to Arc/Info ASCII grid files and display them in QGIS");
}

// Return the type (either UI or MapLayer plugin)
extern "C" int type()
{
    return QgisPlugin::UI;
}

// Return the version number for the plugin
extern "C" QString version()
{
  return pluginVersion;
}

// Delete ourself
extern "C" void unload(QgisPlugin * thePluginPointer)
{
    delete thePluginPointer;
}
