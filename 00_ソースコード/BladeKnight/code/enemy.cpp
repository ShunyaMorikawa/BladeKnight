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

//========================================
//名前空間
//========================================
namespace
{
	const int LIFE = 10;			// 体力
	const float SPEED = 5.0f;		// 速度
	const float GRAVITY = 2.0f;		// 重力
	const float INERTIA = 0.1f;		// 慣性
	const float RADIUS = 150.0f;	// 半径
}

//========================================
//コンストラクタ
//========================================
CEnemy::CEnemy() : 
	m_nLife(0),
	m_fRadius(0.0f)
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
	CEnemy* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemy;

		pEnemy->Init(pfile);
	}

	return pEnemy;
}

//========================================
// 初期化
//========================================
HRESULT CEnemy::Init(std::string pfile)
{
	// キャラの初期化
	CCharacter::Init(pfile);

	// 位置設定
	SetPos(D3DXVECTOR3(0.0f, 0.0f, -300.0f));

	// 向き設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 体力
	m_nLife = LIFE;

	// 半径
	m_fRadius = RADIUS;

#ifdef _DEBUG
	m_nLife = 50;
#endif

	return S_OK;
}

//========================================
// 終了
//========================================
void CEnemy::Uninit(void)
{
	// 終了
	CCharacter::Uninit();
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
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer();

	// 位置取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	// プレイヤーとの角度
	RotDest.y = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z);

	// プレイヤー方向に移動
	move.x = sinf(rot.y + D3DX_PI);
	move.z = cosf(rot.y + D3DX_PI);

	// 位置を更新
	pos += move;

	// 移動量を更新(減衰させる)
	move.x += (0.0f + move.x) * INERTIA;
	move.z += (0.0f + move.z) * INERTIA;


	//目的の向き
	DiffRot.y = RotDest.y - rot.y;

	USEFUL::NormalizeRot(DiffRot.y);

	//向きの正規化
	if (DiffRot.y > D3DX_PI)
	{//3.14を超えたときに反対にする
		DiffRot.y -= D3DX_PI * 2.0f;
	}

	//-3.14を超えたときに反対にする
	if (DiffRot.y < -D3DX_PI)
	{
		DiffRot.y += D3DX_PI * 2.0f;
	}

	//Diffに補正係数をかける
	rot.y += DiffRot.y * 0.1f;

	//角度の正規化
	if (rot.y > D3DX_PI)
	{//3.14を超えたときに反対にする
		rot.y -= D3DX_PI * 2.0f;
	}

	//-3.14を超えたときに反対にする
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	// 重力
	move.y -= GRAVITY;

	if (pos.y <= 0.0f)
	{// 位置と移動量の更新停止
		pos.y = 0.0f;
		move.y = 0.0f;
	}

	// 位置設定
	SetPos(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRot(rot);

	// 目的の向き設定
	SetRotDest(RotDest);

	// ポインタ
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("\n敵の位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("敵の向き：%f、%f、%f\n", rot.x, rot.y, rot.z);
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

//========================================
// 目的の方向へ向かう
//========================================
void CEnemy::DestMove()
{
}
