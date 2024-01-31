//========================================
//
//�V���[�e�B���O�A�N�V����[object.h]
//Author�F�X��x��
//
//========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

//========================================
//�}�N����`
//========================================
#define MAX_DATA (100)	//�ő吔

//========================================
//�I�u�W�F�N�g�N���X
//========================================
class CObject
{
public: 
	CObject();				//�R���X�g���N�^
	virtual ~CObject();		//�f�X�g���N�^

	//�������z�֐�
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetVertex(void) = 0;
	virtual void SetSize(float fWidht, float fHeight) = 0;
	virtual void SetPosition(D3DXVECTOR3 pos) = 0;

	//�����o�֐�
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CObject *GetObject(int nIdx);
	void BindTexture(int pTexture);				//�e�N�X�`�����蓖��

	D3DXVECTOR3 Getpos(void) { return m_pos; }
	D3DXVECTOR3 Getrot(void) { return m_rot; }
	D3DXVECTOR3 GetMove() { return m_move; }
protected:
	void Release(void);

private:
	static CObject *m_apObject[MAX_DATA];
	static int m_nNumAll;	// �I�u�W�F�N�g����
	int m_nID;				// �������g��ID
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_move;		// �ړ���
	int m_nIdxTexture;		// �e�N�X�`���̔ԍ�
};

#endif
