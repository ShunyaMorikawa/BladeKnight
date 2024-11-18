//========================================
//
// 管理[manager.cpp]
// Author：森川駿弥
//
//========================================

#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "result.h"

//========================================
//静的メンバ変数
//========================================
CManager *CManager::m_pManager = nullptr;

//========================================
//コンストラクタ
//========================================
CManager::CManager(void) : 
m_nCnt				(0),	//自動遷移のカウンター
m_state				(0),	//状態変更
m_NowScene			(CScene::MODE_NONE),	//現在のシーン
m_pRenderer			(nullptr),	// レンダラーのポインタ
m_pInputKeyboard	(nullptr),	// キーボード
m_pInputPad			(nullptr),	// パッド
m_pInputMouse		(nullptr),	// マウス
m_pScene			(nullptr),	// シーン
m_pDebugProc		(nullptr),	// デバッグ表示
m_pTexture			(nullptr),	// テクスチャ
m_pCamera			(nullptr),	// カメラ
m_pLight			(nullptr),	// ライト
m_pFade				(nullptr),	// フェード
m_pSound			(nullptr)	// サウンド
{//値クリア
}

//========================================
//デストラクタ
//========================================
CManager::~CManager(void)
{
}

//========================================
// マネージャーの情報取得
//========================================
CManager *CManager::GetInstance()
{
	if (m_pManager == nullptr)
	{//インスタンス生成
		return m_pManager = new CManager;
	}
	else
	{//ポインタを返す
		return m_pManager;
	}
}

//========================================
//初期化
//========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラーの生成
	if (m_pRenderer == nullptr)
	{//g_pRendererがnullptrの時
		m_pRenderer = new CRenderer;

		//レンダラーの初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}

	//キーボードの生成
	if (m_pInputKeyboard == nullptr)
	{//m_pInputKeyboardがnullptrの時
		m_pInputKeyboard = new CInputKeyboard;

		//キーボードの初期化処理
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}

	//コントローラーの生成
	if (m_pInputPad == nullptr)
	{//m_pInputKeyboardがnullptrの時
		m_pInputPad = new CInputPad;

		//コントローラーの初期化処理
		if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}

	//カメラ生成
	if (m_pCamera == nullptr)
	{//m_pCameraがnullptrの時
		m_pCamera = new CCamera;

		//カメラの初期化処理
		if (FAILED(m_pCamera->Init()))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}

	//debugprocの生成
	if (m_pDebugProc == nullptr)
	{//m_pDebugProcがnullptrの時
		m_pDebugProc = new CDebugProc;

		//初期化
		m_pDebugProc->Init();
	}

	if (m_pLight == nullptr)
	{// m_pLightがnullptrの時
		m_pLight = new CLight;

		if (FAILED(m_pLight->Init()))
		{// 初期化失敗
			return E_FAIL;
		}
	}

	if (m_pSound == nullptr)
	{
		// インスタンス生成
		m_pSound = new CSound;

		if (FAILED(m_pSound->Init(hWnd)))
		{// 初期化失敗
			return E_FAIL;
		}

	}

	//テクスチャ生成
	m_pTexture = new CTexture;
	m_pTexture->Load();

	if (m_pFade == nullptr)
	{
		// フェード生成・設定
#ifdef _DEBUG
		m_pFade = CFade::Create(CScene::MODE_GAME);
#else
		m_pFade = CFade::Create(CScene::MODE_TITLE);
#endif
	}

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CManager::Uninit(void)
{
	if (m_pScene != nullptr)
	{//シーンの終了
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{//キーボードの破棄
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pFade != nullptr)
	{// フェード終了
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	//オブジェクトの破棄
	CObject::ReleaseAll();
	
	if (m_pRenderer != nullptr)
	{//レンダラーの終了
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pCamera != nullptr)
	{//カメラの破棄
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{//ライトの破棄
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pInputPad != nullptr)
	{//コントローラーの破棄
		m_pInputPad->Uninit();
		delete m_pInputPad;
		m_pInputPad = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{//debugprocの破棄
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pTexture != nullptr)
	{//テクスチャ破棄
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pSound != nullptr)
	{// サウンド終了
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
}

//========================================
//更新
//========================================
void CManager::Update(void)
{
	if (m_pRenderer != nullptr)
	{//g_pRendererがnullptrの時
		m_pRenderer->Update();
	}
	
	if (m_pInputKeyboard != nullptr)
	{//キーボードの更新
		m_pInputKeyboard->Update();
	}
	
	if (m_pInputPad != nullptr)
	{//コントローラーの更新
		m_pInputPad->Update();
	}
	
	if (m_pDebugProc != nullptr)
	{//DebugProの更新
		m_pDebugProc->Update();
	}

	if (m_pFade != nullptr)
	{// フェード更新
		m_pFade->Update();
	}
	
	if (m_pScene != nullptr)
	{//シーンの更新
		m_pScene->Update();
	}

	if (m_pCamera != nullptr)
	{// カメラ更新
		m_pCamera->Update();
	}
}

//========================================
//描画
//========================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{//g_pRendererがnullptrの時
		//描画処理
		m_pRenderer->Draw();
	}

	if (m_pScene != nullptr)
	{//シーンの更新
		m_pScene->Draw();
	}
}

//========================================
//モード設定
//========================================
void CManager::SetMode(CScene::MODE mode)
{	
	if (m_pScene != nullptr)
	{//モード破棄
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	//全て破棄
	CObject::ReleaseAll();

	//モード生成
	m_pScene = CScene::Create(mode);
}

//========================================
//コンストラクタ
//========================================
CScene::CScene() : 
	m_pVtxBuff(nullptr),	//頂点情報を格納
	m_nIdxTexture(0),		//テクスチャ番号
	m_mode(MODE_NONE)		//モード
{//値クリア
}

//========================================
//デストラクタ
//========================================
CScene::~CScene()
{
}

//========================================
//生成
//========================================
CScene* CScene::Create(int nMode)
{
	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();;

	//CSceneのポインタ
	CScene* pScene = nullptr;

	switch (nMode)
	{
	case MODE_TITLE:
		// タイトル生成
		pScene = CTitle::Create();
		break;

	case MODE_TUTORIAL:
		// チュートリアル生成
		pScene = CTutorial::Create();
		break;

	case MODE_GAME:
		// ゲーム生成
		pScene = CGame::Create();
		break;

	case MODE_RESULT:
		// リザルト生成
		pScene = CResult::Create();
		break;

	default:
		break;
	}

	if (pScene != nullptr)
	{
		// カメラの初期化
		pCamera->Init();
	}

	//ポインタを返す
	return pScene;
}
 