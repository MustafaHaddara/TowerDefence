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
        
        const float     range = 30.0f;          // Range within aiming
        int             myCount;            // for firing
        Transform4D     originalTransform;  // The target's original transform
        Vector3D        originalView;       // The direction turret is facing before any rotation
        Node			*turretBarrel;		// Marker on Turret Model from which to shoot
        
    public:
        TowerController();
        ~TowerController();
        void PreprocessController(void);
        void MoveController(void);
        void ReceiveMessage(const ControllerMessage *message);
        
        enum {
            kTowerRotateMessage,
            kTowerShootMessage
        };
        
    };
    
    class TowerRotateMessage : public ControllerMessage {
        friend class TowerController;
        
    private:
        
        Point3D target;
        
    public:
        
        TowerRotateMessage(ControllerMessageType type, const Point3D& trgt, int32 index, unsigned_int32 flags = 0);
        ~TowerRotateMessage();
        
        Point3D getTarget() const {
            return target;
        }
        
        void CompressMessage(Compressor& data) const override;
        bool DecompressMessage(Decompressor& data) override;
    };
    
//    class TowerShootMessage : public ControllerMessage {
//        friend class TowerController;
//        
//    private:
//        
//        Point3D target;
//        
//        TowerShootMessage(ControllerMessageType type, int32 controllerIndex);
//        
//    public:
//        
//        TowerShootMessage(ControllerMessageType type, const Point3D& target);
//        ~TowerShootMessage();
//        
//        Point3D getTarget() const {
//            return target;
//        }
//        
//        void CompressMessage(Compressor& data) const override;
//        bool DecompressMessage(Decompressor& data) override;
//    };
}

#endif
