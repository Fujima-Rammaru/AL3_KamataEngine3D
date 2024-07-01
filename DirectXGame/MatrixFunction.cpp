#include "MatrixFunction.h"
#include <cmath>
#include <cassert>

Matrix4x4 MatrixFunction::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result1;
	Matrix4x4 resultScale;
	Matrix4x4 resultRX;
	Matrix4x4 resultRY;
	Matrix4x4 resultRZ;
	Matrix4x4 rotateXYZMatrix;

	// scale
	//===========================
	resultScale.m[0][0] = scale.x;
	resultScale.m[0][1] = 0;
	resultScale.m[0][2] = 0;
	resultScale.m[0][3] = 0;
	resultScale.m[1][0] = 0;
	resultScale.m[1][1] = scale.y;
	resultScale.m[1][2] = 0;
	resultScale.m[1][3] = 0;
	resultScale.m[2][0] = 0;
	resultScale.m[2][1] = 0;
	resultScale.m[2][2] = scale.z;
	resultScale.m[2][3] = 0;
	resultScale.m[3][0] = 0;
	resultScale.m[3][1] = 0;
	resultScale.m[3][2] = 0;
	resultScale.m[3][3] = 1;
	//===========================

	// rotateX
	//===========================
	resultRX.m[0][0] = 1;
	resultRX.m[0][1] = 0;
	resultRX.m[0][2] = 0;
	resultRX.m[0][3] = 0;
	resultRX.m[1][0] = 0;
	resultRX.m[1][1] = std::cos(rotate.x);
	resultRX.m[1][2] = std::sin(rotate.x);
	resultRX.m[1][3] = 0;
	resultRX.m[2][0] = 0;
	resultRX.m[2][1] = -std::sin(rotate.x);
	resultRX.m[2][2] = std::cos(rotate.x);
	resultRX.m[2][3] = 0;
	resultRX.m[3][0] = 0;
	resultRX.m[3][1] = 0;
	resultRX.m[3][2] = 0;
	resultRX.m[3][3] = 1;
	// rotateY
	//===========================
	resultRY.m[0][0] = std::cos(rotate.y);
	resultRY.m[0][1] = 0;
	resultRY.m[0][2] = -std::sin(rotate.y);
	resultRY.m[0][3] = 0;
	resultRY.m[1][0] = 0;
	resultRY.m[1][1] = 1;
	resultRY.m[1][2] = 0;
	resultRY.m[1][3] = 0;
	resultRY.m[2][0] = std::sin(rotate.y);
	resultRY.m[2][1] = 0;
	resultRY.m[2][2] = std::cos(rotate.y);
	resultRY.m[2][3] = 0;
	resultRY.m[3][0] = 0;
	resultRY.m[3][1] = 0;
	resultRY.m[3][2] = 0;
	resultRY.m[3][3] = 1;
	// rotateZ
	//===========================
	resultRZ.m[0][0] = std::cos(rotate.z);
	resultRZ.m[0][1] = std::sin(rotate.z);
	resultRZ.m[0][2] = 0;
	resultRZ.m[0][3] = 0;
	resultRZ.m[1][0] = -std::sin(rotate.z);
	resultRZ.m[1][1] = std::cos(rotate.z);
	resultRZ.m[1][2] = 0;
	resultRZ.m[1][3] = 0;
	resultRZ.m[2][0] = 0;
	resultRZ.m[2][1] = 0;
	resultRZ.m[2][2] = 1;
	resultRZ.m[2][3] = 0;
	resultRZ.m[3][0] = 0;
	resultRZ.m[3][1] = 0;
	resultRZ.m[3][2] = 0;
	resultRZ.m[3][3] = 1;

	//=========================
	rotateXYZMatrix = Multiply(resultRX, Multiply(resultRY, resultRZ)); // 全軸の回転行列

	//=========================
	result1.m[0][0] = resultScale.m[0][0] * rotateXYZMatrix.m[0][0];
	result1.m[0][1] = resultScale.m[0][0] * rotateXYZMatrix.m[0][1];
	result1.m[0][2] = resultScale.m[0][0] * rotateXYZMatrix.m[0][2];
	result1.m[0][3] = 0;

	result1.m[1][0] = resultScale.m[1][1] * rotateXYZMatrix.m[1][0];
	result1.m[1][1] = resultScale.m[1][1] * rotateXYZMatrix.m[1][1];
	result1.m[1][2] = resultScale.m[1][1] * rotateXYZMatrix.m[1][2];
	result1.m[1][3] = 0;

	result1.m[2][0] = resultScale.m[2][2] * rotateXYZMatrix.m[2][0];
	result1.m[2][1] = resultScale.m[2][2] * rotateXYZMatrix.m[2][1];
	result1.m[2][2] = resultScale.m[2][2] * rotateXYZMatrix.m[2][2];
	result1.m[2][3] = 0;

	result1.m[3][0] = translate.x;
	result1.m[3][1] = translate.y;
	result1.m[3][2] = translate.z;
	result1.m[3][3] = 1;
	return result1;
}

Matrix4x4 MatrixFunction::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];

	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];

	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];

	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	return result;
}

Matrix4x4 MatrixFunction::MakeRotateZMatrix(const float& rotate) { 
	
	Matrix4x4 result;

	result.m[0][0] = std::cos(rotate);
	result.m[0][1] = std::sin(rotate);
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = -std::sin(rotate);
	result.m[1][1] = std::cos(rotate);
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Vector3 MatrixFunction::Transform(const Vector3& vector, const Matrix4x4& matrix) { 

	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}
