/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkMRMLVolumeRenderingParametersNode.h,v $
  Date:      $Date: 2006/03/19 17:12:29 $
  Version:   $Revision: 1.3 $

=========================================================================auto=*/
// .NAME vtkMRMLLongitudinalPETCTFindingNode
// This node keeps pointers to the two elements of the Finding: annotation
// and markup.
//
// Annotation element will (eventually) contain the Finding template and the
// user-initialized fields of the template.
//
// Markup element will point to the hierarchy of the markup elements.
//
// On export/import, this node will be used by the IO logic to determine how
// to code the formatted Finding.
// .SECTION Description
// This node keeps pointers to the two elements of the Finding: annotation
// and markup.
//

#ifndef __vtkMRMLLongitudinalPETCTFindingNode_h
#define __vtkMRMLLongitudinalPETCTFindingNode_h

// MRML includes
//#include "vtkDoubleArray.h"
#include "vtkMRML.h"
#include "vtkMRMLNode.h"

#include <vtkIntArray.h>

#include <vtkSmartPointer.h>

#include <vtkMRMLAnnotationROINode.h>

#include <vtkSlicerLongitudinalPETCTModuleMRMLExport.h>



class map;
class vtkMRMLLongitudinalPETCTStudyNode;
class vtkMRMLScalarVolumeNode;
class vtkMRMLLongitudinalPETCTSegmentationNode;

class vtkEventForwarderCommand;


#include <vtkNew.h>
#include <vtkSmartPointer.h>
/// \ingroup Slicer_QtModules_LongitudinalPETCTFindingNode
class VTK_SLICER_LONGITUDINALPETCT_MODULE_MRML_EXPORT vtkMRMLLongitudinalPETCTFindingNode : public vtkMRMLNode
{
  public:   

  static vtkMRMLLongitudinalPETCTFindingNode *New();
  vtkTypeMacro(vtkMRMLLongitudinalPETCTFindingNode,vtkMRMLNode);
  void PrintSelf(ostream& os, vtkIndent indent);

  typedef std::map<std::string,std::string> StudyIDSegIDMap;
  typedef std::map<std::string,std::vector<double> > StudyIDROIControlPointsMap;

  virtual vtkMRMLNode* CreateNodeInstance();

  /// Set node attributes
  virtual void ReadXMLAttributes( const char** atts);

  /// Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  /// Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node);

  /// Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() {return "PETCT_Finding";};

  bool MapStudyNodeIDToSegmentationNodeID(const char* studyNodeID, const char* segmentationNodeID);
  vtkMRMLLongitudinalPETCTSegmentationNode* RemoveStudyNodeIDToSegmentationNodeIDMap(const char* studyNodeID);
  vtkMRMLLongitudinalPETCTSegmentationNode* GetSegmentationMappedByStudyNodeID(const char* studyNodeID);

  vtkGetMacro(ColorID, int);
  vtkSetMacro(ColorID, int);

  int GetNumberOfSegmentations();

  const StudyIDSegIDMap& GetStudyNodeIDToSegmentationNodeIDMap() const;

  void ProcessMRMLEvents(vtkObject *caller, unsigned long event, void *callData);
  void UpdateReferences();
  void UpdateReferenceID(const char *oldID, const char *newID);
  void UpdateScene(vtkMRMLScene *scene);
  void SetScene(vtkMRMLScene* scene);

  void SetROIxyz(double roiXYZ[3]);
  void GetROIxyz(double xyz[3]);

  void SetROIRadius(double roiRadius[3]);
  void GetROIRadius(double radius[3]);

  vtkBooleanMacro(SegmentationROISpecified, bool);
  vtkGetMacro(SegmentationROISpecified, bool);
  vtkSetMacro(SegmentationROISpecified, bool);

protected:
  vtkMRMLLongitudinalPETCTFindingNode();
  ~vtkMRMLLongitudinalPETCTFindingNode();
  vtkMRMLLongitudinalPETCTFindingNode(const vtkMRMLLongitudinalPETCTFindingNode&);
  void operator=(const vtkMRMLLongitudinalPETCTFindingNode&);


  bool SegmentationROISpecified;

  bool IsSegmentationNodeInMap(const char* segmentationNodeID);

  StudyIDSegIDMap StudyIDToSegmentationIDMap;

  int ColorID;

  double ROIxyz[3];
  double ROIRadius[3];


};

#endif

