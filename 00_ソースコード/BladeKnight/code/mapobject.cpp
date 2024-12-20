//========================================
//
// マップオブジェクト[mapobject.cpp]
// Author：森川駿弥
//
//========================================

#include "mapobject.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// 定数定義
//========================================
namespace
{
	const char* OBJECT_PASS = "data\\MODEL\\stage\\tougijo_001.x";	// マップオブジェクトのパス
}

//========================================
// コンストラクタ
//========================================
CMapObject::CMapObject() :
m_pModel	(nullptr)	// モデルのポインタ
{
}

//========================================
// デストラクタ
//========================================
CMapObject::~CMapObject()
{
}

//========================================
// マップオブジェクト生成
//========================================
CMapObject* CMapObject::Create()
{
	// CMapObject型のポインタ
	CMapObject* pMapObject = nullptr;

	if (pMapObject == nullptr)
	{// インスタンス生成
		pMapObject = new CMapObject;

		pMapObject->Init();
	}

	//ポインタを返す
	return pMapObject;
}

//========================================
// 初期化
//========================================
HRESULT CMapObject::Init(void)
{
	if (m_pModel == nullptr)
	{// マップオブジェクトの生成
		m_pModel = CModel::Create(OBJECT_PASS);

		// 階層があるかどうか
		m_pModel->SetType(CModel::TYPE_NOT_HIERARCHY);
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CMapObject::Uninit(void)
{
	if (m_pModel != nullptr)
	{// モデルの削除
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// 自身の終了
	Release();
}

//========================================
// 更新
//========================================
void CMapObject::Update(void)
{
}

//========================================
// 描画
//========================================
void CMapObject::Draw(void)
{
	if (m_pModel != nullptr)
	{// モデルの描画
		m_pModel->Draw();
	}
}
