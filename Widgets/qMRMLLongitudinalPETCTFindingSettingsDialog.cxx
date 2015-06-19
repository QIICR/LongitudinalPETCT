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

// LongitudinalPETCTFindingSelection Widgets includes
#include "qMRMLLongitudinalPETCTFindingSettingsDialog.h"
#include "ui_qMRMLLongitudinalPETCTFindingSettingsDialog.h"

#include <QAbstractButton>
#include <QDebug>
#include <QMessageBox>

#include <vtkMRMLLongitudinalPETCTFindingNode.h>
#include <vtkMRMLLongitudinalPETCTReportNode.h>
#include <vtkMRMLLongitudinalPETCTSegmentationNode.h>



//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_LongitudinalPETCT
class qMRMLLongitudinalPETCTFindingSettingsDialogPrivate : public Ui_qMRMLLongitudinalPETCTFindingSettingsDialog
{
  Q_DECLARE_PUBLIC(qMRMLLongitudinalPETCTFindingSettingsDialog);

protected:
  qMRMLLongitudinalPETCTFindingSettingsDialog* const q_ptr;

public:
  qMRMLLongitudinalPETCTFindingSettingsDialogPrivate(
      qMRMLLongitudinalPETCTFindingSettingsDialog& object);

  virtual
  ~qMRMLLongitudinalPETCTFindingSettingsDialogPrivate();

  virtual void
  setupUi(qMRMLLongitudinalPETCTFindingSettingsDialog* widget);

  QString findingName;
  int findingColorID;
  QString findingPriorSegmentationID;

};

// --------------------------------------------------------------------------
qMRMLLongitudinalPETCTFindingSettingsDialogPrivate::qMRMLLongitudinalPETCTFindingSettingsDialogPrivate(
    qMRMLLongitudinalPETCTFindingSettingsDialog& object) :
  q_ptr(&object), findingName(""), findingColorID(-1), findingPriorSegmentationID("")
{
}

// --------------------------------------------------------------------------
qMRMLLongitudinalPETCTFindingSettingsDialogPrivate::~qMRMLLongitudinalPETCTFindingSettingsDialogPrivate()
{
}

// --------------------------------------------------------------------------
void
qMRMLLongitudinalPETCTFindingSettingsDialogPrivate::setupUi(
    qMRMLLongitudinalPETCTFindingSettingsDialog* widget)
{
  Q_Q(qMRMLLongitudinalPETCTFindingSettingsDialog);

  this->Ui_qMRMLLongitudinalPETCTFindingSettingsDialog::setupUi(widget);
  this->ComboBoxColor->setNoneEnabled(false);

  QObject::connect( this->ButtonGroupFindingPresets, SIGNAL(buttonClicked(QAbstractButton*)), q, SLOT(presetButtonClicked(QAbstractButton*)) );

  QObject::connect(this->PriorSegmentationMRMLNodeComboBox,
                   SIGNAL(currentNodeChanged(vtkMRMLNode*)),
                   q,
                   SLOT(priorSegmentationChanged(vtkMRMLNode*)));

}



//-----------------------------------------------------------------------------
// qMRMLLongitudinalPETCTFindingSettingsDialog methods

//-----------------------------------------------------------------------------
qMRMLLongitudinalPETCTFindingSettingsDialog::qMRMLLongitudinalPETCTFindingSettingsDialog(QWidget* parentWidget) :
    Superclass(parentWidget), d_ptr(
        new qMRMLLongitudinalPETCTFindingSettingsDialogPrivate(*this))
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  d->setupUi(this);

  this->setWindowTitle("Finding Settings");
  this->setModal(true);

}

//-----------------------------------------------------------------------------
qMRMLLongitudinalPETCTFindingSettingsDialog::~qMRMLLongitudinalPETCTFindingSettingsDialog()
{
}



//-----------------------------------------------------------------------------
void qMRMLLongitudinalPETCTFindingSettingsDialog::accept()
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  Q_ASSERT(d->ComboBoxColor);

  QString name = d->LineEditName->text();
  int id = d->ComboBoxColor->currentColor();
  QString priorLabelMapID;
  if (d->PriorSegmentationMRMLNodeComboBox->currentNode())
    {
    priorLabelMapID = d->PriorSegmentationMRMLNodeComboBox->currentNodeID();
    }

  d->findingName = name;
  d->findingColorID = id;
  d->findingPriorSegmentationID = priorLabelMapID;

  double c[4];

  vtkMRMLLongitudinalPETCTReportNode* report = this->reportNode();

  if(!report)
    return;

  vtkMRMLColorNode* cn = const_cast<vtkMRMLColorTableNode*>(this->reportNode()->GetColorTableNode());

  if (cn->GetColor(findingColorID(), c))
    {
      if (c[0] == 0. && c[1] == 0. && c[2] == 0.)
        {
          QMessageBox::warning(this, "Finding Settings Warning",
              "The selected color seems to be a background color, which can not be used for Findings. Please select another color");
        }
      else if (report->IsFindingColorInUse(findingColorID()))
        {
          QMessageBox::warning(this, "Finding Settings Warning",
              "The selected color is already assigned to a different Finding. Please select another color.");
        }
      else if (d->findingName.isEmpty())
        {
          QMessageBox::warning(this, "Finding Settings Warning",
              "The Finding name is invalid, please type in a valid name.");
        }
      else if (report->IsFindingNameInUse(d->findingName.toStdString().c_str()))
        {
          QMessageBox::warning(this, "Finding Settings Warning",
              "The Finding name is already in use, please select another name.");
        }
      // the prior segmentation is optional
      else
        {
        emit findingSpecified(name, id, priorLabelMapID);
        Superclass::accept();
        }
    }
}

//-----------------------------------------------------------------------------
void qMRMLLongitudinalPETCTFindingSettingsDialog::updateDialogFromMRML()
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  Q_ASSERT(d->ComboBoxColor);
  Q_ASSERT(d->ButtonPresetTumor);
  Q_ASSERT(d->PriorSegmentationMRMLNodeComboBox);

  vtkMRMLLongitudinalPETCTReportNode* report = this->reportNode();

  if (report)
    {
    d->ComboBoxColor->setMRMLColorNode(const_cast<vtkMRMLColorTableNode*>(report->GetColorTableNode()));
    d->ButtonPresetTumor->click();

    bool enablePriorFlag = 0;
    d->PriorSegmentationMRMLNodeComboBox->setMRMLScene(report->GetScene());
    if (this->reportNode()->GetScene() &&
        this->reportNode()->GetScene()->GetNumberOfNodesByClass("vtkMRMLLabelMapVolumeNode") > 0)
      {
      int numLabelMaps = this->reportNode()->GetScene()->GetNumberOfNodesByClass("vtkMRMLLabelMapVolumeNode");
      // right now, there's no way to check that there are label map
      // volumes in the scene that aren't associated with this study
      // (the finding node and active label map node are set after
      // hitting accept, the label map node doesn't have any specific
      // attributes), so for now if there are more than 2 label
      // volumes in the scene, activate it
      if (numLabelMaps > 1)
        {
        enablePriorFlag = true;
        }
      }
      d->InitializeFromPriorSegmentationCollapsibleButton->setEnabled(enablePriorFlag);
    }
}


//-----------------------------------------------------------------------------
void
qMRMLLongitudinalPETCTFindingSettingsDialog::presetButtonClicked(
    QAbstractButton* button)
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  Q_ASSERT(d->LineEditName);
  Q_ASSERT(d->ComboBoxColor);

  QString name = button->accessibleName();

  if (name.compare("Tumor") == 0)
    {
      d->LineEditName->setText("Tumor");
      d->ComboBoxColor->setCurrentColor(7);
    }
  else if (name.compare("Node") == 0)
    {
      d->LineEditName->setText("Lymph Node");
      d->ComboBoxColor->setCurrentColor(23);
    }
  else if (name.compare("Liver") == 0)
    {
      d->LineEditName->setText("Liver");
      d->ComboBoxColor->setCurrentColor(216);
    }
  else if (name.compare("Cerebellum") == 0)
    {
      d->LineEditName->setText("Cerebellum");
      d->ComboBoxColor->setCurrentColor(105);
    }
  else if (name.compare("Aorta") == 0)
    {
      d->LineEditName->setText("Aorta");
      d->ComboBoxColor->setCurrentColor(191);
    }
}

//-----------------------------------------------------------------------------
void
qMRMLLongitudinalPETCTFindingSettingsDialog::priorSegmentationChanged(
  vtkMRMLNode *node)
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  Q_ASSERT(d->PriorSegmentationMRMLNodeComboBox);

  if (!node || !node->GetID())
    {
    return;
    }

  const char *activeLabelMapID = NULL;

  // check to be sure that it's not the current segmentation associated with
  // this finding
  if (this->reportNode() &&
      this->reportNode()->GetActiveStudyNode() &&
      this->reportNode()->GetActiveFindingNode())
    {
    vtkSmartPointer<vtkMRMLLongitudinalPETCTSegmentationNode> segmentationNode = NULL;
    segmentationNode = this->reportNode()->GetActiveFindingNode()->GetSegmentationMappedByStudyNodeID(this->reportNode()->GetActiveStudyNode()->GetID());
    if (segmentationNode)
      {
      activeLabelMapID = segmentationNode->GetLabelVolumeNodeID();

      if (!activeLabelMapID)
        {
        return;
        }
      if (strcmp(activeLabelMapID, node->GetID()) == 0)
        {
        qDebug() << "priorSegmentationChanged: selected node is the same as the active label map id, choose another!";
        return;
        }
      }
    else
      {
      return;
      }
    }

  this->setFindingPriorSegmentationID(node->GetID());
}

//-----------------------------------------------------------------------------
QString
qMRMLLongitudinalPETCTFindingSettingsDialog::findingName()
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  return d->findingName;
}

//-----------------------------------------------------------------------------
int
qMRMLLongitudinalPETCTFindingSettingsDialog::findingColorID()
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  return d->findingColorID;
}

//-----------------------------------------------------------------------------
QString
qMRMLLongitudinalPETCTFindingSettingsDialog::findingPriorSegmentationID()
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  return d->findingPriorSegmentationID;
}

//-----------------------------------------------------------------------------
void
qMRMLLongitudinalPETCTFindingSettingsDialog::setFindingName(const QString& name)
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  d->findingName = name;
}

//-----------------------------------------------------------------------------
void
qMRMLLongitudinalPETCTFindingSettingsDialog::setFindingColorID(int id)
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  d->findingColorID = id;
}

//-----------------------------------------------------------------------------
void
qMRMLLongitudinalPETCTFindingSettingsDialog::setFindingPriorSegmentationID(const QString& id)
{
  Q_D(qMRMLLongitudinalPETCTFindingSettingsDialog);
  d->findingPriorSegmentationID = id;
}
