//Gameシステム用ヘッダー(自作)インクルード
#include "D3DClass.h"

D3DClass::D3DClass()
{
	m_swap_chain = nullptr;
	m_device = nullptr;
	m_device_context = nullptr;
	m_render_target_view = nullptr;
	m_depth_stencil_buffer = nullptr;
	m_depth_stencil_state = nullptr;
	m_depth_disabled_stencil_state = nullptr;
	m_depth_stencil_view = nullptr;
	m_raster_state = nullptr;
	m_alpha_enable_blending_state = nullptr;
	m_alpha_disable_blending_state = nullptr;
}

/*初期化関数
引数1 int screenWidth	:ウィンドウの幅
引数2 int screenHeight	:ウィンドウの高さ
引数3 bool vsync		:true = ゲームのフレームレートがモニターのリフレッシュレートと同期　false = できるだけ早く
引数4 HWND hwnd			:ウィンドウハンドル
引数5 bool fullscreen	:true = フルスクリーン　false = ウィンドウモード
引数6 float screenDepth	:遠いビュー平面のZ値
引数7 float screenNear	:近いビュー平面のZ値
*/
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	D3DClass();
	HRESULT result;											//正常に行われたかどうかの確認用
	IDXGIFactory* factory;									//フルスクリーン切り替え処理をするクラス
	IDXGIAdapter* adapter;									//ディスプレイ サブシステム
	IDXGIOutput* adapter_output;							//アダプター出力 (モニターなど) 
	unsigned int num_modes;									//表示モードの数
	unsigned int numerator = 0;									//リフレッシュ レート (Hz 単位) を表す分子
	unsigned int denominator = 0;								//リフレッシュ レート (Hz 単位) を表す分母
	unsigned int string_length;								//ビデオカードの名前の文字数
	DXGI_MODE_DESC* display_mode_list;						//表示モード
	DXGI_ADAPTER_DESC adapter_desc;							//アダプター
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;					//スワップチェーン(フロントバッファとバックバッファを使って、画面を更新する仕組み。)
	D3D_FEATURE_LEVEL feature_level;						//ターゲットとなる機能セット(DirectXのバージョン)
	ID3D11Texture2D* back_buffer_ptr;						//構造化されたメモリーであるテクセル データを管理(バックバッファへのポインタ)
	D3D11_TEXTURE2D_DESC depth_buffer_desc;					//2D テクスチャー(深度バッファ)
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;			//深度ステンシル ステート
	D3D11_DEPTH_STENCIL_DESC depth_disabied_stencil_desc;	//深度ステンシル ステート(ZバッファOFF)
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;	//深度ステンシル ビューからアクセス可能なテクスチャーのサブリソース
	D3D11_RASTERIZER_DESC raster_desc;						//ラスタライザー ステート(どのようにポリゴンを描画するかを決定するラスタ記述)
	D3D11_VIEWPORT viewport;								//ビューポート
	D3D11_BLEND_DESC blend_state_description;				//ブレンド状態を設定す津ラメの新しい記述変数

															//vsyncの設定を保存する
	m_vsync_enabled = vsync;

	//---------------ビデオカード/モニタからリフレッシュレートを取得---------------
	//DirectXグラフィックスインターフェイスファクトリを作成
	/*
	CreateDXGIFactoryは、アダプターの列挙、スワップ チェーンの作成、
	および全画面表示モードとの間の切り替えに使用される
	Alt + Enter キー シーケンスとのウィンドウの関連付けを行うオブジェクトを生成するために使用します。
	*/
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
		return false;

	//ファクトリを使用して、プライマリグラフィックスインターフェイス(ビデオカード)のアダプタを作成
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
		return false;

	//プライマリアダプタ出力(モニタ)を列挙
	result = adapter->EnumOutputs(0, &adapter_output);
	if (FAILED(result))
		return false;

	//アダプタ出力(モニタ)のDXGI_FORMAT_R8G8B8A8_UNORM表示形式に適合するモードの数を取得
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
	if (FAILED(result))
		return false;

	//このモニター/ビデオカードの組み合わせで可能なすべての表示モードを保持するリストを作成
	display_mode_list = new DXGI_MODE_DESC[num_modes];
	if (!display_mode_list)
		return false;

	//表示モードのリスト構造を記入
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
	if (FAILED(result))
		return false;

	//すべての表示モードを行って、画面の幅と高さを一致する表示モードを見つける
	//一致するものが見つかると、そのモニターのリフレッシュレートの分子と分母を格納
	for (unsigned int i = 0; i < num_modes; i++)
	{
		if (display_mode_list[i].Width == (unsigned int)screenWidth)
		{
			if (display_mode_list[i].Height == (unsigned int)screenHeight)
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denominator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}
	if (numerator == 0 && denominator == 0)
	{
		MessageBox(NULL, L"画面の幅と高さが正しく取得できませんでした", NULL, MB_OK);
	}
	//-----------------------------------------------------------------------------
	//------------------ビデオカードの名前とメモリ量を取得-------------------------
	//アダプタ(ビデオカード)の説明を取得
	result = adapter->GetDesc(&adapter_desc);
	if (FAILED(result))
		return false;

	//専用のビデオカードのメモリをメガバイト単位で保存
	m_video_card_memory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);

	//ビデオカードの名前を文字列に変換して保存
	int error = wcstombs_s(&string_length, m_video_card_description, 128, adapter_desc.Description, 128);
	if (error != 0)
		return false;
	//-----------------------------------------------------------------------------
	//リフレッシュレートとビデオカード情報の分子、分母を保存したので、表示モードリストとアダプタとファクトリを解放------
	//表示モードリストを解放
	delete[] display_mode_list;
	display_mode_list = 0;

	//アダプタ出力を解放
	adapter_output->Release();
	adapter_output = 0;

	//アダプタを解放
	adapter->Release();
	adapter = 0;

	//ファクトリを解放
	factory->Release();
	factory = 0;
	//------------------------------------------------------------------------------------------------------------------
	//-----------------------------DirectXの初期化-----------------------------
	//スワップチェーンの説明を初期化
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

	//単一のバックバッファに設定
	swap_chain_desc.BufferCount = 1;

	//バックバッファの幅と高さを設定
	swap_chain_desc.BufferDesc.Width = screenWidth;
	swap_chain_desc.BufferDesc.Height = screenHeight;

	//バックバッファ用に通常の３２ビットサーフェスを設定
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//バックバッファのリフレッシュレートを設定
	if (m_vsync_enabled)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//バックバッファの使用方法を設定
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//レンダリングするウィンドウハンドルを設定
	swap_chain_desc.OutputWindow = hwnd;

	//マルチサンプリングをオフにする(オンにするとポリゴンの端っこのギザギザをスムーズにできる)
	swap_chain_desc.SampleDesc.Count = 1;	//ピクセル単位のマルチサンプリングの数
	swap_chain_desc.SampleDesc.Quality = 0;	//イメージの品質レベル 0～-1

											//フルスクリーンモードまたはウィンドウモードに設定
	if (fullscreen)
		swap_chain_desc.Windowed = false;
	else
		swap_chain_desc.Windowed = true;

	//走査線の順序とスケーリングを指定しないように設定
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//プレゼンテーション後にバックバッファの内容を破棄設定
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//スワップ チェーンの動作の設定
	swap_chain_desc.Flags = 0;

	//機能レベル(バージョン)をDirectX 11に設定
	feature_level = D3D_FEATURE_LEVEL_11_0;

	//スワップチェーン、Direct3Dデバイス、Direct3Dデバイスコンテキストを作成
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &feature_level, 1,
		D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, NULL, &m_device_context);
	if (FAILED(result))
		return false;	//DirectX 11ビデオカードを持っていない場合、D3D11CreateDeviceAndSwapChain関数呼び出しはデバイスとデバイスコンテキストの作成に失敗します
						//-------------------------------------------------------------------------

						//--------------------------バックバッファポインタからスワップチェーンにアタッチする-----------------
						//バックバッファへのポインタを取得
	result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);
	if (FAILED(result))
		return false;


	//バックバッファポインタでレンダーターゲットビューを作成
	result = m_device->CreateRenderTargetView(back_buffer_ptr, NULL, &m_render_target_view);
	if (FAILED(result))
		return false;

	//使い終わったバックバッファポインタを解放する
	back_buffer_ptr->Release();
	back_buffer_ptr = 0;
	//---------------------------------------------------------------------------------------------------

	//------------深度バッファを作成し、ポリゴンを3D空間で適切にレンダリングできるようにする-------------
	//深度バッファの記述を初期化する
	ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

	//深度バッファの記述を設定
	depth_buffer_desc.Width = screenWidth;					//テクスチャーの幅(テクセル単位)
	depth_buffer_desc.Height = screenHeight;					//テクスチャーの高さ(テクセル単位)
	depth_buffer_desc.MipLevels = 1;							//ミップマップレベルの最大数
	depth_buffer_desc.ArraySize = 1;							//テクスチャー配列内のテクスチャーの数
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//テクスチャーフォーマット
	depth_buffer_desc.SampleDesc.Count = 1;							//ピクセル単位のマルチサンプリングの数
	depth_buffer_desc.SampleDesc.Quality = 0;							//イメージの品質レベル。０～ー１
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;			//テクスチャーの読み込みおよび書き込み方法を識別する値
	depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//パイプライン ステージへのバインドに関するフラグ
	depth_buffer_desc.CPUAccessFlags = 0;							//許可する CPU アクセスの種類を指定するフラグ
	depth_buffer_desc.MiscFlags = 0;							//他の一般性の低いリソース オプションを識別するフラグ

																//記述された説明を使用して深度バッファのテクスチャを作成する
	result = m_device->CreateTexture2D(&depth_buffer_desc, NULL, &m_depth_stencil_buffer);
	if (FAILED(result))
		return false;
	//---------------------------------------------------------------------------------------------------

	//-------------------------------深度ステンシル状態を設定・作成する----------------------------------
	//ステンシル状態の記述を初期化する
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

	//ステンシル状態の説明を設定
	depth_stencil_desc.DepthEnable = true;							//深度テストを使用可能にするかどうか
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//深度ステンシルバッファの中で、深度データによる変更が可能な部分を識別
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;		//深度データを既存の深度データとどう比較する

	depth_stencil_desc.StencilEnable = true;							//ステンシルテストを使用可能にするかどうか
	depth_stencil_desc.StencilReadMask = 0xFF;							//深度ステンシルバッファの中で、ステンシルデータを読み取る部分を識別
	depth_stencil_desc.StencilWriteMask = 0xFF;							//深度ステンシルバッファの中で、ステンシルデータを書き込む部分を識別

																		//ピクセルが前面に向いている場合のステンシル操作
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//ステンシルテストで不合格になったときに実行するステンシル処理
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;	//ステンシルテストに合格し、深度テストで不合格になったときに実行するステンシル処理
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//ステンシルテストと深度テストの両方に合格したとき、実行するステンシル処理
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//ステンシルデータを既存のステンシルデータとどう比較する

																		//ピクセルが背面に向いている場合のステンシル演算
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//ステンシルテストで不合格になったときに実行するステンシル処理
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;	//ステンシルテストに合格し、深度テストで不合格になったときに実行するステンシル処理
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//ステンシルテストと深度テストの両方に合格したとき、実行するステンシル処理
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//ステンシルデータを既存のステンシルデータとどう比較する

																		//深度ステンシル状態を作成
	result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depth_stencil_state);
	if (FAILED(result))
		return false;

	//深度のステンシル状態を設定
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
	//---------------------------------------------------------------------------------------------------

	//-----深度ステンシルビューの作成、レンダーターゲットビューとデプスステンシルバッファを出力レンダリングパイプラインにバインド-----
	//深度ステンシルビューを初期化
	ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

	//深度ステンシルビューの説明を設定
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//リソース データのフォーマット
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//リソースのタイプ
	depth_stencil_view_desc.Texture2D.MipSlice = 0;								//テクスチャーが読み取り専用かどうかを記述する値

																				//深度ステンシルビューを作成
	result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
	if (FAILED(result))
		return false;

	//レンダーターゲットビューとデプスステンシルバッファを出力レンダリングパイプラインにバインドする
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
	//--------------------------------------------------------------------------------------------------------------------------------
	//----------------------ラスタライザの状態を作成----------------------
	//どのようにポリゴンを描画するかを決定するラスタ記述を設定
	raster_desc.AntialiasedLineEnable = false;			//線のアンリエイリアシングの有無。線の描画中でMultisampleEnableがfalseの場合にのみ適用される
	raster_desc.CullMode = D3D11_CULL_BACK;	//特定の方向を向いている三角形の描画の有無
	raster_desc.DepthBias = 0;				//指定のピクセルに加算する深度値
	raster_desc.DepthBiasClamp = 0.0f;				//ピクセルの最大深度バイアス
	raster_desc.DepthClipEnable = true;				//距離に基づいてクリッピングの有無
	raster_desc.FillMode = D3D11_FILL_SOLID;	//レンダリング時に使用する描画モード
	raster_desc.FrontCounterClockwise = false;			//三角形が前向きか後ろ向きかを決定
	raster_desc.MultisampleEnable = false;			//マルチサンプリングのアンチエイリアシングの有無
	raster_desc.ScissorEnable = false;			//シザー短形カリングの有無。アクティブなシザー短形の外側のピクセルはすべてカリングされます。
	raster_desc.SlopeScaledDepthBias = 0.0f;				//指定のピクセルのスロープに対するスカラ

															//先ほど記入した説明からラスタライザの状態を作成
	result = m_device->CreateRasterizerState(&raster_desc, &m_raster_state);
	if (FAILED(result))
		return false;

	//ラスタライザの状態を設定
	m_device_context->RSSetState(m_raster_state);
	//--------------------------------------------------------------------

	//-----------------------ビューボードの作成---------------------------
	//レンダリング用のビューボードを設定
	viewport.Width = (float)screenWidth;	//ビューボードの幅
	viewport.Height = (float)screenHeight;	//ビューボードの高さ
	viewport.MinDepth = 0.0f;					//ビューボードの最小深度。
	viewport.MaxDepth = 1.0f;					//ビューボードの最大深度。
	viewport.TopLeftX = 0.0f;					//ビューボードの左側のX位置
	viewport.TopLeftY = 0.0f;					//ビューボードの上部のY位置

												//ビューボードを作成
	m_device_context->RSSetViewports(1, &viewport);
	//--------------------------------------------------------------------

	//---------------------------行列の作成など---------------------------
	//投影行列を設定
	float field_of_view = (float)D3DX_PI / 4.0f;					//Y方向への視野
	float screen_aspect = (float)screenWidth / (float)screenHeight;	//アスペクト比。ビュー空間の高さを幅で除算した値

																	//3Dレンダリング用の投影行列を作成
	D3DXMatrixPerspectiveFovLH(&m_projection_matrix, field_of_view, screen_aspect, screenNear, screenDepth);

	//ワールド行列を単位行列に初期化
	D3DXMatrixIdentity(&m_world_matrix);

	//2Dレンダリング用の正射影投影行列を作成
	D3DXMatrixOrthoLH(&m_ortho_matrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	//--------------------------------------------------------------------

	//--------------------------深度ステンシル状態(ZバッファOFF)を設定・作成する------------------------
	//パラメータを設定する前に、2番目の深度のステンシル状態をkリアする
	ZeroMemory(&depth_disabied_stencil_desc, sizeof(depth_disabied_stencil_desc));

	//2DレンダリングのためにZバッファをOFFにする2番目の深度ステンシル状態を作成。
	//唯一の違いはDepthEnableがfalseに設定されている場合、他のすべてのパラメータは他の深度ステンシル状態を同じ
	depth_disabied_stencil_desc.DepthEnable = false;							//深度テストを使用可能にするかどうか
	depth_disabied_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//深度ステンシルバッファの中で、深度データによる変更が可能な部分を識別
	depth_disabied_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;				//深度データを既存の深度データとどう比較する

	depth_disabied_stencil_desc.StencilEnable = true;							//ステンシルテストを使用可能にするかどうか
	depth_disabied_stencil_desc.StencilReadMask = 0xFF;							//深度ステンシルバッファの中で、ステンシルデータを読み取る部分を識別
	depth_disabied_stencil_desc.StencilWriteMask = 0xFF;						//深度ステンシルバッファの中で、ステンシルデータを書き込む部分を識別

																				//ピクセルが前面に向いている場合のステンシル操作
	depth_disabied_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		//ステンシルテストで不合格になったときに実行するステンシル処理
	depth_disabied_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;	//ステンシルテストに合格し、深度テストで不合格になったときに実行するステンシル処理
	depth_disabied_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		//ステンシルテストと深度テストの両方に合格したとき、実行するステンシル処理
	depth_disabied_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		//ステンシルデータを既存のステンシルデータとどう比較する

																						//ピクセルが背面に向いている場合のステンシル演算
	depth_disabied_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			//ステンシルテストで不合格になったときに実行するステンシル処理
	depth_disabied_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;	//ステンシルテストに合格し、深度テストで不合格になったときに実行するステンシル処理
	depth_disabied_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;			//ステンシルテストと深度テストの両方に合格したとき、実行するステンシル処理
	depth_disabied_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;			//ステンシルデータを既存のステンシルデータとどう比較する

																						//デバイスを使用して状態を作成
	result = m_device->CreateDepthStencilState(&depth_disabied_stencil_desc, &m_depth_disabled_stencil_state);
	if (FAILED(result))
		return false;
	//--------------------------------------------------------------------------------------------------

	//-------------ブレンド状態(アルファブレンディングON)を設定・作成-------------
	//ブレンドステートの説明をクリアする
	ZeroMemory(&blend_state_description, sizeof(D3D11_BLEND_DESC));

	//アルファ対応ブレンドステートの記述を作成
	blend_state_description.RenderTarget[0].BlendEnable = TRUE;						//ブレンディングを有無
	blend_state_description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;			//ブレンディングオプション(最初のRGBデータソースを指定、ブレンディング前の処理)
	blend_state_description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//ブレンディングオプション(２番目のRGBデータソースを指定、ブレンディング前の処理)
	blend_state_description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;		//ブレンディング処理(RGBデータソースの組み合せ方法を定義)
	blend_state_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//ブレンディングオプション(最初のアルファデータソースを指定、ブレンディング前の処理)
	blend_state_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//ブレンディングオプション(２番目のアルファデータソースを指定、ブレンディング前の処理)
	blend_state_description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		//ブレンディング処理(アルファデータソースの組み合わせ方法を定義)
	blend_state_description.RenderTarget[0].RenderTargetWriteMask = 0x0f;						//書き込みマスク

																								//記述を使用してブレンド状態を作成
	result = m_device->CreateBlendState(&blend_state_description, &m_alpha_disable_blending_state);
	if (FAILED(result))
		return false;
	//----------------------------------------------------------------------------
	//------------ブレンド状態(アルファブレンディングOFF)を設定・作成------------
	//アルファ不使用ブレンド状態を記述を作成するために記述を変更
	blend_state_description.RenderTarget[0].BlendEnable = FALSE;
	//記述を使用してブレンド状態を作成
	result = m_device->CreateBlendState(&blend_state_description, &m_alpha_enable_blending_state);
	if (FAILED(result))
		return false;
	//---------------------------------------------------------------------------
	return true;
}

//シャットダウン関数
void D3DClass::Shutdown()
{
	//フルスクリーンモードでスワップチェーンを解放できないことがあるので
	//ウィンドウモードにして解放を行う
	if (m_swap_chain)
	{
		m_swap_chain->SetFullscreenState(false, NULL);
		m_swap_chain->Release();
		m_swap_chain = 0;
	}

	if (m_alpha_enable_blending_state)
	{
		m_alpha_enable_blending_state->Release();
		m_alpha_enable_blending_state = 0;
	}

	if (m_alpha_disable_blending_state)
	{
		m_alpha_disable_blending_state->Release();
		m_alpha_disable_blending_state = 0;
	}

	if (m_raster_state)
	{
		m_raster_state->Release();
		m_raster_state = 0;
	}

	if (m_depth_stencil_view)
	{
		m_depth_stencil_view->Release();
		m_depth_stencil_view = 0;
	}

	if (m_depth_stencil_state)
	{
		m_depth_stencil_state->Release();
		m_depth_stencil_state = 0;
	}

	if (m_depth_disabled_stencil_state)
	{
		m_depth_disabled_stencil_state->Release();
		m_depth_disabled_stencil_state = 0;
	}

	if (m_depth_stencil_buffer)
	{
		m_depth_stencil_buffer->Release();
		m_depth_stencil_buffer = 0;
	}

	if (m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = 0;
	}

	if (m_device_context)
	{
		m_device_context->Release();
		m_device_context = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	return;
}
/*各フレームの始めに新しい3Dシーンを描画すたびに呼び出される関数
引数1 float red	:バッファをクリアするときの色情報　R
引数2 float green	:バッファをクリアするときの色情報　G
引数3 float bluea	:バッファをクリアするときの色情報　B
引数4 float alpha	:バッファをクリアするときの色情報　A*/
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	//バッファをクリアする色情報RGBA
	float color[4];

	//バッファをクリアする色を設定
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//バックバッファをクリア
	m_device_context->ClearRenderTargetView(m_render_target_view, color);

	//深度バッファをクリア
	m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

//各フレームの最後にすべての描画が完了したら3Dシーンを表示するようにスワップチェーンに指示する関数
void D3DClass::EndScene()
{
	//レンダリングが完了したので、バックバッファを画面に表示する
	if (m_vsync_enabled)
	{
		//画面のリフレッシュレートにロック
		m_swap_chain->Present(1, 0);
	}
	else
	{
		//できるだけ早く掲示する
		m_swap_chain->Present(0, 0);
	}
	return;
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}
ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_device_context;
}
void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectonMatrix)
{
	projectonMatrix = m_projection_matrix;
	return;
}
void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_world_matrix;
	return;
}
void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMateix)
{
	orthoMateix = m_ortho_matrix;
	return;
}
void D3DClass::TurnZBufferOn()
{
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
	return;
}
void D3DClass::TurnZBufferOff()
{
	m_device_context->OMSetDepthStencilState(m_depth_disabled_stencil_state, 1);
	return;
}

void D3DClass::TurnOnAlphaBlending()
{
	float blend_factor[4];

	//ブレンド係数を設定
	blend_factor[0] = 0.0f;
	blend_factor[1] = 0.0f;
	blend_factor[2] = 0.0f;
	blend_factor[3] = 0.0f;

	//アルファブレンディングをオンにする
	m_device_context->OMSetBlendState(m_alpha_enable_blending_state, blend_factor, 0xffffffff);

	return;
}

void D3DClass::TurnOffAlphaBlending()
{
	float blend_factor[4];

	//ブレンド係数を設定
	blend_factor[0] = 0.0f;
	blend_factor[1] = 0.0f;
	blend_factor[2] = 0.0f;
	blend_factor[3] = 0.0f;

	//アルファブレンディングオフにする
	m_device_context->OMSetBlendState(m_alpha_disable_blending_state, blend_factor, 0xffffffff);

	return;
}

//深度ステンシルビューにアクセス用
ID3D11DepthStencilView* D3DClass::GetDepthStencilView()
{
	return m_depth_stencil_view;
}
//バックバッファレンダーターゲットをアクティブにする用
void D3DClass::SetBackBufferRenderTarget()
{
	//レンダリングターゲットビューと深度ステンシルバッファを出力レンダリングパイプラインにバインド
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

}