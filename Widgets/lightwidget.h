#ifndef LIGHTWIDGET_H
#define LIGHTWIDGET_H

#include "Widgets/widgetbase.h"

namespace Ui {
class LightWidget;
}
/** A Widget for altering light properties.

 */
class LightWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit LightWidget(MainWindow *inMainWindow, QWidget *parent = nullptr);
    virtual ~LightWidget();
    virtual void updateData() override;

private slots:
    void on_ambientXSpinbox_valueChanged(double arg1);

    void on_ambientYSpinbox_valueChanged(double arg1);

    void on_ambientZSpinbox_valueChanged(double arg1);

    void on_diffuseXSpinbox_valueChanged(double arg1);

    void on_diffuseYSpinbox_valueChanged(double arg1);

    void on_diffuseZSpinbox_valueChanged(double arg1);

    void on_specularXSpinbox_valueChanged(double arg1);

    void on_specularYSpinbox_valueChanged(double arg1);

    void on_specularZSpinbox_valueChanged(double arg1);

private:
    Ui::LightWidget *ui;
};

#endif // LIGHTWIDGET_H
