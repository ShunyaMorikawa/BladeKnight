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
	CResult();	//�R���X�g���N�^
	~CResult();	//�f�X�g���N�^

	//�����o�֐�
	static CResult* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObject2D* m_pObj2D;
	CObject2D* m_pTitleObj;
};

#endif
