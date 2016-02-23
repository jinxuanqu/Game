#include <list>
#include "bullet.h"
#include "collisionStrategy.h"

class Bullets {
public:
	Bullets(const std::string&);
	Bullets(const Bullets&);
	~Bullets();
	void draw() const;
	void update(Uint32 ticks);
	void shoot(const Vector2f& pos, const Vector2f& vel);

	unsigned int bulletCount() const {
		return bulletList.size();
	}
	unsigned int freeCount() const {
		return freeList.size();
	}
	bool shooting() const{
		return bulletList.empty();
	}
	bool collideWith(const Drawable* obj) const;

private:
	std::string name;
	CollisionStrategy* strategy;
	Frame* bulletFrame;
	float frameInterval;
	float timeSinceLastFrame;
	mutable std::list<Bullet> bulletList;
	mutable std::list<Bullet> freeList;

	Bullets& operator=(const Bullets&);
};
