#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

class MeshComponent;
class LandscapeComponent;
/** Is responsible for constructing and sculpting the landscapes, and updating their normals.
 */
class LandscapeManager
{
public:
    LandscapeManager();
    ~LandscapeManager();

    /** Constructs a flat landscape that can be sculpted.
     * @param lComp: Decides which entity has the landscapes, and also gives size values.
     */
    void construct(LandscapeComponent* lComp);

    /** Sculpts a landscape.
     * @param lComp: Landscape to be sculpted.
     * @param collisionPoint: Where the mouse cursor ray intersects witht he landscape. Center of sculpting brush.
     * @param sculptUp: Wheter the vertices should be moved upwards or downwards when sculpting.
     */
    void sculpt(LandscapeComponent* lComp, gsl::Vector3D collisionPoint, bool sculptUp = true);

    /** Loads a landscape from a height map.
     * @param landscape: Landscape to load to. Also holds the heightmap filename to use.
     */
    void loadLandscape(LandscapeComponent *landscape);

    ///Sculpting is impossible when this is false.
    bool isSculptingAllowed = false;

    LandscapeComponent *makeLandscape();
private:
    /** Calculates indices for a given landscape.
     * @param lComp: Landscape to calculate for.
     * @return indices for the landscape.
     */
    std::vector<unsigned> makeIndices(LandscapeComponent* lComp);

    /** Calculates normals for a landscape and updates the mesh with them.
     * @param lComp: landscape to update.
     */
    void calculateNormals(LandscapeComponent* lComp);


    /** A different way to calculate normals.
     * @param lComp: landscape to update.
     */
    void makeNormals(LandscapeComponent* lComp);

    /** A function for finding all the vertices inside a sphere, with sculptRange as the radius.

       @param lComp: The landscape we are finding the vertices for
       @param point: The center of the sphere.
       @return A vector with pointers to the vertices, along with their distance from point.
     */
    std::vector<std::pair<Vertex *, float> > findVerticesInRange(LandscapeComponent *lComp, gsl::Vector3D point);

    float sculptSensitivity = 0.1f;
    float sculptRange = 5.f;

    ///Needed to tell the landscapes apart from each other.
    int numOfLandscapes = 0;
    friend class LandscapeWidget;
};

#endif //TRIANGLESURFACE_H
