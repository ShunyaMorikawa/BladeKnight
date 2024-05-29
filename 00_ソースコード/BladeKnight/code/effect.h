//=======================================
//
//エフェクト処理[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EFFECT_H_     //このマクロ定義が定義されていなかったら
#define _EFFECT_H_     //2連インクルード防止のマクロを定義する

#include "main.h"
#include "billboard.h"

//===========================================
//マクロ定義
//===========================================
#define MAX_EFFECT		(30)		//エフェクトの最大数

//===========================================
//エフェクトクラス
//===========================================
class CEffect : public CBillboard
{
public:
	// 構造体の定義
	struct Effect
	{
		int nLife;			// 寿命(表示時間)
		int nFirstLife;		// 初期寿命
		bool balpha;			//加算合成
	};

	CEffect(int nPriority = 5);		//コンストラクタ
	~CEffect();		//デストラクタ

	//メンバ関数
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//メンバ変数
	Effect m_aEffect;	// 構造体の情報
};

#endif