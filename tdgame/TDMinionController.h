//
//  TDMinionController.hpp
//  Tombstone
//
//  Created by Mustafa Haddara on 2017-03-06.
//
//

#ifndef TDMinionController_h
#define TDMinionController_h

#include "TSController.h"

namespace TDGame
{
    using namespace Tombstone;
    
    // Controller types
    enum
    {
        kControllerMinion       = 'cmin'
    };
    
    // Message types
    enum: MessageType {
        kMinionMoveMessage,
        kMinionShotMessage,
        kMessageMinionDead,
    };
    
    class MinionController final : public Controller {
    
    private:
        const float     STEP_SIZE = 0.1;    // Distance travelled per frame
        int32           health = 100;       // Initial health
        Node			*target;            // Location marker that we're moving towards
        int32           id;                 // Every minion should have a unique Id
        
    public:
        static int32 LATEST_ID;
        
        MinionController();
        MinionController(Node* t);
        ~MinionController();
        void PreprocessController(void) override;
        void MoveController(void) override;
        void GetNextTarget(void);
        void DealDamage(int32 damage);
        
        int32 GetId(void) {
            return id;
        }
        
        bool IsDead() {
            return health > 0;
        }
        
        ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        void ReceiveMessage(const ControllerMessage *message) override;
        
    };
    
    class MinionMoveMessage : public ControllerMessage {
        friend class MinionController;
        
    private:
        Point3D target;
        
    public:
        MinionMoveMessage(ControllerMessageType type, int32 index);
        MinionMoveMessage(ControllerMessageType type, const Point3D trgt, int32 index);
        ~MinionMoveMessage();
        
        Point3D getTarget() const {
            return target;
        }
        
        void CompressMessage(Compressor& data) const override;
        bool DecompressMessage(Decompressor& data) override;
    };
    
    class MinionShotMessage : public ControllerMessage {
        friend class MinionController;
        
    private:
        int32 damage;
        int32 minionId;
        
    public:
        MinionShotMessage(ControllerMessageType type, int32 index);
        MinionShotMessage(ControllerMessageType type, int32 dmg, int32 mId, int32 index);
        ~MinionShotMessage();
        
        int32 GetDamage() const {
            return damage;
        }
        
        int32 GetMinionId() const {
            return minionId;
        }
        
        void CompressMessage(Compressor& data) const override;
        bool DecompressMessage(Decompressor& data) override;
    };
    
    class MinionDeadMessage : public Message {
    private:
        int32 minionId;
        
    public:
        MinionDeadMessage(int32 flags=0);
        MinionDeadMessage(int32 minionId, int32 flags=0);
        ~MinionDeadMessage();
        
        int32 GetMinionId() const {
            return minionId;
        }
        
        void CompressMessage(Compressor& data) const override;
        bool DecompressMessage(Decompressor& data) override;
        
        void HandleMessage(Player* sender);
    };
}

#endif /* TDMinionController_h */
