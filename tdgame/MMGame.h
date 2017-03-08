// MODIFIED my MvM

//=============================================================
//
// Tombstone Engine version 1.0
// Copyright 2016, by Terathon Software LLC
//
// This file is part of the Tombstone Engine and is provided under the
// terms of the license agreement entered by the registed user.
//
// Unauthorized redistribution of source code is strictly
// prohibited. Violators will be prosecuted.
//
//=============================================================


#ifndef MMGame_h
#define MMGame_h


#include "TSWorld.h"
#include "TSMarkings.h"
#include "TSShakers.h"

#include "TSInterface.h"
#include "TSApplication.h"
#include "TSInput.h"
#include "TSEngine.h"


#include "MMInput.h"
#include "MMSkin.h"
//#include "MMCameras.h"
//#include "MMFighter.h"

//#include "MMGameWorld.h"
//#include "MMGamePlayer.h"

#include "TDTowerController.h"
#include "TDMinionController.h"


namespace MMGame
{
	using namespace Tombstone;
    
    class GameWorld;


	#if MMCHEATS

		enum
		{
			kGameGodMode		= 1 << 0
		};

	#endif

	class Game : public Application, public Global<Game>
	{
		private:


			Creator<Controller>								controllerCreator;

			//DisplayEventHandler								displayEventHandler;
            ModelRegistration     soldierModelReg;
            ModelRegistration     appleModelRegistration;

			//ControllerReg<GameRigidBodyController>			gameRigidBodyControllerRegistration;
            ControllerReg<TowerController> towerControllerRegistration;
            ControllerReg<MinionController> minionControllerRegistration;

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

// ZUXSVMT
