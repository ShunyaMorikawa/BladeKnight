//=======================================
//
//エフェクト処理[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _LOCKONMARKER_H_     //このマクロ定義が定義されていなかったら
#define _LOCKONMARKER_H_     //2連インクルード防止のマクロを定義する

#include "billboard.h"

//===========================================
//エフェクトクラス
//===========================================
class CLockonMarker : public CBillboard
{
public:
	CLockonMarker(int nPriority = 5);		//コンストラクタ
	~CLockonMarker();		//デストラクタ

	//メンバ関数
	static CLockonMarker* Create(bool alpha);

	HRESULT Init(bool alpha);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ変数
	bool m_bAlpha;		// 加算合成
};

#endif
