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
	// �R���X�g���N�^�E�f�X�g���N�^
	CSmallEnemy();
	~CSmallEnemy();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

};

#endif
