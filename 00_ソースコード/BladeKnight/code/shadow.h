//========================================
//
// シャドウ処理[shadow.h]
// Author：森川駿弥
//
//========================================

#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

//=======================================
//シャドウクラス
//=======================================
class CShadow : public CObject3D
{
public:
	CShadow(int nPriority = 3);		//コンストラクタ
	~CShadow();	//デストラクタ

	//メンバ関数
	static CShadow* Create();
	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

private:
	//メンバ変数
};

#endif