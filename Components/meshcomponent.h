#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "basecomponent.h"
#include "vertex.h"

/** Enum with all the different possible ways a mesh can be rendered.
 */
enum class RENDERTYPE
{
    TRIANGLES,
    LINES,
    TRIANGLE_STRIP,
    POINTS
};

/** Holds the vertices and indices, as well as the VAO, VBO and EAB and rendertype for a mesh.
 */
struct Mesh : public QOpenGLFunctions_4_1_Core
{
    Mesh() {}
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    RENDERTYPE renderType = RENDERTYPE::TRIANGLES;
    ~Mesh()
    {
        initializeOpenGLFunctions();
        glDeleteVertexArrays( 1, &mVAO );
        glDeleteBuffers( 1, &mVBO );
    }
};

/** Component that holds a pointer to the mesh for this entity.
   Also has a bool to toggle visibility.
 */
class MeshComponent : public BaseComponent
{
public:
    MeshComponent(unsigned inID) : BaseComponent(inID) {}
    virtual ~MeshComponent() override   {}
    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;
    bool bIsVisible = true;
    Mesh* mesh = nullptr;
    std::string fileName = "";
};

#endif // MESHCOMPONENT_H
