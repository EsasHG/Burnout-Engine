#ifndef SCRIPTWIDGET_H
#define SCRIPTWIDGET_H

#include "Widgets/widgetbase.h"

namespace Ui {
class ScriptWidget;
}
/** A Widget that lets you give entities scripts on runtime.
 */
class ScriptWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit ScriptWidget(class MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual ~ScriptWidget() override;
    virtual void updateData() override;

private slots:
    void on_ReadFile_button_clicked();

private:
    Ui::ScriptWidget *ui;
};

#endif // SCRIPTWIDGET_H
