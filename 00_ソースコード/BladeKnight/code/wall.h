//========================================
//
// �E�H�[��[wall.h]
// Author�F�X��x��
//
//========================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "object3D.h"

//========================================
// �E�H�[���N���X
//========================================
class CWall : public CObject3D
{
public:
	CWall(int nPriority = 0);		//�R���X�g���N�^
	~CWall();	//�f�X�g���N�^

	//�����o�֐�
	static CWall* Create(void);	//�|���S������

	HRESULT Init(void);		//������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��

private:
	//�����o�ϐ�
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
};

#endif