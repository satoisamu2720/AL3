#pragma once
#include "EnemyBullet.h"
#include "TextureManager.h"
#include "Model.h"
#include "Player.h"
#include "TimedCall.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Player;

class Enemy;

class GameScene;

class EnemyState {

protected:
	Enemy* enemy_ = nullptr;

public:
	virtual void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	virtual void Update(){};
};

class EnemyStateApproah : public EnemyState {

public:
	void Update();
};

class EnemyStateLeave : public EnemyState {

public:
	void Update();
};

class Enemy {

public:
	~Enemy();

	void Initialize(Model* model, const Vector3& position, GameScene* gameScene);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void ChangeState(EnemyState* newEnemyState);

	WorldTransform GetWT() { return worldTransform_; }

	void SetPosition(Vector3 velcity);

	void Attack();

	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	static const int kShotInterval = 60;

	bool IsDead() const { return isDead_; }

	void OnCollision();
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t texturehandle_;
	Phase phase_ = Phase::Approach;
	Player* player_ = nullptr;
	EnemyState* state;
	GameScene* gameScene_ = nullptr;
	std::list<EnemyBullet*> bullets_;
	std::list<TimedCall*> timedCall_;
	bool isDead_ = false;
	int timer = 0;
};
