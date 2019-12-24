#include "physicswidget.h"
#include "ui_physicswidget.h"

#include "Rendering/mainwindow.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Components/physicscomponent.h"

PhysicsWidget::PhysicsWidget(MainWindow *inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow, parent),
    ui(new Ui::PhysicsWidget)
{
    ui->setupUi(this);
}

PhysicsWidget::~PhysicsWidget()
{
    delete ui;
}

void PhysicsWidget::updateData()
{
    if(mMainWindow->getActiveItemID() <0) {return;}
    unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());

    PhysicsComponent* p = Engine::getInstance()->mObjectManager->physicsComps[id];
    if(p)
        ui->speedSpinBox->setValue(static_cast<double>(p->speed));
}

void PhysicsWidget::on_speedSpinBox_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() <0) {return;}
    unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());

    PhysicsComponent* p = Engine::getInstance()->mObjectManager->physicsComps[id];
    if(p)
        p->speed = static_cast<float>(arg1);
}
