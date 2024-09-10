#pragma once

// <summary>
// 3次元ベクトル
// </summary>
struct Vector3 final {

	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& a) {
		Vector3 c = {};

		c.x = this->x + a.x;
		c.y = this->y + a.y;
		c.z = this->z + a.z;
		return c;
	};

	Vector3 operator-(const Vector3& a) {
		Vector3 c = {};

		c.x = this->x - a.x;
		c.y = this->y - a.y;
		c.z = this->z - a.z;
		return c;
	};

	void operator+=(const Vector3& a) {

		x += a.x;
		y += a.y;
		z += a.z;
	};

	void operator*=(const Vector3& a) {
		x *= a.x;
		y *= a.y;
		z *= a.z;
	}
};
