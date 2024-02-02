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

	virtual HRESULT Init(void) = 0;		//初期化
	virtual void Uninit(void) = 0;		//終了
	virtual void Update(void) = 0;		//更新
	virtual void Draw(void) = 0;		//描画

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
