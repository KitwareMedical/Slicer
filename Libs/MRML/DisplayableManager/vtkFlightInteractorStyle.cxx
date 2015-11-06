/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFlightInteractorStyle.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkFlightInteractorStyle.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCallbackCommand.h"
#include "vtkWindows.h"
#include "vtkMatrix4x4.h"

vtkStandardNewMacro(vtkFlightInteractorStyle);

//---------------------------------------------------------------------------
vtkFlightInteractorStyle::vtkFlightInteractorStyle()
{
  this->UseTimers    = 1;

  this->DiagonalLength            = 1.0;
  this->MotionStepSize            = 1.0/1000.0;  // +/- key adjustment
  this->MotionAccelerationFactor  = 10.0;
  this->AngleStepSize             = 1.0;
  this->AngleAccelerationFactor   = 3.0;

  this->DisableMotion      = 0;
  this->DefaultUpVector[0] = 0;
  this->DefaultUpVector[1] = 0;
  this->DefaultUpVector[2] = 1;

  this->DeltaYaw = 0.0;
  this->DeltaPitch = 0.0;
}

//---------------------------------------------------------------------------
vtkFlightInteractorStyle::~vtkFlightInteractorStyle()
{
}

//----------------------------------------------------------------------------
void vtkFlightInteractorStyle::StartForwardFly()
{
  if (this->State != VTKIS_NONE)
    {
    return;
    }
  this->StartState(VTKIS_FORWARDFLY);
}

//----------------------------------------------------------------------------
void vtkFlightInteractorStyle::StartReverseFly()
{
  if (this->State != VTKIS_NONE)
    {
    return;
    }
  this->StartState(VTKIS_REVERSEFLY);
}

//----------------------------------------------------------------------------
void vtkFlightInteractorStyle::EndFly()
{
  if (this->State == VTKIS_NONE)
    {
    return;
    }
  this->StopState();
}

//---------------------------------------------------------------------------
// All actual motion is performed in the timer
//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnTimer()
{
  if (this->State == VTKIS_NONE)
    {
    return;
    }
  if (this->CurrentRenderer == NULL)
    {
    vtkErrorMacro("OnTimer: CurrentRenderer is null");
    return;
    }
  vtkCamera* cam = this->CurrentRenderer->GetActiveCamera();
  if(!cam)
    {
    vtkErrorMacro("OnTimer: camera is null");
    return;
    }
  this->FlyByMouse(cam);
}

//---------------------------------------------------------------------------
// Mouse event handlers
//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnMouseMove()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];
  this->FindPokedRenderer(x, y);
  vtkCamera* cam = this->CurrentRenderer->GetActiveCamera();
  switch (this->State)
    {
    case VTKIS_FORWARDFLY:
    case VTKIS_REVERSEFLY:
      this->UpdateMouseSteering(cam);
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    }
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnLeftButtonDown()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];
  this->FindPokedRenderer(x, y);
  if (this->CurrentRenderer == NULL)
    {
    return;
    }

  //
  this->GrabFocus(this->EventCallbackCommand);
  switch (this->State)
    {
    case VTKIS_REVERSEFLY:
      this->State = VTKIS_FORWARDFLY;
      break;
    default :
      this->UpdateMouseMotion();
      this->StartForwardFly();
      break;
    }
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnLeftButtonUp()
{
  switch (this->State)
    {
    case VTKIS_FORWARDFLY:
      this->EndFly();
      break;
    default :
      break;
    }
  if ( this->Interactor )
    {
    this->ReleaseFocus();
    }
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnRightButtonDown()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];
  this->FindPokedRenderer(x, y);
  if (this->CurrentRenderer == NULL)
    {
    return;
    }

  //
  this->GrabFocus(this->EventCallbackCommand);
  switch (this->State)
    {
    case VTKIS_FORWARDFLY:
      this->State = VTKIS_REVERSEFLY;
      break;
    default :
      this->UpdateMouseMotion();
      this->StartReverseFly();
      break;
    }
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnRightButtonUp()
{
  switch (this->State)
    {
    case VTKIS_REVERSEFLY:
      this->EndFly();
      break;
    default :
      break;
    }
  if ( this->Interactor )
    {
    this->ReleaseFocus();
    }
}

//---------------------------------------------------------------------------
// Keyboard event handlers
//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnKeyPress()
{
  // Camera
  if (this->CurrentRenderer == NULL)
    {
    vtkErrorMacro("OnKeyPress: CurrentRenderer is null");
    return;
    }
  vtkCamera* cam = this->CurrentRenderer->GetActiveCamera();
  if(!cam)
    {
    vtkErrorMacro("OnKeyPress: camera is null");
    return;
    }

  // Key pressed
  char* keySym = this->Interactor->GetKeySym();

  // Speeds
  double a_vector[3];
  double speed  = this->DiagonalLength * this->MotionStepSize;
  speed *= ( this->Interactor->GetShiftKey() ? this->MotionAccelerationFactor : 1.0);
  if (this->DisableMotion)
    {
    speed = 0;
    }
  double aspeed = this->AngleStepSize;
  aspeed *= ( this->Interactor->GetShiftKey() ? this->MotionAccelerationFactor : 1.0);

  // Yaw
  if (!strcmp(keySym, "Left"))
    {
    cam->Yaw( aspeed);
    }
  if (!strcmp(keySym, "Right"))
    {
    cam->Yaw(-aspeed);
    }
  // Pitch
  if (!strcmp(keySym, "Up"))
    {
    cam->Pitch( aspeed);
    cam->OrthogonalizeViewUp();
    }
  if (!strcmp(keySym, "Down"))
    {
    cam->Pitch(-aspeed);
    cam->OrthogonalizeViewUp();
    }
  // Roll
  if (!strcmp(keySym, "Prior"))
    {
    cam->Roll(-aspeed);
    }
  if (!strcmp(keySym, "Next"))
    {
    cam->Roll( aspeed);
    }
  // Forward / Backward
  cam->GetDirectionOfProjection(a_vector);
  if (!strcmp(keySym, "A") || !strcmp(keySym, "a")
      || !strcmp(keySym, "I") || !strcmp(keySym, "i"))
    {
    this->MotionAlongVector(a_vector,-speed, cam);
    }
  if (!strcmp(keySym, "Z") || !strcmp(keySym, "z")
      || !strcmp(keySym, "K") || !strcmp(keySym, "k"))
    {
    this->MotionAlongVector(a_vector, speed, cam);
    }
  // Left / Right
  this->GetLRVector(a_vector, cam);
  if (!strcmp(keySym, "J") || !strcmp(keySym, "j"))
    {
    this->MotionAlongVector(a_vector, speed, cam);
    }
  if (!strcmp(keySym, "L") || !strcmp(keySym, "l"))
    {
    this->MotionAlongVector(a_vector,-speed, cam);
    }
  // Up / Down
  cam->GetViewUp(a_vector);
  if (!strcmp(keySym, "U") || !strcmp(keySym, "u"))
    {
    this->MotionAlongVector(a_vector,-speed, cam);
    }
  if (!strcmp(keySym, "O") || !strcmp(keySym, "o"))
    {
    this->MotionAlongVector(a_vector, speed, cam);
    }

  // Finish
  this->AdjustRenderer();
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::OnChar()
{
  switch (this->Interactor->GetKeyCode())
    {
    case '+' :
      this->MotionStepSize *= 2.0;
      break;
    case '-' :
      this->MotionStepSize *= 0.5;
      break;
    default:
      this->Superclass::OnChar();
      break;
    }
}

//---------------------------------------------------------------------------
// Perform the motion
//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::FlyByMouse(vtkCamera* cam)
{
  // Speeds
  double a_vector[3];
  double speed  = this->DiagonalLength * this->MotionStepSize;
  speed = speed * ( this->Interactor->GetShiftKey() ? this->MotionAccelerationFactor : 1.0);
  if (this->DisableMotion)
    {
    speed = 0;
    }
  double aspeed = this->AngleStepSize*(this->Interactor->GetShiftKey() ?
      this->AngleAccelerationFactor : 1.0);


  if (!this->Interactor->GetControlKey())
    {
    // Steering
    cam->Yaw(this->DeltaYaw);
    cam->Pitch(this->DeltaPitch);
    this->DeltaYaw = 0;
    this->DeltaPitch = 0;

    // Translation
    cam->GetDirectionOfProjection(a_vector);
    switch (this->State)
      {
      case VTKIS_FORWARDFLY:
        this->MotionAlongVector(a_vector, -speed, cam);
        break;
      case VTKIS_REVERSEFLY:
        this->MotionAlongVector(a_vector, speed, cam);
        break;
      }

    cam->OrthogonalizeViewUp();

    // Restore UpVector
    //this->RestoreUpVector(cam);
    }
  else
    {
    // Roll
    switch (this->State)
      {
      case VTKIS_FORWARDFLY:
        cam->Roll(-aspeed);
        break;
      case VTKIS_REVERSEFLY:
        cam->Roll( aspeed);
        break;
      }
    }

  // Re-adjust camera
  this->AdjustRenderer();
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::UpdateMouseSteering(vtkCamera* cam)
{
  int *thispos = this->Interactor->GetEventPosition();
  int *lastpos = this->Interactor->GetLastEventPosition();

  double aspeed = this->AngleStepSize*(this->Interactor->GetShiftKey() ?
      this->AngleAccelerationFactor : 1.0);
  //
  // we want to steer by an amount proportional to window viewangle and size
  // compute dx and dy increments relative to last mouse click
  int *size = this->Interactor->GetSize();
  double scalefactor = 5*cam->GetViewAngle()/size[0];
  double dx = - (thispos[0] - lastpos[0])*scalefactor*aspeed;
  double dy =   (thispos[1] - lastpos[1])*scalefactor*aspeed;

  // Temporary until I get smooth flight working
  this->DeltaPitch = dy;
  this->DeltaYaw = dx;
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::UpdateMouseMotion()
{
  double bounds[6];
  this->CurrentRenderer->ComputeVisiblePropBounds( bounds );
  if ( !vtkMath::AreBoundsInitialized(bounds) )
    {
    this->DiagonalLength = 1.0;
    }
  else
    {
    this->DiagonalLength =
      sqrt( (bounds[0] - bounds[1]) * (bounds[0] - bounds[1]) +
            (bounds[2] - bounds[3]) * (bounds[2] - bounds[3]) +
            (bounds[4] - bounds[5]) * (bounds[4] - bounds[5]) );
    }
}

//---------------------------------------------------------------------------
// useful utility functions
//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::MotionAlongVector(double vector[3],
    double amount, vtkCamera* cam)
{
  double oldcampos[3], oldcamfoc[3];
  cam->GetPosition(oldcampos);
  cam->GetFocalPoint(oldcamfoc);
  // move camera and focus along DirectionOfProjection
  cam->SetPosition(
    oldcampos[0] - amount * vector[0],
    oldcampos[1] - amount * vector[1],
    oldcampos[2] - amount * vector[2]);
  cam->SetFocalPoint(
    oldcamfoc[0] - amount * vector[0],
    oldcamfoc[1] - amount * vector[1],
    oldcamfoc[2] - amount * vector[2]);
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::GetLRVector(double vector[3], vtkCamera* cam)
{
  vtkMatrix4x4 *vtm;
  vtm = cam->GetViewTransformMatrix();
  vector[0] = vtm->GetElement(0,0);
  vector[1] = vtm->GetElement(0,1);
  vector[2] = vtm->GetElement(0,2);
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::RestoreUpVector(vtkCamera* cam)
{
  cam->OrthogonalizeViewUp();
  double delta[3];
  cam->GetViewUp(delta);
  double weight = vtkMath::Dot(this->DefaultUpVector,delta);
  // only correct up if we're close to it already...
  if (weight>0.3)
    {
    weight = 0.25*fabs(weight);
    delta[0] = delta[0] + (this->DefaultUpVector[0]-delta[0])*weight;
    delta[1] = delta[1] + (this->DefaultUpVector[1]-delta[1])*weight;
    delta[2] = delta[2] + (this->DefaultUpVector[2]-delta[2])*weight;
    cam->SetViewUp(delta);
    }
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::AdjustRenderer()
{
  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }
  if (this->Interactor->GetLightFollowCamera())
    {
    this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }
}

//---------------------------------------------------------------------------
void vtkFlightInteractorStyle::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "MotionAccelerationFactor: "
     << this->MotionAccelerationFactor << "\n";
  os << indent << "AngleStepSize: "
     << this->AngleStepSize << "\n";
  os << indent << "AngleAccelerationFactor: "
     << this->AngleAccelerationFactor << "\n";
  os << indent << "MotionStepSize: "
     << this->MotionStepSize << "\n";
  os << indent << "DisableMotion: "
     << this->DisableMotion << "\n";
  os << indent << "DefaultUpVector: "
     << this->DefaultUpVector[0] << " "
     << this->DefaultUpVector[1] << " "
     << this->DefaultUpVector[2] << "\n";
}


