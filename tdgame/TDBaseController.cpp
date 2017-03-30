#include "TDBaseController.h"
#include "TDMultiplayer.h"

#include "TSController.h"
#include "TSNode.h"
#include "TSMarkers.h"
#include "TSZones.h"

namespace TDGame {
	using namespace Tombstone;

	using Tombstone::ControllerType;

	//BaseController -> rigidbody
	BaseController::BaseController() : RigidBodyController(kControllerBase) {
		SetRigidBodyType(kRigidBodyBase);
		shapeInitFlag = false;
	}

	BaseController::~BaseController() {

	}

	void BaseController::PreprocessController(void) {
		RigidBodyController::PreprocessController();
	}

	// dont generate these programmatically instead just attach the controller in the world editor



	void BaseController::takeDamage(int32 damage) {
		// fighterController => instance of player
		// 

		health = health - damage;
		// take damage only called by the server.
		TheMessageMgr->SendMessageAll(UpdateBaseHealthMessage(health, GetControllerIndex()));
		

	}
}
