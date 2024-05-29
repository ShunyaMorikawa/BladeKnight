//========================================
//
//チュートリアル[tutorial.h]
//Author：森川駿弥
//
//========================================
#include "tutorial.h"
#include "manager.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"

//========================================
//静的メンバ変数
//========================================

//=======================================
//コンストラクタ
//=======================================
CTutorial::CTutorial()
{
}

//=======================================
//デストラクタ
//=======================================
CTutorial::~CTutorial()
{
}

//=======================================
//生成
//=======================================
CTutorial* CTutorial::Create(void)
{
	//タイトルのポインタ
	CTutorial* pTutorial = nullptr;

	//インスタンス生成
	pTutorial = new CTutorial;

	//初期化
	pTutorial->Init();

	//ポインタを返す
	return pTutorial;
}

//=======================================
//初期化
//=======================================
HRESULT CTutorial::Init(void)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	if (m_pObj2D == nullptr)
	{
		//CObject2Dのポインタ
		m_pObj2D = CObject2D::Create();

		//位置取得
		D3DXVECTOR3 pos = m_pObj2D->GetPos();

		//頂点情報
		m_pObj2D->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		//ポリゴンの位置
		pos = (D3DXVECTOR3(640.0f, 360.0f, 0.0f));

		//位置設定
		m_pObj2D->SetPos(pos);

		//テクスチャ割り当て
		m_pObj2D->BindTexture(pTexture->Regist("data\\TEXTURE\\tutorial.png"));
	}

	//成功を返す
	return S_OK;
}

//=======================================
//終了
//=======================================
void CTutorial::Uninit(void)
{
	if (m_pObj2D != nullptr)
	{
		m_pObj2D->Uninit();
		m_pObj2D = nullptr;
	}
}

//=======================================
//更新
//=======================================
void CTutorial::Update(void)
{
	//CInputKeyboard情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad情報取得
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true)
	{
		// 画面遷移(フェード)
		CManager::GetInstance()->SetMode(CScene::MODE_GAME);
	}
}

//=======================================
//描画
//=======================================
void CTutorial::Draw(void)
{
}
