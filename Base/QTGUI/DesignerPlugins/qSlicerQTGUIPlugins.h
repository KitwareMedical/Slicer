/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerQTGUIPlugins_h
#define __qSlicerQTGUIPlugins_h

// QT includes
#include <QDesignerCustomWidgetCollectionInterface>

// QtGUI includes
#include "qSlicerWidgetPlugin.h"
#include "qSlicerModulePanelPlugin.h"
#include "qSlicerMouseModeToolBarPlugin.h"

// \class Group the plugins in one library
class Q_SLICER_BASE_QTGUI_PLUGIN_EXPORT qSlicerQTGUIPlugins : public QObject,
                         public QDesignerCustomWidgetCollectionInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetCollectionInterface);

public:
  QList<QDesignerCustomWidgetInterface*> customWidgets() const
    {
    QList<QDesignerCustomWidgetInterface *> plugins;
    plugins << new qSlicerWidgetPlugin;
    plugins << new qSlicerModulePanelPlugin;
    plugins << new qSlicerMouseModeToolBarPlugin;
    return plugins;
    }
};

#endif

