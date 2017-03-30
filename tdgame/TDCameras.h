#ifndef TDCameras_h
#define TDCameras_h


#include "TSCameras.h"
#include "TSModels.h"
#include "TDBase.h"


namespace TDGame
{
	using namespace Tombstone;


	extern const float kCameraPositionHeight;

    /**
     * \brief Base camera class.
     *
     * Every camera follows a model in the world and inherits from this class.
     */
	class ModelCamera : public FrustumCamera {
    private:

        Model		*targetModel;

    protected:

        /**
         * \brief Constructor
         */
        ModelCamera();

    public:

        /**
         * \brief Destructor
         */
        ~ModelCamera();

        /**
         * \brief Get the model the camera is currently following.
         */
        Model *GetTargetModel(void) const {
            return (targetModel);
        }

        /**
         * \brief Set the model for the camera to start following.
         * \param model The model that the camera should begin following.
         */
        void SetTargetModel(Model *model) {
            targetModel = model;
        }
	};

    /**
     * \brief A camera which follows a model from behind.
     *
     * This is in the style of a "Third Person" camera.
     */
	class ChaseCamera final : public ModelCamera {
    public:

        /**
         * \brief Constructor
         */
        ChaseCamera();
        
        /**
         * \brief Destructor
         */
        ~ChaseCamera();

        /**
         * \brief Move the camera
         *
         * The camera moves to follow its target model.
         */
		void MoveCamera(void) override;
	};
}


#endif

