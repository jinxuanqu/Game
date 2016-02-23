#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "rotatesprite.h"
#include "health.h"
#include "sprite.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  void makeSprites();
  void checkForBulletCollisions();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World world1;
  World world2;
  World world3;
  World worldWin;
  World worldDead;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<RotateSprite*> rotateSprites;
  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  unsigned int orbsNum;
  Player player;
  bool showHud;
  Hud aaline;
  Health bar;
  int numberOfSmart;
  bool god;

  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();

};
