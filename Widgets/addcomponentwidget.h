#ifndef ADDCOMPONENTWIDGET_H
#define ADDCOMPONENTWIDGET_H

#include <QWidget>
#include "Widgets/widgetbase.h"
namespace Ui {
class AddComponentWidget;
}
class MainWindow;
/** A Widget for adding components to an entity in runtime.
 */
class AddComponentWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit AddComponentWidget(class MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual ~AddComponentWidget() override;

    virtual void updateData() override;
private slots:
    void on_pushButton_clicked();

    void on_comboBox_activated(int index);

private:
    Ui::AddComponentWidget *ui;
    std::vector<QString> componentStrings;

};

#endif // ADDCOMPONENTWIDGET_H
