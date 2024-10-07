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
#include "titleplayer.h"
#include "useful.h"

//========================================
// �萔��`
//========================================
namespace
{
	const std::string PLAYER_TXT = "data\\FILE\\titleplayer.txt";	// �v���C���[���̃p�X
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
CResult::CResult()
{
}

//=======================================
//�f�X�g���N�^
//=======================================
CResult::~CResult()
{
}

//=======================================
//����
//=======================================
CResult* CResult::Create(void)
{
	//�^�C�g���̃|�C���^
	CResult* pResult = nullptr;

	//�C���X�^���X����
	pResult = new CResult;

	//������
	pResult->Init();

	//�|�C���^��Ԃ�
	return pResult;
}

//=======================================
//������
//=======================================
HRESULT CResult::Init(void)
{
	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̏�����
	pCamera->Init();

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
void CResult::Uninit(void)
{
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h��~
	pSound->Stop();
}

//=======================================
//�X�V
//=======================================
void CResult::Update(void)
{
	//CInputKeyboard�^�̃|�C���^
	CInputKeyboard* pInputKeyboard = pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad�^�̃|�C���^
	CInputPad* pInputPad = pInputPad = CManager::GetInstance()->GetInputPad();

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
