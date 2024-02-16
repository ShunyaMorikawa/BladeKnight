//========================================
//
//チュートリアル[tutorial.h]
//Author：森川駿弥
//
//========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "main.h"
#include "manager.h"

//========================================
//前方宣言
//========================================
class CObjectX;
class CObject3D;
class CLight;
class CPlayer;
class CPolygon;
class CGame_Object;
class CScarecrow;

//========================================
//チュートリアルクラス
//========================================
class CTutorial : public CScene
{
public:
	CTutorial();	//コンストラクタ
	~CTutorial();	//デストラクタ

	//メンバ関数
	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTutorial *GetTutorial(void) { return m_pTutorial; }		// チュートリアルの情報
	static CPlayer *GetPlayer(void) { return m_pPlayer; }			// プレイヤーの情報
	static CScarecrow *GetScarecrow(void) { return m_pScarecrow; }	// 案山子の情報

private:
	static CObject *m_pObject[MAX_DATA];		//オブジェクトのポインタ
	static CObjectX *m_pObjectX;		//Xファイルオブジェクトのポインタ
	static CObject3D *m_pObject3D;		//オブジェクト3Dのポインタ
	static CTutorial *m_pTutorial;		//ゲームマネージャーのポインタ
	static CLight *m_pLight;			//ライトのポインタ
	static CPlayer *m_pPlayer;			//プレイヤーのポインタ
	static CPolygon *m_pPolygon;		//ポリゴンのポインタ
	static CScarecrow *m_pScarecrow;	// チュートリアルエネミーポインタ
};

#endif
