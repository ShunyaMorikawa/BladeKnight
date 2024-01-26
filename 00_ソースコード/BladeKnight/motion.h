//========================================
//
//シューティングアクション[motion.h]
//Author：森川駿弥
//
//========================================
#ifndef _MOTION_H_
#define _MOTION_H_
#include "main.h"
#include "model.h"

//========================================
//マクロ定義
//========================================
#define MAX_MODEL	(16)	//パーツの最大数
#define MAX_MOTION	(8)		//モーションの最大数

//========================================
//モーションクラス
//========================================
class CMotion
{
public:
	typedef struct 
	{//キーの構造体
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}KEY;	//位置・向き

	typedef struct
	{//キー情報の構造体
		int nFrame;				//再生フレーム
		KEY aKey[MAX_MODEL];	//各モデルの情報
	}KEY_INFO;		//カットシーン

	typedef struct 
	{//モーション情報の構造体
		bool bLoop;		//ループするかどうか
		int nNumKey;	//キーの総数
		KEY_INFO aKeyInfo[MAX_MODEL];	//キー情報
	}INFO;

	CMotion();		//コンストラクタ
	~CMotion();		//デストラクタ

	//メンバ関数
	static CMotion *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Set(int nType);
	void Update(void);
	void Draw(void);
	int GetType(void);
	bool IsFinish(void);
	void SetInfo(int nType);
	void SetModel(CModel *ppModel, int nNumModel);
	CModel **GetModel(void) { return &m_ppModel[0]; }

	void Load(char *pPath);

private:
	//メンバ変数
	INFO m_aInfo[MAX_MOTION];	//モーション情報
	int m_nNumAll;		//モーション総数
	int m_nType;		//モーション種類
	int m_nKey;			//現在のキーナンバー
	int m_nCounter;		//モーションカウンター
	int m_nNumMotion;	// 現在のモーション
	int m_nNumParts;
	bool m_bFinish;		//終了したかどうか
	int m_nPartsIndex;	//パーツの番号

	CModel *m_ppModel[MAX_PARTS];	//モデルへのダブルポインタ
	int m_nNumModel;	//モデルの総数
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_motionPos;	//位置

	D3DXVECTOR3 m_rot;	//向き
	D3DXVECTOR3 m_motionRot;	//向き

	CModel *m_pParent;				//親モデルへのポインタ
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数
	LPDIRECT3DTEXTURE9 m_pTexture;	//共有テクスチャ
};

#endif
