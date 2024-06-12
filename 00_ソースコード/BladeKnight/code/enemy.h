//========================================
//
//敵関連[enemy.h]
//Author：森川駿弥
//
//========================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "manager.h"
#include "character.h"
#include "gauge.h"

//========================================
//エネミークラス
//========================================
class CEnemy : public CCharacter
{
public:
	CEnemy();		//コンストラクタ
	~CEnemy();		//デストラクタ

	// メンバ関数
	static CEnemy* Create(std::string pfile);
	HRESULT Init(void) { return S_OK; }		// 純粋仮想
	HRESULT Init(std::string pfile);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(int nLife);
	void NockBack();
	void Motion();
	void CollisionPlayer(int nDamage);

	int GetLife() { return m_nLife; }			// 体力取得

	float GetRadius() { return m_fRadius; }		// 半径取得

private:
	//メンバ変数
	int m_nLife;		// 体力
	int m_nCnt;			// カウント

	float m_fRadius;	// 半径

	bool m_bWalk;		// 移動判定
	bool m_bAttack;		// 攻撃判定

	CGauge* m_pGauge;	// ゲージのポインタ
};

#endif
