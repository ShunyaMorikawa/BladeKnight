//=======================================
//
//�t�F�[�h[fade.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "fade.h"

//=======================================
//�R���X�g���N�^
//=======================================
CFade::CFade() :
	m_State(FADE_NONE),					// �������Ȃ����
	m_modeNext(CScene::MODE_TITLE),		// �^�C�g��
	m_color(0.0f, 0.0f, 0.0f, 0.0f),	// �t�F�[�h���̐F
	m_pVtxBuff(nullptr), 
	m_pObject2D(nullptr)
{//�l�N���A
}

//=======================================
//�f�X�g���N�^
//=======================================
CFade::~CFade()
{
}

//=======================================
//������
//=======================================
HRESULT CFade::Init()
{
	//2D�I�u�W�F�N�g�̐���
	m_pObject2D = CObject2D::Create();

	// �ʒu�ݒ�
	m_pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	// �T�C�Y�ݒ�
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return S_OK;
}

//=======================================
//������
//=======================================
HRESULT CFade::Init(CScene::MODE modenext)
{
	//2D�I�u�W�F�N�g�̐���
	m_pObject2D = CObject2D::Create();

	// �ʒu�ݒ�
	m_pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	// �T�C�Y�ݒ�
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	SetFade(modenext);

	return S_OK;
}

//=======================================
//�I��
//=======================================
void CFade::Uninit()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}
}

//=======================================
//�X�V
//=======================================
void CFade::Update()
{
	if (m_State != FADE_NONE)
	{
		if (m_State == FADE_IN)
		{//�t�F�[�h���
			m_color.a -= 0.05f;
			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;

				//�������ĂȂ����
				m_State = FADE_NONE;
			}
		}
		else if (m_State == FADE_OUT)
		{
			m_color.a += 0.05f;
			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;

				//�t�F�[�h�C��
				m_State = FADE_IN;

				//���[�h�ݒ�
				CManager::GetInstance()->SetMode(m_modeNext);
			}
		}
	}

	if (m_pObject2D != nullptr)
	{
		m_pObject2D->SetVertexColor(m_color);
	}
}

//=======================================
//�`��
//=======================================
void CFade::Draw()
{
}

//=======================================
//�ݒ�
//=======================================
void CFade::SetFade(CScene::MODE modenext)
{
	// �t�F�[�h�A�E�g
	m_State = FADE_OUT;

	// ���̃��[�h�ݒ�
	m_modeNext = modenext;

	// �����|���S��(�s����)
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=======================================
// ��Ԑݒ�
//=======================================
void CFade::SetState(FADE state)
{
	m_State = state;

	switch (state)
	{
	case FADE_NONE:
		m_color.a = 0.0f;
		break;

	case FADE_IN:
		m_color.a = 1.0f;
		break;

	case FADE_OUT:
		m_color.a = 0.0f;
		break;

	default:
		break;
	}

	if (m_pObject2D != nullptr)
	{
		m_pObject2D->SetVertexColor(m_color);
	}
}

//=======================================
//����
//=======================================
CFade *CFade::Create(CScene::MODE modenext)
{
	//CFade�^�̃|�C���^
	CFade *pFade = nullptr;

	if (pFade == nullptr)
	{
		//�C���X�^���X����
		pFade = new CFade;

		//������
		pFade->Init(modenext);
		pFade->SetState(FADE_IN);
	}

	//�|�C���^��Ԃ�
	return pFade;
}
