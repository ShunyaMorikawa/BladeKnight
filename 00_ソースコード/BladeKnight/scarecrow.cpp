//========================================
//
// �`���[�g���A���G�l�~�[[scarecrow.cpp]
// Author�F�X��x��
//
//========================================
#include "scarecrow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "game.h"
#include "player.h"

//========================================
//�}�N����`
//========================================
#define MOTION_PATH	"data\\FILE\\scarecrow.txt"	//�ǂݍ��ރt�@�C���̃p�X

//========================================
// �R���X�g���N�^
//========================================
CScarecrow::CScarecrow():
	m_pos(0.0f, 0.0f, 0.0f),		// �ʒu
	m_rot(0.0f, 0.0f, 0.0f),		// ����
	m_RotDest(0.0f, 0.0f, 0.0f),	// �ړI�̌���
	m_vtxMin(0.0f, 0.0f, 0.0f),		// �ŏ��l
	m_vtxMax(0.0f, 0.0f, 0.0f),		// �ő�l
	m_fAngle(0.0f)
{
}

//========================================
// �f�X�g���N�^
//========================================
CScarecrow::~CScarecrow()
{
}

//========================================
// ����
//========================================
CScarecrow *CScarecrow::Create()
{
	CScarecrow *pScarecrow = nullptr;

	if (pScarecrow == nullptr)
	{
		// �C���X�^���X����
		pScarecrow = new CScarecrow;

		// ������
		pScarecrow->Init();
	}

	return pScarecrow;
}

//========================================
// ������
//========================================
HRESULT CScarecrow::Init(void)
{	
	// ����
	m_rot = D3DXVECTOR3(0.0f, -1.7f, 0.0f);

	// �����̐ݒ�
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ŏ��l
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ő�l
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ʒu�ݒ�
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���[�V�����̃|�C���^
	m_pMotion = nullptr;

	if (m_pMotion == nullptr)
	{
		//���[�V��������
		m_pMotion = CMotion::Create();

		//���[�V�����ǂݍ���
		m_pMotion->Load(MOTION_PATH);

		//�ҋ@���[�V����
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}

	return S_OK;
}

//========================================
// �I��
//========================================
void CScarecrow::Uninit(void)
{
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
// �X�V
//========================================
void CScarecrow::Update(void)
{
	//�ړI�̌���
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�v���C���[�̏��擾
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer == NULL)
	{
		return;
	}

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	//�v���C���[�Ƃ̊p�x
	m_fAngle = atan2f(posPlayer.x - m_pos.x, posPlayer.z - m_pos.z);

	//�v���C���[�̕����Ɍ�����
	m_rot.y = m_fAngle;

	if (m_pMotion != nullptr)
	{//���[�V�����X�V
		m_pMotion->Update();
	}

	//�|�C���^
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//�f�o�b�O�\��
	pDebugProc->Print("\n�G�̈ʒu�F%f�A%f�A%f\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("�G�̌����F%f�A%f�A%f\n", m_rot.x, m_rot.y, m_rot.z);
}

//========================================
// �`��
//========================================
void CScarecrow::Draw(void)
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

//=======================================
// rot�̐��K��
//=======================================
float CScarecrow::RotNormalize(float RotN, float Rot)
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
// 3D�I�u�W�F�N�g�̐���
//========================================
void CScarecrow::SetVertex(void)
{
}

//========================================
// 3D�I�u�W�F�N�g�̐���
//========================================
void CScarecrow::SetSize(float fWidht, float fHeight)
{
}
