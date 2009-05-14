/***************************************************************************
    qgsshortcutsmanager.cpp
    ---------------------
    begin                : May 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsshortcutsmanager.h"

#include <QSettings>

QgsShortcutsManager::QgsShortcutsManager()
{
}

QgsShortcutsManager* QgsShortcutsManager::mInstance = NULL;

QgsShortcutsManager* QgsShortcutsManager::instance()
{
  if (!mInstance)
    mInstance = new QgsShortcutsManager;
  return mInstance;
}

bool QgsShortcutsManager::registerAction( QAction* action, QString defaultShortcut )
{
  mActions.insert(action, defaultShortcut);

  QString actionText = action->text();
  actionText.remove('&'); // remove the accelerator

  // load overridden value from settings
  QSettings settings;
  QString shortcut = settings.value("/shortcuts/"+actionText, defaultShortcut).toString();

  if ( !shortcut.isEmpty() )
    action->setShortcut( shortcut );

  return true;
}

bool QgsShortcutsManager::unregisterAction( QAction* action )
{
  mActions.remove( action );
  return true;
}

QList<QAction*> QgsShortcutsManager::listActions()
{
  return mActions.keys();
}

QString QgsShortcutsManager::actionDefaultShortcut( QAction* action )
{
  if ( !mActions.contains( action ) )
    return QString();

  return mActions.value( action );
}

bool QgsShortcutsManager::setActionShortcut( QAction* action, QString shortcut )
{
  action->setShortcut( shortcut );

  QString actionText = action->text();
  actionText.remove('&'); // remove the accelerator

  // save to settings
  QSettings settings;
  settings.setValue("/shortcuts/"+actionText, shortcut );
  return true;
}

QAction* QgsShortcutsManager::actionForShortcut( QKeySequence s )
{
  if ( s.isEmpty() )
    return NULL;

  for (ActionsHash::iterator it = mActions.begin(); it != mActions.end(); ++it)
  {
    if (it.key()->shortcut() == s)
      return it.key();
  }

  return NULL;
}