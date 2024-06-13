#pragma once

// <summary>
// 3次元ベクトル
// </summary>
struct Vector3 final {
	
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3 & a) {
		Vector3 c = {};
		

		c.x = this->x + a.x;
		c.y = this->y + a.y;
		c.z = this->z + a.z;
		return c;
	};

	//bool operator==(const Vector3& other) const = default;
};
	

//	Vector3 

	

	




