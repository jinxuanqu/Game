#include <iostream>
#include <cmath>
#include "bullet.h"
#include "collisionStrategy.h"

void Bullet::update(Uint32 ticks) {
	Vector2f pos = getPosition();
	Sprite::update(ticks);
	distance += ( hypot(X()-pos[0], Y()-pos[1]));
	if (distance > maxDistance)
		tooFar = true;
}
