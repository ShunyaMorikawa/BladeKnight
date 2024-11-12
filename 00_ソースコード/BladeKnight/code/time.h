//=======================================
//
// 経過時間[time.h]
// Author:森川駿弥
//
//=======================================
#ifndef _TIME_H_     //このマクロ定義が定義されていなかったら
#define _TIME_H_     //2連インクルード防止のマクロを定義する

//========================================
// タイムクラス
//========================================
class CTIME
{
public:
	void Init();
	void Update();
	float Get() { return m_fDeltaTime; }

private:
	float m_fDeltaTime;		// デルタタイム

};


#endif