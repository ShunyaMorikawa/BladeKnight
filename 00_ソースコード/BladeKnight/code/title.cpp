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
CTitle::CTitle(int nPriority)
{
	m_pObj2D = nullptr;
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

	pCamera->Init();

	if (m_pObj2D == nullptr)
	{
		////CObject2Dのポインタ
		//m_pObj2D = CObject2D::Create();

		////位置取得
		//D3DXVECTOR3 pos = m_pObj2D->GetPos();

		////頂点情報
		//m_pObj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

		////ポリゴンの位置
		//pos = (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

		////位置設定
		//m_pObj2D->SetPos(pos);

		////テクスチャ割り当て
		//m_pObj2D->BindTexture(pTexture->Regist(TITLE_TEX));
	}

	// マップオブジェクト生成
	CMapObject::Create();

	// フィールド生成
	CField::Create();
	
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// 4方向に壁生成
	CWall::Create(D3DXVECTOR3(0.0f, 2000.0f, -4000.0f), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, 2000.0f, 4000.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(4000.0f, 2000.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-4000.0f, 2000.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

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
