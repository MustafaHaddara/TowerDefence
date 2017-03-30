#ifndef TDMultiplayer_h
#define TDMultiplayer_h


#include "TDCharacter.h"

namespace TDGame
{
	using namespace Tombstone;


	typedef unsigned_int32	ModelMessageType;
    
    
    /**
     * \brief Encapsulates player state within a 32-bit-wide bitmask.
     */
    struct playerState
    {
        int32 state;
    };


	enum
	{
		kGameProtocol	= 0x00000019,
		kGamePort		= 28327
	};

	// Message types
	enum
	{
		kMessageServerInfo = kMessageBaseCount,
		kMessageGameInfo,
		kMessagePlayerStyle,
		kMessageCreateModel,
		kMessageScore,
		kMessageHealth,
		kMessagePrimaryAmmo,
		kMessageSecondaryAmmo,
		kMessageWeapon,
		kMessagePower,
		kMessageTreasure,
		kMessageAddInventory,
		kMessageRemoveInventory,
		kMessageDeath,
		kMessageRefreshScoreboard,
		kMessageClientStyle,
		kMessageClientOrientation,
		kMessageClientMovementBegin,
		kMessageClientMovementEnd,
		kMessageClientMovementChange,
		kMessageClientFiringPrimaryBegin,
		kMessageClientFiringSecondaryBegin,
		kMessageClientFiringEnd,
		kMessageClientInteractionBegin,
		kMessageClientInteractionEnd,
		kMessageClientSwitchWeapon,
		kMessageClientDeferWeapon,
		kMessageClientSpecialWeapon,
		kMessageClientCycleWeapon,
		kMessageClientSpawn,
		kMessageClientScoreOpen,
		kMessageClientScoreClose,
		kMessageClientChatOpen,
		kMessageClientChatClose,
		kMessageClientVoiceReceiveStart,
		kMessageClientVoiceReceiveStop,
        kMessageCreateCharacter, // ADDED THIS
        kMessageRequestAvantar,
		kMessageTowerOne,	// Message to select tower to place
		kMessageTowerTwo,
		kMessageTowerThree
	};


	enum
	{
		kModelMessageArrow,
		kModelMessageExplosiveArrow,
		kModelMessageSpike,
		kModelMessageRailSpike,
		kModelMessageGrenade,
		kModelMessageCharge,
		kModelMessageRocket,
		kModelMessagePlasma,
		kModelMessageFireball,
		kModelMessageTarball,
		kModelMessageLavaball,
		kModelMessageVenom,
		kModelMessageBlackCat,
		kModelMessageGusGraves,
		kModelMessageSoldier
	};


	enum
	{
		kPlayerStyleEmissionColor,
		kPlayerStyleArmorColor,
		kPlayerStyleArmorTexture,
		kPlayerStyleHelmetType,
		kPlayerStyleCount
	};


	enum
	{
		kPlayerInactive				= 1 << 0,
		kPlayerReceiveVoiceChat		= 1 << 1,
		kPlayerScoreUpdate			= 1 << 2,
		kPlayerScoreboardOpen		= 1 << 3
	};


	enum 
	{ 
		kMultiplayerDedicated		= 1 << 0
	}; 
 
 
	class GameWorld;
	class FighterController; 
 
	/**
	* \brief Message containing server information.
	*/
	class ServerInfoMessage : public Message
	{
		friend class Game; 

		private:

			int32						playerCount;
			int32						maxPlayerCount;
			String<kMaxGameNameLength>	gameName;
			ResourceName				worldName;

			ServerInfoMessage();

		public:
			/**
			* \brief Constructor
			* \param numPlayers Number of players in the server
			* \param maxPlayers Maximum number of players the server can hold
			* \param game Name of the game
			* \param world Name of the world
			*/
			ServerInfoMessage(int32 numPlayers, int32 maxPlayers, const char *game, const char *world);
			/**
			* \brief Destructor
			*/
			~ServerInfoMessage();

			/**
			* \brief Returns the number of players
			*/
			int32 GetPlayerCount(void) const
			{
				return (playerCount);
			}

			/**
			* \brief Returns the maximum player capacity
			*/
			int32 GetMaxPlayerCount(void) const
			{
				return (maxPlayerCount);
			}

			/**
			* \brief Returns the name of the game
			*/
			const char *GetGameName(void) const
			{
				return (gameName);
			}
			
			/**
			* \brief Returns the name of the world
			*/
			const char *GetWorldName(void) const
			{
				return (worldName);
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
	* \brief Message containing game information.
	*/
	class GameInfoMessage : public Message
	{
		friend class Game;

		private:

			unsigned_int32		multiplayerFlags;
			ResourceName		worldName;

			GameInfoMessage();

		public:

			/**
			* \brief Constructor
			* \param flags Multiplayer game flags
			* \param world Name of the world
			*/
			GameInfoMessage(unsigned_int32 flags, const char *world);
			/**
			* \brief Destructor
			*/
			~GameInfoMessage();

			/**
			* \brief Returns the multiplayer flags
			*/
			unsigned_int32 GetMultiplayerFlags(void) const
			{
				return (multiplayerFlags);
			}

			/**
			* \brief Returns the name of the world
			*/
			const char *GetWorldName(void) const
			{
				return (worldName);
			}

			/**
			* \brief Serializes message
			*/
			void CompressMessage(Compressor& data) const override;

			/**
			* \brief Deserializes message
			*/
			bool DecompressMessage(Decompressor& data) override;

			/**
			* \brief Handles messages from the server.
			*/
			bool HandleMessage(Player *sender) const override;
	};

	/**
	* \brief Message containing information about a graphical character model
	*/
	class CreateModelMessage : public Message
	{
		private:

			ModelMessageType		modelMessageType;

			int32					controllerIndex;
			Point3D					initialPosition;

		protected:

			CreateModelMessage(ModelMessageType type);

			void InitializeModel(GameWorld *world, Model *model, Controller *controller) const;

		public:

			/**
			* \brief Constructor
			* \param type Type of model message being created
			* \param index Index of the model controller
			* \param position Initial location of the model
			*/
			CreateModelMessage(ModelMessageType type, int32 index, const Point3D& position);
			~CreateModelMessage();

			/**
			* \brief Returns the type of message
			*/
			ModelMessageType GetModelMessageType(void) const
			{
				return (modelMessageType);
			}

			/**
			* \brief Returns the model controller index
			*/
			int32 GetControllerIndex(void) const
			{
				return (controllerIndex);
			}

			/**
			* \brief Returns the initial position of the model
			*/
			const Point3D& GetInitialPosition(void) const
			{
				return (initialPosition);
			}

			/**
			* \brief Serializes message
			*/
			void CompressMessage(Compressor& data) const override;

			/**
			* \brief Deserializes message
			*/
			bool DecompressMessage(Decompressor& data) override;

			static CreateModelMessage *CreateMessage(ModelMessageType type);
	};

	/**
	* \brief Message containing information about a character death.
	*/
	class DeathMessage : public Message
	{
		friend class Game;

		private:

			PlayerKey		playerKey;
			PlayerKey		attackerKey;

			DeathMessage();

		public:

			/**
			* \brief Constructor
			* \param player The key of the dead player
			* \param attacker The key of the attacker
			*/
			DeathMessage(PlayerKey player, PlayerKey attacker);
			~DeathMessage();

			/**
			* \brief Returns the key of the dead player
			*/
			PlayerKey GetPlayerKey(void) const
			{
				return (playerKey);
			}

			/**
			* \brief Returns the key of the attacker
			*/
			PlayerKey GetAttackerKey(void) const
			{
				return (attackerKey);
			}

			/**
			* \brief Serializes message
			*/
			void CompressMessage(Compressor& data) const override;

			/**
			* \brief Deserializes message
			*/
			bool DecompressMessage(Decompressor& data) override;

			bool HandleMessage(Player *sender) const override;
	};

	/**
	* \brief Message containing information about client rotation.
	*/
	class ClientOrientationMessage : public Message
	{
		friend class Game;

		private:

			float		orientationAzimuth;
			float		orientationAltitude;

			ClientOrientationMessage();

		public:

			/**
			* \brief Constructor
			* \param azimuth Rotation azimuth
			* \param altitude Rotation altitude
			*/
			ClientOrientationMessage(float azimuth, float altitude);
			/**
			* \brief Destructor
			*/
			~ClientOrientationMessage();

			/**
			* \brief Returns the rotation azimuth
			*/
			float GetOrientationAzimuth(void) const
			{
				return (orientationAzimuth);
			}

			/**
			* \brief Returns the rotation altitude
			*/
			float GetOrientationAltitude(void) const
			{
				return (orientationAltitude);
			}

			/**
			* \brief Serializes message
			*/
			void CompressMessage(Compressor& data) const override;

			/**
			* \brief Deserializes message
			*/
			bool DecompressMessage(Decompressor& data) override;

			/**
			* \brief Handles client controller orientation update messages.
			*/
			bool HandleMessage(Player *sender) const override;
	};

	/**
	* \brief Message containing information about client movement.
	*/
	class ClientMovementMessage : public Message
	{
		friend class Game;

		private:

			unsigned_int32	movementFlag;
			float			movementAzimuth;
			float			movementAltitude;

			ClientMovementMessage(MessageType type);

		public:

			/**
			* \brief Constructor
			* \param type Message type
			* \param flag Movement direction
			* \param azimuth Rotation azimuth
			* \param altitude Rotation altitude
			*/
			ClientMovementMessage(MessageType type, unsigned_int32 flag, float azimuth, float altitude);
			/**
			* \brief Destructor
			*/
			~ClientMovementMessage();

			/**
			* \brief Returns the movement direction
			*/
			int32 GetMovementFlag(void) const
			{
				return (movementFlag);
			}

			/**
			* \brief Returns the look rotation azimuth
			*/
			float GetMovementAzimuth(void) const
			{
				return (movementAzimuth);
			}

			/**
			* \brief Returns the look rotation altitude
			*/
			float GetMovementAltitude(void) const
			{
				return (movementAltitude);
			}

			/**
			* \brief Serializes message
			*/
			void CompressMessage(Compressor& data) const override;

			/**
			* \brief Deserializes message
			*/
			bool DecompressMessage(Decompressor& data) override;

			/**
			* \brief Handles client controller movement messages.
			*/
			bool HandleMessage(Player *sender) const override;
	};

	/**
	* \brief Message containing information about client weapon firing.
	*/
	class ClientFiringMessage : public Message
	{
		friend class Game;

		private:

			float		firingAzimuth;
			float		firingAltitude;

			ClientFiringMessage(MessageType type);

		public:

			/**
			* \brief Constructor
			* \param type Message type
			* \param azimuth Firing azimuth
			* \param altitude Firing altitude
			*/
			ClientFiringMessage(MessageType type, float azimuth, float altitude);
			/**
			* \brief Destructor
			*/
			~ClientFiringMessage();

			/**
			* \brief Returns the firing azimuth
			*/
			float GetFiringAzimuth(void) const
			{
				return (firingAzimuth);
			}

			/**
			* \brief Returns the firing altitude
			*/
			float GetFiringAltitude(void) const
			{
				return (firingAltitude);
			}

			/**
			* \brief Serializes message
			*/
			void CompressMessage(Compressor& data) const override;
			/**
			* \brief Deserializes message
			*/
			bool DecompressMessage(Decompressor& data) override;

			bool HandleMessage(Player *sender) const override;
	};

	/**
	* \brief Message containing miscellaneous information
	*/
	class ClientMiscMessage : public Message
	{
		public:

			/**
			* \brief Constructor
			* \param type Message type
			*/
			ClientMiscMessage(MessageType type);
			/**
			* \brief Destructor
			*/
			~ClientMiscMessage();

			bool HandleMessage(Player *sender) const override;
	};



/*---------------------------------------------------------------------------------------------------*/

// I added these

//typedef int EnemyType;

//! Message to request the generation of a character (from server)

typedef int EnemyType;

	/**
	* \brief Message from the client to request an object from the server
	*/
    class ClientRequestMessage : public Message
    {
        friend class Game;
    private:
        
        int myData;
        
        ClientRequestMessage(MessageType type);
        
    public:
		/**
		* \brief Constructor
		* \param type Message type
		* \param data Client request
		*/
        ClientRequestMessage(MessageType type, const long data);

		/**
		* \brief Destructor
		*/
        ~ClientRequestMessage();
        
		/**
		* \brief Returns the client request
		*/
        const long getData(void) const
        {
            return (myData);
        }
        
		/**
		* \brief Serializes message
		*/
        void CompressMessage(Compressor& data) const override;
		/**
		* \brief Deserializes message
		*/
        bool DecompressMessage(Decompressor& data) override;

		/**
		* \brief Replies to the client request
		*/
        bool HandleMessage(Player *sender) const override;    };
    
/**
* \brief Message with information about character creation.
*/
class CreateCharacterMessage: public Message
{
    friend class Game;
    
private:
    
    Point3D         pos;
    unsigned_int16  index;
    EnemyType       chartype;
    PlayerKey       ownerKey;
    
    CreateCharacterMessage(MessageType type);
    
public:

	/**
	* \brief Constructor
	* \param type Message type
	* \param index Character controller index
	* \param chartype The type of character to be placed
	* \param key Key of the player to which this character belongs
	* \param position World location for character to be placed
	*/
    CreateCharacterMessage(MessageType type,long index, EnemyType chartype,PlayerKey key, Point3D position);
	/**
	* \brief Destructor
	*/
    ~CreateCharacterMessage();

	/**
	* \brief Serializes message
	*/
    void CompressMessage(Compressor& data) const override;
	/**
	* \brief Deserializes message
	*/
    bool DecompressMessage(Decompressor& data) override;

	/*
	* \brief Places the character at the predetermined location
	*/
    bool HandleMessage(Player *sender) const override;

    
	/**
	* \brief Returns the controller index
	*/
    long GetControllerIndex(void){
        return(index);
    }
    
	/**
	* \brief Returns the character position
	*/
    Point3D GetPosition(void){
        return(pos);
    }
    
	/**
	* \brief Returns the character type
	*/
    EnemyType GetCharType(void){
        return(chartype);
    }
};


/**
* \brief Message with information about a tower being selected for placement
*/
class ClientTowerSelectMessage : public Message
{
	friend class Game;

private:

	unsigned towerNumber;	// Which tower was selected? 1, 2 or 3?

public:

	/**
	* \brief Constructor
	* \param type Selected tower type
	*/
	ClientTowerSelectMessage(MessageType type);

	/**
	* \brief Destructor
	*/
	~ClientTowerSelectMessage();

	/**
	* \brief Serializes message
	*/
	void CompressMessage(Compressor& data) const override;

	/**
	* \brief Deserializes message
	*/
	bool DecompressMessage(Decompressor& data) override;

	/**
	* \brief Updates the UI to show which tower was selected
	*/
	bool HandleMessage(Player *sender) const override;
};

}

#endif
