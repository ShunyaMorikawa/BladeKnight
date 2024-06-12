//========================================
//
//ゲーム内処理[game.h]
//Author：森川駿弥
//
//========================================
#ifndef _GAME_H_
#define _GAME_H_
#include "main.h"
#include "manager.h"

//========================================
//前方宣言
//========================================
class CObjectX;
class CObject3D;
class CLight;
class CPlayer;
class CIdxMeshField;
class CField;
class CEnemy;
class CFade;
class CGauge;

//========================================
//マクロ
//========================================
#define MAX_ITEM	(256)	//アイテムの最大数

//========================================
//ゲームクラス
//========================================
class CGame : public CScene
{
public:
	CGame();	//コンストラクタ
	~CGame();	//デストラクタ

	//メンバ関数
	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void PauseState(bool bPauseState) { m_bPause = bPauseState; }	//ポーズ状態かどうか

	void SetPlayer(CPlayer* pPlayer);
	CPlayer *GetPlayer() { return m_pPlayer; }	// プレイヤーの取得
	CPlayer **GetPlayerPtr() { return &m_pPlayer; }	// プレイヤーの取得
	CEnemy* GetEnemy() { return m_pEnemy; }		// 敵の情報取得
	static CGame *GetInstance(void);		// ゲームマネージャーの情報
	
private:
	static CGame *m_pGame;				// ゲームマネージャーのポインタ

	int m_nTransition;

	bool m_bPause;				// ポーズ状態のON/OFF

	CObjectX *m_pObjectX;		// Xファイルオブジェクトのポインタ
	CPlayer *m_pPlayer;			// プレイヤーのポインタ
	CIdxMeshField *m_pIdxMesh;	// メッシュフィールドのポインタ
	CField *m_pField;			// ポリゴンのポインタ
	CEnemy* m_pEnemy;			// 敵のポインタ
	CFade* m_pFade;				// フェードのポインタ
};

#endif