#pragma once
#include "Enemy.h"
#include "VectraCalculation.h"

///
///
/// 
void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Enemy.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
};

	
	

///
///
///
void Enemy::Update() {
	worldTransform_.TransferMatrix();
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.5f;
	worldTransform_.translation_.z -= kCharacterSpeed;

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	/* worldTransform_.translation_.x = enemyInputFloat[0];
	 worldTransform_.translation_.y = enemyInputFloat[1];*/
	 worldTransform_.translation_.z = enemyInputFloat[2] * kCharacterSpeed;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	if (worldTransform_.translation_.z >= 60) {
		worldTransform_.translation_.z = 0;
	}
	ImGui::SliderFloat3("enemyPositions", enemyInputFloat, -30.0f, 20.0f);
	// ImGui終わり
	ImGui::End();
	//switch (phase_) {
	//case Phase::Approach():
	//default:
	//	
	//	//移動（ベクトルを加算）
	//	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	//	//規定の位置に到達したら離脱
	//	if (worldTransform_.rotation_.z < 0.0f) {
	//		phase_ = Phase::Leave;
	//	}
	//	break;
	//case Phase::Leave:
	//	
	//	//移動（ベクトルを加算）
	//	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	//	break;
	//}
	

	
};

///
///
///
void Enemy::Draw(const ViewProjection view){ 
	model_->Draw(worldTransform_, view, textureHandle_); 
};