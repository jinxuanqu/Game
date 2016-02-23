#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "drawable.h"
#include "bullets.h"
#include "collisionStrategy.h"
#include "smartSprite.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  void right();
  void left();
  void up();
  void down();
  void shoot();

  bool collidedWith(const Drawable* d) const {
    return strategy->execute(*this, *d);
  }
  bool collidedWithBullets(const Drawable* d) const {
    return bullets.collideWith(d);
  }
  void setCollisionStrategy(int index) {
    strategy = strategies[index];
  }


  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );

protected:
  const std::vector<Frame *> framesRight;
  const std::vector<Frame *> framesLeft;
  //const std::vector<Frame *> framesDead;
  std::vector<Frame *> frames;
  

  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  Vector2f velocity;

  int timeSinceLastBullet;
  std::string bulletName;
  Bullets bullets;
  float minSpeed;
  int bulletInterval;
 
  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy * strategy;

  std::list<SmartSprite*> observers;

  void advanceFrame(Uint32 ticks);
};
#endif
