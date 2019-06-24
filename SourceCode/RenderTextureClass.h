#pragma once

//システム系のヘッダーインクルード
#include <d3d11.h>

//自作ヘッダーインクルード
#include "D3DClass.h"
/*
RenderTextureClassを使用すると、
バックバッファではなくレンダリングターゲットとして設定できます。
また、ID3D11ShaderResourceViewの形式でそれにレンダリングされたデータを取得することもできます。
*/
class RenderTextureClass
{
public:
	RenderTextureClass();
	~RenderTextureClass();

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
	bool Initialize(const int& texture_width, const int& texture_height);
	void Shutdown();

	/*
	SetRenderTarget関数は、このクラスのレンダリングターゲットビューを、
	レンダリング先のすべてのグラフィックの現在のレンダリング位置として設定します。
	*/
	void SetRenderTarget();
	/*
	ClearRenderTargetは、このクラス内のレンダーターゲットビューに対して機能することを除いて、
	D3DClass :: BeginScene関数の機能を模倣しています。
	このレンダーターゲットビューにレンダリングする前に、
	これを各フレームと呼びます。
	*/
	void ClearRenderTarget(const D3DXCOLOR& color);
	/*
	GetShaderResourceView関数は、レンダリングをシェーダリソースビューとしてテクスチャデータに返します。
	この方法では、レンダーターゲットビューにレンダリングされたものはすべて、
	この関数を呼び出すさまざまなシェーダでテクスチャとして使用できます。
	通常、テクスチャをシェーダに送信する場所では、代わりにこの関数への呼び出しを代わりに送信することができ、
	テクスチャへのレンダリングが使用されます。
	*/
	ID3D11ShaderResourceView* GetShaderResourceView();
private:
	ID3D11Texture2D* m_render_rarget_texture;			//レンダリングターゲットテクスチャ
	ID3D11RenderTargetView* m_render_target_view;		//レンダリングターゲットビュー
	ID3D11ShaderResourceView* m_shader_resource_view;	//シェーダーリソースビュー
};