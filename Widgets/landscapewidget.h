#ifndef LANDSCAPEWIDGET_H
#define LANDSCAPEWIDGET_H

#include "Widgets/widgetbase.h"

namespace Ui {
class LandscapeWidget;
}
/** A Widget for constructing new landscapes and changing sculpt brush settings.
 */
class LandscapeWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit LandscapeWidget(class MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual~LandscapeWidget() override;
    virtual void updateData() override;

private slots:
    /** Alters the sculpting brush size
        Updates LandscapeManager::sculptRange.
       @param arg1: new brush size.
     */
    void on_RadiusSpinBox_valueChanged(double arg1);

    /** Alters the brush sensitivity for sculpting.
       Updates LandscapeManager::sculptSensitivity.
       @param arg1. new sculptSensitivity.
     */
    void on_SensitivitySpinBox_valueChanged(double arg1);

    /** Toggles whether sculpting is allowed or not.
       @param checked: The radiobutton status. Decides if sculpting is allowed.
     */
    void on_radioButton_toggled(bool checked);

    /** Constructs a default flat landscape.
        If the entity selected has no LandscapeComponent, a new entity will be created and
        given a MeshComponent, MaterialComponent and LandscapeComponent.
     */
    void on_pushButton_clicked();

private:
    Ui::LandscapeWidget *ui;
};

#endif // LANDSCAPEWIDGET_H
