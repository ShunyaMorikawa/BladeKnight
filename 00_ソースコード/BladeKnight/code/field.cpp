//========================================
//
//シューティングアクション[polygon.cpp]
//Author：森川駿弥
//
//========================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
//マクロ定義
//========================================
#define POLYGON_TEX		"data\\texture\\soil.jpg"

//========================================
//静的メンバ変数
//========================================
LPDIRECT3DTEXTURE9 CField::m_pTexture = nullptr;

//========================================
//コンストラクタ
//========================================
CField::CField(int nPriority) :
	CObject3D(nPriority)
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
CField *CField::Create(void)
{
	//CPlayer型のポインタ
	CField *pPolygon = nullptr;

	if (pPolygon == nullptr)
	{
		//プレイヤー生成
		pPolygon = new CField;

		//初期化
		pPolygon->Init();
	}

	//ポインタを返す
	return pPolygon;
}

//========================================
//初期化
//========================================
HRESULT CField::Init(void)
{
	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (m_pObj3D == nullptr)
	{
		//CObject2Dのポインタ
		m_pObj3D = CObject3D::Create();

		//テクスチャ割り当て
		m_pObj3D->BindTexture(pTexture->Regist(POLYGON_TEX));
	}

	return S_OK;
}

//========================================
//終了
//========================================
void CField::Uninit(void)
{
}

//========================================
//更新
//========================================
void CField::Update(void)
{
}

//========================================
//描画
//========================================
void CField::Draw(void)
{
	//描画
	CObject3D::Draw();
}