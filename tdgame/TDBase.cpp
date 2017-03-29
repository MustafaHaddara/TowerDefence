#include "TDBase.h"
#include "TDGame.h"

using namespace BaseInvaders;

Tombstone::Application *CreateApplication(void)
{
	return (new Game);
}
