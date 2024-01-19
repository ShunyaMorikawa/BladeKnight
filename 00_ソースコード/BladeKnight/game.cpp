//========================================
//
//シューティングアクション[game.cpp]
//Author：森川駿弥
//
//========================================
#include "game.h"
#include "object2D.h"
#include "objectx.h"
#include "time.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "light.h"
#include "player.h"
#include "polygon.h"
#include "gameobject.h"

//========================================
//マクロ定義
//========================================
#define SCENE_COUNTER	(3600)	//リザルトまでの遷移時間

//========================================
//静的メンバ変数
//========================================
CObject *CGame::m_pObject[] = {};			//オブジェクトのポインタ
CObjectX *CGame::m_pObjectX = nullptr;		//オブジェクトXのポインタ
CObject3D *CGame::m_pObject3D = nullptr;	//オブジェクト3Dのポインタ
CGame *CGame::m_pGame = nullptr;			//ゲームのポインタ
CLight *CGame::m_pLight = nullptr;			//ライトのポインタ
CPlayer *CGame::m_pPlayer = nullptr;		//プレイヤーのポインタ
CPolygon *CGame::m_pPolygon = nullptr;		//ポリゴンのポインタ

//========================================
//コンストラクタ
//========================================
CGame::CGame() : 
	m_bPause(false)
{
}

//========================================
//デストラクタ
//========================================
CGame::~CGame()
{
}

//========================================
//生成
//========================================
CGame *CGame::Create(void)
{
	//CGameのポインタ
	CGame *pGame = nullptr;

	if (pGame == nullptr)
	{
		//インスタンス生成
		pGame = new CGame;

		//初期化
		pGame->Init();
	}

	//ポインタを返す
	return pGame;
}

//========================================
//初期化
//========================================
HRESULT CGame::Init(void)
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
		m_pPlayer = new CPlayer;
	}

	//プレイヤーの初期化処理
	if (FAILED(m_pPlayer->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	// ポリゴンの初期化
	if (m_pPolygon == nullptr)
	{
		m_pPolygon = new CPolygon;
	}

	//プレイヤーの初期化処理
	if (FAILED(m_pPolygon->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	//ポーズの状態
	m_bPause = false;

	return S_OK;
}

//========================================
//終了
//========================================
void CGame::Uninit(void)
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
}

//========================================
//更新
//========================================
void CGame::Update(void)
{
	//CInputKeyboard型のポインタ
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInputKeyboard();	//キーボードの情報取得

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CManager::SetMode(CScene::MODE_RESULT);
	}
}

//========================================
//描画
//========================================
void CGame::Draw(void)
{
}
