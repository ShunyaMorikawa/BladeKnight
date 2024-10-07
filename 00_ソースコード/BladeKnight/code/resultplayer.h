//========================================
//
//リザルトプレイヤー処理[resultplayer.h]
//Author：森川駿弥
//
//========================================

#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "character.h"

//========================================
// 前方宣言
//========================================
class CModel;
class CMotion;

//========================================
//プレイヤークラス
//========================================
class CResultPlayer : public CCharacter
{
public:
	// 状態
	enum PLAYERSTATE
	{// プレイヤーの状態
		STATE_NONE = 0,
		STATE_NORMAL,		// 通常
		STATE_MAX
	};

	CResultPlayer(int nPriority = 4);		//コンストラクタ
	~CResultPlayer();		//デストラクタ

	// メンバ関数
	static CResultPlayer* Create(std::string pfile);

	HRESULT Init() { return S_OK; }		//純粋仮想
	HRESULT Init(std::string pfile);
	void Uninit();
	void Update();
	void Draw();
	void Motion();

	// 自身の情報
	static CResultPlayer* GetInstance() { return m_pResultPlayer; }

private:

	//メンバ変数
	int m_apNumModel;		// モデル(パーツ)の総数
	int m_nOldMotion;		// 前回のモーション

	PLAYERSTATE m_nState;	// 状態

	bool m_bWait;			// 待機

	CModel* m_apModel[MAX_PARTS];	// モデルのダブルポインタ

	static CResultPlayer* m_pResultPlayer;		// 自身のポインタ
};

#endif
