//========================================
//
//�^�C�g���v���C���[����[titleplayer.h]
//Author�F�X��x��
//
//========================================

#ifndef _TITLEPLAYER_H_
#define _TITLEPLAYER_H_

#include "character.h"

//========================================
// �O���錾
//========================================
class CModel;
class CMotion;

//========================================
//�v���C���[�N���X
//========================================
class CTitlePlayer : public CCharacter
{
public:
	// ���
	enum PLAYERSTATE
	{// �v���C���[�̏��
		STATE_NONE = 0,
		STATE_NORMAL,		// �ʏ�
		STATE_MAX
	};

	CTitlePlayer(int nPriority = 4);		//�R���X�g���N�^
	~CTitlePlayer();		//�f�X�g���N�^

	// �����o�֐�
	static CTitlePlayer* Create(std::string pfile);

	HRESULT Init() { return S_OK; }		//�������z
	HRESULT Init(std::string pfile);
	void Uninit();
	void Update();
	void Draw();

	static CTitlePlayer* GetInstance() { return m_pTitlePlayer; }

private:
	//�����o�ϐ�

	int m_apNumModel;		// ���f��(�p�[�c)�̑���
	int m_nOldMotion;		// �O��̃��[�V����

	int m_nState;			// ���

	bool m_bMove;			// �ړ�

	CModel* m_apModel[MAX_PARTS];	// ���f���̃_�u���|�C���^

	static CTitlePlayer* m_pTitlePlayer;		// ���g�̃|�C���^
};

#endif
