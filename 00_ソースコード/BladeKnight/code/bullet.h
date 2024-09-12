//=======================================
//
//弾処理[bullet.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _BULLET_H_     //このマクロ定義が定義されていなかったら
#define _BULLET_H_     //2連インクルード防止のマクロを定義する

#include "main.h"
#include "billboard.h"

//=======================================
// 定数定義
//=======================================
namespace
{
const int MAX_BULLET = 128;		// 弾の最大数
}

//===========================================
//エフェクトクラス
//===========================================
class CBullet : public CBillboard
{
public:

	CBullet();		//コンストラクタ
	~CBullet();		//デストラクタ

	//メンバ関数
	static CBullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionEnemy(D3DXVECTOR3 pos);
	void CollisionPlayer(D3DXVECTOR3 pos);
	void CollisionCircle();

private:
	//メンバ変数
	int m_nLife;			// 寿命(表示時間)

};

#endif
