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
	CResult();	//コンストラクタ
	~CResult();	//デストラクタ

	//メンバ関数
	static CResult* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObject2D* m_pObj2D;
	CObject2D* m_pTitleObj;
};

#endif
