#ifndef PHYSICSWIDGET_H
#define PHYSICSWIDGET_H

#include "Widgets/widgetbase.h"

namespace Ui {
class PhysicsWidget;
}
/** A Widget for editing an entity's PhysicsComponent.
 */
class PhysicsWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit PhysicsWidget(MainWindow* inMainWindow, QWidget *parent = nullptr);
    ~PhysicsWidget();
    virtual void updateData() override;
private slots:
    void on_speedSpinBox_valueChanged(double arg1);

private:
    Ui::PhysicsWidget *ui;
};

#endif // PHYSICSWIDGET_H
