//========================================
//
//�v���C���[����[player.h]
//Author�F�X��x��
//
//========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"
#include "boss.h"

//========================================
//�v���C���[�N���X
//========================================
class CPlayer : public CObject
{
public:
	CPlayer();		//�R���X�g���N�^
	~CPlayer();		//�f�X�g���N�^

	//�����o�֐�
	static CPlayer *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);

	void Act(float fSpeed);

	float RotNormalize(float RotN, float Rot);			// rot�̐��K��

	int GetLife() { return m_nLife; }					// �̗͎擾

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }		// �ʒu�擾

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾

	int GetMotionType() { return m_pMotion->GetType(); }				// ���[�V�����̎�ގ擾
	bool GetMotionLoop(int nType) { return m_pMotion->IsLoop(nType); }	// ���[�V�����̃��[�v�擾

private:
	//�����o�ϐ�

	//�ꊇ�Ǘ�
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X
	LPD3DXMESH m_pMesh;				//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;		//�}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;				//�}�e���A���̐�

	//�I�u�W�F�N�g
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//����

	CModel *m_apModel[MAX_PARTS];	//���f���ւ̃|�C���^
	CMotion *m_pMotion;		//���[�V�����̃|�C���^
	CBoss *m_pBoss;			// �{�X�̃|�C���^
	D3DXVECTOR3 m_RotDest;	// �ړI�̌���
	int m_apNumModel;		// ���f��(�p�[�c)�̑���
	int m_nLife;			// �̗�
	int m_nOldMotion;		// �O��̃��[�V����
	bool m_bMove;			// �ړ�
	bool m_bWait;			// �ҋ@
	bool m_bAttack;			// �U��
	bool m_bMowingdown;		// �؂艺�낵
	bool m_bStrongattack;	// ���U��
};

#endif
