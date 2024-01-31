//========================================
//
//エネミー処理[player.h]
//Author：森川駿弥
//
//========================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"

//========================================
//エネミークラス
//========================================
class CEnemy : public CObject
{
public:
	CEnemy();		//コンストラクタ
	~CEnemy();		//デストラクタ

	//メンバ関数
	static CEnemy *Create(void);	//エネミー生成

	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

	float RotNormalize(float RotN, float Rot);		//rotの正規化

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);

	void SetPosition(D3DXVECTOR3 pos);	//位置設定
	void SetRot(D3DXVECTOR3 rot);		//向き設定

	int GetMotionType();	// モーションの種類取得
	bool GetMotionLoop(int nType);	// モーションのループ
	int GetLife() { return m_nLife; }					// 体力取得

private:
	//メンバ変数

	//一括管理
	LPDIRECT3DTEXTURE9 m_pTexture;	//共有テクスチャ
	int m_nIdxTexture;				//テクスチャの番号
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数

	//オブジェクト
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	//モデルの最大、最小値

	CModel *m_apModel[MAX_PARTS];	//モデルへのポインタ
	CMotion *m_pMotion;		//モーションのポインタ
	D3DXVECTOR3 m_RotDest;	//目的の向き
	int m_apNumModel;		//モデル(パーツ)の総数
	int m_nLife;			// 体力
	int m_nOldMotion;		// 前回のモーション
	bool m_bJump;			//ジャンプ
	bool m_bMove;			//移動
	bool m_bWait;			//待機
};

#endif
