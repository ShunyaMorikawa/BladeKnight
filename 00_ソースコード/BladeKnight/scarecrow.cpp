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
#include "game.h"

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
	m_vtxMax(0.0f, 0.0f, 0.0f)		// 最大値
{
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
	m_RotDest = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	// 最小値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 最大値
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 位置設定
	SetPosition(D3DXVECTOR3(0.0f, 50.0f, 0.0f));

	//モーションのポインタ
	m_pMotion = nullptr;

	if (m_pMotion == nullptr)
	{
		//モーション生成
		m_pMotion = CMotion::Create();

		//モーション読み込み
		m_pMotion->Load(MOTION_PATH);

		//待機モーション
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CScarecrow::Uninit(void)
{
	//オブジェクト(自分自身)の破棄
	Release();
}

//========================================
// 更新
//========================================
void CScarecrow::Update(void)
{
	if (m_pMotion != nullptr)
	{//モーション更新
		m_pMotion->Update();
	}

	//ポインタ
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//デバッグ表示
	pDebugProc->Print("\n敵の位置：%f、%f、%f\n", m_pos.x, m_pos.y, m_pos.z);
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
}
