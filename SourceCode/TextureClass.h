#pragma once
//自作ヘッダーインクルード
#include "TextureShaderClass.h"

//システム系のヘッダーインクルード
#include <d3d11.h>
#include <D3DX11tex.h>
#include <d3dx9.h>
#include <vector>
#include <memory>

using namespace std;

//単一のテクスチャのロード、アンロード、およびアクセスをカプセル化
class TextureClass
{

public:
	TextureClass();
	~TextureClass();

	//初期化関数(ファイル名からテクスチャを読み込む)
	bool Initialize(const WCHAR* file_name);
	//シャットダウン関数
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
private:
	ID3D11ShaderResourceView* m_texture;	//シェーダがアクセス可能なテクスチャデータ
	WCHAR* m_texture_file_name;				//テクスチャのファイル名
	int m_width, m_height;					//テクスチャのサイズ情報
};


class TextureData
{
	//分割用データ構造体
	struct SplitData
	{
		int all_num;		//画像の分割総数
		int x_num;			//分割する際のX方向にいくつ分割するかの数
		int y_num;			//分割する際のY方向にいくつ分割するかの数
		int x_size;			//分割された1つの大きさ(x)
		int y_size;			//分割された1つの大きさ(y)
	};
public:
	TextureData(int id);
	~TextureData();
	bool Initialize(const WCHAR* file_name);
	/*　分割データを設定する
	引数1 all_num	:画像の分割総数
	引数2 x_num		:分割する際のX方向にいくつ分割するかの数
	引数3 y_num		:分割する際のY方向にいくつ分割するかの数
	引数4 x_size	:分割された1つの大きさ(X)
	引数5 y_size	:分割された1つの大きさ(Y)
	*/
	void SetSplit(int all_num, int x_num, int y_num, int x_size, int y_size);
	/*分割データ取得用
	*nullputを設定してもよい
	戻り値 bool	:true = 分割データがある :false = 分割データが無い
	*/
	bool GetSplit(int* out_all_num, int* out_x_num, int* out_y_num, int* out_x_size, int* out_y_size);
	TextureClass* GetTextureClass() { return m_texture; }
	int GetID() { return m_id; }
private:
	TextureClass* m_texture;	//テクスチャデータ
	int m_id;					//登録ID
	SplitData* m_split_data;	//画像を分割するときに使用する変数
};

//2D画像描画用
class DrawTexture2DClass
{
	//頂点情報(シェーダーのものと同じになければならない)
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	DrawTexture2DClass();
	bool Initialize(int window_width,int window_height);
	void Delete();
	//ファイル名からテクスチャを読み込む
	bool LoadTexture(int id, const WCHAR* file_name);
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
	bool LoadDevTexture(int id, const WCHAR* file_name, int all_num, int x_num, int y_num, int x_size, int y_size);
	void Draw(int texture_id,float pos_x, float pos_y, float size_x, float size_y, float color[4], float r, int dev_id = 0);
	bool Render(int id, float positionX, float positionY, float sizeX, float sizeY, const D3DXCOLOR& color, float r, int dev_id=0);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture(int id);
private://関数用
	//初期化関数(バッファ専用)
	bool InitalizeBuffers();
	//削除関数(バッファ専用)
	void DeleteBuffer();
	//頂点バッファの内容を更新し、必要に応じて2Dビットマップイメージを画面上に再配置
	bool UpdateBuffers(int id, float positionX, float positionY, float sizeX, float sizeY, const D3DXCOLOR& color, float r, int dev_id);
	//描画関数(GPUの入力アセンブラで頂点、インデックスバッファをアクティブに設定する)
	void RenderBuffers();
	
	//登録したテクスチャ情報を破棄する
	void DeleteTextures();
private://変数用

	int m_screen_width;	//ウィンドウの幅
	int m_screen_height;	//ウィンドウの高さ

	vector<shared_ptr<TextureData>>* m_texture_list;	//保管用テクスチャーデータリスト

	ID3D11Buffer* m_vertex_buffer;	//頂点バッファ
	ID3D11Buffer* m_index_buffer;	//インデックスバッファ
	int m_vertex_count;	//頂点の数
	int m_index_count;	//インデックスの数

	TextureShaderClass* m_texture_shader;	//2D用テクスチャシェーダー
};

