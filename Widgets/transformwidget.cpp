#include "transformwidget.h"
#include "ui_transformwidget.h"

#include <QTreeWidget>

#include "Rendering/mainwindow.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Components/transformcomponent.h"
#include "Systems/transformsystem.h"
TransformWidget::TransformWidget(MainWindow *inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow, parent),
    ui(new Ui::TransformWidget)
{
    ui->setupUi(this);
}

TransformWidget::~TransformWidget()
{
    delete ui;
}

void TransformWidget::updateData()
{

    int id = mMainWindow->getActiveItemID();
    if(id <0) return;
    unsigned eID = static_cast<unsigned>(id);
    gsl::Vector3D pos = Engine::getInstance()->mTransformSystem->getLocalPosition(eID);
    gsl::Vector3D rot = Engine::getInstance()->mTransformSystem->getLocalRotation(eID);
    gsl::Vector3D scale = Engine::getInstance()->mTransformSystem->getLocalScale(eID);

    ui->posSpinBoxX->setValue(static_cast<double>(pos.x));
    ui->posSpinBoxY->setValue(static_cast<double>(pos.y));
    ui->posSpinboxZ->setValue(static_cast<double>(pos.z));
    ui->rotSpinBoxX->setValue(static_cast<double>(rot.x));
    ui->rotSpinBoxY->setValue(static_cast<double>(rot.y));
    ui->rotSpinBoxZ->setValue(static_cast<double>(rot.z));
    ui->scaleSpinBoxX->setValue(static_cast<double>(scale.x));
    ui->scaleSpinBoxY->setValue(static_cast<double>(scale.y));
    ui->scaleSpinBoxZ->setValue(static_cast<double>(scale.z));
}


void TransformWidget::on_posSpinBoxX_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    gsl::Vector3D pos = Engine::getInstance()->mTransformSystem->getLocalPosition(eID);
    Engine::getInstance()->mTransformSystem->setPosition(eID, gsl::Vector3D(static_cast<float>(arg1),pos.y,pos.z ));
}
void TransformWidget::on_posSpinBoxY_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    gsl::Vector3D pos = Engine::getInstance()->mTransformSystem->getLocalPosition(eID);
    Engine::getInstance()->mTransformSystem->setPosition(eID, gsl::Vector3D(pos.x,static_cast<float>(arg1),pos.z ));
}
void TransformWidget::on_posSpinboxZ_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    gsl::Vector3D pos = Engine::getInstance()->mTransformSystem->getLocalPosition(eID);
    Engine::getInstance()->mTransformSystem->setPosition(eID, gsl::Vector3D(pos.x,pos.y, static_cast<float>(arg1)));
}

void TransformWidget::on_rotSpinBoxX_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    Engine::getInstance()->mTransformSystem->setRotation(eID, gsl::Vector3D(static_cast<float>(arg1), ui->rotSpinBoxY->value(), ui->rotSpinBoxZ->value()));
}
void TransformWidget::on_rotSpinBoxY_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    Engine::getInstance()->mTransformSystem->setRotation(eID, gsl::Vector3D(ui->rotSpinBoxX->value(), static_cast<float>(arg1), ui->rotSpinBoxZ->value()));
}
void TransformWidget::on_rotSpinBoxZ_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    Engine::getInstance()->mTransformSystem->setRotation(eID, gsl::Vector3D(ui->rotSpinBoxX->value(), ui->rotSpinBoxY->value(), static_cast<float>(arg1)));
}

void TransformWidget::on_scaleSpinBoxX_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    gsl::Vector3D scale = Engine::getInstance()->mTransformSystem->getLocalScale(eID);
    Engine::getInstance()->mTransformSystem->setScale(eID, gsl::Vector3D(static_cast<float>(arg1), scale.y, scale.z));
}
void TransformWidget::on_scaleSpinBoxY_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    gsl::Vector3D scale = Engine::getInstance()->mTransformSystem->getLocalScale(eID);
    Engine::getInstance()->mTransformSystem->setScale(eID, gsl::Vector3D(scale.x,static_cast<float>(arg1), scale.z));
}
void TransformWidget::on_scaleSpinBoxZ_valueChanged(double arg1)
{
    int id = mMainWindow->getActiveItemID();
    if(id<0) {return;}
    unsigned eID = static_cast<unsigned>(id);
    gsl::Vector3D scale = Engine::getInstance()->mTransformSystem->getLocalScale(eID);
    Engine::getInstance()->mTransformSystem->setScale(eID, gsl::Vector3D(scale.x,scale.y, static_cast<float>(arg1)));
}


