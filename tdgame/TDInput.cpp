#include "TDInput.h"
#include "TDGame.h"
#include "TDMultiPlayer.h"

#include "TDFighter.h"
#include "TDGameWorld.h"


using namespace TDGame;


MovementAction::MovementAction(ActionType type, unsigned_int32 moveFlag, unsigned_int32 specFlag) : Action(type)
{
	movementFlag = moveFlag;
	spectatorFlag = specFlag;
}

MovementAction::~MovementAction()
{
}

void MovementAction::HandleEngage(void)
{


	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		const FighterController *controller = static_cast<const GamePlayer *>(player)->GetPlayerController();
		if (controller)
		{
			ClientMovementMessage message(kMessageClientMovementBegin, movementFlag, controller->GetLookAzimuth(), controller->GetLookAltitude());
			TheMessageMgr->SendMessage(kPlayerServer, message);
			return;
		}
	}

	GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
	if (world)
	{
		SpectatorCamera *camera = world->GetSpectatorCamera();
		camera->SetSpectatorFlags(camera->GetSpectatorFlags() | spectatorFlag);
	}
}

void MovementAction::HandleDisengage(void)
{
	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		const FighterController *controller = static_cast<const GamePlayer *>(player)->GetPlayerController();
		if (controller)
		{
			ClientMovementMessage message(kMessageClientMovementEnd, movementFlag, controller->GetLookAzimuth(), controller->GetLookAltitude());
			TheMessageMgr->SendMessage(kPlayerServer, message);
			return;
		}
	}

	GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
	if (world)
	{
		SpectatorCamera *camera = world->GetSpectatorCamera();
		camera->SetSpectatorFlags(camera->GetSpectatorFlags() & ~spectatorFlag);
	}
}

void MovementAction::HandleMove(int32 value)
{
	static const unsigned_int32 movementFlags[8] =
	{
		kMovementForward,
		kMovementForward | kMovementRight,
		kMovementRight,
		kMovementRight | kMovementBackward,
		kMovementBackward,
		kMovementBackward | kMovementLeft,
		kMovementLeft,
		kMovementLeft | kMovementForward
	};

	static const unsigned_int32 spectatorFlags[8] =
	{
		kSpectatorMoveForward,
		kSpectatorMoveForward | kSpectatorMoveRight,
		kSpectatorMoveRight,
		kSpectatorMoveRight | kSpectatorMoveBackward,
		kSpectatorMoveBackward,
		kSpectatorMoveBackward | kSpectatorMoveLeft,
		kSpectatorMoveLeft,
		kSpectatorMoveLeft | kSpectatorMoveForward
	};

	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		const FighterController *controller = static_cast<const GamePlayer *>(player)->GetPlayerController();
		if (controller)
		{
			unsigned_int32 flags = (value >= 0) ? movementFlags[value] : 0;
			ClientMovementMessage message(kMessageClientMovementChange, flags, controller->GetLookAzimuth(), controller->GetLookAltitude());
			TheMessageMgr->SendMessage(kPlayerServer, message);
			return;
		}
	}

	GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
	if (world)
	{ 
		unsigned_int32 flags = (value >= 0) ? spectatorFlags[value] : 0; 
		SpectatorCamera *camera = world->GetSpectatorCamera();
		camera->SetSpectatorFlags((camera->GetSpectatorFlags() & ~kSpectatorPlanarMask) | flags); 
	} 
} 

 
LookAction::LookAction(ActionType type) : Action(type) 
{ 
}

LookAction::~LookAction() 
{
}

void LookAction::HandleUpdate(float value)
{
	
}


FireAction::FireAction(ActionType type) : Action(type)
{
}

FireAction::~FireAction()
{
}

void FireAction::HandleEngage(void)
{
	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		FighterController *controller = static_cast<const GamePlayer *>(player)->GetPlayerController();
		if (controller)
		{
			// The player is controlling a fighter, so either fire the weapon or interact with a panel effect.

			const FighterInteractor *interactor = controller->GetFighterInteractor();
			const Node *interactionNode = interactor->GetInteractionNode();
			if ((!interactionNode) || (interactionNode->GetNodeType() != kNodeEffect))
			{
				// No panel effect, so fire the weapon.
				ClientFiringMessage message((GetActionType() == kActionFirePrimary) ? kMessageClientFiringPrimaryBegin : kMessageClientFiringSecondaryBegin, controller->GetLookAzimuth(), controller->GetLookAltitude());
				TheMessageMgr->SendMessage(player->GetPlayerKey(), message);
			}
			else
			{
				// The player's interacting with a panel effect.

				//ClientInteractionMessage message(kMessageClientInteractionBegin, interactor->GetInteractionPosition());
				//TheMessageMgr->SendMessage(kPlayerServer, message);
			}
		}
		else
		{
			// There's no fighter yet, so spawn the player in response to the fire button being pressed.

			if (TheMessageMgr->GetSynchronizedFlag())
			{
				if ((!TheMessageMgr->GetServerFlag()))
				{
					TheMessageMgr->SendMessage(kPlayerServer, ClientMiscMessage(kMessageClientSpawn));
				}
			}
		}
	}
}

void FireAction::HandleDisengage(void)
{
	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		FighterController *controller = static_cast<const GamePlayer *>(player)->GetPlayerController();
		if (controller)
		{
			// The player is controlling a fighter, so either stop firing the weapon or interact with a panel effect.

			const FighterInteractor *interactor = controller->GetFighterInteractor();
			const Node *interactionNode = interactor->GetInteractionNode();
			if ((!interactionNode) || (interactionNode->GetNodeType() != kNodeEffect))
			{
				// No panel effect, so stop firing the weapon.

				ClientFiringMessage message(kMessageClientFiringEnd, controller->GetLookAzimuth(), controller->GetLookAltitude());
				TheMessageMgr->SendMessage(kPlayerServer, message);
			}
			else
			{
				// The player's interacting with a panel.

				//ClientInteractionMessage message(kMessageClientInteractionEnd, interactor->GetInteractionPosition());
				//TheMessageMgr->SendMessage(kPlayerServer, message);
			}
		}
	}
}

TowerAction::TowerAction(ActionType type) : Action(type)
{
}

TowerAction::~TowerAction()
{
}

void TowerAction::HandleEngage(void)
{
	const Player *player = TheMessageMgr->GetLocalPlayer();
			
	unsigned messageType;

	switch (GetActionType())
	{
	case kActionTowerOne:
		messageType = kMessageTowerOne;
		break;
	case kActionTowerTwo:
		messageType = kMessageTowerTwo;
		break;
	case kActionTowerThree:
		messageType = kMessageTowerThree;
		break;
	}

	TheMessageMgr->SendMessage(kPlayerServer, ClientTowerSelectMessage(messageType));
}



