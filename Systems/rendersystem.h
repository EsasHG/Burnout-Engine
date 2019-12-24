#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include <QOpenGLFunctions_4_1_Core>

class RenderComponent;
class MeshComponent;
class TransformComponent;
class MaterialComponent;
class LightComponent;
class Shader;
class Texture;

/** Renders all entities with both a MeshComponent and a MaterialComponent. Holds the shaders.
 */
class RenderSystem : public QOpenGLFunctions_4_1_Core
{
public:
    RenderSystem();
    ~RenderSystem();
    ///mainpage

    /** Initializes a mesh. Gives it new VBO, VAO, and EBO.
        Sends the vertex data to the gpu.
       @param mesh: The Mesh holds the vertices and indices to initialize.
     */
    void initMesh(struct Mesh *mesh);

    /** Renders a single object in the scene

       @param transformComp: Decides where in the scene the entity will be rendered.
       @param meshComp: Decides the vertices to be rendered, and how to render them.
       @param material: Holds the shader to be used, and the potential texture and color.
     */
    void draw(TransformComponent *transformComp, MeshComponent *meshComp, MaterialComponent *material);

    /** Getter function to find a shader by its index in the mShaderProgram array.
       @param inIndex: the index of the shader we want to find.
       @return the found shader
     */
    Shader* findShaderByIndex(unsigned inIndex);

    /** Finds the index of a given shader in the mShaderProgram array.
       @param inShader: The shader we are finding.
       @return The index of the shader.
     */
    int findIndex(Shader* inShader);

    /** Reinitializes a mesh. Keeps the VAO, VBO and EAB the same, but sends the vertices in again.
       @param mesh: The mesh to reinitialize.
     */
    void reinitializeMesh(Mesh *mesh);

    void changeLight(LightComponent* inNewLight);
    ///Holds all the shaders in the program
    Shader* mShaderProgram[4]{nullptr};

private:

};

#endif // RENDERSYSTEM_H
