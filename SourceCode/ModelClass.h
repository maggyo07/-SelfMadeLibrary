#pragma once

//システム系のヘッダーインクルード
#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>

//Gameシステム用ヘッダー(自作)インクルード
#include "TextureArrayClass.h"

using namespace std;

//3Dモデルのジオメトリをカプセル化
class ModelClass
{
private:
	//頂点情報(シェーダーのものと同じになければならない)
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	struct ModelType
	{
		float x, y, z;		//位置
		float tu, tv;		//テクスチャ座標
		float nx, ny, nz;	//法線
		float tx, ty, tz;	//
		float bx, by, bz;	//
	};

	//タンジェントとバイノーマルの計算に使う
	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	//初期化関数
	bool Initialize(WCHAR* modelFilename);
	//シャットダウン関数
	void Shutdown();
	//ファイル名からテクスチャを読み込む
	bool LoadTexture(WCHAR*);
	//描画関数
	void Render(ID3D11DeviceContext*);

	//モデルの接線ベクトルと二値ベクトルを計算用
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType,D3DXVECTOR3&,D3DXVECTOR3&);
	void CalculateNormal(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3&);

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	//初期化関数(モデルの読み込み、バッファを作成)
	bool InitalizeBuffers(const WCHAR* modelFilename);
	//シャットダウン関数(バッファ専用)
	void ShutdownBuffers();
	//描画関数(GPUの入力アセンブラで頂点、インデックスバッファをアクティブに設定する)
	void RenderBuffers(ID3D11DeviceContext*);


	void ReleaseTexture();

	//ファイルからモデル情報を読み込む
	bool LoadModel(const WCHAR* filename, VertexType*& vertexs, unsigned long*& indices);
	void ReleaseModel();
private:
	ID3D11Buffer* m_vertex_buffer;	//頂点バッファ
	ID3D11Buffer* m_index_buffer;	//インデックスバッファ
	int m_vertex_count;	//頂点の数
	int m_index_count;	//インデックスの数
	TextureArrayClass* m_texture_array;	//テクスチャデータ
	ModelType* m_model;			//モデル情報
};