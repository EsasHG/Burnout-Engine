#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

class MainWindow;

class LandscapeComponent;
/** Takes care of saving and loading scenes.
 */
class SceneManager
{
public:
    SceneManager(MainWindow *inMainWindow);

    /** Deletes all entities in the engine, and loads new one from a JSON file.

       @param filePath: The entire filepath of the file to load.
       @return true if successful, false if not.
     */
    bool loadScene(QString filePath);

    /** Writes all the entities in the scene into a JSON file that can be read with loadScene.
       @param fileName: the name the file will be saved as. Needs the entire filepath.
     */
    void saveScene(QString fileName);



    void saveHeightMap(QString fileName, LandscapeComponent *lComp) const;

    ///The name of the scene. Taken from saveScene and loadScene functions.
    QString sceneName = "";
private:
    ///pointer to the main window of the application.
    MainWindow* mMainWindow;
};

#endif // SCENEMANAGER_H
