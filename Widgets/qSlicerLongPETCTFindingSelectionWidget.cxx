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

// LongPETCTFindingSelection Widgets includes
#include "qSlicerLongPETCTFindingSelectionWidget.h"
#include "ui_qSlicerLongPETCTFindingSelectionWidget.h"


#include <QMessageBox>


#include <qMRMLNodeComboBox.h>
#include <vtkMRMLLongPETCTFindingNode.h>
#include <vtkMRMLColorNode.h>

#include "qSlicerLongPETCTFindingSettingsDialog.h"




//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_LongPETCT
class qSlicerLongPETCTFindingSelectionWidgetPrivate :
  public Ui_qSlicerLongPETCTFindingSelectionWidget
//  : public QWidget
{
  Q_DECLARE_PUBLIC(qSlicerLongPETCTFindingSelectionWidget);
protected:
  qSlicerLongPETCTFindingSelectionWidget* const q_ptr;

public:
  qSlicerLongPETCTFindingSelectionWidgetPrivate(
    qSlicerLongPETCTFindingSelectionWidget& object);

  virtual ~qSlicerLongPETCTFindingSelectionWidgetPrivate();

  virtual void setupUi(qSlicerLongPETCTFindingSelectionWidget* widget);

  vtkMRMLColorNode* ColorNode;

};

// --------------------------------------------------------------------------
qSlicerLongPETCTFindingSelectionWidgetPrivate
::qSlicerLongPETCTFindingSelectionWidgetPrivate(
  qSlicerLongPETCTFindingSelectionWidget& object)
  : q_ptr(&object), ColorNode(NULL)
{
}

// --------------------------------------------------------------------------
qSlicerLongPETCTFindingSelectionWidgetPrivate
::~qSlicerLongPETCTFindingSelectionWidgetPrivate()
{
}

// --------------------------------------------------------------------------
void qSlicerLongPETCTFindingSelectionWidgetPrivate
::setupUi(qSlicerLongPETCTFindingSelectionWidget* widget)
{
  Q_Q(qSlicerLongPETCTFindingSelectionWidget);

  this->Ui_qSlicerLongPETCTFindingSelectionWidget::setupUi(widget);

  this->MRMLNodeComboBoxFinding->setNodeTypes(QStringList("vtkMRMLLongPETCTFindingNode"));

  QObject::connect( this->MRMLNodeComboBoxFinding, SIGNAL(nodeAddedByUser(vtkMRMLNode*)), q, SIGNAL(findingNodeAddedByUser(vtkMRMLNode*)) );
  QObject::connect( this->CheckBoxROIVisiblity, SIGNAL(toggled(bool)), q, SIGNAL(roiVisibilityChanged(bool)) );
  QObject::connect( this->ButtonHelp, SIGNAL(clicked()), q, SIGNAL(helpRequested()) );
  QObject::connect( this->ButtonAddSegmentationToFinding, SIGNAL(clicked()), q, SIGNAL(addSegmentationToFinding()) );
}
  //-----------------------------------------------------------------------------
// qSlicerLongPETCTFindingSelectionWidget methods

//-----------------------------------------------------------------------------
qSlicerLongPETCTFindingSelectionWidget
::qSlicerLongPETCTFindingSelectionWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerLongPETCTFindingSelectionWidgetPrivate(*this) )
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerLongPETCTFindingSelectionWidget
::~qSlicerLongPETCTFindingSelectionWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerLongPETCTFindingSelectionWidget::setMRMLScene(vtkMRMLScene* mrmlScene)
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  Q_ASSERT(d->MRMLNodeComboBoxFinding);

  d->MRMLNodeComboBoxFinding->setMRMLScene(mrmlScene);
  d->MRMLNodeComboBoxFinding->setAddEnabled(true);
  d->MRMLNodeComboBoxFinding->setRemoveEnabled(true);
  d->MRMLNodeComboBoxFinding->setRenameEnabled(false);
  d->MRMLNodeComboBoxFinding->setEditEnabled(true);
}


//-----------------------------------------------------------------------------
vtkMRMLScene* qSlicerLongPETCTFindingSelectionWidget::mrmlScene()
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);

  return d->MRMLNodeComboBoxFinding->mrmlScene();
}


//-----------------------------------------------------------------------------
qMRMLNodeComboBox* qSlicerLongPETCTFindingSelectionWidget::mrmlNodeComboBoxFinding()
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);

  return d->MRMLNodeComboBoxFinding;
}


//-----------------------------------------------------------------------------
void qSlicerLongPETCTFindingSelectionWidget
::updateView()
{
  //Q_D(qSlicerLongPETCTFindingSelectionWidget);
}


//-----------------------------------------------------------------------------
void qSlicerLongPETCTFindingSelectionWidget
::setSelectionEnabled(bool enabled)
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  Q_ASSERT(d->LabelSelectFinding);
  Q_ASSERT(d->MRMLNodeComboBoxFinding);

  d->LabelSelectFinding->setEnabled(enabled);
  d->MRMLNodeComboBoxFinding->setEnabled(enabled);
}

//-----------------------------------------------------------------------------
bool qSlicerLongPETCTFindingSelectionWidget
::selectionEnabled()
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  Q_ASSERT(d->LabelSelectFinding);
  Q_ASSERT(d->MRMLNodeComboBoxFinding);

  return d->LabelSelectFinding->isEnabled() && d->MRMLNodeComboBoxFinding->isEnabled();
}

//-----------------------------------------------------------------------------
void qSlicerLongPETCTFindingSelectionWidget
::setROIVisibility(bool visibility)
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  Q_ASSERT(d->CheckBoxROIVisiblity);

  d->CheckBoxROIVisiblity->setChecked(visibility);
}

//-----------------------------------------------------------------------------
bool qSlicerLongPETCTFindingSelectionWidget
::roiVisibility()
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  Q_ASSERT(d->CheckBoxROIVisiblity);

  return d->CheckBoxROIVisiblity->isChecked();
}

//-----------------------------------------------------------------------------
void qSlicerLongPETCTFindingSelectionWidget
::hideAddButton(bool hide)
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  Q_ASSERT(d->ButtonAddSegmentationToFinding);

  d->ButtonAddSegmentationToFinding->setHidden(hide);
}

//-----------------------------------------------------------------------------
void qSlicerLongPETCTFindingSelectionWidget
::setEditorWidget(QWidget* editorWidget)
{
  Q_D(qSlicerLongPETCTFindingSelectionWidget);
  Q_ASSERT(d->FormLayout);
  Q_ASSERT(d->ButtonAddSegmentationToFinding);

  d->ButtonAddSegmentationToFinding->setVisible(false);

  d->FormLayout->setWidget(1,QFormLayout::SpanningRole,editorWidget);
  d->FormLayout->setWidget(2,QFormLayout::SpanningRole,d->ButtonAddSegmentationToFinding);

  int minWidth = editorWidget->minimumWidth();
  this->setMinimumWidth(minWidth);
}
