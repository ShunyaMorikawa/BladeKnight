//========================================
//
// チュートリアルエネミー[scarecrow.cpp]
// Author：森川駿弥
//
//========================================
#include "scarecrow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "tutorial.h"
#include "player.h"

//========================================
//マクロ定義
//========================================
#define MOTION_PATH	"data\\FILE\\scarecrow.txt"	//読み込むファイルのパス

//========================================
// コンストラクタ
//========================================
CScarecrow::CScarecrow():
	m_pos(0.0f, 0.0f, 0.0f),		// 位置
	m_rot(0.0f, 0.0f, 0.0f),		// 向き
	m_RotDest(0.0f, 0.0f, 0.0f),	// 目的の向き
	m_vtxMin(0.0f, 0.0f, 0.0f),		// 最小値
	m_vtxMax(0.0f, 0.0f, 0.0f),		// 最大値
	m_fSize(0.0f),					// サイズ
	m_fAngle(0.0f),					// 目的の向き
	m_nState(STATE_NONE),			// 状態
	m_pMotion(nullptr),				// モーションのポインタ
	m_pBuffMat(nullptr)				// マテリアルのポインタ
{
	memset(&m_apModel[0], 0, sizeof(m_apModel));	// モデルのポインタ
}

//========================================
// デストラクタ
//========================================
CScarecrow::~CScarecrow()
{
}

//========================================
// 生成
//========================================
CScarecrow *CScarecrow::Create()
{
	CScarecrow *pScarecrow = nullptr;

	if (pScarecrow == nullptr)
	{
		// インスタンス生成
		pScarecrow = new CScarecrow;

		// 初期化
		pScarecrow->Init();
	}

	return pScarecrow;
}

//========================================
// 初期化
//========================================
HRESULT CScarecrow::Init(void)
{	
	// 向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 向きの設定
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 最小値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 最大値
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 位置設定
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 500.0f));

	// サイズ
	m_fSize = 250.0f;

	// マテリアルのポインタ
	m_pBuffMat = nullptr;

	//モーションのポインタ
	m_pMotion = nullptr;

	if (m_pMotion == nullptr)
	{
		//モーション生成
		m_pMotion = CMotion::Create();

		//モーション読み込み
		m_pMotion->Load(MOTION_PATH);
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CScarecrow::Uninit(void)
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
void CScarecrow::Update(void)
{
	//目的の向き
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//プレイヤーの情報取得
	CPlayer *pPlayer = CTutorial::GetPlayer();

	if (pPlayer == NULL)
	{
		return;
	}

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	//プレイヤーとの角度
	m_fAngle = atan2f(posPlayer.x - m_pos.x, posPlayer.z - m_pos.z);

	//プレイヤーの方向に向ける
	m_rot.y = m_fAngle;

	if (m_pMotion != nullptr)
	{//モーション更新
		m_pMotion->Update();
	}

	//ポインタ
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//デバッグ表示
	pDebugProc->Print("\n敵の位置：%f、%f、%f\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("敵の向き：%f、%f、%f\n", m_rot.x, m_rot.y, m_rot.z);
}

//========================================
// 描画
//========================================
void CScarecrow::Draw(void)
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

	// マテリアル取得
	pDevice->GetMaterial();

	//モーション描画
	m_pMotion->Draw();
}

//=======================================
// rotの正規化
//=======================================
float CScarecrow::RotNormalize(float RotN, float Rot)
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
void CScarecrow::SetVertex(void)
{
}

//========================================
// 3Dオブジェクトの生成
//========================================
void CScarecrow::SetSize(float fWidht, float fHeight)
{
	fWidht = 0.0f;
	fHeight = 0.0f;
}
