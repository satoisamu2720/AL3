#pragma once
#include "Model.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "assert.h"



class Enemy {
public:

	///
	///
	///
	///
	///
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///
	///
	///
	void Update();

	///
	///
	///
	void Draw(ViewProjection view);

	/// <summary>
	/// 行動フェーズ
	/// </summary>
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_;
	float enemyInputFloat[3]{0, 0, 0};
	Phase phase_ = Phase ::Approach;
	Vector3 velocity_;
	
};
