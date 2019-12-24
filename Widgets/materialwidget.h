#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include "Widgets/widgetbase.h"

namespace Ui {
class MaterialWidget;
}

/** A Widget class for editing an entity's MaterialComponent.
 */
class MaterialWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit MaterialWidget(MainWindow *inMainWindow = nullptr);
    virtual ~MaterialWidget() override;
    virtual void updateData() override;

private slots:
    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_comboBox_activated(int index);

    void on_comboBox_2_activated(const QString &arg1);

    void on_loadTextureButton_clicked();

    void on_shininessSpinBox_valueChanged(int arg1);

    void on_ambientSpinboxX_valueChanged(double arg1);

    void on_ambientSpinboxY_valueChanged(double arg1);

    void on_ambientSpinboxZ_valueChanged(double arg1);

    void on_specularSpinBoxX_valueChanged(double arg1);

    void on_specularSpinBoxY_valueChanged(double arg1);

    void on_specularSpinBoxZ_valueChanged(double arg1);

private:
    Ui::MaterialWidget *ui;
};

#endif // MATERIALWIDGET_H
