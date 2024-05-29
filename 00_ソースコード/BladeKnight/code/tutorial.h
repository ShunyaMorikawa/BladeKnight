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
class CObject2D;

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

private:
	CObject2D* m_pObj2D;
};

#endif
