#pragma once
#include "Vector3.h"
#include "math.h"

Vector3 lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;

	result.x = v1.x + (t * (v2.x - v1.x));
	result.y = v1.y + (t * (v2.x - v1.y));
	result.z = v1.z + (t * (v2.x - v1.z));
	return result;
}

// Vector3 slerp(const Vector3& v1, const Vector3& v2, float t) {
//	Vector3 result;
//
//	result.x= ;
//	result.y= ;
//	result.z= ;
//
//	return result;
// }
//  正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 result;
	result.x = v.x / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.y = v.y / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.z = v.z / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
}