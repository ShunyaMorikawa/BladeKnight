//========================================
//
//���U���g�v���C���[����[resultplayer.h]
//Author�F�X��x��
//
//========================================

#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "character.h"

//========================================
// �O���錾
//========================================
class CModel;
class CMotion;

//========================================
//�v���C���[�N���X
//========================================
class CResultPlayer : public CCharacter
{
public:
	// ���
	enum PLAYERSTATE
	{// �v���C���[�̏��
		STATE_NONE = 0,
		STATE_NORMAL,		// �ʏ�
		STATE_MAX
	};

	CResultPlayer(int nPriority = 4);		//�R���X�g���N�^
	~CResultPlayer();		//�f�X�g���N�^

	// �����o�֐�
	static CResultPlayer* Create(std::string pfile);

	HRESULT Init() { return S_OK; }		//�������z
	HRESULT Init(std::string pfile);
	void Uninit();
	void Update();
	void Draw();
	void Motion();

	// ���g�̏��
	static CResultPlayer* GetInstance() { return m_pResultPlayer; }

private:

	//�����o�ϐ�
	int m_apNumModel;		// ���f��(�p�[�c)�̑���
	int m_nOldMotion;		// �O��̃��[�V����

	PLAYERSTATE m_nState;	// ���

	bool m_bWait;			// �ҋ@

	CModel* m_apModel[MAX_PARTS];	// ���f���̃_�u���|�C���^

	static CResultPlayer* m_pResultPlayer;		// ���g�̃|�C���^
};

#endif
