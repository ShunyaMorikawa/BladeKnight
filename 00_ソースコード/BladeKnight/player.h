//========================================
//
//プレイヤー処理[player.h]
//Author：森川駿弥
//
//========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"
#include "boss.h"
#include "scarecrow.h"

//========================================
//プレイヤークラス
//========================================
class CPlayer : public CObject
{
public:
	CPlayer();		//コンストラクタ
	~CPlayer();		//デストラクタ

	//メンバ関数
	static CPlayer *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);

	void Act(float fSpeed);

	float RotNormalize(float RotN, float Rot);

	void CollisionScarecrow();

	void CollisionBoss();

	int GetLife() { return m_nLife; }					// 体力取得
	float GetSize() { return m_fSize; }					// サイズ調整

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }		// 位置取得

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得

	int GetMotionType() { return m_pMotion->GetType(); }				// モーションの種類取得
	bool GetMotionLoop(int nType) { return m_pMotion->IsLoop(nType); }	// モーションのループ取得

private:
	//メンバ変数

	//一括管理
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数

	//オブジェクト
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//向き

	CModel *m_apModel[MAX_PARTS];	//モデルへのポインタ
	CMotion *m_pMotion;		// モーションのポインタ
	CBoss *m_pBoss;			// ボスのポインタ
	D3DXVECTOR3 m_RotDest;	// 目的の向き
	int m_apNumModel;		// モデル(パーツ)の総数
	int m_nLife;			// 体力
	int m_nOldMotion;		// 前回のモーション
	float m_fSize;			// サイズ
	bool m_bMove;			// 移動
	bool m_bWait;			// 待機
	bool m_bMowingdown;		// 攻撃
	bool m_bCutdown;		// 切り下ろし
	bool m_bStrongAttack;	// 強攻撃
};

#endif
