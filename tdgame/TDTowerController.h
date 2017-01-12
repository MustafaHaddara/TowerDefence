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
    
    class TowerController final : public Controller
    {
    private:
        
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
    };
}

#endif
