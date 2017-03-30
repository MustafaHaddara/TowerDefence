#include "TDGameWorld.h"

#include "TDMultiPLayer.h"
#include "TDGamePlayer.h"
#include "TDFighter.h"
#include "TDMinionController.h"

using namespace TDGame;

GameWorld::GameWorld(const char *name) :
World(name),
spectatorCamera(1.,1.0F, 0.3F)
{
}

GameWorld::~GameWorld()
{
    
}

WorldResult GameWorld::PreprocessWorld(void)
{
    WorldResult result = World::PreprocessWorld();
    if (result != kWorldOkay)
    {
        return (result);
    }
    
    SetWorldCamera(&spectatorCamera);
    playerCamera = &firstPersonCamera;
    
    spawnLocatorCount = 0;
    collLocatorCount = 0;
    minionCount = 0;
    CollectZoneMarkers(GetRootNode());
    //printf("WE found %d coll Locators \n",collLocatorCount);
    
    {
        spectatorCamera.SetNodePosition(Point3D(0.0F, 0.0F, 1.0F));
    }
    
    bloodIntensity = 0.0F;
    return (kWorldOkay);
}

void GameWorld::CollectZoneMarkers(Zone *zone)
{
    Marker *marker = zone->GetFirstMarker();
    while (marker) {
        Marker *next = marker->GetNextListElement();
        
        if ((marker->GetMarkerType() == kMarkerLocator) && (marker->NodeEnabled())) {
            LocatorMarker *locator = static_cast<LocatorMarker *>(marker);
            switch (locator->GetLocatorType()) {
                case kLocatorSpawn:
                    spawnLocatorCount++;
                    spawnLocatorList.AppendListElement(locator);
                    break;
                case kLocatorColl:
                    collLocatorCount++;
                    collLocatorList.AppendListElement(locator);
                    break;
                    
            }
        }
        
        marker = next;
    }

    Zone *subzone = zone->GetFirstSubzone();
    while (subzone)
    {
        CollectZoneMarkers(subzone);
        subzone = subzone->GetNextListElement();
    }
}

RigidBodyStatus GameWorld::HandleNewRigidBodyContact(RigidBodyController *rigidBody, const RigidBodyContact *contact, RigidBodyController *contactBody)
{
    return (kRigidBodyUnchanged);
}

/*
 RigidBodyStatus GameWorld::HandleNewGeometryContact(RigidBodyController *rigidBody, const GeometryContact *contact)
 {
 
 
 
	return (kRigidBodyUnchanged);
 }
 */

void GameWorld::DetectInteractions(void)
{
    
    
    World::DetectInteractions();
}

void GameWorld::BeginRendering(void)
{
    World::BeginRendering();
}

void GameWorld::EndRendering(FrameBuffer *frameBuffer)
{
    World::EndRendering(frameBuffer);
    
    
}

void GameWorld::SetCameraTargetModel(Model *model)
{
    firstPersonCamera.SetTargetModel(model);
    chaseCamera.SetTargetModel(model);
    SetWorldCamera(playerCamera);
    
}

void GameWorld::SetSpectatorCamera(const Point3D& position, float azm, float alt)
{
    firstPersonCamera.SetTargetModel(nullptr);
    chaseCamera.SetTargetModel(nullptr);
    SetWorldCamera(&spectatorCamera);
    spectatorCamera.SetNodePosition(position);
    spectatorCamera.SetCameraAzimuth(azm);
    spectatorCamera.SetCameraAltitude(alt);
}

void GameWorld::SetLocalPlayerVisibility(void)
{
    
}

void GameWorld::ChangePlayerCamera(void)
{
    const Player *player = TheMessageMgr->GetLocalPlayer();
    if ((player) && (static_cast<const GamePlayer *>(player)->GetPlayerController()))
    {
        if (playerCamera == &firstPersonCamera)
        {
            playerCamera = &chaseCamera;
        }
        else
        {
            playerCamera = &firstPersonCamera;
        }
        
        
        SetWorldCamera(playerCamera);
        SetLocalPlayerVisibility();
    }
}

void GameWorld::SetFocalLength(float focal)
{
    spectatorCamera.GetObject()->SetFocalLength(focal);
    firstPersonCamera.GetObject()->SetFocalLength(focal);
    chaseCamera.GetObject()->SetFocalLength(focal);
}

/*------------------------------------------*/

// THis is an "NICE" way to convert constants into strings for debugging !!

#define TYPE_NAME(type) \
(kSoldierEntity       == type ? "kSoldierEntity"    :  \
(0 == type ? "yellow" : "unknown"))))


/*----------------------------------------------------------------------------------*/

/// HERE ARE THE FUNCTIONS FOR RUNTIME DISTRIBUTED SCENE GRAPH MANAGEMENT
// ADDING ANYHTING TO THE SCENE GRAPH WE REQUEST IT AND THEN A MESSAGE IS SEND TO ALL
// PARTICIPANTS TO ADD IT !


void GameWorld::ReqestOjectAtLocation(const Point3D& pos ,ObjectType type,PlayerKey key)
{
    
    // NO CALLS TP THIS BUT ON SERVER !!!!!
    
    // VERY IMPROTANT !! THIS IS THE UNIQUE CONTROLLER INDEX
    long cIndex=TheWorldMgr->GetWorld()->NewControllerIndex();
    //PlayerKey key=TheMessageMgr->GetLocalPlayer()->GetPlayerKey();
    
    
    //printf("Request Oject: Index %d Type %s key %d \n",cIndex,TYPE_NAME(type),key);
    if(type==kSoldierEntity){
        //printf("THis is an Avatar, Spwn a spawn point \n");
        GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
        const Marker *marker = world->GetSpawnLocator(0);
        
        CreateCharacterMessage  message(kMessageCreateCharacter,cIndex,type,key,marker->GetWorldPosition());
        TheMessageMgr->SendMessageAll(message,true);

    }else{

        CreateCharacterMessage *message = new CreateCharacterMessage(kMessageCreateCharacter,cIndex,type,key,pos);
        TheMessageMgr->SendMessageAll(*message,true);

        
    }
    
 }



/*----------------------------------------------------------------------------------*/

Controller* GameWorld::CreateAvatar(const Point3D& pos ,long index,PlayerKey key)
{
    
    GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
    //const Marker *locator = world->GetSpawnLocator(0);
    
    
    // Calculate the angle corresponding to the direction the character is initially facing.
    //const Vector3D direction = locator->GetWorldTransform()[0];
    //float azimuth = atan2(direction.y, direction.x);
    
    
    Model *model = Model::GetModel(kModelSoldier);
    FighterController *contr = new FighterController(12);
    model->SetController(contr);
    
    // SET INDEX AND KEY !!! AND FOR THE PLAYER !!!!!!!!!!!!
    contr->SetPlayerKey(key);
    GamePlayer* player = static_cast<GamePlayer *>(TheMessageMgr->GetPlayer(key));
    player->SetPlayerController(contr, nullptr);
    
    
    // NOW WE Have to set the camara to this avantar if it is the local!
    if(key==TheMessageMgr->GetLocalPlayer()->GetKey()){
        
        //printf("This is my player, Set Camara \n");
        
        
        world->SetCameraTargetModel(model);
        world->ChangePlayerCamera();
        
    }
    
    // I Do nto need these here actually
    contr->SetControllerIndex(index);
    model->SetNodePosition(pos);
    world->AddNewNode(model);

    return(contr);
}


/*----------------------------------------------------------------------------------*/


void GameWorld::AddOjectAtLocation(const Point3D& pos ,ObjectType type,long index,PlayerKey key)
{
    
    //printf("CREATE object of type %s with index %d for player key %d \n",TYPE_NAME(type),index,key);
    //printf("CREATE  Oject: Index %d Type %s key %d \n",index,TYPE_NAME(type),key);
    
    // Create the controller and the model and put it together
    Controller* controller;
    Model *model;
    
    GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
    
    switch(type){
        case kSoldierEntity:
            // SPECIAL CASE !!!!
            CreateAvatar(pos , index, key);
            return;

        case kTowerEntity:
            controller = new TowerController();
            model = Model::GetModel(kModelTower);
            break;
            
    }
    
    // SET THE CONTROLLER KEY
    model->SetController(controller);
    controller->SetControllerIndex(index);
    
    // Put the model in the world at the locator's position.
    
    Node* node=controller->GetTargetNode();
    node->SetNodePosition(pos);
    world->AddNewNode(node);
    
}

/*----------------------------------------------------------*/

void GameWorld::PopulateWorld(void) {
    
}

void GameWorld::DeleteMinion(int32 minionId) {
    GetMinions();
    for (int i=0; i<minionCount; i++) {
        Node *minionNode = minionList[i];
        if (minionNode != nullptr) {
            MinionController *mc = static_cast<MinionController *>(minionNode->GetController());
            if (mc->GetId() == minionId) {
                minionList[i] = nullptr;
                minionNode->GetSuperNode()->RemoveSubnode(minionNode);
                delete minionNode;
            }
        }
    }
}

Node** GameWorld::GetMinions() {
    Node *child = GetRootNode()->GetFirstSubnode();
    minionCount = 0;
    while (child) {
        Controller *c = child->GetController();
        if (c != nullptr && c->GetControllerType() == kControllerMinion) {
            if (minionCount > MAX_NUM_MINIONS) {
                printf("too many minions!");
                return;
            }
            minionList[minionCount] = child;
            minionCount++;
        }
        child=child->GetNextSubnode();
    }
    return minionList;

}

