//========================================
//
//�Q�[��������[game.cpp]
//Author�F�X��x��
//
//========================================

#include "game.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "field.h"
#include "bullet.h"
#include "enemy.h"
#include "fade.h"
#include "wall.h"
#include "mapobject.h"
#include "sound.h"
#include "debugproc.h"
#include "texture.h"
#include "useful.h"

//========================================
//�ÓI�����o�ϐ�
//========================================
CGame *CGame::m_pGame = nullptr;			// �Q�[���̃|�C���^

//========================================
// �萔��`
//========================================
namespace
{
	const int TRANSITION_TIME = 180;		// �J�ڂ���܂ł̎���
	
	const char* GUIDE_TEX = "data\\texture\\guide_game.png";	// �e�N�X�`���̃p�X
	const std::string PLAYER_TXT = "data//FILE//player.txt";	// �v���C���[���̃p�X

	const float GUIDE_WIDTH = 300.0f;	// �K�C�h�̕�
	const float GUIDE_HEIGHT = 350.0f;	// �K�C�h�̍���
	
	const D3DXVECTOR3 GUIDE_POS = { 1100.0f, 300.0f, 0.0f };	// �K�C�h�̈ʒu
}

//========================================
//�R���X�g���N�^
//========================================
CGame::CGame() : 
m_bPause		(false),	// �|�[�Y
m_nTransition	(0),		// �J�ڎ���
m_pObjectX		(nullptr),	// �I�u�W�F�N�gX�̃|�C���^
m_pIdxMesh		(nullptr),	// �C���f�b�N�X���b�V���̃|�C���^
m_pFade			(nullptr),	// �t�F�[�h�̃|�C���^
m_pObj2D		(nullptr)	// �I�u�W�F�N�g2D�̃|�C���^
{
	m_pGame = nullptr;		// �Q�[���̃|�C���^
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
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �v���C���[����
	CPlayer::Create(PLAYER_TXT);

	// �G�l�~�[����
	CEnemy::Create(Constance::ENEMY_TXT);

	// �t�B�[���h����
	CField::Create();

	// 4�����ɕǐ���
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, -Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

	// �}�b�v�I�u�W�F�N�g����
	CMapObject::Create();

	// �J�ڎ���
	m_nTransition = 0;

	//�|�[�Y�̏��
	m_bPause = false;

	// �C���X�^���X����
	m_pObj2D = CObject2D::Create();

	// �ʒu�ݒ�
	m_pObj2D->SetPos(GUIDE_POS);

	// �T�C�Y�ݒ�
	m_pObj2D->SetSize(GUIDE_WIDTH, GUIDE_HEIGHT);

	// �e�N�X�`���ݒ�
	m_pObj2D->BindTexture(pTexture->Regist(GUIDE_TEX));

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h�Đ�
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//========================================
//�I��
//========================================
void CGame::Uninit(void)
{
	if(m_pObj2D != nullptr)
	{
		m_pObj2D->Uninit();
		m_pObj2D = nullptr;
	}

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h��~
	pSound->Stop(CSound::SOUND_LABEL_BGM_TUTORIAL);

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

	if (pEnemy == nullptr || pPlayer == nullptr)
	{// �G���v���C���[�̗̑͂�0�ȉ��ɂȂ�����
		m_nTransition++;

		if (m_nTransition > TRANSITION_TIME ||
			pInputKeyboard->GetTrigger(DIK_RETURN))
		{
			// ��ʑJ��(�t�F�[�h)
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);

			m_nTransition = 0;
		}
	}

	// �f�o�b�O�\���̏��擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// �f�o�b�O�\��
	pDebugProc->Print("\n�J�E���^�[�F%d\n", m_nTransition);

#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
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
