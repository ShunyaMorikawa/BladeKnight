//========================================
//
// 管理[manager.h]
// Author：森川駿弥
//
//========================================

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "object.h"

//========================================
//前方宣言
//========================================
class CObject;
class CInputKeyboard;
class CRenderer;
class CScene;
class CDebugProc;
class CTexture;
class CCamera;
class CInputPad;
class CCamera;
class CInputMouse;
class CLight;
class CResult;
class CTutorial;
class CFade;
class CSound;

//========================================
//シーンクラス
//========================================
class CScene
{
public:
	enum MODE
	{//モードの列挙型
		MODE_NONE = 0,	//	設定なし
		MODE_TITLE,		//	タイトル
		MODE_TUTORIAL,	//	チュートリアル
		MODE_GAME,		//	ゲーム
		MODE_RESULT,	//	リザルト
		MODE_MAX
	};

	CScene();	//コンストラク
	~CScene();	//デストラクタ

	//メンバ関数
	static CScene *Create(int nMode);

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	MODE GetMode(void) { return m_mode; }	//モード取得

private:
	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点情報を格納
	int m_nIdxTexture;		//テクスチャ番号
	MODE m_mode;			//現在のモード
};

//========================================
//マネージャクラス
//========================================
class CManager
{
private:	//他の場所でアクセスさせない
	CManager();		//コンストラクタ
public:
	virtual ~CManager();	//デストラクタ
	static CManager *GetInstance();

public:
	//メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// レンダラー取得
	CRenderer *GetRenderer(void) { return m_pRenderer; }

	// キーボード・パッド・マウス取得
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	CInputPad *GetInputPad(void) { return m_pInputPad; }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse; }

	// テクスチャ取得
	CTexture *GetTexture(void) { return m_pTexture; }

	// デバッグ表示
	CDebugProc *GetDebugProc(void) { return m_pDebugProc; }

	// モード設定。取得
	void SetMode(CScene::MODE mode);
	CScene::MODE GetMode(void) { return m_pScene->GetMode(); }

	// カメラ取得
	CCamera *GetCamera(void) { return m_pCamera; }

	// フェード取得
	CFade* GetFade() { return m_pFade; }

	// 音取得
	CSound* GetSound() { return m_pSound; }

	// 勝敗判定
	bool bResult() {return m_IsResult;}

private:
	static CManager *m_pManager;

	//メンバ変数
	int m_nCnt;		//自動遷移カウンター
	int m_state;	//状態変更
	int m_NowScene;	//現在のシーン

	bool m_IsResult;

	CRenderer *m_pRenderer;				// レンダラーのポインタ
	CInputKeyboard *m_pInputKeyboard;	// キーボードのポインタ
	CInputPad *m_pInputPad;		// コントローラーのポインタ
	CInputMouse *m_pInputMouse; // マウスのポインタ
	CScene *m_pScene;			// シーンのポインタ
	CDebugProc *m_pDebugProc;	// デバッグ表示のポインタ
	CTexture *m_pTexture;		// テクスチャへのポインタ
	CCamera *m_pCamera;			// カメラのポインタ
	CLight* m_pLight;			// ライトのポインタ
	CFade* m_pFade;				// フェードのポインタ
	CSound* m_pSound;			// サウンドのポインタ
};

#endif
