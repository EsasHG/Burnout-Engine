#include "meshwidget.h"
#include "ui_meshwidget.h"

#include <QFileDialog>

#include "Rendering/mainwindow.h"
#include "Components/meshcomponent.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
MeshWidget::MeshWidget(MainWindow* inMainWindow, QWidget *parent) :
    WidgetBase(inMainWindow, parent),
    ui(new Ui::MeshWidget)
{
    ui->setupUi(this);

    ui->comboBox->clear();
}

MeshWidget::~MeshWidget()
{
    delete ui;
}

void MeshWidget::updateData()
{

    //had a crash here
    if(mMainWindow->getActiveItemID() <=0)
    { return; }
    unsigned temp = static_cast<unsigned>(mMainWindow->getActiveItemID());
    MeshComponent* m = Engine::getInstance()->mObjectManager->meshComps[temp];
    if(!m) return;
    QString currentItem = QString::fromStdString(m->fileName);


    ui->comboBox->clear();
    QStringList meshNames = Engine::getInstance()->mObjectManager->getAllMeshNames();
    ui->comboBox->addItems(meshNames);
    int id = ui->comboBox->findText(currentItem);
    if(id>=0)
    {
        ui->comboBox->setCurrentIndex(id);
    }
}

void MeshWidget::on_comboBox_activated(const QString &arg1)
{
    Engine::getInstance()->mObjectManager->changeMesh(arg1.toStdString(),
                                                      mMainWindow->getActiveItemID());
}

void MeshWidget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Mesh"), QString::fromStdString(gsl::assetFilePath + "Meshes/"),
        tr("Object Files(*.obj);;Text Files(*.txt);;All Files (*)"));
    QStringList temp = fileName.split("/");

    MeshComponent* mComp = Engine::getInstance()->mObjectManager->meshComps[mMainWindow->getActiveItemID()];
    if(!mComp || temp.last() == "") return;

    mComp->mesh = Engine::getInstance()->mObjectManager->getMesh(temp.last().toStdString());
    //mComp->mesh = Engine::getInstance()->mObjectManager->getMesh(fileName.toStdString());
    mComp->fileName = temp.last().toStdString();
    updateData();
}
