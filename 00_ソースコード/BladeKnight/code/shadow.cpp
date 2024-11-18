//========================================
//
// シャドウ処理[shadow.h]
// Author：森川駿弥
//
//========================================

#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"


//========================================
// コンストラクタ
//========================================
CShadow::CShadow(int nPriority)
{
}

//========================================
// デストラクタ
//========================================
CShadow::~CShadow()
{
}

//========================================
// 生成
//========================================
CShadow* CShadow::Create()
{
	// インスタンス生成
	CShadow* pShadow = new CShadow;

	// 初期化
	pShadow->Init();

	return pShadow;
}

//========================================
// 初期化
//========================================
HRESULT CShadow::Init(void)
{
	// 継承クラスの初期化
	CObject3D::Init();

	return S_OK;
}

//========================================
// 終了
//========================================
void CShadow::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//========================================
// 更新
//========================================
void CShadow::Update(void)
{
	// 継承クラスの更新
	CObject3D::Update();
}

//========================================
// 描画
//========================================
void CShadow::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 継承クラスの描画
	CObject3D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
