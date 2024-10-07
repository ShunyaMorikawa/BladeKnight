//========================================
//
// リザルトプレイヤー処理[resultplayer.h]
// Author：森川駿弥
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
		STATE_WIN,		// 勝利
		STATE_LOSE,		// 敗北
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

	// 自身の情報
	static CResultPlayer* GetInstance() { return m_pResultPlayer; }

private:
	void Motion();

	//メンバ変数
	int m_apNumModel;		// モデル(パーツ)の総数
	int m_nOldMotion;		// 前回のモーション

	PLAYERSTATE m_nState;	// 状態

	bool m_IsWin;	// 勝利
	bool m_IsLose;	// 敗北

	CModel* m_apModel[MAX_PARTS];	// モデルのダブルポインタ

	static CResultPlayer* m_pResultPlayer;		// 自身のポインタ
};

#endif
