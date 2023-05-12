#include "Player.h"
#include <cassert>




void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model); 
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	//�V���O���g���C���X�^���X�𖳎�����
	input_ = Input::GetInstance();

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
}

void Player::Update() { 
	worldTransform_.TransferMatrix(); 

	//�L�����ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	//�L�����̈ړ�����
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX�i���E�j
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// �����������ňړ��x�N�g����ύX�i�㉺�j
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

}

void Player::Draw(ViewProjection view) { model_->Draw(worldTransform_, view, textureHandle_); }




