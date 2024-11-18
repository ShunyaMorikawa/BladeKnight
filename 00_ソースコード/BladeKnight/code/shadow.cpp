//========================================
//
// �V���h�E����[shadow.h]
// Author�F�X��x��
//
//========================================

#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"


//========================================
// �R���X�g���N�^
//========================================
CShadow::CShadow(int nPriority)
{
}

//========================================
// �f�X�g���N�^
//========================================
CShadow::~CShadow()
{
}

//========================================
// ����
//========================================
CShadow* CShadow::Create()
{
	// �C���X�^���X����
	CShadow* pShadow = new CShadow;

	// ������
	pShadow->Init();

	return pShadow;
}

//========================================
// ������
//========================================
HRESULT CShadow::Init(void)
{
	// �p���N���X�̏�����
	CObject3D::Init();

	return S_OK;
}

//========================================
// �I��
//========================================
void CShadow::Uninit(void)
{
	// �p���N���X�̏I��
	CObject3D::Uninit();
}

//========================================
// �X�V
//========================================
void CShadow::Update(void)
{
	// �p���N���X�̍X�V
	CObject3D::Update();
}

//========================================
// �`��
//========================================
void CShadow::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �p���N���X�̕`��
	CObject3D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
