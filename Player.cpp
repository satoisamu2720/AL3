#pragma once
#include "Player.h"
#include "VectraCalculation.h"

Player::~Player() {
	for (PlayerBullet * bullet : bullets_) {

		delete bullet;
	}
	delete sprite2DReticle_;
}


void Player::Atack() {
	
	if (input_->PushKey(DIK_SPACE)) {
		if (StopTimer == 0) {
			// 弾の速度
			const float kBulletSpeed = 1.0f;

			Vector3 velocity;
			Vector3 Reticle3DPos;
			Reticle3DPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
			Reticle3DPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
			Reticle3DPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

			velocity.x = Reticle3DPos.x - worldTransform_.matWorld_.m[3][0];
			velocity.y = Reticle3DPos.y - worldTransform_.matWorld_.m[3][1];
			velocity.z = Reticle3DPos.z - worldTransform_.matWorld_.m[3][2];
			velocity = FVMultiply(kBulletSpeed, Normalize(velocity));

			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);
			// 弾を登録
			// bullet_ = newBullet;
			bullets_.push_back(newBullet);
			StopTimer++;
		}
	} else {
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
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("cursor.png");

	sprite2DReticle_ = Sprite::Create(
	    textureReticle, {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2}, {1, 1, 1, 1},
	    {0.5f, 0.5f});
};

void Player::Update(ViewProjection view) {
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
	const float KDistancePlayerTo3DReticle = 50.0f;
	Matrix4x4 Reticle3DTransMat = MakeTranslateMatrix(worldTransform3DReticle_.translation_);
	Vector3 offset = {0, 0, 1.0f};
	Matrix4x4 worMat = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	offset =TransformNormal(offset, worldTransform_.matWorld_);
	offset = FVMultiply(KDistancePlayerTo3DReticle, Normalize(offset));
	worldTransform3DReticle_.translation_.x = offset.x + worldTransform_.matWorld_.m[3][0];
	worldTransform3DReticle_.translation_.y = offset.y + worldTransform_.matWorld_.m[3][1];
	worldTransform3DReticle_.translation_.z = offset.z + worldTransform_.matWorld_.m[3][2];
	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle;
	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	Matrix4x4 matViewPort = MakeViewPortMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewPort = MMMultiply(view.matProjection, matViewPort);
	positionReticle = TransformCoord(positionReticle, matViewProjectionViewPort);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
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
	
void Player::DrawUI() {
	sprite2DReticle_->Draw(); 
}
