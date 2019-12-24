#include "materialwidget.h"
#include "ui_materialwidget.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Systems/rendersystem.h"
#include "Rendering/mainwindow.h"
#include "Rendering/texture.h"
#include "Components/materialcomponent.h"

#include <QFileDialog>
MaterialWidget::MaterialWidget(MainWindow *inMainWindow) :
    WidgetBase(inMainWindow),
    ui(new Ui::MaterialWidget)
{
    ui->setupUi(this);
}

MaterialWidget::~MaterialWidget()
{
    delete ui;
}

void MaterialWidget::updateData()
{

    MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[mMainWindow->getActiveItemID()];
    if(!m) return;

    ui->ambientSpinboxX->setValue(static_cast<double>(m->mAmbientColor.x));
    ui->ambientSpinboxY->setValue(static_cast<double>(m->mAmbientColor.y));
    ui->ambientSpinboxZ->setValue(static_cast<double>(m->mAmbientColor.z));
    ui->doubleSpinBox->setValue(static_cast<double>(m->mDiffuseColor.x));
    ui->doubleSpinBox_2->setValue(static_cast<double>(m->mDiffuseColor.y));
    ui->doubleSpinBox_3->setValue(static_cast<double>(m->mDiffuseColor.z));
    ui->specularSpinBoxX->setValue(static_cast<double>(m->mSpecularColor.x));
    ui->specularSpinBoxY->setValue(static_cast<double>(m->mSpecularColor.y));
    ui->specularSpinBoxZ->setValue(static_cast<double>(m->mSpecularColor.z));

    ui->shininessSpinBox->setValue(m->mShininess);

    QString currentItem = QString::fromStdString(m->mTextureFile);

    ui->comboBox_2->clear();
    QStringList texNames = Engine::getInstance()->mObjectManager->getAllTextureNames();
    ui->comboBox_2->addItems(texNames);
    int id = ui->comboBox->findText(currentItem);
    if(id>=0)
    {
        ui->comboBox_2->setCurrentIndex(id);
    }
    ui->comboBox->setCurrentIndex( Engine::getInstance()->mRenderSystem->findIndex(m->mShader));
}

//ambient
void MaterialWidget::on_ambientSpinboxX_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mAmbientColor.x = static_cast<float>(arg1);
    }
}

void MaterialWidget::on_ambientSpinboxY_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mAmbientColor.y = static_cast<float>(arg1);
    }
}

void MaterialWidget::on_ambientSpinboxZ_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mAmbientColor.z = static_cast<float>(arg1);
    }
}

//diffuse
void MaterialWidget::on_doubleSpinBox_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mDiffuseColor.x = static_cast<float>(arg1);
    }
}

void MaterialWidget::on_doubleSpinBox_2_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mDiffuseColor.y = static_cast<float>(arg1);
    }
}

void MaterialWidget::on_doubleSpinBox_3_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mDiffuseColor.z = static_cast<float>(arg1);
    }
}

//specular
void MaterialWidget::on_specularSpinBoxX_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mSpecularColor.x = static_cast<float>(arg1);
    }
}

void MaterialWidget::on_specularSpinBoxY_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mSpecularColor.y = static_cast<float>(arg1);
    }
}

void MaterialWidget::on_specularSpinBoxZ_valueChanged(double arg1)
{
    if(mMainWindow->getActiveItemID() >= 0)
    {
        unsigned id = static_cast<unsigned>(mMainWindow->getActiveItemID());
        MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[id];
        m->mSpecularColor.z = static_cast<float>(arg1);
    }
}

void MaterialWidget::on_comboBox_activated(int index)
{
    MaterialComponent* m = Engine::getInstance()->mObjectManager->materials[(unsigned)mMainWindow->getActiveItemID()];
    m->mShader = Engine::getInstance()->mRenderSystem->mShaderProgram[index];
}

void MaterialWidget::on_comboBox_2_activated(const QString &arg1)
{
    //Engine::getInstance()->mObjectManager->changeTexture(arg1.toStdString(), mMainWindow->getActiveItemID());
    MaterialComponent* mat = Engine::getInstance()->mObjectManager->materials[mMainWindow->getActiveItemID()];


    mat->mTexture = Engine::getInstance()->mObjectManager->getTexture(arg1.toStdString());
    mat->mTextureFile = arg1.toStdString();
}

void MaterialWidget::on_loadTextureButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Mesh"), QString::fromStdString(gsl::assetFilePath + "Textures/"),
        tr("Bump Files(*.bmp);;All Files (*)"));
    QStringList temp = fileName.split("/");
    MaterialComponent* mComp = Engine::getInstance()->mObjectManager->materials[mMainWindow->getActiveItemID()];
    if(!mComp || temp.last() == "") return;

    mComp->mTexture = Engine::getInstance()->mObjectManager->getTexture(temp.last().toStdString());
    qDebug() << mComp->mTexture->id();
    mComp->mTextureFile = temp.last().toStdString();
    updateData();
}

void MaterialWidget::on_shininessSpinBox_valueChanged(int arg1)
{
    MaterialComponent* mComp = Engine::getInstance()->mObjectManager->materials[mMainWindow->getActiveItemID()];
    if(!mComp) return;
    mComp->mShininess = arg1;
}

