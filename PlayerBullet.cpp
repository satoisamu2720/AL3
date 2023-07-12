#pragma once
#include "PlayerBullet.h"
#include "VectraCalculation.h"



void PlayerBullet::Initialize(Model* model, const Vector3& position ,const Vector3& velocity) { 
	
	assert(model);
	
	//テクスチャ読み込み
	texturehandle_ = TextureManager::Load("sample.png");
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
	
};


void PlayerBullet::Updarte(){ worldTransform_.UpdateMatrix();
	    
worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
worldTransform_.translation_.z += velocity_.z;
};


void PlayerBullet::Draw(const ViewProjection& view) {
	model_->Draw(worldTransform_, view, texturehandle_);
}