#ifndef BaseInvaders_h
#define BaseInvaders_h


#include "TSWorld.h"
#include "TSMarkings.h"
#include "TSShakers.h"

#include "TSInterface.h"
#include "TSApplication.h"
#include "TSInput.h"
#include "TSEngine.h"


#include "TDInput.h"
#include "TDSkin.h"

#include "TDTowerController.h"
#include "TDMinionController.h"
#include "TDMinionSpawnController.h"


namespace BaseInvaders
{
	using namespace Tombstone;
    
    class GameWorld;


	#if TDCHEATS

		enum
		{
			kGameGodMode		= 1 << 0
		};

	#endif

	class Game : public Application, public Global<Game>
	{
		private:


			Creator<Controller>								controllerCreator;

            ModelRegistration     soldierModelReg;
            ModelRegistration     minionModelReg;
            ModelRegistration     appleModelRegistration;

            ControllerReg<TowerController> towerControllerRegistration;
            ControllerReg<MinionController> minionControllerRegistration;
            ControllerReg<MinionSpawnController> minionSpawnControllerRegistration;

			LocatorRegistration								spawnLocatorRegistration;
			LocatorRegistration								collLocatorRegistration;
            LocatorRegistration								physLocatorRegistration;



			CommandObserver<Game>							hostCommandObserver;
			CommandObserver<Game>							joinCommandObserver;

			Command											hostCommand;
			Command											joinCommand;
        


			MovementAction									forwardAction;
			MovementAction									backwardAction;
			MovementAction									leftAction;
			MovementAction									rightAction;
			MovementAction									upAction;
			MovementAction									downAction;
			MovementAction									movementAction;
			FireAction										primaryFireAction;


			InputMgr::KeyCallback							*prevEscapeCallback;
			void											*prevEscapeCookie;

			unsigned_int32									gameFlags;

			ResourceName									currentWorldName;
			//PlayerState										previousPlayerState;

			static World *CreateWorld(const char *name, void *cookie);
			static Player *CreatePlayer(PlayerKey key, void *cookie);

			static Controller *CreateController(Unpacker& data);

			static void EscapeCallback(void *cookie);

			//static void SinglePlayerWorldLoaded(LoadWindow *window, void *cookie);
        
        
            // Skin
             List<GameBoard>                             boardList;

		public:

			Game();
			~Game();

			


			static void InitPlayerStyle(int32 *style);

			float GetCameraFocalLength(void) const;

			void HandleHostCommand(Command *command, const char *text);
			void HandleJoinCommand(Command *command, const char *text);

			void StartSinglePlayerGame(const char *name);
			EngineResult HostMultiplayerGame(const char *name, unsigned_int32 flags);
			EngineResult JoinMultiplayerGame(const char *name, unsigned_int32 flags);
			void ExitCurrentGame(void);

			void RestartWorld(void);

			void HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param) override;
			void HandlePlayerEvent(PlayerEvent event, Player *player, const void *param) override;
			void HandleGameEvent(GameEvent event, const void *param) override;

			Message *CreateMessage(MessageType type, Decompressor& data) const override;
			void ReceiveMessage(Player *sender, const NetworkAddress& address, const Message *message) override;

			void SpawnPlayer(Player *player);

			EngineResult LoadWorld(const char *name) override;
			void UnloadWorld(void) override;

			void ApplicationTask(void) override;
        
        
            // DISPLAY
        void AddBoard(GameBoard *board)
        {
            boardList.AppendListElement(board);
            TheInterfaceMgr->AddWidget(board);
        }


	};

	extern Game *TheGame;
}


#endif
