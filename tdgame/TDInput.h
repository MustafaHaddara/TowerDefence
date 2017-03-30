#ifndef TDInput_h
#define TDInput_h


#include "TSInput.h"
#include "TDBase.h"


namespace TDGame
{
	using namespace Tombstone;
	
	// Input action types
	enum : ActionType
	{
		kActionForward				= 'frwd',
		kActionBackward				= 'bkwd',
		kActionLeft					= 'left',
		kActionRight				= 'rght',
		kActionUp					= 'jump',
		kActionDown					= 'down',
		kActionMovement				= 'move',
		kActionHorizontal			= 'horz',
		kActionVertical				= 'vert',
		kActionFirePrimary			= 'fire',
		kActionFireSecondary		= 'trig',
		kActionUse					= 'uobj',
		kActionTowerOne				= 'twr1',
		kActionTowerTwo				= 'twr2',
		kActionTowerThree			= 'twr3',
		kActionSpikeShooter			= 'spsh',
		kActionGrenadeLauncher		= 'gren',
		kActionQuantumCharger		= 'qchg',
		kActionRocketLauncher		= 'rock',
		kActionPlasmaGun			= 'plas',
		kActionProtonCannon			= 'pcan',
		kActionSpecialWeapon		= 'spec',
		kActionNextWeapon			= 'next',
		kActionPrevWeapon			= 'prev',
		kActionFlashlight			= 'lite',
		kActionCameraView			= 'camr',
		kActionScoreboard			= 'scor',
		kActionChat					= 'mess',
		kActionLoad					= 'load',
		kActionSave					= 'save'
	};

	// Movement types
	enum
	{
		kMovementForward			= 1 << 0,
		kMovementBackward			= 1 << 1,
		kMovementLeft				= 1 << 2,
		kMovementRight				= 1 << 3,
		kMovementUp					= 1 << 4,
		kMovementDown				= 1 << 5,
		kMovementPlanarMask			= 15
	};

	/**
	* \brief Action for character movement.
	*/
	class MovementAction : public Action
	{
		private:

			unsigned_int32		movementFlag;
			unsigned_int32		spectatorFlag;

		public:
			/**
			* \brief Constructor
			* \param type Type of action
			* \param moveFlag Movement direction
			* \param specFlag Spectator movement direction
			*/
			MovementAction(ActionType type, unsigned_int32 moveFlag, unsigned_int32 specFlag);
			/**
			* \brief Destructor
			*/
			~MovementAction();

			/**
			* \brief Handles when movement begins
			*/
			void HandleEngage(void);
			/**
			* \brief Handles when movement ends
			*/
			void HandleDisengage(void);
			/**
			* \brief Handles when movement changes
			*/
			void HandleMove(int32 value);
	};

	/**
	* \brief Action for firing weapons.
	*/
	class FireAction : public Action
	{
		public:
			/**
			* \brief Constructor
			* \param type Type of action
			*/
			FireAction(ActionType type);
			/**
			* \brief Destructor
			*/
			~FireAction();

			/**
			* \brief Handles when firing begins
			*/
			void HandleEngage(void);
			/**
			* \brief Handles when firing ends
			*/
			void HandleDisengage(void);
	};

	/**
	* \brief Action for selecting towers.
	*/
	class TowerAction : public Action
	{
	public:
		/**
		* \brief Constructor
		* \param type Type of action
		*/
		TowerAction(ActionType type);
		/**
		* \brief Destructor
		*/
		~TowerAction();
		/**
		* \brief Handles when a tower is selected
		*/
		void HandleEngage(void);
	};


 

}


#endif

