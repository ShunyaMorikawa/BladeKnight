//=======================================
//
//�G�t�F�N�g����[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EFFECT_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _EFFECT_H_     //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "billboard.h"

//===========================================
//�}�N����`
//===========================================
#define MAX_EFFECT		(30)		//�G�t�F�N�g�̍ő吔

//===========================================
//�G�t�F�N�g�N���X
//===========================================
class CEffect : public CBillboard
{
public:
	// �\���̂̒�`
	struct Effect
	{
		int nLife;			// ����(�\������)
		int nFirstLife;		// ��������
		bool balpha;			//���Z����
	};

	CEffect(int nPriority = 5);		//�R���X�g���N�^
	~CEffect();		//�f�X�g���N�^

	//�����o�֐�
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//�����o�ϐ�
	Effect m_aEffect;	// �\���̂̏��
};

#endif