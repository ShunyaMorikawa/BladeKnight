//========================================
//
//�Q�[�����I�u�W�F�N�g[gameobject.h]
//Author�F�X��x��
//
//========================================
#include "gameobject.h"

//========================================
//�R���X�g���N�^
//========================================
CGameObject::CGameObject() :
	m_pObjX(nullptr),
	m_nNumModel(0)
{
}

//========================================
//�f�X�g���N�^
//========================================
CGameObject::~CGameObject()
{
}

//========================================
//������
//========================================
HRESULT CGameObject::Init(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{//�_�u���|�C���^�̏�����
		m_ppModel[nCntModel] = nullptr;
	}

	//���f���̑���
	m_nNumModel = 0;

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CGameObject::Uninit()
{
	//�I��
	CModel::Uninit();
}

//========================================
//�X�V
//========================================
void CGameObject::Update()
{
}

//========================================
//�`��
//========================================
void CGameObject::Draw()
{
	for (int nCntParts = 0; nCntParts < GAME_OBJ; nCntParts++)
	{//���f���̕`��(�S�p�[�c)
		if (m_ppModel[nCntParts] != nullptr)
		{//m_apModel��nullptr�̎�
			m_ppModel[nCntParts]->Draw();
		}
	}
}

//========================================
//����
//========================================
CGameObject *CGameObject::Create(void)
{
	//CGame_Object�^�̃|�C���^
	CGameObject *pGameObject = nullptr;

	if (pGameObject == nullptr)
	{
		//�C���X�^���X����
		pGameObject = new CGameObject;

		//������
		pGameObject->Init();
	}

	//�|�C���^��Ԃ�
	return pGameObject;
}

//========================================
//�I�u�W�F�N�g�ݒ�
//========================================
void CGameObject::SetObject(CModel * ppModel, int nNumModel)
{
	//�|�C���^
	m_ppModel[nNumModel] = ppModel;

	//�p�[�c�ԍ�
	m_nNumModel++;
}

//========================================
//�t�@�C���ǂݍ���
//========================================
void CGameObject::LoadObj(char * pFilePath)
{
	//�ϐ��錾
	char garbage[640];		//�S�~�i�[�p
	char FileName[32];		//�e���f���̃t�@�C����
	int nNumParts = 0;		//�p�[�c��
	int nNum = 0;				//�ǂݍ��ޔԍ�
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);		//�ʒu
	D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);		//����

	//FILE�̃|�C���^
	FILE *pFile = nullptr;

	//�t�@�C�����J��
	pFile = fopen(pFilePath, "r");

	while (1)
	{//END_SCRIPT������܂ŌJ��Ԃ�
	 //�s�v�ȕ������̂Ă�
		fscanf(pFile, "%s", &garbage[0]);

		if (strcmp(&garbage[0], "END_SCRIPT") == 0)
		{//������END_SCRIPT�̂Ƃ�
		 //�����o��
			break;
		}

		//========================================
		//�I�u�W�F�N�g�t�@�C���̓ǂݍ���
		//========================================
		if (strcmp(&garbage[0], "OBJECT_FILENAME") == 0)
		{//������MODEL_FILENAME�̂Ƃ�
			//=�̂Ă�
			fscanf(pFile, "%s", &garbage[0]);

			//�t�@�C���̖��O�擾
			fscanf(pFile, "%s", &FileName[0]);

			//���f���̐���
			m_ppModel[nNum] = CModel::Create(&FileName[0]);

			//���f���ݒ�
			SetObject(m_ppModel[nNum], nNum);

			//���f���̔ԍ������Z����
			nNum++;
		}

		//========================================
		//�I�u�W�F�N�g�ݒ�
		//========================================
		if (strcmp(&garbage[0], "OBJECTSET") == 0)
		{//������CHARACTERSET�̂Ƃ�
			while (1)
			{//������END_CHARACTERSET�ɂȂ�܂ŌJ��Ԃ�
				fscanf(pFile, "%s", &garbage[0]);

				if (strcmp(&garbage[0], "END_OBJECTSET") == 0)
				{//������END_CHARACTERSET�̎�
				 //�����o��
					break;
				}

				if (strcmp(&garbage[0], "NUM_PARTS") == 0)
				{//������NUM_PARTS�̎�
				 //=�̂Ă�
					fscanf(pFile, "%s", &garbage[0]);

					//�p�[�c��������
					fscanf(pFile, "%d", &nNumParts);
				}

				if (strcmp(&garbage[0], "OBJECTSET") == 0)
				{//������PARTSSET�̎�
					while (1)
					{//END_PARTSSET������܂ŉ�
						fscanf(pFile, "%s", &garbage[0]);

						if (strcmp(&garbage[0], "END_OBJECTSET") == 0)
						{//������END_PARTSSET�̎�
						 //�p�[�c�������Z

						 //�����o��
							break;
						}

					}

					if (strcmp(&garbage[0], "POS") == 0)
					{//������PARENT�̎�
					 //=�̂Ă�
						fscanf(pFile, "%s", &garbage[0]);

						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);

						//�ʒu��ݒ�
					}

					if (strcmp(&garbage[0], "ROT") == 0)
					{//������PARENT�̎�
					 //=�̂Ă�
						fscanf(pFile, "%s", &garbage[0]);

						fscanf(pFile, "%f", &rot.x);
						fscanf(pFile, "%f", &rot.y);
						fscanf(pFile, "%f", &rot.z);

						//������ݒ�
					}
				}
			}
		}
	}
}
