//========================================
//
// リザルト[result.h]
// Author：森川駿弥
//
//========================================

#include "result.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "field.h"
#include "mapobject.h"
#include "wall.h"
#include "useful.h"
#include "resultplayer.h"

//========================================
// 定数定義
//========================================
namespace
{
	const std::string RESULT_PLAYER_TXT = "data\\FILE\\resultplayer.txt";	// プレイヤー情報のパス
	const char* WIN_TEX = "data\\TEXTURE\\title.png";			// 勝利テクスチャのパス
	const char* LOSE_TEX = "data\\TEXTURE\\press_enter.png";	// 敗北テクスチャのパス

	const float LOGO_POS_Y = 150.0f;		// ロゴテクスチャのY座標
	const float LOGO_SIZE_WIDTH = 640.0f;	// ロゴテクスチャのサイズ
	const float LOGO_SIZE_HEIGHT = 250.0f;	// ロゴテクスチャのサイズ
	const float ENTER_POS_Y = 550.0f;		// 文字テクスチャのY座標
	const float ENTER_SIZE = 250.0f;		// 文字テクスチャのサイズ
}

//=======================================
//コンストラクタ
//=======================================
CResult::CResult() : 
m_pWin		(nullptr),	// 勝利テクスチャ
m_pLose		(nullptr)	// 敗北テクスチャ
{
}

//=======================================
//デストラクタ
//=======================================
CResult::~CResult()
{
}

//=======================================
//生成
//=======================================
CResult* CResult::Create(void)
{
	//タイトルのポインタ
	CResult* pResult = nullptr;

	//インスタンス生成
	pResult = new CResult;

	//初期化
	pResult->Init();

	//ポインタを返す
	return pResult;
}

//=======================================
//初期化
//=======================================
HRESULT CResult::Init(void)
{
	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// カメラの初期化
	pCamera->Init();

	// リザルトプレイヤー生成
	CResultPlayer::Create(RESULT_PLAYER_TXT);

	// マップオブジェクト生成
	CMapObject::Create();

	// フィールド生成
	CField::Create();

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
void CResult::Uninit(void)
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド停止
	pSound->Stop();
}

//=======================================
//更新
//=======================================
void CResult::Update(void)
{
	//CInputKeyboard型のポインタ
	CInputKeyboard* pInputKeyboard = pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad型のポインタ
	CInputPad* pInputPad = pInputPad = CManager::GetInstance()->GetInputPad();

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// リザルトカメラ
	pCamera->Result();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{
		// 画面遷移(フェード)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
}

//=======================================
//描画
//=======================================
void CResult::Draw(void)
{
}
