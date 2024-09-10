#pragma once
#include "Vector3.h"
#include <Matrix4x4.h>

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// 積
Vector3 Multiply(const Vector3& v1, const Vector3& v2);
// 長さ
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 1.x軸の回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2.y軸の回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3.z軸の回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// ラープ関数
//  ラープ関数
float Lerp(float& v1, float& v2, float t);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
// スラープ関数
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
// 3次元アフィン変換
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);