#include "WorldTransform.h"
#include "MatrixFunction.h"

void WorldTransform::UpdateMatrix() { 
	
	matWorld_ = MatrixFunction::MakeAffineMatrix(scale_,rotation_,translation_);

	TransferMatrix();
}
