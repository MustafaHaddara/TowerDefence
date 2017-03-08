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

namespace MMGame
{
    using namespace Tombstone;
    
    // Controller types
    enum
    {
        kControllerMinion       = 'cmin'
    };
    
    class MinionController final : public Controller {
    private:
        
        const float     STEP_SIZE = 0.1;   // Distance travelled per frame
        Node			*target;            // Location marker that we're moving towards
        
    public:
        MinionController();
        ~MinionController();
        void PreprocessController(void) override;
        void MoveController(void) override;
        void GetNextTarget(void);
        ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        void ReceiveMessage(const ControllerMessage *message) override;
        
        enum {
            kMinionMoveMessage,
        };
        
    };
    
    class MinionMoveMessage : public ControllerMessage {
        friend class TowerController;
        
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
}

#endif /* TDMinionController_hpp */
