//========================================
//
//�V���[�e�B���O�A�N�V����[polygon.cpp]
//Author�F�X��x��
//
//========================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
//�}�N����`
//========================================
#define POLYGON_TEX		"data\\texture\\soil.jpg"

//========================================
//�ÓI�����o�ϐ�
//========================================
LPDIRECT3DTEXTURE9 CField::m_pTexture = nullptr;

//========================================
//�R���X�g���N�^
//========================================
CField::CField(int nPriority) :
	CObject3D(nPriority)
{
}

//========================================
//�f�X�g���N�^
//========================================
CField::~CField()
{
}

//========================================
//�|���S������
//========================================
CField *CField::Create(void)
{
	//CPlayer�^�̃|�C���^
	CField *pPolygon = nullptr;

	if (pPolygon == nullptr)
	{
		//�v���C���[����
		pPolygon = new CField;

		//������
		pPolygon->Init();
	}

	//�|�C���^��Ԃ�
	return pPolygon;
}

//========================================
//������
//========================================
HRESULT CField::Init(void)
{
	//�e�N�X�`���̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (m_pObj3D == nullptr)
	{
		//CObject2D�̃|�C���^
		m_pObj3D = CObject3D::Create();

		//�e�N�X�`�����蓖��
		m_pObj3D->BindTexture(pTexture->Regist(POLYGON_TEX));
	}

	return S_OK;
}

//========================================
//�I��
//========================================
void CField::Uninit(void)
{
}

//========================================
//�X�V
//========================================
void CField::Update(void)
{
}

//========================================
//�`��
//========================================
void CField::Draw(void)
{
	//�`��
	CObject3D::Draw();
}