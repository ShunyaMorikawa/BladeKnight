//========================================
//
//シューティングアクション[object.h]
//Author：森川駿弥
//
//========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

//========================================
//マクロ定義
//========================================
#define MAX_DATA (100)	//最大数

//========================================
//オブジェクトクラス
//========================================
class CObject
{
public: 
	CObject();				//コンストラクタ
	virtual ~CObject();		//デストラクタ

	//純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetVertex(void) = 0;
	virtual void SetSize(float fWidht, float fHeight) = 0;
	virtual void SetPosition(D3DXVECTOR3 pos) = 0;

	//メンバ関数
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CObject *GetObject(int nIdx);
	void BindTexture(int pTexture);				//テクスチャ割り当て

	D3DXVECTOR3 Getpos(void) { return m_pos; }
	D3DXVECTOR3 Getrot(void) { return m_rot; }
	D3DXVECTOR3 GetMove() { return m_move; }
protected:
	void Release(void);

private:
	static CObject *m_apObject[MAX_DATA];
	static int m_nNumAll;	// オブジェクト総数
	int m_nID;				// 自分自身のID
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_move;		// 移動量
	int m_nIdxTexture;		// テクスチャの番号
};

#endif
