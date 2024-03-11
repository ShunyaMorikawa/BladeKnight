//========================================
//
//チュートリアル[tutorial.h]
//Author：森川駿弥
//
//========================================
#include "tutorial.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "light.h"
#include "player.h"
#include "polygon.h"
#include "scarecrow.h"

//========================================
//静的メンバ変数
//========================================
CObject *CTutorial::m_pObject[] = {};			// オブジェクトのポインタ
CObjectX *CTutorial::m_pObjectX = nullptr;		// オブジェクトXのポインタ
CObject3D *CTutorial::m_pObject3D = nullptr;	// オブジェクト3Dのポインタ
CLight *CTutorial::m_pLight = nullptr;			// ライトのポインタ
CPlayer *CTutorial::m_pPlayer = nullptr;		// プレイヤーのポインタ
CPolygon *CTutorial::m_pPolygon = nullptr;		// ポリゴンのポインタ
CScarecrow *CTutorial::m_pScarecrow = nullptr;	// チュートリアルエネミーのポインタ

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
CTutorial *CTutorial::Create(void)
{
	//タイトルのポインタ
	CTutorial *pTutorial = nullptr;

	if (pTutorial == nullptr)
	{
		//インスタンス生成
		pTutorial = new CTutorial;

		//初期化
		pTutorial->Init();
	}

	//ポインタを返す
	return pTutorial;
}

//=======================================
//初期化
//=======================================
HRESULT CTutorial::Init(void)
{
	//ライト生成
	if (m_pLight == nullptr)
	{//m_pLightがnullptrの時
		m_pLight = new CLight;
	}

	//ライトの初期化処理
	if (FAILED(m_pLight->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	//プレイヤーの生成
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create();
	}

	//プレイヤーの初期化処理
	if (FAILED(m_pPlayer->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	// チュートリアルエネミー生成
	if (m_pScarecrow == nullptr)
	{
		m_pScarecrow = CScarecrow::Create();
	}

	//チュートリアルエネミーの初期化処理
	if (FAILED(m_pScarecrow->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	// ポリゴンの初期化
	if (m_pPolygon == nullptr)
	{
		m_pPolygon = CPolygon::Create();
	}

	//ポリゴン初期化処理
	if (FAILED(m_pPolygon->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	//成功を返す
	return S_OK;
}

//=======================================
//終了
//=======================================
void CTutorial::Uninit(void)
{
	if (m_pPlayer != nullptr)
	{//プレイヤー破棄
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	if (m_pPolygon != nullptr)
	{// ポリゴン破棄
		m_pPolygon->Uninit();
		m_pPolygon = nullptr;
	}

	if (m_pScarecrow != nullptr)
	{// エネミー破棄
		m_pScarecrow->Uninit();
		m_pScarecrow = nullptr;
	}
}

//=======================================
//更新
//=======================================
void CTutorial::Update(void)
{
	// CInputKeyboard型のポインタ
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	//キーボードの情報取得;

	// コントローラーの情報取得	
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{
		CManager::GetInstance()->SetMode(CScene::MODE_GAME);
	}
}

//=======================================
//描画
//=======================================
void CTutorial::Draw(void)
{
}
