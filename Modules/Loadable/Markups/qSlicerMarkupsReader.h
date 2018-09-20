/*==============================================================================

  Program: 3D Slicer

  Copyright (c) BWH

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qSlicerMarkupsReader_h
#define __qSlicerMarkupsReader_h

// SlicerQt includes
#include "qSlicerFileReader.h"

#include "qSlicerMarkupsModuleExport.h"

class qSlicerMarkupsReaderPrivate;
class vtkSlicerMarkupsLogic;

//----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_Markups
class Q_SLICER_QTMODULES_MARKUPS_EXPORT qSlicerMarkupsReader
  : public qSlicerFileReader
{
  Q_OBJECT
public:
  typedef qSlicerFileReader Superclass;
  qSlicerMarkupsReader(QObject* parent = 0);
  qSlicerMarkupsReader(vtkSlicerMarkupsLogic* logic, QObject* parent = 0);
  virtual ~qSlicerMarkupsReader();

  vtkSlicerMarkupsLogic* markupsLogic()const;
  void setMarkupsLogic(vtkSlicerMarkupsLogic* logic);

  virtual QString description()const;
  virtual IOFileType fileType()const;
  virtual QStringList extensions()const;

  virtual bool load(const IOProperties& properties);

protected:
  QScopedPointer<qSlicerMarkupsReaderPrivate> d_ptr;

  virtual char* load(const QString& filename, const QString& name);

private:
  Q_DECLARE_PRIVATE(qSlicerMarkupsReader);
  Q_DISABLE_COPY(qSlicerMarkupsReader);
};

#endif
