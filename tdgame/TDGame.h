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

	/**
	* \brief The main game class for Base Invaders.
	* Contains action, controller and model registrations and lists of UI display widgets
	*/
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

			TowerAction										towerOneAction;
			TowerAction										towerTwoAction;
			TowerAction										towerThreeAction;


			InputMgr::KeyCallback							*prevEscapeCallback;
			void											*prevEscapeCookie;

			unsigned_int32									gameFlags;

			ResourceName									currentWorldName;

			static World *CreateWorld(const char *name, void *cookie);
			static Player *CreatePlayer(PlayerKey key, void *cookie);

			static Controller *CreateController(Unpacker& data);

			static void EscapeCallback(void *cookie);        
        
            // Skin
             List<GameBoard>                             boardList;
			 List<Window>								windowList;

		public:

			/**
			* \brief Default constructor
			*/
			Game();
			/**
			* \brief Destructor
			*/
			~Game();

			


			/**
			* \brief Initializes the player style
			* \param style Player style
			*/
			static void InitPlayerStyle(int32 *style);

			/**
			* \brief Returns the focal length of the camera
			*/
			float GetCameraFocalLength(void) const;

			/**
			* \brief Handles hosting a multiplayer game upon receiving a host command
			* \param text Game address
			*/
			void HandleHostCommand(Command *command, const char *text);
			/**
			* \brief Handles joining a multiplayer game upon receiving a join command
			* \param text Game address
			*/
			void HandleJoinCommand(Command *command, const char *text);

			/**
			* \brief Starts a new single player game
			* \param name Name of the game
			*/
			void StartSinglePlayerGame(const char *name);


			/**
			* \brief Hosts a multiplayer game
			* \param name Game name
			* \param flags Game flags
			*/
			EngineResult HostMultiplayerGame(const char *name, unsigned_int32 flags);
			/**
			* \brief Joins a multiplayer game
			* \param name Game name
			* \param flags Game flags
			*/
			EngineResult JoinMultiplayerGame(const char *name, unsigned_int32 flags);

			/**
			* \brief Ends the current game
			*/
			void ExitCurrentGame(void);

			/**
			* \brief Starts a new game in the same world
			*/
			void RestartWorld(void);

			/**
			* \brief Handles server responses to player connections to the game
			* \param event Server event
			* \param address Server address
			*/
			void HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param) override;

			/**
			* \brief Handles player connections to the game
			* \param event Player connection event
			* \param player Player connecting to the game
			*/
			void HandlePlayerEvent(PlayerEvent event, Player *player, const void *param) override;
			/**
			* \brief Handles player-server synchronization events
			* \param event Game connection event
			*/
			void HandleGameEvent(GameEvent event, const void *param) override;

			/**
			* \brief Creates a game message
			* \param type Message type
			* \param data Message data
			*/
			Message *CreateMessage(MessageType type, Decompressor& data) const override;
			/**
			* \brief Receives a message
			* \param sender Player sending the message
			* \param address Origin address
			* \param message Message being sent
			*/
			void ReceiveMessage(Player *sender, const NetworkAddress& address, const Message *message) override;

			/*
			* \brief Starts a new game in the world specified
			* \param name Name of the world
			*/
			EngineResult LoadWorld(const char *name) override;
			/*
			* \brief Unloads the current world
			*/
			void UnloadWorld(void) override;

			void ApplicationTask(void) override;
        
        

		/**
		* \brief Manages adding a board to the display
		*/
        void AddBoard(GameBoard *board)
        {
            boardList.AppendListElement(board);
            TheInterfaceMgr->AddWidget(board);
        }
		/**
		* \brief Manages adding a window to the display
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

