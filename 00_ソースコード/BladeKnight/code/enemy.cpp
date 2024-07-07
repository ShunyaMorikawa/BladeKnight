//========================================
//
//敵関連[enemy.cpp]
//Author：森川駿弥
//
//========================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "particle.h"
#include "player.h"
#include "game.h"
#include "useful.h"
#include "effect.h"
#include "bullet.h"
#include "useful.h"
#include "sound.h"
#include "fade.h"

//========================================
//名前空間
//========================================
namespace
{
	const int LIFE = 10;			// 体力
	const int ATTACKCOUNTER = 180;	// 攻撃するまでの時間
	const int MAXDIRECTION = 8;		// 弾を飛ばす最大方向
	const float BULLETMOVE = 10.0f;	// 弾の移動量
	const float SPEED = 0.3f;		// 速度
	const float GRAVITY = 2.0f;		// 重力
	const float INERTIA = 0.1f;		// 慣性
	const float RADIUS = 200.0f;	// 半径
	const float NOCKBACK = 50.0f;	// ノックバック値
}

//========================================
// 静的メンバ変数
//========================================
CEnemy* CEnemy::m_pEnemy = nullptr;

//========================================
//コンストラクタ
//========================================
CEnemy::CEnemy() : 
	m_nLife(0),
	m_fRadius(0.0f),
	m_bWalk(false),
	m_bAttack(false),
	m_pGauge(nullptr)
{//値をクリア
}

//========================================
//デストラクタ
//========================================
CEnemy::~CEnemy()
{
}

//========================================
// 生成
//========================================
CEnemy* CEnemy::Create(std::string pfile)
{
	if (m_pEnemy == nullptr)
	{//	インスタンス生成
		m_pEnemy = new CEnemy;

		m_pEnemy->Init(pfile);
	}

	return m_pEnemy;
}

//========================================
// 初期化
//========================================
HRESULT CEnemy::Init(std::string pfile)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// キャラの初期化
	CCharacter::Init(pfile);

	// 位置設定
	SetPos(D3DXVECTOR3(0.0f, 0.0f, -500.0f));

	// 向き設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 体力
	m_nLife = LIFE;

	// 半径
	m_fRadius = RADIUS;

	m_nCnt = 0;

	// ゲージ生成
	m_pGauge = CGauge::Create(m_nLife);

	// 位置設定
	m_pGauge->SetPos(D3DXVECTOR3(600.0f, 25.0f, 0.0f));

	// サイズ設定
	m_pGauge->SetSize(50.0f, 50.0f);

	// ゲージテクスチャ
	m_pGauge->BindTexture(pTexture->Regist("data\\texture\\gauge.png"));

	return S_OK;
}

//========================================
// 終了
//========================================
void CEnemy::Uninit(void)
{
	// 終了
	CCharacter::Uninit();

	if (m_pEnemy != nullptr)
	{
		m_pEnemy = nullptr;
	}

	m_pGauge = nullptr;
}

//========================================
// 更新
//========================================
void CEnemy::Update(void)
{
	// 目的の向き
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 更新
	CCharacter::Update();

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRot();

	// 目的の向き取得
	D3DXVECTOR3 RotDest = GetRotDest();

	// プレイヤー情報の取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// 位置取得
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();

		// プレイヤーとの角度
		RotDest.y = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z);
	}

	// プレイヤー方向に移動
	move.x += sinf(rot.y + D3DX_PI) * SPEED;
	move.z += cosf(rot.y + D3DX_PI) * SPEED;

	// 位置を更新
	pos += move;

	// 移動量を更新(減衰させる)
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

	// 重力
	move.y -= GRAVITY;

	if (pos.y <= 0.0f)
	{// 位置と移動量の更新停止
		pos.y = 0.0f;
		move.y = 0.0f;
	}
	
	// カウント加算
	m_nCnt++;

	if (m_nCnt >= ATTACKCOUNTER)
	{// 攻撃するまでの時間
		m_bAttack = true;

		for (int n = 0; n < MAXDIRECTION; n++)
		{// 弾を8方向に飛ばす
			float fAngle = D3DX_PI * 2.0f / MAXDIRECTION;

			// 発射角度をずらす
			fAngle *= n;
			D3DXVECTOR3 bulletmove;
			bulletmove.x = sinf(fAngle) * BULLETMOVE;
			bulletmove.y = 0.0f;
			bulletmove.z = cosf(fAngle) * BULLETMOVE;

			// 弾の生成
			CBullet::Create(pos, bulletmove, 120);
		}

		// サウンド情報取得
		CSound* pSound = CManager::GetInstance()->GetSound();

		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BULLET);

		m_nCnt = 0;
	}

	// ゲージに体力設定
	m_pGauge->SetLife(m_nLife);

	// 位置設定
	SetPos(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRot(rot);

	// 目的の向き設定
	SetRotDest(RotDest);

	// モーション管理
	Motion();

	// 闘技場との当たり判定
	CollisionCircle();

	// デバッグ表示
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("\n敵の位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("敵の向き：%f、%f、%f\n", rot.x, rot.y, rot.z);
	pDebugProc->Print("敵の移動量：%f、%f、%f\n", move.x, move.y, move.z);
	pDebugProc->Print("敵の体力：%d\n", m_nLife);
}

//========================================
// 描画
//========================================
void CEnemy::Draw(void)
{
	// 描画
	CCharacter::Draw();
}

//========================================
// ヒット処理
//========================================
void CEnemy::Hit(int nLife)
{
	//CInputKeyboard情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad情報取得
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	D3DXVECTOR3 pos = GetPos();

	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 状態取得
	int nState = GetState();

	int nCount = 0;

	nCount++;

	// 体力減らす
	m_nLife -= nLife;

	if (m_pGauge != nullptr)
	{
		// ゲージに体力設定
		m_pGauge->SetLife(m_nLife);
	}

	if (m_nLife <= 0)
	{
		// 死亡状態
		nState = STATE_DEATH;

		// パーティクル生成
		Myparticle::Create(Myparticle::TYPE_DEATH, pos);

		// 終了
		Uninit();

		// 生成
		CObject2D* pObje2D = CObject2D::Create();

		// 位置設定
		pObje2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 00.0f));

		// サイズ設定
		pObje2D->SetSize(1280.0f, 200.0f);

		// 勝利テクスチャ
		pObje2D->BindTexture(pTexture->Regist("data\\texture\\win.png"));
	
		// サウンド情報取得
		CSound* pSound = CManager::GetInstance()->GetSound();

		// サウンド停止
		pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);

		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_WIN);

		//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		//	pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true ||
		//	nCount >= 120)
		//{
		//	// 画面遷移(フェード)
		//	CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);

		//	nCount = 0;
		//}
	}
}

//========================================
// ノックバック
//========================================
void CEnemy::NockBack()
{
	// 敵の情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// 位置取得
		D3DXVECTOR3 posEnemy = GetPos();

		// プレイヤーの位置と移動量
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();

		// 飛ばされる角度
		float angle = atan2f(posEnemy.x - posEnemy.x, posPlayer.z - posEnemy.z);

		// 位置更新
		movePlayer.x = sinf(angle) * -NOCKBACK;
		movePlayer.z = cosf(angle) * -NOCKBACK;
		movePlayer.y = 25.0f;

		// 移動量設定
		pPlayer->SetMove(movePlayer);
	}
}

//========================================
// モーション管理
//========================================
void CEnemy::Motion()
{
	// モーション情報取得
	CMotion* pMotion = GetMotion();

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	if (m_bWalk)
	{// 歩きモーション
		pMotion->Set(CMotion::ENEMY_MOTIONTYPE_WALK);
	}
	else if (m_bAttack)
	{// 切り下ろしモーション
		pMotion->Set(CMotion::ENEMY_MOTIONTYPE_ATTACK);

		m_bWalk = false;

		move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (pMotion->IsFinish() && m_bAttack == true)
		{// モーション終了
			m_bAttack = false;
		}
	}
	else
	{
		pMotion->Set(CMotion::ENEMY_MOTIONTYPE_WALK);
	}

	if (pMotion != nullptr)
	{// モーション更新
		pMotion->Update();
	}

	SetMove(move);
}

//========================================
// プレイヤーとの当たり判定
//========================================
void CEnemy::CollisionPlayer(int nDamage)
{
	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// 武器の位置
	D3DXMATRIX posWeapon;

	// モーション情報取得
	CMotion* pMotion = GetMotion();

	// モデルのオフセット取得
	CModel* pModelOffset = pMotion->GetModel(13);

	// モデルのマトリックス
	D3DXMATRIX MtxModel = pModelOffset->GetMtxWorld();

	// 位置取得
	D3DXVECTOR3 posEnemy = GetPos();

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 150.0f, 0.0f);
	D3DXMatrixMultiply(&posWeapon, &mtxTrans, &MtxModel);

	// マトリックスの位置
	D3DXVECTOR3 pos = D3DXVECTOR3(posWeapon._41, posWeapon._42, posWeapon._43);

#ifdef _DEBUG
	// エフェクト生成
	CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 100.0f, 10, true, CEffect::TYPE::TYPE_NORMAL);
	CEffect::Create(posEnemy, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 25.0f, 10, true, CEffect::TYPE::TYPE_NORMAL);
#endif

	// プレイヤーの半径
	float fRadius = RADIUS;

	// 敵の情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// プレイヤーの位置
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();

		// 半径
		float radiusEnemy = pPlayer->GetRadius();

		// ベクトルを求める
		D3DXVECTOR3 vec = posEnemy - pos;

		// ベクトル代入
		float fLength = D3DXVec3Length(&vec);

		if (fLength <= radiusEnemy + fRadius)
		{// ヒット
			pPlayer->Hit(nDamage);

			// ノックバック
			NockBack();
		}
	}
}

//========================================
// 闘技場との当たり判定
//========================================
void CEnemy::CollisionCircle()
{
	// プレイヤー位置
	D3DXVECTOR3 posEnemy = GetPos();
	D3DXVECTOR3 vec;
	D3DXVec3Normalize(&vec, &posEnemy);

	if (USEFUL::CollisionCircle(posEnemy, Constance::ARENA_SIZE))
	{// 闘技場に当たったら
		posEnemy = vec * Constance::ARENA_SIZE;
	}

	// 位置設定
	SetPos(posEnemy);
}
