#ifndef MMCameras_h
#define MMCameras_h

#include "TSCameras.h"
#include "TSModels.h"
#include "MMBase.h"

namespace BaseInvaders
{
	using namespace Tombstone;


	extern const float kCameraPositionHeight;


	class ModelCamera : public FrustumCamera
	{
		private:

			Model		*targetModel;

		protected:

			ModelCamera();

		public:

			~ModelCamera();

			Model *GetTargetModel(void) const
			{
				return (targetModel);
			}

			void SetTargetModel(Model *model)
			{
				targetModel = model;
			}
	};


	class FirstPersonCamera final : public ModelCamera
	{
		public:

			FirstPersonCamera();
			~FirstPersonCamera();

			void MoveCamera(void) override;


			void setMovementFlag(int);
	};


	class ChaseCamera final : public ModelCamera
	{
		public:

			ChaseCamera();
			~ChaseCamera();

			void MoveCamera(void) override;
	};
}


#endif
