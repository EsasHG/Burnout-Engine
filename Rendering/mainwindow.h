#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Systems/objectmanager.h"
class QWidget;
class RenderWindow;
class ObjectManager;
class QTreeWidgetItem;

class AddComponentWidget;
class LandscapeWidget;

namespace Ui {
class MainWindow;
}

/** The main window widget of the program.
    All other ui and windows are somewhere inside this one.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /** Adds a new iten to the TreeWidget.
        The item will consist of a name and an ID.
       @param objName: Name of the item.
       @param inID: ID of the item.
     */
    void addObjectToUI(QString objName, unsigned inID);

    /** Sets one TreeWidgetItem to a child of another, making it appear below it in the TreeWidget.

       @param parentID: the parent ID.
       @param childID: the child ID.
     */
    void setChildUI(unsigned parentID, unsigned childID);

    /** Looks through the TreeWidget for an item with a matching ID, and selects it if found.
       @param inID: ID to find.
     */
    void selectItem(unsigned inID);

    /** Hides widgets that are irrelevant for the currently selected entity.
        Hides all components in componentWidgets, and removes them from the window. Adds them
        back only if the selected entity has a matching component.
     */
    void updateDetailWindow();

    /** updates all UI data.
        Runs updateDetailWindow, and also updates mAddComponentWidget and mLandscapeWidget.
        Loops through the componentWidgets vector and updates runs their updateData function.
     */
    void updateData();

    /** Function for when a new scene is loaded
        The only thing it does currently is clear the TreeWidget, so it should be renamed.
     */
    void onLoadScene();

    /** Returns the ID for the currently selected entity.

       @return the ID of activeItem.
     */
    int getActiveItemID();

    /// The instance of RenderWindow.
    RenderWindow *mRenderWindow = nullptr;

    /** Clears the TreeWidget and makes the objects again.
     */
    void resetTreeWidget();
    
private slots:
    /** Slot for Play button.
        Calls RenderWindow::startGame()
     */
    void on_pushButton_clicked();

    /** Slot for Stop button.
        Calls  RenderWindow::stopGame()
     */
    void on_StopGameButton_clicked();

    /** Slot for Add button in Spawn Object window.
        Spawns the object selected in the combobox.
     */
    void on_pushButton_3_clicked();

    /** Slot for Remove button in Spawn Object window. Doesnt work properly
        Calls ObjectManager::deleteObject(), but this function is not working as intended atm.
        Does therefore not remove the entity from TreeWidget.
     */
    void on_pushButton_2_clicked();

    /** Updates comboBoxInt.
       @param index: New comboBoxInt.
     */
    void on_comboBox_activated(int index);

    /** Slot for when a TreeWidgetItem is clicked. Updates activeItem and parent-child relationships in code.
       @param item: the clicked item,
       @param column: The column clicked on. This is ignored.
     */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    /** Turns wireframe mode on or off.
       @param arg1: Whether wireframe should be on or off
     */
    void on_actionTurn_On_toggled(bool arg1);

    /** Opens a QFileDialogue window, and loads the selected JSON file.
     */
    void on_actionLoad_File_triggered();

    /** Opens a QFileDialogue window, and saves the current scene to the selected file path.

     */
    void on_actionSave_File_triggered();

    /** Updates the selected entity's name to whatever the lineEdit was edited to.
       As long as it is not empty.

     */
    void on_lineEdit_editingFinished();

private:
    /** Initializes MainWindow and creates RenderWindow.
    */
    void init();

    /** Recursive function for updating parent-child relationships in code.
        Makes sure all children of a TreeWidgetItem are set as children in their TransformComponent as well.
       @param item: Parent item.
     */
    void recursiveSetChildren(QTreeWidgetItem *item);

    /** Looks through the treeWidget for an item with matching id to inID.
       @param inID: The ID we are trying to find
       @return The found QTreeWidgetItem, or a nullptr.
     */
    QTreeWidgetItem* findItemWithID(unsigned inID);

    /** Recursive helper function for SelectItem.
        Goes through one item and all its children to see if one of them is a match.
        If it finds a match, returns it. Otherwise returns nullptr.
       @param inID: ID to find.
       @param startItem: Item to start looking at.
       @return Pointer to the item with inID as id/nullptr
     */
    QTreeWidgetItem *RecursiveFindItemWithID(unsigned inID, QTreeWidgetItem *startItem);

    Ui::MainWindow *ui = nullptr;

    /// Holds all the different component widgets, in the order of the COMPONENTS enum.
    std::vector<class WidgetBase*> componentWidgets;
    /// the currently selected QTreeWidgetItem.
    QTreeWidgetItem* activeItem = nullptr;

    /// Widget for adding components to an entity
    AddComponentWidget* mAddComponentWidget = nullptr;

    /// Widget for altering a landscape component.
    /// Not in componentWidgets cause it should exist even if an entity does not have a landscape component.
    /// See LandscapeWidget for more details.
    LandscapeWidget* mLandscapeWidget = nullptr;

    /// The current index of the Add Object combobox.
    int comboBoxInt = 0;
    QWidget *mRenderWindowContainer = nullptr;

};

#endif // MAINWINDOW_H
