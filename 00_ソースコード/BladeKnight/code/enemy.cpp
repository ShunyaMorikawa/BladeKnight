//========================================
//
//�G�֘A[enemy.cpp]
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
#include "particle.h"
#include "player.h"
#include "game.h"
#include "useful.h"

//========================================
//���O���
//========================================
namespace
{
	const int LIFE = 10;			// �̗�
	const float SPEED = 5.0f;		// ���x
	const float GRAVITY = 2.0f;		// �d��
	const float INERTIA = 0.1f;		// ����
	const float RADIUS = 150.0f;	// ���a
}

//========================================
//�R���X�g���N�^
//========================================
CEnemy::CEnemy() : 
	m_nLife(0),
	m_fRadius(0.0f)
{//�l���N���A
}

//========================================
//�f�X�g���N�^
//========================================
CEnemy::~CEnemy()
{
}

//========================================
// ����
//========================================
CEnemy* CEnemy::Create(std::string pfile)
{
	CEnemy* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemy;

		pEnemy->Init(pfile);
	}

	return pEnemy;
}

//========================================
// ������
//========================================
HRESULT CEnemy::Init(std::string pfile)
{
	// �L�����̏�����
	CCharacter::Init(pfile);

	// �ʒu�ݒ�
	SetPos(D3DXVECTOR3(0.0f, 0.0f, -300.0f));

	// �����ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �̗�
	m_nLife = LIFE;

	// ���a
	m_fRadius = RADIUS;

#ifdef _DEBUG
	m_nLife = 50;
#endif

	return S_OK;
}

//========================================
// �I��
//========================================
void CEnemy::Uninit(void)
{
	// �I��
	CCharacter::Uninit();
}

//========================================
// �X�V
//========================================
void CEnemy::Update(void)
{
	// �ړI�̌���
	D3DXVECTOR3 DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �X�V
	CCharacter::Update();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʎ擾
	D3DXVECTOR3 move = GetMove();

	// �����擾
	D3DXVECTOR3 rot = GetRot();

	// �ړI�̌����擾
	D3DXVECTOR3 RotDest = GetRotDest();

	// �v���C���[���̎擾
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer();

	// �ʒu�擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	// �v���C���[�Ƃ̊p�x
	RotDest.y = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z);

	// �v���C���[�����Ɉړ�
	move.x = sinf(rot.y + D3DX_PI);
	move.z = cosf(rot.y + D3DX_PI);

	// �ʒu���X�V
	pos += move;

	// �ړ��ʂ��X�V(����������)
	move.x += (0.0f + move.x) * INERTIA;
	move.z += (0.0f + move.z) * INERTIA;


	//�ړI�̌���
	DiffRot.y = RotDest.y - rot.y;

	USEFUL::NormalizeRot(DiffRot.y);

	//�����̐��K��
	if (DiffRot.y > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		DiffRot.y -= D3DX_PI * 2.0f;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (DiffRot.y < -D3DX_PI)
	{
		DiffRot.y += D3DX_PI * 2.0f;
	}

	//Diff�ɕ␳�W����������
	rot.y += DiffRot.y * 0.1f;

	//�p�x�̐��K��
	if (rot.y > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		rot.y -= D3DX_PI * 2.0f;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	// �d��
	move.y -= GRAVITY;

	if (pos.y <= 0.0f)
	{// �ʒu�ƈړ��ʂ̍X�V��~
		pos.y = 0.0f;
		move.y = 0.0f;
	}

	// �ʒu�ݒ�
	SetPos(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRot(rot);

	// �ړI�̌����ݒ�
	SetRotDest(RotDest);

	// �|�C���^
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("\n�G�̈ʒu�F%f�A%f�A%f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("�G�̌����F%f�A%f�A%f\n", rot.x, rot.y, rot.z);
	pDebugProc->Print("�G�̗̑́F%d\n", m_nLife);
}

//========================================
// �`��
//========================================
void CEnemy::Draw(void)
{
	// �`��
	CCharacter::Draw();
}

//========================================
// �q�b�g����
//========================================
void CEnemy::Hit(int nLife)
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

//========================================
// �ړI�̕����֌�����
//========================================
void CEnemy::DestMove()
{
}
