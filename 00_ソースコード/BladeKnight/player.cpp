//========================================
//
//プレイヤー[player.cpp]
//Author：森川駿弥
//
//========================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "game.h"
#include "tutorial.h"
#include "scarecrow.h"

//========================================
//マクロ定義
//========================================
#define PLAYER_LIFE		(10)		// 体力
#define PLAYER_SIZE		(50.0f)		// サイズ
#define PLAYER_SPEED	(4.0f)		// プレイヤーの移動速度
#define PLAYER_INERTIA	(0.3f)		// プレイヤーの慣性

#define PLAYER_PATH	"data\\FILE\\player.txt"	// 読み込むファイルのパス

//========================================
//コンストラクタ
//========================================
CPlayer::CPlayer() : 
	m_pos(0.0f, 0.0f, 0.0f),		// 位置
	m_move(0.0f, 0.0f, 0.0f),		// 移動量
	m_rot(0.0f, 0.0f, 0.0f),		// 向き
	m_pMesh(nullptr),				// メッシュ(頂点情報)へのポインタ
	m_pBuffMat(nullptr),			// マテリアルへのポインタ
	m_dwNumMat(0),					// マテリアルの数
	m_apNumModel(0), 				// モデル(パーツ)の総数
	m_RotDest(0.0f, 0.0f, 0.0f),	// 目的の向き
	m_nLife(0),						// 体力
	m_fSize(0.0f),					// サイズ
	m_bMove(false),
	m_bWait(false),
	m_pMotion(nullptr),
	m_pBoss(nullptr)
{//値をクリア
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデルのポインタ
}

//========================================
//デストラクタ
//========================================
CPlayer::~CPlayer()
{
}

//========================================
//プレイヤーの生成
//========================================
CPlayer *CPlayer::Create(void)
{
	//CPlayer型のポインタ
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//プレイヤー生成
		pPlayer = new CPlayer;

		//初期化
		pPlayer->Init();
	}

	//ポインタを返す
	return pPlayer;
}

//========================================
//初期化
//========================================
HRESULT CPlayer::Init(void)
{
	//モーションのポインタ
	m_pMotion = nullptr;

	//目的の向き
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//プレイヤーの初期位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, -300.0f);

	//プレイヤーの初期向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 体力
	m_nLife = PLAYER_LIFE;

	// サイズ
	m_fSize = PLAYER_SIZE;

	if (m_pMotion == nullptr)
	{
		//モーション生成
		m_pMotion = CMotion::Create();

		//モーション読み込み
		m_pMotion->Load(PLAYER_PATH);

		//待機モーション
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);
	}

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CPlayer::Uninit(void)
{
	if (m_pMotion != nullptr)
	{//モーション破棄
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//========================================
//更新
//========================================
void CPlayer::Update(void)
{
	// 歩き
	m_bMove = false;

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 向き取得
	D3DXVECTOR3 rot = GetRot();

	// カメラ追従
	CCamera *pCampera = CManager::GetInstance()->GetCamera();
	pCampera->following(pos, rot);

	// プレイヤー行動
	Act(PLAYER_SPEED);

	if (m_bMove)
	{// 歩きモーション
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_WALK);
	}
	else if (m_bCutdown)
	{// 切り下ろしモーション
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_CUTDOWN);

		if (m_pMotion->IsFinish() && m_bCutdown == true)
		{// モーション終了
			m_bCutdown = false;
		}
	}
	else if (m_bMowingdown)
	{// 薙ぎ払い
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_MOWINGDOWN);

		if (m_pMotion->IsFinish() && m_bMowingdown == true)
		{// モーション終了
			m_bMowingdown = false;
		}
	}
	else if (m_bStrongAttack)
	{// 強攻撃
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_STRONGATTACK);

		if (m_pMotion->IsFinish() && m_bStrongAttack == true)
		{// モーション終了
			m_bStrongAttack = false;
		}
	}
	else
	{// 待機モーション
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);
	}

	if (m_pMotion != nullptr)
	{// モーション更新
		m_pMotion->Update();
	}

	//チュートリアルエネミーとの当たり判定
	CollisionScarecrow();

	// ポインタ
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	// デバッグ表示
	pDebugProc->Print("\nプレイヤーの位置：%f、%f、%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("プレイヤーの向き：%f、%f、%f\n", rot.x, rot.y, rot.z);
	pDebugProc->Print("プレイヤーの体力：%d\n", m_nLife);
}

//========================================
//描画
//========================================
void CPlayer::Draw(void)
{
	//描画
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//位置取得
	D3DXVECTOR3 pos = GetPosition();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モーション描画
	m_pMotion->Draw();
}

//========================================
//プレイヤーの操作
//========================================
void CPlayer::Act(float fSpeed)
{
	// キーボードの情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	
	
	// コントローラーの情報取得	
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	//CCamera型のポインタ
	CCamera *pCamera = nullptr;
	pCamera = CManager::GetInstance()->GetCamera();

	//rotの取得
	D3DXVECTOR3 rot = pCamera->GetRot();

	//注視点の取得
	D3DXVECTOR3 PosR = pCamera->GetPosR();

	//目的の向き
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 終了したかの取得
	bool bFinish = m_pMotion->IsFinish();

	if (pInputKeyboard->GetPress(DIK_A) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//Aが押された
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//左上
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;

			//移動方向にモデルを向ける
			m_RotDest.y = -rot.y - (D3DX_PI * 0.75f);

			// 歩き
			m_bMove = true;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
				 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//左下
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;

			//移動方向にモデルを向ける
			m_RotDest.y = -rot.y + (D3DX_PI * 0.75f);

			// 歩き
			m_bMove = true;
		}
		else
		{//左
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;

			//移動方向にモデルを向ける
			m_RotDest.y = -rot.y + D3DX_PI;

			//注視点をずらす
			PosR - m_move;

			// 歩き
			m_bMove = true;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_D) == true
			 || pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//Dが押された
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0 ) > 0)
		{//右上
			m_move.x += cosf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;

			//移動方向にモデルを向ける
			m_RotDest.y = -rot.y + (-D3DX_PI * 0.25f);

			// 歩き
			m_bMove = true;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
				 || pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//右下
			m_move.x += cosf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;

			//移動方向にモデルを向ける
			m_RotDest.y = -rot.y + (D3DX_PI * 0.25f);

			// 歩き
			m_bMove = true;
		}
		else
		{//右
			m_move.x += cosf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;

			//移動方向にモデルを向ける
			m_RotDest.y = -rot.y;

			//注視点をずらす
			PosR + m_move;

			// 歩き
			m_bMove = true;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_W) == true
			 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0 ) > 0)
	{//Wが押された
		m_move.x -= cosf(rot.y) * fSpeed;
		m_move.z -= sinf(rot.y) * fSpeed;

		//移動方向にモデルを向ける
		m_RotDest.y = -rot.y - (D3DX_PI * 0.5f);

		// 歩き
		m_bMove = true;
	}

	else if (pInputKeyboard->GetPress(DIK_S) == true
			 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//Sが押された
		m_move.x += cosf(rot.y) * fSpeed;
		m_move.z += sinf(rot.y) * fSpeed;

		//移動方向にモデルを向ける
		m_RotDest.y = -rot.y + (D3DX_PI * 0.5f);

		// 歩き
		m_bMove = true;
	}

	//位置を更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	//移動量を更新(減衰させる)
	m_move.x += (0.0f - m_move.x) * PLAYER_INERTIA;
	m_move.z += (0.0f - m_move.z) * PLAYER_INERTIA;

	//目的の向き
	DiffRot.y = m_RotDest.y - m_rot.y;

	//向きの正規化
	if (DiffRot.y > D3DX_PI)
	{//3.14を超えたときに反対にする
		DiffRot.y -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (DiffRot.y < -D3DX_PI)
	{
		DiffRot.y += D3DX_PI * 2;
	}

	//Diffに補正係数をかける
	m_rot.y += DiffRot.y * 0.1f;

	//角度の正規化
	if (m_rot.y > D3DX_PI)
	{//3.14を超えたときに反対にする
		m_rot.y -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	// 攻撃
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_X, 0) == true)
	{// 切りおろし
		m_bCutdown = true;
	}

	if (pInputPad->GetTrigger(CInputPad::BUTTON_B, 0) == true)
	{// 薙ぎ払い
		m_bMowingdown = true;
	}

	if (pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// 強攻撃
		m_bStrongAttack = true;
	}
}

//=======================================
//rotの正規化
//=======================================
float CPlayer::RotNormalize(float RotN, float Rot)
{
	//角度の正規化
	if (RotN > D3DX_PI)
	{//3.14を超えたときに反対にする
		RotN -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (RotN < -D3DX_PI)
	{
		RotN += D3DX_PI * 2;
	}

	//Diffに補正係数をかける
	Rot += RotN * 0.1f;

	//角度の正規化
	if (Rot > D3DX_PI)
	{//3.14を超えたときに反対にする
		Rot -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (Rot < -D3DX_PI)
	{
		Rot += D3DX_PI * 2;
	}

	return RotN, Rot;
}

//========================================
// チュートリアル用エネミーとの当たり判定
//========================================
void CPlayer::CollisionScarecrow()
{
	//変数宣言
	float fLength;		//長さ

	// チュートリアル用エネミーの情報取得
	CScarecrow *pScarecrow = CTutorial::GetScarecrow();

	//チュートリアル用エネミーの位置取得
	D3DXVECTOR3 posScarecrow = pScarecrow->GetPosition();

	// チュートリアル用エネミーのサイズ取得
	float sizeScarecrow = pScarecrow->GetSize();

	//ベクトルを求める
	D3DXVECTOR3 vec = posScarecrow - this->GetPosition();

	//ベクトル代入
	fLength = D3DXVec3Length(&vec);

	if (fLength <= sizeScarecrow)
	{
		CManager::GetInstance()->SetMode(CScene::MODE_RESULT);
	}
}

//========================================
//3Dオブジェクトの生成
//========================================
void CPlayer::SetVertex(void)
{
}

//========================================
//3Dオブジェクトの生成
//========================================
void CPlayer::SetSize(float fWidht, float fHeight)
{
	fWidht = 0.0f;
	fHeight = 0.0f;
}
