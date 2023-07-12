#pragma once
#include "Model.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>
#include "assert.h"



class Enemy {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name= "model">モデル</param>
	/// <param name= "textureHandle">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name= "viewProjection">ビュープロジェクション）</param>
	void Draw(ViewProjection view);

	///
	///
	///
	void Fire();

	/// <summary>
	/// 行動フェーズ
	/// </summary>
	enum class Phase {
		Approach, // 接近する
		Attack,    //攻撃する
		Leave,    // 離脱する
	};
	~Enemy();
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_;
	float enemyInputFloat[3]{0, 0, 0};
	Phase phase_ = Phase ::Approach;
	Vector3 velocity_;
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;
	
};
