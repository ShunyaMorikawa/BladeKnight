//=======================================
//
//エフェクト処理[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _ROCKONMARKER_H_     //このマクロ定義が定義されていなかったら
#define _ROCKONMARKER_H_     //2連インクルード防止のマクロを定義する

#include "billboard.h"

//===========================================
//エフェクトクラス
//===========================================
class CRockonMarker : public CBillboard
{
public:
	CRockonMarker(int nPriority = 5);		//コンストラクタ
	~CRockonMarker();		//デストラクタ

	//メンバ関数
	static CRockonMarker* Create(bool alpha);

	HRESULT Init(bool alpha);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	bool m_bAlpha;
};

#endif