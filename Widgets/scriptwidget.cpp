#include "scriptwidget.h"
#include "ui_scriptwidget.h"
#include "Rendering/mainwindow.h"
#include "Components/scriptcomponent.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"

ScriptWidget::ScriptWidget(MainWindow *inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow,parent),
    ui(new Ui::ScriptWidget)
{
    ui->setupUi(this);
}

ScriptWidget::~ScriptWidget()
{
    delete ui;
}

void ScriptWidget::updateData()
{
    int ID = mMainWindow->getActiveItemID();
    auto scriptComp = Engine::getInstance()->mObjectManager->scriptComps[ID];
    if(scriptComp)
        ui->lineEdit->setText(scriptComp->mFileName);
}


void ScriptWidget::on_ReadFile_button_clicked()
{
    int ID = mMainWindow->getActiveItemID();
    auto scriptComp = Engine::getInstance()->mObjectManager->scriptComps[ID];
    if(scriptComp)
        scriptComp->mFileName = ui->lineEdit->text();
}
