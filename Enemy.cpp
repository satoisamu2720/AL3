#pragma once
#include "Enemy.h"
#include "VectraCalculation.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {

		delete bullet;
	}
}
void Enemy::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Enemy.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
};

void Enemy::Fire() {
	//弾を生成し，初期化
	if (bullet_) {
		delete bullet_;
		bullet_ = nullptr;
	}
		EnemyBullet* newBullet = new EnemyBullet();
		const float kBulletSpeed = -1.0f;
		Vector3 velcity(0, 0, kBulletSpeed);
		velcity = TransformNormal(velcity, worldTransform_.matWorld_);
		newBullet->Initialize(model_, worldTransform_.translation_, velcity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	
}
	
void Enemy::Update() {
	worldTransform_.UpdateMatrix();
	const float kCharacterSpeed = 0.2f;
		//移動（ベクトルを加算）
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		worldTransform_.translation_.z -= kCharacterSpeed;
	    startTimer--;
	if (startTimer <= 5) {

		Fire();
		Approach();
		
		startTimer = kFreInterval;
	 }
	for (EnemyBullet* bullet : bullets_) {
		bullet->Updarte();
	}
};
void Enemy::Approach() { 
	startTimer = 0; 
}

void Enemy::Draw(const ViewProjection viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
 for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
};