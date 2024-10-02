//========================================
//
// フィールド[field.cpp]
// Author：森川駿弥
//
//========================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
//静的メンバ変数
//========================================
LPDIRECT3DTEXTURE9 CField::m_pTexture = nullptr;

//========================================
//コンストラクタ
//========================================
CField::CField(int nPriority) : CObject3D(nPriority)
{
}

//========================================
//デストラクタ
//========================================
CField::~CField()
{
}

//========================================
//ポリゴン生成
//========================================
CField* CField::Create(void)
{
	//CPlayer型のポインタ
	CField* pField = new CField;;

	//初期化
	pField->Init();

	//ポインタを返す
	return pField;
}

//========================================
//初期化
//========================================
HRESULT CField::Init(void)
{
	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// 継承の初期化
	CObject3D::Init();

	//テクスチャ割り当て
	BindTexture(pTexture->Regist("data\\texture\\soil.jpg"));


	return S_OK;
}

//========================================
//終了
//========================================
void CField::Uninit(void)
{
	// 継承の初期化
	CObject3D::Uninit();
}

//========================================
//更新
//========================================
void CField::Update(void)
{
	// 継承の初期化
	CObject3D::Update();
}

//========================================
//描画
//========================================
void CField::Draw(void)
{
	//描画
	CObject3D::Draw();
}
