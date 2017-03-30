#ifndef TDFighter_h
#define TDFighter_h

#include "TSWorld.h"

#include "TDMultiplayer.h"
#include "TDCharacter.h"
#include "TDGamePlayer.h"

namespace TDGame
{
	using namespace Tombstone;
    
    ;;class GamePlayer;
    
        // Model types
       enum : ModelType
        {
                kModelSoldier                   = 'sold',
                kModelMinion                    = 'mmin'
        };
    
    // an enum to recgonize soldier's controller
    enum{
        kControllerSoldier
    };

    // Animator types
	enum : AnimatorType
	{
		kAnimatorSpineTwist			= 'spin',
		kAnimatorScale				= 'scal',
		kAnimatorWeaponIK			= 'wpik'
	};
    
    // Interaction Type
	enum : InteractionType
	{
		kInteractionTake			= 'TAKE'
	};

    // Icon types
	enum
	{
		kFighterIconChat,
		kFighterIconCount
	};

    // Fighter state types
	enum
	{
		kFighterDead				= 1 << 0,
		kFighterFiringPrimary		= 1 << 1,
		kFighterFiringSecondary		= 1 << 2,
		kFighterTargetDistance		= 1 << 3,

		kFighterFiring				= kFighterFiringPrimary | kFighterFiringSecondary
	};

    /** Different motion enum
     *  They are called and received to tell SetFighterMotion
     *  what animation should the Fighter model be doing
     */
	enum
	{
		kFighterMotionNone,
		kFighterMotionStop,
		kFighterMotionStand,
		kFighterMotionForward,
		kFighterMotionBackward,
		kFighterMotionTurnLeft,
		kFighterMotionTurnRight,
		kFighterMotionJump,
		kFighterMotionDeath
	};

    /**
     * \brief SpineTwistAnimator inherits Animator
     *
     *  Animator is a default class in Tombstone engine which
     *  deals with animation for model
     */
    class SpineTwistAnimator : public Animator
    {
    private:
        
        int32			superNodeTransformIndex[2];
        
        // SpineRotation presented in a Quaternion form
        Quaternion		spineRotation;
        
        SpineTwistAnimator();
        
    public:
        
        /**
         * \brief Constructor used to create an animator for spine
         */
        SpineTwistAnimator(Model *model, Node *node);
        
        /**
         * \brief Deconstructor
         */
        ~SpineTwistAnimator();
        
        /**
         * \brief Assign a new value to the spineRotation
         * \param spineRotation The rotation information stored in Quaternion format
         */
        void SetSpineRotation(const Quaternion& q)
        {
            spineRotation = q;
        }
        
        /**
         * \brief Preprocess Animator before it's used in the gameworld
         */
        void PreprocessAnimator(void) override;
        
        /**
         * \brief Configurate Animator
         */
        void ConfigureAnimator(void) override;
        
        /**
         * \brief Move Animator
         */
        void MoveAnimator(void) override;
    };

	class FighterController;

    /**
     * \brief Iteractor
     */
	class FighterInteractor final : public Interactor
	{
		private:

			FighterController	*fighterController;

		public:
            /**
             * \brief Interactor to contain fighterController
             */
			FighterInteractor(FighterController *controller);
			~FighterInteractor();

			void HandleInteractionEvent(InteractionEventType type, Node *node, const InteractionProperty *property, const Point3D *position = nullptr) override;
	};

    /**
     * \brief FighterController are used to manipulate fighter
     */
	class FighterController : public GameCharacterController
	{
		private:
        
			unsigned_int32			fighterFlags;   // Flag for fighter
			Link<Player>			fighterPlayer;
 
			float					primaryAzimuth;
			float					modelAzimuth;
 
			float					lookAzimuth;            // The azimuth camera is looking at
			float					lookAltitude;           // The altitude camera is look at
			float					deltaLookAzimuth;       // The difference of azimuth looking at between two different moment
			float					deltaLookAltitude;      // The difference of altitude looking at between two different moment
			float					lookInterpolateParam; 
 
			Point3D					previousCenterOfMass;   // The coordinates of mass center

			unsigned_int32			movementFlags;  // Flag for movement
			int32					fighterMotion;  // Fighter motion
			bool					motionComplete; // If the motion is completed

			Vector3D				firingDirection;    // The direction the avatar is firing at
			float					targetDistance;     // The Distance between avatar and target

			int32					deathTime;          // Time when a player is dead
			int32					damageTime;         // Time for a damge lasts
			unsigned_int32			weaponSwitchTime;   // Time needed for a player to switch weapon

			Model					*weaponModel;       // 3D model of weapon
			const Marker			*weaponFireMarker;  // Marker for fire
			const Marker			*weaponMountMarker; // Marker for mount

			int32					iconIndex;          // icon index
			QuadEffect				*iconEffect;

			Node					*mountNode;         // Node of mount
			Light					*flashlight;        // Node of light

			MergeAnimator			*rootAnimator;      // Pointer to the root animator
			MergeAnimator			*mergeAnimator;     // Animator that merge animation
			BlendAnimator			*blendAnimator;     // Animator that blend animation
       
            /**
             * \brief FrameAnimator points to transformation from an animation source
             */
			FrameAnimator			*frameAnimator[2];
        
            SpineTwistAnimator			*spineTwistAnimator;
        
            int playerkey;

			FighterInteractor								fighterInteractor;      // Interactor for fighter
			Observer<FighterController, WorldObservable>	worldUpdateObserver;    // Observer for worldUpdate
			FrameAnimatorObserver<FighterController>		frameAnimatorObserver;  // Ovserver for frameAnimator

            /**
             * \brief Set the orientation with given azimuth and altitude
             * \param lookAzimuth The azimuth a camera/player is looking at
             * \param lookAltitude The altitude a camera/player is looking at
             */
			void SetOrientation(float azm, float alt)
			{
				lookAzimuth = azm;
				lookAltitude = alt;
				lookInterpolateParam = 0.0F;
			}
        
            /**
             * \brief Set the mount node transformation
             */
			void SetMountNodeTransform(void);

            /**
             * \brief Handle the event to update game world
             */
			void HandleWorldUpdate(WorldObservable *observable);
        
            /**
             * \brief Handle the animation events
             */
			void HandleAnimationEvent(FrameAnimator *animator, CueType cueType);

            /**
             * \brief Handle the motion completion
             */
			static void HandleMotionCompletion(Interpolator *interpolator, void *cookie);

		protected:

            /**
             * \brief Get the azimuth of model
             */
			float GetModelAzimuth(void) const
			{
				return (modelAzimuth);
			}

            /**
             * \brief Get the interpolated azimuth that a player is looking at
             */
			float GetInterpolatedLookAzimuth(void) const
			{
				return (lookAzimuth + deltaLookAzimuth * lookInterpolateParam);
			}

            /**
             * \brief Get the interpolated altitude that a player is looking at
             */
			float GetInterpolatedLookAltitude(void) const
			{
				return (lookAltitude + deltaLookAltitude * lookInterpolateParam);
			}

            /**
             * \brief Check if the damage time has ended
             */
			bool DamageTimeExpired(int32 reset)
			{
				if (damageTime > 0)
				{
					return (false);
				}

				damageTime = reset;
				return (true);
			}

            /**
             * \brief Get the animator that merges
             */
			MergeAnimator *GetMergeAnimator(void) const
			{
				return (mergeAnimator);
			}

            /**
             * \brief Get the animator that blends
             */
			BlendAnimator *GetBlendAnimator(void) const
			{
				return (blendAnimator);
			}

            /**
             * \brief Get the frame animatior with given index
             */
			FrameAnimator *GetFrameAnimator(int32 index) const
			{
				return (frameAnimator[index]);
			}

            /**
             * \brief Set the observer of fram animator
             */
			void SetFrameAnimatorObserver(FrameAnimator::ObserverType *observer)
			{
				frameAnimator[0]->SetObserver(observer);
				frameAnimator[1]->SetObserver(observer);
			}

		public:

            /**
             * \brief This is where ray casting is performed
             *
             * CollisionData has a prototype of point3D and from collisionData.position we are able to ge the coordinates of cillision
             */
            void fireLaser(void);
        
			FighterController(ControllerType type);

            // Different message type for fighter's state
			enum
			{
				kFighterMessageEngageInteraction = kRigidBodyMessageBaseCount,
				kFighterMessageDisengageInteraction,
				kFighterMessageBeginMovement,
				kFighterMessageEndMovement,
				kFighterMessageChangeMovement,
				kFighterMessageBeginShield,
				kFighterMessageEndShield,
				kFighterMessageBeginIcon,
				kFighterMessageEndIcon,
				kFighterMessageTeleport,
				kFighterMessageLaunch,
				kFighterMessageLand,
				kFighterMessageUpdate,
				kFighterMessageWeapon,
				kFighterMessageEmptyAmmo,
				kFighterMessageDamage,
				kFighterMessageDeath,
                kFireLaser
			};

            /**
             * \brief Deconstructor
             */
			~FighterController();

            /**
             * \brief Return the flag of fighter
             */
			unsigned_int32 GetFighterFlags(void) const
			{
				return (fighterFlags);
			}

            /**
             * \brief Set the flag of fighter
             */
			void SetFighterFlags(unsigned_int32 flags)
			{
				fighterFlags = flags;
		
            }

            /**
             * \brief Set the player to a fighter
             */
			void SetFighterPlayer(Player *player)
			{
				fighterPlayer = player;
			}

            /**
             * \brief Return the primary azimuth
             */
			float GetPrimaryAzimuth(void) const
			{
				return (primaryAzimuth);
			}

            /**
             * \brief Set the primary azimuth
             */
			void SetPrimaryAzimuth(float azimuth)
			{
				primaryAzimuth = azimuth;
			}

            /**
             * \brief Return the azimuth looking at
             */
			float GetLookAzimuth(void) const
			{
				return (lookAzimuth);
			}

            /**
             * \brief Set the azimuth looking at
             */
			void SetLookAzimuth(float azimuth)
			{
				lookAzimuth = azimuth;
			}

            /**
             * \brief Get the altitude looking at
             */
			float GetLookAltitude(void) const
			{
				return (lookAltitude);
			}

            /**
             * \brief Set the altitude looking at
             */
			void SetLookAltitude(float altitude)
			{
				lookAltitude = altitude;
			}

            /**
             * \brief Return the movement flag
             */
			unsigned_int32 GetMovementFlags(void) const
			{
				return (movementFlags);
			}

            /**
             * \brief Set the movement flag
             */
			void SetMovementFlags(unsigned_int32 flags)
			{
				movementFlags = flags;
			}
        
            /**
             * \brief Get the fighter motion enum
             */
			int32 GetFighterMotion(void) const
			{
				return (fighterMotion);
			}

            /**
             * \brief Return the direction camera/player is looking at
             */
			const Vector3D& GetFiringDirection(void) const
			{
				return (firingDirection);
			}

            /**
            * \brief Return the distance between something and target
            */
			float GetTargetDistance(void) const
			{
				return (targetDistance);
			}

            /**
             * \brief Set the distance between something and target
             */
			void SetTargetDistance(float distance)
			{
				targetDistance = distance;
			}

            /**
             * \brief Return the weapon model
             */
			Model *GetWeaponModel(void) const
			{
				return (weaponModel);
			}

            /**
             * \brief Return the marker of weapon fire
             */
			const Marker *GetWeaponFireMarker(void) const
			{
				return (weaponFireMarker);
			}


            /**
             * \brief Return the light of flash
             */
			Light *GetFlashlight(void) const
			{
				return (flashlight);
			}

            /**
             * \brief Return the fighter interactor
             */
			FighterInteractor *GetFighterInteractor(void)
			{
				return (&fighterInteractor);
			}

            /**
             * \brief Return the interactor for fighter(constant version)
             */
			const FighterInteractor *GetFighterInteractor(void) const
			{
				return (&fighterInteractor);
			}

        
            /**
             * \brief Pack method
             *
             * FighterController is inherited from  GameCharacterController. In GameCharacterController, Pack method is called to chunk data and compress. In FighterController's Pack method, we chunck information depending on special strings ('Flag','ORNT','DGTM') and take a certain length of data following those strings to put into the data pointer.
             */
			void Pack(Packer& data, unsigned_int32 packFlags) const override;
        
            /**
             * \brief Unpack the data for further use
             */
			void Unpack(Unpacker& data, unsigned_int32 unpackFlags) override;
        
            /**
             * \brief Unpack chunk with given chunk header
             */
			void UnpackChunk(const ChunkHeader *chunkHeader, Unpacker& data, unsigned_int32 unpackFlags);

            /**
             * \brief Controller needs to be proprocessed before it's actually used in the game world
             */
			void PreprocessController(void) override;
        
            /**
             * \brief Move the controller
             */
			void MoveController(void) override;

            /**
             * \brief Message creator with given message type
             */
			ControllerMessage *CreateMessage(ControllerMessageType type) const override;
        
            /**
             * \brief Control what to do when receiving message
             */
			void ReceiveMessage(const ControllerMessage *message) override;
        
            /**
             * \brief Send an initial state message with player information
             */
			void SendInitialStateMessages(Player *player) const override;
        
            /**
             * \brief This function is being called by message manager at a constent rate
             */
			void SendSnapshot(void) override;

            /**
             * \brief Deal with new geometry contact
             */
			RigidBodyStatus HandleNewGeometryContact(const GeometryContact *contact) override;

            /**
             * \brief Deal with entering the world, positioning.
             */
			void EnterWorld(World *world, const Point3D& worldPosition) override;

            /**
             * \brief Caleed when damage is made
             */
			CharacterStatus Damage(Fixed damage, unsigned_int32 flags, GameCharacterController *attacker, const Point3D *position = nullptr, const Vector3D *impulse = nullptr) override;
        
            /**
             * \brief Called when something is killed
             */
			void Kill(GameCharacterController *attacker, const Point3D *position = nullptr, const Vector3D *impulse = nullptr) override;
        
            /**
             * \brief Update the orientation information
             */
			void UpdateOrientation(float azm, float alt);
        
            /**
             * \brief Called when fighter starts moving
             */
			void BeginMovement(unsigned_int32 flag, float azm, float alt);
        
            /**
             * \brief Called when fighter stops moving
             */
			void EndMovement(unsigned_int32 flag, float azm, float alt);
        
            /**
             * \brief Called to change fighter movement
             */
			void ChangeMovement(unsigned_int32 flags, float azm, float alt);

            /**
             * \brief Called when fighter starts firing
             */
			void BeginFiring(bool primary, float azm, float alt);
        
            /**
             * \brief Called when fighter stops firing
             */
			void EndFiring(float azm, float alt);

        
            /**
             * \brief Set the current weapon
             */
			void SetWeapon(int32 weaponIndex, int32 weaponControllerIndex);

            /**
             * \brief Play the sound effect
             */
			void PlayInventorySound(int32 inventoryIndex);

            /**
             * \brief Acitvate flash light
             */
			void ActivateFlashlight(void);
        
            /**
             * \brief Deactivate flash light
             */
			void DeactivateFlashlight(void);
        
            /**
             * \brief Toggle flash light
             */
			void ToggleFlashlight(void);

    
            /**
             * \brief Set the mask for perspective exclustion
             */
			void SetPerspectiveExclusionMask(unsigned_int32 mask) const;

            /**
            * \brief Set the fighter style
            */
			virtual void SetFighterStyle(const int32 *style, bool prep = true);
        
            /**
             * \brief SetFighterMotion method assigns animation of model to different states/process of the game
             */
			virtual void SetFighterMotion(int32 motion);

            /**
             * \brief Animate fighter consructor
             */
            void AnimateFighter(void);
        
        
        /**
         * \brief Set the player key
         */
        void SetPlayerKey(int key){
            playerkey=key;
        }
        
    };

    /**
     * \brief For message passing.
     *
     *  We need to create, send and receive message properly.
     So that different components in the game can exchange
     data and coordinate
     */
	class CreateFighterMessage : public CreateModelMessage
	{
		private:

			float				initialAzimuth;
			float				initialAltitude;

			unsigned_int32		movementFlags;

			int32				weaponIndex;
			int32				weaponControllerIndex;

			int32				playerKey;

        public:

            /**
             * \brief Create the fighter message with a specified type of model message type
             */
			CreateFighterMessage(ModelMessageType type);
        
            /**
             * \brief Deconstructor
             */
			~CreateFighterMessage();

		public:
            /**
             * \brief FighterMessage contain fighter index, location and some other important information
             *
             * Create a message which contains multiple data
             */
			CreateFighterMessage(ModelMessageType type, int32 fighterIndex, const Point3D& position, float azm, float alt, unsigned_int32 movement, int32 weapon, int32 weaponController, int32 key);

            /**
             * \brief Get the initial azimuth
             */
			float GetInitialAzimuth(void) const
			{
				return (initialAzimuth);
			}

            /**
             * \brief Get the initial altitude
             */
			float GetInitialAltitude(void) const
			{
				return (initialAltitude);
			}

            /**
             * \brief Get movement flag
             */
			unsigned_int32 GetMovementFlags(void) const
			{
				return (movementFlags);
			}

            /**
             * \brief Get weapon index
             */
			int32 GetWeaponIndex(void) const
			{
				return (weaponIndex);
			}

            /**
             * \brief Get weapon controller index
             */
			int32 GetWeaponControllerIndex(void) const
			{
				return (weaponControllerIndex);
			}

            /**
             * \brief Get player key
             */
			int32 GetPlayerKey(void) const
			{
				return (playerKey);
			}

            /**
             * \brief A meesage needs to be compressed and serialized before sending out
             */
			void CompressMessage(Compressor& data) const override;
        
            /**
             * \brief A meesage needs to be decompressed and deserialized before sending out
             */
            bool DecompressMessage(Decompressor& data) override;
	};



    /**
     * \brief FighterMovementMessage contains controller, position and velocity information
     */
	class FighterMovementMessage : public CharacterStateMessage
	{
		friend class FighterController;

		private:

			float				movementAzimuth;        // Azimuth of movement
			float				movementAltitude;       // Altitude of movement
			unsigned_int32		movementFlag;           // Flag of movement

            /**
             * \brief Create a movement message with specified controller message type and controller index
             */
			FighterMovementMessage(ControllerMessageType type, int32 controllerIndex);

		public:

            /**
             * \brief Create a movement message with lots of information
             */
			FighterMovementMessage(ControllerMessageType type, int32 controllerIndex, const Point3D& position, const Vector3D& velocity, float azimuth, float altitude, unsigned_int32 flag);
            /**
             * \brief Deconstructor
             */
			~FighterMovementMessage();

            /**
             * \brief Return the azimuth of movement
             */
			float GetMovementAzimuth(void) const
			{
				return (movementAzimuth);
			}

            /**
             * \brief Return the altitude of movement
             */
			float GetMovementAltitude(void) const
			{
				return (movementAltitude);
			}

            /**
             * \brief Return the movement flag
             */
			int32 GetMovementFlag(void) const
			{
				return (movementFlag);
			}

            /**
             * \brief Compress the message before sending it out
             */
			void CompressMessage(Compressor& data) const override;
        
            /**
             * \brief Decompress the message after receiving it
             */
			bool DecompressMessage(Decompressor& data) override;
	};

    /**
     * \brief FighterUpdateMessage contains controller index and position information
     */
	class FighterUpdateMessage : public ControllerMessage
	{
		friend class FighterController;

		private:

			float		updateAzimuth;
			float		updateAltitude;

            /**
             * \brief Create a fighter update message with controller index
             */
			FighterUpdateMessage(int32 controllerIndex);

		public:

            /**
             * \brief Create a fighter update message with controller index, azimuth and altitude
             */
			FighterUpdateMessage(int32 controllerIndex, float azimuth, float altitude);
        
            /**
             * \brief Deconstructor
             */
			~FighterUpdateMessage();
        
            /**
             * \brief Get the azimuth after update
             */
			float GetUpdateAzimuth(void) const
			{
				return (updateAzimuth);
			}

            /**
             * \brief Get the altitude after update
             */
			float GetUpdateAltitude(void) const
			{
				return (updateAltitude);
			}

            /**
             * \brief A message needs to be compressed before sending out
             */
			void CompressMessage(Compressor& data) const override;
        
            /**
             * \brief A message needs to be decompressed after receiving it
             */
			bool DecompressMessage(Decompressor& data) override;
	};

    /**
     * \brief Messages about the weapon setting of a fighter
     */
	class FighterWeaponMessage : public ControllerMessage
	{
		friend class FighterController;

		private:

			int32		weaponIndex;                // Index for weapon
			int32		weaponControllerIndex;      // Index for weapon's controller

            /**
             * \brief Create a fighter weapon message with a controller index
             */
			FighterWeaponMessage(int32 controllerIndex);

		public:

            /**
             * \brief Create a fighter weapon message with a controller index, the weapon index and the weapon controller
             */
			FighterWeaponMessage(int32 controllerIndex, int32 weapon, int32 weaponController);
        
            /**
             * \brief Deconstructor
             */
			~FighterWeaponMessage();

            /**
             * \brief Get the index of weapon
             */
			int32 GetWeaponIndex(void) const
			{
				return (weaponIndex);
			}

            /**
             * \brief Get the index of weapon controller
             */
			int32 GetWeaponControllerIndex(void) const
			{
				return (weaponControllerIndex);
			}

            /**
             * \brief Compress message before sending it out
             */
			void CompressMessage(Compressor& data) const override;
        
            /**
             * \brief Decompress message after receiving it
             */
			bool DecompressMessage(Decompressor& data) override;
	};


    /**
     * \brief An animator which can scale some animation
     */
	class ScaleAnimator : public Animator
	{
		private:

			float		scale; // The scale to scale an animation

            /**
             * \brief Scale an animation
             */
			ScaleAnimator();

		public:

            /**
             * \brief Scale an animation with given model and node
             */
			ScaleAnimator(Model *model, Node *node = nullptr);
        
            /**
             * \brief Deconstructor
             */
			~ScaleAnimator();

            /**
             * \brief Set the scale for scaling the animation
             * \param s The scale
             */
			void SetScale(float s)
			{
				scale = s;
			}

            /**
             * \brief Configuring an animator
             */
			void ConfigureAnimator(void) override;
        
            /**
             * \brief Move an animator
             */
			void MoveAnimator(void) override;
	};


#endif

