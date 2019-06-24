//システム系のヘッダーインクルード

//Gameシステム用ヘッダー(自作)インクルード
#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass()
{
	m_vertex_shader = nullptr;
	m_pixel_shader = nullptr;
	m_layout = nullptr;
	m_matrix_buffer = nullptr;
	m_sample_state = nullptr;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{

}

TextureShaderClass::~TextureShaderClass()
{

}

//初期化関数
bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	//頂点シェーダとピクセルシェーダを初期化する
	if (!InitializeShader(device, hwnd, L"Texture.vs", L"Texture.ps"))
		return false;

	return true;
}

//シャットダウン関数
void TextureShaderClass::Shutdown()
{
	//頂点シェーダとピクセルシェーダ、および関連するオブジェクトをシャットダウン
	ShutdownShader();

	return;
}

//描画関数
bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix,
				D3DXMATRIX viewMatrix, D3DXMATRIX ortho_matrix,ID3D11ShaderResourceView* texture)
{
	//レンダリングに使用するシェーダパラメータを設定
	if (!SetShaderVariables(deviceContext, worldMatrix, viewMatrix, ortho_matrix,texture))
		return false;

	//準備したバッファをシェーダでレンダリング
	RenderShader(deviceContext, indexCount);
	
	return true;
}
//初期化関数(シェーダファイルをロードし、DirectXとGPUで使用できるようにする)
//引数1 ID3D11Device* device	:デバイスのポインタ
//引数2 HWND hwnd				:ウィンドウハンドル
//引数3 WCHAR* vsFilename		:頂点シェーダコードが格納されているファイル名(パス)
//引数4 WCHAR* psFilename		:ピクセルシェーダコードが格納されているファイル名(パス)
bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;	//動作確認用
	ID3D10Blob* error_message;					//エラーメッセージ
	ID3D10Blob* vertex_shader_buffer;			//頂点シェーダ情報格納用バッファ
	ID3D10Blob* pixel_shader_buffer;			//ピクセルシェーダ情報格納用バッファ
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];	//ポリゴンレイアウト
	unsigned int num_elements;					//ポリゴンレイアウトの要素数
	D3D11_SAMPLER_DESC sampler_desc;			//テクスチャの記述を保持する
	D3D11_BUFFER_DESC matrix_buffer_desc;		//頂点シェーダの行列定数バッファ

	//ポインタ変数の初期化
	error_message = nullptr;
	vertex_shader_buffer = nullptr;
	pixel_shader_buffer = nullptr;

	//頂点シェーダコードをコンパイル
	//関数情報->//https://msdn.microsoft.com/ja-jp/library/ee416856(v=vs.85).aspx
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertex_shader_buffer, &error_message, NULL);
	if (FAILED(result))
	{
		//シェーダがコンパイル失敗した場合は、エラーメッセージに何か書きこまれるはず
		if (error_message)
			OutputShaderErrorMessage(error_message, hwnd, vsFilename);
		//エラーメッセージに何もなかった場合、単にシェーダファイル自体を見つけることができなかった
		else
			MessageBox(hwnd, vsFilename, L"頂点シェーダファイルがありません。", MB_OK);

		return false;
	}

	//ピクセルシェーダコードをコンパイル
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixel_shader_buffer, &error_message, NULL);
	if (FAILED(result))
	{
		//シェーダがコンパイル失敗した場合は、エラーメッセージに何か書きこまれるはず
		if (error_message)
			OutputShaderErrorMessage(error_message, hwnd, psFilename);
		//エラーメッセージに何もなかった場合、単にシェーダファイル自体を見つけることができなかった
		else
			MessageBox(hwnd, psFilename, L"ピクセルシェーダファイルがありません。", MB_OK);

		return false;
	}

	//バッファから頂点シェーダを作成
	result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), NULL, &m_vertex_shader);
	if (FAILED(result))
		return false;

	//バッファからピクセルシェーダを作成
	result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), NULL,&m_pixel_shader);
	if (FAILED(result))
		return false;

	//シェーダに入るデータのレイアウトを設定
	//この設定は、ModelClassとシェーダのVertexType構造に一致する必要がある
	polygon_layout[0].SemanticName = "POSITION";						//シェーダー入力署名でこの要素に関連付けられている HLSL セマンティクス
	polygon_layout[0].SemanticIndex = 0;								//要素のセマンティクス インデックス
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				//要素データのデータ型
	polygon_layout[0].InputSlot = 0;									//入力アセンブラーを識別する整数値
	polygon_layout[0].AlignedByteOffset = 0;							//各要素間のオフセット 
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;		//単一の入力スロットの入力データ クラスを識別
	polygon_layout[0].InstanceDataStepRate = 0;							//バッファーの中で要素の 1 つ分進む前に、インスタンス単位の同じデータを使用して描画するインスタンスの数です

	polygon_layout[1].SemanticName = "TEXCOORD";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

	//レイアウト内の要素数を取得
	num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

	//頂点入力レイアウトを作成
	result = device->CreateInputLayout(polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(),
		vertex_shader_buffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
		return false;

	//頂点シェーダバッファとピクセルシェーダバッファは、もう必要ないので解放
	vertex_shader_buffer->Release();
	vertex_shader_buffer = 0;

	pixel_shader_buffer->Release();
	pixel_shader_buffer = 0;

	//テクスチャサンプラの状態記述を作成
	sampler_desc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//フィルタリング
	sampler_desc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;		//０～１の範囲外にあるUテクスチャ座標の解決方法
	sampler_desc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;		//０～１の範囲外にあるVテクスチャ座標の解決方法
	sampler_desc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;		//０～１の範囲外にあるW	テクスチャ座標の解決方法
	sampler_desc.MipLODBias		= 0.0f;								//計算されたミップマップレベルからのオフセット
	sampler_desc.MaxAnisotropy	= 1;								//特定のフィルタリングの時に使用されるクランプ値
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;			//既存のサンプリングデータに対してデータを比較する関数	
	sampler_desc.BorderColor[0] = 0;								//AddressU,V,WにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用されt境界の色
	sampler_desc.BorderColor[1] = 0;								//		
	sampler_desc.BorderColor[2] = 0;								//
	sampler_desc.BorderColor[3] = 0;								//
	sampler_desc.MinLOD			= 0;								//アクセスをクランプするミップマップ範囲の下限
	sampler_desc.MaxLOD			= D3D11_FLOAT32_MAX;				//アクセスをクランプするミップマップ範囲の上限

	//テクスチャサンプラの状態を作成
	result = device->CreateSamplerState(&sampler_desc, &m_sample_state);
	if (FAILED(result))
		return false;

	//頂点シェーダにある行列定数バッファの記述をセットアップ
	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;			//バッファーで想定されている読み込みおよび書き込みの方法
	matrix_buffer_desc.ByteWidth = sizeof(MatrixBufferType);		//バッファのサイズ
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//バッファーをどのようにパイプラインにバインドするか
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//CPU アクセスのフラグ
	matrix_buffer_desc.MiscFlags = 0;							//その他のフラグ
	matrix_buffer_desc.StructureByteStride = 0;							//構造体が構造化バッファーを表す場合、その構造体のサイズ

	//このクラス内から頂点シェーダ定数バッファにアクセスできるように、定数バッファポインタを作成
	result = device->CreateBuffer(&matrix_buffer_desc, NULL, &m_matrix_buffer);
	if (FAILED(result))
		return false;

	return true;
}
//シャットダウン関数(シェーダ関連のメモリ解放)
void TextureShaderClass::ShutdownShader()
{
	//行列定数バッファを解放
	if (m_matrix_buffer)
	{
		m_matrix_buffer->Release();
		m_matrix_buffer = nullptr;
	}

	//サンプラーの状態を解放
	if (m_sample_state)
	{
		m_sample_state->Release();
		m_sample_state = nullptr;
	}

	//レイアウトを解放
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	//ピクセルシェーダ解放
	if (m_pixel_shader)
	{
		m_pixel_shader->Release();
		m_pixel_shader = nullptr;
	}

	//頂点シェーダを解放
	if (m_vertex_shader)
	{
		m_vertex_shader->Release();
		m_vertex_shader = nullptr;
	}

	return;
}

//シェーダをコンパイルするときに生成されるエラーメッセージを書き込む関数
//引数1 ID3D10Blob* errorMessage	:エラーメッセージ
//引数2 HWND		hwnd			:ウィンドウハンドル
//引数3 WVHAR*		shaderFilename	:シェーダファイルの名前
void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMassage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compile_errors;			//メッセージ(文字列)
	unsigned long buffer_size, i;	//メッセージの長さとループ用
	ofstream fout;					//ファイルの書き込み用

	//エラーメッセージのテキストバッファへのポインタを取得
	compile_errors = (char*)(errorMassage->GetBufferPointer());

	//メッセージの長さを取得
	buffer_size = errorMassage->GetBufferSize();

	//エラーメッセージを書き込むファイルを開く
	//なければ生成される
	fout.open("shader-error.txt");

	//エラーメッセージを出力
	for (i = 0; i < buffer_size; i++)
		fout << compile_errors[i];

	//ファイルを閉じる
	fout.close();

	//エラーメッセージを解放
	errorMassage->Release();
	errorMassage = 0;

	//画面上にメッセージをポップアップして、テキストファイルのコンパイラエラーをチェックするようにユーザーに通知する
	MessageBox(hwnd, L"シェーダーのコンパイル中にエラーが発生しました。shader-error.txtでメッセージを確認してください。", shaderFilename, MB_OK);

	return;
}

//シェーダー内のグローバル変数の設定を容易にするための関数
bool TextureShaderClass::SetShaderVariables(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
								D3DXMATRIX viewMatrix, D3DXMATRIX projextionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;								//動作確認用
	D3D11_MAPPED_SUBRESOURCE mapped_resource;	//マップされたサブリソースへのポインター
	unsigned int buffer_number;					//頂点シェーダ内の定数バッファの位置
	MatrixBufferType* data_ptr;					//行列定数バッファ内のデータへのポインタ

	//行列を移調してシェーダのために準備。これはDirectX11の要件です
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projextionMatrix, &projextionMatrix);

	//書き込むことができるように定数バッファロックする
	result = deviceContext->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//行列定数バッファ内のデータへのポインタを取得
	data_ptr = (MatrixBufferType*)mapped_resource.pData;

	//行列を定数バッファにコピーする
	data_ptr->world = worldMatrix;
	data_ptr->view = viewMatrix;
	data_ptr->projection = projextionMatrix;

	//定数バッファのロックを解除
	deviceContext->Unmap(m_matrix_buffer, 0);

	//頂点シェーダ内の定数バッファの位置を設定
	buffer_number = 0;

	//更新された値で頂点シェーダの行列定数バッファをFinanlyに設定
	deviceContext->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);

	//ピクセルシェーダにシェーダテクスチャリソースを設定
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

//入力アセンブラで入力レイアウトをアクティブに設定し、
//頂点バッファのレンダリングに使用する頂点、ピクセルシェーダを設定
void TextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//頂点の入力レイアウトを設定
	deviceContext->IASetInputLayout(m_layout);

	//この三角形を描画するために使用される頂点シェーダとピクセルシェーダを設定
	deviceContext->VSSetShader(m_vertex_shader, NULL, 0);
	deviceContext->PSSetShader(m_pixel_shader, NULL, 0);

	//ピクセルシェーダでサンプラの状態を設定
	deviceContext->PSSetSamplers(0, 1, &m_sample_state);

	//レンダリングします
	deviceContext->DrawIndexed(indexCount, 0, 0);
}



