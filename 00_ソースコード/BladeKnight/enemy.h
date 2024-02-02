//========================================
//
//�G�l�~�[����[player.h]
//Author�F�X��x��
//
//========================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"

//========================================
//�G�l�~�[�N���X
//========================================
class CEnemy : public CObject
{
public:
	CEnemy();		//�R���X�g���N�^
	~CEnemy();		//�f�X�g���N�^

	virtual HRESULT Init(void) = 0;		//������
	virtual void Uninit(void) = 0;		//�I��
	virtual void Update(void) = 0;		//�X�V
	virtual void Draw(void) = 0;		//�`��

private:
	//�����o�ϐ�

	//�ꊇ�Ǘ�
	LPDIRECT3DTEXTURE9 m_pTexture;	//���L�e�N�X�`��
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X
	LPD3DXMESH m_pMesh;				//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;		//�}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;				//�}�e���A���̐�

	//�I�u�W�F�N�g
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	//���f���̍ő�A�ŏ��l

	CModel *m_apModel[MAX_PARTS];	//���f���ւ̃|�C���^
	CMotion *m_pMotion;		//���[�V�����̃|�C���^
	D3DXVECTOR3 m_RotDest;	//�ړI�̌���
	int m_apNumModel;		//���f��(�p�[�c)�̑���
	int m_nLife;			// �̗�
	int m_nOldMotion;		// �O��̃��[�V����
	bool m_bJump;			//�W�����v
	bool m_bMove;			//�ړ�
	bool m_bWait;			//�ҋ@
};

#endif
