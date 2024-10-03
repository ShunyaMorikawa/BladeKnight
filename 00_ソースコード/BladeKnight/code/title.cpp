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
#include "player.h"
#include "useful.h"

//========================================
// 定数定義
//========================================
namespace
{
	const char* TITLE_TEX = "data\\TEXTURE\\title.png";	// タイトルテクスチャのパス
}

//=======================================
//コンストラクタ
//=======================================
CTitle::CTitle() :
m_pObj2D	(nullptr)	// オブジェクト2Dのポインタ
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

	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// カメラの初期化
	pCamera->Init();

	if (m_pObj2D == nullptr)
	{
	}

	// プレイヤー生成
	CPlayer::Create("data//FILE//player.txt");

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

	// サウンド停止
	pSound->Stop();

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
