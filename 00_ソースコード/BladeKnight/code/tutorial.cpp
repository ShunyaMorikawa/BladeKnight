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
#include "wall.h"
#include "mapobject.h"
#include "sound.h"
#include "useful.h"

//========================================
// 定数定義
//========================================
namespace
{
	const int TRANSITIONTIME = 180;		// 遷移するまでの時間

	const float GUIDE_WIDTH = 300.0f;	// ガイドの幅
	const float GUIDE_HEIGHT = 350.0f;	// ガイドの高さ

	const D3DXVECTOR3 GUIDE_POS = { 1100.0f, 300.0f, 0.0f };	// ガイドの位置
}

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
	m_pPlayer = CPlayer::Create("data\\FILE\\player.txt");

	// フィールド生成
	m_pField = CField::Create();

	// 壁生成
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, -Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

	// マップオブジェクト生成
	m_pMobj = CMapObject::Create();

	// インスタンス生成
	m_pObj2D = CObject2D::Create();

	// 位置設定
	m_pObj2D->SetPos(GUIDE_POS);

	// サイズ設定
	m_pObj2D->SetSize(GUIDE_WIDTH, GUIDE_HEIGHT);

	// テクスチャ設定
	m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\guide_tutorial.png"));

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド停止
	pSound->Stop(CSound::SOUND_LABEL_BGM_TITLE);

	// サウンド再生
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_TUTORIAL);

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

	if (pInputKeyboard->GetTrigger(DIK_RETURN)||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0))
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
