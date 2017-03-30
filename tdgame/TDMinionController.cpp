#include "TDMinionController.h"

#include "TSController.h"
#include "TSNode.h"
#include "TSMarkers.h"
#include "TSZones.h"

#include "TDBase.h"

namespace TDGame {
    using namespace Tombstone;
    
    using Tombstone::ControllerType;
    
    int32 MinionController::LATEST_ID = 0;
    
    MinionController::MinionController() : Controller(kControllerMinion) {
        id = LATEST_ID;
        LATEST_ID++;
    }
    
    MinionController::MinionController(Node* t):MinionController() {
        target = t;
    }
    
    MinionController::~MinionController() {
        
    }
    
    void MinionController::PreprocessController(void) {
        Controller::PreprocessController();
        
        if (target == nullptr) {
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
            Connector *c = static_cast<Connector*>(target->GetHub()->GetFirstOutgoingRelation());
            
            int num_paths = 1;
            while (c != nullptr) {
                c = static_cast<Connector*>(c->GetNextOutgoingRelation());
                num_paths++;
            }
            
            if (num_paths == 1) {
                return;
            }
            
            int path_chosen = Tombstone::Math::RandomInteger(num_paths);
            c = static_cast<Connector*>(target->GetHub()->GetFirstOutgoingRelation());
            while (path_chosen > 0) {
                target = c->GetConnectorTarget();
                c = static_cast<Connector*>(c->GetNextOutgoingRelation());
                path_chosen--;
            }

        }
        if (target == nullptr) {
            //printf("completed motion\n");
        }
    }
    
    void MinionController::DealDamage(int32 damage) {
        MinionShotMessage *msg = new MinionShotMessage(kMinionShotMessage, damage, GetControllerIndex());
        TheMessageMgr->SendMessageAll(*msg);
    }
    
    ControllerMessage *MinionController::CreateMessage(ControllerMessageType type) const {
        switch (type) {
            case kMinionMoveMessage:
                return new MinionMoveMessage(type, GetControllerIndex());
            case kMinionShotMessage:
                return new MinionShotMessage(type, GetControllerIndex());
        }
    
    }
    
    void MinionController::ReceiveMessage(const ControllerMessage *message) {
        switch (message->GetControllerMessageType()) {
            case kMinionMoveMessage: {
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
                break;
            }
            case kMinionShotMessage: {
                const MinionShotMessage *m = static_cast<const MinionShotMessage *>(message);
                int dmg = m->GetDamage();
                health -= dmg;
                
                Node* model = GetTargetNode();
                Transform4D scaled = model->GetNodeTransform();
                model->SetNodeTransform(scaled * scaled.MakeScale((100.0 - float(dmg))/100.0));
                if (health == 0) {
                    // TODO send a game message object and get the server to do this
                    delete GetTargetNode();
                }
            }
            default: {
                Controller::ReceiveMessage(message);
            }
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
    
    MinionShotMessage::MinionShotMessage(ControllerMessageType type, int32 index): Tombstone::ControllerMessage(type, index) {
        
    }
    
    MinionShotMessage::MinionShotMessage(ControllerMessageType type, int32 dmg, int32 index): Tombstone::ControllerMessage(type, index) {
        damage = dmg;
    }
    
    MinionShotMessage::~MinionShotMessage() {
        
    }
    
    void MinionShotMessage::CompressMessage(Compressor& data) const {
        ControllerMessage::CompressMessage(data);
        
        data << damage;
        data << minionId;
    }
    
    bool MinionShotMessage::DecompressMessage(Decompressor& data) {
        if (ControllerMessage::DecompressMessage(data)) {
            data >> damage;
            data >> minionId;
            
            return true;
        }
        
        return false;
    }
}
