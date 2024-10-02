//=======================================
//
//カメラ[camera.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "camera.h"   
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "useful.h"

//=======================================
// 定数定義
//=======================================
namespace
{
	const float CAMERA_POS_Y = 375.0f;		// カメラのY座標の初期位置
	const float CAMERA_POS_Z = -1000.0f;	// カメラのZ座標の初期位置
	const float CAMERA_DISTANCE = 400.0f;	// カメラとの距離
	const float CAMERA_SPEED = 1.5f;		// カメラの移動速度
	const float CMAERA_INERTIA = 0.2f;		// カメラの慣性
	const float CAMERA_R_DISTANCE = 25.0f;	// 注視点の距離
	const float CAMERA_V_MOVE = 0.03f;		// 視点の移動速度
	const float CAMERA_R_INERTIA = 0.2f;	// 注視点の慣性
	const float CAMERA_V_INERTIA = 0.2f;	// 視点の慣性
	const float CAMERA_DISTANCE_TITLE = 300.0f;		// カメラ
}

//=======================================
//コンストラクタ
//=======================================
CCamera::CCamera() : 
m_posV		(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 視点
m_posR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 注視点
m_vecU		(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 上方向ベクトル
m_rot		(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 向き
m_move		(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 移動量
m_VDest		(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 目的の視点
m_RDest		(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 目的の注視点
m_fDistance	(0.0f)								// 距離
{
}

//=======================================
//デストラクタ
//=======================================
CCamera::~CCamera()
{

}

//=======================================
//カメラの初期化処理
//=======================================
HRESULT CCamera::Init(void)
{
	//視点
	m_posV = D3DXVECTOR3(0.0f, CAMERA_POS_Y, CAMERA_POS_Z);

	//注視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//上方向ベクトル
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 目的の視点・注視点
	m_VDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//プレイヤーとカメラの距離
	m_fDistance = CAMERA_DISTANCE;

	// 視点・注視点
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance;

	//成功を返す
	return S_OK;
}

//=======================================
//カメラの終了処理
//=======================================
void CCamera::Uninit(void)
{

}

//=======================================
//カメラの更新処理
//=======================================
void CCamera::Update(void)
{	
	//CDebugProc型ポインタ
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//視点の移動
	CameraMoveV();

	// 闘技場との判定
	CollisionArena();

#ifdef _DEBUG
	//デバッグ表示
	pDebugProc->Print("\n視点の位置：%f、%f、%f\n", m_posV.x, m_posV.y, m_posV.z);
	pDebugProc->Print("注視点の位置：%f、%f、%f\n", m_posR.x, m_posR.y, m_posR.z);
	pDebugProc->Print("カメラの向き：%f、%f、%f\n", m_rot.x, m_rot.y, m_rot.z);
#endif
}

//=======================================
//カメラの設定処理
//=======================================
void CCamera::SetCamera(void)
{
	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成[透視投影]
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,		//プロジェクションマトリックス
								D3DXToRadian(100.0f),	//視野角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//アスペクト比
								10.0f,		//Z値の最小値
								400000.0f);	//Z値の最大値(描画距離)

	////プロジェクションマトリックスを設定[平行投影]
	//D3DXMatrixOrthoLH(&m_mtxProjection,		//プロジェクションマトリックス
	//					SCREEN_WIDTH,	//画面の幅
	//					SCREEN_HEIGHT,	//画面の高さ
	//					10.0f,		//Z値の最小値
	//					1000.0f);	//Z値の最大値

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
						&m_posV,	//視点
						&m_posR,	//注視点
						&m_vecU);	//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=======================================
//注視点の移動
//=======================================
void CCamera::CameraMoveV(void)
{
	// キーボードの情報取得
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// コントローラーの情報取得
	CInputPad *pInputPad = nullptr; 
	pInputPad = CManager::GetInstance()->GetInputPad();

	//Cキーが押された
	if (pInputKeyboard->GetPress(DIK_C) == true
		|| pInputPad->GetRStickXPress(CInputPad::BUTTON_R_STICK, 0) > 0)
	{
		m_rot.y += CAMERA_V_MOVE;

		//角度の正規化
		m_rot.y = RotNor(m_rot.y);
	}
	//Zキーが押された
	else if (pInputKeyboard->GetPress(DIK_Z) == true
			 || pInputPad->GetRStickXPress(CInputPad::BUTTON_R_STICK, 0) < 0)
	{
		m_rot.y -= CAMERA_V_MOVE;

		//角度の正規化
		m_rot.y = RotNor(m_rot.y);
	}

	//位置を更新
	m_posV += m_move;

	m_posR += m_move;

	//移動量を更新(減衰させる)
	m_move.x += (0.0f - m_move.x) * CMAERA_INERTIA;
	m_move.y += (0.0f - m_move.y) * CMAERA_INERTIA;
	m_move.z += (0.0f - m_move.z) * CMAERA_INERTIA;
}

//=======================================
//rotの正規化
//=======================================
float CCamera::RotNor(float RotN)
{
	//3.14を超えたときに反対にする
	if (RotN > D3DX_PI)
	{
		RotN -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (RotN < -D3DX_PI)
	{
		RotN += D3DX_PI * 2;
	}
	return RotN;
}

//=======================================
//カメラ追従
//=======================================
void CCamera::following(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//注視点
	D3DXVECTOR3 Diff;

	//視点
	D3DXVECTOR3 VDiff;

	m_rot.y = rot.y;

	//注視点
	m_RDest = D3DXVECTOR3(
		pos.x + -sinf(rot.y) 
		* CAMERA_R_DISTANCE,
		pos.y, 
		pos.z + -cosf(rot.y) * 25.0f);

	m_VDest.x = m_RDest.x + sinf(m_rot.y) * m_fDistance;
	m_VDest.z = m_RDest.z + cosf(m_rot.y) * m_fDistance;

	//注視点
	Diff = D3DXVECTOR3(m_RDest.x - m_posR.x, 
		0.0f, 
		m_RDest.z - m_posR.z);

	//注視点の慣性
	m_posR += Diff * CAMERA_R_INERTIA;

	//視点
	VDiff = D3DXVECTOR3(m_VDest.x - m_posV.x, 
		0.0f,
		m_VDest.z - m_posV.z);

	//視点の慣性
	m_posV += VDiff * CAMERA_V_INERTIA;
}

//=======================================
// タイトル用カメラ
//=======================================
void CCamera::TitleCamera()
{
	m_rot.y += 0.005f;

	//3.14を超えたときに反対にする
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	//-3.14を超えたときに反対にする
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	m_fDistance = 500.0f;

	// 視点
	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
}

//=======================================
// 闘技場との判定
//=======================================
void CCamera::CollisionArena()
{
	D3DXVECTOR3 vec;
	D3DXVec3Normalize(&vec, &m_posV);

	if (USEFUL::CollisionCircle(m_posV, Constance::ARENA_SIZE))
	{// 闘技場に当たったら
		m_posV.x = vec.x * Constance::ARENA_SIZE;
		m_posV.z = vec.z * Constance::ARENA_SIZE;
	}
}

//=======================================
//向きの取得
//=======================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

//=======================================
//視点の取得
//=======================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_posV;
}

//=======================================
//注視点の取得
//=======================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_posR;
}
