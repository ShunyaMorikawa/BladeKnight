//========================================
//
//シューティングアクション[title.h]
//Author：森川駿弥
//
//========================================

#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "manager.h"
#include "object2D.h"
#include "renderer.h"

//========================================
//タイトルクラス
//========================================
class CTitle : public CScene
{
public:
	CTitle();	//コンストラクタ
	~CTitle();	//デストラクタ

	static CTitle* GetInstance();

	//メンバ関数
	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void TitleLogo();
	void PressEnter();

private:
	static CTitle* m_pTitle;		// チュートリアルのポインタ

	CObject2D* m_pTitleLogo;
	CObject2D* m_pEnter;
};

#endif
