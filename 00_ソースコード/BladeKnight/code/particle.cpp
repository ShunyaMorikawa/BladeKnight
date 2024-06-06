//
//
//パーティクル処理[particle.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //作成したmain.hをインクルードする
#include "particle.h"    //作成したeffect.hをインクルードする
#include "effect.h"

//=======================================
// 名前空間
//=======================================
namespace
{
	D3DXVECTOR3 POS;	// 位置
}

//=======================================
// 生成
//=======================================
void Myparticle::Create(TYPE nType, D3DXVECTOR3 pos)
{
	POS = pos;

	(*(ParticleList[nType]))();
}

//=======================================
// 歩行時のパーティクル
//=======================================
void Mypartcile_Func::WALK_PARTICLE()
{
	int life = 60;		// 寿命
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.7f, 0.6f, 1.0f);	// 色
	const int max = 10;		// 最大数

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 移動量の基準値
		float StandardMove = 1.0f + (rand() % 10) * 0.9f;

		// 半径の基準値
		float StandardRadius = 9.0f + (rand() % 50) * 0.9f;

		// 大きさ
		move.x = sinf((rand() % 629 - 314) / 10.0f) * StandardMove;

		CEffect::Create(POS, move, col, StandardRadius, life, false);
	}
}

//=======================================
// 死亡時のパーティクル
//=======================================
void Mypartcile_Func::DEATH_PARTICLE()
{
	int life = 60;		// 寿命
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 色
	const int max = 32;		// 最大数

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 移動量の基準値
		float StandardMove = 9.0f + (rand() % 100) * 0.1f;

		// 半径の基準値
		float StandardRadius = 9.0f + (rand() % 100) * 0.9f;

		// 大きさ
		move.x = sinf((rand() % 629 - 314) / 100.0f) * StandardMove;
		move.y = sinf((rand() % 629 - 314) / 150.0f) * StandardMove;

		// エフェクト生成
		CEffect::Create(POS, move, col, StandardRadius, life, true);
	}
}
