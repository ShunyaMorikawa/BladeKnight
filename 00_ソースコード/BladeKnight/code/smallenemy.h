//========================================
//
// 雑魚敵の処理[smallenemy.h]
// Author：森川駿弥
//
//========================================
#ifndef _SMALLENEMY_H_
#define _SMALLENEMY_H_

#include "enemy.h"

//========================================
// 雑魚敵クラス
//========================================
class CSmallEnemy : CEnemy
{
public:
	// コンストラクタ・デストラクタ
	CSmallEnemy();
	~CSmallEnemy();

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CSmallEnemy* Create();

private:
	// メンバ変数
};

#endif
