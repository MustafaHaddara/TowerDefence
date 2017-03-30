#include "TDMinionSpawnController.h"
#include "TDMinionController.h"

#include "TSController.h"
#include "TSNode.h"
#include "TSMarkers.h"
#include "TSZones.h"
#include "TSWorld.h"

#include "TDBase.h"
#include "TDFighter.h"

namespace TDGame {
    using namespace Tombstone;
    
    using Tombstone::ControllerType;
    
    MinionSpawnController::MinionSpawnController(): Controller(kControllerMinionSpawn) {
        
    }
    
    MinionSpawnController::~MinionSpawnController() {
        
    }
    
    void MinionSpawnController::PreprocessController() {
        Controller::PreprocessController();
    }
    
    void MinionSpawnController::MoveController() {
        Controller::MoveController();
        
        if (TheMessageMgr->GetServerFlag()) {
            counter += TheTimeMgr->GetDeltaTime();
            
            if (counter >= rate) {
                counter = 0;
                long cIndex=TheWorldMgr->GetWorld()->NewControllerIndex();
                MinionSpawnMessage *m = new MinionSpawnMessage(kMinionSpawnMessage, GetControllerIndex(), cIndex);
                TheMessageMgr->SendMessageAll(*m);
            }
        }
        
    }
    
    ControllerMessage* MinionSpawnController::CreateMessage(ControllerMessageType type) const {
        switch (type) {
            case kMinionSpawnMessage:
                return new MinionSpawnMessage(kMinionSpawnMessage, GetControllerIndex());
        }
    }
    
    void MinionSpawnController::ReceiveMessage(const ControllerMessage *message) {
        if (message->GetControllerMessageType() == kMinionSpawnMessage) {
            const MinionSpawnMessage *msm = static_cast<const MinionSpawnMessage *>(message);
            int32 index = msm->GetNewControllerIndex();
            
            Controller* controller = new MinionController(GetTargetNode());
            Model *model = Model::GetModel(kModelMinion);
            
            // SET THE CONTROLLER KEY
            model->SetController(controller);
            controller->SetControllerIndex(index);
            
            // Put the model in the world at the locator's position.
            Node* node = model;
            Transform4D scaled = node->GetNodeTransform().MakeScale(3);
            node->SetNodeTransform(scaled);
            Point3D pos = GetTargetNode()->GetNodePosition();
            node->SetNodePosition(pos);
            TheWorldMgr->GetWorld()->AddNewNode(node);
        } else {
            // error
        }
    }
    
    MinionSpawnMessage::MinionSpawnMessage(ControllerMessageType type, int32 index): ControllerMessage(type, index) {

    }
    
    MinionSpawnMessage::MinionSpawnMessage(ControllerMessageType type, int32 index, int32 n): ControllerMessage(type, index) {
        this->newControllerIndex = n;
    }
    
    MinionSpawnMessage::~MinionSpawnMessage() {
        
    }
    
    void MinionSpawnMessage::CompressMessage(Compressor& data) const {
        ControllerMessage::CompressMessage(data);
        data << newControllerIndex;
    }
    
    bool MinionSpawnMessage::DecompressMessage(Decompressor& data) {
        if (ControllerMessage::DecompressMessage(data)) {
            data >> newControllerIndex;
            return true;
        }
        return false;
    }
}
