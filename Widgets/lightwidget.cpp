#include "lightwidget.h"
#include "ui_lightwidget.h"

#include "Rendering/mainwindow.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Components/lightcomponent.h"


LightWidget::LightWidget(MainWindow* inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow, parent),
    ui(new Ui::LightWidget)
{
    ui->setupUi(this);
}

LightWidget::~LightWidget()
{
    delete ui;
}

void LightWidget::updateData()
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(!light)
    { return; }
    ui->ambientXSpinbox->setValue(light->mAmbientColor.x);
    ui->ambientYSpinbox->setValue(light->mAmbientColor.y);
    ui->ambientZSpinbox->setValue(light->mAmbientColor.z);
    ui->diffuseXSpinbox->setValue(light->mDiffuseColor.x);
    ui->diffuseYSpinbox->setValue(light->mDiffuseColor.y);
    ui->diffuseZSpinbox->setValue(light->mDiffuseColor.z);
    ui->specularXSpinbox->setValue(light->mSpecularColor.x);
    ui->specularYSpinbox->setValue(light->mSpecularColor.y);
    ui->specularZSpinbox->setValue(light->mSpecularColor.z);
}

//ambient
void LightWidget::on_ambientXSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mAmbientColor.x = static_cast<float>(arg1);
}

void LightWidget::on_ambientYSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mAmbientColor.y = static_cast<float>(arg1);
}

void LightWidget::on_ambientZSpinbox_valueChanged(double arg1)
{

    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mAmbientColor.z = static_cast<float>(arg1);
}

//diffuse
void LightWidget::on_diffuseXSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mDiffuseColor.x = static_cast<float>(arg1);
}

void LightWidget::on_diffuseYSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mDiffuseColor.y = static_cast<float>(arg1);
}

void LightWidget::on_diffuseZSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mDiffuseColor.z = static_cast<float>(arg1);
}

//specular
void LightWidget::on_specularXSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mSpecularColor.x = static_cast<int>(arg1);
}

void LightWidget::on_specularYSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mSpecularColor.y = static_cast<int>(arg1);
}

void LightWidget::on_specularZSpinbox_valueChanged(double arg1)
{
    LightComponent* light = Engine::getInstance()->mObjectManager->lightComps[static_cast<unsigned>(mMainWindow->getActiveItemID())];
    if(light)
        light->mSpecularColor.z = static_cast<int>(arg1);
}
