//========================================
//
//シューティングアクション[player.cpp]
//Author：森川駿弥
//
//========================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "motion.h"
#include "model.h"
#include "game.h"

//========================================
//マクロ定義
//========================================
#define PLAYER_SPEED	(4.0f)		//プレイヤーの移動速度
#define PLAYER_INERTIA	(0.3f)		//プレイヤーの慣性

#define PLAYER_PATH	"data\\FILE\\player.txt"	//読み込むファイルのパス

//========================================
//コンストラクタ
//========================================
CEnemy::CEnemy() :
	m_pTexture(nullptr),
	m_move(0.0f, 0.0f, 0.0f),		//移動量
	m_rot(0.0f, 0.0f, 0.0f),		//向き
	m_nIdxTexture(0),				//テクスチャの番号
	m_pMesh(nullptr),				//メッシュ(頂点情報)へのポインタ
	m_pBuffMat(nullptr),			//マテリアルへのポインタ
	m_dwNumMat(0),					//マテリアルの数
	m_apNumModel(0), 				//モデル(パーツ)の総数
	m_RotDest(0.0f, 0.0f, 0.0f),	//目的の向き
	m_nLife(0),						// 体力
	m_bJump(false),
	m_bMove(false),
	m_bWait(false)
{//値をクリア
}

//========================================
//デストラクタ
//========================================
CEnemy::~CEnemy()
{
}