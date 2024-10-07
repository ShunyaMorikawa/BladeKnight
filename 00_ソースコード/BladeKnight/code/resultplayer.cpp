//========================================
//
// �^�C�g���v���C���[����[titleplayer.h]
// Author�F�X��x��
//
//========================================

#include "resultplayer.h"
#include "debugproc.h"

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
CResultPlayer* CResultPlayer::m_pResultPlayer = nullptr;

//========================================
// �R���X�g���N�^
//========================================
CResultPlayer::CResultPlayer(int nPriority) : CCharacter(nPriority),
m_apNumModel	(0),			// ���f���̑���
m_nOldMotion	(0),			// �O��̃��[�V����
m_nState		(STATE_NONE)	// ���
{//�l���N���A
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//���f�����
}

//========================================
// �f�X�g���N�^
//========================================
CResultPlayer::~CResultPlayer()
{
}

//========================================
// �^�C�g���v���C���[����
//========================================
CResultPlayer* CResultPlayer::Create(std::string pfile)
{
	if (m_pResultPlayer == nullptr)
	{
		//�v���C���[����
		m_pResultPlayer = new CResultPlayer;

		//������
		m_pResultPlayer->Init(pfile);
	}

	//�|�C���^��Ԃ�
	return m_pResultPlayer;
}

//========================================
// ������
//========================================
HRESULT CResultPlayer::Init(std::string pfile)
{
	// �L�����̏�����
	CCharacter::Init(pfile);

	// �v���C���[��Ԃ̏�����
	m_nState = STATE_NONE;

	// �ʒu�ݒ�
	SetPos(INITIAL_POS);

	// �����ݒ�
	SetRot(INITIAL_ROT);

	return S_OK;
}

//========================================
//�I��
//========================================
void CResultPlayer::Uninit(void)
{
	// �I��
	CCharacter::Uninit();
	m_pResultPlayer = nullptr;
}

//========================================
//�X�V
//========================================
void CResultPlayer::Update(void)
{
	// �ʒu�E�ړ��ʁE�����擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRot();

	// ���[�V����
	Motion();

	// �f�o�b�O�\���̏��擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// �f�o�b�O�\��
	pDebugProc->Print("\n�v���C���[�̈ʒu�F%f�A%f�A%f\n", pos.x, pos.y, pos.z);
}

//========================================
//�`��
//========================================
void CResultPlayer::Draw(void)
{
	// �`��
	CCharacter::Draw();
}

//========================================
// ���[�V�����Ǘ�
//========================================
void CResultPlayer::Motion()
{
	// ���[�V�������擾
	CMotion* pMotion = GetMotion();

	// �^�C�g�����[�V����
	pMotion->Set(CMotion::PLAYER_MOTIONTYPE_NEUTRAL);

	if (pMotion != nullptr)
	{// ���[�V�����X�V
		pMotion->Update();
	}
}
