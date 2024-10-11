//========================================
//
// リザルトプレイヤー処理[resultplayer.h]
// Author：森川駿弥
//
//========================================

#include "resultplayer.h"
#include "debugproc.h"
#include "player.h"
#include "enemy.h"
#include "result.h"
#include "sound.h"
#include "texture.h"

//========================================
// 定数定義
//========================================
namespace
{
	const char* WIN_TEX = "data\\TEXTURE\\Result_Win.png";		// 勝利テクスチャのパス
	const char* LOSE_TEX = "data\\TEXTURE\\Result_Lose.png";	// 敗北テクスチャのパス

	const float LOGO_POS_Y = 150.0f;		// ロゴテクスチャのY座標
	const float LOGO_SIZE_WIDTH = 640.0f;	// ロゴテクスチャのサイズ
	const float LOGO_SIZE_HEIGHT = 250.0f;	// ロゴテクスチャのサイズ
	const float ENTER_POS_Y = 550.0f;		// 文字テクスチャのY座標
	const float ENTER_SIZE = 250.0f;		// 文字テクスチャのサイズ

	const D3DXVECTOR3 INITIAL_POS = { 0.0f, 0.0f, 50.0f };		// プレイヤー初期位置
	const D3DXVECTOR3 INITIAL_ROT = { 0.0f, D3DX_PI, 0.0f };	// プレイヤー初期向き
}

//========================================
// 静的メンバ変数
//========================================
CResultPlayer* CResultPlayer::m_pResultPlayer = nullptr;

//========================================
// コンストラクタ
//========================================
CResultPlayer::CResultPlayer(int nPriority) : CCharacter(nPriority),
m_apNumModel	(0),			// モデルの総数
m_nOldMotion	(0),			// 前回のモーション
m_eState		(STATE_NONE),	// 状態
m_pWin(nullptr),	// 勝利テクスチャ
m_pLose(nullptr)	// 敗北テクスチャ

{//値をクリア
	m_pResultPlayer = nullptr;
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデル情報
}

//========================================
// デストラクタ
//========================================
CResultPlayer::~CResultPlayer()
{
}

//=======================================
//シングルトン
//=======================================
CResultPlayer* CResultPlayer::GetInstance()
{
	if (m_pResultPlayer == nullptr)
	{//インスタンス生成
		return m_pResultPlayer = new CResultPlayer;
	}
	else
	{//ポインタを返す
		return m_pResultPlayer;
	}
}

//========================================
// タイトルプレイヤー生成
//========================================
CResultPlayer* CResultPlayer::Create(std::string pfile)
{
	if (m_pResultPlayer == nullptr)
	{
		//プレイヤー生成
		m_pResultPlayer = new CResultPlayer;

		//初期化
		m_pResultPlayer->Init(pfile);
	}

	//ポインタを返す
	return m_pResultPlayer;
}

//========================================
// 初期化
//========================================
HRESULT CResultPlayer::Init(std::string pfile)
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// キャラの初期化
	CCharacter::Init(pfile);

	// プレイヤー状態の初期化
	m_eState = STATE_NONE;

	// 位置設定
	SetPos(INITIAL_POS);

	// 向き設定
	SetRot(INITIAL_ROT);

	// リザルトフラグの取得
	bool frag = CResult::GetFrag();

	if (frag)
	{// 勝利なら
		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_WIN);
	}
	else
	{// 敗北なら
		// サウンド再生
		pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_LOSE);
	}

	// リザルトフラグ管理
	CResult::ChangeFrag(frag);

	return S_OK;
}

//========================================
// 終了
//========================================
void CResultPlayer::Uninit(void)
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド停止
	pSound->Stop();

	// 終了
	CCharacter::Uninit();
	m_pResultPlayer = nullptr;
}

//========================================
// 更新
//========================================
void CResultPlayer::Update(void)
{
	// 位置・移動量・向き取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRot();

	// モーション
	Motion();

	// デバッグ表示の情報取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// デバッグ表示
	pDebugProc->Print("\nプレイヤーの位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
}

//========================================
// 描画
//========================================
void CResultPlayer::Draw(void)
{
	// 描画
	CCharacter::Draw();
}

//========================================
// モーション管理
//========================================
void CResultPlayer::Motion()
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// モーション情報取得
	CMotion* pMotion = GetMotion();

	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 向き
	D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f };

	// リザルトフラグの取得
	bool frag = CResult::GetFrag();

	if (frag)
	{// 勝利なら
		pMotion->Set(CMotion::RESULT_MOTIONTYPE_WIN);

		// インスタンス生成
		if (m_pWin == nullptr)
		{
			m_pWin = CObject2D::Create();

			// 位置設定
			m_pWin->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, LOGO_POS_Y, 0.0f));

			// サイズ設定
			m_pWin->SetSize(LOGO_SIZE_WIDTH, LOGO_SIZE_HEIGHT);

			// テクスチャ設定
			m_pWin->BindTexture(pTexture->Regist(WIN_TEX));
		}

	}
	else
	{// 敗北なら
		// 敗北モーション
		pMotion->Set(CMotion::RESULT_MOTIONTYPE_LOSE);

		if (m_pLose == nullptr)
		{
			m_pLose = CObject2D::Create();

			// 位置設定
			m_pLose->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, LOGO_POS_Y, 0.0f));

			// サイズ設定
			m_pLose->SetSize(LOGO_SIZE_WIDTH, LOGO_SIZE_HEIGHT);

			// テクスチャ設定
			m_pLose->BindTexture(pTexture->Regist(LOSE_TEX));
		}

	}

	// リザルトフラグ管理
	CResult::ChangeFrag(frag);

	if (pMotion != nullptr)
	{// モーション更新
		pMotion->Update();
	}
}
