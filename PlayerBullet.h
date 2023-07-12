#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"
class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name= "model">モデル</param>
	/// <param name= "textureHandle">初期座標</param>
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Updarte();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name= "viewProjection">ビュープロジェクション）</param>
	void Draw(const ViewProjection& view);

	private:
	Vector3 velocity_;
		WorldTransform worldTransform_;
		Model* model_;
		uint32_t texturehandle_;
};
