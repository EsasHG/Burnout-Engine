#include "camerawidget.h"
#include "ui_camerawidget.h"
#include "Rendering/mainwindow.h"
#include "Systems/engine.h"
#include "Systems/camerasystem.h"
#include "Systems/objectmanager.h"
#include "Components/cameracomponent.h"


CameraWidget::CameraWidget(MainWindow* inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow, parent),
    ui(new Ui::CameraWidget)
{
    ui->setupUi(this);
}

CameraWidget::~CameraWidget()
{
    delete ui;
}

void CameraWidget::on_checkBox_toggled(bool checked)
{
    CameraComponent* cComp = Engine::getInstance()->mObjectManager->cameraComps[mMainWindow->getActiveItemID()];
    if(cComp)
    {
        cComp->bIsThirdPersonCamera = checked;
        ui->horizontalWidget->setEnabled(checked);
    }
}

void  CameraWidget::updateData()
{
    CameraComponent* cComp = Engine::getInstance()->mObjectManager->cameraComps[mMainWindow->getActiveItemID()];
    if(cComp)
    {
        ui->checkBox->setChecked(cComp->bIsThirdPersonCamera);
        ui->spinBox->setValue(cComp->entityToLookAt);
        ui->spinBox->setRange(0, Engine::getInstance()->mObjectManager->cameraComps.size()-1);
    }
}

void CameraWidget::on_spinBox_valueChanged(int arg1)
{
    CameraComponent* cComp = Engine::getInstance()->mObjectManager->cameraComps[mMainWindow->getActiveItemID()];
    if(cComp)
    {
        cComp->entityToLookAt = ui->spinBox->value();
    }
}
