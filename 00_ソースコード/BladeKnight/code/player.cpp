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
#include "game.h"
#include "particle.h"
#include "enemy.h"

//========================================
//���O���
//========================================
namespace
{
	const int LIFE = 10;			// �̗�
	const float NOCKBACK = 50.0f;	// �m�b�N�o�b�N�l
	const float SPEED = 4.0f;		// ���x
	const float INERTIA = 0.3f;		// ����
	const float RADIUS = 50.0f;		// ���a
}

//========================================
//�R���X�g���N�^
//========================================
CPlayer::CPlayer(int nPriority) : CCharacter(nPriority)
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
CPlayer *CPlayer::Create(std::string pfile)
{
	//CPlayer�^�̃|�C���^
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[����
		pPlayer = new CPlayer;

		//������
		pPlayer->Init(pfile);
	}

	//�|�C���^��Ԃ�
	return pPlayer;
}

//========================================
// ������
//========================================
HRESULT CPlayer::Init(std::string pfile)
{
	// �L�����̏�����
	CCharacter::Init(pfile);

	// �ʒu�ݒ�
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �����ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �̗�
	m_nLife = LIFE;

	return S_OK;
}

//========================================
//�I��
//========================================
void CPlayer::Uninit(void)
{
	// �I��
	CCharacter::Uninit();
}

//========================================
//�X�V
//========================================
void CPlayer::Update(void)
{
	// �L�[�{�[�h�̏��擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �R���g���[���[�̏��擾	
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRot();

	// �J�����Ǐ]
	CCamera* pCampera = CManager::GetInstance()->GetCamera();
	pCampera->following(pos, rot);

	// �v���C���[�s��
	Act(SPEED);

	//if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	//{// �e�̐���
	//	CBullet::Create(D3DXVECTOR3(pos),		// �ʒu
	//		D3DXVECTOR3(0.0f, 0.0f, -30.0f),	// �ړ��� 
	//		60);	// �̗�
	//}

	// �f�o�b�O�\���̏��擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// �f�o�b�O�\��
	pDebugProc->Print("\n�v���C���[�̈ʒu�F%f�A%f�A%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("�v���C���[�̈ړ��ʁF%f�A%f�A%f\n", move.x, move.y, move.z);
	pDebugProc->Print("�v���C���[�̌����F%f�A%f�A%f\n", rot.x, rot.y, rot.z);
	pDebugProc->Print("�v���C���[�̗̑́F%d\n", m_nLife);
}

//========================================
//�`��
//========================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//========================================
// �s��
//========================================
void CPlayer::Act(float fSpeed)
{
	// ���[�V��������
	m_bMove = false;

	// �L�[�{�[�h�̏��擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �R���g���[���[�̏��擾	
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	//CCamera�^�̃|�C���^
	CCamera* pCamera = nullptr;
	pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̌����擾
	D3DXVECTOR3 Camrot = pCamera->GetRot();

	// �ړI�̌���
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRot();

	// �ړI�̌����擾
	D3DXVECTOR3 RotDest = GetRotDest();

	if (pInputKeyboard->GetPress(DIK_A) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//A�������ꂽ
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//����
			//�ړ������Ƀ��f����������
			RotDest.y = -D3DX_PI * 0.25f;

			// �p�[�e�B�N������
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// ����
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.75f) * fSpeed;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//����
			//�ړ������Ƀ��f����������
			RotDest.y = -D3DX_PI * 0.75f;

			// �p�[�e�B�N������
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// ����
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//��
			//�ړ������Ƀ��f����������
			RotDest.y = -D3DX_PI * 0.5f;

			// �p�[�e�B�N������
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// ����
			m_bMove = true;

			move.x += sinf(Camrot.y + D3DX_PI * 0.5f) * fSpeed;
			move.z += cosf(Camrot.y + D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true
		|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//D�������ꂽ
		if (pInputKeyboard->GetPress(DIK_W) == true
			|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
		{//�E��
			//�ړ������Ƀ��f����������
			RotDest.y = D3DX_PI * 0.25f;

			// �p�[�e�B�N������
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// ����
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.75f) * fSpeed;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true
			|| pInputPad->GetLStickXPress(CInputPad::BUTTON_L_STICK, 0) < 0)
		{//�E��
			//�ړ������Ƀ��f����������
			RotDest.y = D3DX_PI * 0.75f;

			// �p�[�e�B�N������
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// ����
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//�E
			//�ړ������Ƀ��f����������
			RotDest.y = D3DX_PI * 0.5f;

			// �p�[�e�B�N������
			Myparticle::Create(Myparticle::TYPE_WALK, pos);

			// ����
			m_bMove = true;

			move.x += sinf(Camrot.y + -D3DX_PI * 0.5f) * fSpeed;
			move.z += cosf(Camrot.y + -D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true
		|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) > 0)
	{//W�������ꂽ
		//�ړ������Ƀ��f����������
		RotDest.y = 0.0f;

		// �p�[�e�B�N������
		Myparticle::Create(Myparticle::TYPE_WALK, pos);

		// ����
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI) * fSpeed;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true
		|| pInputPad->GetLStickYPress(CInputPad::BUTTON_L_STICK, 0) < 0)
	{//S�������ꂽ
		//�ړ������Ƀ��f����������
		RotDest.y = D3DX_PI;

		// �p�[�e�B�N������
		Myparticle::Create(Myparticle::TYPE_WALK, pos);

		// ����
		m_bMove = true;

		move.x += sinf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;
		move.z += cosf(Camrot.y + D3DX_PI * 0.0f) * fSpeed;
	}

	//�ʒu���X�V
	pos.x += move.x;
	pos.z += move.z;

	//�ړ��ʂ��X�V(����������)
	move.x += (0.0f - move.x) * INERTIA;
	move.z += (0.0f - move.z) * INERTIA;

	//�ړI�̌���
	DiffRot.y = RotDest.y - rot.y;

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
	rot.y += DiffRot.y * 0.1f;

	//�p�x�̐��K��
	if (rot.y > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		rot.y -= D3DX_PI * 2;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	// �ʒu�ݒ�
	SetPos(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRot(rot);

	// �ړI�̌����ݒ�
	SetRotDest(RotDest);

	// �U��
	Attack();

	// ���[�V����
	Motion();
}

//========================================
// �U��
//========================================
void CPlayer::Attack()
{
	// �L�[�{�[�h�̏��擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �R���g���[���[�̏��擾	
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_X, 0) == true)
	{// �؂肨�낵
		m_bCutdown = true;

		// �G�Ƃ̓����蔻��
		CollisionEnemy(1);
	}

	if (pInputKeyboard->GetTrigger(DIK_E) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// �؂肨�낵
		m_bMowingdown = true;

		// �G�Ƃ̓����蔻��
		CollisionEnemy(2);
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true
		|| pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0) == true)
	{// �؂肨�낵
		m_bStrongAttack = true;

		// �G�Ƃ̓����蔻��
		CollisionEnemy(3);
	}
}

//========================================
// ���[�V�����Ǘ�
//========================================
void CPlayer::Motion()
{
	// ���[�V�������擾
	CMotion* pMotion = GetMotion();

	if (m_bMove)
	{// �������[�V����
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_WALK);
	}
	else if (m_bCutdown)
	{// �؂艺�낵���[�V����
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_CUTDOWN);

		if (pMotion->IsFinish() && m_bCutdown == true)
		{// ���[�V�����I��
			m_bCutdown = false;
		}
	}
	else if (m_bMowingdown)
	{// �ガ����
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_MOWINGDOWN);

		if (pMotion->IsFinish() && m_bMowingdown == true)
		{// ���[�V�����I��
			m_bMowingdown = false;
		}
	}
	else if (m_bStrongAttack)
	{// ���U��
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_STRONGATTACK);

		if (pMotion->IsFinish() && m_bStrongAttack == true)
		{// ���[�V�����I��
			m_bStrongAttack = false;
		}
	}
	else
	{// �ҋ@���[�V����
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);
	}

	if (pMotion != nullptr)
	{// ���[�V�����X�V
		pMotion->Update();
	}
}

//========================================
// �G�ƃv���C���[�̓����蔻��
//========================================
void CPlayer::CollisionEnemy(int nDamage)
{
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans;

	// ����̈ʒu
	D3DXMATRIX posWeapon;

	// ���[�V�������擾
	CMotion* pMotion = GetMotion();

	// ���f���̃I�t�Z�b�g�擾
	CModel *pModelOffset = pMotion->GetModel(13);

	// ���f���̃}�g���b�N�X
	D3DXMATRIX MtxModel = pModelOffset->GetMtxWorld();

	// �ʒu�擾
	D3DXVECTOR3 posPlayer = GetPos();

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 150.0f, 0.0f);
	D3DXMatrixMultiply(&posWeapon, &mtxTrans, &MtxModel);

	// �}�g���b�N�X�̈ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(posWeapon._41, posWeapon._42, posWeapon._43);

#ifdef _DEBUG

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �F

	// ���a�̊�l
	float StandardRadius = 25.0f;

	int nLife = 10;

	// �G�t�F�N�g����
	CEffect::Create(pos, move, col, StandardRadius, nLife, true);
#endif

	// ����
	float fLength;

	// �v���C���[�̔��a
	float fRadius = RADIUS;

	// �G�̏��擾
	CEnemy* pEnemy = CGame::GetInstance()->GetEnemy();

	// �G�̈ʒu
	D3DXVECTOR3 posEnemy = pEnemy->GetPos();
	D3DXVECTOR3 moveEnemy = pEnemy->GetMove();

	// ���a
	float radiusEnemy = pEnemy->GetRadius();

#ifdef _DEBUG
	CEffect::Create(posEnemy, move, col, radiusEnemy, nLife, true);
#endif

	// �x�N�g�������߂�
	D3DXVECTOR3 vec = posEnemy - pos;

	// �x�N�g�����
	fLength = D3DXVec3Length(&vec);

	if (fLength <= radiusEnemy + fRadius)
	{// �q�b�g
		pEnemy->Hit(nDamage);

		// �m�b�N�o�b�N
		NockBack();
	}
}

//========================================
// �m�b�N�o�b�N
//========================================
void CPlayer::NockBack()
{
	// �G�̏��擾
	CEnemy* pEnemy = CGame::GetInstance()->GetEnemy();

	// �ʒu�擾
	D3DXVECTOR3 posPlayer = GetPos();

	// �G�̈ʒu�ƈړ���
	D3DXVECTOR3 posEnemy = pEnemy->GetPos();
	D3DXVECTOR3 moveEnemy = pEnemy->GetMove();

	// ��΂�������
	float angle = atan2f(posPlayer.x - posEnemy.x, posPlayer.z - posEnemy.z);

	// �ʒu�X�V
	moveEnemy.x = sinf(angle) * -NOCKBACK;
	moveEnemy.z = cosf(angle) * -NOCKBACK;

	moveEnemy.y = 25.0f;

	pEnemy->SetMove(moveEnemy);
}

//========================================
// �q�b�g����
//========================================
void CPlayer::Hit(int nLife)
{
	D3DXVECTOR3 pos = GetPos();

	// �̗͌��炷
	m_nLife -= nLife;

	if (m_nLife <= 0)
	{
		Uninit();

		// �p�[�e�B�N������
		Myparticle::Create(Myparticle::TYPE_DEATH, pos);
	}
}
