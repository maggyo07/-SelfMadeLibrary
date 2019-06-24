#pragma once

//システム系のヘッダーインクルード
#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
//Gameシステム用ヘッダー(自作)インクルード
#include "TextureClass.h"
using namespace std;

class FontClass
{
private:
	//フォントインデックスファイルから読み込まれたインデックスデータ保持用
	struct FontType
	{
		float left, right;	//TUテクスチャ座標
		int size;			//文字の幅(ピクセル単位)
	};

	//頂点シェーダのものと一致
	struct VertexType
	{
		D3DXVECTOR3 possition;
		D3DXVECTOR2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	//文字文をレンダリングする三角形の頂点配列を返す
	//引数1	void* vertices	:返す頂点配列へのポインタ
	//引数2 char* sentence	:描画する文字文
	//引数3 float drawX		:文を描く場所のスクリーンX座標
	//引数4 float drawY		:文を描く場所のスクリーンY座標
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	FontType* m_font;
	TextureClass* m_texture;
};