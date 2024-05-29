//=======================================
//
//エフェクト処理[effect.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "effect.h"      //作成したeffect.hをインクルードする
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===========================================
//コンストラクタ
//===========================================
CEffect::CEffect(int nPriority) : CBillboard(nPriority)
{
	m_aEffect.nLife = 0;		// 寿命(表示時間)
	m_aEffect.nFirstLife = 0;	// 初期寿命
	m_aEffect.balpha = false;	// 加算合成
}

//===========================================
//デストラクタ
//===========================================
CEffect::~CEffect()
{
}

//===========================================
//生成
//===========================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha)
{
	//CEffect型のポインタ
	CEffect *pEffect = nullptr;

	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (pEffect == nullptr)
	{//nullptrの時
		//インスタンス生成
		pEffect = new CEffect;

		pEffect->SetPos(pos);
		pEffect->SetMove(move);
		pEffect->SetCol(col);
		pEffect->SetSize(fRadius, fRadius);
		pEffect->m_aEffect.nLife = nLife;
		pEffect->m_aEffect.balpha = balpha;

		//初期化
		pEffect->Init();

		//テクスチャ割り当て
		pEffect->BindTexture(pTexture->Regist("data\\texture\\effect000.png"));
	}

	//ポインタを返す
	return pEffect;
}

//===========================================
//初期化処理
//===========================================
HRESULT CEffect::Init(void)
{
	//初期化
	CBillboard::Init();

	// 初期寿命
	m_aEffect.nFirstLife = m_aEffect.nLife;

	//成功を返す
	return S_OK;
}

//===========================================
//終了処理
//===========================================
void CEffect::Uninit(void)
{
	//終了
	CBillboard::Uninit();
}

//===========================================
//更新処理
//===========================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXCOLOR col = GetCol();

	//更新
	CBillboard::Update();

	//α値(消える速度)
	col.a = 0.1f;

	// 色設定
	SetCol(col);

	// 位置更新
	pos += move;

	// 位置設定
	SetPos(pos);

	//体力減算
	m_aEffect.nLife--;

	if (m_aEffect.nLife < 0)
	{
		//破棄する
		Uninit();
	}
}

//===========================================
//描画処理
//===========================================
void CEffect::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_aEffect.balpha == true)
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
