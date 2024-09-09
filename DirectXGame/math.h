#pragma once
#include "Vector3.h"

Vector3 lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;

	result.x = v1.x + (t * (v2.x - v1.x));
	result.y = v1.y + (t * (v2.x - v1.y));
	result.z = v1.z + (t * (v2.x - v1.z));
	return result;
}

//Vector3 slerp(const Vector3& v1, const Vector3& v2, float t) {
//	Vector3 result;
//
//	result.x= ;
//	result.y= ;
//	result.z= ;
//
//	return result;
//}
