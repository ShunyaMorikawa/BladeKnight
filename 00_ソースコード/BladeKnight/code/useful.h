//=======================================
//
// 便利関数[useful.h]
// Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _CONVENIENCE_H_     //このマクロ定義が定義されていなかったら
#define _CONVENIENCE_H_     //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
// 名前空間
//=======================================
namespace USEFUL
{// 便利関数
	//	向きの正規化
	void NormalizeRot(float fRot);

}

#endif