//=======================================
//
// ステンシルバッファに必要な2Dマスク[mask.h]
// Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _MASK_H_	 //このマクロ定義が定義されていなかったら
#define _MASK_H_	 //2連インクルード防止のマクロを定義する

#include "object2d.h"
#include "enemy.h"

//=======================================
// マスククラス
//=======================================
class CMask : public CObject2D
{
public:
	CMask(int nPriority = 5);	//コンストラクタ
	~CMask();	//デストラクタ

	static CMask* Create(D3DXCOLOR col);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// ステンシルの参照値設定・取得
	void SetRefidx(int nRef) { m_nRefidx = nRef; }
	int GetRefIdx() { return m_nRefidx; }

private:
	//メンバ変数
	int m_nRefidx;	// ステンシルの参照値
};

#endif
