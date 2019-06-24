#include "TextureArrayClass.h"
#include "AccessController.h"

using Graphics::D3D;

#pragma region TextureArrayClass

#pragma region struct Texture
TextureArrayClass::Texture::Texture()
{
	m_texture = nullptr;
}

TextureArrayClass::Texture::~Texture()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

bool TextureArrayClass::Texture::Initialize(const WCHAR* file_name)
{
	HRESULT result;	//結果

	result = D3DX11CreateShaderResourceViewFromFile(D3D().GetDevice(), file_name, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"テクスチャが見つかりません。", file_name, MB_OK);
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* TextureArrayClass::Texture::GetTexture()
{
	return m_texture;
}
#pragma endregion

TextureArrayClass::TextureArrayClass()
{
	m_textures = new vector<shared_ptr<Texture>>;
}

TextureArrayClass::~TextureArrayClass()
{
	if (m_textures)
	{
		//テクスチャデータの破棄
		//for (auto itr = m_textures->begin(); itr != m_textures->end(); itr++)
		//	itr->get()->Release();

		m_textures->clear();
		delete m_textures;
	}
}

//テクスチャを読み込み
bool TextureArrayClass::LoadTexture(const WCHAR* file_name)
{
	HRESULT result;	//結果

	//配列に追加するテクスチャデータ
	shared_ptr<Texture> add_texture(new Texture);
	//テクスチャをロードする
	if (!add_texture->Initialize(file_name))
		return false;

	//配列に追加
	m_textures->push_back(move(add_texture));

	return true;
}

//テクスチャの配列の先頭ポインタを返す
//※必ず使い終わったらdelete[]してください。
ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray(int* out_count)
{
	//何もなければ終了
	if (m_textures->empty())
		return nullptr;

	//ポインタがあれば数を返す
	if (out_count != nullptr)
		*out_count = (int)m_textures->size();

	//最終的に返す変数
	ID3D11ShaderResourceView** out_data;
	//メモリをセット
	out_data = new ID3D11ShaderResourceView*[m_textures->size()];
	//渡すデータのポインタを格納していく
	int data_count = 0;	//カウント用
	for (auto itr = m_textures->begin(); itr != m_textures->end(); itr++)
	{
		out_data[data_count++] = itr->get()->GetTexture();
	}
	//配列の先頭ポインタを返す
	return out_data;
}

#pragma endregion