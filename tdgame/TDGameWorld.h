#ifndef __Tombstone__TDGameWorld__
#define __Tombstone__TDGameWorld__

#include "TDGame.h"
#include "TDCameras.h"

namespace TDGame
{
    using namespace Tombstone;


	/**
	* \brief Sets up the game world.
	*
	* Allows for interaction with game world for other game elements.
	*/


class GameWorld : public World
{
private:
    
    int32					spawnLocatorCount; // Number of spawn locations on map
    int32					collLocatorCount; // Collection objects were never implemented
    int32                   minionCount;	// Current Number of living minions
    static const int32      MAX_NUM_MINIONS = 64; // Maximum number of living minions. Ideally would be infinity, but maximum of 64 due to technical limitations.
    
    List<Marker>			spawnLocatorList; // List of all spawn locations
    List<Marker>			collLocatorList; // List of collection object locations
    Node*                   minionList[MAX_NUM_MINIONS]; // Initializer for minionList[]
    
    SpectatorCamera			spectatorCamera; // Camera initializer for spectators
    ChaseCamera				chaseCamera; // Camera initializer for chase camera
    ModelCamera				*playerCamera; // Camera initizalier for player camera
    
    
    
    float					bloodIntensity; // Initialize blood intensity variable
    
	/*
	 * \brief Collection zone mark initializer
	 * \param *zone points at collection zone
	 *
	 *
	*/
    void CollectZoneMarkers(Zone *zone); 
    
	/*
	 * \brief Handles shake completion
	 * \param *shaker points at active RumbleShaker
	 * \param *cookie -frankly, unsure what this is necessary for as it doesn't seem to be used anywhere
	 *
	*/
    static void HandleShakeCompletion(RumbleShaker *shaker, void *cookie); 
    
	/*
	 * \brief Handles spash damage application
	 * \param *node points at active node
	 * \param center is the location of the center of the splash damage
	 * \param *cookie -frankly, unsure what this is necessary for as it doesn't seem to be used anywhere
	 *
	*/
    static ProximityResult RecordSplashDamage(Node *node, const Point3D& center, float radius, void *cookie);
    
public:
    
	/*
	 * \brief Constructor
	 * \param *name Points to name of game world
	 * 
	 * A constructor for the game world.
	*/

    GameWorld(const char *name);

	/*
 	 * \brief Destructor
	 * A destructor for the game world.
	*/ 
    ~GameWorld();
    
	/*
	 * \brief Count number of spawn locations
	 * A function that returns the number of spawn locations present in the game world.
	*/
    int32 GetSpawnLocatorCount(void) const
    {
        return (spawnLocatorCount);
    }
	/*
	 * \brief Return spawn locator list
	 * \param index points to the index of the spawn locator list
	* A function that returns the list of spawn locations present in the game world.
	*/
    Marker *GetSpawnLocator(int32 index) const
    {
        return (spawnLocatorList[index]);
    }
    
	/*
	 * \brief Count number of collectable object locators
	 * A function that returns the number of collectable object locators present in the game world.
	*/
    int32 GetCollLocatorCount(void) const
    {
        return (collLocatorCount);
    }
    
	/*
	 * \brief Return collectable object locator list
	 * \param index points to the index of the collectable object locator list
	* A function that returns the collectable object locators present in the game world.
	*/

    Marker *GetCollLocator(int32 index) const
    {
        return (collLocatorList[index]);
    }
    
	/*
	 * \brief Return spectator camera pointer.
	 * A function that returns the pointer to the spectator camera.
	*/
    SpectatorCamera *GetSpectatorCamera(void)
    {
        return (&spectatorCamera);
    }
    
	/*
	 *\brief Return player camera object.
	 *A function that returns the player camera object.
	*/
    ModelCamera *GetPlayerCamera(void)
    {
        return (playerCamera);
    }
    
	/*
	 * \brief A boolean check to test which camera is being used.
	 * A function that checks which camera is being used.
	*/
    bool UsingFirstPersonCamera(void) const
    {
        return false; //(playerCamera == &firstPersonCamera);
    }
	
	/*
	* \brief Sets intensity of blood graphic effects
	* \param blood is passed float to use in blood intensity calculation
	* A function that sets the intensity of blood graphic effects.
	*/
    void SetBloodIntensity(float blood)
    {
        bloodIntensity = Fmin(blood, 1.0F);
    }
    
	/*
	* \brief Function that returns list of minions
	* A function that returns the list of minions (currently not used)
	*/
    Node** GetMinions(void);
    
	/*
	* \brief Return number of minions
	* A function that returns the number of minions present in the world.
	*/
    int32 GetNumMinions(void) {
        return minionCount;
    }
    
	/*
	 *\brief Constructor for world preprocessing
	 */
    WorldResult PreprocessWorld(void) override;
    
	/*
	 *\brief Constructor for rigid body collision detection
	 *\param *rigidBody points to object A
	 *\param *contact points to RigidBodyContact object
	 *\param *contactBody points to object B
	 */
    RigidBodyStatus HandleNewRigidBodyContact(RigidBodyController *rigidBody, const RigidBodyContact *contact, RigidBodyController *contactBody) override;
    
	/*
	*\brief Constructor for interaction detection
	*/
    void DetectInteractions(void) override;
    
	/*
	*\brief Constructor for rendering start
	*/
    void BeginRendering(void) override;

	/*
	*\brief Constructor for EndRendering, effectively garbage collection
	*/
    void EndRendering(FrameBuffer *frameBuffer = nullptr) override;
    
	/*
	*\brief Constructor for setting camera model.
	*\param *model points to the model to attach camera to.
	*/
    void SetCameraTargetModel(Model *model);
	/*
	*\brief Constructor for setting spectator camera view
	*\param position is the x y z coordinates of the camera
	*\param azm is the azimuth of the camera
	*\param alt is the altitude of the camera
	*/
    void SetSpectatorCamera(const Point3D& position, float azm, float alt);
    
	/*
	*\brief Constructor for setting player visibility
	*/
    void SetLocalPlayerVisibility(void);

	/*
	*\brief Constructor for setting focal length
	*\param focal is the float passed to set the focal length.
	*/
    void SetFocalLength(float focal);
    
	/*
	*\brief Constructor for shake camera.
	*\param intensity sets intensity of the shaking
	*\param duration sets duration of shaking
	*/
    void ShakeCamera(float intensity, int32 duration);

	/*
	*\brief Constructor for shake camera.
	*\param position sets location for shaking to emanate from.
	*\param intensity sets intensity of the shaking.
	*\param duration sets duration of shaking.
	*/
    void ShakeCamera(const Point3D& position, float intensity, int32 duration);
    
    

	/*
	*\brief Constructor for game object creation function
	*\param pos is the position at which to add the object.
	*\param type is the type of object to add.
	*\param index is index of the objects
	*\param key is the player key to which the object is attached
	*/
    void AddOjectAtLocation(const Point3D& pos ,ObjectType type,long index,PlayerKey key);

	/*
	*\brief Constructor for game object creation request function
	*\param pos is the position at which to request the object.
	*\param type is the type of object to add.
	*\param index is the player key to which the object is attached
	*/
    void ReqestOjectAtLocation(const Point3D& pos ,ObjectType type,PlayerKey index);

	/*
	*\brief Constructor for avatar creation function
	*\param pos is the position at which to create the avatar.
	*\param index is index of the objects
	*\param key is the player key to which the avatar is attached
	*/
    Controller* CreateAvatar(const Point3D& pos ,long index,PlayerKey key);

	/*
	*\brief Constructor for world population (not used)
	*/
    void PopulateWorld(void);
    
	/*
	*\brief Constructor for minion deletion function
	*\param minionId is the id number of the minion to be deleted.
	*/
    void DeleteMinion(int32 minionId);
    
};

}

#endif /* defined(__Tombstone__TDGameWorld__) */
