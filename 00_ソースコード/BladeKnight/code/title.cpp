//========================================
//
//タイトル[title.cpp]
//Author：森川駿弥
//
//========================================

#include "title.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "field.h"
#include "mapobject.h"
#include "wall.h"
#include "titleplayer.h"
#include "useful.h"

//========================================
// 定数定義
//========================================
namespace
{
	const std::string PLAYER_TXT = "data\\FILE\\titleplayer.txt";	// プレイヤー情報のパス
	const char* TITLE_TEX = "data\\TEXTURE\\title.png";			// タイトルテクスチャのパス
	const char* ENTER_TEX = "data\\TEXTURE\\press_enter.png";	// 文字テクスチャのパス

	const float LOGO_POS_Y = 150.0f;		// ロゴテクスチャのY座標
	const float LOGO_SIZE_WIDTH = 640.0f;	// ロゴテクスチャのサイズ
	const float LOGO_SIZE_HEIGHT = 250.0f;	// ロゴテクスチャのサイズ
	const float ENTER_POS_Y = 550.0f;		// 文字テクスチャのY座標
	const float ENTER_WIDTH = 500.0f;		// 文字テクスチャのサイズ
	const float ENTER_HEIGHT = 150.0f;		// 文字テクスチャのサイズ
}

//=======================================
//コンストラクタ
//=======================================
CTitle::CTitle() :
m_pTitleLogo	(nullptr),	// オブジェクト2Dのポインタ
m_pEnter	(nullptr)	// PressEnter文字用のポインタ
{
}

//=======================================
//デストラクタ
//=======================================
CTitle::~CTitle()
{
}

//=======================================
//生成
//=======================================
CTitle* CTitle::Create(void)
{
	//タイトルのポインタ
	CTitle* pTitle = nullptr;

	//インスタンス生成
	pTitle = new CTitle;

	//初期化
	pTitle->Init();

	//ポインタを返す
	return pTitle;
}

//=======================================
//初期化
//=======================================
HRESULT CTitle::Init(void)
{
	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラの初期化
	pCamera->Init();

	// タイトルロゴ
	TitleLogo();

	// PressEnter文字
	PressEnter();

	// タイトルプレイヤー読み込み
	CTitlePlayer::Create(PLAYER_TXT);

	// マップオブジェクト生成
	CMapObject::Create();

	// フィールド生成
	CField::Create();
	
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// 4方向に壁生成
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, -Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

	// サウンド再生
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);

	//成功を返す
	return S_OK;
}

//=======================================
//終了
//=======================================
void CTitle::Uninit(void)
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド停止
	pSound->Stop();
}

//=======================================
//更新
//=======================================
void CTitle::Update(void)
{
	//CInputKeyboard型のポインタ
	CInputKeyboard *pInputKeyboard = pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	
	//CInputPad型のポインタ
	CInputPad *pInputPad = pInputPad = CManager::GetInstance()->GetInputPad();
	
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// タイトルカメラ
	pCamera->TitleCamera();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || 
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		// 画面遷移(フェード)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TUTORIAL);
	}
}

//=======================================
//描画
//=======================================
void CTitle::Draw(void)
{
}

//=======================================
// タイトルロゴ
//=======================================
void CTitle::TitleLogo()
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// インスタンス生成
	m_pTitleLogo = CObject2D::Create();

	// 位置設定
	m_pTitleLogo->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, LOGO_POS_Y, 0.0f));

	// サイズ設定
	m_pTitleLogo->SetSize(LOGO_SIZE_WIDTH, LOGO_SIZE_HEIGHT);

	// テクスチャ設定
	m_pTitleLogo->BindTexture(pTexture->Regist(TITLE_TEX));
}

//=======================================
// PressEnter文字
//=======================================
void CTitle::PressEnter()
{
	// テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// インスタンス生成
	m_pEnter = CObject2D::Create();

	// 位置設定
	m_pEnter->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, ENTER_POS_Y, 0.0f));

	// サイズ設定
	m_pEnter->SetSize(ENTER_WIDTH, ENTER_HEIGHT);

	// テクスチャ設定
	m_pEnter->BindTexture(pTexture->Regist(ENTER_TEX));
}
