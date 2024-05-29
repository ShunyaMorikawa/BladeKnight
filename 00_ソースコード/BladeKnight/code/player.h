//========================================
//
//プレイヤー処理[player.h]
//Author：森川駿弥
//
//========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "model.h"
#include "motion.h"
#include "effect.h"
#include "bullet.h"
#include "character.h"

//========================================
//プレイヤークラス
//========================================
class CPlayer : public CCharacter
{
public:
	CPlayer(int nPriority = 4);		//コンストラクタ
	~CPlayer();		//デストラクタ

	// メンバ関数
	HRESULT Init() { return S_OK; }		//純粋仮想
	HRESULT Init(std::string pfile);	//初期化
	void Uninit();		//終了
	void Update();		//更新
	void Draw();		//描画
	void Act(float fSpeed);
	void Attack();
	void Motion();
	void CollisionEnemy(int nDamage);
	void NockBack();

	//メンバ関数
	static CPlayer *Create(std::string pfile);

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
	CModel *m_apModel[MAX_PARTS];	// モデルへのポインタ
};

#endif
