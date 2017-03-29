#ifndef TDMinionController_h
#define TDMinionController_h

#include "TSController.h"

namespace BaseInvaders
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
    
    /**
     * MinionController class controls minions
     */
    class MinionController final : public Controller {
    
    private:
        const float     STEP_SIZE = 0.1;    // Distance travelled per frame
        int32           health = 100;       // Initial health
        Node			*target;            // Location marker that we're moving towards
        int32           id;                 // Every minion should have a unique Id
        
    public:
        static int32 LATEST_ID;
        
        /**
         * Default constructor
         */
        MinionController();
        
        /**
         * Constructor with target
         * \param t Node pointing at the first node in the minion's path
         */
        MinionController(Node* t);
        
        /**
         * Destructor
         */
        ~MinionController();
        
        /**
         * PreprocessController 
         * This is called once to initialize the controller
         */
        void PreprocessController(void) override;
        
        /**
         * MoveController is called once every frame by the Tombstone engine
         * Here we do all of the processing (on the server) to move the minion to its next node in its path.
         */
        void MoveController(void) override;
        
        /**
         * GetNextTarget is called once the minion has reached within `STEP_SIZE` of the next node
         */
        void GetNextTarget(void);
        
        /**
         * \brief Deals damage to the minion
         * \param damage The amount of damage to deal to the minion
         * Every minion starts with 100 health. Calling this function subtracts the amount of the damage from the health. 
         */
        void DealDamage(int32 damage);
        
        /**
         * Constructor
         */
        int32 GetId(void) {
            return id;
        }
        
        /**
         * Constructor
         */
        bool IsDead() {
            return health > 0;
        }
        
        /**
         * Constructor
         */
        ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        
        /**
         * Constructor
         */
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
