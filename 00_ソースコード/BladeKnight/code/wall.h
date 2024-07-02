//========================================
//
// ウォール[wall.h]
// Author：森川駿弥
//
//========================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "object3D.h"

//========================================
// ウォールクラス
//========================================
class CWall : public CObject3D
{
public:
	CWall(int nPriority = 0);		//コンストラクタ
	~CWall();	//デストラクタ

	//メンバ関数
	static CWall* Create(void);	//ポリゴン生成

	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

private:
	//メンバ変数
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
};

#endif