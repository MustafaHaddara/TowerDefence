//
//  MMGameWorld.h
//
//
//  Created by Martin on 2016-10-04.
//
//

#ifndef __Tombstone__MMGameWorld__
#define __Tombstone__MMGameWorld__

#include "MMGame.h"
#include "MMCameras.h"

namespace MMGame
{
    using namespace Tombstone;





class GameWorld : public World
{
private:
    
    int32					spawnLocatorCount;
    int32					collLocatorCount;
    int32                   minionCount;
	int32					towerCount;
    static const int32      MAX_NUM_MINIONS = 32; // assuming we can only have 16 minions at a time derp
	static const int32		MAX_NUM_TOWERS = 32; // set tower max here
    List<Marker>			spawnLocatorList;
    List<Marker>			collLocatorList;
    Node*                   minionList[MAX_NUM_MINIONS];
	Node*					towerList[MAX_NUM_TOWERS];
    SpectatorCamera			spectatorCamera;
    FirstPersonCamera		firstPersonCamera;
    ChaseCamera				chaseCamera;
    ModelCamera				*playerCamera;
    
    
    
    float					bloodIntensity;
    
    void CollectZoneMarkers(Zone *zone);
    
    static void HandleShakeCompletion(RumbleShaker *shaker, void *cookie);
    
    static ProximityResult RecordSplashDamage(Node *node, const Point3D& center, float radius, void *cookie);
    
public:
    
    GameWorld(const char *name);
    ~GameWorld();
    
    int32 GetSpawnLocatorCount(void) const
    {
        return (spawnLocatorCount);
    }
    
    Marker *GetSpawnLocator(int32 index) const
    {
        return (spawnLocatorList[index]);
    }
    
    // ADDED MORE
    int32 GetCollLocatorCount(void) const
    {
        return (collLocatorCount);
    }
    
    Marker *GetCollLocator(int32 index) const
    {
        return (collLocatorList[index]);
    }
    
    SpectatorCamera *GetSpectatorCamera(void)
    {
        return (&spectatorCamera);
    }
    
    ModelCamera *GetPlayerCamera(void)
    {
        return (playerCamera);
    }
    
    bool UsingFirstPersonCamera(void) const
    {
        return (playerCamera == &firstPersonCamera);
    }
    
    void SetBloodIntensity(float blood)
    {
        bloodIntensity = Fmin(blood, 1.0F);
    }
    
    Node** GetMinions(void);
	//Node** GetTowers(void);
    
    int32 GetNumMinions(void) {
        return minionCount;
    }
	/*
	int32 GetNumTowers(void) {
		return towerCount;
	}
    */
    WorldResult PreprocessWorld(void) override;
    
    RigidBodyStatus HandleNewRigidBodyContact(RigidBodyController *rigidBody, const RigidBodyContact *contact, RigidBodyController *contactBody) override;
    
    
    void DetectInteractions(void) override;
    
    void BeginRendering(void) override;
    void EndRendering(FrameBuffer *frameBuffer = nullptr) override;
    
    void SetCameraTargetModel(Model *model);
    void SetSpectatorCamera(const Point3D& position, float azm, float alt);
    
    void SetLocalPlayerVisibility(void);
    void ChangePlayerCamera(void);
    void SetFocalLength(float focal);
    
    void ShakeCamera(float intensity, int32 duration);
    void ShakeCamera(const Point3D& position, float intensity, int32 duration);
    
    
			 //MVM (these functions are to create  Objects
    
    void AddOjectAtLocation(const Point3D& pos ,ObjectType type,long index,PlayerKey key);
    void ReqestOjectAtLocation(const Point3D& pos ,ObjectType type,PlayerKey index);
    // PRIVATE
    Controller* CreateAvatar(const Point3D& pos ,long index,PlayerKey key);

    void PopulateWorld(void);

    void DeleteMinion(int32 minionId);
	//void DeleteTower(int32 towerID);

};

}

#endif /* defined(__Tombstone__MMGameWorld__) */
