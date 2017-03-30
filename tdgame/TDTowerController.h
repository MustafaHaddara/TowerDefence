#ifndef SimpleTower_h
#define SimpleTower_h

#include "TSController.h"
#include "TDMinionController.h"

namespace MMGame
{
	using namespace Tombstone;

	// Controller types
	enum
	{
		kControllerTower = 'Turr'
	};

	enum
	{
		kModelTower = 'Turt'
	};

	class TowerController final : public Controller {
	private:
		int32			health = 50;
		const int       SHOOT_DURATION = 1000 * 2;  // Fire every 2 seconds
		const float     RANGE = 30.0f;              // Range within aiming
		const int       DAMAGE_DEALT = 20;          // Damage per shot

		int             myCount;                    // for firing
		Transform4D     originalTransform;          // The target's original transform
		Vector3D        originalView;               // The direction turret is facing before any rotation
		Node			*turretBarrel;              // Marker on Turret Model from which to shoot
		Node*           currentTarget = nullptr;

	public:
		TowerController();
		~TowerController();
		void PreprocessController(void) override;
		void takeDamage(int32 damage);
		int32 getTowerHealth(void)
		{
			return(health);
		}

		void setTowerHealth(int32 newHealth)
		{
			health = newHealth;
		}
		void MoveController(void) override;
		MinionController* GetTargetPoint(int32 range, Vector3D *out);
		ControllerMessage *CreateMessage(ControllerMessageType type) const override;
		void ReceiveMessage(const ControllerMessage *message) override;
		bool OrientationToMinion(Node* minion, int32 max_dist, Vector3D* out);

		enum {
			kTowerRotateMessage,
		};

	};

	class TowerRotateMessage : public ControllerMessage {
		friend class TowerController;

	private:

		Vector3D target;

	public:

		TowerRotateMessage(ControllerMessageType type, int32 index);
		TowerRotateMessage(ControllerMessageType type, const Vector3D& trgt, int32 index, unsigned_int32 flags = 0);
		~TowerRotateMessage();

		Vector3D getTarget() const {
			return target;
		}

		void CompressMessage(Compressor& data) const override;
		bool DecompressMessage(Decompressor& data) override;
	};

}

#endif
