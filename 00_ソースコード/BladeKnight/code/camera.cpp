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

//=======================================
// 定数定義
//=======================================
namespace
{
	const float CAMERA_SPEED = 1.5f;	// カメラの移動速度
	const float MOVEFAST = 0.2f;		// カメラの慣性
	const float CAMR = 25.0f;			// 注視点の距離
	const float CAMV_MOVE = 0.03f;		// 視点の移動速度
	const float CAM_DISDTANCE = 400.0f;	// カメラとプレイヤーの距離
	const float CAM_R_INERTIA = 0.2f;	// 注視点の慣性
	const float CAM_V_INERTIA = 0.2f;	// 視点の慣性
}

//=======================================
//コンストラクタ
//=======================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//注視点
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_fDistance = 0.0f;							//距離
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
	m_posV = D3DXVECTOR3(0.0f, 500.0f, -1000.0f);

	//注視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//上方向ベクトル
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//プレイヤーとカメラの距離
	m_fDistance = CAM_DISDTANCE;

	m_posV.x = m_posR.x + cosf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z + sinf(m_rot.y) * m_fDistance;

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
								4000.0f);	//Z値の最大値(描画距離)

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
//カメラの移動
//=======================================
void CCamera::Move(void)
{
	//CInputKeyboard型のポインタ
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//Aキーが押された
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//左上
			m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.75f)) * CAMERA_SPEED;
			m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.75f)) * CAMERA_SPEED;

			m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
			m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//左下
			m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.25f)) * CAMERA_SPEED;
			m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.25f)) * CAMERA_SPEED;

			m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
			m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
		}
		else
		{//左
			m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.5f)) * CAMERA_SPEED;
			m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.5f)) * CAMERA_SPEED;

			m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
			m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
		}
	}
	//Dキーが押された
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//右上
			m_posV.x += cosf(m_rot.y + (D3DX_PI * 0.75f)) * CAMERA_SPEED;
			m_posV.z += sinf(m_rot.y + (D3DX_PI * 0.75f)) * CAMERA_SPEED;

			m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
			m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//右下
			m_posV.x += cosf(m_rot.y + (D3DX_PI * 0.25f)) * CAMERA_SPEED;
			m_posV.z += sinf(m_rot.y + (D3DX_PI * 0.25f)) * CAMERA_SPEED;

			m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
			m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
		}
		else
		{//右
			m_posV.x += cosf(m_rot.y + (+D3DX_PI * 0.5f)) * CAMERA_SPEED;
			m_posV.z += sinf(m_rot.y + (+D3DX_PI * 0.5f)) * CAMERA_SPEED;

			m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
			m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
		}
	}
	//Wキーが押された
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		m_posV.x -= cosf(m_rot.y) * CAMERA_SPEED;
		m_posV.z -= sinf(m_rot.y) * CAMERA_SPEED;

		m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
	}
	//Sキーが押された
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		m_posV.x += cosf(m_rot.y) * CAMERA_SPEED;
		m_posV.z += sinf(m_rot.y) * CAMERA_SPEED;

		m_posR.x = m_posV.x - cosf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z - sinf(m_rot.y) * m_fDistance;
	}

	//Yキーが押された
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{
		m_posV.y += CAMERA_SPEED;
		m_posV.z += CAMERA_SPEED;
	}
	//Nキーが押された
	else if (pInputKeyboard->GetPress(DIK_N) == true)
	{
		m_posV.y -= CAMERA_SPEED;
		m_posV.z -= CAMERA_SPEED;
	}

	//位置を更新
	m_posV.x += m_move.x;
	m_posV.y += m_move.y;
	m_posV.z += m_move.z;

	m_posR.x += m_move.x;
	m_posR.y += m_move.y;
	m_posR.z += m_move.z;

	//移動量を更新(減衰させる)
	m_move.x += (0.0f - m_move.x) * MOVEFAST;
	m_move.y += (0.0f - m_move.y) * MOVEFAST;
	m_move.z += (0.0f - m_move.z) * MOVEFAST;
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
		m_rot.y += CAMV_MOVE;

		//角度の正規化
		m_rot.y = RotNor(m_rot.y);
	}
	//Zキーが押された
	else if (pInputKeyboard->GetPress(DIK_Z) == true
			 || pInputPad->GetRStickXPress(CInputPad::BUTTON_R_STICK, 0) < 0)
	{
		m_rot.y -= CAMV_MOVE;

		//角度の正規化
		m_rot.y = RotNor(m_rot.y);
	}

	////Yキーが押された
	//if (pInputKeyboard->GetPress(DIK_Y) == true)
	//{
	//	m_rot.x += CAMV_MOVE;

	//	//角度の正規化
	//	m_rot.x = RotNor(m_rot.x);

	//	m_posV.y = m_posR.y + cosf(m_rot.x) * m_fDistance;
	//	m_posV.z = m_posR.z + sinf(m_rot.x) * m_fDistance;
	//}
	////Nキーが押された
	//else if (pInputKeyboard->GetPress(DIK_N) == true)
	//{
	//	m_rot.x -= CAMV_MOVE;

	//	//角度の正規化
	//	m_rot.x = RotNor(m_rot.x);

	//	m_posV.y = m_posR.y + cosf(m_rot.x) * m_fDistance;
	//	m_posV.z = m_posR.z + sinf(m_rot.x) * m_fDistance;
	//}

	//位置を更新
	m_posV.x += m_move.x;
	m_posV.y += m_move.y;
	m_posV.z += m_move.z;

	m_posR.x += m_move.x;
	m_posR.y += m_move.y;
	m_posR.z += m_move.z;

	//移動量を更新(減衰させる)
	m_move.x += (0.0f - m_move.x) * MOVEFAST;
	m_move.y += (0.0f - m_move.y) * MOVEFAST;
	m_move.z += (0.0f - m_move.z) * MOVEFAST;
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

	//注視点
	m_RDest = D3DXVECTOR3(pos.x + -sinf(rot.y) 
		* CAMR, 
		pos.y, 
		pos.z + -cosf(rot.y) * 25.0f);

	m_VDest.x = m_RDest.x + sinf(m_rot.y) * m_fDistance;
	m_VDest.z = m_RDest.z + cosf(m_rot.y) * m_fDistance;

	//注視点
	Diff = D3DXVECTOR3(m_RDest.x - m_posR.x, 
		0.0f, 
		m_RDest.z - m_posR.z);

	//注視点の慣性
	m_posR += Diff * CAM_R_INERTIA;

	//視点
	VDiff = D3DXVECTOR3(m_VDest.x - m_posV.x, 
		0.0f,
		m_VDest.z - m_posV.z);

	//視点の慣性
	m_posV += VDiff * CAM_V_INERTIA;
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
