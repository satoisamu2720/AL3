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
	if (!isControl) {
		XINPUT_STATE joyState;
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			isAttack = true;
		} else {
			// isAttack = false;
		}

	} else if (isControl) {
		if (input_->PushKey(DIK_SPACE)|| input_->IsPressMouse(0)) {
			isAttack = true;
		} else {
			// isAttack = false;
		}
	}

	if (isAttack) {

		if (input_->PushKey(DIK_SPACE) || input_->IsPressMouse(0)) {
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
	
	XINPUT_STATE joyState;
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


	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// アフィン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列更新
	worldTransform_.UpdateMatrix();

	float inputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		inputFloat3[0] = worldTransform_.translation_.x;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		inputFloat3[1] = worldTransform_.translation_.y;
	}
	// デバッグ
	
	
	
	
	ImGui::Begin("Debug");
	ImGui::Text("Toggle Camera Flag :LEFT SHIFT");
	ImGui::Text("Change Mouse : F");
	ImGui::Text("Change Controller : C");
	ImGui::SliderFloat3("player", inputFloat3, -30.0f, 30.0f);
	ImGui::End();
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];

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
	Matrix4x4 worMat = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = FVMultiply(KDistancePlayerTo3DReticle, Normalize(offset));
	worldTransform3DReticle_.translation_.x = offset.x + worldTransform_.matWorld_.m[3][0];
	worldTransform3DReticle_.translation_.y = offset.y + worldTransform_.matWorld_.m[3][1];
	worldTransform3DReticle_.translation_.z = offset.z + worldTransform_.matWorld_.m[3][2];
	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle;
	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	Matrix4x4 matViewPort =
	    MakeViewPortMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewPort = MMMultiply(view.matProjection, matViewPort);
	positionReticle = TransformCoord(positionReticle, matViewProjectionViewPort);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	MouseUpdate(view);
	
	// ImGui終わり
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
void Player::MouseUpdate(ViewProjection& view) {

	if (input_->TriggerKey(DIK_F)) {
		isControl = true;
	} else if (input_->TriggerKey(DIK_C)) {
		isControl = false;
	}

	if (isControl) {
		POINT mousePosition;

		GetCursorPos(&mousePosition);

		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		sprite2DReticle_->SetPosition({float(mousePosition.x), float(mousePosition.y)});

	} else if (!isControl) {
		XINPUT_STATE joyState;
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 8.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 8.0f;

			sprite2DReticle_->SetPosition(spritePosition);
		}
	}

	Matrix4x4 viewPortMat =MakeViewPortMatrix(
	    0, 0, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);
	Matrix4x4 matVPV =
	    MMMultiply(view.matView, MMMultiply(view.matProjection, viewPortMat));
	Matrix4x4 matInverseVPV =Inverse(matVPV);

	Vector3 posNear =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	posNear = TransformCoord(posNear, matInverseVPV);
	posFar = TransformCoord(posFar, matInverseVPV);

	Vector3 mouseDirection;
	mouseDirection.x = posFar.x - posNear.x;
	mouseDirection.y = posFar.y - posNear.y;
	mouseDirection.z = posFar.z - posNear.z;
	mouseDirection = Normalize(mouseDirection);

	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_.x =
	    FVMultiply(kDistanceTestObject, mouseDirection).x + posNear.x;
	worldTransform3DReticle_.translation_.y =
	    FVMultiply(kDistanceTestObject, mouseDirection).y + posNear.y;
	worldTransform3DReticle_.translation_.z =
	    FVMultiply(kDistanceTestObject, mouseDirection).z + posNear.z;

	worldTransform3DReticle_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text(
	    "2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
}