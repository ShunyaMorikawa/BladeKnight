//========================================
//
//�Q�[�W[gauge.h]
//Author�F�X��x��
//
//========================================
#include "gauge.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//========================================
// ���O���
//========================================
namespace
{
	float fMax = 640.0f;
}

//========================================
// �R���X�g���N�^
//========================================
CGauge::CGauge(int nPriority) : CObject2D(nPriority)
{
	m_nMaxLife = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CGauge::~CGauge()
{
}

//========================================
// ����
//========================================
CGauge* CGauge::Create(int nMaxLife)
{
	CGauge* pGauge = new CGauge;

	// �̗͂̍ő�l
	pGauge->m_nMaxLife = nMaxLife;

	pGauge->Init();

	return pGauge;
}

//========================================
// ������
//========================================
HRESULT CGauge::Init()
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	D3DXVECTOR3 pos = GetPos();

	pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	// �̗͂̏����l
	m_nLife = m_nMaxLife;

	// �p���N���X�̏�����
	CObject2D::Init();

	// ���_���̐ݒ�
	SetVertex();

	// �ʒu�ݒ�
	CObject2D::SetPos(pos);

	// �e�N�X�`���ݒ�
	//BindTexture(pTexture->Regist("data\\texture\\tutorial.png"));

	return S_OK;
}

//========================================
// �I��
//========================================
void CGauge::Uninit()
{
	CObject2D::Uninit();
}

//========================================
// �X�V
//========================================
void CGauge::Update()
{
	CObject2D::Update();
}

//========================================
// �`��
//========================================
void CGauge::Draw()
{
	CObject2D::Draw();
}

//========================================
// ���_���̐ݒ�
//========================================
void CGauge::SetVertex(void)
{
	CObject2D::SetVertex();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR col = GetCol();

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 Vertex = GetVtxBuff();

	//���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	Vertex->Lock(0, 0, (void**)&pVtx, 0);

	float ratio = (float)m_nLife / (float)m_nMaxLife;

	pVtx[0].pos = pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fMax * ratio, 0.0f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fMax * ratio, 100.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N
	Vertex->Unlock();

	SetPos(pos);
	SetRot(rot);
}
