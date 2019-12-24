#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include "Widgets/widgetbase.h"

namespace Ui {
class CameraWidget;
}
/** A Widget for editing an entity's CameraComponent.
 */
class CameraWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit CameraWidget(MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual ~CameraWidget() override;
    virtual void updateData() override;

private slots:
    void on_checkBox_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::CameraWidget *ui;
};

#endif // CAMERAWIDGET_H
