#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include <QWidget>
/** A virtual base class that all other custom widgets inherit from.
 */
class WidgetBase : public QWidget
{
public:
    WidgetBase(class MainWindow* inMainWindow, QWidget *parent = nullptr);
    virtual ~WidgetBase() {}
    MainWindow* mMainWindow = nullptr;

    /** Updates the data in the widget to match the current entity.
     */
    virtual void updateData()=0;

};

#endif // WIDGETBASE_H
