#include "innpch.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDesktopWidget>
#include <QTreeWidget>
#include <QModelIndex>
#include <QFileDialog>

#include "Rendering/renderwindow.h"
#include "Systems/engine.h"
#include "Systems/inputsystem.h"
#include "Systems/scenemanager.h"
#include "Components/transformcomponent.h"
#include "Widgets/landscapewidget.h"
#include "Widgets/addcomponentwidget.h"
#include "Widgets/transformwidget.h"
#include "Widgets/soundwidget.h"
#include "Widgets/meshwidget.h"
#include "Widgets/materialwidget.h"
#include "Widgets/scriptwidget.h"
#include "Widgets/camerawidget.h"
#include "Widgets/physicswidget.h"
#include "Widgets/lightwidget.h"
#include "GSL/vector3d.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    //ui->verticalWidget->hide();
//    ui->treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->viewport()->setAcceptDrops(true);
    ui->treeWidget->setDropIndicatorShown(true);
    ui->treeWidget->setDragDropMode(QAbstractItemView::InternalMove);

    componentWidgets.resize(static_cast<int>(COMPONENTS::LENGTH));
    componentWidgets[static_cast<int>(COMPONENTS::MESH)] = new MeshWidget(this);
    componentWidgets[static_cast<int>(COMPONENTS::TRANSFORM)] = new TransformWidget(this);
    componentWidgets[static_cast<int>(COMPONENTS::SOUND)] = new SoundWidget(this);
    componentWidgets[static_cast<int>(COMPONENTS::MATERIAL)] = new MaterialWidget(this);
    componentWidgets[static_cast<int>(COMPONENTS::SCRIPT)] = new ScriptWidget(this);
    componentWidgets[static_cast<int>(COMPONENTS::CAMERA)] = new CameraWidget(this);
    componentWidgets[static_cast<int>(COMPONENTS::PHYSICS)] = new PhysicsWidget(this);
    componentWidgets[static_cast<int>(COMPONENTS::LIGHT)] = new LightWidget(this);

    mAddComponentWidget = new AddComponentWidget(this);
    mLandscapeWidget = new LandscapeWidget(this);
//    ui->componentWidget->addWidget(mTransformWidget);
//    ui->componentWidget->addWidget(mSoundWidget);
    ui->verticalLayout_5->addWidget(mAddComponentWidget);
    ui->verticalLayout_5->addWidget(mLandscapeWidget);
//    ui->scrollArea->addScrollBarWidget(mTransformWidget, Qt::AlignTop);
//    ui->scrollArea->addScrollBarWidget(mSoundWidget, Qt::AlignTop);


}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::updateDetailWindow()
{
    std::vector<BaseComponent*>comps = Engine::getInstance()->mObjectManager->getAllComponentsWithID(activeItem->text(0).toInt());
    ui->widget->show();
    //hides all component windows
    for(int i=0; i<static_cast<int>(COMPONENTS::LENGTH); i++)
        if(componentWidgets[i])
        {
            componentWidgets[i]->hide();
            ui->componentWidget->removeWidget(componentWidgets[i]);
        }

    //Shows the ones where a component exists
    for(int i=0; i<static_cast<int>(COMPONENTS::LENGTH); i++)
    {
        if(comps[i] && componentWidgets[i])
        {
            componentWidgets[i]->show();
            ui->componentWidget->addWidget(componentWidgets[i]);
        }
    }
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this...)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types that OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Set the number of samples used for multisampling
    format.setSamples(8);

    //Turn off VSync. If this is set to 1, VSync is on - default behaviour
    format.setSwapInterval(0);

    //Just prints out what OpenGL format we will get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    //Set size of program in % of available screen
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

void MainWindow::setChildUI(unsigned parentID, unsigned childID)
{

    QTreeWidgetItem* parent  = findItemWithID(parentID);

    if(parent)
    {
        QTreeWidgetItem* child = findItemWithID(childID);
        if(child)
        {
            ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(child));
            parent->addChild(child);
        }
        else
            qDebug() << "[MainWindow::setChildUI]: Child " << childID << " doesnt seem to exist";
    }
    else
    {
        qDebug() << "[MainWindow::setChildUI]: Parent " << parentID << " doesnt seem to exist";
    }
}

QTreeWidgetItem *MainWindow::findItemWithID(unsigned inID)
{
    for(unsigned i=0; i<ui->treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* item = RecursiveFindItemWithID(inID, ui->treeWidget->topLevelItem(i));
        if(item)
        {
            return item;
        }
    }
    return nullptr;
}

QTreeWidgetItem *MainWindow::RecursiveFindItemWithID(unsigned inID, QTreeWidgetItem* startItem)
{
    if(startItem->text(0).toInt() == inID)
    {
        return startItem;
    }
    else
    {
        for(int i = 0; i<startItem->childCount(); i++)
        {
             QTreeWidgetItem* item = RecursiveFindItemWithID(inID,startItem->child(i));
             if(item)
                return item;
        }
    }
}


void MainWindow::selectItem(unsigned inID)
{
    QTreeWidgetItem* temp = findItemWithID(inID);
    if(temp)
        on_treeWidget_itemClicked(temp,0);
}

void MainWindow::on_pushButton_clicked()
{
    ui->dockWidget->setVisible(false);
    ui->dockWidget_2->setVisible(false);
    ui->detailDockWidget->setVisible(false);

    mRenderWindowContainer->setFocus();
    mRenderWindow->startGame();
}


void MainWindow::on_StopGameButton_clicked()
{
    mRenderWindow->stopGame();
    ui->dockWidget->setVisible(true);
    ui->dockWidget_2->setVisible(true);
    ui->detailDockWidget->setVisible(true);
}


void MainWindow::on_pushButton_3_clicked()
{
    Engine::getInstance()->mObjectManager->spawnObject(
                    static_cast<OBJECTS>(comboBoxInt),
                    Engine::getInstance()->mRenderSystem->mShaderProgram[0], ObjectManager::OBJTypeToString(static_cast<OBJECTS>(comboBoxInt)));
}

void MainWindow::addObjectToUI(QString objName, unsigned inID)
{
    QTreeWidgetItem* wI = new QTreeWidgetItem;
    wI->setText(1,objName);
    wI->setText(0, QString::number(inID)); // .toInt()
//    wI->setFlags(wI->flags() | Qt::ItemIsEditable);
    ui->treeWidget->addTopLevelItem(wI);
}

void MainWindow::on_comboBox_activated(int index)
{
    comboBoxInt = index;
}

void MainWindow::resetTreeWidget()
{
    std::shared_ptr<ObjectManager> objManager = Engine::getInstance()->mObjectManager;
    ui->treeWidget->clear();
    for(unsigned i=0; i<objManager->mNames.size(); i++)
    {
        if(objManager->mNames[i] != "")
        {
            addObjectToUI(objManager->mNames[i], i);
        }
    }
    std::vector<TransformComponent*> tComps = Engine::getInstance()->mObjectManager->transformComps;
    for(auto& t : tComps)
    {
        if(t && t->mParent >=0)
            setChildUI(t->mParent, t->eID);
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(activeItem)
    {
        qDebug() << "Deleting Object!";
        Engine::getInstance()->mObjectManager->markForDeletion(getActiveItemID());
        activeItem = nullptr;
        //ui->treeWidget->indexFro(activeItem);
        //clear qtreewidget and add everything again
    }
}

void MainWindow::recursiveSetChildren(QTreeWidgetItem* item)
{
    for(unsigned int j=0; j<item->childCount();j++)
    {
        Engine::getInstance()->mObjectManager->addChild(item->text(0).toInt(),item->child(j)->text(0).toInt());

        recursiveSetChildren(item->child(j));
    }
}

void MainWindow::updateData()
{

    mAddComponentWidget->updateData();
    mLandscapeWidget->updateData();
    updateDetailWindow();
    for (auto& w : componentWidgets)
    {
        if(w)
            w->updateData();
        //findItemWithID(1)->parent()->removeChild(findItemWithID(1));
    }

}

void MainWindow::onLoadScene()
{
    ui->treeWidget->clear();
    activeItem = nullptr;
    //updateDetailWindow();
    ui->widget->hide();
}

int MainWindow::getActiveItemID()
{
    if(activeItem)
        return activeItem->text(0).toInt();
    return -1;
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    activeItem = item;
    Engine::getInstance()->mInputSystem->possessedEntity = getActiveItemID();
    //updates parent-child relationships in code
    //could be wrong if some children or parens dont have transformcomponents
    Engine::getInstance()->mObjectManager->removeAllChildren();
    for(unsigned int i=0; i<ui->treeWidget->topLevelItemCount(); i++)
    {
        recursiveSetChildren(ui->treeWidget->topLevelItem(i));
    }
    ui->lineEdit->setText(activeItem->text(1));
    updateData();
}

void MainWindow::on_actionTurn_On_toggled(bool arg1)
{
    mRenderWindow->toggleWireframe(arg1);
}

void MainWindow::on_actionLoad_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Scene"), gsl::sceneFilePath,
        tr("Json File(*.json);;All Files (*)"));
    qDebug() << fileName;
    Engine::getInstance()->mSceneManager->loadScene(fileName);

}

void MainWindow::on_actionSave_File_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Load Scene"), gsl::sceneFilePath,
        tr("Json File(*.json);;All Files (*)"));
    Engine::getInstance()->mSceneManager->saveScene(fileName);
    qDebug() << fileName;
}

void MainWindow::on_lineEdit_editingFinished()
{
    if(getActiveItemID() >=0)
    {
        if(ui->lineEdit->text().length())
        {
            activeItem->setText(1, ui->lineEdit->text());
            if(getActiveItemID() < Engine::getInstance()->mObjectManager->mNames.size())
                Engine::getInstance()->mObjectManager->mNames[getActiveItemID()] = ui->lineEdit->text();

        }
        else
        {
            ui->lineEdit->text() = activeItem->text(1);
        }
    }
}
