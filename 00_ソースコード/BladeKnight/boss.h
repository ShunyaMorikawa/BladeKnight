//========================================
//
// ボス[boss.h]
//Author：森川駿弥
//
//========================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"
#include "enemy.h"

//========================================
// ボスクラス
//========================================
class CBoss : public CEnemy
{
public:
	enum MOTIONTYPE
	{//モーションの種類
		MOTIONTYPE_NEUTRAL = 0,		// 待機状態
		MOTIONTYPE_WALK,			// 歩き状態
		MOTIONTYPE_ATTACK,			// 攻撃
		MOTIONTYPE_MAX
	};

	CBoss();		// コンストラクタ
	~CBoss();		// デストラクタ

	// メンバ関数
	static CBoss *Create();	// チュートリアルエネミー生成

	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

	void Move(float fSpeed);

	float RotNormalize(float RotN, float Rot);		// rotの正規化

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得
	void SetPosition(D3DXVECTOR3 pos);
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
	D3DXVECTOR3 m_pos;		// 向き
	D3DXVECTOR3 m_move;		// 向き
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	// モデルの最大、最小値

	D3DXVECTOR3 m_RotDest;	// 目的の向き

	CModel *m_apModel[MAX_PARTS];	//モデルへのポインタ
	CMotion *m_pMotion;		//モーションのポインタ
	float m_fAngle;			// 目的の向き
	bool m_bWalk;			// 移動
	bool m_bAttack;			// 攻撃
};

#endif