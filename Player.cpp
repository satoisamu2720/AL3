#pragma once
#include "Player.h"
#include "VectraCalculation.h"

Player::~Player() {
	for (PlayerBullet * bullet : bullets_) {

		delete bullet;
	}
}


void Player::Atack() {
	
	if (input_->PushKey(DIK_SPACE) && StopTimer == 0) {
		StopTimer = 1;
		const float kBulletSpeed = 1.0f;
		Vector3 velcity(0, 0, kBulletSpeed);
		velcity = TransformNormal(velcity, worldTransform_.matWorld_);
		PlayerBullet* newBulllet = new PlayerBullet();
		newBulllet->Initialize(model_, GetWorldPosition(), velcity);
		// 弾を登録する
		bullets_.push_back(newBulllet);
	} else 
	{
	  StopTimer = 0;
	}
	
	
};
	
	


void Player::Initialize(Model* model, uint32_t textureHandle ,Vector3 position) {

	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	input_ = Input::GetInstance();
	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
};

void Player::Update() {
	// falseになった弾を消す
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.TransferMatrix();
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;
	const float kRotSpeed = 0.2f;
	if (input_->PushKey(DIK_A)) {
	  worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
	  worldTransform_.rotation_.y += kRotSpeed;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
	  move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
	  move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {
	  move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
	  move.y -= kCharacterSpeed;
	}

	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// アフィン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列更新
	worldTransform_.UpdateMatrix();



	float imputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	// デバッグ
	ImGui::Begin("Debug");
	ImGui::Text("Toggle Camera Flag :LEFT SHIFT key");
	ImGui::SliderFloat3("player", imputFloat3, -30.0f, 30.0f);
	ImGui::End();
	worldTransform_.translation_.x = imputFloat3[0];
	worldTransform_.translation_.y = imputFloat3[1];
	worldTransform_.translation_.z = imputFloat3[2];

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
	Atack();
	
	
	if (bullet_) {
		bullet_->Updarte();
	}

	for (PlayerBullet* bullet : bullets_) {
		bullet->Updarte();
	}
	
}

void Player::Draw(ViewProjection view) { 

	model_->Draw(worldTransform_, view, textureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(view);
	}

};

void Player::OnCollision() {  }


Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return  worldPos;
}
	
