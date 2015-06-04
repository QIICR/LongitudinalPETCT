/*=auto=========================================================================

Portions (c) Copyright 2005 Brigham and Women\"s Hospital (BWH) All Rights Reserved.

See COPYRIGHT.txt
or http://www.slicer.org/copyright/copyright.txt for details.

Program:   3D Slicer
Module:    $RCSfile: vtkMRMLReprtingStudyNode.cxx,v $
Date:      $Date: 2006/03/17 15:10:10 $
Version:   $Revision: 1.2 $

=========================================================================auto=*/

// VTK includes
#include <vtkCommand.h>
#include <vtkObjectFactory.h>



// MRML includes
#include "vtkMRMLLongitudinalPETCTStudyNode.h"

#include <vtkMRMLLinearTransformNode.h>
#include <vtkMRMLAnnotationROINode.h>
#include <vtkMRMLVolumePropertyNode.h>
#include <vtkMRMLVolumeRenderingDisplayNode.h>

#include <vtkNew.h>
#include <vtkMatrix4x4.h>
#include <vtkCollection.h>



// STD includes


//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLLongitudinalPETCTStudyNode);

//----------------------------------------------------------------------------
vtkMRMLLongitudinalPETCTStudyNode::vtkMRMLLongitudinalPETCTStudyNode()
{
  this->SetHideFromEditors(true);
  this->SelectedForSegmentation = false;
  this->SelectedForAnalysis = true;

  this->PETVolumeNode = NULL;
  this->PETVolumeNodeID = NULL;

  this->CTVolumeNode = NULL;
  this->CTVolumeNodeID = NULL;

  this->PETLabelVolumeNode = NULL;
  this->PETLabelVolumeNodeID = NULL;

  this->RegistrationTransformNode = NULL;
  this->RegistrationTransformNodeID = NULL;

  this->VolumeRenderingDisplayNode = NULL;
  this->VolumeRenderingDisplayNodeID = NULL;
}


//----------------------------------------------------------------------------
vtkMRMLLongitudinalPETCTStudyNode::~vtkMRMLLongitudinalPETCTStudyNode()
{

  this->SetAndObservePETVolumeNodeID(NULL);
  this->SetAndObserveCTVolumeNodeID(NULL);
  this->SetAndObservePETLabelVolumeNodeID(NULL);
  this->SetAndObserveVolumeRenderingDisplayNodeID(NULL);
  this->SetAndObserveRegistrationTransformNodeID(NULL);

  if(this->PETVolumeNodeID)
    delete [] this->PETVolumeNodeID;

  if(this->CTVolumeNodeID)
    delete [] this->CTVolumeNodeID;

  if(this->PETLabelVolumeNodeID)
    delete [] this->PETLabelVolumeNodeID;

  if(this->RegistrationTransformNodeID)
    delete [] this->RegistrationTransformNodeID;

  if(this->VolumeRenderingDisplayNodeID)
    delete [] this->VolumeRenderingDisplayNodeID;
}

//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::ReadXMLAttributes(const char** atts)
{
  int disabledModify = this->StartModify();

  Superclass::ReadXMLAttributes(atts);

  const char* attName;
  const char* attValue;

  while (*atts != NULL)
    {
      attName = *(atts++);
      attValue = *(atts++);

      if (!strcmp(attName, "SelectedForSegmentation"))
        {
          if (!strcmp(attValue, "true"))
            this->SetSelectedForSegmentation(true);
          else
            this->SetSelectedForSegmentation(false);
        }
      else if (!strcmp(attName, "SelectedForAnalysis"))
        {
          if (!strcmp(attValue, "true"))
            this->SetSelectedForAnalysis(true);
          else
            this->SetSelectedForAnalysis(false);
        }
      else if (!strcmp(attName, "PETVolumeNodeID"))
        {
          this->SetAndObservePETVolumeNodeID(attValue);
        }
      else if (!strcmp(attName, "CTVolumeNodeID"))
        {
          this->SetAndObserveCTVolumeNodeID(attValue);
        }
      else if (!strcmp(attName, "PETLabelVolumeNodeID"))
        {
          this->SetAndObservePETLabelVolumeNodeID(attValue);
        }
      else if (!strcmp(attName, "RegistrationTransformNodeID"))
        {
          this->SetAndObserveRegistrationTransformNodeID(attValue);
        }
      else if (!strcmp(attName, "VolumeRenderingDisplayNodeID"))
        {
          this->SetAndObserveVolumeRenderingDisplayNodeID(attValue);
        }
    }

  this->EndModify(disabledModify);
}

//----------------------------------------------------------------------------
void vtkMRMLLongitudinalPETCTStudyNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);

  vtkIndent indent(nIndent);

  of << indent << " SelectedForSegmentation=\"" << (this->SelectedForSegmentation ? "true" : "false") << "\"";
  of << indent << " SelectedForAnalysis=\"" << (this->SelectedForAnalysis ? "true" : "false") << "\"";

  if (this->RegistrationTransformNodeID)
      of << indent << " RegistrationTransformNodeID=\"" << this->RegistrationTransformNodeID << "\"";
  if (this->PETVolumeNodeID)
      of << indent << " PETVolumeNodeID=\"" << this->PETVolumeNodeID << "\"";
  if (this->CTVolumeNodeID)
      of << indent << " CTVolumeNodeID=\"" << this->CTVolumeNodeID << "\"";
  if (this->PETLabelVolumeNodeID)
      of << indent << " PETLabelVolumeNodeID=\"" << this->PETLabelVolumeNodeID << "\"";
  if (this->VolumeRenderingDisplayNodeID)
      of << indent << " VolumeRenderingDisplayNodeID=\"" << this->VolumeRenderingDisplayNodeID << "\"" << std::endl;
}

//----------------------------------------------------------------------------
// Copy the node\"s attributes to this object.
// Does NOT copy: ID
void vtkMRMLLongitudinalPETCTStudyNode::Copy(vtkMRMLNode *anode)
{
  int disabledModify = this->StartModify();
  
  Superclass::Copy(anode);

  vtkMRMLLongitudinalPETCTStudyNode* node = vtkMRMLLongitudinalPETCTStudyNode::SafeDownCast(anode);
    if (node)
      {
        this->SetSelectedForSegmentation(node->GetSelectedForSegmentation());
        this->SetSelectedForAnalysis(node->GetSelectedForAnalysis());

        this->SetAndObservePETVolumeNodeID(node->GetPETVolumeNodeID());
        this->SetAndObserveCTVolumeNodeID(node->GetCTVolumeNodeID());
        this->SetAndObservePETLabelVolumeNodeID(node->GetPETLabelVolumeNodeID());
        this->SetAndObserveRegistrationTransformNodeID(node->GetRegistrationTransformNodeID());
        this->SetAndObserveVolumeRenderingDisplayNodeID(node->GetVolumeRenderingDisplayNodeID());
      }

  this->EndModify(disabledModify);
}

//----------------------------------------------------------------------------
void vtkMRMLLongitudinalPETCTStudyNode::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Selected for Segmentation: " << this->SelectedForSegmentation << "\n";
  os << indent << "Selected for Analysis: " << this->SelectedForAnalysis << "\n";

  os << indent << "PETVolumeNodeID: " << (this->PETVolumeNodeID ? this->PETVolumeNodeID : "(none)") << "\n";
  os << indent << "CTVolumeNodeID: " << (this->CTVolumeNodeID ? this->CTVolumeNodeID : "(none)") << "\n";
  os << indent << "PETLabelVolumeNodeID: " << (this->PETLabelVolumeNodeID ? this->PETLabelVolumeNodeID : "(none)") << "\n";
  os << indent << "RegistrationTransformNodeID: " << (this->RegistrationTransformNodeID ? this->RegistrationTransformNodeID : "(none)") << "\n";
  os << indent << "VolumeRenderingDisplayNodeID: " << (this->VolumeRenderingDisplayNodeID ? this->VolumeRenderingDisplayNodeID : "(none)") << "\n";

}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObservePETVolumeNodeID(
    const char* petVolumeNodeID)
{

  // first remove references and observers from old node
  if (this->PETVolumeNode)
    {
      vtkUnObserveMRMLObjectMacro(this->PETVolumeNode);

      if (this->Scene
          && this->Scene->IsNodeReferencingNodeID(this,
              this->PETVolumeNode->GetID()))
        this->Scene->RemoveReferencedNodeID(this->PETVolumeNode->GetID(),
            this);
    }

  vtkMRMLScalarVolumeNode* petNode = NULL;

  if (this->GetScene() && petVolumeNodeID)
    {
      petNode = vtkMRMLScalarVolumeNode::SafeDownCast(
          this->GetScene()->GetNodeByID(petVolumeNodeID));
    }

  vtkSetAndObserveMRMLObjectMacro(this->PETVolumeNode, petNode);
  this->SetPETVolumeNodeID(petVolumeNodeID);

  // TODO: check why not possible VolumeRenderingDisplayNode observes the set PETLabelVolume
  //if(this->PETVolumeNode && this->VolumeRenderingDisplayNode)
    //this->VolumeRenderingDisplayNode->SetAndObserveVolumeNodeID(this->PETVolumeNode->GetID());


  if (this->Scene && this->PETVolumeNode)
    this->Scene->AddReferencedNodeID(this->PETVolumeNodeID, this);

  if(this->PETVolumeNode && this->RegistrationTransformNode)
    this->PETVolumeNode->SetAndObserveTransformNodeID(this->RegistrationTransformNode->GetID());

}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObservePETVolumeNodeID(
    const std::string& petVolumeNodeID)
{
  this->SetAndObservePETVolumeNodeID(petVolumeNodeID.c_str());
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObserveCTVolumeNodeID(
    const char* ctVolumeNodeID)
{

  // first remove references and observers from old node
  if (this->CTVolumeNode)
    {
      vtkUnObserveMRMLObjectMacro(this->CTVolumeNode);

      if (this->Scene
          && this->Scene->IsNodeReferencingNodeID(this,
              this->CTVolumeNode->GetID()))
        this->Scene->RemoveReferencedNodeID(this->CTVolumeNode->GetID(),
            this);
    }

  vtkMRMLScalarVolumeNode* ctNode = NULL;

  if (this->GetScene() && ctVolumeNodeID)
    {
      ctNode = vtkMRMLScalarVolumeNode::SafeDownCast(
          this->GetScene()->GetNodeByID(ctVolumeNodeID));
    }

  vtkSetAndObserveMRMLObjectMacro(this->CTVolumeNode, ctNode);
  this->SetCTVolumeNodeID(ctVolumeNodeID);

  if (this->Scene && this->CTVolumeNode)
    this->Scene->AddReferencedNodeID(this->CTVolumeNodeID, this);

  if(this->CTVolumeNode && this->RegistrationTransformNode)
    this->CTVolumeNode->SetAndObserveTransformNodeID(this->RegistrationTransformNode->GetID());
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObserveCTVolumeNodeID(
    const std::string& ctVolumeNodeID)
{
  this->SetAndObserveCTVolumeNodeID(ctVolumeNodeID.c_str());
}

//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObservePETLabelVolumeNodeID(
    const char* petLabelVolumeNodeID)
{

  // first remove references and observers from old node
  if (this->PETLabelVolumeNode)
    {
      vtkUnObserveMRMLObjectMacro(this->PETLabelVolumeNode);

      if (this->Scene
          && this->Scene->IsNodeReferencingNodeID(this,
              this->PETLabelVolumeNode->GetID()))
        this->Scene->RemoveReferencedNodeID(this->PETLabelVolumeNode->GetID(),
            this);
    }

  vtkMRMLLabelMapVolumeNode* petLblNode = NULL;

  if (this->GetScene() && petLabelVolumeNodeID)
    {
      petLblNode = vtkMRMLLabelMapVolumeNode::SafeDownCast(
          this->GetScene()->GetNodeByID(petLabelVolumeNodeID));
    }

  vtkSetAndObserveMRMLObjectMacro(this->PETLabelVolumeNode, petLblNode);
  this->SetPETLabelVolumeNodeID(petLabelVolumeNodeID);

  if (this->Scene && this->PETLabelVolumeNode)
    this->Scene->AddReferencedNodeID(this->PETLabelVolumeNodeID, this);

  if(this->PETLabelVolumeNode && this->RegistrationTransformNode)
    this->PETLabelVolumeNode->SetAndObserveTransformNodeID(this->RegistrationTransformNode->GetID());
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObservePETLabelVolumeNodeID(
    const std::string& petLabelVolumeNodeID)
{
  this->SetAndObservePETLabelVolumeNodeID(petLabelVolumeNodeID.c_str());
}

//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObserveRegistrationTransformNodeID(
    const char* registrationTransformNodeID)
{
  // first remove references and observers from old node
  if (this->RegistrationTransformNode)
    {
      vtkUnObserveMRMLObjectMacro(this->RegistrationTransformNode);

      if (this->Scene
          && this->Scene->IsNodeReferencingNodeID(this,
              this->RegistrationTransformNode->GetID()))
        this->Scene->RemoveReferencedNodeID(this->RegistrationTransformNode->GetID(),
            this);
    }

  vtkMRMLLinearTransformNode* tnode = NULL;

  if (this->GetScene() && registrationTransformNodeID)
    {
      tnode = vtkMRMLLinearTransformNode::SafeDownCast(
          this->GetScene()->GetNodeByID(registrationTransformNodeID));
    }

  vtkSetAndObserveMRMLObjectMacro(this->RegistrationTransformNode, tnode);
  this->SetRegistrationTransformNodeID(registrationTransformNodeID);

  this->ObserveRegistrationTransform(true); //this->CenteredVolumes);

  if (this->Scene && this->RegistrationTransformNode)
    this->Scene->AddReferencedNodeID(this->RegistrationTransformNodeID, this);
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObserveRegistrationTransformNodeID(
    const std::string& registrationTransformNodeID)
{
  this->SetAndObserveRegistrationTransformNodeID(registrationTransformNodeID.c_str());
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObserveVolumeRenderingDisplayNodeID(
    const char* volumeRenderingDisplayNodeID)
{

  // first remove references and observers from old node
  if (this->VolumeRenderingDisplayNode)
    {
      vtkUnObserveMRMLObjectMacro(this->VolumeRenderingDisplayNode);

      if (this->Scene
          && this->Scene->IsNodeReferencingNodeID(this,
              this->VolumeRenderingDisplayNode->GetID()))
        this->Scene->RemoveReferencedNodeID(this->VolumeRenderingDisplayNode->GetID(),
            this);
    }

  vtkMRMLVolumeRenderingDisplayNode* vrdNode = NULL;

  if (this->GetScene() && volumeRenderingDisplayNodeID)
    {
      vrdNode = vtkMRMLVolumeRenderingDisplayNode::SafeDownCast(
          this->GetScene()->GetNodeByID(volumeRenderingDisplayNodeID));
    }

  vtkSetAndObserveMRMLObjectMacro(this->VolumeRenderingDisplayNode, vrdNode);
  this->SetVolumeRenderingDisplayNodeID(volumeRenderingDisplayNodeID);

  // TODO: check why not possible: observe the current PETLabelVolume
  //if(this->VolumeRenderingDisplayNode && this->PETVolumeNode)
    //this->VolumeRenderingDisplayNode->SetAndObserveVolumeNodeID(this->PETVolumeNode->GetID());

  if (this->Scene && this->VolumeRenderingDisplayNode)
    this->Scene->AddReferencedNodeID(this->VolumeRenderingDisplayNodeID, this);
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::SetAndObserveVolumeRenderingDisplayNodeID(
    const std::string& volumeRenderingDisplayNodeID)
{
  this->SetAndObserveVolumeRenderingDisplayNodeID(volumeRenderingDisplayNodeID.c_str());
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::ObserveRegistrationTransform(bool observe)
{
  if (observe && this->RegistrationTransformNode != NULL)
    {
      if (this->PETVolumeNode
          && this->PETVolumeNode->GetParentTransformNode()
              != this->RegistrationTransformNode)
        this->PETVolumeNode->SetAndObserveTransformNodeID(
            this->RegistrationTransformNode->GetID());
      if (this->CTVolumeNode
          && this->CTVolumeNode->GetParentTransformNode()
              != this->RegistrationTransformNode)
        this->CTVolumeNode->SetAndObserveTransformNodeID(
            this->RegistrationTransformNode->GetID());
      if (this->PETLabelVolumeNode
          && this->PETLabelVolumeNode->GetParentTransformNode()
              != this->RegistrationTransformNode)
        this->PETLabelVolumeNode->SetAndObserveTransformNodeID(
            this->RegistrationTransformNode->GetID());
      if (this->VolumeRenderingDisplayNode)
        {
          vtkMRMLAnnotationROINode* roiNode =
              this->VolumeRenderingDisplayNode->GetROINode();
          if (roiNode
              && roiNode->GetParentTransformNode()
                  != this->RegistrationTransformNode)
            roiNode->SetAndObserveTransformNodeID(
                this->RegistrationTransformNode->GetID());


          vtkMRMLVolumePropertyNode* propNode =
              this->VolumeRenderingDisplayNode->GetVolumePropertyNode();
          if (propNode
              && propNode->GetParentTransformNode()
                  != this->RegistrationTransformNode)
            propNode->SetAndObserveTransformNodeID(
                this->RegistrationTransformNode->GetID());

        }
    }
  else
    {
      if (this->PETVolumeNode)
        this->PETVolumeNode->SetAndObserveTransformNodeID(NULL);
      if (this->CTVolumeNode)
        this->CTVolumeNode->SetAndObserveTransformNodeID(NULL);
      if (this->PETLabelVolumeNode)
        this->PETLabelVolumeNode->SetAndObserveTransformNodeID(NULL);
      if (this->VolumeRenderingDisplayNode)
        {
          vtkMRMLAnnotationROINode* roiNode =
              this->VolumeRenderingDisplayNode->GetROINode();
          if (roiNode)
            roiNode->SetAndObserveTransformNodeID(NULL);

          vtkMRMLVolumePropertyNode* propNode =
              this->VolumeRenderingDisplayNode->GetVolumePropertyNode();
          if (propNode)
            propNode->SetAndObserveTransformNodeID(NULL);
        }
    }


  this->Modified();
}


//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::UpdateReferences()
{
  this->Superclass::UpdateReferences();

  if(this->Scene)
    {
      if(this->PETVolumeNodeID && this->Scene->GetNodeByID(this->PETVolumeNodeID) == NULL)
        this->SetAndObservePETVolumeNodeID(NULL);

      else if(this->CTVolumeNodeID && this->GetScene()->GetNodeByID(this->CTVolumeNodeID) == NULL)
        this->SetAndObserveCTVolumeNodeID(NULL);

      else if(this->PETLabelVolumeNodeID && this->GetScene()->GetNodeByID(this->PETLabelVolumeNodeID) == NULL)
        this->SetAndObservePETLabelVolumeNodeID(NULL);

      else if(this->RegistrationTransformNodeID && this->GetScene()->GetNodeByID(this->RegistrationTransformNodeID) == NULL)
        this->SetAndObserveRegistrationTransformNodeID(NULL);

      else if(this->VolumeRenderingDisplayNodeID && this->GetScene()->GetNodeByID(this->VolumeRenderingDisplayNodeID) == NULL)
        this->SetAndObserveVolumeRenderingDisplayNodeID(NULL);
    }
}

//----------------------------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::UpdateReferenceID(const char *oldID, const char *newID)
{
  this->Superclass::UpdateReferenceID(oldID,newID);

  if(this->PETVolumeNode && !strcmp(oldID,this->PETLabelVolumeNodeID))
    this->SetAndObservePETVolumeNodeID(newID);

  else if(this->CTVolumeNode && !strcmp(oldID,this->CTVolumeNodeID))
    this->SetAndObserveCTVolumeNodeID(newID);

  else if(this->PETLabelVolumeNode && !strcmp(oldID,this->PETLabelVolumeNodeID))
    this->SetAndObservePETLabelVolumeNodeID(newID);

  else if(this->RegistrationTransformNode && !strcmp(oldID,this->RegistrationTransformNodeID))
    this->SetAndObserveRegistrationTransformNodeID(newID);

  else if(this->VolumeRenderingDisplayNode && !strcmp(oldID,this->VolumeRenderingDisplayNodeID))
    this->SetAndObserveVolumeRenderingDisplayNodeID(newID);
}

//----------------------------------------------------------------------------
void vtkMRMLLongitudinalPETCTStudyNode::SetScene(vtkMRMLScene* scene)
{
  bool update = this->Scene != scene;

  Superclass::SetScene(scene);

  if(update)
    this->UpdateScene(this->Scene);
}


//-----------------------------------------------------------
void
vtkMRMLLongitudinalPETCTStudyNode::UpdateScene(vtkMRMLScene *scene)
{
  Superclass::UpdateScene(scene);

  if (this->Scene && this->Scene == scene)
    {
      vtkMRMLNode* petVolumeNode = this->Scene->GetNodeByID(this->PETVolumeNodeID);
      vtkMRMLNode* ctVolumeNode = this->Scene->GetNodeByID(this->CTVolumeNodeID);
      vtkMRMLNode* petLabelVolumeNode = this->Scene->GetNodeByID(this->PETLabelVolumeNodeID);
      vtkMRMLNode* registrationTransformNode = this->Scene->GetNodeByID(this->RegistrationTransformNodeID);
      vtkMRMLNode* volumeRenderingDisplayNode = this->Scene->GetNodeByID(this->VolumeRenderingDisplayNodeID);

      if(petVolumeNode && this->PETVolumeNode != petVolumeNode)
        this->SetAndObservePETVolumeNodeID(this->PETVolumeNodeID);

      if(ctVolumeNode && this->CTVolumeNode != ctVolumeNode)
        this->SetAndObserveCTVolumeNodeID(this->CTVolumeNodeID);

      if(petLabelVolumeNode && this->PETLabelVolumeNode != petLabelVolumeNode)
        this->SetAndObservePETLabelVolumeNodeID(this->PETLabelVolumeNodeID);

      if(registrationTransformNode && this->RegistrationTransformNode != registrationTransformNode)
        this->SetAndObserveRegistrationTransformNodeID(this->RegistrationTransformNodeID);

      if(volumeRenderingDisplayNode && this->VolumeRenderingDisplayNode != volumeRenderingDisplayNode)
        this->SetAndObserveVolumeRenderingDisplayNodeID(this->VolumeRenderingDisplayNodeID);
    }
}



