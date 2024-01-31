//========================================
//
// �{�X[boss.h]
//Author�F�X��x��
//
//========================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"
#include "enemy.h"

//========================================
// �{�X�N���X
//========================================
class CBoss : public CEnemy
{
public:
	enum MOTIONTYPE
	{//���[�V�����̎��
		MOTIONTYPE_NEUTRAL = 0,		// �ҋ@���
		MOTIONTYPE_WALK,			// �������
		MOTIONTYPE_ATTACK,			// �U��
		MOTIONTYPE_MAX
	};

	CBoss();		// �R���X�g���N�^
	~CBoss();		// �f�X�g���N�^

	// �����o�֐�
	static CBoss *Create();	// �`���[�g���A���G�l�~�[����

	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	void Move(float fSpeed);

	float RotNormalize(float RotN, float Rot);		// rot�̐��K��

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾
	void SetPosition(D3DXVECTOR3 pos);
private:
	//�����o�ϐ�

	//�ꊇ�Ǘ�
	LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`��
	int m_nIdxTexture;				// �e�N�X�`���̔ԍ�
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	LPD3DXMESH m_pMesh;				// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;		// �}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;				// �}�e���A���̐�

	//�I�u�W�F�N�g
	D3DXVECTOR3 m_pos;		// ����
	D3DXVECTOR3 m_move;		// ����
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	// ���f���̍ő�A�ŏ��l

	D3DXVECTOR3 m_RotDest;	// �ړI�̌���

	CModel *m_apModel[MAX_PARTS];	//���f���ւ̃|�C���^
	CMotion *m_pMotion;		//���[�V�����̃|�C���^
	float m_fAngle;			// �ړI�̌���
	bool m_bWalk;			// �ړ�
	bool m_bAttack;			// �U��
};

#endif