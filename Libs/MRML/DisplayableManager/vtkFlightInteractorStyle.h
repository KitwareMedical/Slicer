/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFlightInteractorStyle.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkFlightInteractorStyle - provides flight motion routines
//
// .SECTION Description
// Left  mouse button press : forward motion.
// Right mouse button press : reverse motion.
// Moving mouse during motion : steers in desired direction.
// Ctrl + Left mouse button : left Roll.
// Ctrl + Right mouse button : right Roll.
// Left/Right keys : Yaw.
// Up/Down keys : Pitch.
// Page Up/Page Down keys : Roll.
// 'A' : forward step. 'Z' : reverse step.
// Shift key : accelerator
//
// By default, one "step" of motion corresponds to 1/1000th of the diagonal
// of bounding box of visible actors, '+' and '-' keys allow user to
// increase or decrease step size.

#ifndef vtkFlightInteractorStyle_h
#define vtkFlightInteractorStyle_h

#include "vtkInteractionStyleModule.h" // For export macro
#include "vtkInteractorStyle.h"
class vtkCamera;
class vtkPerspectiveTransform;

class VTKINTERACTIONSTYLE_EXPORT vtkFlightInteractorStyle : public vtkInteractorStyle
{
public:
  static vtkFlightInteractorStyle *New();
  vtkTypeMacro(vtkFlightInteractorStyle,vtkInteractorStyle);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the basic unit step size : by default 1/1000 of bounding diagonal
  vtkSetMacro(MotionStepSize,double);
  vtkGetMacro(MotionStepSize,double);

  // Description:
  // Set acceleration factor when shift key is applied : default 10
  vtkSetMacro(MotionAccelerationFactor,double);
  vtkGetMacro(MotionAccelerationFactor,double);

  // Description:
  // Set the basic angular unit for turning : default 1 degree
  vtkSetMacro(AngleStepSize,double);
  vtkGetMacro(AngleStepSize,double);

  // Description:
  // Set angular acceleration when shift key is applied : default 5
  vtkSetMacro(AngleAccelerationFactor,double);
  vtkGetMacro(AngleAccelerationFactor,double);

  // Description:
  // Disable motion (temporarily - for viewing etc)
  vtkSetMacro(DisableMotion,int);
  vtkGetMacro(DisableMotion,int);
  vtkBooleanMacro(DisableMotion,int);

  // Specify "up" (by default {0,0,1} but can be changed)
  vtkGetVectorMacro(DefaultUpVector,double,3);
  vtkSetVectorMacro(DefaultUpVector,double,3);

  // Description:
  // Concrete implementation of event bindings for flight
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();
  virtual void OnKeyPress();
  virtual void OnChar();

  virtual void StartForwardFly();
  virtual void StartReverseFly();
  virtual void EndFly();
  virtual void OnTimer();

protected:
   vtkFlightInteractorStyle();
  ~vtkFlightInteractorStyle();

  void FlyByMouse(vtkCamera *cam);
  void UpdateMouseSteering(vtkCamera *cam);
  void UpdateMouseMotion();
  void MotionAlongVector(double vector[3], double amount, vtkCamera* cam);
  void GetLRVector(double vector[3], vtkCamera* cam);
  void RestoreUpVector(vtkCamera* cam);
  void AdjustRenderer();

  double DiagonalLength;
  double MotionStepSize;
  double MotionAccelerationFactor;
  double AngleStepSize;
  double AngleAccelerationFactor;
  int DisableMotion;
  double DefaultUpVector[3];
  double DeltaYaw;
  double DeltaPitch;

};

#endif
