//
//  TDTowerController.cpp
//  Tombstone
//
//  Created by Mustafa Haddara on 2017-01-07.
//
//

#include "TDTowerController.h"

#include "TSController.h"
#include "TSCameras.h"
#include "TSGeometries.h"
#include "TSMethods.h"
#include "TSWorld.h"
#include "TSZones.h"

namespace MMGame
{
    using namespace Tombstone;

    using Tombstone::ControllerType;
    
    TowerController::TowerController() : Controller(kControllerTower) {
    }

    TowerController::~TowerController() {
    }

    void TowerController::PreprocessController(void) {
        Controller::PreprocessController();
        
        myCount = 0;
        const Node *target = GetTargetNode();
        originalTransform = target->GetWorldTransform();//GetNodeTransform();
        Point3D originalDirection = Point3D(originalTransform[0].x,originalTransform[0].y, originalTransform[0].z);
        originalView = (originalDirection).Normalize();
        
        //Find the marker on the Turret. It is the point from which to fire.
        Node *root = GetTargetNode();
        Node *thisnode = root;
        String<30> nodeName = "NoName";
        bool found = false;
        do
        {
            nodeName = "NoName";
            if (thisnode->GetNodeName() != NULL)
                nodeName = thisnode->GetNodeName();
            if ((Text::CompareText(nodeName, "TurretBarrel")) ) {
                turretBarrel = thisnode;
                found = true;
            }
            thisnode = root->GetNextTreeNode(thisnode);
        } while (thisnode && !found);
    }

    void TowerController::MoveController(void) {
        /* Only track the server player */
        if (Tombstone::TheMessageMgr->GetServerFlag()) {
            myCount += TheTimeMgr->GetDeltaTime();

            /* To fire at camera */
            World *myWorld = TheWorldMgr->GetWorld();
            const FrustumCamera *myCamera =	myWorld->GetWorldCamera();
            Point3D targetPoint = myCamera->GetNearPlaneCenter();

            Point3D startPos = turretBarrel->GetWorldPosition();
            Vector3D view = (targetPoint - startPos);
            float x = view.x, y = view.y, z = view.z;
            float distance = Sqrt(x * x + y * y + z * z);
            view = view.Normalize();
            
            if (distance < range) {
                //interpolate between the current viewing direction and the target viewing direction.
                //If you use say 40.0 instead of 20.0 then it will move twice as slowly.
                Vector3D updatedView = Vector3D(originalView.x+(view.x-originalView.x)/40.0f,
                                                originalView.y+(view.y-originalView.y)/40.0f,
                                                originalView.z+(view.z-originalView.z)/40.0f);
                
                originalView = updatedView;
                
                TheMessageMgr->SendMessageAll(TowerRotateMessage(kTowerRotateMessage, updatedView, GetControllerIndex()));
                
                // Fire on every SHOOT_DURATION milliseconds
                if (myCount >= SHOOT_DURATION) {
                    myCount = 0;
                    // Fire
                    TheMessageMgr->SendMessageAll(TowerShootMessage(kTowerShootMessage, targetPoint, GetControllerIndex()));
                }
            }
        }
    }
    
    ControllerMessage *TowerController::CreateMessage(ControllerMessageType type) const {
        switch (type)
        {
            case kTowerRotateMessage:
                return new TowerRotateMessage(type, GetControllerIndex());
                
            case kTowerShootMessage:
                return new TowerShootMessage(type, GetControllerIndex());
        }
        
        return (Controller::CreateMessage(type));
    }
    
    void TowerController::ReceiveMessage(const ControllerMessage *message) {
        if (message->GetControllerMessageType() == kTowerRotateMessage) {
            Node *target = GetTargetNode();
            
            const TowerRotateMessage *m = static_cast<const TowerRotateMessage *>(message);
            Vector3D updatedView = m->getTarget();
            
            float x = updatedView.x;
            float y = updatedView.y;
            float f = InverseSqrt(x * x + y * y);
            Vector3D right(y * f, -x * f, 0.0F);
            Vector3D down = Cross(updatedView, right);
            
            target->SetNodeMatrix3D(updatedView, -right, -down);
            // Invalidate the target node so that it gets updated properly
            target->InvalidateNode();
            
        } else if (message->GetControllerMessageType() == kTowerShootMessage) {
            const TowerShootMessage *m = static_cast<const TowerShootMessage *>(message);
            Vector3D shootTarget = m->getTarget();
            printf("firing in direction %f %f %f\n", shootTarget.x, shootTarget.y, shootTarget.z);
        } else {
            Controller::ReceiveMessage(message);
        }
    }
    
    TowerRotateMessage::TowerRotateMessage(ControllerMessageType type, int32 index): Tombstone::ControllerMessage(type, index) {
        
    }
    
    TowerRotateMessage::TowerRotateMessage(ControllerMessageType type, const Vector3D& trgt, int32 index, unsigned_int32 flags): Tombstone::ControllerMessage(type, index, flags) {
        target = trgt;
    }

    TowerRotateMessage::~TowerRotateMessage()
    {
    }

    void TowerRotateMessage::CompressMessage(Compressor& data) const
    {
        ControllerMessage::CompressMessage(data);
        
        data << target.x;
        data << target.y;
        data << target.z;
    }

    bool TowerRotateMessage::DecompressMessage(Decompressor& data)
    {
        if (ControllerMessage::DecompressMessage(data)) {
            data >> target.x;
            data >> target.y;
            data >> target.z;
            
            return true;
        }
        
        return false;
    }
    
    TowerShootMessage::TowerShootMessage(ControllerMessageType type, int32 index): Tombstone::ControllerMessage(type, index) {
    
    }
    
    TowerShootMessage::TowerShootMessage(ControllerMessageType type, const Point3D& trgt, int32 index): Tombstone::ControllerMessage(type, index) {
        target = trgt;
    }
    
    TowerShootMessage::~TowerShootMessage() {}
    
    void TowerShootMessage::CompressMessage(Compressor& data) const {
        ControllerMessage::CompressMessage(data);
        
        data << target.x;
        data << target.y;
        data << target.z;
    
    }
    
    bool TowerShootMessage::DecompressMessage(Decompressor& data) {
        if (ControllerMessage::DecompressMessage(data)) {
            data >> target.x;
            data >> target.y;
            data >> target.z;
            
            return true;
        }
        
        return false;
    }
}
