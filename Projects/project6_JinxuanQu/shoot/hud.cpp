#include "hud.h"

Hud::Hud():
HUD_WIDTH(Gamedata::getInstance().getXmlInt("hud/width")),
HUD_HEIGHT(Gamedata::getInstance().getXmlInt("hud/height")),
io( IOManager::getInstance() ),
clock( Clock::getInstance() ),
xLoc(Gamedata::getInstance().getXmlInt("hud/xLoc")),
yLoc(Gamedata::getInstance().getXmlInt("hud/yLoc"))
{ }

void Hud::drawHUD(SDL_Surface* screen) const {
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, xLoc, yLoc+HUD_HEIGHT/2, 
                      xLoc+HUD_WIDTH,yLoc+HUD_HEIGHT/2, 
                      HUD_HEIGHT, 0xff, 0xff, 0xff, 0xff/2);
  Draw_AALine(screen, xLoc,yLoc, xLoc+HUD_WIDTH,yLoc, RED);
  clock.draw();
  io.printStringAfterMessage("asdw moves player", xLoc, yLoc+60);
  io.printStringAfterMessage("O shoots enemies", xLoc, yLoc+80);
  io.printStringAfterMessage("R rotates bees", xLoc, yLoc+100);
  io.printStringAfterMessage("H resets health bar", xLoc, yLoc+120);
  
}