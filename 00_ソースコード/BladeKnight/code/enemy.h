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
	HRESULT Init(std::string pfile);		// 初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画
	void Hit(int nLife);
	void DestMove();

	int GetLife() { return m_nLife; }			// 体力取得

	float GetRadius() { return m_fRadius; }		// 半径取得

private:
	//メンバ変数
	int m_nLife;			// 体力
	float m_fRadius;		// 半径
};

#endif
