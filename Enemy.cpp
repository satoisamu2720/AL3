#pragma once
#include "Enemy.h"
#include "Player.h"
#include "VectraCalculation.h"

Enemy::~Enemy() {
	delete state;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	for (TimedCall* timedCall : timedCall_) {
		delete timedCall;
	}
	
}
void Enemy::Initialize(Model* model, const Vector3& position, GameScene* gameScene) {
	assert(model);
	model_ = model;
	texturehandle_ = TextureManager::Load("Enemy.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	state = new EnemyStateApproah();
	state->SetEnemy(this);
	SetGameScene(gameScene);
	
};

void Enemy::Fire() {
	
	
	Attack();

	timedCall_.push_back(new TimedCall(std::bind(&Enemy::Fire, this), 60));
	
}
void Enemy::Attack() {
	assert(player_);
	timer--;

	if (timer < 0) {
		const float kBulletSpeed = 0.6f;
		Vector3 playerPos = player_->GetWorldPosition();
		Vector3 enemyPos = GetWorldPosition();
		Vector3 velcity = Subtract(playerPos, enemyPos);
		velcity = Normalize(velcity);
		velcity.x *= kBulletSpeed;
		velcity.y *= kBulletSpeed;
		velcity.z *= kBulletSpeed;
		velcity = TransformNormal(velcity, worldTransform_.matWorld_);

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velcity);

		// 弾を登録する
		bullets_.push_back(newBullet);

		timer = kShotInterval;
	}
}
void Enemy::ChangeState(EnemyState* newEnemyState) {
	delete state;

	state = newEnemyState;
	state->SetEnemy(this);
}

void Enemy::SetPosition(Vector3 velcity) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velcity);
}

void EnemyStateApproah::Update() {
	Vector3 appSpeed(0, 0, -0.2f);
	enemy_->SetPosition(appSpeed);

	enemy_->Fire();

	if (enemy_->GetWT().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave);
	}
}

void EnemyStateLeave::Update() {
	Vector3 leaveSpeed(-0.2f, 0.2f, 0.2f);
	enemy_->SetPosition(leaveSpeed);

	enemy_->Fire();
}

void Enemy::Update() {

	// デスフラグの立った球を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	state->Update();

	// Attack();

	timedCall_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return false;
		}
		return true;
	});

	worldTransform_.UpdateMatrix();

	for (TimedCall* timedCalls : timedCall_) {
		timedCalls->Update();
	}
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
};



void Enemy::OnCollision() {}


Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}