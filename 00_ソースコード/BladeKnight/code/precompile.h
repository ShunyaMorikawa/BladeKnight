//========================================
//
//プリコンパイル[precompile.h]
//Author：森川駿弥
//
//========================================
#ifndef PRECOMPILE_H__
#define PRECOMPILE_H__
#include <windows.h>
#define  DIRECTINPUT_VERSION (0x0800)   //ビルド時の警告対処用マクロ
#include "d3dx9.h"		//描画処理に必要
#include "xaudio2.h"	//サウンド処理に必要
#include "dinput.h"		//入力処理に必要
#include "Xinput.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <time.h>

#endif // _PRECOMPILE_H_