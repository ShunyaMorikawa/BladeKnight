//========================================
//
//ゲーム内オブジェクト[gameobject.h]
//Author：森川駿弥
//
//========================================
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "main.h"
#include "objectX.h"
#include "model.h"

//========================================
//ゲーム内オブジェクトクラス
//========================================
#define GAME_OBJ	(1)		//ゲーム内オブジェクトの最大数

//========================================
//ゲーム内オブジェクトクラス
//========================================
class CGameObject : public CModel
{
public:
	CGameObject();		//コンストラクタ
	~CGameObject();	//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit();
	void Update();
	void Draw();
	void SetObject(CModel *ppModel, int nNumModel);

	static CGameObject *Create(void);	//プレイヤー生成
	void LoadObj(char *pFilePath);

	CModel **GetModel(void) { return &m_ppModel[0]; }	//モデル取得

private:
	//メンバ変数
	CModel *m_ppModel[GAME_OBJ];	//モデルへのポインタ
	CObjectX *m_pObjX;		//オブジェクトXのポインタ

	int m_nNumModel;	//モデルの総数
};

#endif