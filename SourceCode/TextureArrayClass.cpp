#include "TextureArrayClass.h"
#include "AccessController.h"

using Graphics::D3D;

#pragma region TextureArrayClass

#pragma region struct Texture
TextureArrayClass::Texture::Texture()
{
	m_texture = nullptr;
}

TextureArrayClass::Texture::~Texture()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

bool TextureArrayClass::Texture::Initialize(const WCHAR* file_name)
{
	HRESULT result;	//����

	result = D3DX11CreateShaderResourceViewFromFile(D3D().GetDevice(), file_name, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"�e�N�X�`����������܂���B", file_name, MB_OK);
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* TextureArrayClass::Texture::GetTexture()
{
	return m_texture;
}
#pragma endregion

TextureArrayClass::TextureArrayClass()
{
	m_textures = new vector<shared_ptr<Texture>>;
}

TextureArrayClass::~TextureArrayClass()
{
	if (m_textures)
	{
		//�e�N�X�`���f�[�^�̔j��
		//for (auto itr = m_textures->begin(); itr != m_textures->end(); itr++)
		//	itr->get()->Release();

		m_textures->clear();
		delete m_textures;
	}
}

//�e�N�X�`����ǂݍ���
bool TextureArrayClass::LoadTexture(const WCHAR* file_name)
{
	HRESULT result;	//����

	//�z��ɒǉ�����e�N�X�`���f�[�^
	shared_ptr<Texture> add_texture(new Texture);
	//�e�N�X�`�������[�h����
	if (!add_texture->Initialize(file_name))
		return false;

	//�z��ɒǉ�
	m_textures->push_back(move(add_texture));

	return true;
}

//�e�N�X�`���̔z��̐擪�|�C���^��Ԃ�
//���K���g���I�������delete[]���Ă��������B
ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray(int* out_count)
{
	//�����Ȃ���ΏI��
	if (m_textures->empty())
		return nullptr;

	//�|�C���^������ΐ���Ԃ�
	if (out_count != nullptr)
		*out_count = (int)m_textures->size();

	//�ŏI�I�ɕԂ��ϐ�
	ID3D11ShaderResourceView** out_data;
	//���������Z�b�g
	out_data = new ID3D11ShaderResourceView*[m_textures->size()];
	//�n���f�[�^�̃|�C���^���i�[���Ă���
	int data_count = 0;	//�J�E���g�p
	for (auto itr = m_textures->begin(); itr != m_textures->end(); itr++)
	{
		out_data[data_count++] = itr->get()->GetTexture();
	}
	//�z��̐擪�|�C���^��Ԃ�
	return out_data;
}

#pragma endregion