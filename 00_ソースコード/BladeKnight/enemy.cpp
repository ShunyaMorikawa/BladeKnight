//========================================
//
//�V���[�e�B���O�A�N�V����[player.cpp]
//Author�F�X��x��
//
//========================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "game.h"

//========================================
//�}�N����`
//========================================
#define PLAYER_SPEED	(4.0f)		//�v���C���[�̈ړ����x
#define PLAYER_INERTIA	(0.3f)		//�v���C���[�̊���

#define PLAYER_PATH	"data\\FILE\\player.txt"	//�ǂݍ��ރt�@�C���̃p�X

//========================================
//�R���X�g���N�^
//========================================
CEnemy::CEnemy() :
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
CEnemy::~CEnemy()
{
}

//========================================
//�v���C���[�̐���
//========================================
CEnemy *CEnemy::Create(void)
{
	//CEnemy�^�̃|�C���^
	CEnemy *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[����
		pPlayer = new CEnemy;

		//������
		pPlayer->Init();
	}

	//�|�C���^��Ԃ�
	return pPlayer;
}

//========================================
//������
//========================================
HRESULT CEnemy::Init(void)
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

		//�ҋ@���
		m_bWait = true;
	}

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CEnemy::Uninit(void)
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
//�X�V
//========================================
void CEnemy::Update(void)
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

	//�ʒu�X�V
	//SetPosition(D3DXVECTOR3(pos.x += m_move.x, 0.0f, pos.z += m_move.z));

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
void CEnemy::Draw(void)
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
// ���[�V������ނ̎擾
//========================================
int CEnemy::GetMotionType()
{
	return m_pMotion->GetType();
}

//========================================
// ���[�V�������[�v�̎擾
//========================================
bool CEnemy::GetMotionLoop(int nType)
{
	return m_pMotion->GetLoop(nType);
}

//=======================================
//rot�̐��K��
//=======================================
float CEnemy::RotNormalize(float RotN, float Rot)
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
void CEnemy::SetVertex(void)
{
}

//========================================
//3D�I�u�W�F�N�g�̐���
//========================================
void CEnemy::SetSize(float fWidht, float fHeight)
{
}
