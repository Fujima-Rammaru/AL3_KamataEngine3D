#pragma once
#include"Matrix4x4.h"
#include"Vector3.h"

class MatrixFunction {
public:
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

private:
};
