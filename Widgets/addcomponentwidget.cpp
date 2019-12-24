#include "addcomponentwidget.h"
#include "ui_addcomponentwidget.h"

#include <QTreeWidgetItem>
#include "Components/basecomponent.h"
#include "Rendering/mainwindow.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"

AddComponentWidget::AddComponentWidget(MainWindow *inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow,parent),
    ui(new Ui::AddComponentWidget)
{
    ui->setupUi(this);
}

AddComponentWidget::~AddComponentWidget()
{
    delete ui;
}

void AddComponentWidget::on_pushButton_clicked()
{
    if(mMainWindow->getActiveItemID() < 0)
    {
        return;
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::MESH))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::MESH,
                    mMainWindow->getActiveItemID()
                    );
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::TRANSFORM))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::TRANSFORM,
                    mMainWindow->getActiveItemID()
                    );
    }

    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::SOUND))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::SOUND,
                    mMainWindow->getActiveItemID()
                    );
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::COLLISION))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::COLLISION,
                    mMainWindow->getActiveItemID()
                    );
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::MATERIAL))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::MATERIAL,
                    mMainWindow->getActiveItemID()
                    );
    }

    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::PHYSICS))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::PHYSICS,
                    mMainWindow->getActiveItemID()
                    );
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::SCRIPT))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::SCRIPT,
                    mMainWindow->getActiveItemID()
                    );
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::CAMERA))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::CAMERA,
                    mMainWindow->getActiveItemID()
                    );
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::INPUT))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::INPUT,
                    mMainWindow->getActiveItemID()
                    );
    }
    if(ui->comboBox->currentText() == BaseComponent::compTypeToString(COMPONENTS::LIGHT))
    {
        Engine::getInstance()->mObjectManager->addComponent(
                    COMPONENTS::LIGHT,
                    mMainWindow->getActiveItemID()
                    );
    }
    mMainWindow->updateData();
}


void AddComponentWidget::on_comboBox_activated(int index)
{

}

void AddComponentWidget::updateData()
{
    ui->comboBox->clear();
    std::vector<BaseComponent*> components =
            Engine::getInstance()->mObjectManager->getAllComponentsWithID(
        mMainWindow->getActiveItemID()
                );
    ui->comboBox->clear();

    for(int i=0; i< static_cast<int>(COMPONENTS::LENGTH); i++)
    {
        if(!components[i])
            ui->comboBox->addItem(BaseComponent::compTypeToString(static_cast<COMPONENTS>(i)));
    }
}
