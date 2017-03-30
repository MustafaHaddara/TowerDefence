#include "TDTowerController.h"
#include "TDGameWorld.h"

#include "TSController.h"
#include "TSGeometries.h"
#include "TSMethods.h"
#include "TSWorld.h"
#include "TSZones.h"

namespace TDGame {
    using namespace Tombstone;
    
    using Tombstone::ControllerType;
    
	int32 id;
	int32 health=100;

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
        Node *turretBase;
        String<30> nodeName = "NoName";
        int found = 0;
        do {
            nodeName = "NoName";
            if (thisnode->GetNodeName() != NULL)
                nodeName = thisnode->GetNodeName();
            if ((Text::CompareText(nodeName, "TurretBarrel")) ) {
                turretBarrel = thisnode;
                found += 1;
            }
            if ((Text::CompareText(nodeName, "TurretBase")) ) {
                turretBase = thisnode;
                found += 1;
            }
            thisnode = root->GetNextTreeNode(thisnode);
        } while (thisnode && found<2);
        
        originalView = turretBarrel->GetWorldPosition() - turretBase->GetWorldPosition();
    }
    
    void TowerController::MoveController(void) {
        /* Only track the server player */
        if (Tombstone::TheMessageMgr->GetServerFlag()) {
            myCount += TheTimeMgr->GetDeltaTime();
            
            Vector3D view = Vector3D(0,0,0);
            MinionController *mController = GetTargetPoint(RANGE, &view);
            if (mController) {
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
                    mController->DealDamage(DAMAGE_DEALT);
                }
            }
        }
    }
    
    MinionController* TowerController::GetTargetPoint(int32 max_dist, Vector3D *out) {
        // if we're tracking something already
        if (currentTarget != nullptr) {
            if (OrientationToMinion(currentTarget, max_dist, out)) {
                MinionController *mc = static_cast<MinionController *>(currentTarget->GetController());
                return mc;
            } else {
                // minion got out of our range
                currentTarget = nullptr;
            }
        }
        
        GameWorld *gw = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
        Node** minions = gw->GetMinions();
        
        for (int i=0; i<gw->GetNumMinions(); i++) {
            if (minions[i] == nullptr) continue;
            if (OrientationToMinion(minions[i], max_dist, out)) {
                MinionController *mc = static_cast<MinionController *>(currentTarget->GetController());
                return mc;
            }
        }
        return nullptr;
    }
    
    bool TowerController::OrientationToMinion(Node* minion, int32 max_dist, Vector3D* out) {
        Point3D startPos = turretBarrel->GetWorldPosition();
        
        Point3D targetPoint = minion->GetWorldPosition();
        
        Vector3D view = (targetPoint - startPos);
        float distance = Magnitude(view);
        if (distance < max_dist) {
            view = view.Normalize();
            out->x = view.x;
            out->y = view.y;
            out->z = view.z;
            currentTarget = minion;
            return true;
        }
        return false;
    }
    
    ControllerMessage *TowerController::CreateMessage(ControllerMessageType type) const {
        switch (type) {
            case kTowerRotateMessage:
                return new TowerRotateMessage(type, GetControllerIndex());
        }
        
        return (Controller::CreateMessage(type));
    }
    
	void TowerController::ReceiveMessage(const ControllerMessage *message) {
		if (message->GetControllerMessageType() == kTowerRotateMessage) {
			Node *target = GetTargetNode();

			const TowerRotateMessage *m = static_cast<const TowerRotateMessage *>(message);
			Vector3D updatedView = m->getTarget();
            updatedView.z = 0;

			float x = updatedView.x;
			float y = updatedView.y;
			float f = InverseSqrt(x * x + y * y);
			Vector3D right(y * f, -x * f, 0.0F);
			Vector3D down = Cross(updatedView, right);

			target->SetNodeMatrix3D(updatedView, -right, -down);

			// Invalidate the target node so that it gets updated properly
			target->InvalidateNode();
    
		} else {
			Controller::ReceiveMessage(message);
		}
	}
    
    TowerRotateMessage::TowerRotateMessage(ControllerMessageType type, int32 index): Tombstone::ControllerMessage(type, index) {
        
    }
    
    TowerRotateMessage::TowerRotateMessage(ControllerMessageType type, const Vector3D& trgt, int32 index, unsigned_int32 flags): Tombstone::ControllerMessage(type, index, flags) {
        target = trgt;
    }
    
    TowerRotateMessage::~TowerRotateMessage() {
        
    }
    
    void TowerRotateMessage::CompressMessage(Compressor& data) const {
        ControllerMessage::CompressMessage(data);
        
        data << target.x;
        data << target.y;
        data << target.z;
    }
    
    bool TowerRotateMessage::DecompressMessage(Decompressor& data) {
        if (ControllerMessage::DecompressMessage(data)) {
            data >> target.x;
            data >> target.y;
            data >> target.z;
            
            return true;
        }
        
        return false;
    }
}
