//=======================================
//
// 便利関数[useful.h]
// Author　:　MORIKAWA SHUNYA
//
//=======================================

#include "useful.h"

//=======================================
// 向きと角度の正規化
//=======================================
void USEFUL::NormalizeRotAngle(float& fRotAngle)
{
	//向きの正規化
	if (fRotAngle > D3DX_PI)
	{//3.14を超えたときに反対にする
		fRotAngle -= D3DX_PI * 2.0f;
	}

	//-3.14を超えたときに反対にする
	if (fRotAngle < -D3DX_PI)
	{
		fRotAngle += D3DX_PI * 2.0f;
	}
}
