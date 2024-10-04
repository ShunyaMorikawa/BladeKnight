//========================================
//
//タイトルプレイヤー処理[titleplayer.h]
//Author：森川駿弥
//
//========================================

#ifndef _TITLEPLAYER_H_
#define _TITLEPLAYER_H_

#include "character.h"

//========================================
// 前方宣言
//========================================
class CModel;
class CMotion;

//========================================
//プレイヤークラス
//========================================
class CTitlePlayer : public CCharacter
{
public:
	// 状態
	enum PLAYERSTATE
	{// プレイヤーの状態
		STATE_NONE = 0,
		STATE_NORMAL,		// 通常
		STATE_MAX
	};

	CTitlePlayer(int nPriority = 4);		//コンストラクタ
	~CTitlePlayer();		//デストラクタ

	// メンバ関数
	static CTitlePlayer* Create(std::string pfile);

	HRESULT Init() { return S_OK; }		//純粋仮想
	HRESULT Init(std::string pfile);
	void Uninit();
	void Update();
	void Draw();

	static CTitlePlayer* GetInstance() { return m_pTitlePlayer; }

private:
	//メンバ変数

	int m_apNumModel;		// モデル(パーツ)の総数
	int m_nOldMotion;		// 前回のモーション

	int m_nState;			// 状態

	bool m_bMove;			// 移動

	CModel* m_apModel[MAX_PARTS];	// モデルのダブルポインタ

	static CTitlePlayer* m_pTitlePlayer;		// 自身のポインタ
};

#endif
