//========================================
//
//�`���[�g���A��[tutorial.h]
//Author�F�X��x��
//
//========================================
#include "tutorial.h"
#include "manager.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"

//========================================
//�ÓI�����o�ϐ�
//========================================

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
CTutorial* CTutorial::Create(void)
{
	//�^�C�g���̃|�C���^
	CTutorial* pTutorial = nullptr;

	//�C���X�^���X����
	pTutorial = new CTutorial;

	//������
	pTutorial->Init();

	//�|�C���^��Ԃ�
	return pTutorial;
}

//=======================================
//������
//=======================================
HRESULT CTutorial::Init(void)
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	if (m_pObj2D == nullptr)
	{
		//CObject2D�̃|�C���^
		m_pObj2D = CObject2D::Create();

		//�ʒu�擾
		D3DXVECTOR3 pos = m_pObj2D->GetPos();

		//���_���
		m_pObj2D->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		//�|���S���̈ʒu
		pos = (D3DXVECTOR3(640.0f, 360.0f, 0.0f));

		//�ʒu�ݒ�
		m_pObj2D->SetPos(pos);

		//�e�N�X�`�����蓖��
		m_pObj2D->BindTexture(pTexture->Regist("data\\TEXTURE\\tutorial.png"));
	}

	//������Ԃ�
	return S_OK;
}

//=======================================
//�I��
//=======================================
void CTutorial::Uninit(void)
{
	if (m_pObj2D != nullptr)
	{
		m_pObj2D->Uninit();
		m_pObj2D = nullptr;
	}
}

//=======================================
//�X�V
//=======================================
void CTutorial::Update(void)
{
	//CInputKeyboard���擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad���擾
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true)
	{
		// ��ʑJ��(�t�F�[�h)
		CManager::GetInstance()->SetMode(CScene::MODE_GAME);
	}
}

//=======================================
//�`��
//=======================================
void CTutorial::Draw(void)
{
}
