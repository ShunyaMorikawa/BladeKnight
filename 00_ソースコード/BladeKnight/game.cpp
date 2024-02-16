//========================================
//
//�Q�[��������[game.cpp]
//Author�F�X��x��
//
//========================================
#include "game.h"
#include "object2D.h"
#include "objectx.h"
#include "time.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "light.h"
#include "player.h"
#include "polygon.h"
#include "gameobject.h"
#include "boss.h"
#include "billboard.h"
#include "effect.h"

//========================================
//�ÓI�����o�ϐ�
//========================================
CObject *CGame::m_pObject[] = {};			// �I�u�W�F�N�g�̃|�C���^
CObjectX *CGame::m_pObjectX = nullptr;		// �I�u�W�F�N�gX�̃|�C���^
CObject3D *CGame::m_pObject3D = nullptr;	// �I�u�W�F�N�g3D�̃|�C���^
CGame *CGame::m_pGame = nullptr;			// �Q�[���̃|�C���^
CLight *CGame::m_pLight = nullptr;			// ���C�g�̃|�C���^
CPlayer *CGame::m_pPlayer = nullptr;		// �v���C���[�̃|�C���^
CPolygon *CGame::m_pPolygon = nullptr;		// �|���S���̃|�C���^
CBoss *CGame::m_pBoss = nullptr;			// �{�X�̃|�C���^
CBillboard *CGame::m_pBillboard = nullptr;	// �r���{�[�h�̃|�C���^
CEffect *CGame::m_pEffect = nullptr;		// �G�t�F�N�g�̃|�C���^

//========================================
//�R���X�g���N�^
//========================================
CGame::CGame() : 
	m_bPause(false)
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
	//CGame�̃|�C���^
	CGame *pGame = nullptr;

	if (pGame == nullptr)
	{
		//�C���X�^���X����
		pGame = new CGame;

		//������
		pGame->Init();
	}

	//�|�C���^��Ԃ�
	return pGame;
}

//========================================
//������
//========================================
HRESULT CGame::Init(void)
{
	//���C�g����
	if (m_pLight == nullptr)
	{//m_pLight��nullptr�̎�
		m_pLight = new CLight;
	}

	//���C�g�̏���������
	if (FAILED(m_pLight->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	//�v���C���[�̐���
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create();
	}

	//�v���C���[�̏���������
	if (FAILED(m_pPlayer->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	// �{�X�̐���
	if (m_pBoss == nullptr)
	{
		m_pBoss = CBoss::Create();
	}

	//�{�X�̏���������
	if (FAILED(m_pBoss->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	// �|���S���̐���
	if (m_pPolygon == nullptr)
	{
		m_pPolygon = CPolygon::Create();
	}

	//�|���S������������
	if (FAILED(m_pPolygon->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	//�|�[�Y�̏��
	m_bPause = false;

	return S_OK;
}

//========================================
//�I��
//========================================
void CGame::Uninit(void)
{
	if (m_pPlayer != nullptr)
	{//�v���C���[�j��
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	if (m_pPolygon != nullptr)
	{// �|���S���j��
		m_pPolygon->Uninit();
		m_pPolygon = nullptr;
	}

	if (m_pBoss != nullptr)
	{// �G�l�~�[�j��
		m_pBoss->Uninit();
		m_pBoss = nullptr;
	}
}

//========================================
//�X�V
//========================================
void CGame::Update(void)
{
	//CInputKeyboard�^�̃|�C���^
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	//�L�[�{�[�h�̏��擾

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{// ��ʑJ��
		CManager::GetInstance()->SetMode(CScene::MODE_RESULT);
	}
}

//========================================
//�`��
//========================================
void CGame::Draw(void)
{
}
