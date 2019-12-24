#include "landscapewidget.h"
#include "ui_landscapewidget.h"
#include "Rendering/mainwindow.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Systems/landscapemanager.h"
#include "Components/landscapecomponent.h"
#include "Components/meshcomponent.h"
#include "Components/materialcomponent.h"

LandscapeWidget::LandscapeWidget(MainWindow *inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow, parent),
    ui(new Ui::LandscapeWidget)
{
    ui->setupUi(this);

}

LandscapeWidget::~LandscapeWidget()
{
    delete ui;
}

void LandscapeWidget::updateData()
{
    if(mMainWindow->getActiveItemID()<0) return;
    LandscapeComponent* lComp = Engine::getInstance()->mObjectManager->landscapeComps[mMainWindow->getActiveItemID()];
    ui->RadiusSpinBox->setValue(Engine::getInstance()->mLandscapeManager->sculptRange);
    ui->SensitivitySpinBox->setValue(Engine::getInstance()->mLandscapeManager->sculptSensitivity);
    if(!lComp)
    {
        qDebug() << "No Landscape";
        //setEnabled(false);
        return;
    }

    ui->maxXSpinBox->setValue(lComp->xmax);
    ui->minXSpinBox->setValue(lComp->xmin);
    ui->maxZSpinBox->setValue(lComp->zmax);
    ui->minZSpinBox->setValue(lComp->zmin);
    ui->doubleSpinBox_2->setValue(lComp->h);
}

void LandscapeWidget::on_RadiusSpinBox_valueChanged(double arg1)
{
    Engine::getInstance()->mLandscapeManager->sculptRange = arg1;
}

void LandscapeWidget::on_SensitivitySpinBox_valueChanged(double arg1)
{
    Engine::getInstance()->mLandscapeManager->sculptSensitivity = arg1;
}

void LandscapeWidget::on_radioButton_toggled(bool checked)
{
    Engine::getInstance()->mLandscapeManager->isSculptingAllowed = checked;
    updateData();
}

void LandscapeWidget::on_pushButton_clicked()
{
    LandscapeComponent* lComp = nullptr;
    if(mMainWindow->getActiveItemID()>=0)
    {
        lComp = Engine::getInstance()->mObjectManager->landscapeComps[mMainWindow->getActiveItemID()];
    }
    if(!lComp)
    {
        lComp = Engine::getInstance()->mLandscapeManager->makeLandscape();
    }
    lComp->xmax = ui->maxXSpinBox->value();
    lComp->xmin = ui->minXSpinBox->value();
    lComp->zmax = ui->maxZSpinBox->value();
    lComp->zmin = ui->minZSpinBox->value();
    lComp->h = ui->doubleSpinBox_2->value();
    Engine::getInstance()->mLandscapeManager->construct(lComp);
}

