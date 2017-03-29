#include "MMBase.h"
#include "MMGame.h"

using namespace BaseInvaders;

Tombstone::Application *CreateApplication(void)
{
	return (new Game);
}
