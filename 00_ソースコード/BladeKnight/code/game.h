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
class CObject2D;
class CObject3D;
class CLight;
class CPlayer;
class CIdxMeshField;
class CField;
class CEnemy;
class CFade;
class CGauge;
class CWall;
class CMapObject;

//========================================
// 定数定義
//========================================
namespace
{
const int ITEM_MAX = 256;	// アイテムの最大数
}

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

	static CGame *GetInstance(void);		// ゲームマネージャーの情報
	
private:
	static CGame *m_pGame;		// ゲームマネージャーのポインタ

	int m_nTransition;			// 加算

	bool m_bPause;				// ポーズ状態のON/OFF

	CObjectX *m_pObjectX;		// Xファイルオブジェクトのポインタ
	CIdxMeshField *m_pIdxMesh;	// メッシュフィールドのポインタ
	CField *m_pField;			// ポリゴンのポインタ
	CFade* m_pFade;				// フェードのポインタ
	CMapObject* m_pMobj;		// マップオブジェクトのポインタ
	CObject2D* m_pObj2D;		// オブジェクト2Dのポインタ
};

#endif
