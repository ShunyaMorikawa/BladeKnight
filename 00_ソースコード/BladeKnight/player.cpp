//========================================
//
//�V���[�e�B���O�A�N�V����[player.cpp]
//Author�F�X��x��
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
//�}�N����`
//========================================
#define PLAYER_SPEED	(2.0f)		//�v���C���[�̈ړ����x
#define PLAYER_INERTIA	(0.3f)		//�v���C���[�̊���

#define PLAYER_PATH	"data\\FILE\\player.txt"	//�ǂݍ��ރt�@�C���̃p�X

//========================================
//�R���X�g���N�^
//========================================
CPlayer::CPlayer() : 
	m_pTexture(nullptr),
	m_pos(0.0f, 0.0f, 0.0f),		//�ʒu
	m_move(0.0f, 0.0f, 0.0f),		//�ړ���
	m_rot(0.0f, 0.0f, 0.0f),		//����
	m_nIdxTexture(0),				//�e�N�X�`���̔ԍ�
	m_pMesh(nullptr),				//���b�V��(���_���)�ւ̃|�C���^
	m_pBuffMat(nullptr),			//�}�e���A���ւ̃|�C���^
	m_dwNumMat(0),					//�}�e���A���̐�
	m_apNumModel(0), 				//���f��(�p�[�c)�̑���
	m_RotDest(0.0f, 0.0f, 0.0f),	//�ړI�̌���
	m_nLife(0),						// �̗�
	m_bJump(false),
	m_bMove(false),
	m_bWait(false)
{//�l���N���A
}

//========================================
//�f�X�g���N�^
//========================================
CPlayer::~CPlayer()
{
}

//========================================
//�v���C���[�̐���
//========================================
CPlayer *CPlayer::Create(void)
{
	//CPlayer�^�̃|�C���^
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[����
		pPlayer = new CPlayer;

		//������
		pPlayer->Init();
	}

	//�|�C���^��Ԃ�
	return pPlayer;
}

//========================================
//������
//========================================
HRESULT CPlayer::Init(void)
{
	//���[�V�����̃|�C���^
	m_pMotion = nullptr;

	//�ړI�̌���
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�v���C���[�̏����ʒu
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, -300.0f));

	//�v���C���[�̏�������
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �̗�
	m_nLife = 10;

	if (m_pMotion == nullptr)
	{
		//���[�V��������
		m_pMotion = CMotion::Create();

		//���[�V�����ǂݍ���
		m_pMotion->Load(PLAYER_PATH);

		//�ҋ@���[�V����
		m_pMotion->Set(MOTION_STANDBY);

		//�ҋ@���
		m_bWait = true;
	}

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CPlayer::Uninit(void)
{
	if (m_pTexture != nullptr)
	{//�e�N�X�`���̔j��
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	if (m_pMotion != nullptr)
	{//���[�V�����j��
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//========================================
//�X�V
//========================================
void CPlayer::Update(void)
{
	// ���݂̃��[�V����
	EMotion nowMotion = MOTION_STANDBY;

	//CInputKeyboard�^�̃|�C���^
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	//�����擾
	D3DXVECTOR3 rot = GetRot();

	//�v���C���[�ړ�
	Move(PLAYER_SPEED);

	// ���[�V�����Ǘ�
	ManagementMotion(nowMotion);

	//�ʒu�X�V
	//SetPosition(D3DXVECTOR3(pos.x += m_move.x, 0.0f, pos.z += m_move.z));

	//�J�����Ǐ]
	CCamera *pCampera = CManager::GetInstance()->GetCamera();
	pCampera->following(pos, rot);

	if (m_pMotion != nullptr)
	{//���[�V�����X�V
		m_pMotion->Update();
	}

	//�|�C���^
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//�f�o�b�O�\��
	pDebugProc->Print("\n�v���C���[�̈ʒu�F%f�A%f�A%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("�v���C���[�̌����F%f�A%f�A%f\n", rot.x, rot.y, rot.z);
}

//========================================
//�`��
//========================================
void CPlayer::Draw(void)
{
	//�`��
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���[�V�����`��
	m_pMotion->Draw();
}

//========================================
//�v���C���[�̑���
//========================================
void CPlayer::Move(float fSpeed)
{
	//CInputKeyboard�^�̃|�C���^
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	//�L�[�{�[�h�̏��擾

	//CInputPad�^�̃|�C���^
	CInputPad *pInputPad = nullptr;
	pInputPad = CManager::GetInstance()->GetInputPad();	//�R���g���[���[�̏��擾

	//CCamera�^�̃|�C���^
	CCamera *pCamera = nullptr;
	pCamera = CManager::GetInstance()->GetCamera();

	//rot�̎擾
	D3DXVECTOR3 rot = pCamera->GetRot();

	//���_�̎擾
	D3DXVECTOR3 PosV = pCamera->GetPosV();

	//�����_�̎擾
	D3DXVECTOR3 PosR = pCamera->GetPosR();

	//�ړI�̌���
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//A�������ꂽ
	if (pInputKeyboard->GetPress(DIK_A) == true || pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{
		//����
		if (pInputKeyboard->GetPress(DIK_W) == true || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y - (D3DX_PI * 0.75f);
		}
		//����
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + (D3DX_PI * 0.75f);
		}
		else
		{//��
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + D3DX_PI;

			//�����_�����炷
			PosR - m_move;
		}
	}

	//D�������ꂽ
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//�E��
			m_move.x += cosf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + (-D3DX_PI * 0.25f);

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//�E��
			m_move.x += cosf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + (D3DX_PI * 0.25f);
		}
		else
		{//�E
			m_move.x += cosf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y;

			//�����_�����炷
			PosR + m_move;
		}
	}

	//W�������ꂽ
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{
		m_move.x -= cosf(rot.y) * fSpeed;
		m_move.z -= sinf(rot.y) * fSpeed;

		//�ړ������Ƀ��f����������
		m_RotDest.y = -rot.y - (D3DX_PI * 0.5f);
	}

	//S�������ꂽ
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{
		m_move.x += cosf(rot.y) * fSpeed;
		m_move.z += sinf(rot.y) * fSpeed;

		//�ړ������Ƀ��f����������
		m_RotDest.y = -rot.y + (D3DX_PI * 0.5f);
	}

	//�ʒu���X�V
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	//�ړ��ʂ��X�V(����������)
	m_move.x += (0.0f - m_move.x) * PLAYER_INERTIA;
	m_move.z += (0.0f - m_move.z) * PLAYER_INERTIA;

	//�ړI�̌���
	DiffRot.y = m_RotDest.y - m_rot.y;

	//�p�x�̐��K��
	if (DiffRot.y > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		DiffRot.y -= D3DX_PI * 2;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (DiffRot.y < -D3DX_PI)
	{
		DiffRot.y += D3DX_PI * 2;
	}

	//Diff�ɕ␳�W����������
	m_rot.y += DiffRot.y * 0.1f;

	//�p�x�̐��K��
	if (m_rot.y > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		m_rot.y -= D3DX_PI * 2;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//========================================
// �U��
//========================================
void CPlayer::Attack()
{
	// CInputKeyboard�^�̃|�C���^
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	//�L�[�{�[�h�̏��擾

	// CInputPad�^�̃|�C���^
	CInputPad *pInputPad = nullptr;
	pInputPad = CManager::GetInstance()->GetInputPad();	//�R���g���[���[�̏��擾

	// A�������ꂽ
	if (pInputKeyboard->GetPress(DIK_J) == true || pInputPad->GetTrigger(CInputPad::BUTTON_RB, 0) == true)
	{

	}
}

//========================================
//���[�V�����Ǘ�
//========================================
void CPlayer::ManagementMotion(int Motion)
{
	//���݂̃��[�V����
	int nNowMotion = GetMotionType();

	m_nOldMotion = nNowMotion;


}

//========================================
// ���[�V������ނ̎擾
//========================================
int CPlayer::GetMotionType()
{
	return m_pMotion->GetType();
}

//=======================================
//rot�̐��K��
//=======================================
float CPlayer::RotNormalize(float RotN, float Rot)
{
	//�p�x�̐��K��
	if (RotN > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		RotN -= D3DX_PI * 2;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (RotN < -D3DX_PI)
	{
		RotN += D3DX_PI * 2;
	}

	//Diff�ɕ␳�W����������
	Rot += RotN * 0.1f;

	//�p�x�̐��K��
	if (Rot > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		Rot -= D3DX_PI * 2;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (Rot < -D3DX_PI)
	{
		Rot += D3DX_PI * 2;
	}

	return RotN, Rot;
}

//========================================
//3D�I�u�W�F�N�g�̐���
//========================================
void CPlayer::SetVertex(void)
{
}

//========================================
//3D�I�u�W�F�N�g�̐���
//========================================
void CPlayer::SetSize(float fWidht, float fHeight)
{
}
