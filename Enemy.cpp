#pragma once
#include "Enemy.h"
#include "VectraCalculation.h"

///
///
/// 
void Enemy::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Enemy.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
};

	
	

///
///
///
void Enemy::Update() {

	//const float kCharacterSpeed = 0.2f;

	(this->*spFuncTable[static_cast<size_t>(phase_)])();
	worldTransform_.UpdateMatrix();
	
	

	//// 移動（ベクトルを加算）
	//worldTransform_.translation_ = Add(worldTransform_.translation_, {-0.5f, 0.5f, 0.0f});
}
	
void Enemy::Approach() {
	// 移動（ベクトルを加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, {0.0f, 0.0f, -0.5f});

	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
	
void Enemy::Leave() {
	// 移動（ベクトルを加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, {-0.5f, 0.5f, 0.0f});
}

void Enemy::Draw(const ViewProjection view){ 
	model_->Draw(worldTransform_, view, textureHandle_); 
};

void (Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave
};

