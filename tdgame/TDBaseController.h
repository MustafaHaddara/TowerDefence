#ifndef TDBaseController_h
#define TDBaseController_h

#include "TSController.h"
#include "TSPhysics.h"

namespace TDGame
{
	using namespace Tombstone;

	// Controller types
	enum
	{
		kControllerBase = 'cbas'
	};
	
	class BaseController final : public RigidBodyController {

	private:
		
		int32           health = 1000;       // Initial health
		bool			shapeInitFlag;

	public:
		/**
		* \brief constructor
		*/
		BaseController();
		/**
		* \brief default deconstructor
		*/
		~BaseController();
		/**
		* \brief Initializes controller
		*
		* This is called once to initialize the controller
		*/
		void PreprocessController(void) override;
		/**
		* \ health is recalculated and sent to clients
		*/
		void takeDamage(int32 damage);
		
		/**
		* \brief get health method
		*/
		int32 getHealth(void)
		{
			return(health);
		}
		/**
		* \brief set health method
		*/
		void setHealth(int32 newHealth)
		{
			health = newHealth;
		}
// commeneted out because giving an error. No congroller messages that are supposed to be recieved by this controller.
//		ControllerMessage *CreateMessage(ControllerMessageType type) const override;
//		void ReceiveMessage(const ControllerMessage *message) override;

	};

}

#endif /* TDMinionController_h */
