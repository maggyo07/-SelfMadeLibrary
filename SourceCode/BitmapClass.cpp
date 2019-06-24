//Gameシステム用ヘッダー(自作)インクルード
#include "BitmapClass.h"

BitmapClass::BitmapClass()
{
	m_vertex_buffer = nullptr;
	m_index_buffer  = nullptr;
	m_texture = nullptr;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{

}

BitmapClass::~BitmapClass()
{

}

//初期化関数
bool BitmapClass::Initialize(ID3D11Device* device,int screenWidth,int screenHeight,WCHAR* textureFilename,int bitmapWidth,int bitmapHeight)
{
	//画像サイズを保存する
	m_screen_width = screenWidth;
	m_screen_height = screenHeight;

	//このビットマップをレンダリングするピクセル単位のサイズを格納
	m_bitmap_width = bitmapWidth;
	m_bitmap_height = bitmapHeight;

	//前のレンダリング位置を負の値に初期化
	m_previous_posX = -1;
	m_previous_posY = -1;

	//頂点バッファとインデックスバッファを初期化
	if (!InitalizeBuffers(device))
		return false;

	//このモデルのテクスチャをロードする
	if (!LoadTexture(device, textureFilename))
		return false;

	return true;
}

//シャットダウン関数
void BitmapClass::Shutdown()
{
	//モデルテクスチャを解放
	ReleaseTexture();

	//頂点バッファとインデックスバッファを解放
	ShutdownBuffers();

	return;
}

//描画関数
 bool BitmapClass::Render(ID3D11DeviceContext* deviceContext,int positionX, int positionY)
{
	//レンダリングのために動的頂点バッファを再構築して、おそらく画面上の別の場所にします。
	if (!UpdateBuffers(deviceContext, positionX, positionY))
		return false;

	//頂点バッファとインデックスバッファをグラフィックパイプラインに配置して、描画の準備
	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_index_count;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_texture->GetTexture();
}

//初期化関数(モデルの読み込み、バッファを作成)
bool BitmapClass::InitalizeBuffers(ID3D11Device* device)
{
	VertexType* vertexs;	//頂点情報格納用
	unsigned long* indices;	//インデックス情報格納用
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//頂点、インデックスバッファを作成するのに使用する変数
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//頂点、インデックスバッファを作成するのに使用する変数
	HRESULT result;			//正常に行われたかどうかの確認用

	//-----------4角形の板ポリゴン作成-----------
	//頂点配列の頂点の数を設定
	m_vertex_count = 6;
	//インデックス配列内のインデックスの数を設定
	m_index_count = m_vertex_count;

	//頂点配列を作成
	vertexs = new VertexType[m_vertex_count];
	if (!vertexs)
		return false;

	//インデックス配列を作成
	indices = new unsigned long[m_index_count];
	if (!indices)
		return false;

	//最初に頂点配列をゼロに初期化する
	memset(vertexs, 0, (sizeof(VertexType) * m_vertex_count));
	
	//インデックス配列をデータでロードする
	for (int i = 0; i < m_index_count; i++)
		indices[i] = i;
	//-------------------------------------------
	//動的頂点バッファの説明を設定
	vertex_buffer_desc.Usage				= D3D11_USAGE_DYNAMIC;					//バッファで想定されている読み込み及び書き込みの方法
	vertex_buffer_desc.ByteWidth			= sizeof(VertexType)* m_vertex_count;	//バッファのサイズ
	vertex_buffer_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;				//バッファをどのようにパイプラインにバインドするか
	vertex_buffer_desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;				//CPUアクセスフラグ
	vertex_buffer_desc.MiscFlags			= 0;									//その他のフラグ
	vertex_buffer_desc.StructureByteStride	= 0;									//構造体が構造化バッファを表す場合、その構造体のサイズ

	//subresource構造体に頂点データのポインタを与える
	vertex_data.pSysMem				= vertexs;	//初期化データへのポインタ
	vertex_data.SysMemPitch			= 0;		//テクスチャにある１本の線の先端から隣の線までの距離(テクスチャデータのみで使用される)
	vertex_data.SysMemSlicePitch	= 0;		//１つの深度レベルの先端から隣の深度レベルまでの距離(3Dテクスチャデータのみで使われる)

	//頂点バッファを作成
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
	if (FAILED(result))
		return false;

	//静的インデックスバッファの説明を設定
	index_buffer_desc.Usage					= D3D11_USAGE_DEFAULT;					//バッファで想定されている読み込み及び書き込みの方法
	index_buffer_desc.ByteWidth				= sizeof(unsigned long)* m_index_count;	//バッファのサイズ
	index_buffer_desc.BindFlags				= D3D11_BIND_INDEX_BUFFER;				//バッファをどのようにパイプラインにバインドするか
	index_buffer_desc.CPUAccessFlags		= 0;									//CPUアクセスフラグ
	index_buffer_desc.MiscFlags				= 0;									//その他のフラグ
	index_buffer_desc.StructureByteStride	= 0;									//構造体が構造化バッファを表す場合、その構造体のサイズ

	//サブリソース構造体にインデックスデータへのポインタを返します。
	index_data.pSysMem			= indices;	//初期化データへのポインタ
	index_data.SysMemPitch		= 0;		//テクスチャにある１本の線の先端から隣の線までの距離(テクスチャデータのみで使用される)
	index_data.SysMemSlicePitch	= 0;		//１つの深度レベルの先端から隣の深度レベルまでの距離(3Dテクスチャデータのみで使われる)

	//インデックスバッファを作成
	result = device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
	if (FAILED(result))
		return false;

	//頂点とインデックスのバッファが作成されロードされたので、配列を解放
	delete[] vertexs;
	vertexs = 0;

	delete[] indices;
	indices = 0;

	return true;
}

//シャットダウン関数(バッファ専用)
void BitmapClass::ShutdownBuffers()
{
	//インデックスバッファを解放
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = 0;
	}

	//頂点バッファを解放
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = 0;
	}

	return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left,right, top, bottom;				//ビットマップの各画面の座標
	VertexType* vertices;						//更新する頂点情報
	D3D11_MAPPED_SUBRESOURCE mapped_resource;	//頂点バッファの情報
	VertexType* vertices_ptr;					//頂点バッファ内へのポインタ
	HRESULT result;								//動作確認用

	//前のフレームと位置に変化がなければ、頂点バッファを更新しない
	if ((positionX == m_previous_posX) && (positionY == m_previous_posY))
		return true;

	//変更されている場合は、レンダリングされる位置を更新
	m_previous_posX = positionX;
	m_previous_posY = positionY;

	//ビットマップの左側の画面座標を計算
	left = (float)((m_screen_width / 2)*-1) + (float)positionX;

	//ビットマップの右側の画面座標を計算
	right = left + (float)m_bitmap_width;

	//ビットマップの上部の画面座標を計算
	top = (float)(m_screen_height / 2) - (float)positionY;

	//ビットマップの下部の画像座標を計算
	bottom = top - (float)m_bitmap_height;

	//頂点配列を作成
	vertices = new VertexType[m_vertex_count];
	if (!vertices)
		return false;

	//頂点配列にデータをロードする
	//最初の三角形
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);	//左上
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);//右下
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);	//左下
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	//２番目の三角形
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);	//左上
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);	//右上
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);//右下
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	//書き込むことができるように頂点バッファをロック
	result = deviceContext->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//頂点バッファ内のデータへのポインタを取得
	vertices_ptr = (VertexType*)mapped_resource.pData;

	//データを頂点バッファにコピーする
	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType)*m_vertex_count));

	//頂点バッファのロックを解除
	deviceContext->Unmap(m_vertex_buffer, 0);

	//頂点配列が不要になったら解放
	delete[] vertices;
	vertices = nullptr;

	return true;
}
//描画関数(GPUの入力アセンブラで頂点、インデックスバッファをアクティブに設定する)
void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//ストライド
	unsigned int stride;
	//オフセット(基準点から距離で表した位置)
	unsigned int offset;

	//頂点バッファのストライドとオフセットを設定
	stride = sizeof(VertexType);
	offset = 0;

	//レンダリングできるように、入力アセンブラで頂点バッファをアクティブに設定
	deviceContext->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	//入力アセンブラでインデックスバッファをアクティブに設定して、レンダリングできるようにする
	deviceContext->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//この頂点バッファからレンダリングするプリミティブの型を設定する。この場合は三角形です
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return;

}
//ファイル名からテクスチャを読み込む
bool BitmapClass::LoadTexture(ID3D11Device* device,WCHAR* filename)
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
void BitmapClass::ReleaseTexture()
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
