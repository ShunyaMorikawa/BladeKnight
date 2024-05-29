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
	const float NOCKBACK = 50.0f;	// �m�b�N�o�b�N�l
}

//========================================
//�R���X�g���N�^
//========================================
CEnemy::CEnemy() : 
	m_nLife(0),
	m_fRadius(0.0f),
	m_bWalk(false),
	m_bAttack(false)
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

	// �����̐��K��
	USEFUL::NormalizeRotAngle(DiffRot.y);

	//Diff�ɕ␳�W����������
	rot.y += DiffRot.y * 0.1f;

	// �p�x�̐��K��
	USEFUL::NormalizeRotAngle(rot.y);

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

	// ���[�V�����Ǘ�
	Motion();

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

	// �m�b�N�o�b�N
	NockBack();

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
// �q�b�g����
//========================================
void CEnemy::NockBack()
{
	// �G�̏��擾
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer();

	// �ʒu�擾
	D3DXVECTOR3 posEnemy = GetPos();

	// �v���C���[�̈ʒu�ƈړ���
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();

	// ��΂����p�x
	float angle = atan2f(posEnemy.x - posEnemy.x, posPlayer.z - posEnemy.z);

	// �ʒu�X�V
	movePlayer.x = sinf(angle) * -NOCKBACK;
	movePlayer.z = cosf(angle) * -NOCKBACK;

	movePlayer.y = 25.0f;

	pPlayer->SetMove(movePlayer);
}

//========================================
// ���[�V�����Ǘ�
//========================================
void CEnemy::Motion()
{
	// ���[�V�������擾
	CMotion* pMotion = GetMotion();

	if (m_bWalk)
	{// �������[�V����
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_WALK);
	}
	else if (m_bAttack)
	{// �؂艺�낵���[�V����
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_CUTDOWN);

		if (pMotion->IsFinish() && m_bAttack == true)
		{// ���[�V�����I��
			m_bAttack = false;
		}
	}

	if (pMotion != nullptr)
	{// ���[�V�����X�V
		pMotion->Update();
	}
}

//========================================
// �U��
//========================================
void CEnemy::Attack()
{
	//if (fLength <= radiusEnemy + fRadius)
	//{// �؂肨�낵
	//	m_bAttack = true;

	//	// �G�Ƃ̓����蔻��
	//	CollisionPlayer(1);
	//}
}

//========================================
// �v���C���[�Ƃ̓����蔻��
//========================================
void CEnemy::CollisionPlayer(int nDamage)
{
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans;

	// ����̈ʒu
	D3DXMATRIX posWeapon;

	// ���[�V�������擾
	CMotion* pMotion = GetMotion();

	// ���f���̃I�t�Z�b�g�擾
	CModel* pModelOffset = pMotion->GetModel(13);

	// ���f���̃}�g���b�N�X
	D3DXMATRIX MtxModel = pModelOffset->GetMtxWorld();

	// �ʒu�擾
	D3DXVECTOR3 posEnemy = GetPos();

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

	// �v���C���[�̈ʒu
	D3DXVECTOR3 posPlayer = pEnemy->GetPos();
	D3DXVECTOR3 movePlayer = pEnemy->GetMove();

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
