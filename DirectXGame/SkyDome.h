#pragma once
#include "WorldTransform.h"
#include "Model.h"
class SkyDome {
public:
	void Initialize();

	void Update();

	void Draw();


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

};

