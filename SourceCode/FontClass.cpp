
//Gameシステム用ヘッダー(自作)インクルード
#include "FontClass.h"

FontClass::FontClass()
{
	m_font = nullptr;
	m_texture = nullptr;
}

FontClass::FontClass(const FontClass& other)
{

}

FontClass::~FontClass()
{

}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	//フォントデータを含むテキストファイルを読み込む
	if (!LoadFontData(fontFilename))
		return false;

	//フォント文字を持つテクスチャを読み込む
	if (!LoadTexture(device, textureFilename))
		return false;

	return true;
}

void FontClass::Shutdown()
{
	//フォントテクスチャを解放
	ReleaseTexture();

	///フォントデータを解放
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;//ファイル読み込み用
	char temp; //文字読み込み用

	//フォント間隔バッファを作成
	m_font = new FontType[95]; //配列のサイズは、テクスチャの文字数
	if (!m_font)
		return false;

	//フォントサイズと文字間
	fin.open(filename);
	if (fin.fail())
		return false;

	//テキストとして使用されている95文字のASCII文字を読み込む
	for (int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	//ファイルを閉じる
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	//フォントデータ配列を解放
	if (m_font)
	{
		delete[] m_font;
		m_font = nullptr;
	}
	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	//テクスチャオブジェクトを作成
	m_texture = new TextureClass;
	if (!m_texture)
		return false;

	//テクスチャオブジェクトを初期化
	if (!m_texture->Initialize(filename))
		return false;

	return true;
}

void FontClass::ReleaseTexture()
{
	//テクスチャオブジェクトを解放
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_texture->GetTexture();
}

//文字文をレンダリングする三角形の頂点配列を返す
//引数1	void* vertices	:返す頂点配列へのポインタ
//引数2 char* sentence	:描画する文字文
//引数3 float drawX		:文を描く場所のスクリーンX座標
//引数4 float drawY		:文を描く場所のスクリーンY座標
void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertex_ptr;	//引数のvertices。型はVertexTypeに強制変換
	int num_letters	//文の文字数
		, index		//vertex_ptrの要素数(カウント用)
		, letter;	//文の1文字取得用

	//入力頂点をVertexType構造体に強制変換
	vertex_ptr = (VertexType*)vertices;

	//表示する文字数を取得
	num_letters = (int)strlen(sentence);

	//インデックスを頂点配列に初期化
	index = 0;

	//各文字をクワッドに描画
	for (int i = 0; i < num_letters; i++)
	{
		//文の1文字を取得
		letter = ((int)sentence[i]) - 32;

		//文字がスペースの場合は、3ピクセル以上移動
		if (letter == 0)
			drawX = drawX + 3.0f;
		else
		{
			//クワッドの最初の三角形
			vertex_ptr[index].possition = D3DXVECTOR3(drawX, drawY, 0.0f);	//左上
			vertex_ptr[index].texture	= D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3((drawX + m_font[letter].size), (drawY-16), 0.0f);	//右下
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3(drawX, (drawY-16), 0.0f);	//左下
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].left, 1.0f);
			index++;

			//クワッドの2番目の三角形
			vertex_ptr[index].possition = D3DXVECTOR3(drawX, drawY, 0.0f);	//左上
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3((drawX + m_font[letter].size), drawY, 0.0f);	//右上
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].right, 0.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);	//右下
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			//文字のサイズと1ピクセルで描画するためのX位置を更新
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}
	return;
}