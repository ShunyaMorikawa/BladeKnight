//========================================
//
//チュートリアル[tutorial.h]
//Author：森川駿弥
//
//========================================
#include "tutorial.h"
#include "manager.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "fade.h"
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

	// プレイヤー生成
	m_pPlayer = CPlayer::Create("data//FILE//player.txt");

	// フィールド生成
	m_pField = CField::Create();

	m_pObj2D = CObject2D::Create();

	// 位置設定
	m_pObj2D->SetPos(D3DXVECTOR3(1000.0f, 200.0f, 0.0f));

	// サイズ設定
	m_pObj2D->SetSize(300.0f, 350.0f);

	// テクスチャ設定
	m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\guide.png"));


	//成功を返す
	return S_OK;
}

//=======================================
//終了
//=======================================
void CTutorial::Uninit(void)
{
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
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
	}
}

//=======================================
//描画
//=======================================
void CTutorial::Draw(void)
{
}
