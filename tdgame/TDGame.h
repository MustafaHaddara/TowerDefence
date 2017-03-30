#ifndef TDGame_h
#define TDGame_h


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
#include "TDBaseController.h"


namespace TDGame
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

			//DisplayEventHandler								displayEventHandler;
            ModelRegistration     soldierModelReg;
            ModelRegistration     minionModelReg;
            ModelRegistration     appleModelRegistration;

			//ControllerReg<GameRigidBodyController>			gameRigidBodyControllerRegistration;
            ControllerReg<TowerController> towerControllerRegistration;
			ControllerReg<BaseController> baseControllerRegistration;
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
			/**
			* \creates world
			*/
			static World *CreateWorld(const char *name, void *cookie);
			

			/**
			* \creates player
			*/
			static Player *CreatePlayer(PlayerKey key, void *cookie);


			/**
			* \creates controller
			*/
			static Controller *CreateController(Unpacker& data);

			static void EscapeCallback(void *cookie);

			//static void SinglePlayerWorldLoaded(LoadWindow *window, void *cookie);
        
        
            // Skin
             List<GameBoard>                             boardList;
			 List<Window>								windowList;

		public:
			/**
			* \brief Default constructor
			*/
			Game();
			/**
			* \brief denstructor
			*/
			~Game();

			


			static void InitPlayerStyle(int32 *style);

			float GetCameraFocalLength(void) const;
			/**
			* \handles host commans
			*/
			void HandleHostCommand(Command *command, const char *text);
			/**
			* \Handles join commands
			*/
			void HandleJoinCommand(Command *command, const char *text);
			/**
			* \allows person to start single player game
			*/
			void StartSinglePlayerGame(const char *name);
			/**
			* \Allows for hosting of multiplayer game
			*/
			EngineResult HostMultiplayerGame(const char *name, unsigned_int32 flags);
			/**
			* \Allows player to join multiplayer game
			*/
			EngineResult JoinMultiplayerGame(const char *name, unsigned_int32 flags);
			/**
			* \Exits game
			*/
			void ExitCurrentGame(void);
			/**
			* \restarts world
			*/
			void RestartWorld(void);
			/**
			* \Allows players to connect to server
			*/
			void HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param) override;
			/**
			* \allows players to do things
			*/
			void HandlePlayerEvent(PlayerEvent event, Player *player, const void *param) override;
			/**
			* \brief HandleGameEvent
			*/
			void HandleGameEvent(GameEvent event, const void *param) override;

			/**
			* \brief Creates a message when recieving messages client-side
			* \param type Dictates what kind of message to create
			*/
			Message *CreateMessage(MessageType type, Decompressor& data) const override;
			/**
			* \brief recieves message
			*/
			void ReceiveMessage(Player *sender, const NetworkAddress& address, const Message *message) override;
			/**
			* \Controls play spawn
			*/
			void SpawnPlayer(Player *player);
			/**
			* \allows player to load game
			*/
			EngineResult LoadWorld(const char *name) override;
			/**
			* \allows player to exit game
			*/
			void UnloadWorld(void) override;
			
			void ApplicationTask(void) override;
        
			/**
			* \ Adds board
			*/
        void AddBoard(GameBoard *board)
        {
            boardList.AppendListElement(board);
            TheInterfaceMgr->AddWidget(board);
        }
		/**
		* \Adds Window
		*/
		void AddWindow(Window *window)
		{
			windowList.AppendListElement(window);
			TheInterfaceMgr->AddWidget(window);
		}


	};

	extern Game *TheGame;
}


#endif

