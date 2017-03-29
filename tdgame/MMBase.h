#ifndef MMBase_h
#define MMBase_h

#include "TSSound.h"
#include "TSZones.h"
#include "TSApplication.h"

#define MMCHEATS	1

extern "C"
{
	TOMBSTONE_MODULE_EXPORT Tombstone::Application *CreateApplication(void);
}


namespace BaseInvaders
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
        kCollectEntity      ='ccol',  // SOMETHING WE CAN COLLECT
    };
}


#endif
