//=======================================
//
//�G�t�F�N�g����[effect.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "effect.h"      //�쐬����effect.h���C���N���[�h����
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===========================================
//�R���X�g���N�^
//===========================================
CEffect::CEffect(int nPriority) : CBillboard(nPriority)
{
	m_aEffect.nLife = 0;		// ����(�\������)
	m_aEffect.nFirstLife = 0;	// ��������
	m_aEffect.balpha = false;	// ���Z����
}

//===========================================
//�f�X�g���N�^
//===========================================
CEffect::~CEffect()
{
}

//===========================================
//����
//===========================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha)
{
	//CEffect�^�̃|�C���^
	CEffect *pEffect = nullptr;

	//�e�N�X�`���̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (pEffect == nullptr)
	{//nullptr�̎�
		//�C���X�^���X����
		pEffect = new CEffect;

		pEffect->SetPos(pos);
		pEffect->SetMove(move);
		pEffect->SetCol(col);
		pEffect->SetSize(fRadius, fRadius);
		pEffect->m_aEffect.nLife = nLife;
		pEffect->m_aEffect.balpha = balpha;

		//������
		pEffect->Init();

		//�e�N�X�`�����蓖��
		pEffect->BindTexture(pTexture->Regist("data\\texture\\effect000.png"));
	}

	//�|�C���^��Ԃ�
	return pEffect;
}

//===========================================
//����������
//===========================================
HRESULT CEffect::Init(void)
{
	//������
	CBillboard::Init();

	// ��������
	m_aEffect.nFirstLife = m_aEffect.nLife;

	//������Ԃ�
	return S_OK;
}

//===========================================
//�I������
//===========================================
void CEffect::Uninit(void)
{
	//�I��
	CBillboard::Uninit();
}

//===========================================
//�X�V����
//===========================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXCOLOR col = GetCol();

	//�X�V
	CBillboard::Update();

	//���l(�����鑬�x)
	col.a = 0.1f;

	// �F�ݒ�
	SetCol(col);

	// �ʒu�X�V
	pos += move;

	// �ʒu�ݒ�
	SetPos(pos);

	//�̗͌��Z
	m_aEffect.nLife--;

	if (m_aEffect.nLife < 0)
	{
		//�j������
		Uninit();
	}
}

//===========================================
//�`�揈��
//===========================================
void CEffect::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_aEffect.balpha == true)
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
