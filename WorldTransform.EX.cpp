#include "WorldTransform.h"
#include "VectraCalculation.h"
void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}