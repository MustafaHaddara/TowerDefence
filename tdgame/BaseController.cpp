
//
//  TDBaseController.cpp
//  Tombstone
//
//  Created by Mustafa Haddara on 2017-03-06.
//
//

#include <stdio.h>
#include "BaseController.h"

#include "TSController.h"
#include "TSNode.h"
#include "TSMarkers.h"
#include "TSZones.h"
#include "MMMultiplayer.h"

namespace MMGame {
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
