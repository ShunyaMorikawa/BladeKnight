//========================================
//
//�`���[�g���A��[tutorial.h]
//Author�F�X��x��
//
//========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "main.h"
#include "manager.h"

//========================================
//�O���錾
//========================================
class CObjectX;
class CObject3D;
class CLight;
class CPlayer;
class CPolygon;
class CGame_Object;
class CScarecrow;

//========================================
//�`���[�g���A���N���X
//========================================
class CTutorial : public CScene
{
public:
	CTutorial();	//�R���X�g���N�^
	~CTutorial();	//�f�X�g���N�^

	//�����o�֐�
	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTutorial *GetTutorial(void) { return m_pTutorial; }		// �`���[�g���A���̏��
	static CPlayer *GetPlayer(void) { return m_pPlayer; }			// �v���C���[�̏��
	static CScarecrow *GetScarecrow(void) { return m_pScarecrow; }	// �ĎR�q�̏��

private:
	static CObject *m_pObject[MAX_DATA];		//�I�u�W�F�N�g�̃|�C���^
	static CObjectX *m_pObjectX;		//X�t�@�C���I�u�W�F�N�g�̃|�C���^
	static CObject3D *m_pObject3D;		//�I�u�W�F�N�g3D�̃|�C���^
	static CTutorial *m_pTutorial;		//�Q�[���}�l�[�W���[�̃|�C���^
	static CLight *m_pLight;			//���C�g�̃|�C���^
	static CPlayer *m_pPlayer;			//�v���C���[�̃|�C���^
	static CPolygon *m_pPolygon;		//�|���S���̃|�C���^
	static CScarecrow *m_pScarecrow;	// �`���[�g���A���G�l�~�[�|�C���^
};

#endif
