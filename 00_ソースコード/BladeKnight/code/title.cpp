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
#include "player.h"
#include "useful.h"

//========================================
// �萔��`
//========================================
namespace
{
	const char* TITLE_TEX = "data\\TEXTURE\\title.png";	// �^�C�g���e�N�X�`���̃p�X
}

//=======================================
//�R���X�g���N�^
//=======================================
CTitle::CTitle() :
m_pObj2D	(nullptr)	// �I�u�W�F�N�g2D�̃|�C���^
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

	//�e�N�X�`���̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// �J�����̏�����
	pCamera->Init();

	if (m_pObj2D == nullptr)
	{
	}

	// �v���C���[����
	CPlayer::Create("data//FILE//player.txt");

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

	// �T�E���h��~
	pSound->Stop();

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
