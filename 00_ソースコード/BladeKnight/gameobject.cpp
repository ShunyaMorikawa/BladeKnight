//========================================
//
//ゲーム内オブジェクト[gameobject.h]
//Author：森川駿弥
//
//========================================
#include "gameobject.h"

//========================================
//コンストラクタ
//========================================
CGameObject::CGameObject() :
	m_pObjX(nullptr),
	m_nNumModel(0)
{
}

//========================================
//デストラクタ
//========================================
CGameObject::~CGameObject()
{
}

//========================================
//初期化
//========================================
HRESULT CGameObject::Init(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{//ダブルポインタの初期化
		m_ppModel[nCntModel] = nullptr;
	}

	//モデルの総数
	m_nNumModel = 0;

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CGameObject::Uninit()
{
	//終了
	CModel::Uninit();
}

//========================================
//更新
//========================================
void CGameObject::Update()
{
}

//========================================
//描画
//========================================
void CGameObject::Draw()
{
	for (int nCntParts = 0; nCntParts < GAME_OBJ; nCntParts++)
	{//モデルの描画(全パーツ)
		if (m_ppModel[nCntParts] != nullptr)
		{//m_apModelがnullptrの時
			m_ppModel[nCntParts]->Draw();
		}
	}
}

//========================================
//生成
//========================================
CGameObject *CGameObject::Create(void)
{
	//CGame_Object型のポインタ
	CGameObject *pGameObject = nullptr;

	if (pGameObject == nullptr)
	{
		//インスタンス生成
		pGameObject = new CGameObject;

		//初期化
		pGameObject->Init();
	}

	//ポインタを返す
	return pGameObject;
}

//========================================
//オブジェクト設定
//========================================
void CGameObject::SetObject(CModel * ppModel, int nNumModel)
{
	//ポインタ
	m_ppModel[nNumModel] = ppModel;

	//パーツ番号
	m_nNumModel++;
}

//========================================
//ファイル読み込み
//========================================
void CGameObject::LoadObj(char * pFilePath)
{
	//変数宣言
	char garbage[640];		//ゴミ格納用
	char FileName[32];		//各モデルのファイル名
	int nNumParts = 0;		//パーツ数
	int nNum = 0;				//読み込む番号
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);		//位置
	D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);		//向き

	//FILEのポインタ
	FILE *pFile = nullptr;

	//ファイルを開く
	pFile = fopen(pFilePath, "r");

	while (1)
	{//END_SCRIPTが来るまで繰り返す
	 //不要な文字を捨てる
		fscanf(pFile, "%s", &garbage[0]);

		if (strcmp(&garbage[0], "END_SCRIPT") == 0)
		{//文字列がEND_SCRIPTのとき
		 //抜け出す
			break;
		}

		//========================================
		//オブジェクトファイルの読み込み
		//========================================
		if (strcmp(&garbage[0], "OBJECT_FILENAME") == 0)
		{//文字列がMODEL_FILENAMEのとき
			//=捨てる
			fscanf(pFile, "%s", &garbage[0]);

			//ファイルの名前取得
			fscanf(pFile, "%s", &FileName[0]);

			//モデルの生成
			m_ppModel[nNum] = CModel::Create(&FileName[0]);

			//モデル設定
			SetObject(m_ppModel[nNum], nNum);

			//モデルの番号を加算する
			nNum++;
		}

		//========================================
		//オブジェクト設定
		//========================================
		if (strcmp(&garbage[0], "OBJECTSET") == 0)
		{//文字列がCHARACTERSETのとき
			while (1)
			{//文字列がEND_CHARACTERSETになるまで繰り返す
				fscanf(pFile, "%s", &garbage[0]);

				if (strcmp(&garbage[0], "END_OBJECTSET") == 0)
				{//文字列がEND_CHARACTERSETの時
				 //抜け出す
					break;
				}

				if (strcmp(&garbage[0], "NUM_PARTS") == 0)
				{//文字列がNUM_PARTSの時
				 //=捨てる
					fscanf(pFile, "%s", &garbage[0]);

					//パーツ数を入れる
					fscanf(pFile, "%d", &nNumParts);
				}

				if (strcmp(&garbage[0], "OBJECTSET") == 0)
				{//文字列がPARTSSETの時
					while (1)
					{//END_PARTSSETが来るまで回す
						fscanf(pFile, "%s", &garbage[0]);

						if (strcmp(&garbage[0], "END_OBJECTSET") == 0)
						{//文字列がEND_PARTSSETの時
						 //パーツ数を加算

						 //抜け出す
							break;
						}

					}

					if (strcmp(&garbage[0], "POS") == 0)
					{//文字列がPARENTの時
					 //=捨てる
						fscanf(pFile, "%s", &garbage[0]);

						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);

						//位置を設定
					}

					if (strcmp(&garbage[0], "ROT") == 0)
					{//文字列がPARENTの時
					 //=捨てる
						fscanf(pFile, "%s", &garbage[0]);

						fscanf(pFile, "%f", &rot.x);
						fscanf(pFile, "%f", &rot.y);
						fscanf(pFile, "%f", &rot.z);

						//向きを設定
					}
				}
			}
		}
	}
}
