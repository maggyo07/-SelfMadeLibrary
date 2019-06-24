#pragma once

//システム系のヘッダーインクルード
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

//マルチテクスチャシェーダークラス
class MultiTextureShaderClass
{
private:
	/*シェーダーのグローバル
	※シェーダーのものと同じになければならない
	※メモリサイズが16の倍数でなければならない
	*/
	//行列情報
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	//ガンマ値
	struct GammaBufferTupe
	{
		float gamma;
		float padding1, padding2, padding3; 
	};
public:
	MultiTextureShaderClass();
	~MultiTextureShaderClass();
	
	//初期化関数
	bool Initialize(const HWND&);
	//破棄関数
	void Shutdown();
	//描画関数
	bool Render(const int& indexCount, const D3DXMATRIX& worldMatrix,
		const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projectionMatrix, ID3D11ShaderResourceView** texture,float gamma = 2.0f);
private://関数用
	//初期化関数(シェーダファイルをロードし、DirectXとGPUで使用できるようにする)
	//引数1 HWND hwnd				:ウィンドウハンドル
	//引数2 WCHAR* vsFilename		:頂点シェーダコードが格納されているファイル名(パス)
	//引数3 WCHAR* psFilename		:ピクセルシェーダコードが格納されているファイル名(パス)
	bool InitializeShader(HWND, WCHAR*, WCHAR*);
	//シャットダウン関数(シェーダ関連のメモリ解放)
	void ShutdownShader();
	//シェーダをコンパイルするときに生成されるエラーメッセージを書き込む関数
	//引数1 ID3D10Blob* errorMessage	:エラーメッセージ
	//引数2 HWND		hwnd			:ウィンドウハンドル
	//引数3 WVHAR*		shaderFilename	:シェーダファイルの名前
	void OutputShaderErrorMessage(ID3D10Blob* errorMassage, HWND hwnd, WCHAR* shaderFilename);

	//シェーダー内のグローバル変数の設定を容易にするための関数
	bool SetShaderParameters(D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix, D3DXMATRIX projextionMatrix, 
						ID3D11ShaderResourceView** texture, float gamma = 2.0f);

	//入力アセンブラで入力レイアウトをアクティブに設定し、
	//頂点バッファのレンダリングに使用する頂点、ピクセルシェーダを設定
	void RenderShader(int indexCount);
private://変数用
	ID3D11VertexShader* m_vertex_shader;	//頂点シェーダ
	ID3D11PixelShader* m_pixel_shader;		//ピクセルシェーダ
	ID3D11InputLayout* m_layout;			//頂点入力レイアウト
	ID3D11SamplerState* m_sample_state;		//テクスチャシェーダとインターフェイスに使用する
	ID3D11Buffer* m_matrix_buffer;			//行列の定数バッファ
	ID3D11Buffer* m_gamma_buffer;			//ガンマ値のバッファ

};