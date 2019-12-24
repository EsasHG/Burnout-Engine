#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QWidget>
#include "Widgets/widgetbase.h"

namespace Ui {
class TransformWidget;
}
class MainWindow;
/** A Widget for displaying and altering an entity's transforms.
 */
class TransformWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit TransformWidget(MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual ~TransformWidget() override;

    virtual void updateData() override;
private slots:
    void on_posSpinBoxX_valueChanged(double arg1);
    void on_posSpinBoxY_valueChanged(double arg1);
    void on_posSpinboxZ_valueChanged(double arg1);

    void on_rotSpinBoxX_valueChanged(double arg1);
    void on_rotSpinBoxY_valueChanged(double arg1);
    void on_rotSpinBoxZ_valueChanged(double arg1);

    void on_scaleSpinBoxX_valueChanged(double arg1);
    void on_scaleSpinBoxY_valueChanged(double arg1);
    void on_scaleSpinBoxZ_valueChanged(double arg1);

private:
    Ui::TransformWidget *ui;
};

#endif // TRANSFORMWIDGET_H
