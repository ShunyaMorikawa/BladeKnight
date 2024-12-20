//=======================================
//
//ビルボード[billboard.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _BILLBOARD_H_     //このマクロ定義が定義されていなかったら
#define _BILLBOARD_H_    //2連インクルード防止のマクロを定義する

#include "main.h"
#include "object3D.h"

//=======================================
//ビルボードクラス
//=======================================
class CBillboard : public CObject
{
public:
	CBillboard(int nPriority = 3);	//コンストラクタ
	~CBillboard();	//デストラクタ

	//メンバ関数
	static CBillboard *Create(const D3DXVECTOR3& pos, float fwidth, float fheight);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(int pTexture);

	void SetPosVertex(float size);
	void SetVertexAnim(int Num, int Cnt);

	void SetSizeVertex(float size, float fheight);
	void SetEffectVertex(float fwidth, float fHeight);

	void SetVertex(void);

	void SetSize(float fWidht, float fHeight);
	float GetSize() { return m_fWidth, m_fHeight; }	// サイズ取得

	// 色の設定・取得
	void SetCol(D3DXCOLOR col) { m_col = col; }
	D3DXCOLOR GetCol() { return m_col; }

private:
	int m_nIdxTexture;		//テクスチャの番号
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		//頂点情報を格納
	D3DXCOLOR m_col;        //色
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	float m_fLength;	//対角線の長さ
	float m_fAngle;		//対角線の角度
};

#endif