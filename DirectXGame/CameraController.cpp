#define NOMINMAX
#include "CameraController.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Player.h"
#include <algorithm>

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = *target_->GetWorldTransform();

	targetVelocity = target_->GetVelocity();

	CameraShake();

	// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	goalPosition.x = (targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias) + shakePosition.x;
	goalPosition.y = (targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias) + shakePosition.y;
	goalPosition.z = (targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias);

	// 座標補間によりゆったり追従
	viewProjection_.translation_ = LerpVector3();

	// 追従対象が画面外に出ないように補正
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, targetWorldTransform.translation_.x + kTargetMovableArea.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, targetWorldTransform.translation_.x + kTargetMovableArea.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, targetWorldTransform.translation_.y + kTargetMovableArea.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, targetWorldTransform.translation_.y + kTargetMovableArea.top);

	// 移動範囲制限
	//viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.left);
	//viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.right);
	//viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.bottom);
	//viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.top);
	// 行列を更新
	viewProjection_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Text("shakeRange.x=%3.2f", shakeRange.x);
	ImGui::Text("shakeRange.y=%3.2f", shakeRange.y);
#endif
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = *target_->GetWorldTransform();
	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	viewProjection_.UpdateMatrix();
}

Matrix4x4 CameraController::GetMatView() { return viewProjection_.matView; }

Matrix4x4 CameraController::GetMatProjection() { return viewProjection_.matProjection; }

Vector3 CameraController::LerpVector3() {
	Vector3 result;

	result.x = viewProjection_.translation_.x + kInterpolationRate * (goalPosition.x - viewProjection_.translation_.x);
	result.y = viewProjection_.translation_.y + kInterpolationRate * (goalPosition.y - viewProjection_.translation_.y);
	result.z = viewProjection_.translation_.z + kInterpolationRate * (goalPosition.z - viewProjection_.translation_.z);

	return result;
}

void CameraController::CameraShake() {
	srand(currentTime);
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		shakeRange.x = 40.0f;
		shakeRange.y = 40.0f;
		shakeTimer = 90;
		offSet = shakeRange.x / 2;
		isCountingTimer = true;
		isShaking = true;
	}

	// 更新処理
	if (isShaking) {
		shakePosition.x = rand() % int(shakeRange.x) - offSet;
		shakePosition.y = rand() % int(shakeRange.y) - offSet;
	}

	if (isCountingTimer) {
		shakeTimer--;
		// 振れ幅を徐々に小さくする
		if (shakeTimer % 2 == 0) {
			shakeRange.x -= 1.0f;
			shakeRange.y -= 1.0f;
			offSet = shakeRange.x / 2;
		}
	}

	if (shakeRange.x == 0) {
		shakeRange = Vector3(0, 0, 0);
		isShaking = false;
		isCountingTimer = false;
	}
}
