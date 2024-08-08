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

//========================================
//静的メンバ変数
//========================================
CGame *CGame::m_pGame = nullptr;			// ゲームのポインタ

//========================================
// 定数定義
//========================================
namespace
{
const int TRANSITIONTIME = 180;		// 遷移するまでの時間
}

//========================================
//コンストラクタ
//========================================
CGame::CGame() : 
	m_bPause(false),
	m_nTransition(0),
	m_pObjectX(nullptr),
	m_pIdxMesh(nullptr),
	m_pFade(nullptr),
	m_pObj2D(nullptr)
{
	m_pGame = nullptr;
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
	CPlayer::Create("data//FILE//player.txt");

	// フィールド生成
	CField::Create();

	// エネミー生成
	CEnemy::Create("data//FILE//motion.txt");

	// 4方向に壁生成
	CWall::Create(D3DXVECTOR3(0.0f, 2000.0f, -4000.0f), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, 2000.0f, 4000.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(4000.0f, 2000.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f,  -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-4000.0f, 2000.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f,  D3DX_PI * 0.5f, 0.0f));

	// マップオブジェクト生成
	CMapObject::Create();

	// 遷移時間
	m_nTransition = 0;

	//ポーズの状態
	m_bPause = false;

	// インスタンス生成
	m_pObj2D = CObject2D::Create();

	// 位置設定
	m_pObj2D->SetPos(D3DXVECTOR3(1100.0f, 300.0f, 0.0f));

	// サイズ設定
	m_pObj2D->SetSize(300.0f, 350.0f);

	// テクスチャ設定
	m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\guide_game.png"));

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド停止
	pSound->Stop(CSound::SOUND_LABEL_BGM_TUTORIAL);

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

		if (m_nTransition > TRANSITIONTIME ||
			pInputKeyboard->GetTrigger(DIK_RETURN))
		{
			// 画面遷移(フェード)
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);

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
