//=======================================
//
// ロックンマーカー[rockonMarker.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "lockonMarker.h"      //作成したeffect.hをインクルードする
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===========================================
// 定数定義
//===========================================
namespace
{
const float MARKERSIZE = 90.0f;		// マーカーのサイズ
const std::string TEXTURE = "data\\TEXTURE\\rockon_point2.png";	// マーカーのテクスチャ
}

//===========================================
//コンストラクタ
//===========================================
CLockonMarker::CLockonMarker(int nPriority) : CBillboard(nPriority)
{
}

//===========================================
//デストラクタ
//===========================================
CLockonMarker::~CLockonMarker()
{
}

//===========================================
//生成
//===========================================
CLockonMarker *CLockonMarker::Create(bool alpha)
{
	//CLockonMarker型のポインタ
	CLockonMarker *pRockonMarker = nullptr;

	if (pRockonMarker == nullptr)
	{//nullptrの時
		//インスタンス生成
		pRockonMarker = new CLockonMarker;

		//初期化
		pRockonMarker->Init(alpha);
	}

	//ポインタを返す
	return pRockonMarker;
}

//===========================================
//初期化処理
//===========================================
HRESULT CLockonMarker::Init(bool alpha)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//初期化
	CBillboard::Init();

	// サイズ設定
	SetSize(MARKERSIZE, MARKERSIZE);

	//テクスチャ割り当て
	BindTexture(pTexture->Regist(TEXTURE));

	m_bAlpha = alpha;

	//成功を返す
	return S_OK;
}

//===========================================
//終了処理
//===========================================
void CLockonMarker::Uninit(void)
{
	//終了
	CBillboard::Uninit();
}

//===========================================
//更新処理
//===========================================
void CLockonMarker::Update(void)
{
	//更新
	CBillboard::Update();
}

//===========================================
//描画処理
//===========================================
void CLockonMarker::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_bAlpha == true)
	{// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//描画
	CBillboard::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
