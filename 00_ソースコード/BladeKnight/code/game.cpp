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
#include "wall.h"
#include "mapobject.h"

//========================================
//�ÓI�����o�ϐ�
//========================================
CGame *CGame::m_pGame = nullptr;			// �Q�[���̃|�C���^

//========================================
// ���O���
//========================================
namespace
{
	const int TRANSITIONTIME = 120;
}

//========================================
//�R���X�g���N�^
//========================================
CGame::CGame() : 
	m_bPause(false),
	m_nTransition(0),
	m_pObjectX(nullptr),
	m_pIdxMesh(nullptr),
	m_pField(nullptr),
	m_pFade(nullptr),
	m_pMobj(nullptr)
{
	m_pGame = nullptr;
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
	CPlayer::Create("data//FILE//player.txt");

	// �t�B�[���h����
	m_pField = CField::Create();

	// �G�l�~�[����
	CEnemy::Create("data//FILE//motion.txt");

	// �ǐ���
	CWall::Create(D3DXVECTOR3(0.0f, 2000.0f, -4000.0f), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, 2000.0f, 4000.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(4000.0f, 2000.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f,  -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-4000.0f, 2000.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f,  D3DX_PI * 0.5f, 0.0f));

	// �}�b�v�I�u�W�F�N�g����
	m_pMobj = CMapObject::Create();

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
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	
	// �v���C���[�̏��擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	// �G�̏��擾
	CEnemy* pEnemy = CEnemy::GetInstance();

	if (pEnemy != nullptr && pPlayer != nullptr)
	{
		// �v���C���[�̗͎̑擾
		int PlayerLife = pPlayer->GetLife();

		// �G�̗͎̑擾
		int EnemyLife = pEnemy->GetLife();

		if (EnemyLife <= 0 || PlayerLife <= 0)
		{// �G���v���C���[�̗̑͂�0�ȉ��ɂȂ�����
			m_nTransition++;

			if (m_nTransition >= TRANSITIONTIME)
			{
				// ��ʑJ��(�t�F�[�h)
				CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);

				m_nTransition = 0;
			}
		}
	}

#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{// �Q�[����ʂɑJ��
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
#endif
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
