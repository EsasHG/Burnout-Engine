#ifndef SOUNDWIDGET_H
#define SOUNDWIDGET_H

#include <QWidget>
#include "Widgets/widgetbase.h"

namespace Ui {
class SoundWidget;
}
class MainWindow;
/** A Widget that lets you upload sound effects to entities.
 */
class SoundWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit SoundWidget(MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual~SoundWidget() override;

    virtual void updateData() override;

private slots:
    void on_pushButton_clicked();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::SoundWidget *ui;
};

#endif // SOUNDWIDGET_H
