#include "collider.h"

Collider::~Collider() {

	if (colliderShape_) {
		delete colliderShape_;
	}

}
