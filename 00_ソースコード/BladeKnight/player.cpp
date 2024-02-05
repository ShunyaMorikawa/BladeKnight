//========================================
//
//シューティングアクション[player.cpp]
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

//========================================
//マクロ定義
//========================================
#define PLAYER_SPEED	(4.0f)		//プレイヤーの移動速度
#define PLAYER_INERTIA	(0.3f)		//プレイヤーの慣性

#define PLAYER_PATH	"data\\FILE\\player.txt"	//読み込むファイルのパス

//========================================
//コンストラクタ
//========================================
CPlayer::CPlayer() : 
	m_pos(0.0f, 0.0f, 0.0f),		//位置
	m_move(0.0f, 0.0f, 0.0f),		//移動量
	m_rot(0.0f, 0.0f, 0.0f),		//向き
	m_pMesh(nullptr),				//メッシュ(頂点情報)へのポインタ
	m_pBuffMat(nullptr),			//マテリアルへのポインタ
	m_dwNumMat(0),					//マテリアルの数
	m_apNumModel(0), 				//モデル(パーツ)の総数
	m_RotDest(0.0f, 0.0f, 0.0f),	//目的の向き
	m_nLife(0),						// 体力
	m_bMove(false),
	m_bWait(false)
{//値をクリア
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
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, -300.0f));

	//プレイヤーの初期向き
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 体力
	m_nLife = 10;

	if (m_pMotion == nullptr)
	{
		//モーション生成
		m_pMotion = CMotion::Create();

		//モーション読み込み
		m_pMotion->Load(PLAYER_PATH);

		//待機モーション
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
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

	// 攻撃
	m_bAttack = false;

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 向き取得
	D3DXVECTOR3 rot = GetRot();

	// カメラ追従
	CCamera *pCampera = CManager::GetInstance()->GetCamera();
	pCampera->following(pos, rot);

	// プレイヤー移動
	Move(PLAYER_SPEED);

	// プレイヤー攻撃
	Attack();

	if (m_bMove)
	{// 歩きモーション
		m_pMotion->Set(MOTIONTYPE_WALK);
	}
	else if (m_bAttack)
	{// 攻撃モーション
		m_pMotion->Set(MOTIONTYPE_ATTACK);
	}
	else
	{// 待機モーション
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}

	if (m_pMotion != nullptr)
	{// モーション更新
		m_pMotion->Update();
	}

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
void CPlayer::Move(float fSpeed)
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

	//Aが押された
	if (pInputKeyboard->GetPress(DIK_A) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{
		//左上
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;

			//移動方向にモデルを向ける
			m_RotDest.y = -rot.y - (D3DX_PI * 0.75f);

			// 歩き
			m_bMove = true;
		}
		//左下
		else if (pInputKeyboard->GetPress(DIK_S) == true
				 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{
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

	//Dが押された
	else if (pInputKeyboard->GetPress(DIK_D) == true
			 || pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{
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

	//Wが押された
	else if (pInputKeyboard->GetPress(DIK_W) == true
			 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0 ) > 0)
	{
		m_move.x -= cosf(rot.y) * fSpeed;
		m_move.z -= sinf(rot.y) * fSpeed;

		//移動方向にモデルを向ける
		m_RotDest.y = -rot.y - (D3DX_PI * 0.5f);

		// 歩き
		m_bMove = true;
	}

	//Sが押された
	else if (pInputKeyboard->GetPress(DIK_S) == true
			 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{
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

	//角度の正規化
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
}

//========================================
// 攻撃
//========================================
void CPlayer::Attack()
{
	// キーボードの情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();;
	
	// コントローラーの情報取得
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true 
		|| pInputPad->GetTrigger(CInputPad::BUTTON_X, 0) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_RB, 0) == true)
	{
		// 攻撃
		m_bAttack = true;
	}
}

//========================================
// モーション種類の取得
//========================================
int CPlayer::GetMotionType()
{
	return m_pMotion->GetType();
}

//========================================
// モーションループの取得
//========================================
bool CPlayer::GetMotionLoop(int nType)
{
	return m_pMotion->GetLoop(nType);
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
