#include "aaline.h"
#include "ioManager.h"
#include "clock.h"

class Hud
{
public:
	Hud();
	void drawHUD(SDL_Surface*) const;
private:
	const int HUD_WIDTH;
	const int HUD_HEIGHT;
	const IOManager& io;
	Clock& clock;
	int xLoc;
	int yLoc;	
};
