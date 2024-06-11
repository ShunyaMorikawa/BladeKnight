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

//=======================================
//マクロ定義
//=======================================
#define TITLE_TEX	"data\\TEXTURE\\title.png"

//=======================================
//コンストラクタ
//=======================================
CTitle::CTitle(int nPriority)
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
	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (pObj2D == nullptr)
	{
		//CObject2Dのポインタ
		pObj2D = CObject2D::Create();

		//位置取得
		D3DXVECTOR3 pos = pObj2D->GetPos();

		//頂点情報
		pObj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

		//ポリゴンの位置
		pos = (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

		//位置設定
		pObj2D->SetPos(pos);

		//テクスチャ割り当て
		pObj2D->BindTexture(pTexture->Regist(TITLE_TEX));
	}

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
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad型のポインタ
	CInputPad *pInputPad = nullptr;
	pInputPad = CManager::GetInstance()->GetInputPad();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
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
