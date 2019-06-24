#pragma once

//システム系のヘッダーインクルード
#include <d3d11.h>
#include <D3DX10math.h>

//Gameシステム用ヘッダー(自作)インクルード
#include "TextureClass.h"

using namespace std;

//前提定義
class TextureClass;

//2D画像描画用
class BitmapClass
{
private:
	//頂点情報(シェーダーのものと同じになければならない)
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();
	//初期化関数
	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight);
	//シャットダウン関数
	void Shutdown();
	//描画関数
	bool Render(ID3D11DeviceContext*,int,int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	//初期化関数()
	bool InitalizeBuffers(ID3D11Device*);
	//シャットダウン関数(バッファ専用)
	void ShutdownBuffers();
	//頂点バッファの内容を更新し、必要に応じて2Dビットマップイメージを画面上に再配置
	bool UpdateBuffers(ID3D11DeviceContext*, int positionX, int positionY);
	//描画関数(GPUの入力アセンブラで頂点、インデックスバッファをアクティブに設定する)
	void RenderBuffers(ID3D11DeviceContext*);
	//ファイル名からテクスチャを読み込む
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertex_buffer;	//頂点バッファ
	ID3D11Buffer* m_index_buffer;	//インデックスバッファ
	int m_vertex_count;	//頂点の数
	int m_index_count;	//インデックスの数
	TextureClass* m_texture;	//テクスチャ情報
	int m_screen_width, m_screen_height;	//画面のサイズ
	int m_bitmap_width, m_bitmap_height;	//ビットマップのサイズ
	int m_previous_posX,m_previous_posY;	//1フレーム前のレンダリング位置
};