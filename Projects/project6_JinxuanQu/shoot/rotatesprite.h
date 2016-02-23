#ifndef ROTATESPRITE__H
#define ROTATESPRITE__H
#include <string>
#include <vector>
#include "multisprite.h"

class RotateSprite : public Drawable {
public:
  RotateSprite(const std::string&);
  RotateSprite(const RotateSprite&);
  virtual ~RotateSprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void rotate();
  
  double getScale() const { return scale; }
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

protected:
  
  const std::vector<Frame *> framesRight;
  const std::vector<Frame *> framesLeft;
  std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  float scale;
  int frameWidth;
  int frameHeight;
  

  bool rotated;
  
  float angle;
  void advanceFrame(Uint32 ticks);
};
#endif
