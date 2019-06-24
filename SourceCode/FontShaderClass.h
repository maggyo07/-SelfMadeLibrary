#pragma once
//システム系のヘッダーインクルード
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>


using namespace std;

//HLSLシェーダを呼び出すためのクラス
class FontShaderClass
{
	private:	//シェーダーのグローバル(シェーダーのものと同じになければならない)
		//行列情報
		struct MatrixBufferType
		{
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
		};

		//レンダリングされるテキストのピクセル色
		struct PixelBufferType
		{
			D3DXVECTOR4 pixelColor;
		};
public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass&);
	~FontShaderClass();
	
	//初期化関数
	bool Initialize(ID3D11Device*, HWND);
	//シャットダウン関数
	void Shutdown();
	//描画関数
	bool Render(ID3D11DeviceContext*, int indexCount, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,ID3D11ShaderResourceView* texture,
		D3DXVECTOR4 pixelColor);

private:
	//初期化関数(シェーダファイルをロードし、DirectXとGPUで使用できるようにする)
	//引数1 ID3D11Device* device	:デバイスのポインタ
	//引数2 HWND hwnd				:ウィンドウハンドル
	//引数3 WCHAR* vsFilename		:頂点シェーダコードが格納されているファイル名(パス)
	//引数4 WCHAR* psFilename		:ピクセルシェーダコードが格納されているファイル名(パス)
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	//シャットダウン関数(シェーダ関連のメモリ解放)
	void ShutdownShader();
	//シェーダをコンパイルするときに生成されるエラーメッセージを書き込む関数
	//引数1 ID3D10Blob* errorMessage	:エラーメッセージ
	//引数2 HWND		hwnd			:ウィンドウハンドル
	//引数3 WVHAR*		shaderFilename	:シェーダファイルの名前
	void OutputShaderErrorMessage(ID3D10Blob* errorMassage, HWND hwnd, WCHAR* shaderFilename);

	//シェーダー内のグローバル変数の設定を容易にするための関数
	bool SetShaderVariables(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projextionMatrix,ID3D11ShaderResourceView* texture,
		D3DXVECTOR4 pixelColor);

	//入力アセンブラで入力レイアウトをアクティブに設定し、
	//頂点バッファのレンダリングに使用する頂点、ピクセルシェーダを設定
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertex_shader;	//頂点シェーダ
	ID3D11PixelShader* m_pixel_shader;		//ピクセルシェーダ
	ID3D11InputLayout* m_layout;			//頂点入力レイアウト
	ID3D11SamplerState* m_sample_state;		//テクスチャシェーダとインターフェイスに使用する
	ID3D11Buffer* m_matrix_buffer;			//行列の定数バッファ
	ID3D11Buffer* m_pixel_buffer;			//ピクセルカラー定数バッファ
};