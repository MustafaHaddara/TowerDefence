//
//  TDMinionController.hpp
//  Tombstone
//
//  Created by Mustafa Haddara on 2017-03-06.
//
//

#ifndef TDMinionSpawnController_h
#define TDMinionSpawnController_h

#include "TSController.h"

namespace BaseInvaders
{
    using namespace Tombstone;
    
    // Controller types
    enum
    {
        kControllerMinionSpawn = 'cmsp'
    };
    
    // Message types
    enum: ControllerMessageType {
        kMinionSpawnMessage,
    };
    
    class MinionSpawnController final : public Controller {
        
    private:
        const static int rate = 1000 * 3;  // spawn a new minion every 3 seconds
        int counter = 0;
        
    public:
        MinionSpawnController();
        ~MinionSpawnController();
        void PreprocessController(void) override;
        void MoveController(void) override;
        
        ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        void ReceiveMessage(const ControllerMessage *message) override;
        
    };
    
    class MinionSpawnMessage : public ControllerMessage {
        friend class MinionController;
        
    private:
        int32 newControllerIndex;
        
    public:
        MinionSpawnMessage(ControllerMessageType type, int32 index);
        MinionSpawnMessage(ControllerMessageType type, int32 index, int32 n);
        ~MinionSpawnMessage();
        
        int32 GetNewControllerIndex() const {
            return newControllerIndex;
        }
        
        void CompressMessage(Compressor& data) const override;
        bool DecompressMessage(Decompressor& data) override;
    };
}

#endif /* TDMinionSpawnController_h */
