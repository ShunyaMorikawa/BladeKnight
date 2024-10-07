//========================================
//
//ゲーム内処理[game.cpp]
//Author：森川駿弥
//
//========================================

#include "game.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "enemy.h"
#include "fade.h"
#include "wall.h"
#include "mapobject.h"
#include "sound.h"
#include "debugproc.h"
#include "texture.h"
#include "useful.h"

//========================================
//静的メンバ変数
//========================================
CGame *CGame::m_pGame = nullptr;			// ゲームのポインタ

//========================================
// 定数定義
//========================================
namespace
{
	const int TRANSITION_TIME = 180;		// 遷移するまでの時間
	
	const char* GUIDE_TEX = "data\\texture\\guide_game.png";	// テクスチャのパス
	const std::string PLAYER_TXT = "data//FILE//player.txt";	// プレイヤー情報のパス

	const float GUIDE_WIDTH = 300.0f;	// ガイドの幅
	const float GUIDE_HEIGHT = 350.0f;	// ガイドの高さ
	
	const D3DXVECTOR3 GUIDE_POS = { 1100.0f, 300.0f, 0.0f };	// ガイドの位置
}

//========================================
//コンストラクタ
//========================================
CGame::CGame() : 
m_bPause		(false),	// ポーズ
m_nTransition	(0),		// 遷移時間
m_pObjectX		(nullptr),	// オブジェクトXのポインタ
m_pIdxMesh		(nullptr),	// インデックスメッシュのポインタ
m_pFade			(nullptr),	// フェードのポインタ
m_pObj2D		(nullptr)	// オブジェクト2Dのポインタ
{
	m_pGame = nullptr;		// ゲームのポインタ
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
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// プレイヤー生成
	CPlayer::Create(PLAYER_TXT);

	// エネミー生成
	CEnemy::Create(Constance::ENEMY_TXT);

	// フィールド生成
	CField::Create();

	// 4方向に壁生成
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, -Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

	// マップオブジェクト生成
	CMapObject::Create();

	// 遷移時間
	m_nTransition = 0;

	//ポーズの状態
	m_bPause = false;

	// インスタンス生成
	m_pObj2D = CObject2D::Create();

	// 位置設定
	m_pObj2D->SetPos(GUIDE_POS);

	// サイズ設定
	m_pObj2D->SetSize(GUIDE_WIDTH, GUIDE_HEIGHT);

	// テクスチャ設定
	m_pObj2D->BindTexture(pTexture->Regist(GUIDE_TEX));

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド再生
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//========================================
//終了
//========================================
void CGame::Uninit(void)
{
	if(m_pObj2D != nullptr)
	{
		m_pObj2D->Uninit();
		m_pObj2D = nullptr;
	}

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド停止
	pSound->Stop(CSound::SOUND_LABEL_BGM_TUTORIAL);

	m_pGame = nullptr;
}

//========================================
//更新
//========================================
void CGame::Update(void)
{
	// CInputKeyboard型のポインタ
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// プレイヤーの情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	// 敵の情報取得
	CEnemy* pEnemy = CEnemy::GetInstance();

	if (pEnemy == nullptr || pPlayer == nullptr)
	{// 敵かプレイヤーの体力が0以下になったら
		m_nTransition++;

		if (m_nTransition > TRANSITION_TIME ||
			pInputKeyboard->GetTrigger(DIK_RETURN))
		{
			// 画面遷移(フェード)
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);

			m_nTransition = 0;
		}
	}

	// デバッグ表示の情報取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// デバッグ表示
	pDebugProc->Print("\nカウンター：%d\n", m_nTransition);

#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{// ゲーム画面に遷移
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
#endif
}

//========================================
//描画
//========================================
void CGame::Draw(void)
{
}
