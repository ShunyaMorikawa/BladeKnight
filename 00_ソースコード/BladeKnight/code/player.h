//========================================
//
//プレイヤー処理[player.h]
//Author：森川駿弥
//
//========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

//========================================
// 前方宣言
//========================================
class CModel;
class CMotion;
class CEffect;
class CGauge;

//========================================
//プレイヤークラス
//========================================
class CPlayer : public CCharacter
{
public:
	CPlayer(int nPriority = 4);		//コンストラクタ
	~CPlayer();		//デストラクタ

	// メンバ関数
	static CPlayer* Create(std::string pfile);

	HRESULT Init() { return S_OK; }		//純粋仮想
	HRESULT Init(std::string pfile);
	void Uninit();
	void Update();
	void Draw();
	void Act(float fSpeed);
	void Attack();
	void Motion();
	void CollisionEnemy(int nDamage);
	void NockBack();
	void Hit(int nLife);

	float GetRadius() { return m_fRadius; }		// 半径取得

	int GetLife() { return m_nLife; }		// 体力取得

	void CollisionField();

	void CollisionCircle();

	static CPlayer* GetInstance() { return m_pPlayer; }

private:
	//メンバ変数

	int m_apNumModel;		// モデル(パーツ)の総数
	int m_nLife;			// 体力
	int m_nOldMotion;		// 前回のモーション

	float m_fRadius;		// 半径

	bool m_bJump;			//ジャンプ
	bool m_bMove;			//移動
	bool m_bWait;			//待機
	bool m_bMowingdown;		// 攻撃
	bool m_bCutdown;		// 切り下ろし
	bool m_bStrongAttack;	// 強攻撃

	CEffect *m_pEffect;				// エフェクトのポインタ
	CGauge* m_pGauge;				// ゲージのポインタ
	CModel* m_apModel[MAX_PARTS];	// モデルのダブルポインタ

	static CPlayer* m_pPlayer;		// 自身のポインタ
};

#endif
