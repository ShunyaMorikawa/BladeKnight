//========================================
//
//ゲージ[gauge.h]
//Author：森川駿弥
//
//========================================
#include "gauge.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//========================================
// 名前空間
//========================================
namespace
{
	float fMax = 640.0f;
}

//========================================
// コンストラクタ
//========================================
CGauge::CGauge(int nPriority) : CObject2D(nPriority)
{
	m_nMaxLife = 0;
}

//========================================
// デストラクタ
//========================================
CGauge::~CGauge()
{
}

//========================================
// 生成
//========================================
CGauge* CGauge::Create(int nMaxLife)
{
	CGauge* pGauge = new CGauge;

	// 体力の最大値
	pGauge->m_nMaxLife = nMaxLife;

	pGauge->Init();

	return pGauge;
}

//========================================
// 初期化
//========================================
HRESULT CGauge::Init()
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	D3DXVECTOR3 pos = GetPos();

	pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	// 体力の初期値
	m_nLife = m_nMaxLife;

	// 継承クラスの初期化
	CObject2D::Init();

	// 頂点情報の設定
	SetVertex();

	// 位置設定
	CObject2D::SetPos(pos);

	// テクスチャ設定
	//BindTexture(pTexture->Regist("data\\texture\\tutorial.png"));

	return S_OK;
}

//========================================
// 終了
//========================================
void CGauge::Uninit()
{
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CGauge::Update()
{
	CObject2D::Update();
}

//========================================
// 描画
//========================================
void CGauge::Draw()
{
	CObject2D::Draw();
}

//========================================
// 頂点情報の設定
//========================================
void CGauge::SetVertex(void)
{
	CObject2D::SetVertex();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR col = GetCol();

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 Vertex = GetVtxBuff();

	//頂点情報へのポインタ
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	Vertex->Lock(0, 0, (void**)&pVtx, 0);

	float ratio = (float)m_nLife / (float)m_nMaxLife;

	pVtx[0].pos = pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fMax * ratio, 0.0f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fMax * ratio, 100.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	Vertex->Unlock();

	SetPos(pos);
	SetRot(rot);
}
