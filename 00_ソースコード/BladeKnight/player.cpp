//========================================
//
//�v���C���[[player.cpp]
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
#include "tutorial.h"
#include "scarecrow.h"

//========================================
//�}�N����`
//========================================
#define PLAYER_LIFE		(10)		// �̗�
#define PLAYER_SIZE		(50.0f)		// �T�C�Y
#define PLAYER_SPEED	(4.0f)		// �v���C���[�̈ړ����x
#define PLAYER_INERTIA	(0.3f)		// �v���C���[�̊���

#define PLAYER_PATH	"data\\FILE\\player.txt"	// �ǂݍ��ރt�@�C���̃p�X

//========================================
//�R���X�g���N�^
//========================================
CPlayer::CPlayer() : 
	m_pos(0.0f, 0.0f, 0.0f),		// �ʒu
	m_move(0.0f, 0.0f, 0.0f),		// �ړ���
	m_rot(0.0f, 0.0f, 0.0f),		// ����
	m_pMesh(nullptr),				// ���b�V��(���_���)�ւ̃|�C���^
	m_pBuffMat(nullptr),			// �}�e���A���ւ̃|�C���^
	m_dwNumMat(0),					// �}�e���A���̐�
	m_apNumModel(0), 				// ���f��(�p�[�c)�̑���
	m_RotDest(0.0f, 0.0f, 0.0f),	// �ړI�̌���
	m_nLife(0),						// �̗�
	m_fSize(0.0f),					// �T�C�Y
	m_bMove(false),
	m_bWait(false),
	m_pMotion(nullptr),
	m_pBoss(nullptr)
{//�l���N���A
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//���f���̃|�C���^
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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, -300.0f);

	//�v���C���[�̏�������
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �̗�
	m_nLife = PLAYER_LIFE;

	// �T�C�Y
	m_fSize = PLAYER_SIZE;

	if (m_pMotion == nullptr)
	{
		//���[�V��������
		m_pMotion = CMotion::Create();

		//���[�V�����ǂݍ���
		m_pMotion->Load(PLAYER_PATH);

		//�ҋ@���[�V����
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);
	}

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CPlayer::Uninit(void)
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
void CPlayer::Update(void)
{
	// ����
	m_bMove = false;

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	// �����擾
	D3DXVECTOR3 rot = GetRot();

	// �J�����Ǐ]
	CCamera *pCampera = CManager::GetInstance()->GetCamera();
	pCampera->following(pos, rot);

	// �v���C���[�s��
	Act(PLAYER_SPEED);

	if (m_bMove)
	{// �������[�V����
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_WALK);
	}
	else if (m_bCutdown)
	{// �؂艺�낵���[�V����
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_CUTDOWN);

		if (m_pMotion->IsFinish() && m_bCutdown == true)
		{// ���[�V�����I��
			m_bCutdown = false;
		}
	}
	else if (m_bMowingdown)
	{// �ガ����
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_MOWINGDOWN);

		if (m_pMotion->IsFinish() && m_bMowingdown == true)
		{// ���[�V�����I��
			m_bMowingdown = false;
		}
	}
	else if (m_bStrongAttack)
	{// ���U��
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_STRONGATTACK);

		if (m_pMotion->IsFinish() && m_bStrongAttack == true)
		{// ���[�V�����I��
			m_bStrongAttack = false;
		}
	}
	else
	{// �ҋ@���[�V����
		m_pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);
	}

	if (m_pMotion != nullptr)
	{// ���[�V�����X�V
		m_pMotion->Update();
	}

	//�`���[�g���A���G�l�~�[�Ƃ̓����蔻��
	CollisionScarecrow();

	// �|�C���^
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	// �f�o�b�O�\��
	pDebugProc->Print("\n�v���C���[�̈ʒu�F%f�A%f�A%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("�v���C���[�̌����F%f�A%f�A%f\n", rot.x, rot.y, rot.z);
	pDebugProc->Print("�v���C���[�̗̑́F%d\n", m_nLife);
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
void CPlayer::Act(float fSpeed)
{
	// �L�[�{�[�h�̏��擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	
	
	// �R���g���[���[�̏��擾	
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	//CCamera�^�̃|�C���^
	CCamera *pCamera = nullptr;
	pCamera = CManager::GetInstance()->GetCamera();

	//rot�̎擾
	D3DXVECTOR3 rot = pCamera->GetRot();

	//�����_�̎擾
	D3DXVECTOR3 PosR = pCamera->GetPosR();

	//�ړI�̌���
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �I���������̎擾
	bool bFinish = m_pMotion->IsFinish();

	if (pInputKeyboard->GetPress(DIK_A) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//A�������ꂽ
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//����
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y - (D3DX_PI * 0.75f);

			// ����
			m_bMove = true;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
				 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//����
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + (D3DX_PI * 0.75f);

			// ����
			m_bMove = true;
		}
		else
		{//��
			m_move.x += cosf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_move.z += sinf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + D3DX_PI;

			//�����_�����炷
			PosR - m_move;

			// ����
			m_bMove = true;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_D) == true
			 || pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//D�������ꂽ
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0 ) > 0)
		{//�E��
			m_move.x += cosf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + (-D3DX_PI * 0.25f);

			// ����
			m_bMove = true;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
				 || pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//�E��
			m_move.x += cosf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y + (D3DX_PI * 0.25f);

			// ����
			m_bMove = true;
		}
		else
		{//�E
			m_move.x += cosf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_move.z += sinf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;

			//�ړ������Ƀ��f����������
			m_RotDest.y = -rot.y;

			//�����_�����炷
			PosR + m_move;

			// ����
			m_bMove = true;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_W) == true
			 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0 ) > 0)
	{//W�������ꂽ
		m_move.x -= cosf(rot.y) * fSpeed;
		m_move.z -= sinf(rot.y) * fSpeed;

		//�ړ������Ƀ��f����������
		m_RotDest.y = -rot.y - (D3DX_PI * 0.5f);

		// ����
		m_bMove = true;
	}

	else if (pInputKeyboard->GetPress(DIK_S) == true
			 || pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//S�������ꂽ
		m_move.x += cosf(rot.y) * fSpeed;
		m_move.z += sinf(rot.y) * fSpeed;

		//�ړ������Ƀ��f����������
		m_RotDest.y = -rot.y + (D3DX_PI * 0.5f);

		// ����
		m_bMove = true;
	}

	//�ʒu���X�V
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	//�ړ��ʂ��X�V(����������)
	m_move.x += (0.0f - m_move.x) * PLAYER_INERTIA;
	m_move.z += (0.0f - m_move.z) * PLAYER_INERTIA;

	//�ړI�̌���
	DiffRot.y = m_RotDest.y - m_rot.y;

	//�����̐��K��
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

	// �U��
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_X, 0) == true)
	{// �؂肨�낵
		m_bCutdown = true;
	}

	if (pInputPad->GetTrigger(CInputPad::BUTTON_B, 0) == true)
	{// �ガ����
		m_bMowingdown = true;
	}

	if (pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// ���U��
		m_bStrongAttack = true;
	}
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
// �`���[�g���A���p�G�l�~�[�Ƃ̓����蔻��
//========================================
void CPlayer::CollisionScarecrow()
{
	//�ϐ��錾
	float fLength;		//����

	// �`���[�g���A���p�G�l�~�[�̏��擾
	CScarecrow *pScarecrow = CTutorial::GetScarecrow();

	//�`���[�g���A���p�G�l�~�[�̈ʒu�擾
	D3DXVECTOR3 posScarecrow = pScarecrow->GetPosition();

	// �`���[�g���A���p�G�l�~�[�̃T�C�Y�擾
	float sizeScarecrow = pScarecrow->GetSize();

	//�x�N�g�������߂�
	D3DXVECTOR3 vec = posScarecrow - this->GetPosition();

	//�x�N�g�����
	fLength = D3DXVec3Length(&vec);

	if (fLength <= sizeScarecrow)
	{
		CManager::GetInstance()->SetMode(CScene::MODE_RESULT);
	}
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
	fWidht = 0.0f;
	fHeight = 0.0f;
}
