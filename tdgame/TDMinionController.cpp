//
//  TDMinionController.cpp
//  Tombstone
//
//  Created by Mustafa Haddara on 2017-03-06.
//
//

#include "TDMinionController.h"

#include "TSController.h"
#include "TSNode.h"
#include "TSMarkers.h"
#include "TSZones.h"

#include "MMBase.h"

namespace MMGame {
    using namespace Tombstone;
    
    using Tombstone::ControllerType;
    
    MinionController::MinionController() : Controller(kControllerMinion) {
        
    }
    
    MinionController::~MinionController() {
    
    }
    
    void MinionController::PreprocessController(void) {
        Controller::PreprocessController();
        
        Node *root = GetTargetNode();

        Tombstone::Marker *marker = root->GetOwningZone()->GetFirstMarker();
        while (marker) {
            if ((marker->GetMarkerType() == kMarkerLocator) && (marker->NodeEnabled())) {
                LocatorMarker *locator = static_cast<LocatorMarker *>(marker);
                if (locator->GetLocatorType() == kLocatorMinionSpawn) {
                    target = marker;
                    break;
                }
                marker = marker->GetNextListElement();
            }
        }
    }
    
    // dont generate these programmatically instead just attach the controller in the world editor
    void MinionController::MoveController(void) {
        if (TheMessageMgr->GetServerFlag() && target != nullptr) {
            const Point3D pos = target->GetWorldPosition();
            MinionMoveMessage *msg = new MinionMoveMessage(kMinionMoveMessage, pos, GetControllerIndex());
            TheMessageMgr->SendMessageAll(*msg);
        }
    }
    
    void MinionController::GetNextTarget() {
        if (target != nullptr) {
            target = target->GetConnectedNode("next_in_path"); // next_in_path
        }
        if (target == nullptr) {
            //printf("completed motion\n");
        }
    }
    
    ControllerMessage *MinionController::CreateMessage(ControllerMessageType type) const {
        switch (type) {
            case kMinionMoveMessage:
                return new MinionMoveMessage(type, GetControllerIndex());
        }
    
    }
    
    void MinionController::ReceiveMessage(const ControllerMessage *message) {
        if (message->GetControllerMessageType() == kMinionMoveMessage) {
            const MinionMoveMessage *m = static_cast<const MinionMoveMessage *>(message);
            Point3D trgt = m->getTarget();
            Node *attached = GetTargetNode();
            Point3D current = attached->GetNodePosition();
            Vector3D diff = trgt - current;
            if (Magnitude(diff) < STEP_SIZE) {
                GetNextTarget();
                return;
            }
            diff.Normalize();
            diff = diff * STEP_SIZE;
            Point3D step = current + diff;
            
            attached->SetNodePosition(step);
            attached->InvalidateNode();
        } else {
            Controller::ReceiveMessage(message);
        }
    }
    
    /*  TODO maybe a better structure for this is:
        - every client computes minion movement individually
        - periodically (~5-10s) server sends a MinionSyncMessage with position
        - whenever the server's minion hits the target in the path and switches targets,
            the server sends a MinionNewTargetMessage with new target
     
        until then, server just computes every movement step because why not
     */
    MinionMoveMessage::MinionMoveMessage(ControllerMessageType type, int32 index): Tombstone::ControllerMessage(type, index) {
    }
    
    MinionMoveMessage::MinionMoveMessage(ControllerMessageType type, const Point3D trgt, int32 index): Tombstone::ControllerMessage(type, index) {
        target = trgt;
    }
    
    MinionMoveMessage::~MinionMoveMessage() {
    
    }
    
    void MinionMoveMessage::CompressMessage(Compressor& data) const {
        ControllerMessage::CompressMessage(data);
        
        data << target.x;
        data << target.y;
        data << target.z;
    }
    
    bool MinionMoveMessage::DecompressMessage(Decompressor& data) {
        if (ControllerMessage::DecompressMessage(data)) {
            data >> target.x;
            data >> target.y;
            data >> target.z;
            
            return true;
        }
        
        return false;
    }

}
