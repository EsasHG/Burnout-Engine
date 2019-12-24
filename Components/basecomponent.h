#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <QJsonObject>
#include <QJsonArray>

/** An enum of all existing component types

 */
enum class COMPONENTS
{
    MESH,
    TRANSFORM,
    SOUND,
    COLLISION,
    MATERIAL,
    PHYSICS,
    SCRIPT,
    CAMERA,
    INPUT,
    LANDSCAPE,
    LIGHT,
    LENGTH,
};

/** The base class that all components inherits from.
   Holds their id, and virtual functions for writing components to and from json.
 */
class BaseComponent
{
public:
    BaseComponent(unsigned int inID) : eID(inID) {}
    /// The entity ID. Unique for all entities. Used to find out what component an entity belongs to.
    unsigned int eID;
    virtual ~BaseComponent() {}
    /** A virtual function for writing a component into Json for saving.
      Has a default implementation because not all components need to save any data. For some, its enough to exist in Json-format.
       @return A QJsonObject with the information the component need to save.
     */
    virtual QJsonObject toJson() const {return QJsonObject();}

    /** A virtual function for reading a component from Json for loading.
      Has a default implementation because not all components need to save any data.
       @param obj: The QJsonObject to read from.
     */
    virtual void fromJson(QJsonObject obj) {obj.isEmpty();}

    /** A static function that writes a value from the COMPONENTS enum class into a string.
       Used frequently for UI.
     * @param COMP: the comp type you want as a string
     * @return the name of the component type in string form.
     */
    static QString compTypeToString(COMPONENTS COMP)
    {
        switch(COMP)
        {
        case COMPONENTS::MESH:
            return "Mesh Component";
        case COMPONENTS::TRANSFORM:
            return "Transform Component";
        case COMPONENTS::SOUND:
            return  "Audio Component";
        case COMPONENTS::COLLISION:
            return  "Collision Component";
        case COMPONENTS::MATERIAL:
            return "Material Component";
        case COMPONENTS::PHYSICS:
            return "Physics Component";
        case COMPONENTS::SCRIPT:
            return "Script Component";
        case COMPONENTS::CAMERA:
            return "Camera Component";
        case COMPONENTS::INPUT:
            return "Input Component";
        case COMPONENTS::LANDSCAPE:
            return "Landscape Component";
        case COMPONENTS::LIGHT:
            return "Light Component";


        case COMPONENTS::LENGTH:
            return QString(static_cast<int>(COMPONENTS::LENGTH));
        }
        return "0";
    }
};

#endif // BASECOMPONENT_H
