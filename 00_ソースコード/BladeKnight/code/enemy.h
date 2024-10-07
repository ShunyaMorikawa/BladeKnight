//========================================
//
// 敵の処理[enemy.h]
// Author：森川駿弥
//
//========================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "character.h"

//========================================
// 前方宣言
//========================================
class CModel;
class CMotion;
class CEffect;
class CGauge;

//========================================
//エネミークラス
//========================================
class CEnemy : public CCharacter
{
public:
	CEnemy(int nPriority = 4);		//コンストラクタ
	~CEnemy();		//デストラクタ

	// メンバ関数
	static CEnemy* Create(std::string pfile);
	HRESULT Init(void) { return S_OK; }		// 純粋仮想
	HRESULT Init(std::string pfile);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(int nLife);

	int GetLife() { return m_nLife; }			// 体力取得

	float GetRadius() { return m_fRadius; }		// 半径取得

	static CEnemy* GetInstance() { return m_pEnemy; }	// 敵の情報取得

private:
	void NockBack();
	void Motion();
	void CollisionPlayer(int nDamage);
	void CollisionCircle();

	//メンバ変数
	int m_nLife;		// 体力
	int m_nCnt;			// カウント
	int m_nState;		// 状態

	float m_fRadius;	// 半径

	bool m_bWalk;		// 移動判定
	bool m_bAttack;		// 攻撃判定

	CGauge* m_pGauge;	// ゲージのポインタ

	static CEnemy* m_pEnemy;	// 自身のポインタ
};

#endif
