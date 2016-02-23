#include <cmath>
#include "rotatesprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "SDL/SDL_rotozoom.h"
#include "ioManager.h"

int getMiniRandomFactor() {
    int number = Gamedata::getInstance().getRandInRange(50,140);
    number *=rand()%2?-1:1;
    return number;
}
RotateSprite::~RotateSprite(){
  /*
  for (unsigned i = 0; i < frames.size(); ++i) {
    //surface = frames[i]->getSurface();
    delete frames[i];
    SDL_FreeSurface(frames[i]->getSurface());  
  }
  */
}
void RotateSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    //currentFrame = (currentFrame+1) % (numberOfFrames/2)+framesRight*numberOfFrames/2;
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

RotateSprite::RotateSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")+getMiniRandomFactor(), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")+2*getMiniRandomFactor()), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX")+getMiniRandomFactor(),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  framesRight( FrameFactory::getInstance().getFrames(name) ),
  framesLeft( FrameFactory::getInstance().getFrames(name+"Left") ),
  frames(framesRight),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  scale(Gamedata::getInstance().getRandFloat(0.5,1)),
  frameWidth(Gamedata::getInstance().getXmlInt(name+"/width") ),
  frameHeight(Gamedata::getInstance().getXmlInt(name+"/height") ),
  rotated(false),
  angle(0)
{ 
  /*
  for (unsigned int i=0; i<frames.size(); i++){
    SDL_Surface* surface = rotozoomSurface(frames[i]->getSurface(), scale, scale, SMOOTHING_ON);
    //frames[i] = new Frame (name, surface);
    frames[i] = new Frame (surface, surface->w, surface->h, 
                Gamedata::getInstance().getXmlInt(name +"/srcX"),
                 Gamedata::getInstance().getXmlInt(name +"/srcY"));
  }
   */  
}

RotateSprite::RotateSprite(const RotateSprite& s) :
  Drawable(s), 
  framesRight(s.framesRight),
  framesLeft(s.framesLeft),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  scale(s.scale),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  rotated(s.rotated),
  angle(s.angle)
  { }

void RotateSprite::rotate(){
  if(!rotated){
    rotated = true;
  ++angle;
} 
}

void RotateSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if(rotated)
    frames[currentFrame]->draw(x, y,scale,scale,angle);
  else
    frames[currentFrame]->draw(x, y,scale, scale,0);
}

void RotateSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  if(rotated) {
    if (angle>=360){
      angle =0;
      rotated = false;}
    else
    angle = angle+1;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
     frames = framesRight;

  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
     frames = framesLeft;
  }  

}