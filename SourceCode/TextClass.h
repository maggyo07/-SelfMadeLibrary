#pragma once

//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "FontClass.h"
#include "FontShaderClass.h"


//�t�H���g�pTextureClass
class TextClass
{
	private:
		//�e�e�L�X�g���̃����_�����O���i�[�p
		struct SentenceType
		{
			ID3D11Buffer* vertexBuffer, *indexBuffer;
			int vertexCount, indexCount, maxLength;
			float red, green, blue;
		};
	
		//FontClass����VertexType�ƈ�v
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
		//���̃`���[�g���A���ł�2�̕����g�p
		SentenceType* m_sentence1;
		SentenceType* m_sentence2;
	
};