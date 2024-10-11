//========================================
//
// タイトルプレイヤー処理[titleplayer.h]
// Author：森川駿弥
//
//========================================

#include "titleplayer.h"
#include "debugproc.h"

//========================================
// 定数定義
//========================================
namespace
{
	const D3DXVECTOR3 INITIAL_POS = { 0.0f, 0.0f, 50.0f };	// プレイヤー初期位置
	const D3DXVECTOR3 INITIAL_ROT = { 0.0f, 0.0f, 0.0f };	// プレイヤー初期向き
}

//========================================
// 静的メンバ変数
//========================================
CTitlePlayer* CTitlePlayer::m_pTitlePlayer = nullptr;

//========================================
// コンストラクタ
//========================================
CTitlePlayer::CTitlePlayer(int nPriority) : CCharacter(nPriority),
m_apNumModel	(0),	// モデルの総数
m_nOldMotion	(0)		// 前回のモーション
{//値をクリア
	m_pTitlePlayer = nullptr;
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデル情報
}

//========================================
// デストラクタ
//========================================
CTitlePlayer::~CTitlePlayer()
{
}

//=======================================
//シングルトン
//=======================================
CTitlePlayer* CTitlePlayer::GetInstance()
{
	if (m_pTitlePlayer == nullptr)
	{//インスタンス生成
		return m_pTitlePlayer = new CTitlePlayer;
	}
	else
	{//ポインタを返す
		return m_pTitlePlayer;
	}
}

//========================================
// タイトルプレイヤー生成
//========================================
CTitlePlayer* CTitlePlayer::Create(std::string pfile)
{
	if (m_pTitlePlayer == nullptr)
	{
		//プレイヤー生成
		m_pTitlePlayer = new CTitlePlayer;

		//初期化
		m_pTitlePlayer->Init(pfile);
	}

	//ポインタを返す
	return m_pTitlePlayer;
}

//========================================
// 初期化
//========================================
HRESULT CTitlePlayer::Init(std::string pfile)
{
	// キャラの初期化
	CCharacter::Init(pfile);

	// 位置設定
	SetPos(INITIAL_POS);

	// 向き設定
	SetRot(INITIAL_ROT);

	return S_OK;
}

//========================================
//終了
//========================================
void CTitlePlayer::Uninit(void)
{
	// 終了
	CCharacter::Uninit();
	m_pTitlePlayer = nullptr;
}

//========================================
//更新
//========================================
void CTitlePlayer::Update(void)
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
//描画
//========================================
void CTitlePlayer::Draw(void)
{
	// 描画
	CCharacter::Draw();
}

//========================================
// モーション管理
//========================================
void CTitlePlayer::Motion()
{
	// モーション情報取得
	CMotion* pMotion = GetMotion();

	// タイトルモーション
	pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);

	if (pMotion != nullptr)
	{// モーション更新
		pMotion->Update();
	}
}
