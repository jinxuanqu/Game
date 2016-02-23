#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    //currentFrame = (currentFrame+1) % (numberOfFrames/2)+framesRight*numberOfFrames/2;
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  framesRight( FrameFactory::getInstance().getFrames(name) ),
  framesLeft( FrameFactory::getInstance().getFrames(name+"Left") ),
  frames(framesRight),
  actionRight( FrameFactory::getInstance().getFrames("actionRight") ),
  actionLeft( FrameFactory::getInstance().getFrames("actionLeft") ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  added(false)
  //framesRight(0)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s),
  framesRight(s.framesRight),
  framesLeft(s.framesLeft), 
  frames(s.frames),
  actionRight(s.actionRight),
  actionLeft(s.actionLeft),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  added(s.added)
  //framesRight( s.framesRight)
  { }

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

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
      frames=framesRight;
  }
  if ( X() > worldWidth-frameWidth) {
      velocityX( -abs( velocityX() ) );
      frames=framesLeft;
  } 
 
  
 }
  void MultiSprite::addAction(){
      added = true;
      if ((velocityX() <0)&&X()>0 ){
    frames=actionLeft;
  }
  if (( velocityX()>0)&& (X()<worldWidth-frameWidth)) {
    frames=actionRight;
  } 
   
  } 

  void MultiSprite::unaddAction(){
    added = false;
    if ( velocityX()<0) {
      frames=framesLeft;
  }
    if ( velocityX()>0) {
     frames=framesRight;
  } 
  
  }


