#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <memory>
#include "Systems/rendersystem.h"
#include "Components/meshcomponent.h"
/** Different types of object prefabs that can easily be spawned.
 */
enum class OBJECTS
{
    CUBE,
    BALL,
    MONKEY,
    BILLBOARD,
    ENTITY,
    AXISOBJECT,
    SKYBOX,
    LIGHT,
    CAMERA,
    LENGTH
};

enum class RENDERTYPE;
enum class COMPONENTS;

struct Mesh;

class Vertex;
class MainWindow;
class BaseComponent;
class TransformComponent;
class CollisionComponent;
class MeshComponent;
class AudioComponent;
class PhysicsComponent;
class MaterialComponent;
class ScriptComponent;
class CameraComponent;
class InputComponent;
class LandscapeComponent;
class LightComponent;
/** Holds pointers to all the components in the project. Also works as a resource manager for meshes and textures.
 */
class ObjectManager : protected QOpenGLFunctions_4_1_Core
{
public:
    ObjectManager(MainWindow* inMainWindow);

    /** Spawns an object.
     * @param OBJ: the object to spawn.
     * @param inShader: the shader to be used for the object. Can be ignored if the object is not renderable, and therefore spawns without a material.
     */
    unsigned spawnObject(OBJECTS OBJ, Shader* inShader, QString inName);

    /** Makes a new component, or returns existing one if it exists.
     * @param component type to make.
     * @param inID owner's id.
     * @return return the newly made component, or the existing one if found.
     */
    BaseComponent* addComponent(COMPONENTS comp, unsigned inID);

    /** Sets up a parent-child relationship between two entities.
     * @param parentID: ID of the parent entity.
     * @param childID: ID of the child entity.
     */
    void addChild(unsigned parentID, unsigned childID);

    /** Gives an entity a new mesh.
     * @param inFileName: The filename of the new mesh.
     * @param inID: ID of the entity that will recieve the new mesh.
     */
    void changeMesh(std::string inFileName,unsigned inID);

    /** Gets all components belonging to an entity.
     * @param inID: the ID of the entity we want the conponents.
     * @return Returns a vector with all components belonging to an entity. Will have a space for all components,
     * but if an entity doesnt have a component in that slot, it will be a nullptr.
     */
    std::vector<BaseComponent*> getAllComponentsWithID(unsigned inID);

    /** A static function that writes a value from the OBJECTS enum class into a string.
       Used frequently for UI.
     * @param OBJ: the comp type you want as a string.
     * @return the name of the component type in string form.
     */
    static QString OBJTypeToString(OBJECTS OBJ);

    /** Marks an entity for deletion by adding it to entitiesToDelete vector.
        Actual deletion happens at the end of the frame.
       @param inID: The entity we want to delete.
     */
    void markForDeletion(unsigned inID);

    /** Deletes an entity.
       Deletes the components for the entity, but leaves empty spaces for them in the vectors.
     * @param inID: Entity to delete.
     */
    void deleteMarkedObjects();

    /** Deletes all entities in the engine.
       Also clears the component vectors entirely.
     */
    void clearEntities();

    /** Returns a list of names for all the textures currently loaded in the engine.
     * @return A list of names for all meshes currently loaded in the engine.
     */
    QStringList getAllMeshNames();

    /** Returns a list of names for all the textures currently loaded in the engine.
       @return A list of names for all textures currently loaded in the engine.
     */
    QStringList getAllTextureNames();

    /** Returns the mesh with index filename from MeshMap.
       If no mesh with that filename is loaded, it will call loadMesh.
       @param fileName: Filename of mesh to find.
       @return pointer to the Mesh.
     */
    Mesh* getMesh(std::string fileName);

    /** Returns the texture with index filename from TextureMap.
       If no mesh with that filename is loaded, it will call loadTexture.
       @param fileName: Filename of Texture to find.
       @return pointer to the Texture.
     */
    Texture *getTexture(std::string fileName);

    /** Removes all children for all entities.
     */
    void removeAllChildren();

    MainWindow* mMainWindow = nullptr;

    ///Holds the names for all the entities in the engine. Can be indexed by entityID.
    std::vector<QString> mNames;
    ///Holds all MeshComponents. Can be indexed by entityID.
    std::vector<MeshComponent*> meshComps;
    ///Holds all TransformComponent. Can be indexed by entityID.
    std::vector<TransformComponent*> transformComps;
    ///Holds all AudioComponent. Can be indexed by entityID.
    std::vector<AudioComponent*> audioComps;
    ///Holds all CollisionComponent. Can be indexed by entityID.
    std::vector<CollisionComponent*> collisionComps;
    ///Holds all PhysicsComponent. Can be indexed by entityID.
    std::vector<PhysicsComponent*> physicsComps;
    ///Holds all ScriptComponent. Can be indexed by entityID.
    std::vector<ScriptComponent*> scriptComps;
    ///Holds all CameraComponent. Can be indexed by entityID.
    std::vector<CameraComponent*> cameraComps;
    ///Holds all InputComponent. Can be indexed by entityID.
    std::vector<InputComponent*> inputComps;
    ///Holds all LandscapeComponent. Can be indexed by entityID.
    std::vector<LandscapeComponent*> landscapeComps;
    ///Holds all MaterialComponent. Can be indexed by entityID.
    std::vector<MaterialComponent*> materials;
    ///Holds all LightComponent. Can be indexed by entityID.
    std::vector<LightComponent*> lightComps;


private:
    /** Loads a new mesh into the game engine.
       @param fileName: the mesh to load, without the entire filepath.
       @return Pointer to the loaded mesh.
     */
    Mesh* loadMesh(std::string fileName);

    /** Loads a new texture into the game engine.
       @param fileName: the texture to load, without the entire filepath.
       @return Pointer to the loaded texture.
     */
    Texture *loadTexture(std::string fileName);

    /** Returns vertices and indices for a mesh that can be used as a skybox.
       @return A pair of vectors, the first with vertices, the other with indices
     */
    std::pair<std::vector<Vertex>, std::vector<unsigned int>> getSkyboxVertices();

    /** Returns vertices and indices for a mesh that can be used as a light.
       @return A pair of vectors, the first with vertices, the other with indices
     */
    std::pair<std::vector<Vertex>, std::vector<unsigned int>> getLightVertices();

    /** Returns vertices and indices for a mesh that can be used as a camera frustum.
       @return A vector with the vertices
     */
    std::pair<std::vector<Vertex>, std::vector<unsigned> > getCameraFrustum();

    ///Holder for all the textures currently loaded in the engine, with their filename as a string.
    std::unordered_map<std::string, Texture*> textureMap;

    ///Holder for all the meshes currently loaded in the engine, with their filename as a string.
    std::unordered_map<std::string, Mesh*> meshMap;

    ///List of entities to be deleted on end of frame.
    std::vector<unsigned> entitiesToDelete;

    /// The id to give to the next entity that will spawn. Will also be equal to number of entities in the engine.
    unsigned int idIncrementer =0;


};

#endif // OBJECTMANAGER_H
