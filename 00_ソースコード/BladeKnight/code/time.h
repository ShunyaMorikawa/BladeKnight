//=======================================
//
// �o�ߎ���[time.h]
// Author:�X��x��
//
//=======================================
#ifndef _TIME_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _TIME_H_     //2�A�C���N���[�h�h�~�̃}�N�����`����

//========================================
// �^�C���N���X
//========================================
class CTIME
{
public:
	void Init();
	void Update();
	float Get() { return m_fDeltaTime; }

private:
	float m_fDeltaTime;		// �f���^�^�C��

};


#endif