//========================================
//
// �V���h�E����[shadow.h]
// Author�F�X��x��
//
//========================================

#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

//=======================================
//�V���h�E�N���X
//=======================================
class CShadow : public CObject3D
{
public:
	CShadow(int nPriority = 3);		//�R���X�g���N�^
	~CShadow();	//�f�X�g���N�^

	//�����o�֐�
	static CShadow* Create();
	HRESULT Init(void);		//������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��

private:
	//�����o�ϐ�
};

#endif