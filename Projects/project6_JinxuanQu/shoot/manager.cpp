#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"

#include "gamedata.h"
#include "manager.h"
#include <algorithm>


class SclaedSpritesCompare
{
public:
  bool operator () (const RotateSprite*lhs, const RotateSprite *rhs){
    return lhs->getScale() <  rhs->getScale();
  }
};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
 std::vector<Drawable*>::iterator ptr = sprites.begin();
  while(ptr!=sprites.end())
  {
    delete *ptr;
    ++ptr;
  }

  for (unsigned i = 0; i < rotateSprites.size(); ++i) {
    delete rotateSprites[i];
  }
}


Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world1("world1", Gamedata::getInstance().getXmlInt("world1/factor") ),
  world2("world2", Gamedata::getInstance().getXmlInt("world2/factor") ),
  world3("world3", Gamedata::getInstance().getXmlInt("world3/factor") ),
  worldWin("win",Gamedata::getInstance().getXmlInt("win/factor")),
  worldDead("dead",Gamedata::getInstance().getXmlInt("dead/factor")),
  viewport( Viewport::getInstance() ),
  sprites(),
  rotateSprites(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  orbsNum(Gamedata::getInstance().getXmlInt("orbsNum")),
  player("player"),
  showHud(false),
  aaline(),
  bar(),
  numberOfSmart(),
  god(false)
{
  if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  for (unsigned int i=0; i < orbsNum/4;++i)
    sprites.push_back( new Sprite("enemy1") );
  for (unsigned int i=0; i < orbsNum;++i){
    sprites.push_back( new Sprite("enemy2") );
    sprites.push_back( new Sprite("enemy3") );
  }
  for (unsigned int i=0; i < orbsNum;++i)
    rotateSprites.push_back(new RotateSprite("enemy"));
  sort(rotateSprites.begin(), rotateSprites.end(), SclaedSpritesCompare());
  for (unsigned int i=0; i < orbsNum;++i)
    std::cout<<rotateSprites[i]->getScale()<<std::endl;
  viewport.setObjectToTrack((Drawable*)&player);
  
}

void Manager::checkForBulletCollisions() {
  /*
  for (unsigned i = 0; i < sprites.size(); ++i) {
    ExplodingSprite* e = dynamic_cast<ExplodingSprite*>(sprites[i]);
    if ( !e && player.collidedWithBullets(sprites[i]) ) { 
      Sprite* sprite = dynamic_cast<Sprite*>(sprites[i]);
      sprites[i] = new ExplodingSprite(sprite);
      delete sprite;
    }
  }
*/
  for (unsigned i = 1; i < sprites.size(); ++i) {
    Sprite* sprite = static_cast<Sprite*>( sprites[i] );
    ExplodingSprite* e = dynamic_cast<ExplodingSprite*>(sprite);
    if ( e && e->chunkCount() == 0 ) { 
      // Make a smart sprite
      SmartSprite* ss =
        new SmartSprite(sprite->getName(),sprite->getPosition(),
                        sprite->getFrame()->getWidth()/2, 
                        sprite->getFrame()->getHeight()/2);
      sprites[i] = ss;
      delete sprite;
      player.attach(ss);
      ++numberOfSmart;
    }
    else if ( !e && player.collidedWithBullets(sprite) ) { 
      if ( dynamic_cast<SmartSprite*>(sprite) ) {
        player.
          detach(static_cast<SmartSprite*>(sprite) );
      }
      sprites[i] = new ExplodingSprite(sprite);
      delete sprite;
      if ( orbsNum ) --orbsNum;
    }
  }
}


void Manager::draw() const {
  world1.draw();
  for (unsigned i = 0; i < rotateSprites.size()/2; ++i) {
    rotateSprites[i]->draw();
  }
  world2.draw();
  world3.draw();
  //clock.draw();
  if(orbsNum==0){
    worldWin.draw();
    for (unsigned i = 0; i < sprites.size(); ++i) {
      static_cast<Sprite*>( sprites[i] )->stop();
  }
  }
  
  if(orbsNum<4){
    for (unsigned i = 0; i < orbsNum/4; ++i) {
      sprites[i]->draw();
  }
  }
  for (unsigned i = orbsNum/4; i < sprites.size(); ++i)
    sprites[i]->draw();
  

  for (unsigned i = rotateSprites.size()/2; i < rotateSprites.size(); ++i) {
    rotateSprites[i]->draw();
  }
  if(bar.getCurrenLength()!=0){
    player.draw(); 
  }
  else{
    worldDead.draw();
  }
  bar.draw();
  viewport.draw();

  if(clock.getSeconds()<4)
   aaline.drawHUD(screen);
  if (showHud)
   aaline.drawHUD(screen);
  io.printMessageAt("Press F1 to show hud", 10, 450);
  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  /*
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(get(currentSprite,sprites));
  */
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  for (unsigned int i = 0; i < rotateSprites.size(); ++i) {
    rotateSprites[i]->update(ticks);
  }
  player.update(ticks);
  checkForBulletCollisions();

  for (unsigned int i = 0; i < sprites.size(); ++i){
    if(player.collidedWith(sprites[i]) && (!god) &&(! dynamic_cast<ExplodingSprite*>(sprites[i]))){
      bar.update(ticks);
    }
  }
  

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world1.update();
  world2.update();
  world3.update();
  viewport.update(); // always update viewport last
}
void Manager::makeSprites(){
   for (unsigned int i=0; i < orbsNum;++i){
   sprites.push_back( new Sprite("sprite") );
  }
}
void Manager::play() {
  SDL_Event event;
  bool done = false;
  clock.start();
  SDLSound sound;

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F3]) {
          clock.toggleSloMo();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if(keystate[SDLK_r]) {
          for(unsigned int i=1; i<rotateSprites.size();i++)
            //static_cast<RotateSprite*>(get(i,sprites))->rotate();
            rotateSprites[i]->rotate();
        }
        if(keystate[SDLK_m]) {
          makeSprites();
        }
        /*
        if(keystate[SDLK_p]) {
          if(dynamic_cast<MultiSprite*>(sprites[0])->isAdded())
            static_cast<MultiSprite*>(sprites[0])->unaddAction();
          else
            static_cast<MultiSprite*>(sprites[0])->addAction();
        }
        */
        if (keystate[SDLK_F1]) {
          showHud = !showHud;
        }
        if(keystate[SDLK_a])
          player.left();
        if(keystate[SDLK_d])
          player.right();
        if(keystate[SDLK_s])
          player.down();
        if(keystate[SDLK_w])
          player.up();
        if(keystate[SDLK_h]){
          bar.reset();
        }
        if(keystate[SDLK_o]){
          player.shoot();
          sound[3];
        }
        if(keystate[SDLK_g])
          god = !god;

      }
    }
    draw();
    update();
  }
}
