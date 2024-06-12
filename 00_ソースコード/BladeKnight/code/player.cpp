//========================================
//
//プレイヤー[player.cpp]
//Author：森川駿弥
//
//========================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "particle.h"
#include "enemy.h"
#include "useful.h"
#include "gauge.h"
#include "main.h"
#include "model.h"
#include "motion.h"
#include "effect.h"
#include "object2D.h"

//========================================
//名前空間
//========================================
namespace
{
	const int LIFE = 10;			// 体力
	const float NOCKBACK = 50.0f;	// ノックバック値
	const float SPEED = 4.0f;		// 速度
	const float INERTIA = 0.3f;		// 慣性
	const float RADIUS = 50.0f;		// 半径
}

//========================================
//コンストラクタ
//========================================
CPlayer::CPlayer(int nPriority) : CCharacter(nPriority)
{//値をクリア

	m_pEffect = nullptr;			// エフェクトのポインタ
	m_pGauge = nullptr;				// ゲージのポインタ
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデル情報
}

//========================================
//デストラクタ
//========================================
CPlayer::~CPlayer()
{
}

//========================================
//プレイヤーの生成
//========================================
CPlayer *CPlayer::Create(std::string pfile)
{
	//CPlayer型のポインタ
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//プレイヤー生成
		pPlayer = new CPlayer;

		//初期化
		pPlayer->Init(pfile);
	}

	//ポインタを返す
	return pPlayer;
}

//========================================
// 初期化
//========================================
HRESULT CPlayer::Init(std::string pfile)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// キャラの初期化
	CCharacter::Init(pfile);

	// 位置設定
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 500.0f));

	// 向き設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 体力
	m_nLife = LIFE;

	// ゲージ生成
	m_pGauge = CGauge::Create(m_nLife);

	// 位置設定
	m_pGauge->SetPos(D3DXVECTOR3(50.0f, 600.0f, 0.0f));

	// サイズ設定
	m_pGauge->SetSize(50.0f, 50.0f);

	// テクスチャ設定
	m_pGauge->BindTexture(pTexture->Regist("data\\texture\\gauge.png"));

	return S_OK;
}

//========================================
//終了
//========================================
void CPlayer::Uninit(void)
{
	// 敵の情報取得
	CGame::GetInstance()->SetPlayer(nullptr);

	// 終了
	CCharacter::Uninit();
}

//========================================
//更新
//========================================
void CPlayer::Update(void)
{
	// キーボードの情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// コントローラーの情報取得	
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRot();

	// カメラ追従
	CCamera* pCampera = CManager::GetInstance()->GetCamera();
	pCampera->following(pos, rot);

	// プレイヤー行動
	Act(SPEED);

	if (pInputKeyboard->GetTrigger(DIK_F1))
	{
		Hit(1);
	}

	// ゲージに体力設定
	m_pGauge->SetLife(m_nLife);

	// デバッグ表示の情報取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// デバッグ表示
	pDebugProc->Print("\nプレイヤーの位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("プレイヤーの移動量：%f、%f、%f\n", move.x, move.y, move.z);
	pDebugProc->Print("プレイヤーの向き：%f、%f、%f\n", rot.x, rot.y, rot.z);
	pDebugProc->Print("プレイヤーの体力：%d\n", m_nLife);
}

//========================================
//描画
//========================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//========================================
// 行動
//========================================
void CPlayer::Act(float fSpeed)
{
	// モーション解除
	m_bMove = false;

	// キーボードの情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// コントローラーの情報取得	
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	//CCamera型のポインタ
	CCamera* pCamera = CManager::GetInstance()->GetCamera();;

	// カメラの向き取得
	D3DXVECTOR3 Camrot = pCamera->GetRot();

	// 目的の向き
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRot();

	// 目的の向き取得
	D3DXVECTOR3 RotDest = GetRotDest();

	if (pInputKeyboard->GetPress(DIK_A) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//Aが押された
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//左上
			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y - D3DX_PI * 0.25f;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//左下
			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y - D3DX_PI * 0.75f;
		}
		else
		{//左
			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.5f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.5f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y - D3DX_PI * 0.5f;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//Dが押された
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//右上
			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y + D3DX_PI * 0.25f;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//右下
			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y + D3DX_PI * 0.75f;
		}
		else
		{//右
			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.5f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.5f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y + D3DX_PI * 0.5f;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true
		|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//Wが押された
		// パーティクル生成
		Myparticle::Create(Myparticle::TYPE_WALK, pos);

		// 歩き
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI) * fSpeed;

		//移動方向にモデルを向ける
		RotDest.y = Camrot.y;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true
		|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//Sが押された
		// パーティクル生成
		Myparticle::Create(Myparticle::TYPE_WALK, pos);

		// 歩き
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;

		//移動方向にモデルを向ける
		RotDest.y = Camrot.y + D3DX_PI;
	}

	//位置を更新
	pos.x += move.x;
	pos.z += move.z;

	//移動量を更新(減衰させる)
	move.x += (0.0f - move.x) * INERTIA;
	move.z += (0.0f - move.z) * INERTIA;

	//目的の向き
	DiffRot.y = RotDest.y - rot.y;

	// 向きの正規化
	USEFUL::NormalizeRotAngle(DiffRot.y);

	//Diffに補正係数をかける
	rot.y += DiffRot.y * 0.1f;

	// 角度の正規化
	USEFUL::NormalizeRotAngle(rot.y);

	// 位置設定
	SetPos(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRot(rot);

	// 目的の向き設定
	SetRotDest(RotDest);

	// 攻撃
	Attack();

	// モーション
	Motion();

	// 敵との当たり判定
	//CollisionEnemy(1);5
}

//========================================
// 攻撃
//========================================
void CPlayer::Attack()
{
	// キーボードの情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// コントローラーの情報取得	
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	// モード取得
	int nMode = CManager::GetInstance()->GetMode();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_X, 0) == true)
	{// 切りおろし
		m_bCutdown = true;

		// 敵との当たり判定
		CollisionEnemy(1);
	}

	if (pInputKeyboard->GetTrigger(DIK_E) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// 横薙ぎ
		m_bMowingdown = true;

		// 敵との当たり判定
		CollisionEnemy(2);
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_RB, 0) == true)
	{// 強攻撃
		m_bStrongAttack = true;

		// 敵との当たり判定
		CollisionEnemy(3);
	}
}

//========================================
// モーション管理
//========================================
void CPlayer::Motion()
{
	// モーション情報取得
	CMotion* pMotion = GetMotion();

	if (m_bMove)
	{// 歩きモーション
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_WALK);
	}
	else if (m_bCutdown)
	{// 切り下ろしモーション
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_CUTDOWN);

		if (pMotion->IsFinish() && m_bCutdown == true)
		{// モーション終了
			m_bCutdown = false;
		}
	}
	else if (m_bMowingdown)
	{// 薙ぎ払い
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_MOWINGDOWN);

		if (pMotion->IsFinish() && m_bMowingdown == true)
		{// モーション終了
			m_bMowingdown = false;
		}
	}
	else if (m_bStrongAttack)
	{// 強攻撃
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_STRONGATTACK);

		if (pMotion->IsFinish() && m_bStrongAttack == true)
		{// モーション終了
			m_bStrongAttack = false;
		}
	}
	else
	{// 待機モーション
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);
	}

	if (pMotion != nullptr)
	{// モーション更新
		pMotion->Update();
	}
}

//========================================
// 敵とプレイヤーの当たり判定
//========================================
void CPlayer::CollisionEnemy(int nDamage)
{
	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// 武器の位置
	D3DXMATRIX posWeapon;

	// モーション情報取得
	CMotion* pMotion = GetMotion();

	// モデルのオフセット取得
	CModel *pModelOffset = pMotion->GetModel(13);

	// モデルのマトリックス取得
	D3DXMATRIX MtxModel = pModelOffset->GetMtxWorld();

	// 位置取得
	D3DXVECTOR3 posPlayer = GetPos();

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 150.0f, 0.0f);
	D3DXMatrixMultiply(&posWeapon, &mtxTrans, &MtxModel);

	// マトリックスの位置
	D3DXVECTOR3 pos = D3DXVECTOR3(posWeapon._41, posWeapon._42, posWeapon._43);

#ifdef _DEBUG
	// エフェクト生成
	CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 100.0f, 1, true, CEffect::TYPE::TYPE_NORMAL);
#endif

	// 長さ
	float fLength;

	// プレイヤーの半径
	float fRadius = RADIUS;

	// 敵の情報取得
	CEnemy* pEnemy = CGame::GetInstance()->GetEnemy();

	if (pEnemy != nullptr)
	{
		// 位置取得
		D3DXVECTOR3 posEnemy = pEnemy->GetPos();

		// 移動量取得
		D3DXVECTOR3 moveEnemy = pEnemy->GetMove();

		// 半径
		float radiusEnemy = pEnemy->GetRadius();

#ifdef _DEBUG
	CEffect::Create(posEnemy, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), radiusEnemy, 1, true, CEffect::TYPE::TYPE_NORMAL);
#endif

		// ベクトルを求める
		D3DXVECTOR3 vec = posEnemy - pos;

		// ベクトル代入
		fLength = D3DXVec3Length(&vec);

		if (fLength <= radiusEnemy + fRadius)
		{// ヒット
			pEnemy->Hit(nDamage);

			// ノックバック
			NockBack();
		}
	}
}

//========================================
// ノックバック
//========================================
void CPlayer::NockBack()
{
	// 敵の情報取得
	CEnemy* pEnemy = CGame::GetInstance()->GetEnemy();

	// 位置取得
	D3DXVECTOR3 posPlayer = GetPos();

	// 敵の位置と移動量
	D3DXVECTOR3 posEnemy = pEnemy->GetPos();
	D3DXVECTOR3 moveEnemy = pEnemy->GetMove();

	// 飛ばされる向き
	float angle = atan2f(posPlayer.x - posEnemy.x, posPlayer.z - posEnemy.z);

	// 位置更新
	moveEnemy.x = sinf(angle) * -NOCKBACK;
	moveEnemy.z = cosf(angle) * -NOCKBACK;

	moveEnemy.y = 25.0f;

	pEnemy->SetMove(moveEnemy);
}

//========================================
// ヒット処理
//========================================
void CPlayer::Hit(int nLife)
{
	D3DXVECTOR3 pos = GetPos();

	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 体力減らす
	m_nLife -= nLife;

	// ゲージに体力設定
	m_pGauge->SetLife(m_nLife);

	// パーティクル生成
	Myparticle::Create(Myparticle::TYPE_DEATH, pos);

	if (m_nLife <= 0)
	{
		Uninit();

		CObject2D* pObje2D = CObject2D::Create();

		pObje2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 00.0f));

		// サイズ設定
		pObje2D->SetSize(1280.0f, 200.0f);

		// 勝敗テクスチャ
		pObje2D->BindTexture(pTexture->Regist("data\\texture\\lose.png"));
	}
}
