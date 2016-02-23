#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    //currentFrame = (currentFrame+1) % (numberOfFrames/2)+framesRight*numberOfFrames/2;
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::~Player() {
  for (unsigned i = 0; i < strategies.size(); ++i) {
    delete strategies[i];
  }
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  framesRight( FrameFactory::getInstance().getFrames(name) ),
  framesLeft( FrameFactory::getInstance().getFrames(name+"Left") ),
  //framesDead (FrameFactory::getInstance().getFrames(name+"Dead") ),
  frames(framesRight),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  velocity(getVelocity()),

  timeSinceLastBullet(0),
  bulletName(name+"Bullet"),
  bullets(bulletName),
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/SpeedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/BulletInterval")),
  strategies(),
  strategy( NULL ),
  observers()
{ 
  strategies.push_back( new MidPointCollisionStrategy );
  strategy = strategies[0];
}

Player::Player(const Player& s) :
  Drawable(s),
  framesRight(s.framesRight),
  framesLeft(s.framesLeft),
  //framesDead(s.framesDead), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  velocity(s.velocity),
  timeSinceLastBullet(s.timeSinceLastBullet),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  strategies(s.strategies),
  strategy(s.strategy),
  observers(s.observers)
  { }

void Player::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
  bullets.draw();
}

void Player::update(Uint32 ticks) { 
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  timeSinceLastBullet += ticks;
  bullets.update(ticks);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }
  
  if ( X() < 0 && (velocityX()!=0)) {
      velocityX( abs( velocityX() ) );
      frames=framesRight;
  }
  if ( X() > worldWidth-frameWidth) {
      velocityX( -abs( velocityX() ) );
      frames=framesLeft;
  } 

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
}

void Player::right() {
	if( (X() < worldWidth-frameWidth)){
		velocityX(abs(velocity[0]));
    frames = framesRight;
    velocityY(0);
    
  }
	
}

void Player::left() {
	if( X() > 0)
		velocityX(-abs(velocity[0]));
	frames = framesLeft;
	velocityY(0);
}

void Player::up() {
	if (Y() > 0 ) {
    velocityY (-abs(velocity[1]+200));
  }
}


void Player::down() {
	if ( Y() < worldHeight-frameHeight ){
    velocityY (abs(velocity[1]+200));
  }
}

void Player::shoot() {
  if(timeSinceLastBullet > bulletInterval) {
    float x;
    Vector2f vel = getVelocity();
    float y = Y() + frameHeight/4;
    if ( vel[0] >=0) {
      x = X()+frameWidth;
      vel[0] += minSpeed;
    }
    else {
      x=X();
      vel[0] -= minSpeed;
    }
    bullets.shoot(Vector2f(x,y), vel);
    timeSinceLastBullet = 0; 

  }
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
/*
void Player::dead() {
  frames = framesDead;
  velocityX (0);
  velocityY (0);

}
*/
