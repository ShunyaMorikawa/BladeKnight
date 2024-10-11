//========================================
//
//プリコンパイル[precompile.h]
//Author：森川駿弥
//
//========================================

//========================================
// 事前インクルード
//========================================

#ifndef PRECOMPILE_H__
#define PRECOMPILE_H__

#include <windows.h>
#define  DIRECTINPUT_VERSION (0x0800)   //ビルド時の警告対処用マクロ
#include "d3dx9.h"		//描画処理に必要
#include "xaudio2.h"	//サウンド処理に必要
#include "dinput.h"		//入力処理に必要
#include "Xinput.h"
#include "time.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <vector>
#include "useful.h"

#ifdef _DEBUG
//#define CHECK_MEM_LEAK
#endif // _DEBUG

//普段使用禁止
#ifdef CHECK_MEM_LEAK
#define new new ( NORMALBLOCK , FILE , __LINE )
#endif // CHECK_MEM_LEAK

#endif // _PRECOMPILE_H_