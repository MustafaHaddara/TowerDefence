#ifndef TDMinionController_h
#define TDMinionController_h

#include "TSController.h"

namespace TDGame {
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
    };
    
    /**
     * \brief Controls minions.
     *
     * Minions follow paths in the world, randomly chosing between forks in the path. Minions can also take damage and die.
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
         * \brief Default constructor
         */
        MinionController();
        
        /**
         * \brief Constructor with target
         * \param t Node pointing at the first node in the minion's path
         */
        MinionController(Node* t);
        
        /**
         * \brief Destructor
         */
        ~MinionController();
        
        /**
         * \brief Initializes controller
         *
         * This is called once to initialize the controller
         */
        void PreprocessController(void) override;
        
        /**
         * \brief This is called once every frame by the Tombstone engine
         *
         * Here we do all of the processing (on the server) to move the minion to its next node in its path.
         */
        void MoveController(void) override;
        
        /**
         * \brief Gets the next target for the minion to travel to.
         *
         * This is called once the minion has reached within `STEP_SIZE` of the next node. It searchs the current node for outgoing connectors, which point at successive nodes in the path. If there is more than one outgoing connector, one is randomly chosen to be the target to travel to.
         */
        void GetNextTarget(void);
        
        /**
         * \brief Deals damage to the minion.
         * \param damage The amount of damage to deal to the minion
         *
         * Every minion starts with 100 health. Calling this function subtracts the amount of the damage from the health. If the minion's health falls to 0 or below, the minion deletes itself from the world.
         */
        void DealDamage(int32 damage);
        
        /**
         * \brief Returns the ID of the minion
         */
        int32 GetId(void) {
            return id;
        }
        
        /**
         * \brief Creates a message when recieving messages client-side
         * \param type Dictates what kind of message to create
         */
        ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        
        /**
         * \brief Recieves messages
         */
        void ReceiveMessage(const ControllerMessage *message) override;
        
    };
    
    /**
     * \brief Message encapsulating movement instructions for the minions
     */
    class MinionMoveMessage : public ControllerMessage {
        friend class MinionController;
        
    private:
        Point3D target;
        
    public:
        /**
         * \brief Base constructor used by Tombstone Engine
         */
        MinionMoveMessage(ControllerMessageType type, int32 index);
        
        /**
         * \brief Constructor used by Server to signal a change in targets to the minions
         * \param trgt The new target for the minion
         */
        MinionMoveMessage(ControllerMessageType type, const Point3D trgt, int32 index);
        
        /**
         * \brief Destructor
         */
        ~MinionMoveMessage();
        
        /**
         * \brief Returns the new target of the minion
         */
        Point3D getTarget() const {
            return target;
        }
        
        /**
         * \brief Serializes message
         */
        void CompressMessage(Compressor& data) const override;
        
        /**
         * \brief Deserializes message
         */
        bool DecompressMessage(Decompressor& data) override;
    };
    
    /**
     * \brief Message encapsulating telling a client minion that it got shot
     */
    class MinionShotMessage : public ControllerMessage {
        friend class MinionController;
        
    private:
        int32 damage;
        int32 minionId;
        
    public:
        /**
         * \brief Base constructor used by Tombstone Engine
         */
        MinionShotMessage(ControllerMessageType type, int32 index);
        
        /**
         * \brief Constructor used by Server to signal a change in targets to the minions
         * \param dmg The amount of damage dealt to the minion
         */
        MinionShotMessage(ControllerMessageType type, int32 dmg, int32 index);
        
        /**
         * Destructor
         */
        ~MinionShotMessage();
        
        int32 GetDamage() const {
            return damage;
        }
        
        /**
         * \brief Serializes message
         */
        void CompressMessage(Compressor& data) const override;
        
        /**
         * \brief Deserializes message
         */
        bool DecompressMessage(Decompressor& data) override;
    };
}

#endif
