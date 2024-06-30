#include "AABB.h"

bool AABB::isHit(const AABB& other) {
	if (min.x < other.max.x && other.min.x < max.x) {
		if (min.y < other.max.y && other.min.y < max.y) {
			if (min.z < other.max.z && other.min.z < max.z) {
				return true;
			}
		}
	}
	return false;
}