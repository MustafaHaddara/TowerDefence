#include "TSToolWindows.h"
#include "TSAudioCapture.h"
#include "TSWater.h"

#include "TDGame.h"
#include "TDMultiplayer.h"
#include "TDGameWorld.h"
#include "TDFighter.h"



using namespace TDGame;



Game *TDGame::TheGame = nullptr;



Game::Game() :
		Global<Game>(TheGame),

		controllerCreator(&CreateController),

        spawnLocatorRegistration(kLocatorSpawn, "Spawn Location"),
        collLocatorRegistration(kLocatorColl, "Coll Mark"),
        physLocatorRegistration(kLocatorPys, "Phys Mark"),
//        soldierModelReg(kModelSoldier, nullptr, "soldier/Soldier", kModelPrecache | kModelPrivate, kControllerSoldier),
        soldierModelReg(kModelSoldier, nullptr, "model/robot", kModelPrecache | kModelPrivate, kControllerSoldier),
        minionModelReg(kModelMinion, nullptr, "zombie/Zombie", kModelPrecache | kModelPrivate, kControllerSoldier),
        appleModelRegistration(kModelApple,nullptr, "health/Apple", 0, kControllerCollectable),
        towerModelRegistration(kModelTower,nullptr, "tower", 0, kControllerTower),
        //soldierModelReg(kModelSoldier, nullptr, "GUS/gus", kModelPrecache | kModelPrivate, kControllerSoldier),

//        gameRigidBodyControllerRegistration(kControllerGameRigidBody, stringTable.GetString(StringID('CTRL', kControllerGameRigidBody))),

        towerControllerRegistration(kControllerTower, "Tower"),
        minionControllerRegistration(kControllerMinion, "Minion"),
        minionSpawnControllerRegistration(kControllerMinionSpawn, "Minion Spawn"),

		hostCommandObserver(this, &Game::HandleHostCommand),
		joinCommandObserver(this, &Game::HandleJoinCommand),


		hostCommand("host", &hostCommandObserver),
		joinCommand("join", &joinCommandObserver),

		forwardAction(kActionForward, kMovementForward, kSpectatorMoveForward),
		backwardAction(kActionBackward, kMovementBackward, kSpectatorMoveBackward),
		leftAction(kActionLeft, kMovementLeft, kSpectatorMoveLeft),
		rightAction(kActionRight, kMovementRight, kSpectatorMoveRight),
		upAction(kActionUp, kMovementUp, kSpectatorMoveUp),
		downAction(kActionDown, kMovementDown, kSpectatorMoveDown),
		movementAction(kActionMovement, 0, 0),
		primaryFireAction(kActionFirePrimary),
		towerOneAction(kActionTowerOne),
		towerTwoAction(kActionTowerTwo),
		towerThreeAction(kActionTowerThree)

{

	TheEngine->AddCommand(&hostCommand);
	TheEngine->AddCommand(&joinCommand);
	


	TheInputMgr->AddAction(&forwardAction);
	TheInputMgr->AddAction(&backwardAction);
	TheInputMgr->AddAction(&leftAction);
	TheInputMgr->AddAction(&rightAction);
	TheInputMgr->AddAction(&upAction);
	TheInputMgr->AddAction(&downAction);
	TheInputMgr->AddAction(&movementAction);
	TheInputMgr->AddAction(&primaryFireAction);
	TheInputMgr->AddAction(&towerOneAction);
	TheInputMgr->AddAction(&towerTwoAction);
	TheInputMgr->AddAction(&towerThreeAction);

	prevEscapeCallback = TheInputMgr->GetEscapeCallback();
	prevEscapeCookie = TheInputMgr->GetEscapeCookie();
	TheInputMgr->SetEscapeCallback(&EscapeCallback, this);

	TheWorldMgr->SetWorldCreator(&CreateWorld);

	gameFlags = 0;

	currentWorldName[0] = 0;


	TheInterfaceMgr->SetInputManagementMode(kInputManagementAutomatic);

	// Show splash
	MainWindow::OpenWindow();

}

Game::~Game()
{
	ExitCurrentGame();

	TheWorldMgr->SetWorldCreator(nullptr);
	TheMessageMgr->SetPlayerCreator(nullptr);

	TheInputMgr->SetEscapeCallback(prevEscapeCallback, prevEscapeCookie);
}

World *Game::CreateWorld(const char *name, void *cookie)
{
	return (new GameWorld(name));
}

Player *Game::CreatePlayer(PlayerKey key, void *cookie)
{
	return (new GamePlayer(key));
}

Controller *Game::CreateController(Unpacker& data)
{
	switch (data.GetType())
	{
	}

	return (nullptr);
}



void Game::EscapeCallback(void *cookie)
{
}





void Game::InitPlayerStyle(int32 *style)
{
}

void Game::HandleHostCommand(Command *command, const char *text)
{
	if (*text != 0)
	{
		ResourceName	name;

		Text::ReadString(text, name, kMaxResourceNameLength);
		HostMultiplayerGame(name, 0);
	}
}


void Game::StartSinglePlayerGame(const char *name)
{
    ExitCurrentGame();
    
    currentWorldName = name;
    
    HostMultiplayerGame(name,0);
}


// I hard coded this, so it connects to the local machine
void Game::HandleJoinCommand(Command *command, const char *text)
{
    TheMessageMgr->BeginMultiplayerGame(false);
    
    // We'll first want to provide the user with some feedback - so he'll know what he's doing.
    String<128> str("Attempting to join --> ");
    str += text;
    
    NetworkAddress address = MessageMgr::StringToAddress(text);
    if (address.GetPort() == 0)address.SetPort(kGamePort);

    
    TheEngine->Report(str, kReportError);
    
    // Now we're just going to (try to) connect to the entered address.
    NetworkAddress local_addr = TheNetworkMgr->GetLocalAddress();
    local_addr.SetPort(kGamePort);
    
    TheMessageMgr->Connect(local_addr);
}



EngineResult Game::HostMultiplayerGame(const char *name, unsigned_int32 flags)
{
	ExitCurrentGame();
	TheNetworkMgr->SetPortNumber(kGamePort);
	TheNetworkMgr->SetBroadcastPortNumber(kGamePort);
    TheNetworkMgr->Initialize();

	EngineResult result = TheMessageMgr->BeginMultiplayerGame(true);
	if (result == kNetworkOkay)
	{
		result = TheWorldMgr->LoadWorld(name);
		if (result == kWorldOkay)
		{
            GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());

			GamePlayer *player = static_cast<GamePlayer *>(TheMessageMgr->GetLocalPlayer());
            
            const Point3D pos= Point3D(0.0F, 0.0F, 2.0F);
            
            // Request an Avatar  ONLY ON SERVER !!!!!
            
            world->ReqestOjectAtLocation(pos ,kSoldierEntity,player->GetPlayerKey());
            
            // POPULATE ONLY ON SERVER !!!
            world->PopulateWorld();
            
             DisplayBoard::OpenBoard();
            TheDisplayBoard->ShowMessageText("Server");

		}
	}

	return (result);
}

EngineResult Game::JoinMultiplayerGame(const char *name, unsigned_int32 flags)
{

	EngineResult result = TheWorldMgr->LoadWorld(name);
	if (result == kWorldOkay)
	{
        DisplayBoard::OpenBoard();
        TheDisplayBoard->ShowMessageText("Client");
	}

	return (result);
}

void Game::ExitCurrentGame(void)
{
    TheMessageMgr->EndGame();
    delete TheDisplayBoard;

	gameFlags = 0;

	TheWorldMgr->UnloadWorld();    
}

void Game::RestartWorld(void)
{
	GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
	if ((world) && (world->GetSpawnLocatorCount() != 0))
	{
		StartSinglePlayerGame(currentWorldName);
	}
}

EngineResult Game::LoadWorld(const char *name)
{
	StartSinglePlayerGame(name);
	return (kEngineOkay);
}

void Game::UnloadWorld(void)
{
	ExitCurrentGame();
	
}



void Game::HandleConnectionEvent(ConnectionEvent event, const NetworkAddress& address, const void *param)
{
    switch (event)
    {
        case kConnectionQueryReceived:
        {
            World *world = TheWorldMgr->GetWorld();
            if (world){
                const char *gameName = TheEngine->GetVariable("gameName")->GetValue();
                const char *worldName = world->GetWorldName();
                
                ServerInfoMessage message(TheMessageMgr->GetPlayerCount(), TheMessageMgr->GetMaxPlayerCount(), gameName, worldName);
                TheMessageMgr->SendConnectionlessMessage(address, message);
            }
            
            break;
        }
            
        case kConnectionAttemptFailed:
            
            // The server rejected our connection.
            
            
            break;
            
        case kConnectionServerAccepted:
            
            // The server accepted our connection.
            
            // Tell the server what player styles the user has set.
            // The server will forward this information to the other players.
            
            //TheMessageMgr->SendMessage(kPlayerServer, ClientStyleMessage(static_cast<GamePlayer *>(TheMessageMgr->GetLocalPlayer())->GetPlayerStyle()));
            break;
            
        case kConnectionServerClosed:
            
            // The server was shut down.
            
            ExitCurrentGame();
            break;
            
        case kConnectionServerTimedOut:
            
            // The server has stopped responding.
            
            ExitCurrentGame();
            break;
    }
    
    Application::HandleConnectionEvent(event, address, param);
}

void Game::HandlePlayerEvent(PlayerEvent event, Player *player, const void *param)
{
    switch (event)
    {
        case kPlayerDisconnected:
        {
            Controller *controller = static_cast<GamePlayer *>(player)->GetPlayerController();
            if (controller)
            {
                //delete controller->GetTargetNode();
            }
            
            break;
        }
            
        case kPlayerTimedOut:
        {
            Controller *controller = static_cast<GamePlayer *>(player)->GetPlayerController();
            if (controller)
            {
                //delete controller->GetTargetNode();
            }
            
            break;
        }
            
        case kPlayerInitialized:
        {
            // A new player joining the game has been initialized. For each player already
            // in the game, send a message  TO constrcut the coresponding Avatar
            
            const GamePlayer *gamePlayer = static_cast<GamePlayer *>(TheMessageMgr->GetFirstPlayer());
            do
            {                
                gamePlayer = gamePlayer->GetNextPlayer();
            } while (gamePlayer);
            
            // Now tell the new player what world is being played.
            
            World *world = TheWorldMgr->GetWorld();
            if (world) player->SendMessage(GameInfoMessage(0,world->GetWorldName()));
            
            
            break;
        }
            
            
    }
    
    Application::HandlePlayerEvent(event, player, param);
}

// THIS IS CALLED WHEN ALL THE LOADING ON THE CLIENT IS DONE
void Game::HandleGameEvent(GameEvent event, const void *param)
{
    switch (event)
    {
        case kGameSynchronized:
            ClientRequestMessage message(kMessageRequestAvantar,0);
            TheMessageMgr->SendMessage(kPlayerServer,message);
            break;
    }
}

// Add all messages so they can be send over the network !!
Message *Game::CreateMessage(MessageType type, Decompressor& data) const
{
    switch (type)
    {
        case kMessageServerInfo:
            
            return (new ServerInfoMessage);
            
        case kMessageGameInfo:
            
            return (new GameInfoMessage);
            
        case kMessageCreateModel:
        {
            unsigned_int8	modelType;
            
            data >> modelType;
            return (CreateModelMessage::CreateMessage(modelType));
        }
            
        case kMessageClientMovementBegin:
        case kMessageClientMovementEnd:
        case kMessageClientMovementChange:
            
            return (new ClientMovementMessage(type));
            
        case kMessageClientFiringPrimaryBegin:
        case kMessageClientFiringEnd:
            
            return (new ClientFiringMessage(type));
            
        case kMessageCreateCharacter:
            return(new CreateCharacterMessage(type));
            
        case kMessageRequestAvantar:
            
            return(new ClientRequestMessage(type));

//        case kMessageMinionDead:
//            return new MinionDeadMessage();
            
    }
    
    return (nullptr);
}

void Game::ReceiveMessage(Player *sender, const NetworkAddress& address, const Message *message) {

}


void Game::ApplicationTask(void)
{
}
