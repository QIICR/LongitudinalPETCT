/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QtPlugin>
#include <QFileInfo>
#include <QScopedPointer>

// ExtensionTemplate Logic includes
#include <vtkSlicerLongitudinalPETCTLogic.h>

// ExtensionTemplate includes
#include "qSlicerLongitudinalPETCTModule.h"

// SlicerQT includes
#include <qSlicerModuleManager.h>
#include <qSlicerScriptedLoadableModuleWidget.h>
#include <qSlicerUtils.h>
#include <vtkSlicerConfigure.h>

//-----------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtPlugin>
Q_EXPORT_PLUGIN2(qSlicerLongitudinalPETCTModule, qSlicerLongitudinalPETCTModule);
#endif

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerLongitudinalPETCTModulePrivate
{
public:
  qSlicerLongitudinalPETCTModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerLongitudinalPETCTModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerLongitudinalPETCTModulePrivate::qSlicerLongitudinalPETCTModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerLongitudinalPETCTModule methods

//-----------------------------------------------------------------------------
qSlicerLongitudinalPETCTModule::qSlicerLongitudinalPETCTModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerLongitudinalPETCTModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerLongitudinalPETCTModule::~qSlicerLongitudinalPETCTModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerLongitudinalPETCTModule::helpText()const
{
  QString help = "The purpose of the Longitudinal PET/CT Analysis module is to provide a user friendly Slicer interface for quantification of DICOM PET/CT image data by computing the standardized uptake value (SUV<span style=\"vertical-align:sub;\">bw</span>) based on bodyweight for different regions of interest at different timepoints.<br />For more detailed documentation see the <a href=\"%1/Documentation/Nightly/Extensions/LongitudinalPETCT\">online documentation</a>.";
  return help.arg(this->slicerWikiUrl());
}

//-----------------------------------------------------------------------------
QString qSlicerLongitudinalPETCTModule::acknowledgementText()const
{
  return "This work is done as part of a master thesis (for the University of Heidelberg) at the Surgical Planning Lab (Brigham and Women's Hospital, Boston)";
}

//-----------------------------------------------------------------------------
QStringList qSlicerLongitudinalPETCTModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Paul Mercea (University of Heidelberg), Markus Van Tol (University of Iowa) and Andrey Fedorov (SPL)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerLongitudinalPETCTModule::icon()const
{
  return QIcon(":/Resources/Icons/LongitudinalPETCT.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerLongitudinalPETCTModule::categories() const
{
  return QStringList() << "Work in Progress";
}

//-----------------------------------------------------------------------------
QStringList qSlicerLongitudinalPETCTModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerLongitudinalPETCTModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerLongitudinalPETCTModule::createWidgetRepresentation()
{
  QString pythonPath = qSlicerUtils::pathWithoutIntDir(
              QFileInfo(this->path()).path(), Slicer_QTLOADABLEMODULES_LIB_DIR);

  QScopedPointer<qSlicerScriptedLoadableModuleWidget> widget(new qSlicerScriptedLoadableModuleWidget);
  QString classNameToLoad = "qSlicerLongitudinalPETCTModuleWidget";
  bool ret = widget->setPythonSource(
        pythonPath + "/Python/" + classNameToLoad + ".py", classNameToLoad);
  if (!ret)
    {
    return 0;
    }
  return widget.take();
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerLongitudinalPETCTModule::createLogic()
{
  vtkSlicerLongitudinalPETCTLogic* logic = vtkSlicerLongitudinalPETCTLogic::New();

  return logic;
}
