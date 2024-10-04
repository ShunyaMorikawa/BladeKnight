//========================================
//
//�^�C�g��[title.cpp]
//Author�F�X��x��
//
//========================================

#include "title.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "field.h"
#include "mapobject.h"
#include "wall.h"
#include "titleplayer.h"
#include "useful.h"

//========================================
// �萔��`
//========================================
namespace
{
	const std::string PLAYER_TXT = "data//FILE//player.txt";	// �v���C���[���̃p�X
	const char* TITLE_TEX = "data\\TEXTURE\\title.png";			// �^�C�g���e�N�X�`���̃p�X
	const char* ENTER_TEX = "data\\TEXTURE\\press_enter.png";	// �����e�N�X�`���̃p�X

	const float LOGO_POS_Y = 150.0f;		// ���S�e�N�X�`����Y���W
	const float LOGO_SIZE_WIDTH = 640.0f;	// ���S�e�N�X�`���̃T�C�Y
	const float LOGO_SIZE_HEIGHT = 250.0f;	// ���S�e�N�X�`���̃T�C�Y
	const float ENTER_POS_Y = 550.0f;		// �����e�N�X�`����Y���W
	const float ENTER_SIZE = 250.0f;		// �����e�N�X�`���̃T�C�Y
}

//=======================================
//�R���X�g���N�^
//=======================================
CTitle::CTitle() :
m_pObj2D	(nullptr),	// �I�u�W�F�N�g2D�̃|�C���^
m_pTitleObj	(nullptr)	// PressEnter�����p�̃|�C���^
{
}

//=======================================
//�f�X�g���N�^
//=======================================
CTitle::~CTitle()
{
}

//=======================================
//����
//=======================================
CTitle* CTitle::Create(void)
{
	//�^�C�g���̃|�C���^
	CTitle* pTitle = nullptr;

	//�C���X�^���X����
	pTitle = new CTitle;

	//������
	pTitle->Init();

	//�|�C���^��Ԃ�
	return pTitle;
}

//=======================================
//������
//=======================================
HRESULT CTitle::Init(void)
{
	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̏�����
	pCamera->Init();

	// �^�C�g�����S
	TitleLogo();

	// PressEnter����
	PressEnter();

	// �^�C�g���v���C���[�ǂݍ���
	CTitlePlayer::Create(PLAYER_TXT);

	// �}�b�v�I�u�W�F�N�g����
	CMapObject::Create();

	// �t�B�[���h����
	CField::Create();
	
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// 4�����ɕǐ���
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, -Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(0.0f, Constance::WALL_POS_Y, Constance::WALL_POS), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
	CWall::Create(D3DXVECTOR3(Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0.0f));
	CWall::Create(D3DXVECTOR3(-Constance::WALL_POS, Constance::WALL_POS_Y, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

	// �T�E���h�Đ�
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);

	//������Ԃ�
	return S_OK;
}

//=======================================
//�I��
//=======================================
void CTitle::Uninit(void)
{
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h��~
	pSound->Stop();
}

//=======================================
//�X�V
//=======================================
void CTitle::Update(void)
{
	//CInputKeyboard�^�̃|�C���^
	CInputKeyboard *pInputKeyboard = pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	
	//CInputPad�^�̃|�C���^
	CInputPad *pInputPad = pInputPad = CManager::GetInstance()->GetInputPad();
	
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �^�C�g���J����
	pCamera->TitleCamera();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || 
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		// ��ʑJ��(�t�F�[�h)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TUTORIAL);
	}
}

//=======================================
//�`��
//=======================================
void CTitle::Draw(void)
{
}

//=======================================
// �^�C�g�����S
//=======================================
void CTitle::TitleLogo()
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �C���X�^���X����
	m_pObj2D = CObject2D::Create();

	// �ʒu�ݒ�
	m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, LOGO_POS_Y, 0.0f));

	// �T�C�Y�ݒ�
	m_pObj2D->SetSize(LOGO_SIZE_WIDTH, LOGO_SIZE_HEIGHT);

	// �e�N�X�`���ݒ�
	m_pObj2D->BindTexture(pTexture->Regist(TITLE_TEX));
}

//=======================================
// PressEnter����
//=======================================
void CTitle::PressEnter()
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �C���X�^���X����
	m_pTitleObj = CObject2D::Create();

	// �ʒu�ݒ�
	m_pTitleObj->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, ENTER_POS_Y, 0.0f));

	// �T�C�Y�ݒ�
	m_pTitleObj->SetSize(ENTER_SIZE, ENTER_SIZE);

	// �e�N�X�`���ݒ�
	m_pTitleObj->BindTexture(pTexture->Regist(ENTER_TEX));
}
