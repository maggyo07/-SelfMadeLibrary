
//Gameシステム用ヘッダー(自作)インクルード
#include "TextClass.h"


TextClass::TextClass()
{
	m_font = nullptr;
	m_font_shader = nullptr;

	m_sentence1 = nullptr;
	m_sentence2 = nullptr;
}

TextClass::TextClass(const TextClass& other)
{

}

TextClass::~TextClass()
{

}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd,
int screenWidth, int screenHeight, D3DXMATRIX beseViewMatrix)
{
	//結果情報格納用
	bool result;

	//画面の幅と高さを格納
	m_screen_width = screenWidth;
	m_screen_height = screenHeight;

	//ベースビュー行列を格納
	m_base_view_matrix = beseViewMatrix;

	//フォントオブジェクト作成
	m_font = new FontClass;
	if (!m_font)
		return false;

	//フォントオブジェクトを初期化
	result = m_font->Initialize(device, "fontdata.txt",L"font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"フォントオブジェクトを初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//フォントシェーダオブジェクト作成
	m_font_shader = new FontShaderClass;
	if (!m_font_shader)
		return false;

	//フォントシェーダオブジェクト初期化
	result = m_font_shader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"フォントシェーダオブジェクトを初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//最初の文字を初期化
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result)
		return false;

	//文字の頂点バッファを新しい文字列情報で更新
	result = UpdataSentence(m_sentence1, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	//最初の文字を初期化
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result)
		return false;

	//文の頂点バッファを新しい文字列情報で更新
	result = UpdataSentence(m_sentence2, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
		return false;

	return true;
}

void TextClass::Shutdown()
{
	//最初の文を解放
	ReleaseSentence(&m_sentence1);

	//２番目の文を解放
	ReleaseSentence(&m_sentence2);

	//フォントシェーダオブジェクトを解放
	if (m_font_shader)
	{
		m_font_shader->Shutdown();
		delete m_font_shader;
		m_font_shader = nullptr;
	}

	//フォントオブジェクトを解放
	if (m_font)
	{
		m_font->Shutdown();
		delete m_font;
		m_font = nullptr;
	}
	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthMatrix)
{
	bool result;	//結果情報格納用

	//最初の文を描画
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthMatrix);
	if (!result)
		return false;

	//２番目の文を描画
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthMatrix);
	if (!result)
		return false;

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	bool result;//結果情報格納用

	char temp_string[16];
	char mouse_string[16];
	//mouseX整数を文字列形式に変換
	_itoa_s(mouseX, temp_string, 10);

	//mouseX文字列を設定
	strcpy_s(mouse_string, "MouseX:");
	strcpy_s(mouse_string, temp_string);

	//文字の頂点バッファを新しい文字列情報で更新
	result = UpdataSentence(m_sentence1, mouse_string, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	//mouseY整数を文字列形式に変換
	_itoa_s(mouseY, temp_string, 10);

	//mouseY文字列を設定
	strcpy_s(mouse_string, "MouseY:");
	strcpy_s(mouse_string, temp_string);

	//文字の頂点バッファを新しい文字列情報で更新
	result = UpdataSentence(m_sentence2, mouse_string, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;									//頂点
	unsigned long* indices;									//インデックス
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//頂点、インデックスバッファ
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//頂点、インデックスデータ
	HRESULT result;											//結果情報格納用

	//新しい文オブジェクトを作成
	*sentence = new SentenceType;
	if (!*sentence)
		return false;

	//文のバッファをnulptrに初期化
	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;

	//文の最大長を設定
	(*sentence)->maxLength = maxLength;

	//頂点配列の頂点数を設定
	(*sentence)->vertexCount = 6 * maxLength;

	//インデックス配列内のインデックスの数を設定
	(*sentence)->indexCount = (*sentence)->vertexCount;

	//頂点配列を作成
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
		return false;

	//インデックス配列を作成
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
		return false;

	//最初に頂点配列をゼロに初期化する
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	//インデックス配列を初期化
	for (int i = 0; i < (*sentence)->indexCount; i++)
		indices[i] = i;

	//動的頂点バッファの記述を設定
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;									//バッファで想定されている読み込み及び書き込みの方法
	vertex_buffer_desc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;	//バッファのサイズ
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;						//バッファをどのようにパイプラインにバインドするか
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						//CPUアクセスフラグ
	vertex_buffer_desc.MiscFlags = 0;												//その他のフラグ
	vertex_buffer_desc.StructureByteStride = 0;										//構造体が構造化バッファを表す場合、その構造体のサイズ

	//subresource構造体に頂点データのポインタを与える
	vertex_data.pSysMem = vertices;	//初期化データへのポインタ
	vertex_data.SysMemPitch = 0;		//テクスチャにある１本の線の先端から隣の線までの距離(テクスチャデータのみで使用される)
	vertex_data.SysMemSlicePitch = 0;		//１つの深度レベルの先端から隣の深度レベルまでの距離(3Dテクスチャデータのみで使われる)

	//頂点バッファを作成
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &(*sentence)->vertexBuffer);
	if (FAILED(result))
		return false;

	//動的インデックスバッファの記述を設定
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;									//バッファで想定されている読み込み及び書き込みの方法
	index_buffer_desc.ByteWidth = sizeof(unsigned long)* (*sentence)->indexCount;	//バッファのサイズ
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;						//バッファをどのようにパイプラインにバインドするか
	index_buffer_desc.CPUAccessFlags = 0;											//CPUアクセスフラグ
	index_buffer_desc.MiscFlags = 0;												//その他のフラグ
	index_buffer_desc.StructureByteStride = 0;										//構造体が構造化バッファを表す場合、その構造体のサイズ

	//subresource構造体に頂点データのポインタを与える
	index_data.pSysMem = indices;	//初期化データへのポインタ
	index_data.SysMemPitch = 0;		//テクスチャにある１本の線の先端から隣の線までの距離(テクスチャデータのみで使用される)
	index_data.SysMemSlicePitch = 0;		//１つの深度レベルの先端から隣の深度レベルまでの距離(3Dテクスチャデータのみで使われる)

	//インデックスバッファを作成
	result = device->CreateBuffer(&index_buffer_desc, &index_data, &(*sentence)->indexBuffer);
	if (FAILED(result))
		return false;

	//頂点配列が不要になったら解放
	delete[] vertices;
	vertices = nullptr;

	//インデックス配列を解放
	delete[] indices;
	indices = nullptr;


	return true;
}

bool TextClass::UpdataSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
	ID3D11DeviceContext* deviceContext)
{
	int num_letters;			//textの文字数
	VertexType* vertices;		//上書きする頂点配列
	float drawX, drawY;			//描画位置 スクリーン座標
	HRESULT result;				//結果情報格納用
	D3D11_MAPPED_SUBRESOURCE mapped_resource;//シェーダのデータ
	VertexType* vertices_ptr;	//シェーダの頂点配列へのポインタ

	//文の色を格納
	sentence->red	= red;
	sentence->green = green;
	sentence->blue	= blue;

	//文章中の文字数を取得
	num_letters = (int)strlen(text);

	//可能性のあるバッファオーバーフローをチェック
	if (num_letters > sentence->maxLength)
		return false;

	//頂点配列を作成
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
		return false;

	//最初に頂点配列をゼロに初期化する
	memset(vertices, 0, (sizeof(VertexType)* sentence->vertexCount));

	//描画を開始する画面上のXピクセル位置とYピクセル位置を計算する
	drawX = (float)(((m_screen_width / 2) * -1) + positionX);
	drawY = (float)((m_screen_height / 2) - positionY);

	//フォントクラスを使用して、文テキストと文描画位置から頂点配列を構成
	m_font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	//書き込むことができすように頂点バッファをロック
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//頂点バッファ内のデータへのポインタを取得
	vertices_ptr = (VertexType*)mapped_resource.pData;

	//データを頂点バッファにコピー
	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType)* sentence->vertexCount));

	//頂点バッファのロックを解除
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	//頂点配列が不要になったら解放
	delete[] vertices;
	vertices = nullptr;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		//文の頂点バッファを解放
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = nullptr;
		}

		//文のインデックスバッファを解放
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = nullptr;
		}

		//文を解放する
		delete *sentence;
		*sentence = nullptr;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
	D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixel_color;	//ピクセルカラー
	bool result;				//結果情報格納用

	//頂点バッファのストライドとオフセットを設定
	stride = sizeof(VertexType);
	offset = 0;

	//レンダリングできるように、入力アセンブラで頂点バッファをアクティブに設定
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	//入力アセンブラでインデックスバッファをアクティブに設定して、レンダリングでいるようにする
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//この頂点バッファからレンダリングするプリミティブの方を設定。この場合は三角形
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//入力文の色でピクセルカラーベクトルを作成する
	pixel_color = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	//フォントシェーダを使用してテキストをレンダリングする
	result = m_font_shader->Render(deviceContext, sentence->indexCount, worldMatrix, m_base_view_matrix, orthoMatrix, m_font->GetTexture(),
				pixel_color);
	if (!result)
		return false;

	return true;
}