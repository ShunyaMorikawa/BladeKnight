//========================================
//
// プレイヤー[player.cpp]
// Author：森川駿弥
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
#include "object3D.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "lockonMarker.h"

//========================================
// 定数定義
//========================================
namespace
{
const int LIFE = 10;			// 体力
const float NOCKBACK = 50.0f;	// ノックバック値
const float SPEED = 4.0f;		// 速度
const float INERTIA = 0.3f;		// 慣性
const float RADIUS = 50.0f;		// 半径
const float FIELD_LIMIT = 4000.0f;	// フィールドの大きさ
const float MARKERPOS = 200.0f;		// ロックオンマーカーの位置
}

//========================================
// 静的メンバ変数
//========================================
CPlayer* CPlayer::m_pPlayer = nullptr;

//========================================
//コンストラクタ
//========================================
CPlayer::CPlayer(int nPriority) : CCharacter(nPriority)
{//値をクリア
	m_WalkCounter = 0;
	m_pEffect = nullptr;			// エフェクトのポインタ
	m_pGauge = nullptr;				// ゲージのポインタ
	m_IsLock = false;
	m_pMarker = nullptr;
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
	if (m_pPlayer == nullptr)
	{
		//プレイヤー生成
		m_pPlayer = new CPlayer;

		//初期化
		m_pPlayer->Init(pfile);
	}

	//ポインタを返す
	return m_pPlayer;
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

	// 歩行時のカウンター
	m_WalkCounter = 0;

	// 体力
	m_nLife = LIFE;

	// ゲージ生成
	m_pGauge = CGauge::Create(m_nLife);

	// 位置設定
	m_pGauge->SetPos(D3DXVECTOR3(50.0f, 600.0f, 0.0f));

	// サイズ設定
	m_pGauge->SetSize(50.0f, 50.0f);

	return S_OK;
}

//========================================
//終了
//========================================
void CPlayer::Uninit(void)
{
	// 終了
	CCharacter::Uninit();

	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}

	if (m_pGauge != nullptr)
	{
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}
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

	//テクスチャの情報取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// 敵の情報取得
	CEnemy* pEnemy = CEnemy::GetInstance();

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRot();

	// プレイヤー行動
	Act(SPEED);

#ifdef _DEBUG
	if (pInputKeyboard->GetPress(DIK_F1))
	{// 体力減算
		Hit(1);
	}

	if (pInputKeyboard->GetPress(DIK_F3))
	{// 体力減算
		Hit(LIFE);
	}
#endif

	// ゲージに体力設定
	m_pGauge->SetLife(m_nLife);

	if (m_nLife <= 0)
	{
		// 生成
		CObject2D* pObje2D = CObject2D::Create();

		// 位置設定
		pObje2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

		// サイズ設定
		pObje2D->SetSize(SCREEN_WIDTH, 200.0f);

		// 敗北テクスチャ
		pObje2D->BindTexture(pTexture->Regist("data\\texture\\lose.png"));

		// サウンド停止
		pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);

		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_LOSE);

		// 終了
		Uninit();
	}

	if (pInputKeyboard->GetTrigger(DIK_R) ||
		pInputPad->GetTrigger(CInputPad::BUTTON_PUSHING_R, 0))
	{
		m_IsLock = m_IsLock ? false : true;

		if (m_IsLock)
		{
			if (m_pMarker == nullptr)
			{// マーカー生成
				m_pMarker = CLockonMarker::Create(true);
			}
		}
		else
		{
			if (m_pMarker != nullptr)
			{// マーカー削除
				m_pMarker->Uninit();
				m_pMarker = nullptr;
			}
		}
	}

	// ロックオン
	LockOn();

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
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

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
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y - D3DX_PI * 0.25f;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//左下
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y - D3DX_PI * 0.75f;
		}
		else
		{//左
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
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y + D3DX_PI * 0.25f;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//右下
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;

			//移動方向にモデルを向ける
			RotDest.y = Camrot.y + D3DX_PI * 0.75f;
		}
		else
		{//右
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
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI) * fSpeed;

		//移動方向にモデルを向ける
		RotDest.y = Camrot.y;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true
		|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//Sが押された
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;

		//移動方向にモデルを向ける
		RotDest.y = Camrot.y + D3DX_PI;
	}

	if (m_bMove)
	{//位置を更新
		pos += move;

		//移動量を更新(減衰させる)
		move.x += (0.0f - move.x) * INERTIA;
		move.z += (0.0f - move.z) * INERTIA;

		m_WalkCounter = (m_WalkCounter + 1) % 20;

		if (m_WalkCounter == 0)
		{
			// パーティクル生成
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// サウンド再生
			CSound* pSound = CManager::GetInstance()->GetSound();
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		}
	}

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

	// フィールドとの当たり判定
	CollisionField();

	// 闘技場との当たり判定
	CollisionCircle();
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

		m_bMove = false;

		// 敵との当たり判定
		CollisionEnemy(1);
	}

	if (pInputKeyboard->GetTrigger(DIK_E) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// 横薙ぎ
		m_bMowingdown = true;

		m_bMove = false;

		// 敵との当たり判定
		CollisionEnemy(2);
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_RB, 0) == true)
	{// 強攻撃
		m_bStrongAttack = true;

		m_bMove = false;

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

		m_bCutdown = false;
	}
	else if (m_bMowingdown)
	{// 薙ぎ払い
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_MOWINGDOWN);

		m_bMowingdown = false;
	}
	else if (m_bStrongAttack)
	{// 強攻撃
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_STRONGATTACK);

		m_bStrongAttack = false;
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
	CModel* pModelOffset = pMotion->GetModel(13);

	// モデルのマトリックス取得
	D3DXMATRIX MtxModel = pModelOffset->GetMtxWorld();

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
	CEnemy* pEnemy = CEnemy::GetInstance();

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

		// 無敵時間
		int Invincible = 0;

		if (fLength <= radiusEnemy + fRadius)
		{// ヒット
			pEnemy->Hit(nDamage);

			// ノックバック
			NockBack();

			// サウンド情報取得
			CSound* pSound = CManager::GetInstance()->GetSound();

			// サウンド再生
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMYHIT);
		}
	}
}

//========================================
// ノックバック
//========================================
void CPlayer::NockBack()
{
	// 敵の情報取得
	CEnemy* pEnemy = CEnemy::GetInstance();

	// 位置取得
	D3DXVECTOR3 posPlayer = GetPos();

	if (pEnemy != nullptr)
	{
		// 敵の位置と移動量
		D3DXVECTOR3 posEnemy = pEnemy->GetPos();
		D3DXVECTOR3 moveEnemy = pEnemy->GetMove();

		// 飛ばされる向き
		float angle = atan2f(posPlayer.x - posEnemy.x, posPlayer.z - posEnemy.z);

		// 位置更新
		moveEnemy.x = sinf(angle) * -NOCKBACK;
		moveEnemy.z = cosf(angle) * -NOCKBACK;
		moveEnemy.y = 25.0f;

		// 移動量設定
		pEnemy->SetMove(moveEnemy);
	}
}

//========================================
// ヒット処理
//========================================
void CPlayer::Hit(int nLife)
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド再生
	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_HIT);

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 体力減らす
	m_nLife -= nLife;

	// ゲージに体力設定
	m_pGauge->SetLife(m_nLife);

	// パーティクル生成
	Myparticle::Create(Myparticle::TYPE_DEATH, pos);
}

//========================================
// フィールド外に行かないよう
//========================================
void CPlayer::CollisionField()
{
	// 位置・移動量取得
	D3DXVECTOR3 posPlayer = GetPos();
	D3DXVECTOR3 movePlayer = GetMove();

	if (posPlayer.x > FIELD_LIMIT)
	{
		posPlayer.x = FIELD_LIMIT;
		movePlayer.x = 0.0f;
	}
	else if (posPlayer.x < -FIELD_LIMIT)
	{
		posPlayer.x = -FIELD_LIMIT;
		movePlayer.x = 0.0f;

	}
	if (posPlayer.z > FIELD_LIMIT)
	{
		posPlayer.z = FIELD_LIMIT;
		movePlayer.z = 0.0f;
	}
	else if (posPlayer.z < -FIELD_LIMIT)
	{
		posPlayer.z = -FIELD_LIMIT;
		movePlayer.z = 0.0f;
	}

	// 位置・移動量設定
	SetPos(posPlayer);
	SetMove(movePlayer);
}

//========================================
// 闘技場との判定
//========================================
void CPlayer::CollisionCircle()
{
	// プレイヤー位置
	D3DXVECTOR3 posPlayer = GetPos();
	D3DXVECTOR3 vec;
	D3DXVec3Normalize(&vec, &posPlayer);

	if (USEFUL::CollisionCircle(posPlayer, Constance::ARENA_SIZE))
	{// 闘技場に当たったら
		posPlayer = vec * Constance::ARENA_SIZE;
	}

	// 位置設定
	SetPos(posPlayer);
}

//========================================
// ロックオン
//========================================
void CPlayer::LockOn()
{
	// カメラの情報取得
	CCamera* pCampera = CManager::GetInstance()->GetCamera();

	// 敵の情報取得
	CEnemy* pEnemy = CEnemy::GetInstance();

	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	if (pCampera == nullptr)
	{
		return;
	}

	if (pEnemy == nullptr)
	{
		// プレイヤーにカメラを追従させる
		D3DXVECTOR3 rot = pCampera->GetRot();

		pCampera->following(GetPos(), D3DXVECTOR3(0.0f, rot.y, 0.0f));

		return;
	}

	// プレイヤー・敵の位置
	D3DXVECTOR3 posPlayer = GetPos();
	D3DXVECTOR3 posEnemy = pEnemy->GetPos();

	// プレイヤーとの角度
	float RotDest = atan2f(posPlayer.x - posEnemy.x, posPlayer.z - posEnemy.z);

	if (m_IsLock)
	{// 敵の方向に向く
		pCampera->following(posPlayer, D3DXVECTOR3(0.0f, RotDest, 0.0f));

		// マーカーの位置設定
		D3DXVECTOR3 setpos = posEnemy;
		setpos.y += MARKERPOS;
		m_pMarker->SetPos(setpos);
	}
	else
	{// 追従
		D3DXVECTOR3 rot = pCampera->GetRot();
		pCampera->following(posPlayer, D3DXVECTOR3(0.0f, rot.y, 0.0f));
	}
}
