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
	// キャラの初期化
	CCharacter::Init(pfile);

	// 位置設定
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 向き設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 体力
	m_nLife = LIFE;

	return S_OK;
}

//========================================
//終了
//========================================
void CPlayer::Uninit(void)
{
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

	//if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	//{// 弾の生成
	//	CBullet::Create(D3DXVECTOR3(pos),		// 位置
	//		D3DXVECTOR3(0.0f, 0.0f, -30.0f),	// 移動量 
	//		60);	// 体力
	//}

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
	CCamera* pCamera = nullptr;
	pCamera = CManager::GetInstance()->GetCamera();

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
			//移動方向にモデルを向ける
			RotDest.y = -D3DX_PI * 0.25f;

			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//左下
			//移動方向にモデルを向ける
			RotDest.y = -D3DX_PI * 0.75f;

			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//左
			//移動方向にモデルを向ける
			RotDest.y = -D3DX_PI * 0.5f;

			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.5f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//Dが押された
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//右上
			//移動方向にモデルを向ける
			RotDest.y = D3DX_PI * 0.25f;

			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//右下
			//移動方向にモデルを向ける
			RotDest.y = D3DX_PI * 0.75f;

			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//右
			//移動方向にモデルを向ける
			RotDest.y = D3DX_PI * 0.5f;

			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// 歩き
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.5f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true
		|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//Wが押された
		//移動方向にモデルを向ける
		RotDest.y = 0.0f;

		// パーティクル生成
		Myparticle::Create(Myparticle::TYPE_WALK, pos);

		// 歩き
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI) * fSpeed;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true
		|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//Sが押された
		//移動方向にモデルを向ける
		RotDest.y = D3DX_PI;

		// パーティクル生成
		Myparticle::Create(Myparticle::TYPE_WALK, pos);

		// 歩き
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;
	}

	//位置を更新
	pos.x += move.x;
	pos.z += move.z;

	//移動量を更新(減衰させる)
	move.x += (0.0f - move.x) * INERTIA;
	move.z += (0.0f - move.z) * INERTIA;

	//目的の向き
	DiffRot.y = RotDest.y - rot.y;

	//向きの正規化
	if (DiffRot.y > D3DX_PI)
	{//3.14を超えたときに反対にする
		DiffRot.y -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (DiffRot.y < -D3DX_PI)
	{
		DiffRot.y += D3DX_PI * 2;
	}

	//Diffに補正係数をかける
	rot.y += DiffRot.y * 0.1f;

	//角度の正規化
	if (rot.y > D3DX_PI)
	{//3.14を超えたときに反対にする
		rot.y -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

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

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_X, 0) == true)
	{// 切りおろし
		m_bCutdown = true;

		// 敵との当たり判定
		CollisionEnemy(1);
	}

	if (pInputKeyboard->GetTrigger(DIK_E) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// 切りおろし
		m_bMowingdown = true;

		// 敵との当たり判定
		CollisionEnemy(2);
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// 切りおろし
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

	// モデルのマトリックス
	D3DXMATRIX MtxModel = pModelOffset->GetMtxWorld();

	// 位置取得
	D3DXVECTOR3 posPlayer = GetPos();

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 150.0f, 0.0f);
	D3DXMatrixMultiply(&posWeapon, &mtxTrans, &MtxModel);

	// マトリックスの位置
	D3DXVECTOR3 pos = D3DXVECTOR3(posWeapon._41, posWeapon._42, posWeapon._43);

#ifdef _DEBUG

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 色

	// 半径の基準値
	float StandardRadius = 25.0f;

	int nLife = 10;

	// エフェクト生成
	CEffect::Create(pos, move, col, StandardRadius, nLife, true);
#endif

	// 長さ
	float fLength;

	// プレイヤーの半径
	float fRadius = RADIUS;

	// 敵の情報取得
	CEnemy* pEnemy = CGame::GetInstance()->GetEnemy();

	// 敵の位置
	D3DXVECTOR3 posEnemy = pEnemy->GetPos();
	D3DXVECTOR3 moveEnemy = pEnemy->GetMove();

	// 半径
	float radiusEnemy = pEnemy->GetRadius();

#ifdef _DEBUG
	CEffect::Create(posEnemy, move, col, radiusEnemy, nLife, true);
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

	// 体力減らす
	m_nLife -= nLife;

	if (m_nLife <= 0)
	{
		Uninit();

		// パーティクル生成
		Myparticle::Create(Myparticle::TYPE_DEATH, pos);
	}
}
