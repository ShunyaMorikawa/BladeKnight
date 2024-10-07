//========================================
//
// ���U���g[result.h]
// Author�F�X��x��
//
//========================================

#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"
#include "object2D.h"
#include "renderer.h"

//========================================
// ���U���g�N���X
//========================================
class CResult : public CScene
{
public:
	enum RESULTMOTION
	{
		MOTION_NONE = 0,
		MOTION_WIN,		// ����
		MOTION_LOSE,	// �s�k
		MOTION_MAX
	};

	CResult();	//�R���X�g���N�^
	~CResult();	//�f�X�g���N�^

	//�����o�֐�
	static CResult* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObject2D* m_pWin;	// �����e�N�X�`��
	CObject2D* m_pLose;	// �s�k�e�N�X�`��
};

#endif
