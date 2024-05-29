//========================================
//
// ボス[boss.cpp]
// Author：森川駿弥
//
//========================================
#include "boss.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "game.h"
#include "player.h"

//========================================
//マクロ定義
//========================================
#define MOTION_PATH	"data\\FILE\\boss.txt"	//読み込むファイルのパス
#define ENEMY_MOVE	(1.0f)

//========================================
// コンストラクタ
//========================================
CBoss::CBoss() :
	m_move(0.0f, 0.0f, 0.0f),		// 移動量
	m_pos(0.0f, 0.0f, 0.0f),		// 位置
	m_rot(0.0f, 0.0f, 0.0f),		// 向き
	m_RotDest(0.0f, 0.0f, 0.0f),	// 目的の向き
	m_vtxMin(0.0f, 0.0f, 0.0f),		// 最小値
	m_vtxMax(0.0f, 0.0f, 0.0f),		// 最大値
	m_fAngle(0.0f),					// 目的の向き
	m_fSize(0.0f),					// サイズ
	m_nLife(0),						// 体力
	m_nState(STATE_NONE),			// 状態
	m_pMotion(nullptr)
{// 値クリア
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデルのポインタ
}

//========================================
// デストラクタ
//========================================
CBoss::~CBoss()
{
}

//========================================
// 生成
//========================================
CBoss *CBoss::Create()
{
	CBoss *pBoss = nullptr;

	if (pBoss == nullptr)
	{
		// インスタンス生成
		pBoss = new CBoss;

		// 初期化
		pBoss->Init();
	}

	return pBoss;
}

//========================================
// 初期化
//========================================
HRESULT CBoss::Init(void)
{
	// 移動量
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 向き
	m_rot = D3DXVECTOR3(0.0f, -1.7f, 0.0f);

	// 向きの設定
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 最小値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 最大値
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 位置設定
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// サイズ
	m_fSize = 250.0f;

	// 体力
	m_nLife = 10;

	// 状態
	m_nState = STATE_NONE;

	//モーションのポインタ
	m_pMotion = nullptr;

	if (m_pMotion == nullptr)
	{
		//モーション生成
		m_pMotion = CMotion::Create();

		//モーション読み込み
		m_pMotion->Load("data\\FILE\\boss.txt");

		// 待機
		m_bWait = true;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CBoss::Uninit(void)
{
	if (m_pMotion != nullptr)
	{//モーション破棄
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//========================================
// 更新
//========================================
void CBoss::Update(void)
{
	// 位置取得
	D3DXVECTOR3 posEnemy = Getpos();

	// 移動量取得
	D3DXVECTOR3 moveEnemy = GetMove();

	//目的の向き
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 攻撃
	m_bAttack = false;

	// 突き
	m_bThrust = false;

	// 切り上げ
	m_bRoundup = true;

	//プレイヤーの情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer == NULL)
	{
		return;
	}
	// プレイヤーの位置
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	//プレイヤーとの角度
	m_fAngle = atan2f(posPlayer.x - posEnemy.x, posPlayer.z - posEnemy.z);

	//プレイヤーの方向に向ける
	m_rot.y = m_fAngle;

	//プレイヤー方向に移動
	m_move.x = sinf(m_rot.y) * ENEMY_MOVE;
	m_move.z = cosf(m_rot.y) * ENEMY_MOVE;

	// 位置更新
	m_pos += m_move;

	//if (m_bAttack)
	//{// 攻撃モーション
	//	m_pMotion->Set(CMotion::MOTIONTYPE_ATTACK);
	//}
	//else if (m_bThrust)
	//{// 突き攻撃モーション
	//	m_pMotion->Set(CMotion::MOTIONTYPE_THRUST);
	//}
	if (m_bRoundup)
	{// 切り上げモーション
		m_pMotion->Set(CMotion::ENEMY_MOTIONTYPE_ROUNDUP);

		// プレイヤーとの当たり判定
		//CollisionPlayer();
	}
	else
	{// 待機モーション
		m_pMotion->Set(CMotion::ENEMY_MOTIONTYPE_WALK);
	}

	if (m_pMotion != nullptr)
	{//モーション更新
		m_pMotion->Update();
	}

	if (m_nLife <= 0)
	{// 体力が0以下
		// 死亡状態
		m_nState = STATE_DEATH;
	}

	//ポインタ
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//デバッグ表示
	pDebugProc->Print("\n敵の位置：%f、%f、%f\n", posEnemy.x, posEnemy.y, posEnemy.z);
	pDebugProc->Print("敵の移動量：%f、%f、%f\n", moveEnemy.x, moveEnemy.y, moveEnemy.z);
	pDebugProc->Print("敵の向き：%f、%f、%f\n", m_rot.x, m_rot.y, m_rot.z);
	pDebugProc->Print("敵の体力：%d\n", m_nLife);
}

//========================================
// 描画
//========================================
void CBoss::Draw(void)
{
	//描画
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//位置取得
	D3DXVECTOR3 pos = Getpos();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モーション描画
	m_pMotion->Draw();
}

//=======================================
// 敵の移動
//=======================================
void CBoss::Move(float fSpeed)
{
	fSpeed = 0.0f;
}

//=======================================
// rotの正規化
//=======================================
float CBoss::RotNormalize(float RotN, float Rot)
{
	//角度の正規化
	if (RotN > D3DX_PI)
	{//3.14を超えたときに反対にする
		RotN -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (RotN < -D3DX_PI)
	{
		RotN += D3DX_PI * 2;
	}

	//Diffに補正係数をかける
	Rot += RotN * 0.1f;

	//角度の正規化
	if (Rot > D3DX_PI)
	{//3.14を超えたときに反対にする
		Rot -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (Rot < -D3DX_PI)
	{
		Rot += D3DX_PI * 2;
	}

	return RotN, Rot;
}

//========================================
// 3Dオブジェクトの生成
//========================================
void CBoss::SetVertex(void)
{
}

//========================================
// 3Dオブジェクトの生成
//========================================
void CBoss::SetSize(float fWidht, float fHeight)
{
	fWidht = 0.0f;
	fHeight = 0.0f;
}

//========================================
// 位置設定
//========================================
void CBoss::SetPosition(D3DXVECTOR3 pos)
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================================
// プレイヤーとの当たり判定
//========================================
void CBoss::CollisionPlayer()
{
	//変数宣言
	float fLength;		//長さ

	//プレイヤーの情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	//プレイヤーの位置取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// プレイヤーのサイズ取得
	float sizePlayer = pPlayer->GetSize();

	//ベクトルを求める
	D3DXVECTOR3 vec = posPlayer - this->GetPosition();

	//ベクトル代入
	fLength = D3DXVec3Length(&vec);

	if (fLength <= sizePlayer)
	{

	}
}
