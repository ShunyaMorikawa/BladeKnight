//========================================
//
//シューティングアクション[player.cpp]
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
#include "game.h"

//========================================
//マクロ定義
//========================================
#define PLAYER_SPEED	(4.0f)		//プレイヤーの移動速度
#define PLAYER_INERTIA	(0.3f)		//プレイヤーの慣性

#define PLAYER_PATH	"data\\FILE\\player.txt"	//読み込むファイルのパス

//========================================
//コンストラクタ
//========================================
CEnemy::CEnemy() :
	m_pTexture(nullptr),
	m_pos(0.0f, 0.0f, 0.0f),		//位置
	m_move(0.0f, 0.0f, 0.0f),		//移動量
	m_rot(0.0f, 0.0f, 0.0f),		//向き
	m_nIdxTexture(0),				//テクスチャの番号
	m_pMesh(nullptr),				//メッシュ(頂点情報)へのポインタ
	m_pBuffMat(nullptr),			//マテリアルへのポインタ
	m_dwNumMat(0),					//マテリアルの数
	m_apNumModel(0), 				//モデル(パーツ)の総数
	m_RotDest(0.0f, 0.0f, 0.0f),	//目的の向き
	m_nLife(0),						// 体力
	m_bJump(false),
	m_bMove(false),
	m_bWait(false)
{//値をクリア
}

//========================================
//デストラクタ
//========================================
CEnemy::~CEnemy()
{
}

//========================================
//プレイヤーの生成
//========================================
CEnemy *CEnemy::Create(void)
{
	//CEnemy型のポインタ
	CEnemy *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//プレイヤー生成
		pPlayer = new CEnemy;

		//初期化
		pPlayer->Init();
	}

	//ポインタを返す
	return pPlayer;
}

//========================================
//初期化
//========================================
HRESULT CEnemy::Init(void)
{
	//モーションのポインタ
	m_pMotion = nullptr;

	//目的の向き
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//プレイヤーの初期位置
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, -300.0f));

	//プレイヤーの初期向き
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 体力
	m_nLife = 10;

	if (m_pMotion == nullptr)
	{
		//モーション生成
		m_pMotion = CMotion::Create();

		//モーション読み込み
		m_pMotion->Load(PLAYER_PATH);

		//待機状態
		m_bWait = true;
	}

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CEnemy::Uninit(void)
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
//更新
//========================================
void CEnemy::Update(void)
{
	// 現在のモーション
	EMotion nowMotion = MOTION_STANDBY;

	//CInputKeyboard型のポインタ
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//位置取得
	D3DXVECTOR3 pos = GetPosition();

	//向き取得
	D3DXVECTOR3 rot = GetRot();

	//位置更新
	//SetPosition(D3DXVECTOR3(pos.x += m_move.x, 0.0f, pos.z += m_move.z));

	if (m_pMotion != nullptr)
	{//モーション更新
		m_pMotion->Update();
	}

	//ポインタ
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//デバッグ表示
	pDebugProc->Print("\nプレイヤーの位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("プレイヤーの向き：%f、%f、%f\n", rot.x, rot.y, rot.z);
}

//========================================
//描画
//========================================
void CEnemy::Draw(void)
{
	//描画
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

									//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//位置取得
	D3DXVECTOR3 pos = GetPosition();

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

//========================================
// モーション種類の取得
//========================================
int CEnemy::GetMotionType()
{
	return m_pMotion->GetType();
}

//========================================
// モーションループの取得
//========================================
bool CEnemy::GetMotionLoop(int nType)
{
	return m_pMotion->GetLoop(nType);
}

//=======================================
//rotの正規化
//=======================================
float CEnemy::RotNormalize(float RotN, float Rot)
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
//3Dオブジェクトの生成
//========================================
void CEnemy::SetVertex(void)
{
}

//========================================
//3Dオブジェクトの生成
//========================================
void CEnemy::SetSize(float fWidht, float fHeight)
{
}
