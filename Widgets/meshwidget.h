#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <QWidget>
#include "Widgets/widgetbase.h"

namespace Ui {
class MeshWidget;
}

/** A Widget that lets users change an entity's mesh on runtime.
 */
class MeshWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit MeshWidget(class MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual ~MeshWidget() override;
    virtual void updateData()override;
private slots:
    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::MeshWidget *ui;
};

#endif // MESHWIDGET_H
