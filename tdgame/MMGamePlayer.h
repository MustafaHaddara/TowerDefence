#ifndef __Tombstone__BaseInvadersPlayer__
#define __Tombstone__BaseInvadersPlayer__

#include "TSWorld.h"
#include "TSMarkings.h"
#include "TSShakers.h"

#include "TSInterface.h"
#include "TSApplication.h"
#include "TSInput.h"
#include "TSEngine.h"

#include "MMFighter.h"


namespace BaseInvaders
{
    using namespace Tombstone;
    
    
    
    class GamePlayer : public Player
    {
        friend class Game;
        
    private:
        
        FighterController		*playerController;
        
        unsigned_int32			playerFlags;
        int32					playerPing;
        
        int32					playerStyle[50];
        
        int32					scoreUpdateTime;
        int32					scoreboardTime;
        
        int32					deathTime;
        int32					shieldTime;
        
    public:
        
        GamePlayer(PlayerKey key);
        ~GamePlayer();
        
        GamePlayer *GetPreviousPlayer(void) const
        {
            return (static_cast<GamePlayer *>(Player::GetPreviousPlayer()));
        }
        
        GamePlayer *GetNextPlayer(void) const
        {
            return (static_cast<GamePlayer *>(Player::GetNextPlayer()));
        }
        
        FighterController *GetPlayerController(void) const
        {
            return (playerController);
        }
        
        
        void SetPlayerController(FighterController *controller, const void *state = nullptr);
        void SetPlayerStyle(const int32 *style);
        
        void AddPlayerScore(Fixed delta);
        void SetPlayerHealth(Fixed health);
        
        
        void AddPlayerKill(void);
        void AddPlayerTreasure(int32 treasureIndex, int32 count);
        
        
        CharacterStatus Damage(Fixed damage, unsigned_int32 flags, GameCharacterController *attacker = nullptr);
        void Kill(GameCharacterController *attacker = nullptr);
        
        static GamePlayer *GetAttackerPlayer(const GameCharacterController *attacker);
        static PlayerKey GetAttackerKey(const GameCharacterController *attacker);
    };
    

    
}

#endif
