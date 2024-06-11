#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	//bool operator==(const Vector3& other) const = default;
};
Vector3 operator+(const Vector3& a) {
	Vector3 c = {};
	
	c.x=c.x+a.x;
	c.y=c.y+a.y;
	c.x=c.z+a.z;
	return c;
};
