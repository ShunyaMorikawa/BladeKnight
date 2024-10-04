//========================================
//
// �^�C�g���v���C���[����[titleplayer.h]
// Author�F�X��x��
//
//========================================

#include "titleplayer.h"

//========================================
// �萔��`
//========================================
namespace
{
	const D3DXVECTOR3 INITIAL_POS = { 0.0f, 0.0f, 50.0f };	// �v���C���[�����ʒu
	const D3DXVECTOR3 INITIAL_ROT = { 0.0f, 0.0f, 0.0f };	// �v���C���[��������
}

//========================================
// �ÓI�����o�ϐ�
//========================================
CTitlePlayer* CTitlePlayer::m_pTitlePlayer = nullptr;

//========================================
// �R���X�g���N�^
//========================================
CTitlePlayer::CTitlePlayer(int nPriority) : CCharacter(nPriority),
m_apNumModel	(0),			// ���f���̑���
m_nOldMotion	(0),			// �O��̃��[�V����
m_nState		(STATE_NONE),	// ���
m_bMove			(false)			// �ړ�
{//�l���N���A
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//���f�����
}

//========================================
// �f�X�g���N�^
//========================================
CTitlePlayer::~CTitlePlayer()
{
}

//========================================
// �^�C�g���v���C���[����
//========================================
CTitlePlayer* CTitlePlayer::Create(std::string pfile)
{
	if (m_pTitlePlayer == nullptr)
	{
		//�v���C���[����
		m_pTitlePlayer = new CTitlePlayer;

		//������
		m_pTitlePlayer->Init(pfile);
	}

	//�|�C���^��Ԃ�
	return m_pTitlePlayer;
}

//========================================
// ������
//========================================
HRESULT CTitlePlayer::Init(std::string pfile)
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �L�����̏�����
	CCharacter::Init(pfile);

	// �v���C���[��Ԃ̏�����
	m_nState = STATE_NORMAL;

	// �ʒu�ݒ�
	SetPos(INITIAL_POS);

	// �����ݒ�
	SetRot(INITIAL_ROT);

	return S_OK;
}

//========================================
//�I��
//========================================
void CTitlePlayer::Uninit(void)
{
	// �I��
	CCharacter::Uninit();
	m_pTitlePlayer = nullptr;
}

//========================================
//�X�V
//========================================
void CTitlePlayer::Update(void)
{
}

//========================================
//�`��
//========================================
void CTitlePlayer::Draw(void)
{
	// �`��
	CCharacter::Draw();
}
