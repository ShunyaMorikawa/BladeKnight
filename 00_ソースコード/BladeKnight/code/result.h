//========================================
//
// リザルト[result.h]
// Author：森川駿弥
//
//========================================

#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"
#include "object2D.h"
#include "renderer.h"

//========================================
// リザルトクラス
//========================================
class CResult : public CScene
{
public:
	enum RESULTMOTION
	{
		MOTION_NONE = 0,
		MOTION_WIN,		// 勝利
		MOTION_LOSE,	// 敗北
		MOTION_MAX
	};

	CResult();	//コンストラクタ
	~CResult();	//デストラクタ

	//メンバ関数
	static CResult* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void ChangeFrag(bool frag) { m_bResult = frag; }
	static bool GetFrag() { return m_bResult; }

private:
	static bool m_bResult;	// フラグ

	CObject2D* m_pWin;	// 勝利テクスチャ
	CObject2D* m_pLose;	// 敗北テクスチャ
};

#endif
