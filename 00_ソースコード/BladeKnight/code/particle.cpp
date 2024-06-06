//
//
//�p�[�e�B�N������[particle.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //�쐬����main.h���C���N���[�h����
#include "particle.h"    //�쐬����effect.h���C���N���[�h����
#include "effect.h"

//=======================================
// ���O���
//=======================================
namespace
{
	D3DXVECTOR3 POS;	// �ʒu
}

//=======================================
// ����
//=======================================
void Myparticle::Create(TYPE nType, D3DXVECTOR3 pos)
{
	POS = pos;

	(*(ParticleList[nType]))();
}

//=======================================
// ���s���̃p�[�e�B�N��
//=======================================
void Mypartcile_Func::WALK_PARTICLE()
{
	int life = 60;		// ����
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.7f, 0.6f, 1.0f);	// �F
	const int max = 10;		// �ő吔

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �ړ��ʂ̊�l
		float StandardMove = 1.0f + (rand() % 10) * 0.9f;

		// ���a�̊�l
		float StandardRadius = 9.0f + (rand() % 50) * 0.9f;

		// �傫��
		move.x = sinf((rand() % 629 - 314) / 10.0f) * StandardMove;

		CEffect::Create(POS, move, col, StandardRadius, life, false);
	}
}

//=======================================
// ���S���̃p�[�e�B�N��
//=======================================
void Mypartcile_Func::DEATH_PARTICLE()
{
	int life = 60;		// ����
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �F
	const int max = 32;		// �ő吔

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �ړ��ʂ̊�l
		float StandardMove = 9.0f + (rand() % 100) * 0.1f;

		// ���a�̊�l
		float StandardRadius = 9.0f + (rand() % 100) * 0.9f;

		// �傫��
		move.x = sinf((rand() % 629 - 314) / 100.0f) * StandardMove;
		move.y = sinf((rand() % 629 - 314) / 150.0f) * StandardMove;

		// �G�t�F�N�g����
		CEffect::Create(POS, move, col, StandardRadius, life, true);
	}
}
