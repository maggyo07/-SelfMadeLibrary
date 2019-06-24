#pragma once

//リンク
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

//システム系のヘッダーインクルード
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

class D3DClass
{
public:
	D3DClass();
	/*初期化関数
	引数1 int screenWidth	:ウィンドウの幅
	引数2 int screenHeight	:ウィンドウの高さ
	引数3 bool vsync		:true = ゲームのフレームレートがモニターのリフレッシュレートと同期　false = できるだけ早く
	引数4 HWND hwnd			:ウィンドウハンドル
	引数5 bool fullscreen	:true = フルスクリーン　false = ウィンドウモード
	引数6 float screenDepth	:遠いビュー平面のZ値
	引数7 float screenNear	:近いビュー平面のZ値
	*/
	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	//シャットダウン関数
	void Shutdown();
	/*各フレームの始めに新しい3Dシーンを描画すたびに呼び出される関数
	引数1 float red	:バッファをクリアするときの色情報　R
	引数2 float green	:バッファをクリアするときの色情報　G
	引数3 float bluea	:バッファをクリアするときの色情報　B
	引数4 float alpha	:バッファをクリアするときの色情報　A*/
	void BeginScene(float, float, float, float);
	//各フレームの最後にすべての描画が完了したら3Dシーンを表示するようにスワップチェーンに指示する関数
	void EndScene();
	//Direct3Dデバイス取得(レンダリングおよびリソースの作成に使用する)
	ID3D11Device* GetDevice();
	//Direct3Dデバイスコンテキスト取得(レンダリング コマンドを生成するデバイス コンテキストを表す)
	ID3D11DeviceContext* GetDeviceContext();
	//投影行列を取得
	void GetProjectionMatrix(D3DXMATRIX&);
	//ワールド上列を取得
	void GetWorldMatrix(D3DXMATRIX&);
	//正射影投影行列取得
	void GetOrthoMatrix(D3DXMATRIX&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	//深度ステンシルビューにアクセス用
	ID3D11DepthStencilView* GetDepthStencilView();
	//バックバッファレンダーターゲットをアクティブにする用
	void SetBackBufferRenderTarget();

private:
	bool m_vsync_enabled;							//true = ゲームのフレームレートがモニターのリフレッシュレートと同期　false = できるだけ早く
	int m_video_card_memory;						//専用のビデオカードのメモリ(メガバイト単位)
	char m_video_card_description[128];				//ビデオカードの名前
	IDXGISwapChain* m_swap_chain;					//スワップチェーン(フロントバッファとバックバッファを使って、画面を更新する仕組み。)
	ID3D11Device* m_device;							//Direct3Dデバイス
	ID3D11DeviceContext* m_device_context;			//Direct3Dデバイスコンテキスト
	ID3D11RenderTargetView* m_render_target_view;	//レンダーターゲットビュー
	ID3D11Texture2D* m_depth_stencil_buffer;		//深度バッファのテクスチャ
	ID3D11DepthStencilState* m_depth_stencil_state;	//深度ステンシル状態
	ID3D11DepthStencilState* m_depth_disabled_stencil_state;//深度ステンシル状態(ZバッファOFF)
	ID3D11DepthStencilView* m_depth_stencil_view;	//深度ステンシル ビューからアクセス可能なテクスチャーのサブリソース
	ID3D11RasterizerState* m_raster_state;			//ラスタライザー ステート(どのようにポリゴンを描画するかを決定するラスタ記述)
	D3DXMATRIX m_projection_matrix;					//投影行列。(3Dシーンを以前に作成した2Dビューポート空間に変換するため)
	D3DXMATRIX m_world_matrix;						//ワールド行列
	D3DXMATRIX m_ortho_matrix;						//正射影投影行列。(2D要素を画面上に描画するため)
	ID3D11BlendState* m_alpha_enable_blending_state;//アルファブレンディングON用のブレンド状態
	ID3D11BlendState* m_alpha_disable_blending_state;//アルファブレンディングOFF用のブレンド状態
};
