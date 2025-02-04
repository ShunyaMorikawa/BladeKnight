//========================================
//
// 敵の処理[enemy.cpp]
// Author：森川駿弥
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
#include "gauge.h"
#include "result.h"

//========================================
// 静的メンバ変数
//========================================
CEnemy* CEnemy::m_pEnemy = nullptr;

//========================================
//名前空間
//========================================
namespace
{
	const int LIFE = 20;				// 体力
	const int ATTACKCOUNTER = 180;		// 攻撃するまでの時間
	const int MAXDIRECTION = 8;			// 弾を飛ばす最大方向
	const int BULLETLIFE = 120;			// 弾の寿命
	
	const float BULLETMOVE = 10.0f;		// 弾の移動量
	const float SPEED = 0.3f;			// 速度
	const float GRAVITY = 2.0f;			// 重力
	const float INERTIA = 0.1f;			// 慣性
	const float RADIUS = 200.0f;		// 半径
	const float NOCKBACK = 50.0f;		// ノックバック値
	const float GAUGE_SIZE = 50.0f;		// ゲージサイズ
	const float RESULT_HEIGHT = 200.0f;		// リザルトの高さ

	const D3DXVECTOR3 INITIAL_POS = { 0.0f, 0.0f, -500.0f };	// 敵の初期位置
	const D3DXVECTOR3 GAUGE_POS = { 600.0f, 25.0f, 0.0f };	// 敵ゲージの初期位置
}

//========================================
//コンストラクタ
//========================================
CEnemy::CEnemy(int nPriority) : CCharacter(nPriority),
m_nLife		(0),			// 体力
m_nCnt		(0),			// カウント
m_fRadius	(0.0f),			// 半径
m_bWalk		(false),		// 歩き
m_bAttack	(false),		// 攻撃
m_eState	(STATE_NONE),	// 状態
m_pGauge	(nullptr)		// ゲージ
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
	SetPos(INITIAL_POS);

	// 向き設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 体力
	m_nLife = LIFE;

	// 半径
	m_fRadius = RADIUS;

	// ゲージ生成
	m_pGauge = CGauge::Create(m_nLife);

	// 位置設定
	m_pGauge->SetPos(GAUGE_POS);

	// サイズ設定
	m_pGauge->SetSize(GAUGE_SIZE, GAUGE_SIZE);

	return S_OK;
}

//========================================
// 終了
//========================================
void CEnemy::Uninit(void)
{
	if (m_pGauge != nullptr)
	{// ゲージが使用されていた場合
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}

	// 終了
	CCharacter::Uninit();
	m_pEnemy = nullptr;
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

	// キーボードの情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

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
			CBullet::Create(pos, bulletmove, BULLETLIFE);
		}

		// サウンド情報取得
		CSound* pSound = CManager::GetInstance()->GetSound();

		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BULLET);

		m_nCnt = 0;
	}

	if (m_pGauge != nullptr)
	{
		// ゲージに体力設定
		m_pGauge->SetLife(m_nLife);
	}

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

#ifdef _DEBUG
	if (pInputKeyboard->GetPress(DIK_F4))
	{// 体力減算
		Hit(LIFE);
	}
#endif

	// デバッグ表示
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("\n敵の位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("敵の向き：%f、%f、%f\n", rot.x, rot.y, rot.z);
	pDebugProc->Print("敵の移動量：%f、%f、%f\n", move.x, move.y, move.z);
	pDebugProc->Print("敵の体力：%d\n", m_nLife);
	pDebugProc->Print("F4で敵の体力0\n");
}

//========================================
// 描画
//========================================
void CEnemy::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 描画
	CCharacter::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 描画
	CCharacter::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
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

	//テクスチャの情報取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 体力減らす
	m_nLife -= nLife;

	if (m_pGauge != nullptr)
	{
		// ゲージに体力設定
		m_pGauge->SetLife(m_nLife);
	}

	if (m_nLife <= 0)
	{
		CResult::ChangeFrag(true);

		// パーティクル生成
		Myparticle::Create(Myparticle::TYPE_DEATH, pos);

		// 生成
		CObject2D* pObje2D = CObject2D::Create();

		// 位置設定
		pObje2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 00.0f));

		// サイズ設定
		pObje2D->SetSize(1280.0f, 200.0f);

		// 勝利テクスチャ
		pObje2D->BindTexture(pTexture->Regist("data\\texture\\win.png"));

		// 終了
		Uninit();
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
		movePlayer.y = NOCKBACK * 0.5f;

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
	if (pMotion == nullptr)
	{
		return;
	}

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

	// 移動量設定
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
		// プレイヤーの位置・移動量・半径・状態取得
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		float radiusEnemy = pPlayer->GetRadius();
		int statePlayer = pPlayer->GetState();

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

		if (statePlayer == CPlayer::STATE_DAMAGE)
		{// ダメージ状態の時
			int nCnt = 0;
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
