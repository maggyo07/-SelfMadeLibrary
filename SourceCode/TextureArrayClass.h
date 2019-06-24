#pragma once

//システム系のヘッダーインクルード
#include <d3d11.h>
#include <D3DX11tex.h>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;



//複数のテクスチャ読み込み用クラス
class TextureArrayClass
{
	struct Texture
	{
	public:
		Texture();
		~Texture();
		bool Initialize(const WCHAR* file_name);
		ID3D11ShaderResourceView* GetTexture();
	private:
		ID3D11ShaderResourceView* m_texture;//テクスチャデータ
	};
public:
	TextureArrayClass();
	~TextureArrayClass();
	//テクスチャを読み込み
	bool LoadTexture(const WCHAR* file_name);

	//テクスチャの配列の先頭ポインタを返す
	//※必ず使い終わったらdelete[]してください。
	ID3D11ShaderResourceView** GetTextureArray(int* out_count);
public:
	//テクスチャたち
	vector<shared_ptr<Texture>>* m_textures;

};