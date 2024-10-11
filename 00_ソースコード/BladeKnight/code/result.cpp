//========================================
//
// ���U���g[result.h]
// Author�F�X��x��
//
//========================================

#include "result.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "field.h"
#include "mapobject.h"
#include "wall.h"
#include "useful.h"
#include "resultplayer.h"

//========================================
// �萔��`
//========================================
namespace
{
	const std::string RESULT_PLAYER_TXT = "data\\FILE\\resultplayer.txt";	// �v���C���[���̃p�X
	const char* WIN_TEX = "data\\TEXTURE\\title.png";			// �����e�N�X�`���̃p�X
	const char* LOSE_TEX = "data\\TEXTURE\\press_enter.png";	// �s�k�e�N�X�`���̃p�X

	const float LOGO_POS_Y = 150.0f;		// ���S�e�N�X�`����Y���W
	const float LOGO_SIZE_WIDTH = 640.0f;	// ���S�e�N�X�`���̃T�C�Y
	const float LOGO_SIZE_HEIGHT = 250.0f;	// ���S�e�N�X�`���̃T�C�Y
	const float ENTER_POS_Y = 550.0f;		// �����e�N�X�`����Y���W
	const float ENTER_SIZE = 250.0f;		// �����e�N�X�`���̃T�C�Y
}

//========================================
// �ÓI�����o�ϐ�
//========================================
bool CResult::m_bResult = false;	// ���U���g�t���O
CResult* CResult::m_pResult = nullptr;

//=======================================
//�R���X�g���N�^
//=======================================
CResult::CResult() : 
m_pWin		(nullptr),	// �����e�N�X�`��
m_pLose		(nullptr)	// �s�k�e�N�X�`��
{
	
}

//=======================================
//�f�X�g���N�^
//=======================================
CResult::~CResult()
{
}

//=======================================
//�V���O���g��
//=======================================
CResult* CResult::GetInstance()
{
	if (m_pResult == nullptr)
	{//�C���X�^���X����
		return m_pResult = new CResult;
	}
	else
	{//�|�C���^��Ԃ�
		return m_pResult;
	}
}

//=======================================
//����
//=======================================
CResult* CResult::Create(void)
{
	if (m_pResult == nullptr)
	{
		//�C���X�^���X����
		m_pResult = new CResult;

		//������
		m_pResult->Init();
	}

	//�|�C���^��Ԃ�
	return m_pResult;
}

//=======================================
//������
//=======================================
HRESULT CResult::Init(void)
{
	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �J�����̏�����
	pCamera->Init();

	// ���U���g�v���C���[����
	CResultPlayer::Create(RESULT_PLAYER_TXT);

	// �}�b�v�I�u�W�F�N�g����
	CMapObject::Create();

	// �t�B�[���h����
	CField::Create();

	// 4�����ɕǐ���
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, -Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

	//������Ԃ�
	return S_OK;
}

//=======================================
//�I��
//=======================================
void CResult::Uninit(void)
{
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h��~
	pSound->Stop();

	if (m_pResult != nullptr)
	{//���[�h�j��
		delete m_pResult;
		m_pResult = nullptr;
	}
}

//=======================================
//�X�V
//=======================================
void CResult::Update(void)
{
	//CInputKeyboard�^�̃|�C���^
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad�^�̃|�C���^
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// ���U���g�J����
	pCamera->Result();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{
		// ��ʑJ��(�t�F�[�h)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}
}

//=======================================
//�`��
//=======================================
void CResult::Draw(void)
{
}
