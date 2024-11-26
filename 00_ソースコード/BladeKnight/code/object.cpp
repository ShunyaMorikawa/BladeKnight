//========================================
//
//オブジェクト管理[object.cpp]
//Author：森川駿弥
//
//========================================

#include "object.h"
#include "camera.h"
#include "manager.h"
#include "texture.h"

//========================================
//静的メンバ変数宣言
//========================================
int CObject::m_nNumAll = 0;
CObject *CObject::m_pTop[NUM_PRIORITY] = {};
CObject *CObject::m_pCurrent[NUM_PRIORITY] = {};

//========================================
//コンストラクタ
//========================================
CObject::CObject(int nPriority)
{//値クリア
	if (m_pTop[nPriority] == nullptr && m_pCurrent[nPriority] == nullptr)
	{//先頭と最後がnullptrの時
		m_pTop[nPriority] = this;	// 先頭のnPriority番目に自身を代入
		m_nNumAll++;				// 総数をカウントアップ
		m_nPriority = nPriority;	// 優先順位
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		m_pNext = nullptr;	// 次
		m_pPrev = nullptr;	// 前
		m_pCurrent[nPriority] = this;	// 最後尾のnPriority番目に自身を代入
		m_bRelease = false;		//終了フラグ
		m_bDrawFrag = false;	// 描画するかしないか
	}
	else if (m_pTop[nPriority] != nullptr && m_pCurrent[nPriority] != nullptr)
	{// 先頭のnPriority番目とm_pCurrent[nPriority]があるとき
		m_pPrev = m_pCurrent[nPriority];	// 前回に最後尾のnPriority番目を代入
		m_pNext = nullptr;	// 次がをnullptrに
		m_pPrev->m_pNext = this;	// 前回のnextに自身代入
		m_pCurrent[nPriority] = this; // m_pCurrent[nPriority]に自身代入
	}
}

//========================================
//デストラクタ
//========================================
CObject::~CObject(void)
{
}

//========================================
//すべてのオブジェクトの破棄
//========================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭のnCntPri番目代入
		CObject* Now = m_pTop[nCntPri];

		CObject* Next;

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				// 次の値を保存
				Next = Now->m_pNext;

				if (Now->m_bDrawFrag != true)
				{// 描画しない
					// 先頭の終了
					Now->Uninit();
				}
				else
				{
					int n = 0;
				}
				
				// 次
				Now = Next;
			}
		}
	}

	// すべて削除
	DeleteAll();
}

//========================================
//すべてのオブジェクトの更新
//========================================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭のnCntPri番目代入
		CObject *Now = m_pTop[nCntPri];

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				// 先頭のnext代入
				CObject* pNext = Now->m_pNext;

				// 先頭の更新
				if (Now->m_bRelease != true)
				{// 削除しない場合
					Now->Update();
				}

				// 次
				Now = pNext;
			}
		}		
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭のnCntPri番目代入
		CObject* pNow = m_pTop[nCntPri];

		while (1)
		{
			if (pNow == nullptr)
			{
				break;
			}
			else if (pNow != nullptr)
			{
				// nextにm_pTopのnextを代入
				CObject* pNext = pNow->m_pNext;

				if (pNow->m_bRelease == true)
				{// 削除するとき
					if (pNow->m_bDrawFrag != true)
					{// 描画しない
						// 先頭の削除
						pNow->Delete();
					}
					else
					{
						int n = 0;
					}
				}

				// 次
				pNow = pNext;
			}
		}
	}
}

//========================================
//すべてのオブジェクトの描画
//========================================
void CObject::DrawAll(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	//カメラ設定
	if (pCamera != nullptr)
	{
		pCamera->SetCamera();
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭代入
		CObject* Now = m_pTop[nCntPri];

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				// 先頭のnextを代入
				CObject* pNext = Now->m_pNext;

				// 先頭の更新
				if (Now->m_bRelease != true)
				{// 削除しない
					if (Now->m_bDrawFrag != true)
					{
						// 先頭の描画
						Now->Draw();
					}
				}

				// 次
				Now = pNext;
			}
		}
	}
}

//========================================
//すべてのオブジェクトの終了
//========================================
void CObject::DeleteAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭代入
		CObject* Now = m_pTop[nCntPri];

		CObject* Next;

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				// 現在のnext代入
				Next = Now->m_pNext;

				if (Now->m_bRelease == true)
				{
					//先頭の終了
					Now->Delete();
				}

				//Nextを代入
				Now = Next;
			}
		}
	}
}

//========================================
//自身の削除
//========================================
void CObject::Delete(void)
{
	// 先頭代入
	CObject *Now = this;
	CObject *pNextObj = Now->m_pNext;	// 次保存
	CObject *pPrevObj = Now->m_pPrev;	// 前回保存

	if (pNextObj != nullptr)
	{// nextの前回に自身の前回代入
		pNextObj->m_pPrev = Now->m_pPrev;
	}

	if (pPrevObj != nullptr)
	{// prevの次回に自身の次回代入
		pPrevObj->m_pNext = Now->m_pNext;
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		if (Now == m_pTop[nCntPri])
		{// 先頭に自身の次回代入
			m_pTop[nCntPri] = Now->m_pNext;
		}

		if (Now == m_pCurrent[nCntPri])
		{// 最後尾に自身の前回代入
			m_pCurrent[nCntPri] = Now->m_pPrev;
		}
	}

	// 自身の削除
	delete this;
}

//========================================
//オブジェクトの破棄
//========================================
void CObject::Release(void)
{
	// 死亡フラグ
	m_bRelease = true;
}

//========================================
// テクスチャの割り当て
//========================================
void CObject::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
// 描画するかしないか
//========================================
void CObject::SetDrawFrag(bool bDraw)
{
	m_bDrawFrag = bDraw;
}
