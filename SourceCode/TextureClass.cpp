

//Gameシステム用ヘッダー(自作)インクルード
#include "TextureClass.h"

#include "AccessController.h"

using Graphics::D3D;

#pragma region DrawTexture2DClass
DrawTexture2DClass::DrawTexture2DClass()	:m_vertex_buffer(nullptr),m_index_buffer(nullptr),m_texture_shader(nullptr)
											,m_texture_list(new vector<shared_ptr<TextureData>>)
{

}
//初期化
bool DrawTexture2DClass::Initialize(int window_width,int window_height)
{
	//ウィンドウのサイズ情報を設定
	m_screen_width = window_width;
	m_screen_height = window_height;

	//頂点バッファとインデックスバッファを初期化
	if (!InitalizeBuffers())
		return false;

	return true;
}
//初期化関数(バッファを作成)
bool DrawTexture2DClass::InitalizeBuffers()
{
	VertexType* vertexs;	//頂点情報格納用
	unsigned long* indices;	//インデックス情報格納用
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//頂点、インデックスバッファを作成するのに使用する変数
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//頂点、インデックスバッファを作成するのに使用する変数
	HRESULT result;			//正常に行われたかどうかの確認用

	//-----------4角形の板ポリゴン作成-----------
	//頂点配列の頂点の数を設定
	m_vertex_count = 4;
	//インデックス配列内のインデックスの数を設定
	m_index_count = 6;

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
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 1; indices[4] = 2; indices[5] = 3;
	//-------------------------------------------
	//動的頂点バッファの説明を設定
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;					//バッファで想定されている読み込み及び書き込みの方法
	vertex_buffer_desc.ByteWidth = sizeof(VertexType)* m_vertex_count;	//バッファのサイズ
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//バッファをどのようにパイプラインにバインドするか
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//CPUアクセスフラグ
	vertex_buffer_desc.MiscFlags = 0;									//その他のフラグ
	vertex_buffer_desc.StructureByteStride = 0;									//構造体が構造化バッファを表す場合、その構造体のサイズ

																				//subresource構造体に頂点データのポインタを与える
	vertex_data.pSysMem = vertexs;	//初期化データへのポインタ
	vertex_data.SysMemPitch = 0;		//テクスチャにある１本の線の先端から隣の線までの距離(テクスチャデータのみで使用される)
	vertex_data.SysMemSlicePitch = 0;		//１つの深度レベルの先端から隣の深度レベルまでの距離(3Dテクスチャデータのみで使われる)

											//頂点バッファを作成
	result = D3D().GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
	if (FAILED(result))
		return false;

	//静的インデックスバッファの説明を設定
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;					//バッファで想定されている読み込み及び書き込みの方法
	index_buffer_desc.ByteWidth = sizeof(unsigned long)* m_index_count;	//バッファのサイズ
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;				//バッファをどのようにパイプラインにバインドするか
	index_buffer_desc.CPUAccessFlags = 0;									//CPUアクセスフラグ
	index_buffer_desc.MiscFlags = 0;									//その他のフラグ
	index_buffer_desc.StructureByteStride = 0;									//構造体が構造化バッファを表す場合、その構造体のサイズ

																				//サブリソース構造体にインデックスデータへのポインタを返します。
	index_data.pSysMem = indices;	//初期化データへのポインタ
	index_data.SysMemPitch = 0;		//テクスチャにある１本の線の先端から隣の線までの距離(テクスチャデータのみで使用される)
	index_data.SysMemSlicePitch = 0;		//１つの深度レベルの先端から隣の深度レベルまでの距離(3Dテクスチャデータのみで使われる)

	//インデックスバッファを作成
	result = D3D().GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
	if (FAILED(result))
		return false;

	//頂点とインデックスのバッファが作成されロードされたので、配列を解放
	delete[] vertexs;
	vertexs = 0;

	delete[] indices;
	indices = 0;

	return true;
}
//削除関数
void DrawTexture2DClass::Delete()
{
	//バッファを破棄する
	DeleteBuffer();
	//登録したテクスチャデータを破棄
	DeleteTextures();
}
//削除関数(バッファ専用)
void DrawTexture2DClass::DeleteBuffer()
{
	//インデックスバッファを解放
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = nullptr;
	}

	//頂点バッファを解放
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = nullptr;
	}
}
//登録したテクスチャ情報を破棄する
void DrawTexture2DClass::DeleteTextures()
{
	if (m_texture_list)
	{
		m_texture_list->clear();
		delete m_texture_list;
	}
}
//頂点バッファの内容を更新し、必要に応じて2Dビットマップイメージを画面上に再配置
bool DrawTexture2DClass::UpdateBuffers(int id,float positionX,float positionY,float sizeX,float sizeY, const D3DXCOLOR& color,float r,int dev_id)
{
	float left, right, top, bottom;				//ビットマップの各画面の座標
	int dev_all_num, dev_x_num, dev_y_num, dev_x_size, dev_y_size;//分割データ
	bool dev_flag;								//分割データがあるかどうかのフラグ
	VertexType* vertices;						//更新する頂点情報
	D3D11_MAPPED_SUBRESOURCE mapped_resource;	//頂点バッファの情報
	VertexType* vertices_ptr;					//頂点バッファ内へのポインタ
	HRESULT result;								//動作確認用
	weak_ptr<TextureData> texture_data;			//idで検索してヒットしたテクスチャデータ
	//float texture_sizeX, texture_sizeY;			//テクスチャのサイズ

	//検索
	for each (auto itr in *m_texture_list)
	{
		if (itr->GetID() == id)
		{
			texture_data = itr;
			break;
		}
	}
	//検索失敗
	if (texture_data.expired())
		return false;

	//ビットマップの左側の画面座標を計算
	left = (float)((m_screen_width / 2)*-1) + positionX;

	//ビットマップの右側の画面座標を計算
	if (abs(sizeX) > 1.0f)
		right = left + sizeX;
	else
		right = left + texture_data.lock()->GetTextureClass()->GetWidth();

	//ビットマップの上部の画面座標を計算
	top = (float)(m_screen_height / 2) - positionY;

	//ビットマップの下部の画像座標を計算
	if(abs(sizeY) > 1.0f)
		bottom = top - sizeY;
	else
		bottom = top - texture_data.lock()->GetTextureClass()->GetHeight();

	//頂点配列を作成
	vertices = new VertexType[m_vertex_count];
	if (!vertices)
		return false;
	//分割データ読込
	dev_flag = texture_data.lock()->GetSplit(&dev_all_num, &dev_x_num, &dev_y_num, &dev_x_size, &dev_y_size);

	//頂点配列の位置データを設定
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);	//左上
	vertices[1].position = D3DXVECTOR3(right, top, 0.0f);	//右上
	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);	//左下
	vertices[3].position = D3DXVECTOR3(right, bottom, 0.0f);//右下

	//頂点配列のUVデータを設定
	if (dev_flag)
	{
		double aa = (double)(dev_id % (dev_x_num)*dev_x_size) /(double)texture_data.lock()->GetTextureClass()->GetWidth();
		float a1 = dev_id % (10);
		float a = ((float)(dev_id % (dev_x_num)*dev_x_size) / (float)texture_data.lock()->GetTextureClass()->GetWidth());
		vertices[0].texture = D3DXVECTOR2((float)(dev_id % (dev_x_num)*dev_x_size / (float)texture_data.lock()->GetTextureClass()->GetWidth()),
										 (float)(dev_id / (dev_x_num)*dev_y_size / (float)texture_data.lock()->GetTextureClass()->GetHeight()));	//左上
		vertices[1].texture = vertices[0].texture + D3DXVECTOR2((float)dev_x_size / (float)texture_data.lock()->GetTextureClass()->GetWidth(), 0.0f);  //右上
		vertices[2].texture = vertices[0].texture + D3DXVECTOR2(0.0f, (float)dev_y_size / (float)texture_data.lock()->GetTextureClass()->GetHeight());	//左下
		vertices[3].texture = vertices[0].texture + D3DXVECTOR2((float)dev_x_size / (float)texture_data.lock()->GetTextureClass()->GetWidth(), (float)dev_y_size / (float)texture_data.lock()->GetTextureClass()->GetHeight());	//右下
		for (int i = 0; i < 4; i++)
		{
			vertices[i].texture.x = min(max(0.0f, vertices[i].texture.x), 1.0f);
			vertices[i].texture.y = min(max(0.0f, vertices[i].texture.y), 1.0f);
		}
	}
	else
	{
		vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);	//左上
		vertices[1].texture = D3DXVECTOR2(1.0f, 0.0f);  //右上
		vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);	//左下
		vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);	//右下
		
	}

	//書き込むことができるように頂点バッファをロック
	result = D3D().GetDeviceContext()->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//頂点バッファ内のデータへのポインタを取得
	vertices_ptr = (VertexType*)mapped_resource.pData;

	//データを頂点バッファにコピーする
	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType)*m_vertex_count));

	//頂点バッファのロックを解除
	D3D().GetDeviceContext()->Unmap(m_vertex_buffer, 0);

	//頂点配列が不要になったら解放
	delete[] vertices;
	vertices = nullptr;

	return true;
}
//描画関数(GPUの入力アセンブラで頂点、インデックスバッファをアクティブに設定する)
void DrawTexture2DClass::RenderBuffers()
{
	//ストライド
	unsigned int stride;
	//オフセット(基準点から距離で表した位置)
	unsigned int offset;

	//頂点バッファのストライドとオフセットを設定
	stride = sizeof(VertexType);
	offset = 0;

	//レンダリングできるように、入力アセンブラで頂点バッファをアクティブに設定
	D3D().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	//入力アセンブラでインデックスバッファをアクティブに設定して、レンダリングできるようにする
	D3D().GetDeviceContext()->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//この頂点バッファからレンダリングするプリミティブの型を設定する。この場合はトライアングル ストリップです
	D3D().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return;

}
//描画関数
bool DrawTexture2DClass::Render(int id,float positionX,float positionY,float sizeX,float sizeY,const D3DXCOLOR& color,float r,int dev_id)
{
	//レンダリングのために動的頂点バッファを再構築して、おそらく画面上の別の場所にします。
	if (!UpdateBuffers(id, positionX, positionY,sizeX,sizeY,color,r,dev_id))
		return false;

	//頂点バッファとインデックスバッファをグラフィックパイプラインに配置して、描画の準備
	RenderBuffers();

	return true;
}

//テクスチャ情報を登録
bool DrawTexture2DClass::LoadTexture(int id,const WCHAR* file_name)
{
	//すでに登録しているIDと一緒なら終了
	for each (auto texture in *m_texture_list)
	{
		if (texture->GetID() == id)
			return false;
	}

	//リストに追加するテクスチャデータ
	shared_ptr<TextureData> add_texture_data(new TextureData(id));

	//テクスチャデータ初期化
	if (!add_texture_data->Initialize(file_name))
		return false;

	//リストに追加
	m_texture_list->push_back(move(add_texture_data));

	return true;
}
/*　テクスチャ情報を登録
	引数3 int	all_num	:画像の分割総数
	引数4 int	x_num	:画像の横向きに対する分割数
	引数5 int	y_num	:画像の縦向きに対する分割数
	引数6 int	x_size	:分割された1つの大きさ(X)
	引数7 int	y_size	:分割された1つの大きさ(Y)
	＊分割をした場合、表示するために必要な分割IDは以下のように割り振られる
	XNum = 4 ; YNum = 4 ; AllNum = 14 ;
	0	1	2	3
	4	5	6	7
	8	9	10	11
	12	13
*/
bool DrawTexture2DClass::LoadDevTexture(int id, const WCHAR* file_name, int all_num, int x_num, int y_num, int x_size, int y_size)
{
	//すでに登録しているIDと一緒なら終了
	for each (auto texture in *m_texture_list)
	{
		if (texture->GetID() == id)
			return false;
	}

	//リストに追加するテクスチャデータ
	shared_ptr<TextureData> add_texture_data(new TextureData(id));

	//テクスチャデータ初期化
	if (!add_texture_data->Initialize(file_name))
		return false;
	
	//分割情報を設定
	add_texture_data->SetSplit(all_num, x_num, y_num, x_size, y_size);

	//リストに追加
	m_texture_list->push_back(move(add_texture_data));

	return true;
}
int DrawTexture2DClass::GetIndexCount()
{
    return m_index_count;
}
ID3D11ShaderResourceView* DrawTexture2DClass::GetTexture(int id)
{
	for each (auto itr in *m_texture_list)
	{
		if (itr->GetID() == id)
			return itr->GetTextureClass()->GetTexture();
	}

	return nullptr;
}
#pragma endregion

#pragma region TextureData

TextureData::TextureData(int id)
{
	m_id = id;
	m_split_data = nullptr;
}
TextureData::~TextureData()
{
	//テクスチャ情報を削除
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}

	//分割用変数を削除
	if (m_split_data)
	{
		delete m_split_data;
	}
}
bool TextureData::Initialize(const WCHAR* file_name)
{
	//テクスチャオブジェクトを作成
	m_texture = new TextureClass;
	if (!m_texture)
		return false;

	//テクスチャオブジェクトを初期化
	if (!m_texture->Initialize(file_name))
		return false;

	return true;
}
/*　分割データを設定する
引数1 all_num	:画像の分割総数
引数2 x_num		:分割する際のX方向にいくつ分割するかの数
引数3 y_num		:分割する際のY方向にいくつ分割するかの数
引数4 x_size	:分割された1つの大きさ(X)
引数5 y_size	:分割された1つの大きさ(Y)
*/
void TextureData::SetSplit(int all_num, int x_num, int y_num, int x_size, int y_size)
{
	if (m_split_data == nullptr)
		m_split_data = new SplitData;

	m_split_data->all_num = all_num;
	m_split_data->x_num = x_num;
	m_split_data->y_num = y_num;
	m_split_data->x_size = x_size;
	m_split_data->y_size = y_size;
}
//戻り値 bool	:true = 分割データがある :false = 分割データが無い
bool TextureData::GetSplit(int* out_all_num, int* out_x_num, int* out_y_num, int* out_x_size, int* out_y_size)
{
	if (m_split_data == nullptr)
		return false;

	if (out_all_num)
		*out_all_num = m_split_data->all_num;
	if (out_x_num)
		*out_x_num = m_split_data->x_num;
	if (out_y_num)
		*out_y_num = m_split_data->y_num;
	if (out_x_size)
		*out_x_size = m_split_data->x_size;
	if (out_y_size)
		*out_y_size = m_split_data->y_size;

	return true;
}
#pragma endregion

#pragma region TextureClass

TextureClass::TextureClass()
{
	m_texture = nullptr;
	m_texture_file_name = nullptr;
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(const WCHAR* filename)
{
	HRESULT result;	//動作確認用変数

	//テクスチャファイル名を保存
	m_texture_file_name = new WCHAR[100];
	wcscpy_s(m_texture_file_name,100, filename);

	//テクスチャをロードする
	result = D3DX11CreateShaderResourceViewFromFile(D3D().GetDevice(), filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"テクスチャが見つかりません。", filename, MB_OK);
		return false;
	}
	//テクスチャのサイズを取得する
	// この構造体の中に画像の情報が入る
	D3DX11_IMAGE_INFO info = {0};
	//画像の情報を取得
	result = D3DX11GetImageInfoFromFile(filename, NULL,&info,NULL);
	if (FAILED(result))	//さすがに無いとは思うが一応
	{
		MessageBox(NULL, L"テクスチャが見つかりません。", filename, MB_OK);
		return false;
	}
	//サイズ情報格納
	m_width = info.Width;
	m_height = info.Height;

	return true;
}

void TextureClass::Shutdown()
{
	//テクスチャリソースを解放
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	if (m_texture_file_name)
	{
		delete[] m_texture_file_name;
		m_texture_file_name = nullptr;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}
#pragma endregion

