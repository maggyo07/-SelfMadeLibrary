
//自作ヘッダインクルード
#include "RenderTextureClass.h"

#include "AccessController.h"

using Graphics::D3D;

RenderTextureClass::RenderTextureClass()
{
	m_render_rarget_texture = nullptr;
	m_render_target_view = nullptr;
	m_shader_resource_view = nullptr;
}

RenderTextureClass::~RenderTextureClass()
{

}
/* 初期化関数
引数1 texture_width	:レンダリングをテクスチャにしたい幅
引数2 texture_height:レンダリングをテクスチャにしたい高さ
※画面をレンダリングしている場合は、テクスチャに対するこのレンダリングの
　縦横比を画面の縦横比と同じにする必要がある。でないと、サイズのゆがみが発生する
 説明：
 この関数は、最初にテクスチャの説明を設定してからそのテクスチャを作成することによって、
 レンダーターゲットテクスチャを作成します。
 次に、そのテクスチャを使用してレンダーターゲットビューを設定し、テクスチャをレンダーターゲットとして描画できるようにします。
 最後に行うことは、レンダリングされたデータを呼び出し元のオブジェクトに提供できるように、
 そのテクスチャのシェーダリソースビューを作成することです。
 */
bool RenderTextureClass::Initialize(const int& texture_width, const int& texture_height)
{
	D3D11_TEXTURE2D_DESC texture_desc;	//レンダリングターゲットテクスチャの説明
	HRESULT result;						//関数の結果確認用
	D3D11_RENDER_TARGET_VIEW_DESC  render_target_view_desc;		//レンダリングターゲットビューの説明
	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;	//シェーダーリソースビューの説明

	//レンダリングターゲットテクスチャの説明を初期化
	ZeroMemory(&texture_desc, sizeof(texture_desc));

	//レンダリングターゲットテクスチャの説明を設定
	texture_desc.Width = texture_width;		//テクスチャの幅
	texture_desc.Height = texture_height;		//テクスチャの高さ
	texture_desc.MipLevels = 1;					//ミップマップレベルの最大数
	texture_desc.ArraySize = 1;					//テクスチャ配列内のテクスチャ数
	texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//テクスチャフォーマット
	texture_desc.SampleDesc.Count = 1;					//ピクセル単位のマルチサンプリングの数
	texture_desc.Usage = D3D11_USAGE_DEFAULT;	//テクスチャの読み書き込み方法を識別する値
	texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;//パイプラインステージへのバインドに関するフラグ
	texture_desc.CPUAccessFlags = 0;					//許可するCPUアクセスの種類を指定するフラグ
	texture_desc.MiscFlags = 0;					//他の一般性の低いリソースオプションを識別するフラグ

	//レンダリングターゲットテクスチャを作成
	result = D3D().GetDevice()->CreateTexture2D(&texture_desc, NULL, &m_render_rarget_texture);
	if (FAILED(result))
		return false;


	//レンダリングターゲットビューの説明を設定
	render_target_view_desc.Format = texture_desc.Format;	//データ形式
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;//リソースタイプ
	render_target_view_desc.Texture2D.MipSlice = 0;			//ミップ スライスを使用するミップマップ レベルのインデックスです。

	//レンダリングターゲットを作成
	result = D3D().GetDevice()->CreateRenderTargetView(m_render_rarget_texture, &render_target_view_desc, &m_render_target_view);
	if (FAILED(result))
		return false;


	//シェーダリソースビューの説明を設定
	shader_resource_view_desc.Format = texture_desc.Format;	//表示フォーマット
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;//ビューのリソースタイプ
	shader_resource_view_desc.Texture2D.MostDetailedMip = 0;					//使用する最も詳細なミップマップ レベルのインデックス
	shader_resource_view_desc.Texture2D.MipLevels = 1;					//テクスチャー内のミップマップ レベルの最大数

	//シェーダリソースビューを作成
	result = D3D().GetDevice()->CreateShaderResourceView(m_render_rarget_texture, &shader_resource_view_desc, &m_shader_resource_view);
	if (FAILED(result))
		return false;

	return true;
}

void RenderTextureClass::Shutdown()
{
	if (m_shader_resource_view)
	{
		m_shader_resource_view->Release();
		m_shader_resource_view = nullptr;
	}

	if (m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = nullptr;
	}

	if (m_render_rarget_texture)
	{
		m_render_rarget_texture->Release();
		m_render_rarget_texture = nullptr;
	}

}

/*
SetRenderTarget関数は、このクラスのレンダリングターゲットビューを、
レンダリング先のすべてのグラフィックの現在のレンダリング位置として設定します。
*/
void RenderTextureClass::SetRenderTarget()
{
	//レンダーターゲットビューと深度ステンシルバッファを出力レンダーパイプラインにバインド
	D3D().GetDeviceContext()->OMSetRenderTargets(1, &m_render_target_view, D3D().GetDepthStencilView());

}
/*
ClearRenderTargetは、このクラス内のレンダーターゲットビューに対して機能することを除いて、
D3DClass :: BeginScene関数の機能を模倣しています。
このレンダーターゲットビューにレンダリングする前に、
これを各フレームと呼びます。
*/
void RenderTextureClass::ClearRenderTarget(const D3DXCOLOR& color)
{
	//バックバッファをクリア
	D3D().GetDeviceContext()->ClearRenderTargetView(m_render_target_view, (float*)&color);

	//Zバッファをクリア
	D3D().GetDeviceContext()->ClearDepthStencilView(D3D().GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

}

/*
GetShaderResourceView関数は、レンダリングをシェーダリソースビューとしてテクスチャデータに返します。
この方法では、レンダーターゲットビューにレンダリングされたものはすべて、
この関数を呼び出すさまざまなシェーダでテクスチャとして使用できます。
通常、テクスチャをシェーダに送信する場所では、代わりにこの関数への呼び出しを代わりに送信することができ、
テクスチャへのレンダリングが使用されます。
*/
ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return m_shader_resource_view;
}