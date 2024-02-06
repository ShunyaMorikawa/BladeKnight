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
//�R���X�g���N�^
//========================================
CEnemy::CEnemy() :
	m_pTexture(nullptr),
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
	m_bWait(false),
	m_pMotion(nullptr)
{//�l���N���A
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//���f���̃|�C���^
}

//========================================
//�f�X�g���N�^
//========================================
CEnemy::~CEnemy()
{
}