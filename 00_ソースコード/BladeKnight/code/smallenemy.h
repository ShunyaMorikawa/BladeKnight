//========================================
//
// �G���G�̏���[smallenemy.h]
// Author�F�X��x��
//
//========================================
#ifndef _SMALLENEMY_H_
#define _SMALLENEMY_H_

#include "enemy.h"

//========================================
// �G���G�N���X
//========================================
class CSmallEnemy : CEnemy
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CSmallEnemy();
	~CSmallEnemy();

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CSmallEnemy* Create();

private:
	// �����o�ϐ�
};

#endif
