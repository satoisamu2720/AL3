#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "VectraCalculation.h"
#include <fstream>
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	delete skydome_;
	delete modelSkydome_;
	delete debugCamera_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	textureHandle_ = TextureManager::Load("genshin.png");

	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	worldTransform_.Initialize();
	
	
	viewProjection_.Initialize();

	player_ = new Player();
	Vector3 playerPosition(0, -1, 10);
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, playerPosition);

	LoadEnemyPopData();

	
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
	
	railCamera_ = new RailCamera;
	railCamera_->Initialize({0.0f, 0.0f, -30.0f}, {0.0f, 0.0f, 0.0f});

	player_->SetParent(&railCamera_->GetWorldTransform());

	debugCamera_ = new DebugCamera(1280, 720);
	
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {
	player_->Update(viewProjection_); 
	UpDateEnemyPopCommands();
	CheckAllCollisions();
	skydome_->Update();
	
	

	debugCamera_->Update();
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	CheckAllCollisions();
	//デバックカメラのifdef

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT) && isDebugcameraActive_ == false) {
		isDebugcameraActive_ = true;
	} else if (input_->TriggerKey(DIK_LSHIFT) && isDebugcameraActive_ == true) {
		isDebugcameraActive_ = false;
		
	}
    #endif
	

	
	//カメラ処理
	if (isDebugcameraActive_ ==true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	
}

	void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();


	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();



	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	player_->Draw(viewProjection_);
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
	skydome_->Draw(viewProjection_);
	Model::PostDraw();



	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	player_->DrawUI();
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();


}

	void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;
	float enemyBulletRadius = 0.5f;
	float playerBulletRadius = 0.5f;
	float playerRadius = 1.0f;
	float enemyRadius = 1.0f;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	
	
#pragma region
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : bullets_) {
		posB = bullet->GetWorldPosition();
		float collide = {
		    (posB.x - posA.x) * (posB.x - posA.x)+ (posB.y - posA.y) * (posB.y - posA.y)+
		    (posB.z - posA.z) * (posB.z - posA.z)};
		if (collide <= (playerRadius+enemyBulletRadius)*( enemyRadius+playerBulletRadius)){
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion
#pragma region
	
	
	for (PlayerBullet* Pbullet : playerBullets) {
		posB = Pbullet->GetWorldPosition();
		for (EnemyBullet* Ebullet : bullets_) {
			posA = Ebullet->GetWorldPosition();
		float collide = {
		    (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		    (posB.z - posA.z) * (posB.z - posA.z)};
			if (collide <=
			    (enemyRadius + playerBulletRadius) * (enemyRadius + playerBulletRadius)) {
			Pbullet->OnCollision();
				Ebullet->OnCollision();
			}
		}
	}
#pragma endregion
#pragma region
	for (Enemy* enemy : enemys_) {
		posB = enemy->GetWorldPosition();
		for (PlayerBullet* bullet : playerBullets) {
			posA = bullet->GetWorldPosition();
			
				float collide = {
				    (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
				    (posB.z - posA.z) * (posB.z - posA.z)};
				if (collide <=
				    (enemyRadius + playerBulletRadius) * (enemyRadius + playerBulletRadius)) {
				    enemy->OnCollision();
					bullet->OnCollision();
				}
			
		}
	}
#pragma endregion
 }
    void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	bullets_.push_back(enemyBullet);
    }

    void GameScene::AddEnemy(Enemy* enemy) { enemys_.push_back(enemy); }

    void GameScene::EnemyIni(Model* model, const Vector3 position) {
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model, position, this);
	newEnemy->SetPlayer(player_);
	AddEnemy(newEnemy);
    }

    void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
    }

    void GameScene::UpDateEnemyPopCommands() {

	if (isWait_) {
		waitTimer_--;

		if (waitTimer_ <= 0) {
			isWait_ = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		} else if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyIni(model_, Vector3(x, y, z));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTimer_ = waitTime;
			break;
		}
	}
    }

