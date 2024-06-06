//========================================
//
//ゲージ[gauge.h]
//Author：森川駿弥
//
//========================================
#ifndef _GAUGE_H_
#define _GAUGE_H_
#include "main.h"
#include "object2D.h"

//========================================
//ゲージクラス
//========================================
class CGauge : public CObject2D
{
public:
	CGauge(int nPriority = 7);		//コンストラクタ
	~CGauge();	//デストラクタ

	//メンバ関数
	static CGauge* Create(int nMaxLife);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVertex(void);
	void SetLife(int nLife) { m_nLife = nLife; }

private:
	//メンバ変数
	int m_nMaxLife;		// 体力の最大値
	int m_nLife;		// 現在の体力
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
};

#endif