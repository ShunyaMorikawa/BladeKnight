//========================================
//
//�Q�[��������[game.cpp]
//Author�F�X��x��
//
//========================================
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "enemy.h"
#include "fade.h"

//========================================
//�ÓI�����o�ϐ�
//========================================
CGame *CGame::m_pGame = nullptr;			// �Q�[���̃|�C���^

//========================================
//�R���X�g���N�^
//========================================
CGame::CGame() : 
	m_bPause(false),
	m_nTransition(0)
{
}

//========================================
//�f�X�g���N�^
//========================================
CGame::~CGame()
{
}

//========================================
//����
//========================================
CGame *CGame::Create(void)
{
	if (m_pGame == nullptr)
	{
		//�C���X�^���X����
		m_pGame = new CGame;
	}

	//�|�C���^��Ԃ�
	return m_pGame;
}

//========================================
//������
//========================================
HRESULT CGame::Init(void)
{
	// �v���C���[����
	m_pPlayer = CPlayer::Create("data//FILE//player.txt");

	// �t�B�[���h����
	m_pField = CField::Create();

	// �G�l�~�[����
	m_pEnemy = CEnemy::Create("data//FILE//boss.txt");

	// �J�ڎ���
	m_nTransition = 0;

	//�|�[�Y�̏��
	m_bPause = false;

	return S_OK;
}

//========================================
//�I��
//========================================
void CGame::Uninit(void)
{
	m_pGame = nullptr;
}

//========================================
//�X�V
//========================================
void CGame::Update(void)
{
	// CInputKeyboard�^�̃|�C���^
	CInputKeyboard* pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �G�̗͎̑擾
	int nLife = m_pEnemy->GetLife();

	if (nLife < 0)
	{
		m_nTransition++;

		if (m_nTransition >= 120)
		{
			// ��ʑJ��(�t�F�[�h)
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);

			m_nTransition = 0;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{// �Q�[����ʂɑJ��
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
}

//========================================
//�`��
//========================================
void CGame::Draw(void)
{
}

//========================================
// �V���O���g��
//========================================
CGame *CGame::GetInstance(void)
{
	if (m_pGame == nullptr)
	{//�C���X�^���X����

		return m_pGame = new CGame;
	}
	else
	{//�|�C���^��Ԃ�
		return m_pGame;
	}
}
