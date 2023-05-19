﻿#pragma once
#include "Player.h"
#include "VectraCalculation.h"


void Player::Atack() {
	if (input_->PushKey(DIK_SPACE)) {
		PlayerBullet* newBulllet = new PlayerBullet();
		newBulllet->Initialize(model_, worldTransform_.translation_);
		// 弾を登録する
		bullet_ = newBulllet;
	};
}

void Player::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
};

void Player::Update() {
	
	worldTransform_.TransferMatrix();
	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed= 1.2f; 

	const float kRotSpeed = 0.2f;
	//
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	 // 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
		inputFloat[0] = worldTransform_.translation_.x;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
		inputFloat[0] = worldTransform_.translation_.x;
	}

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
		inputFloat[1] = worldTransform_.translation_.y;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
		inputFloat[1] = worldTransform_.translation_.y;
	}


	 worldTransform_.translation_.x = inputFloat[0];
	 worldTransform_.translation_.y = inputFloat[1];
	 worldTransform_.translation_.z = inputFloat[2];

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	 
	
	// ImGuiスライダー
	ImGui::Begin("PlayerDebug");
	ImGui::Text("DebugCamera Toggle : LEFT SHIFT");
	ImGui::SliderFloat3("Positions", inputFloat, -20.0f, 20.0f);
	// ImGui終わり
	ImGui::End();

	// 移動限界座標
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 15;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	 
	Atack();
	if (bullet_) {
		bullet_->Updarte();
	}
};

void Player::Draw(ViewProjection view) { 

	model_->Draw(worldTransform_, view, textureHandle_);
	if (bullet_) {
		bullet_->Draw(view);
	}

};

	
