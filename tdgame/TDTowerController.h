#ifndef SimpleTower_h
#define SimpleTower_h

#include "TSController.h"
#include "TDMinionController.h"

namespace TDGame
{
    using namespace Tombstone;
    
    // Controller types
    enum
    {
        kControllerTower        = 'Turr'
    };
    
    enum
    {
        kModelTower             = 'Turt'
    };
    
    /**
     * \brief Controls towers.
     */
    class TowerController final : public Controller {
    private:
        
        const int       SHOOT_DURATION = 1000 * 2;  // Fire every 2 seconds
        const float     RANGE = 30.0f;              // Range within aiming
        const int       DAMAGE_DEALT = 20;          // Damage per shot
        
        int             myCount;                    // for firing
        Transform4D     originalTransform;          // The target's original transform
        Vector3D        originalView;               // The direction turret is facing before any rotation
        Node			*turretBarrel;              // Marker on Turret Model from which to shoot
        Node*           currentTarget = nullptr;
		int32			id;
		int32			health = 100;
        
    public:
		static int32 LATEST_ID;

        /**
         * \brief Default constructor
         */
        TowerController();
        
        /**
         * \brief Destructor
         */
        ~TowerController();
        
        /**
         * \brief Initializes controller
         *
         * This is called once to initialize the controller
         */
        void PreprocessController(void) override;
        
        /**
         * \brief This is called once every frame by the Tombstone engine
         *
         * Here we do all of the processing (on the server) to track minions and deal damage to those minions
         */
        void MoveController(void) override;
        
        /**
         * \brief Returns a pointer to the `MinionController` of the minion currently being tracked.
         * \param range The radius to search within for a minion if not currently tracking one
         * \param out This `Vector3D` is set to point at the minion being tracked.
         *
         * If the tower is currently tracking a minion, and that minion is still in range, this function returns a point to that minion's controller. 
         * If the currently tracked minion has gotten too far away, or if the tower is not currently tracking a minion, this finds a random minion within range and returns a pointer to that minion's controller, and sets that minion as the one currently being tracked.
         * In both cases, the `out` parameter is set to a `Vector3D` pointing at the minion being tracked.
         * 
         * If there is no minion within range, this function returns `NULL` and the result of the `out` parameter is undefined.
         */
        MinionController* GetTargetPoint(int32 range, Vector3D *out);
        
        /**
         * \brief Finds the orientation towards a node
         * \param minion A pointer to the minion to target
         * \param max_dist Maximum range to the minion
         * \param out Direction to the minion
         *
         * If the minion is within range, the `out` parameter is set to a vector pointing to the minion, and the function returns `true`.
         * Otherwise the function returns `false`.
         */
        bool OrientationToMinion(Node* minion, int32 max_dist, Vector3D* out);
        
        /**
         * \brief Creates a message when recieving messages client-side
         * \param type Dictates what kind of message to create
         */
        ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        
        /**
         * \brief Recieves messages
         */
        void ReceiveMessage(const ControllerMessage *message) override;
        
        enum {
            kTowerRotateMessage,
			kTowerCreateMessage,
        };

		int32 GetId(void) {
			return id;
		}
        
    };
    
    /**
     * \brief Message encapsulating rotation instructions for the tower.
     */
    class TowerRotateMessage : public ControllerMessage {
        friend class TowerController;
        
    private:
        
        Vector3D target;

    public:
        
        /**
         * \brief Base constructor used by Tombstone Engine
         */
        TowerRotateMessage(ControllerMessageType type, int32 index);
        
        /**
         * \brief Contructor used by Server to signal a new rotation to the clients.
         * \param trgt New orientation for the tower.
         */
        TowerRotateMessage(ControllerMessageType type, const Vector3D& trgt, int32 index, unsigned_int32 flags = 0);
        
        /**
         * \brief Destructor
         */
        ~TowerRotateMessage();
        
        /**
         * \brief Returns the new orientation `Vector3D` of the tower.
         */
        Vector3D getTarget() const {
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
}

#endif
