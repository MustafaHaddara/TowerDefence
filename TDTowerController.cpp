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
        World *world = Tombstone::TheWorldMgr->GetWorld();
        Node *target = GetTargetNode();
        
        /* To fire at camera */
        World *myWorld = TheWorldMgr->GetWorld();
        const FrustumCamera *myCamera =	myWorld->GetWorldCamera();
        Point3D targetPoint = myCamera->GetNearPlaneCenter();
        
        Point3D startPos = turretBarrel->GetWorldPosition();
        Vector3D view = (targetPoint - startPos);
        float x = view.x, y = view.y, z = view.z;
        float distance = Sqrt(x * x + y * y + z * z);
        printf("distance: %f\n", distance);
        view = view.Normalize();
        
        if (distance < range) {
            //interpolate between the current viewing direction and the target viewing direction.
            //If you use say 40.0 instead of 20.0 then it will move twice as slowly.
            Vector3D updatedView = Vector3D(originalView.x+(view.x-originalView.x)/40.0f, originalView.y+(view.y-originalView.y)/40.0f, originalView.z+(view.z-originalView.z)/40.0f);
            
            originalView = updatedView;
            x = updatedView.x;
            y = updatedView.y;
            float f = InverseSqrt(x * x + y * y);
            Vector3D right(y * f, -x * f, 0.0F);
            Vector3D down = Cross(updatedView, right);
            
            target->SetNodeMatrix3D(updatedView, -right, -down);
            updatedView = updatedView.Normalize();
            
            Vector3D objectVelocity = 5.0F * (Vector3D(distance * updatedView.x, distance * updatedView.y, distance * updatedView.z));
            objectVelocity.Normalize();
            
            //Firing
            if (world)
            {
                //Fire on some condition - here is just an example
                //So this is true about once every few seconds - increase (or decrease) "300"
                //for longer (or shorter) pauses between shots
                if ((myCount++ % (300 + Math::RandomInteger(50))) == 0)
                {
                    //shoot
                    printf("firing in direction %f %f %f\n", objectVelocity.x, objectVelocity.y, objectVelocity.z);
                }
            }
            // Invalidate the target node so that it gets updated properly
            target->InvalidateNode();
        } else {
            // rotate by a regular amount
//            Node *center = target->GetConnectedNode("center");
//            Transform4D axis = center->GetNodeTransform();
//            Transform4D rot = axis.MakeRotationZ(10.0f);
////            originalView.SetNodeTransform(rot);
//            target->SetNodeTransform(rot);
////            updatedView = updatedView.Normalize();
//            
//            target->InvalidateNode();
        }
    }

//    void TowerController::ReceiveMessage(const ControllerMessage *message) {
//        if (message->GetControllerMessageType() == kSpinMessageState)
//        {
//            const TDSpinStateMessage *stateMessage = static_cast<const TDSpinStateMessage *>(message);
//            
//            spinSpeed = stateMessage->spinSpeed;
//            currentSpeed = stateMessage->currentSpeed;
//            currentAcceleration = stateMessage->currentAcceleration;
//            
//            if (spinState & kSpinInitialized)
//            {
//                UpdateNodeAngle(stateMessage->spinAngle);
//                
//                if (spinSpeed != 0.0F)
//                {
//                    WakeController();
//                }
//            }
//        }
//        else
//        {
//            KinematicController::ReceiveMessage(message);
//        }
//    }
//
//    void TowerController::SendInitialStateMessages(Player *player) const {
//        player->SendMessage(TDSpinStateMessage(GetControllerIndex(), spinAngle, spinSpeed, currentSpeed, currentAcceleration));
//    }
}
