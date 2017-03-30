#ifndef TDCharacter_h
#define TDCharacter_h


#include "TSModels.h"
#include "TSCharacter.h"

#include "TDBase.h"


namespace TDGame
{
	using namespace Tombstone;


	typedef Type	CharacterType;


	enum : RigidBodyType
	{
		kRigidBodyCharacter		= 'char'
	};


	enum : CharacterType
	{
		kCharacterPlayer		= 'play',
		kCharacterMonster		= 'mnst',
		kCharacterAnimal		= 'anml'
	};


	enum
	{
		kCharacterDead			= 1 << 0,
		kCharacterGround		= 1 << 1,
		kCharacterOffGround		= 1 << 2,
		kCharacterJumping		= 1 << 3,
		kCharacterFlying		= 1 << 4,
		kCharacterSwimming		= 1 << 5,
		kCharacterAttackable	= 1 << 6
	};


	enum
	{
		kDamageBloodInhibit		= 1 << 0
	};


	enum CharacterStatus
	{
		kCharacterUnaffected,
		kCharacterDamaged,
		kCharacterKilled
	};


	struct SubstanceData;

    /**
     * \brief Controlers a game character.
     */
	class GameCharacterController : public CharacterController, public LinkTarget<GameCharacterController> {
    private:

        CharacterType		characterType;
        unsigned_int32		characterState;

        float				standingTime;
        float				offGroundTime;

    protected:

        /**
         * \brief Constructor
         */
        GameCharacterController(CharacterType charType, ControllerType contType);
        
        /**
         * \brief Constructor
         */
        GameCharacterController(const GameCharacterController& gameCharacterController);

        /**
         * \brief Set the game character as able to be attacked.
         */
        void SetAttackable(bool attackable);

    public:

        /**
         * \brief Constructor
         */
        virtual ~GameCharacterController();

        /**
         * \brief Returns character type.
         */
        CharacterType GetCharacterType(void) const
        {
            return (characterType);
        }

        /**
         * \brief Returns character state in a 32-bit bitmask.
         */
        unsigned_int32 GetCharacterState(void) const
        {
            return (characterState);
        }

        /**
         * \brief Set the character state.
         * \param state The new state of the character.
         */
        void SetCharacterState(unsigned_int32 state)
        {
            characterState = state;
        }

        /**
         * \brief Reset the amount of time the character has been standing
         */
        void ResetStandingTime(void) 
        { 
            standingTime = 0.0F;
            SetVelocityMultiplier(1.0F); 
        } 

        /**
         * \brief Get the amount of the time the character has been off the ground (ie. in the air).
         */
        float GetOffGroundTime(void) const
        { 
            return (offGroundTime); 
        } 

        /**
         * \brief Returns the model this character is controlling.
         */
        Model *GetTargetNode(void) const
        { 
            return (static_cast<Model *>(Controller::GetTargetNode()));
        }

        /**
         * \brief Compress controller for transmission.
         */
        void Pack(Packer& data, unsigned_int32 packFlags) const override;
        
        /**
         * \brief Decompress controller when recieving.
         */
        void Unpack(Unpacker& data, unsigned_int32 unpackFlags) override;
        
        /**
         * \brief Decompress part of controller when recieving.
         */
        void UnpackChunk(const ChunkHeader *chunkHeader, Unpacker& data, unsigned_int32 unpackFlags);

        /**
         * \brief Move the controller
         * 
         * Called every frame by the Tombstone Engine
         */
        void MoveController(void) override;

        /**
         * \brief Called when the character enters the world.
         */
        virtual void EnterWorld(World *world, const Point3D& worldPosition);

        /**
         * \brief Damage the character.
         * \param damage The amount of the damage to deal to the chraacter.
         * \param attacker The character dealing damage.
         */
        virtual CharacterStatus Damage(Fixed damage, unsigned_int32 flags, GameCharacterController *attacker, const Point3D *position = nullptr, const Vector3D *impulse = nullptr);
        
        /**
         * \brief Kill the character.
         * \param attacked The other character that killed this one.
         */
        virtual void Kill(GameCharacterController *attacker, const Point3D *position = nullptr, const Vector3D *impulse = nullptr);
	};

    /**
     * \brief Message encapsulating character state.
     */
	class CharacterStateMessage : public ControllerMessage {
    friend class CharacterController;

    private:

        Point3D		initialPosition;
        Vector3D	initialVelocity;

    public:

        /**
         * \brief Base constructor used by Tombstone Engine.
         */
        CharacterStateMessage(ControllerMessageType type, int32 controllerIndex);
        
        /**
         * \brief Contructor used by Server to signal a new rotation to the clients.
         * \param position Current position of the character.
         * \param velocity Current velocity of the character.
         */
        CharacterStateMessage(ControllerMessageType type, int32 controllerIndex, const Point3D& position, const Vector3D& velocity);
        
        /**
         * \brief Destructor
         */
        ~CharacterStateMessage();

        /**
         * \brief Returns the position of the character.
         */
        const Point3D& GetInitialPosition(void) const
        {
            return (initialPosition);
        }

        /**
         * \brief Returns the velocity of the character.
         */
        const Vector3D& GetInitialVelocity(void) const
        {
            return (initialVelocity);
        }

        /**
         * \brief Serializes message
         */
        void CompressMessage(Compressor& data) const override;
        
        /**
         * \brief Deserializes message
         */
        bool DecompressMessage(Decompressor& data) override;
	};


    /**
     * \brief Handles animation.
     */
	class AnimationBlender : public Packable {
    private:

        int32				blendParity;
        BlendAnimator		blendAnimator;
        FrameAnimator		frameAnimator[2];

    public:

        /**
         * \brief Contructor.
         */
        AnimationBlender();
        
        /**
         * \brief Destructor.
         */
        ~AnimationBlender();

        /**
         * \brief Returns BlendAnimator.
         */
        BlendAnimator *GetBlendAnimator(void)
        {
            return (&blendAnimator);
        }

        /**
         * \brief Returns FrameAnimator.
         */
        FrameAnimator *GetFrameAnimator(int32 index)
        {
            return (&frameAnimator[index]);
        }

        /**
         * \brief Returns most recently used FrameAnimator.
         */
        FrameAnimator *GetRecentAnimator(void)
        {
            return (&frameAnimator[blendParity]);
        }

        /**
         * \brief Sets the ObserverType of the used FrameAnimator.
         */
        void SetFrameAnimatorObserver(FrameAnimator::ObserverType *observer)
        {
            frameAnimator[0].SetObserver(observer);
            frameAnimator[1].SetObserver(observer);
        }

        /**
         * \brief Compress controller for transmission.
         */
        void Pack(Packer& data, unsigned_int32 packFlags) const override;
        
        /**
         * \brief Decompress controller when recieving.
         */
        void Unpack(Unpacker& data, unsigned_int32 unpackFlags) override;
        
        /**
         * \brief Decompress part of controller when recieving.
         */
        void UnpackChunk(const ChunkHeader *chunkHeader, Unpacker& data, unsigned_int32 unpackFlags);

        /**
         * \brief Preprocess model for animations
         * \param model The model to be animated
         */
        void PreprocessAnimationBlender(Model *model);

        /**
         * \brief Start the animation.
         */
        FrameAnimator *StartAnimation(const char *name, unsigned_int32 mode, Interpolator::CompletionCallback *callback = nullptr, void *cookie = nullptr);
        
        /**
         * \brief Blend from one animation to another.
         */
        FrameAnimator *BlendAnimation(const char *name, unsigned_int32 mode, float blendRate, Interpolator::CompletionCallback *callback = nullptr, void *cookie = nullptr);
	};
}


#endif

