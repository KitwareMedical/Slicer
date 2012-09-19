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

#ifndef __qMRMLMatrixWidget_h
#define __qMRMLMatrixWidget_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKObject.h>
#include <ctkVTKAbstractMatrixWidget.h>

#include "qMRMLWidgetsExport.h"
 
class vtkMRMLNode; 
class vtkMRMLLinearTransformNode; 
class vtkMatrix4x4;
class qMRMLMatrixWidgetPrivate;

class QMRML_WIDGETS_EXPORT qMRMLMatrixWidget : public ctkVTKAbstractMatrixWidget
{
  Q_OBJECT
  QVTK_OBJECT
  Q_PROPERTY(CoordinateReferenceType coordinateReference READ coordinateReference WRITE setCoordinateReference)
  Q_ENUMS(CoordinateReferenceType)
  
public:
  
  /// Constructors
  typedef ctkVTKAbstractMatrixWidget Superclass;
  explicit qMRMLMatrixWidget(QWidget* parent);
  virtual ~qMRMLMatrixWidget();
  
  /// 
  /// Set/Get Coordinate system
  /// By default, the selector coordinate system will be set to GLOBAL
  enum CoordinateReferenceType { GLOBAL, LOCAL };
  void setCoordinateReference(CoordinateReferenceType coordinateReference);
  CoordinateReferenceType coordinateReference() const;

  vtkMRMLLinearTransformNode* mrmlTransformNode()const;

public slots:
  /// 
  /// Set the MRML node of interest
  void setMRMLTransformNode(vtkMRMLLinearTransformNode* transformNode); 
  void setMRMLTransformNode(vtkMRMLNode* node); 
  
protected slots:
  /// 
  /// Triggered upon MRML transform node updates
  void updateMatrix();

  ///
  /// Triggered when the user modifies the cells of the matrix.
  /// Synchronize with the node.
  void updateTransformNode();

protected:
  QScopedPointer<qMRMLMatrixWidgetPrivate> d_ptr;

  ///
  /// Fill the 'minmax' array with the min/max translation value of the matrix.
  /// Parameter expand allows to specify (using a value between 0 and 1)
  /// which percentage of the found min/max value should be substracted/added
  /// to the min/max value found.
  static QPair<double, double> extractMinMaxTranslationValue(vtkMatrix4x4 * mat,
                                                      double pad = 0);


private:
  Q_DECLARE_PRIVATE(qMRMLMatrixWidget);
  Q_DISABLE_COPY(qMRMLMatrixWidget);
}; 

#endif
