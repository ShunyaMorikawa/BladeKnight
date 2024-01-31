//========================================
//
// �`���[�g���A���G�l�~�[[scarecrow.h]
//Author�F�X��x��
//
//========================================
#ifndef _SCARECROW_H_
#define _SCARECROW_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"
#include "enemy.h"

//========================================
// �`���[�g���A���G�l�~�[�N���X
//========================================
class CScarecrow : public CEnemy
{
public:
	enum MOTIONTYPE
	{//���[�V�����̎��
		MOTIONTYPE_NEUTRAL = 0,	//�ҋ@���
		MOTIONTYPE_WALK,			//�������
		MOTIONTYPE_MAX
	};

	CScarecrow();		// �R���X�g���N�^
	~CScarecrow();		// �f�X�g���N�^

	// �����o�֐�
	static CScarecrow *Create();	// �`���[�g���A���G�l�~�[����

	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	float RotNormalize(float RotN, float Rot);		// rot�̐��K��

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }		// �ʒu�擾
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾

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
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	// ���f���̍ő�A�ŏ��l

	D3DXVECTOR3 m_RotDest;	// �ړI�̌���

	CModel *m_apModel[MAX_PARTS];	//���f���ւ̃|�C���^
	CMotion *m_pMotion;		//���[�V�����̃|�C���^
	float m_fAngle;			// �ړI�̌���
};

#endif