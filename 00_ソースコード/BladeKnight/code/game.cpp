//========================================
//
//ゲーム内処理[game.cpp]
//Author：森川駿弥
//
//========================================
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "enemy.h"
#include "fade.h"

//========================================
//静的メンバ変数
//========================================
CGame *CGame::m_pGame = nullptr;			// ゲームのポインタ

//========================================
//コンストラクタ
//========================================
CGame::CGame() : 
	m_bPause(false),
	m_nTransition(0)
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
	if (m_pGame == nullptr)
	{
		//インスタンス生成
		m_pGame = new CGame;
	}

	//ポインタを返す
	return m_pGame;
}

//========================================
//初期化
//========================================
HRESULT CGame::Init(void)
{
	// プレイヤー生成
	m_pPlayer = CPlayer::Create("data//FILE//player.txt");

	// フィールド生成
	m_pField = CField::Create();

	// エネミー生成
	m_pEnemy = CEnemy::Create("data//FILE//boss.txt");

	// 遷移時間
	m_nTransition = 0;

	//ポーズの状態
	m_bPause = false;

	return S_OK;
}

//========================================
//終了
//========================================
void CGame::Uninit(void)
{
	m_pGame = nullptr;
}

//========================================
//更新
//========================================
void CGame::Update(void)
{
	// CInputKeyboard型のポインタ
	CInputKeyboard* pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// 敵の体力取得
	int nLife = m_pEnemy->GetLife();

	if (nLife < 0)
	{
		m_nTransition++;

		if (m_nTransition >= 120)
		{
			// 画面遷移(フェード)
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);

			m_nTransition = 0;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{// ゲーム画面に遷移
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
}

//========================================
//描画
//========================================
void CGame::Draw(void)
{
}

//========================================
// シングルトン
//========================================
CGame *CGame::GetInstance(void)
{
	if (m_pGame == nullptr)
	{//インスタンス生成

		return m_pGame = new CGame;
	}
	else
	{//ポインタを返す
		return m_pGame;
	}
}
