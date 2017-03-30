#ifndef TDBase_h
#define TDBase_h


#include "TSSound.h"
#include "TSZones.h"
#include "TSApplication.h"


#define TDCHEATS	1


extern "C"
{
	TOMBSTONE_MODULE_EXPORT Tombstone::Application *CreateApplication(void);
}


namespace TDGame
{
	using namespace Tombstone;
	using Tombstone::Fixed;
	using Tombstone::UnsignedFixed;
	using Tombstone::ChunkHeader;
	using Tombstone::Zone;
	using Tombstone::Marker;


	enum
	{
		kInputXInverted				= 1 << 0,
		kInputYInverted				= 1 << 1
	};


	enum
	{
		kPlayerColorCount			= 11
	};


	enum : LocatorType
	{
		kLocatorSpawn				= 'spwn',
        kLocatorMinionSpawn         = 'mspn',
		kLocatorColl				= 'colc',
        kLocatorPys                 = 'ploc'
	};


    enum
    {
        kControllerCollectable  ='1111'
    };
    
    
    
    enum
    {
        kModelApple     ='appl'
    };
    
    
    // THESE ARE THE "ENTITIES" WE CAN ADD TO THE WORLD
    enum{
        kSoldierEntity      ='csol',  // THE AVATAR
        kPhysEntiy          ='cphy',  // SOMETHING WITH PHYSICS
    };
}


#endif
