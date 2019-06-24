
//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "FontClass.h"

FontClass::FontClass()
{
	m_font = nullptr;
	m_texture = nullptr;
}

FontClass::FontClass(const FontClass& other)
{

}

FontClass::~FontClass()
{

}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	//�t�H���g�f�[�^���܂ރe�L�X�g�t�@�C����ǂݍ���
	if (!LoadFontData(fontFilename))
		return false;

	//�t�H���g���������e�N�X�`����ǂݍ���
	if (!LoadTexture(device, textureFilename))
		return false;

	return true;
}

void FontClass::Shutdown()
{
	//�t�H���g�e�N�X�`�������
	ReleaseTexture();

	///�t�H���g�f�[�^�����
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;//�t�@�C���ǂݍ��ݗp
	char temp; //�����ǂݍ��ݗp

	//�t�H���g�Ԋu�o�b�t�@���쐬
	m_font = new FontType[95]; //�z��̃T�C�Y�́A�e�N�X�`���̕�����
	if (!m_font)
		return false;

	//�t�H���g�T�C�Y�ƕ�����
	fin.open(filename);
	if (fin.fail())
		return false;

	//�e�L�X�g�Ƃ��Ďg�p����Ă���95������ASCII������ǂݍ���
	for (int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	//�t�@�C�������
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	//�t�H���g�f�[�^�z������
	if (m_font)
	{
		delete[] m_font;
		m_font = nullptr;
	}
	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	//�e�N�X�`���I�u�W�F�N�g���쐬
	m_texture = new TextureClass;
	if (!m_texture)
		return false;

	//�e�N�X�`���I�u�W�F�N�g��������
	if (!m_texture->Initialize(filename))
		return false;

	return true;
}

void FontClass::ReleaseTexture()
{
	//�e�N�X�`���I�u�W�F�N�g�����
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_texture->GetTexture();
}

//�������������_�����O����O�p�`�̒��_�z���Ԃ�
//����1	void* vertices	:�Ԃ����_�z��ւ̃|�C���^
//����2 char* sentence	:�`�悷�镶����
//����3 float drawX		:����`���ꏊ�̃X�N���[��X���W
//����4 float drawY		:����`���ꏊ�̃X�N���[��Y���W
void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertex_ptr;	//������vertices�B�^��VertexType�ɋ����ϊ�
	int num_letters	//���̕�����
		, index		//vertex_ptr�̗v�f��(�J�E���g�p)
		, letter;	//����1�����擾�p

	//���͒��_��VertexType�\���̂ɋ����ϊ�
	vertex_ptr = (VertexType*)vertices;

	//�\�����镶�������擾
	num_letters = (int)strlen(sentence);

	//�C���f�b�N�X�𒸓_�z��ɏ�����
	index = 0;

	//�e�������N���b�h�ɕ`��
	for (int i = 0; i < num_letters; i++)
	{
		//����1�������擾
		letter = ((int)sentence[i]) - 32;

		//�������X�y�[�X�̏ꍇ�́A3�s�N�Z���ȏ�ړ�
		if (letter == 0)
			drawX = drawX + 3.0f;
		else
		{
			//�N���b�h�̍ŏ��̎O�p�`
			vertex_ptr[index].possition = D3DXVECTOR3(drawX, drawY, 0.0f);	//����
			vertex_ptr[index].texture	= D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3((drawX + m_font[letter].size), (drawY-16), 0.0f);	//�E��
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3(drawX, (drawY-16), 0.0f);	//����
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].left, 1.0f);
			index++;

			//�N���b�h��2�Ԗڂ̎O�p�`
			vertex_ptr[index].possition = D3DXVECTOR3(drawX, drawY, 0.0f);	//����
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3((drawX + m_font[letter].size), drawY, 0.0f);	//�E��
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].right, 0.0f);
			index++;

			vertex_ptr[index].possition = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);	//�E��
			vertex_ptr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			//�����̃T�C�Y��1�s�N�Z���ŕ`�悷�邽�߂�X�ʒu���X�V
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}
	return;
}