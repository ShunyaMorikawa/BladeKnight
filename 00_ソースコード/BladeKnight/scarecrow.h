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
	enum MOTIONTYPE
	{//モーションの種類
		MOTIONTYPE_NEUTRAL = 0,	//待機状態
		MOTIONTYPE_WALK,			//歩き状態
		MOTIONTYPE_MAX
	};

	CScarecrow();		// コンストラクタ
	~CScarecrow();		// デストラクタ

	// メンバ関数
	static CScarecrow *Create();	// チュートリアルエネミー生成

	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

	float RotNormalize(float RotN, float Rot);		// rotの正規化

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }		// 位置取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得

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
	CMotion *m_pMotion;		//モーションのポインタ
	float m_fAngle;			// 目的の向き
};

#endif