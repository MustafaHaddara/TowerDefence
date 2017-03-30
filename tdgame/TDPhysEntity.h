


#ifndef MMPhysEnt_H
#define  MMPhysEnt_H

#include "TSTriggers.h"

#include "TDCharacter.h"




namespace TDGame
{
        using namespace Tombstone;
    
    
    enum{
        kPhysEnity  = 'gphy',
    };

    class PhysEntity : public RigidBodyController
    {
        public:
            PhysEntity();
            ~PhysEntity();
    };

};

#endif
