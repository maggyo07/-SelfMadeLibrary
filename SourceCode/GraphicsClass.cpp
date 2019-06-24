////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#include "FrustumClass.h"

#include "AccessController.h"


using namespace Graphics;

GraphicsClass::GraphicsClass()	:m_camera(nullptr),m_model(nullptr),m_light(nullptr)
								,m_render_texture(nullptr),m_texture_shader(nullptr),m_map_shader(nullptr),m_frustum(nullptr)
								,m_D3D(&D3D()),m_drawTexture2D(&DrawTexture2D()),m_modelList(&ModelList())
{

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{

}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	D3DXMATRIX base_view_matrix;	//ベースビュー行列(テキスト描画用)
	
	//Direct3Dオブジェクトを初期化する
	bool result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Direct3Dを初期化できませんでした", L"Error", MB_OK);
		return false;
	}

	//カメラオブジェクトを作成
	m_camera = new CameraClass;
	if (!m_camera)
		return false;

	//レンダーチェックオブジェクトを作成
	m_frustum = new FrustumClass;
	if (!m_frustum)
		return false;

	//カメラの初期位置を設定
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_camera->Render();
	m_camera->GetViewMatrix(base_view_matrix);

	//テキストオブジェクトを作成
	m_text = new TextClass;
	if (!m_text)
		return false;

	//テキストオブジェクトを初期化
	result = m_text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, base_view_matrix);
	if (!result)
	{
		MessageBox(hwnd, L"テキストオブジェクトを初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//モデルオブジェクトを作成
	m_model = new ModelClass;
	if (!m_model)
		return false;

	//モデルオブジェクトを初期化する
	if (!m_model->Initialize(L"untitled.obj"))
		return false;
	//テクスチャを読み込む
	if (!m_model->LoadTexture(L"Image/specular&bump1.gif"))
		return false;
	if (!m_model->LoadTexture(L"Image/specular&bump2.gif"))
		return false;
	if (!m_model->LoadTexture(L"Image/specular&bump3.gif"))
		return false;

	//シェーダオブジェクト作成
	m_shader = new ShaderClass;
	if (!m_shader)
		return false;

	//シェーダオブジェクト初期化
	if (!m_shader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"シェーダオブジェクトを初期化できませんでした", L"Error", MB_OK);
		return false;
	}

	//テクスチャシェーダオブジェクト作成(2D用)
	m_texture_shader = new TextureShaderClass;
	if (!m_texture_shader)
		return false;

	//テクスチャシェーダオブジェクト初期化(2D用)
	result = m_texture_shader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"テクスチャシェーダオブジェクトを初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//マルチテクスチャシェーダーオブジェクトを作成
	m_multi_texture_shader = new MultiTextureShaderClass;
	if (!m_multi_texture_shader)
		return false;

	//マルチテクスチャシェーダーオブジェクト初期化
	result = m_multi_texture_shader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"マルチテクスチャシェーダーを初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//バンプマップシェーダーを作成
	m_bump_map_shader = new BumpMapShaderClass;
	if (!m_bump_map_shader)
		return false;

	//バンプマップシェーダーを初期化
	result = m_bump_map_shader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"バンプマップシェーダーを初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//マッピングシェーダーを作成
	m_map_shader = new MapShaderClass;
	if (!m_map_shader)
		return false;

	//マッピングシェーダ初期化
	result = m_map_shader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"マッピングシェーダを初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//2Dテクスチャ表示用クラスを初期化
	result = m_drawTexture2D->Initialize(screenWidth, screenHeight);
	if (!result)
		return false;
	//テスト用テクスチャ読み込み
	m_drawTexture2D->LoadTexture(0, L"dog.jpg");
	m_drawTexture2D->LoadDevTexture(1, L"Player.png", 16, 4, 4, 40, 40);

	//ライトオブジェクトを作成
	m_light = new LightClass;
	if (!m_light)
		return false;

	//ライトオブジェクトを初期化する
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetSoecularPower(16.0f);
	m_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャオブジェクトへのレンダリングを作成
	m_render_texture = new RenderTextureClass;
	if (!m_render_texture)
		return false;

	//テクスチャオブジェクトを初期化
	result = m_render_texture->Initialize(screenWidth, screenHeight);
	if (!result)
		return false;

	//モデルリストの初期化
	result = m_modelList->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"モデルリストを初期化できませんでした。", L"エラー", MB_OK);
		return false;
	}
	//テスト用モデルの作成
	m_modelList->TestRandomBllCreation(25);

	return true;
}


void GraphicsClass::Shutdown()
{
	//モデルリストクラスを解放
	m_modelList->Delete();

	//テクスチャオブジェクトにレンダリングを解放
	if (m_render_texture)
	{
		m_render_texture->Shutdown();
		delete m_render_texture;
		m_render_texture = nullptr;
	}

	//テキストオブジェクトを解放
	if (m_text)
	{
		m_text->Shutdown();
		delete m_text;
		m_text = nullptr;
	}
	//ライトオブジェクトを解放
	if (m_light)
	{
		delete m_light;
		m_light = nullptr;
	}
	//シェーダオブジェクトを解放
	if (m_shader)
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = nullptr;
	}
	//テクスチャシェーダオブジェクトを解放
	if (m_texture_shader)
	{
		m_texture_shader->Shutdown();
		delete m_texture_shader;
		m_texture_shader = nullptr;
	}
	//マルチテクスチャシェーダーオブジェクトを解放
	if (m_multi_texture_shader)
	{
		m_multi_texture_shader->Shutdown();
		delete m_multi_texture_shader;
		m_multi_texture_shader = nullptr;
	}
	//バンプマップシェーダーを解放
	if (m_bump_map_shader)
	{
		m_bump_map_shader->Shutdown();
		delete m_bump_map_shader;
		m_bump_map_shader = nullptr;
	}
	//マッピングシェーダを解放
	if (m_map_shader)
	{
		m_map_shader->Shutdown();
		delete m_map_shader;
		m_map_shader = nullptr;
	}
	//モデルオブジェクトを解放
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = nullptr;
	}

	//カメラオブジェクトを解放
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
	
	//レンダーチェックオブジェクトを開放
	if (m_frustum)
	{
		delete m_frustum;
		m_frustum = nullptr;
	}

	//2D描画用クラスを解放
	m_drawTexture2D->Delete();
	//Direct3Dオブジェクトを解放
	m_D3D->Shutdown();

	return;
}


bool GraphicsClass::Frame(float rotationY)
{
	//カメラの位置を設定
	m_camera->SetPosition(0.0f, 0.0f, -5.0f);

	//カメラの回転を設定
	m_camera->SetRotation(0.0f, rotationY, 0.0f);
	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX view_matrix, projection_matrix, world_matrix, ortho_matrix;
	bool result;		//結果情報格納用変数

						//シーン全体を最初にテクスチャにレンダリング
	result = RenderToTexture();
	if (!result)
		return false;

	//バッファをクリアしてシーンを開始する
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//バックバッファに対して通常通りシーンをレンダリング
	result = RenderScene();
	if (!result)
		return false;

	//カメラの位置に基づいてビュー行列を作成する
	m_camera->Render();

	//カメラとD3Dオブジェクトからワールド、ビュー、投影行列を取得
	m_camera->GetViewMatrix(view_matrix);
	m_D3D->GetWorldMatrix(world_matrix);
	m_D3D->GetProjectionMatrix(projection_matrix);
	m_D3D->GetOrthoMatrix(ortho_matrix);

	//円錐台を組み立てる
	m_frustum->ConstructFrustum(SCREEN_DEPTH, projection_matrix, view_matrix);
	//レンダリング数カウント用
	int render_count = 0;

	//すべてのモデルを調べて、カメラビューで表示できる場合にのみそれらをレンダリングします。
	for (int index = 0; index < m_modelList->GetModelCount(); index++)
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
		//このインデックスにおける球体モデルの位置と色を取得
		m_modelList->GetData(index, position, color);

		//球の半径を1.0に設定。
		float radius = 1.0f;

		//球体モデルが視錐台にあるかどうかを確認
		if (m_frustum->ChechCube(position.x, position.y, position.z, radius))
		{
			//モデルをレンダリングする位置に移動
			D3DXMatrixTranslation(&world_matrix, position.x, position.y, position.z);

			//モデルの頂点バッファとインデックスバッファをグラフィックパイプラインに配置して、描画の準備をする
			m_model->Render(m_D3D->GetDeviceContext());

			//シェーダを使用してモデルをレンダリング
			/*result = m_shader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
			m_model->GetTextureArray()[0], m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor(),
			m_camera->GetPosition(), m_light->GetSpecularColor(), m_light->GetSpecularPower());*/

			//元のワールドマトリックスにリセット
			m_D3D->GetWorldMatrix(world_matrix);

			//このモデルはレンダリングされたので、このフレームの数をフヤス
			render_count++;

		}
	}

	//-------------------------2Dオブジェクトをレンダリング-------------------------
	//すべての2Dレンダリングを開始するためにZバッファをOFFにする
	m_D3D->TurnZBufferOff();
	//テキストをレンダリングする前にアルファブレンディングをONにする
	m_D3D->TurnOnAlphaBlending();

	//テキスト文字列をレンダリング
	result = m_text->Render(m_D3D->GetDeviceContext(), world_matrix, ortho_matrix);
	if (!result)
		return false;

	//移動テスト用
	static int x = 100, y = 100;
	x += 1;
	result = m_drawTexture2D->Render(0, x, y, 255, 255, D3DXCOLOR(), 0);
	if (!result)
		return false;

	//ビットマップをテクスチャシェーダでレンダリングする
	result = m_texture_shader->Render(m_D3D->GetDeviceContext(), m_drawTexture2D->GetIndexCount(), world_matrix, view_matrix, ortho_matrix, m_drawTexture2D->GetTexture(0));
	if (!result)
		return false;

	//テスト
	/*m_drawTexture2D->Render(1, 1, 1,50,50,D3DXCOLOR(),0,1);
	m_texture_shader->Render(m_D3D->GetDeviceContext(), m_drawTexture2D->GetIndexCount(), world_matrix, view_matrix, ortho_matrix, m_drawTexture2D->GetTexture(1));*/


	//テクスチャレンダリング
	result = m_drawTexture2D->Render(0, 0, 0, 100, 100, D3DXCOLOR(), 0.0f);
	if (!result)
		return false;
	result = m_texture_shader->Render(m_D3D->GetDeviceContext(), m_drawTexture2D->GetIndexCount(), world_matrix, view_matrix, ortho_matrix, m_render_texture->GetShaderResourceView());
	if (!result)
		return false;

	//テキストのレンダリング後にアルファブレンディングをオフにする
	m_D3D->TurnOffAlphaBlending();
	//すべての2Dレンダリングが完了したので、Zバッファをもとに戻す
	m_D3D->TurnZBufferOn();
	//------------------------------------------------------------------------------
	//-------------------------3Dオブジェクトをレンダリング-------------------------
	//テスト用で世界を回す----------
	//各フレームの更新された回転値
	static float rotation = 0.0f;

	//各フレームのrotation変数を更新
	rotation += (float)D3DX_PI * 0.001f;
	if (rotation > 360.0f)
		rotation -= 360.0f;
	D3DXMatrixRotationY(&world_matrix, rotation);
	//------------------------------
	//モデルの頂点バッファとインデックスバッファをグラフィックスパイプラインに置き、描画の準備をする
	m_model->Render(m_D3D->GetDeviceContext());

	//シェーダを使用してモデルをレンダリング
	/*
	result = m_shader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
	m_model->GetTextureArray()[0], m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor(),
	m_camera->GetPosition(), m_light->GetSpecularColor(),m_light->GetSpecularPower());
	*/
	/*result = m_multi_texture_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
	m_model->GetTextureArray(), 2.0f);
	*/
	/*
	result = m_bump_map_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
	m_model->GetTextureArray(), m_light->GetDirection(), m_light->GetDiffuseColor());
	*/
	/*result = m_map_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
		m_model->GetTextureArray(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_camera->GetPosition(),
		m_light->GetSpecularColor(), m_light->GetSpecularPower());*/
	if (!result)
		return false;

	//------------------------------------------------------------------------------
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


bool GraphicsClass::RenderToTexture()
{
	//レンダリングターゲットをテクスチャへのレンダリングに設定
	m_render_texture->SetRenderTarget();

	//テクスチャへのレンダリングをクリア
	m_render_texture->ClearRenderTarget(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	//シーンを今すぐレンダリングすると、バックバッファではなくテクスチャにレンダリングされる
	if (!RenderScene())
		return false;

	//レンダーターゲットを元のバックバッファにリセットする
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


bool GraphicsClass::RenderScene()
{
	D3DXMATRIX view_matrix, projection_matrix, world_matrix;
	bool result = true;	//結果

						//カメラの位置に基づいてビュー行列を作成
	m_camera->Render();

	//カメラオブジェクトとD3Dオブジェクトからワールド、ビュー、投影行列を取得
	m_D3D->GetWorldMatrix(world_matrix);
	m_camera->GetViewMatrix(view_matrix);
	m_D3D->GetProjectionMatrix(projection_matrix);

	//各フレームで回転変数を変更
	static float rotati;
	rotati += (float)D3DX_PI * 0.005f;
	if (rotati > 360.0f)
		rotati -= 360.0f;

	D3DXMatrixRotationY(&world_matrix, rotati);

	//モデルの頂点バッファとインデックスバッファをグラフィックスパイプラインに設置、描画の準備
	m_model->Render(m_D3D->GetDeviceContext());

	//ライトシェーダを使用してモデルをレンダリング
	result = m_map_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix,
		projection_matrix, m_model->GetTextureArray(), m_light->GetDirection(),
		m_light->GetDiffuseColor(), m_camera->GetPosition(), m_light->GetSpecularColor(), m_light->GetSpecularPower());

	if (!result)
		return false;

	return true;
}