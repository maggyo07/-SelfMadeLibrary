//Gameシステム用ヘッダー(自作)インクルード
#include "ModelClass.h"

#include "AccessController.h"

using Graphics::D3D;

ModelClass::ModelClass()
{
	m_vertex_buffer = nullptr;
	m_index_buffer  = nullptr;
	m_model = nullptr;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

//初期化関数
bool ModelClass::Initialize(WCHAR* modelFilename)
{
	//モデルを読み込み、頂点、インデックスバッファを作成
	if (!InitalizeBuffers(modelFilename))
		return false;
	
	return true;
}

//シャットダウン関数
void ModelClass::Shutdown()
{
	//モデルテクスチャを解放
	ReleaseTexture();

	//頂点バッファとインデックスバッファを解放
	ShutdownBuffers();

	//モデルデータを破棄
	ReleaseModel();

	return;
}

//描画関数
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	//頂点バッファとインデックスバッファをグラフィックパイプラインに配置して、描画の準備
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_index_count;
}

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	if (m_texture_array != nullptr)
		return m_texture_array->GetTextureArray(nullptr);
	else
		return nullptr;
}

//初期化関数(モデルの読み込み、バッファを作成)
bool ModelClass::InitalizeBuffers(const WCHAR* modelFilename)
{
	VertexType* vertexs;	//頂点情報格納用
	unsigned long* indices;	//インデックス情報格納用
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//頂点、インデックスバッファを作成するのに使用する変数
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//頂点、インデックスバッファを作成するのに使用する変数
	HRESULT result;			//正常に行われたかどうかの確認用

	//-----------通常、モデルを読み込むところ。-----------
	vertexs = nullptr; indices = nullptr;
	if (!LoadModel(modelFilename, vertexs, indices))
		return false;
	//モデルの法線ベクトル、接線ベクトル、および双正規ベクトルを計算
	CalculateModelVectors();
	//--------------------------------------------------------------------
	//頂点配列とインデックス配列にデータをロード
	for (int i = 0; i < m_index_count; i++)
	{
		vertexs[i].position = D3DXVECTOR3(m_model[i].x,m_model[i].y, m_model[i].z);
		vertexs[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertexs[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertexs[i].tangent = D3DXVECTOR3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertexs[i].binormal = D3DXVECTOR3(m_model[i].bx, m_model[i].by, m_model[i].bz);

		//indices[i] = i;
	}

	//静的頂点バッファの説明を設定
	vertex_buffer_desc.Usage				= D3D11_USAGE_DEFAULT;					//バッファで想定されている読み込み及び書き込みの方法
	vertex_buffer_desc.ByteWidth			= sizeof(VertexType)* m_vertex_count;	//バッファのサイズ
	vertex_buffer_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;				//バッファをどのようにパイプラインにバインドするか
	vertex_buffer_desc.CPUAccessFlags		= 0;									//CPUアクセスフラグ
	vertex_buffer_desc.MiscFlags			= 0;									//その他のフラグ
	vertex_buffer_desc.StructureByteStride	= 0;									//構造体が構造化バッファを表す場合、その構造体のサイズ

	//subresource構造体に頂点データのポインタを与える
	vertex_data.pSysMem				= vertexs;	//初期化データへのポインタ
	vertex_data.SysMemPitch			= 0;		//テクスチャにある１本の線の先端から隣の線までの距離(テクスチャデータのみで使用される)
	vertex_data.SysMemSlicePitch	= 0;		//１つの深度レベルの先端から隣の深度レベルまでの距離(3Dテクスチャデータのみで使われる)

	//頂点バッファを作成
	result = D3D().GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
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

//シャットダウン関数(バッファ専用)
void ModelClass::ShutdownBuffers()
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

//描画関数(GPUの入力アセンブラで頂点、インデックスバッファをアクティブに設定する)
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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
bool ModelClass::LoadTexture(WCHAR* filename)
{
	//テクスチャデータ配列が作成されていなければ作成する
	if (!m_texture_array)
		m_texture_array = new TextureArrayClass;

	if (!m_texture_array)
		return false;

	//テクスチャをロードする
	if (!m_texture_array->LoadTexture(filename))
		return false;

	return true;
}
void ModelClass::ReleaseTexture()
{
	//テクスチャオブジェクトを解放
	if (m_texture_array)
	{
		delete m_texture_array;
		m_texture_array = nullptr;
	}

	return;
}

//ファイルからモデル情報を読み込む
//現在はファイル形式　objのみ
bool ModelClass::LoadModel(const WCHAR* filename, VertexType*& vertexs,unsigned long*& indices)
{
	ifstream fin;			//ファイルの読み込み用	
	char input;				//ファイルの文字を読み込み用
	char input2;			//OBJファイルの最後の面情報の'/'を読み込むための変数
	D3DXVECTOR3* v_vertexs, *v_texcoords, *v_normal;	//頂点、テクスチャ座標、法線情報格納用
	int vertex_index, texcoord_index, normal_index, face_index;//頂点、テクスチャ座標、法線、面の情報取得時のカウント
	int vertex_count, texture_count, normal_count, face_count;//頂点、テクスチャ座標、法線、面の数

	//面情報格納用(番号)
	typedef struct
	{
		int vindex1, vindex2, vindex3;
		int tindex1, tindex2, tindex3;
		int nindex1, nindex2, nindex3;
	}FaceType;
	FaceType* faces;
	//カウントを初期化
	vertex_count = 0;
	texture_count = 0;
	normal_count = 0;
	face_count = 0;
	vertex_index = 0;
	texcoord_index = 0;
	normal_index = 0;
	face_index = 0;

	//モデルファイルを開く
	fin.open(filename);

	//ファイルをオープンできなかった場合は終了
	if (fin.fail())
		return false;

	//ファイルから読み込み、ファイルの終わりに達するまで読み込み続けます
	fin.get(input);
	while (!fin.eof())
	{
		//行が'v'で始まっている場合は、頂点、テクスチャ座標、または法線ベクトルのいずれかを数える
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertex_count++; }
			if (input == 't') { texture_count++; }
			if (input == 'n') { normal_count++; }
		}

		//行がｆで始まっていれば、面数をインクリメント
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { face_count++; }
		}

		//そうでなければ、行の残りの部分を読み込む
		while (input != '\n')
			fin.get(input);

		//次の行の先端から読み始める
		fin.get(input);
	}
	//ファイルを閉じる
	fin.close();
	
	//６つのデータを初期化
	v_vertexs = new D3DXVECTOR3[vertex_count];
	if (!v_vertexs)
		return false;
	
	v_texcoords = new D3DXVECTOR3[texture_count];
	if (!v_texcoords)
		return false;

	v_normal = new D3DXVECTOR3[normal_count];
	if (!v_normal)
		return false;

	vertexs = new VertexType[face_count*3];
	if (!vertexs)
		return false;

	indices = new unsigned long[face_count * 3];
	if (!indices)
		return false;

	faces = new FaceType[face_count];
	if (!faces)
		return false;

	//メンバの頂点、インデックス数を設定
	m_vertex_count = m_index_count = face_count * 3;

	//ファイルを開く
	fin.open(filename);

	//ファイルオープンに成功したかどうかを確認
	if (fin.fail())
		return false;

	//頂点、テクスチャ座標、法線をデータに読み込み
	//重要：objファイルが右手座標系を使用するので、左手座標系に変換
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			//頂点を読み込む

			if (input == ' ')
			{

				fin >> v_vertexs[vertex_index].x >> v_vertexs[vertex_index].y >> v_vertexs[vertex_index].z;

				//Z頂点を反転させて左手系に変換する
				v_vertexs[vertex_index].z = v_vertexs[vertex_index].z * -1.0f;
				vertex_index++;
			}

			//テクスチャのuv座標を読み込み
			if (input == 't')
			{
				fin >> v_texcoords[texcoord_index].x >> v_texcoords[texcoord_index].y;

				//Vテクスチャ座標を左手系に反転する
				v_texcoords[texcoord_index].y = 1.0f - v_texcoords[texcoord_index].y;
				texcoord_index++;
			}

			//法線を読み込み
			if (input == 'n')
			{
				fin >> v_normal[normal_index].x >> v_normal[normal_index].y >> v_normal[normal_index].z;

				//Z法線を反転して左手系に変換
				v_normal[normal_index].z = v_normal[normal_index].z * -1.0f;
				normal_index++;
			}
		}

		//面を読み込み
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				//右手系から左手系に変換するため、面データを後方に読み込み
				fin >> faces[face_index].vindex3 >> input2 >> faces[face_index].tindex3 >> input2 >> faces[face_index].nindex3
					>> faces[face_index].vindex2 >> input2 >> faces[face_index].tindex2 >> input2 >> faces[face_index].nindex2
					>> faces[face_index].vindex1 >> input2 >> faces[face_index].tindex1 >> input2 >> faces[face_index].nindex1;

				//番号が１～なので０～にするため-1する
				faces[face_index].vindex1 -= 1; faces[face_index].tindex1 -= 1; faces[face_index].nindex1 -= 1;
				faces[face_index].vindex2 -= 1; faces[face_index].tindex2 -= 1; faces[face_index].nindex2 -= 1;
				faces[face_index].vindex3 -= 1; faces[face_index].tindex3 -= 1; faces[face_index].nindex3 -= 1;
				face_index++;
			}
		}

		//残りの行を読み込み
		while (input != '\n')
			fin.get(input);

		//次の行の先端から読み込み始める
		fin.get(input);
	}
	//ファイルを閉じる
	fin.close();

	//頂点情報とインデックス情報を書き込む
	for (int fc=0,vc = 0; fc < face_count; fc++,vc += 3)//fc=面数　vc=頂点数
	{
		vertexs[vc].position = v_vertexs[faces[fc].vindex1];
		vertexs[vc].texture = D3DXVECTOR2(v_texcoords[faces[fc].tindex1]);
		vertexs[vc].normal = v_normal[faces[fc].nindex1];

		vertexs[vc+1].position = v_vertexs[faces[fc].vindex2];
		vertexs[vc+1].texture = D3DXVECTOR2(v_texcoords[faces[fc].tindex2]);
		vertexs[vc+1].normal = v_normal[faces[fc].nindex2];

		vertexs[vc+2].position = v_vertexs[faces[fc].vindex3];
		vertexs[vc+2].texture = D3DXVECTOR2(v_texcoords[faces[fc].tindex3]);
		vertexs[vc+2].normal = v_normal[faces[fc].nindex3];

		indices[vc] = vc;
		indices[vc+1] = vc+1;
		indices[vc+2] = vc+2;
	}
	//メモリ解放
	delete[] v_vertexs;
	v_vertexs = nullptr;

	delete[] v_texcoords;
	v_texcoords = nullptr;

	delete[] v_normal;
	v_normal = nullptr;

	delete[] faces;
	faces = nullptr;


	//m_modelにデータを格納
	//メモリ設定
	if (m_model == nullptr)
		m_model = new ModelType[m_index_count];

	for (int index_count = 0; index_count < m_index_count; index_count++)
	{
		m_model[index_count].x = vertexs[index_count].position.x;
		m_model[index_count].y = vertexs[index_count].position.y;
		m_model[index_count].z = vertexs[index_count].position.z;

		m_model[index_count].nx = vertexs[index_count].normal.x;
		m_model[index_count].ny = vertexs[index_count].normal.y;
		m_model[index_count].nz = vertexs[index_count].normal.z;

		m_model[index_count].tu = vertexs[index_count].texture.x;
		m_model[index_count].tv = vertexs[index_count].texture.y;
	}
	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
}

//モデルの接線と2項正規分布、および再計算された法線ベクトルを作成
//モデル内にいくつの面(三角形)があるかを計算、それらの三角形の
//それぞれについてそれは3つの頂点を得て、それを使って正接、2正規、そして法線を計算。
//これら3つの法線ベクトルを計算した後、それらをモデル構造に保存
void ModelClass::CalculateModelVectors()
{
	int face_count,index;
	TempVertexType vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal, normal;

	//モデル内の面の数を計算
	face_count = m_vertex_count / 3;

	//モデルデータへのインデックスを初期化
	index = 0;

	//全ての面を調べ、接線ベクトル、二項ベクトル。および法線ベクトルを計算
	for (int count = 0; count < face_count; count++)
	{
		//モデルからこの面の3つの頂点を取得する
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		//その面の正接と二項正規分布を計算
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		//タンジェントとバイノーマルを使用して新しい法線を計算
		CalculateNormal(tangent, binormal, normal);

		//この面の法線、接線、および二項正規分布をモデル構造に格納
		m_model[index - 1].nx = normal.x;
		m_model[index - 1].ny = normal.y;
		m_model[index - 1].nz = normal.z;
		m_model[index - 1].tx = tangent.x;
		m_model[index - 1].ty = tangent.y;
		m_model[index - 1].tz = tangent.z;
		m_model[index - 1].bx = binormal.x;
		m_model[index - 1].by = binormal.y;
		m_model[index - 1].bz = binormal.z;

		m_model[index - 2].nx = normal.x;
		m_model[index - 2].ny = normal.y;
		m_model[index - 2].nz = normal.z;
		m_model[index - 2].tx = tangent.x;
		m_model[index - 2].ty = tangent.y;
		m_model[index - 2].tz = tangent.z;
		m_model[index - 2].bx = binormal.x;
		m_model[index - 2].by = binormal.y;
		m_model[index - 2].bz = binormal.z;

		m_model[index - 3].nx = normal.x;
		m_model[index - 3].ny = normal.y;
		m_model[index - 3].nz = normal.z;
		m_model[index - 3].tx = tangent.x;
		m_model[index - 3].ty = tangent.y;
		m_model[index - 3].tz = tangent.z;
		m_model[index - 3].bx = binormal.x;
		m_model[index - 3].by = binormal.y;
		m_model[index - 3].bz = binormal.z;

	}
}

//3つの頂点を取り込んでから、それら3つの頂点の接線と双正規を計算し、返す
void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	D3DXVECTOR3& tangent, D3DXVECTOR3& binormal)
{
	D3DXVECTOR3 vector1, vector2;
	D3DXVECTOR2 tu_vector, tv_vector;
	float den,length;

	//この面の2つのベクトルを計算
	vector1.x = vertex2.x - vertex1.x;
	vector1.y = vertex2.y - vertex1.y;
	vector1.z = vertex2.z - vertex1.z;

	vector2.x = vertex3.x - vertex1.x;
	vector2.y = vertex3.y - vertex1.y;
	vector2.z = vertex3.z - vertex1.z;

	//tuとtvのテクスチャ空間ベクトルを計算
	tu_vector.x = vertex2.tu - vertex1.tu;
	tv_vector.x = vertex2.tv - vertex1.tv;

	tu_vector.y = vertex3.tu - vertex1.tu;
	tv_vector.y = vertex3.tv - vertex1.tv;

	//タンジェント/バイノーマル方程式の分母を計算
	den = 1.0f / (tu_vector.x * tv_vector.y - tu_vector.y * tv_vector.x);

	//外積を計算し、係数をかけて正接と二正規分布を求める
	tangent = (tv_vector.y * vector1 - tv_vector.x * vector2) * den;

	binormal = (tu_vector.x * vector2 - tu_vector.y * vector1) * den;

	//この法線の長さを計算
	length = D3DXVec3Length(&tangent);

	//法線を正規化してから、それを格納
	if (length > 0.0001f || length < -0.0001f)
		binormal = binormal / length;
	else
		binormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}
//正接と二項正規をとりこんでから、外積を実行して法線ベクトルを返す
void ModelClass::CalculateNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal)
{
	float length;

	//法線ベクトルを加える接線と二項正規分布の外積を計算
	D3DXVec3Cross(&normal, &tangent, &binormal);

	//法線の長さを計算
	length = D3DXVec3Length(&normal);

	//法線を正規化する
	normal = normal / length;
}