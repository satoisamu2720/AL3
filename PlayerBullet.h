#pragma once
#include "Model.h"
#include "WorldTransform.h"
class PlayerBullet {
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name= "model">���f��</param>
	/// <param name= "textureHandle">�������W</param>
	void Initialize(Model* model,const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Updarte();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name= "viewProjection">�r���[�v���W�F�N�V�����j</param>
	void Draw(const ViewProjection& viewProjection);

	private:
		WorldTransform worldTransform_;
		Model* model_;
		uint32_t texturehandle_;
};
