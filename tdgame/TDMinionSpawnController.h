#ifndef TDMinionSpawnController_h
#define TDMinionSpawnController_h

#include "TSController.h"

namespace TDGame
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
    
    /**
     * \brief Controls minion spawn.
     *
     * Every 3 seconds, a new minion is spawned in the world.
     */
    class MinionSpawnController final : public Controller {
        
    private:
        const static int rate = 1000 * 3;  // spawn a new minion every 3 seconds
        int counter = 0;
        
    public:
        /**
         * \brief Default constructor
         */
        MinionSpawnController();
        
        /**
         * \brief Destructor
         */
        ~MinionSpawnController();
        
        /**
         * \brief Initializes controller
         *
         * This is called once to initialize the controller
         */
        void PreprocessController(void) override;
        
        /**
         * \brief This is called once every frame by the Tombstone engine
         *
         * Here we do all of the processing (on the server) to spawn minions
         */
        void MoveController(void) override;
        
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
     * \brief Message encapsulating instructing all clients to spawn a new minion
     */
    class MinionSpawnMessage : public ControllerMessage {
        friend class MinionController;
        
    private:
        int32 newControllerIndex;
        
    public:
        /**
         * \brief Base constructor used by Tombstone Engine
         */
        MinionSpawnMessage(ControllerMessageType type, int32 index);
        
        /**
         * \brief Constructor used by Server to signal a change in targets to the minions
         * \param n The index for the new controller
         *
         * Every controller uses an index which is unique between all indices on a machine. By tracking indices this way, a controller can send a message to its counterparts on all other machines.
         */
        MinionSpawnMessage(ControllerMessageType type, int32 index, int32 n);
        
        /**
         * \brief Destructor
         */
        ~MinionSpawnMessage();
        
        /**
         * \brief Returns the index of the controller to be created
         */
        int32 GetNewControllerIndex() const {
            return newControllerIndex;
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
