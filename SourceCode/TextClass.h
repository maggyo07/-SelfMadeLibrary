#pragma once

//Gameシステム用ヘッダー(自作)インクルード
#include "FontClass.h"
#include "FontShaderClass.h"


//フォント用TextureClass
class TextClass
{
	private:
		//各テキスト文のレンダリング情報格納用
		struct SentenceType
		{
			ID3D11Buffer* vertexBuffer, *indexBuffer;
			int vertexCount, indexCount, maxLength;
			float red, green, blue;
		};
	
		//FontClass内のVertexTypeと一致
		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
		};
	public:
		TextClass();
		TextClass(const TextClass&);
		~TextClass();
	
		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd,
			int screenWidth, int screenHeight, D3DXMATRIX beseViewMatrix);
		void Shutdown();
		bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
		bool SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext);
	private:
		bool InitializeSentence(SentenceType**, int, ID3D11Device*);
		bool UpdataSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
		void ReleaseSentence(SentenceType**);
		bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);
	
	private:
		FontClass* m_font;
		FontShaderClass* m_font_shader;
		int m_screen_width, m_screen_height;
		D3DXMATRIX m_base_view_matrix;
		//このチュートリアルでが2つの文を使用
		SentenceType* m_sentence1;
		SentenceType* m_sentence2;
	
};