//========================================
//
//�G�֘A[enemy.h]
//Author�F�X��x��
//
//========================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "manager.h"
#include "character.h"

//========================================
//�G�l�~�[�N���X
//========================================
class CEnemy : public CCharacter
{
public:
	CEnemy();		//�R���X�g���N�^
	~CEnemy();		//�f�X�g���N�^

	// �����o�֐�
	static CEnemy* Create(std::string pfile);
	HRESULT Init(void) { return S_OK; }		// �������z
	HRESULT Init(std::string pfile);		// ������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��
	void Hit(int nLife);
	void DestMove();

	int GetLife() { return m_nLife; }			// �̗͎擾

	float GetRadius() { return m_fRadius; }		// ���a�擾

private:
	//�����o�ϐ�
	int m_nLife;			// �̗�
	float m_fRadius;		// ���a
};

#endif
