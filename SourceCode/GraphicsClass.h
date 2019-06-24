#pragma once

//システム系のヘッダーインクルード
#include <Windows.h>
//Gameシステム用ヘッダー(自作)インクルード
#include "D3DClass.h"
#include "TextureClass.h"
#include "ModelListClass.h"

#include "FrustumClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "LightClass.h"
#include "TextureShaderClass.h"
#include "TextureClass.h"
#include "TextClass.h"
#include "MultiTextureShaderClass.h"
#include "BumpMapShaderClass.h"
#include "MapShaderClass.h"
#include "RenderTextureClass.h"

//フルスクリーン状態か否か
constexpr bool FULL_SCREEN = false;
//true = ゲームのフレームレートがモニターのリフレッシュレートと同期　false = できるだけ早く
constexpr bool VSYNC_ENABLED = true;
//遠いビュー平面のZ値
constexpr float SCREEN_DEPTH = 1000.0f;
//近いビュー平面のZ値
constexpr float SCREEN_NEAR = 0.1f;

//グラフィックスクラス
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	//初期化関数
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	//シャットダウン関数
	void Shutdown();
	//フレーム感覚で呼ばれる関数
	bool Frame(float rotationY);
	//レンダリング関数
	bool Render();
private:
	/*
	RenderToTextureは、新しいプライベートレンダリング機能の1つです。
	すべてのグラフィックもレンダリングする場所として、レンダリングをテクスチャビューに設定します。
	シーンがレンダリングされた後は、D3DClassオブジェクトを使用してレンダリング位置を通常のバックバッファに戻します。
	*/
	bool RenderToTexture();
	/*
	RenderScene関数は、新しいプライベートレンダリング関数の1つです。
	シーン全体を1つの関数でレンダリングするので、シーンのレンダリング先を設定してからこの関数を呼び出すことができます。
	このチュートリアルでは、この関数をRenderToTexture関数で1回呼び出してシーンをテクスチャにレンダリングし、
	次にRender関数で再度呼び出して通常どおりバックバッファにレンダリングします。
	*/
	bool RenderScene();
private:
	CameraClass* m_camera;
	ModelClass* m_model;
	ShaderClass* m_shader;
	TextureShaderClass* m_texture_shader;	//2D用
	LightClass* m_light;
	TextClass* m_text;
	RenderTextureClass* m_render_texture;
	MultiTextureShaderClass* m_multi_texture_shader;//マルチテクスチャ用シェーダー
	BumpMapShaderClass* m_bump_map_shader;	//バンプマップシェーダー
	MapShaderClass* m_map_shader;			//マッピングシェーダ
	FrustumClass* m_frustum;				//レンダリングチェック

	D3DClass* m_D3D;
	DrawTexture2DClass* m_drawTexture2D;
	ModelListClass* m_modelList;
};