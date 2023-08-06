#pragma once
#include "Model.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"

class Player {
public:
	
	///
	///
	///
	///
	///
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	///
	///
	///
	void Update(ViewProjection view);

	///
	///
	///
	void Draw(ViewProjection view);
	///
	///
	///
	void Atack();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	void OnCollision();
	
	void DrawUI();
	Vector3  GetWorldPosition();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	void MouseUpdate(ViewProjection& view);
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	/// <summary>
/// 
/// </summary>
	~Player();
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	
	std::list<PlayerBullet*> bullets_;
	WorldTransform worldTransform3DReticle_;
	Sprite* sprite2DReticle_ = nullptr;
	int StopTimer = 0;
	bool isControl = false;
	bool isAttack = false;
	
};