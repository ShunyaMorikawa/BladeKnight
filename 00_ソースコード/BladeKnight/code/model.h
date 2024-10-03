//========================================
//
//モデル[model.h]
//Author：森川駿弥
//
//========================================

#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "object.h"

//=======================================
//モデルクラス
//=======================================
class CModel
{
public:
	enum HIERARCHY
	{// 階層持ってるか
		TYPE_HIERARCHY = 0,
		TYPE_NOT_HIERARCHY,
		TYPE_MAX
	};

	CModel();		//コンストラクタ
	~CModel();		//デストラクタ

	//メンバ関数
	HRESULT Init(const char *pFilename);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

	static CModel *Create(const char *pFilename);	//オブジェクト生成

	// 親モデルの設定
	void SetParent(CModel* pModel) { m_pParent = pModel; }

	// ワールドマトリックスの取得
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }	

	// 位置設定・取得
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	// 向き
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	// 位置
	void SetPosition(const D3DXVECTOR3& pos) { m_setPos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_setPos; }

	// モーション時の向きの設定と取得
	void SetRotate(const D3DXVECTOR3& rot) { m_setRot = rot; }
	D3DXVECTOR3 GetRotate(void) { return m_setRot; }

	// 番号設定
	void SetIndex(CModel* pModel) { m_pParent = pModel; }

	// 階層設定
	void SetType(HIERARCHY type) { m_nType = type; }

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);

private:
	//メンバ変数
	HIERARCHY m_nType;

	std::vector<LPDIRECT3DTEXTURE9> m_pTexture;	//共有テクスチャ
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_rot;				//向き
	CModel *m_pParent;				//親モデルへのポインタ

	D3DXVECTOR3 m_setPos;			// モーション時のモデルの位置
	D3DXVECTOR3 m_setRot;			// モーション時のモデルの向き
};

#endif