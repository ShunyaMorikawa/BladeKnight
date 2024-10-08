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

//========================================
// 定数定義
//========================================
namespace
{
	const D3DXVECTOR3 INITIAL_POS = { 0.0f, 0.0f, 50.0f };	// プレイヤー初期位置
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
m_eState		(STATE_NONE)	// 状態
{//値をクリア
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデル情報
}

//========================================
// デストラクタ
//========================================
CResultPlayer::~CResultPlayer()
{
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
	// キャラの初期化
	CCharacter::Init(pfile);

	// プレイヤー状態の初期化
	m_eState = STATE_NONE;

	// 位置設定
	SetPos(INITIAL_POS);

	// 向き設定
	SetRot(INITIAL_ROT);

	return S_OK;
}

//========================================
// 終了
//========================================
void CResultPlayer::Uninit(void)
{
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
	// モーション情報取得
	CMotion* pMotion = GetMotion();

	// 向き
	D3DXVECTOR3 rot = {0.0f, 0.0f, 0.0f};

	// 状態
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	// プレイヤーの状態取得
	CPlayer::PLAYERSTATE state = pPlayer->GetState();

	if (pPlayer != nullptr)
	{
		// 状態取得
		pPlayer->SetState(state);

		if (state == STATE_WIN)
		{// 通常状態なら
			pMotion->Set(CMotion::RESULT_MOTIONTYPE_WIN);
		}
		else if (state == CPlayer::STATE_DEATH)
		{// 死亡状態なら
			pMotion->Set(CMotion::RESULT_MOTIONTYPE_NEUTRAL);
		}
	}

	if (pMotion != nullptr)
	{// モーション更新
		pMotion->Update();
	}
}
