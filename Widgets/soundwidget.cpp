#include "soundwidget.h"
#include <QTreeWidgetItem>
#include "ui_soundwidget.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Systems/transformsystem.h"
#include "Components/audiocomponent.h"
#include "Rendering/mainwindow.h"
#include "Sound/soundmanager.h"
SoundWidget::SoundWidget(MainWindow* inMainWindow,QWidget *parent) :
    WidgetBase(inMainWindow, parent),  ui(new Ui::SoundWidget)
{
    ui->setupUi(this);
}

SoundWidget::~SoundWidget()
{
    delete ui;
}

void SoundWidget::updateData()
{
    AudioComponent* audioComp = Engine::getInstance()->mObjectManager->audioComps[mMainWindow->getActiveItemID()];
    auto objM = Engine::getInstance()->mObjectManager;
    audioComp = objM->audioComps[mMainWindow->getActiveItemID()];
    if(!audioComp)
    {
        return;
    }
    ui->lineEdit->setText(audioComp->filename);
    ui->loopCheckBox->setChecked(audioComp->mShouldLoop);
    ui->gainSpinBox->setValue(audioComp->mGain);
}

void SoundWidget::on_pushButton_clicked()
{
    if(mMainWindow->getActiveItemID()<0) {return;}
    AudioComponent* audioComp = Engine::getInstance()->mObjectManager->audioComps[mMainWindow->getActiveItemID()];
    if(!audioComp) { qDebug() << "no audio component!!!"; return;}
    if(ui->lineEdit->text() == "") { qDebug() << "no file to load!!!"; return;}
    if(audioComp->mSoundSource)
    {
        audioComp->mSoundSource->stop();
        delete audioComp->mSoundSource;
        audioComp->mSoundSource = nullptr;
    }
    //remakes soundSource every time a new file is read... unneccesary?
    audioComp->mSoundSource = Engine::getInstance()->mSoundManager->createSource(
                 ui->lineEdit->text().toStdString(),
                Engine::getInstance()->mTransformSystem->getWorldPosition(mMainWindow->getActiveItemID()),
                gsl::assetFilePath + "Sounds/" + ui->lineEdit->text().toStdString(),
                ui->loopCheckBox->isChecked(),
                ui->gainSpinBox->value());
    audioComp->filename =  ui->lineEdit->text();

}

void SoundWidget::on_playButton_clicked()
{
    AudioComponent* audioComp = Engine::getInstance()->mObjectManager->audioComps[mMainWindow->getActiveItemID()];
    if(audioComp && audioComp->mSoundSource)
        audioComp->mSoundSource->play();
}

void SoundWidget::on_pauseButton_clicked()
{
    AudioComponent* audioComp = Engine::getInstance()->mObjectManager->audioComps[mMainWindow->getActiveItemID()];
    if(audioComp && audioComp->mSoundSource)
        audioComp->mSoundSource->pause();
}

void SoundWidget::on_stopButton_clicked()
{
    AudioComponent* audioComp = Engine::getInstance()->mObjectManager->audioComps[mMainWindow->getActiveItemID()];
    if(audioComp && audioComp->mSoundSource)
        audioComp->mSoundSource->stop();
}
