#include "TDBase.h"
#include "TDGame.h"


using namespace TDGame;


Tombstone::Application *CreateApplication(void)
{
	return (new Game);
}
