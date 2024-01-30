//========================================
//
//�`���[�g���A��[tutorial.h]
//Author�F�X��x��
//
//========================================
#include "tutorial.h"
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
#include "scarecrow.h"

//========================================
//�ÓI�����o�ϐ�
//========================================
CObject *CTutorial::m_pObject[] = {};			// �I�u�W�F�N�g�̃|�C���^
CObjectX *CTutorial::m_pObjectX = nullptr;		// �I�u�W�F�N�gX�̃|�C���^
CObject3D *CTutorial::m_pObject3D = nullptr;	// �I�u�W�F�N�g3D�̃|�C���^
CLight *CTutorial::m_pLight = nullptr;			// ���C�g�̃|�C���^
CPlayer *CTutorial::m_pPlayer = nullptr;		// �v���C���[�̃|�C���^
CPolygon *CTutorial::m_pPolygon = nullptr;		// �|���S���̃|�C���^
CScarecrow *CTutorial::m_pScarecrow = nullptr;	// �`���[�g���A���G�l�~�[�̃|�C���^

//=======================================
//�R���X�g���N�^
//=======================================
CTutorial::CTutorial()
{
}

//=======================================
//�f�X�g���N�^
//=======================================
CTutorial::~CTutorial()
{
}

//=======================================
//����
//=======================================
CTutorial *CTutorial::Create(void)
{
	//�^�C�g���̃|�C���^
	CTutorial *pTitle = nullptr;

	if (pTitle == nullptr)
	{
		//�C���X�^���X����
		pTitle = new CTutorial;

		//������
		pTitle->Init();
	}

	//�|�C���^��Ԃ�
	return pTitle;
}

//=======================================
//������
//=======================================
HRESULT CTutorial::Init(void)
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
		m_pPlayer = new CPlayer;
	}

	//�v���C���[�̏���������
	if (FAILED(m_pPlayer->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	// �`���[�g���A���G�l�~�[������
	if (m_pScarecrow == nullptr)
	{
		m_pScarecrow = new CScarecrow;
	}

	//�`���[�g���A���G�l�~�[7�̏���������
	if (FAILED(m_pScarecrow->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	// �|���S���̏�����
	if (m_pPolygon == nullptr)
	{
		m_pPolygon = new CPolygon;
	}

	//�|���S������������
	if (FAILED(m_pPolygon->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	//������Ԃ�
	return S_OK;
}

//=======================================
//�I��
//=======================================
void CTutorial::Uninit(void)
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

	if (m_pScarecrow != nullptr)
	{// �G�l�~�[�j��
		m_pScarecrow->Uninit();
		m_pScarecrow = nullptr;
	}
}

//=======================================
//�X�V
//=======================================
void CTutorial::Update(void)
{
	//CInputKeyboard�^�̃|�C���^
	CInputKeyboard *pInputKeyboard = nullptr;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	//�L�[�{�[�h�̏��擾

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CManager::GetInstance()->SetMode(CScene::MODE_RESULT);
	}

#ifdef _DEBUG
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			CManager::GetInstance()->SetMode(CScene::MODE_GAME);
		}
	}
#endif
}

//=======================================
//�`��
//=======================================
void CTutorial::Draw(void)
{
}
