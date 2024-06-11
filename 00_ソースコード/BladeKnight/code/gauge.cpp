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
	float LENGTH = 640.0f;		// ゲージの長さ
}

//========================================
// コンストラクタ
//========================================
CGauge::CGauge(int nPriority) : CObject2D(nPriority)
{
	m_nMaxLife = 0;			// 体力の最大値
	m_nLife = 0;			// 現在の体力
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_aTexU = 0.0f;			// テクスチャのU値
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

	// 体力の初期値
	m_nLife = m_nMaxLife;

	// テクスチャ座標の初期化(U値)
	m_aTexU = 0.0f;

	// 継承クラスの初期化
	CObject2D::Init();

	// 頂点情報の設定
	SetVertex();

	// 位置設定
	CObject2D::SetPos(pos);

	return S_OK;
}

//========================================
// 終了
//========================================
void CGauge::Uninit()
{
	// 継承クラスの終了
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CGauge::Update()
{
	// 継承クラスの更新
	CObject2D::Update();
}

//========================================
// 描画
//========================================
void CGauge::Draw()
{
	// 継承クラスの描画
	CObject2D::Draw();
}

//========================================
// サイズ設定
//========================================
void CGauge::SetSize(float fWidht, float fHeight)
{
	//対角線の長さ(横、縦)
	m_fLength = sqrtf(fWidht * fWidht + fHeight * fHeight) * 0.5f;

	//対角線の向き(横、縦)
	m_fAngle = atan2f(fWidht, fHeight);

	//頂点情報
	SetVertex();
}

//========================================
// 頂点情報の設定
//========================================
void CGauge::SetVertex()
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

	// 体力の比率
	float ratio = (float)m_nLife / (float)m_nMaxLife;

	pVtx[0].pos = pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(LENGTH * ratio, 0.0f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(0.0f, m_fLength, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(LENGTH * ratio, m_fLength, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//テクスチャ座標の更新(U値)
	m_aTexU += 0.002f;

	pVtx[0].tex = D3DXVECTOR2(m_aTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_aTexU + ratio, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_aTexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_aTexU + ratio, 1.0f);

	//頂点バッファをアンロック
	Vertex->Unlock();

	SetPos(pos);
	SetRot(rot);
}
