//=======================================
//
//フェード[fade.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "fade.h"

//=======================================
//コンストラクタ
//=======================================
CFade::CFade() :
	m_State(FADE_NONE),					// 何もしない状態
	m_modeNext(CScene::MODE_TITLE),		// タイトル
	m_color(0.0f, 0.0f, 0.0f, 0.0f),	// フェード時の色
	m_pVtxBuff(nullptr), 
	m_pObject2D(nullptr)
{//値クリア
}

//=======================================
//デストラクタ
//=======================================
CFade::~CFade()
{
}

//=======================================
//初期化
//=======================================
HRESULT CFade::Init()
{
	//2Dオブジェクトの生成
	m_pObject2D = CObject2D::Create();

	// 位置設定
	m_pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	// サイズ設定
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return S_OK;
}

//=======================================
//初期化
//=======================================
HRESULT CFade::Init(CScene::MODE modenext)
{
	//2Dオブジェクトの生成
	m_pObject2D = CObject2D::Create();

	// 位置設定
	m_pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	// サイズ設定
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	SetFade(modenext);

	return S_OK;
}

//=======================================
//終了
//=======================================
void CFade::Uninit()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}
}

//=======================================
//更新
//=======================================
void CFade::Update()
{
	if (m_State != FADE_NONE)
	{
		if (m_State == FADE_IN)
		{//フェード状態
			m_color.a -= 0.05f;
			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;

				//何もしてない状態
				m_State = FADE_NONE;
			}
		}
		else if (m_State == FADE_OUT)
		{
			m_color.a += 0.05f;
			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;

				//フェードイン
				m_State = FADE_IN;

				//モード設定
				CManager::GetInstance()->SetMode(m_modeNext);
			}
		}
	}

	if (m_pObject2D != nullptr)
	{
		m_pObject2D->SetVertexColor(m_color);
	}
}

//=======================================
//描画
//=======================================
void CFade::Draw()
{
}

//=======================================
//設定
//=======================================
void CFade::SetFade(CScene::MODE modenext)
{
	// フェードアウト
	m_State = FADE_OUT;

	// 次のモード設定
	m_modeNext = modenext;

	// 黒いポリゴン(不透明)
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=======================================
// 状態設定
//=======================================
void CFade::SetState(FADE state)
{
	m_State = state;

	switch (state)
	{
	case FADE_NONE:
		m_color.a = 0.0f;
		break;

	case FADE_IN:
		m_color.a = 1.0f;
		break;

	case FADE_OUT:
		m_color.a = 0.0f;
		break;

	default:
		break;
	}

	if (m_pObject2D != nullptr)
	{
		m_pObject2D->SetVertexColor(m_color);
	}
}

//=======================================
//生成
//=======================================
CFade *CFade::Create(CScene::MODE modenext)
{
	//CFade型のポインタ
	CFade *pFade = nullptr;

	if (pFade == nullptr)
	{
		//インスタンス生成
		pFade = new CFade;

		//初期化
		pFade->Init(modenext);
		pFade->SetState(FADE_IN);
	}

	//ポインタを返す
	return pFade;
}
