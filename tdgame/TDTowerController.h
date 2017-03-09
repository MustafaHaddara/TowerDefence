#ifndef SimpleTower_h
#define SimpleTower_h

#include "TSController.h"

namespace MMGame
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
    
    class TowerController final : public Controller {
    private:
        
        const int       SHOOT_DURATION = 1000 * 2;  // Fire every 2 seconds
        const float     range = 30.0f;              // Range within aiming
        int             myCount;                    // for firing
        Transform4D     originalTransform;          // The target's original transform
        Vector3D        originalView;               // The direction turret is facing before any rotation
        Node			*turretBarrel;              // Marker on Turret Model from which to shoot
        
    public:
        TowerController();
        ~TowerController();
        void PreprocessController(void) override;
        void MoveController(void) override;
        bool GetTargetPoint(int32 range, Vector3D *out);
        ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        void ReceiveMessage(const ControllerMessage *message) override;
        
        enum {
            kTowerRotateMessage,
            kTowerShootMessage
        };
        
    };
    
    class TowerRotateMessage : public ControllerMessage {
        friend class TowerController;
        
    private:
        
        Vector3D target;
        
    public:
        
        TowerRotateMessage(ControllerMessageType type, int32 index);
        TowerRotateMessage(ControllerMessageType type, const Vector3D& trgt, int32 index, unsigned_int32 flags = 0);
        ~TowerRotateMessage();
        
        Vector3D getTarget() const {
            return target;
        }
        
        void CompressMessage(Compressor& data) const override;
        bool DecompressMessage(Decompressor& data) override;
    };
    
    class TowerShootMessage : public ControllerMessage {
        friend class TowerController;
        
    private:
        
        Point3D target;
        
    public:
        
        TowerShootMessage(ControllerMessageType type, int32 index);
        TowerShootMessage(ControllerMessageType type, const Point3D& trgt, int32 index);
        ~TowerShootMessage();
        
        Point3D getTarget() const {
            return target;
        }
        
        void CompressMessage(Compressor& data) const override;
        bool DecompressMessage(Decompressor& data) override;
    };
}

#endif
