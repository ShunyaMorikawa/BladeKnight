//=======================================
//
// ���b�N���}�[�J�[[rockonMarker.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "rockonMarker.h"      //�쐬����effect.h���C���N���[�h����
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===========================================
// �萔��`
//===========================================
namespace
{
const float MARKERSIZE = 25.0f;		// �}�[�J�[�̃T�C�Y
const std::string TEXTURE = "data\\TEXTURE\\effect\\effect000.jpg";	// �}�[�J�[�̃e�N�X�`��
}

//===========================================
//�R���X�g���N�^
//===========================================
CRockonMarker::CRockonMarker(int nPriority) : CBillboard(nPriority)
{
}

//===========================================
//�f�X�g���N�^
//===========================================
CRockonMarker::~CRockonMarker()
{
}

//===========================================
//����
//===========================================
CRockonMarker *CRockonMarker::Create(bool alpha)
{
	//CRockonMarker�^�̃|�C���^
	CRockonMarker *pRockonMarker = nullptr;

	if (pRockonMarker == nullptr)
	{//nullptr�̎�
		//�C���X�^���X����
		pRockonMarker = new CRockonMarker;

		//������
		pRockonMarker->Init(alpha);
	}

	//�|�C���^��Ԃ�
	return pRockonMarker;
}

//===========================================
//����������
//===========================================
HRESULT CRockonMarker::Init(bool alpha)
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//������
	CBillboard::Init();

	// �T�C�Y�ݒ�
	SetSize(MARKERSIZE, MARKERSIZE);

	//�e�N�X�`�����蓖��
	BindTexture(pTexture->Regist(TEXTURE));

	m_bAlpha = alpha;

	//������Ԃ�
	return S_OK;
}

//===========================================
//�I������
//===========================================
void CRockonMarker::Uninit(void)
{
	//�I��
	CBillboard::Uninit();
}

//===========================================
//�X�V����
//===========================================
void CRockonMarker::Update(void)
{
	//�X�V
	CBillboard::Update();
}

//===========================================
//�`�揈��
//===========================================
void CRockonMarker::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_bAlpha == true)
	{// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�`��
	CBillboard::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
