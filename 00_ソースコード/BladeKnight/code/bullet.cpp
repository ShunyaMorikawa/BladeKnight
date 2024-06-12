//=======================================
//
//弾[bullet.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "bullet.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "particle.h"

//===========================================
//コンストラクタ
//===========================================
CBullet::CBullet()
{
}

//===========================================
//デストラクタ
//===========================================
CBullet::~CBullet()
{
}

//===========================================
//生成
//===========================================
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife)
{
	//CBullet型のポインタ
	CBullet* pEffect = nullptr;

	if (pEffect == nullptr)
	{//nullptrの時
		//インスタンス生成
		pEffect = new CBullet;

		pEffect->SetPos(pos);
		pEffect->SetMove(move);
		pEffect->m_nLife = nLife;

		//初期化
		pEffect->Init();
	}

	//ポインタを返す
	return pEffect;
}

//===========================================
//初期化処理
//===========================================
HRESULT CBullet::Init(void)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//初期化
	CBillboard::Init();

	// サイズ設定
	SetSize(150.0f, 150.0f);

	BindTexture(pTexture->Regist("data\\texture\\effect000.png"));

	//成功を返す
	return S_OK;
}

//===========================================
//終了処理
//===========================================
void CBullet::Uninit(void)
{
	//終了
	CBillboard::Uninit();
}

//===========================================
//更新処理
//===========================================
void CBullet::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXCOLOR col = GetCol();

	// 更新
	CBillboard::Update();

	// 位置更新
	pos += move;

	if (m_nLife % 2 == 0)
	{
		Myparticle::Create(Myparticle::TYPE::TYPE_BULLET, pos);
	}

	// 位置設定
	SetPos(pos);

	// 体力減算
	m_nLife--;

	if (m_nLife < 0)
	{
		//破棄する
		Uninit();
	}

	// 敵との当たり判定
	CollisionPlayer(pos);
}

//===========================================
//描画処理
//===========================================
void CBullet::Draw(void)
{
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//CRenderer型のポインタ
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//描画
	//CBillboard::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//===========================================
// 敵との当たり判定
//===========================================
void CBullet::CollisionEnemy(D3DXVECTOR3 pos)
{
	// 長さ
	float fLength;

	float fRadius = GetSize();

	// 敵の情報取得
	CEnemy *pEnemy = CGame::GetInstance()->GetEnemy();

	// プレイヤーの位置
	D3DXVECTOR3 posEnemy = pEnemy->GetPos();

	// 半径
	float radiusEnemy = pEnemy->GetRadius();

	// ベクトルを求める
	D3DXVECTOR3 vec = posEnemy - pos;

	//ベクトル代入
	fLength = D3DXVec3Length(&vec);

	if (fLength <= radiusEnemy + fRadius)
	{
		Uninit();

		pEnemy->Hit(1);
	}
}

//===========================================
// プレイヤーとの当たり判定
//===========================================
void CBullet::CollisionPlayer(D3DXVECTOR3 pos)
{
	// 長さ
	float fLength;

	float fRadius = GetSize();

	// 敵の情報取得
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer();

	if (pPlayer == nullptr)
	{
		return;
	}

	// プレイヤーの位置
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	// 半径
	float radiusPlayer = pPlayer->GetRadius();

	// ベクトルを求める
	D3DXVECTOR3 vec = posPlayer - pos;

	//ベクトル代入
	fLength = D3DXVec3Length(&vec);

	if (fLength <= radiusPlayer + fRadius)
	{// プレイヤーに当たった
		Uninit();

		// 体力消費
		pPlayer->Hit(1);
	}
}
