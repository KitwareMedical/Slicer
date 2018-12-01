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

#ifndef __qSlicerMarkupsFiducialsReader_h
#define __qSlicerMarkupsFiducialsReader_h

// SlicerQt includes
#include "qSlicerMarkupsReader.h"

//----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_Markups
class qSlicerMarkupsFiducialsReader
  : public qSlicerMarkupsReader
{
  Q_OBJECT
public:
  typedef qSlicerMarkupsReader Superclass;
  qSlicerMarkupsFiducialsReader(QObject* parent = 0);
  qSlicerMarkupsFiducialsReader(vtkSlicerMarkupsLogic* logic, QObject* parent = 0);
  virtual ~qSlicerMarkupsFiducialsReader();

  virtual QString description()const;
  virtual IOFileType fileType()const;
  virtual QStringList extensions()const;

protected:
  using qSlicerMarkupsReader::load;
  virtual char* load(const QString& filename, const QString& name);

private:
  Q_DISABLE_COPY(qSlicerMarkupsFiducialsReader);
};

#endif
