//========================================
//
//リザルト[result.cpp]
//Author：森川駿弥
//
//========================================
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "result.h"
#include "object2D.h"
#include "input.h"
#include "fade.h"

//=======================================
//マクロ定義
//=======================================
#define TITLE_TEX	"data\\TEXTURE\\result.png"

//=======================================
//コンストラクタ
//=======================================
CResult::CResult() : 
	m_nCnt(0),
	m_Counter(0),
	m_pObj2D(nullptr)
{
}

//=======================================
//デストラクタ
//=======================================
CResult::~CResult()
{
}

//=======================================
//生成
//=======================================
CResult* CResult::Create(void)
{
	//タイトルのポインタ
	CResult* pResult = nullptr;

	//インスタンス生成
	pResult = new CResult;

	//初期化
	pResult->Init();

	//ポインタを返す
	return pResult;
}

//=======================================
//初期化
//=======================================
HRESULT CResult::Init(void)
{
	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (m_pObj2D == nullptr)
	{
		//CObject2Dのポインタ
		m_pObj2D = CObject2D::Create();

		//位置取得
		D3DXVECTOR3 pos = m_pObj2D->GetPos();

		//頂点情報
		m_pObj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

		//ポリゴンの位置
		pos = (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

		//位置設定
		m_pObj2D->SetPos(pos);

		//テクスチャ割り当て
		m_pObj2D->BindTexture(pTexture->Regist("data\\TEXTURE\\result.png"));
	}

	m_Counter = 0;

	//成功を返す
	return S_OK;
}

//=======================================
//終了
//=======================================
void CResult::Uninit(void)
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
void CResult::Update(void)
{
	//CInputKeyboard型のポインタ
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad型のポインタ
	CInputPad *pInputPad = nullptr;
	pInputPad = CManager::GetInstance()->GetInputPad();			//コントローラーの情報取得

	m_Counter++;

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true 
		|| pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true
		|| m_Counter >= 180)
	{//ゲーム画面に遷移
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
}

//=======================================
//描画
//=======================================
void CResult::Draw(void)
{
}
