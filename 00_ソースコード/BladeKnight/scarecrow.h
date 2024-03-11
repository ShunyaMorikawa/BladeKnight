//========================================
//
// チュートリアルエネミー[scarecrow.h]
//Author：森川駿弥
//
//========================================
#ifndef _SCARECROW_H_
#define _SCARECROW_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"
#include "enemy.h"

//========================================
// チュートリアルエネミークラス
//========================================
class CScarecrow : public CEnemy
{
public:
	CScarecrow();		// コンストラクタ
	~CScarecrow();		// デストラクタ

	enum STATE
	{// 状態
		STATE_NONE = 0,		// 通常
		STATE_DAMAGE,		// ダメージ
		STATE_MAX
	};

	// メンバ関数
	static CScarecrow *Create();	// チュートリアルエネミー生成

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float RotNormalize(float RotN, float Rot);		// rotの正規化

	int GetState() { return m_nState; }			// 状態取得

	float GetSize(void) { return m_fSize; }		// サイズ取得

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }		// 位置取得

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);

private:
	//メンバ変数

	//一括管理
	LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャ
	int m_nIdxTexture;				// テクスチャの番号
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	LPD3DXMESH m_pMesh;				// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		// マテリアルへのポインタ
	DWORD m_dwNumMat;				// マテリアルの数

	//オブジェクト
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	// モデルの最大、最小値
	D3DXVECTOR3 m_RotDest;	// 目的の向き

	CModel *m_apModel[MAX_PARTS];	//モデルへのポインタ
	CMotion *m_pMotion;				// モーションのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	int m_nState;			// 状態
	float m_fAngle;			// 目的の向き
	float m_fSize;			// サイズ
};

#endif
