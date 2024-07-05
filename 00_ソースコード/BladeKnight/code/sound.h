//=============================================================================
//
// サウンド処理 [sound.h]
// Author : MORIKAWA SHUNYA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound
{
public:
	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM_GAME = 0,	//ゲームBGM
		SOUND_LABEL_SE_WALK,		//プレイヤーの歩行時SE
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// サウンド情報の構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *m_pFilename;	// ファイル名
		int m_nCntLoop;		// ループカウント
	} SOUNDINFO;

	CSound();	//コンストラクタ
	~CSound();	//デストラクタ

	//メンバ関数
	HRESULT Init(HWND hWnd);		//初期化
	void Uninit(void);				//終了
	HRESULT PlaySound(SOUND_LABEL label);	//再生
	void Stop(SOUND_LABEL label);			//停止
	void Stop(void);	//すべて停止

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
private:
	//メンバ変数
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{ "data\\BGM\\cave.wav", -1 },		//ゲームBGM
		{ "data\\SE\\walk.wav", 0 },		//プレイヤーのヒット音
	};
};

#endif