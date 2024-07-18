//=======================================
//
//�G�t�F�N�g����[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _ROCKONMARKER_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _ROCKONMARKER_H_     //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "billboard.h"

//===========================================
//�G�t�F�N�g�N���X
//===========================================
class CRockonMarker : public CBillboard
{
public:
	CRockonMarker(int nPriority = 5);		//�R���X�g���N�^
	~CRockonMarker();		//�f�X�g���N�^

	//�����o�֐�
	static CRockonMarker* Create(bool alpha);

	HRESULT Init(bool alpha);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����o�ϐ�
	bool m_bAlpha;
};

#endif