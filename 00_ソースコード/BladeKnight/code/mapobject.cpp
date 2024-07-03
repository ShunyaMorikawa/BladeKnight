//========================================
//
// �}�b�v�I�u�W�F�N�g[mapobject.cpp]
// Author�F�X��x��
//
//========================================
#include "mapobject.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// �萔��`
//========================================
namespace
{
	const char* OBJECT_PASS = "data\\MODEL\\stage\\tougijo_001.x";	// �}�b�v�I�u�W�F�N�g�̃p�X
}

//========================================
// �R���X�g���N�^
//========================================
CMapObject::CMapObject(int nPriority) :
	m_pModel(nullptr)
{
}

//========================================
// �f�X�g���N�^
//========================================
CMapObject::~CMapObject()
{
}

//========================================
// �}�b�v�I�u�W�F�N�g����
//========================================
CMapObject* CMapObject::Create()
{
	// CMapObject�^�̃|�C���^
	CMapObject* pMapObject = new CMapObject;

	pMapObject->Init();

	//�|�C���^��Ԃ�
	return pMapObject;
}

//========================================
// ������
//========================================
HRESULT CMapObject::Init(void)
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	if (m_pModel == nullptr)
	{
		m_pModel = new CModel;
	}

	if (m_pModel != nullptr)
	{
		m_pModel->Init(OBJECT_PASS);
	}

	return S_OK;
}

//========================================
// �I��
//========================================
void CMapObject::Uninit(void)
{
	if (m_pModel != nullptr)
	{
		m_pModel->Uninit();
		m_pModel = nullptr;
	}

	// ���g�̏I��
	Release();
}

//========================================
// �X�V
//========================================
void CMapObject::Update(void)
{
	if (m_pModel != nullptr)
	{
		m_pModel->Update();
	}
}

//========================================
// �`��
//========================================
void CMapObject::Draw(void)
{
	if (m_pModel != nullptr)
	{
		m_pModel->Draw();
	}
}
