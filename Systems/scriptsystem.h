#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <QObject>
#include <QJSEngine>
#include <QJsonArray>

struct ObjectHit;
class QJsonArray;
class ScriptComponent;

/** Reads scripts for entities with ScriptComponents. Handles all communication between scripts and the engine.
 */
class ScriptSystem : public QObject
{
    Q_OBJECT

public:
    ScriptSystem();

    /** Calls the script function beginPlay() for all entities with scripts that have it.
     */
    void beginPlay();

    /** Calls the script function tick() for all entities with scripts that have it.
     */
    void tick();
    /** calls the script function onEntityCollision() on both entities defined in hit.
       Sends the ID of the other entity to the script.
       @param hit: The collision information. Has the two entities that collided.
     */
    void collision(ObjectHit hit);

    /** Calls the script function endPlay() for all entities with scripts that have it.
     */
    void endPlay();

    /** Reads the script file with the filename in ScriptComponent sC into the QJSEngine.
       First makes a QJSEngine for the ScriptComponent.
       @param sC: The ScriptComponent to read into.
     */
    void readScript(ScriptComponent* sC);

    /** Returns the id for the game's designated Trophy entity.
        Used as a getter for Q_INVOKABLE trophyID.
     */
    unsigned getTrophyID() const {return trophyID;}

    /** Sets which entity is the current trophy in the game.
        Used as a setter for Q_INVOKABLE trophyID.
       @param inID: The ID of the new trophy entity.
     */
    void setTrophyID(unsigned inID) {trophyID = inID;}

    /** Gets the ID of the current player entity.
       Can be called from scripts.
    */
    Q_INVOKABLE int getPlayerID();

    /** Sets the ID of the current player entity.
       Can be called from scripts.
    */
    Q_INVOKABLE void setPlayerID(int inID);
    Q_INVOKABLE void playSound(int inID);
    Q_INVOKABLE void debug(QString string);
    Q_INVOKABLE void addInput(unsigned inID, QJsonArray moveArr);
    Q_INVOKABLE void setVisibility(bool value, unsigned inID);
    Q_INVOKABLE void setPosition(QJsonArray inPos, unsigned inID);
    Q_INVOKABLE QJsonArray getPosition(unsigned inID);
    Q_INVOKABLE int randInRange(int min, int max);

    Q_PROPERTY(unsigned trophyID READ getTrophyID WRITE setTrophyID)
private:
    /** Calls a function in a given script.
       @param sC: the script component that holds the script.
       @param functionName: the name of the script function we want to call.
     */
    void callJSFunction(ScriptComponent *sC, QString functionName);


    /** Calls a function in a given script, with arguments.

       @param sC: The script component that holds the script.
       @param functionName: The name of the script function we want to call.
       @param args: The arguments to give the function.
     */
    void callJSFunction(ScriptComponent *sC, QString functionName, QJSValueList args);

    ///
    unsigned trophyID= 0.f;
};

#endif // SCRIPTSYSTEM_H
