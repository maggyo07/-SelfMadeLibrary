
//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "TextClass.h"


TextClass::TextClass()
{
	m_font = nullptr;
	m_font_shader = nullptr;

	m_sentence1 = nullptr;
	m_sentence2 = nullptr;
}

TextClass::TextClass(const TextClass& other)
{

}

TextClass::~TextClass()
{

}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd,
int screenWidth, int screenHeight, D3DXMATRIX beseViewMatrix)
{
	//���ʏ��i�[�p
	bool result;

	//��ʂ̕��ƍ������i�[
	m_screen_width = screenWidth;
	m_screen_height = screenHeight;

	//�x�[�X�r���[�s����i�[
	m_base_view_matrix = beseViewMatrix;

	//�t�H���g�I�u�W�F�N�g�쐬
	m_font = new FontClass;
	if (!m_font)
		return false;

	//�t�H���g�I�u�W�F�N�g��������
	result = m_font->Initialize(device, "fontdata.txt",L"font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"�t�H���g�I�u�W�F�N�g���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//�t�H���g�V�F�[�_�I�u�W�F�N�g�쐬
	m_font_shader = new FontShaderClass;
	if (!m_font_shader)
		return false;

	//�t�H���g�V�F�[�_�I�u�W�F�N�g������
	result = m_font_shader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"�t�H���g�V�F�[�_�I�u�W�F�N�g���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//�ŏ��̕�����������
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result)
		return false;

	//�����̒��_�o�b�t�@��V������������ōX�V
	result = UpdataSentence(m_sentence1, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	//�ŏ��̕�����������
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result)
		return false;

	//���̒��_�o�b�t�@��V������������ōX�V
	result = UpdataSentence(m_sentence2, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
		return false;

	return true;
}

void TextClass::Shutdown()
{
	//�ŏ��̕������
	ReleaseSentence(&m_sentence1);

	//�Q�Ԗڂ̕������
	ReleaseSentence(&m_sentence2);

	//�t�H���g�V�F�[�_�I�u�W�F�N�g�����
	if (m_font_shader)
	{
		m_font_shader->Shutdown();
		delete m_font_shader;
		m_font_shader = nullptr;
	}

	//�t�H���g�I�u�W�F�N�g�����
	if (m_font)
	{
		m_font->Shutdown();
		delete m_font;
		m_font = nullptr;
	}
	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthMatrix)
{
	bool result;	//���ʏ��i�[�p

	//�ŏ��̕���`��
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthMatrix);
	if (!result)
		return false;

	//�Q�Ԗڂ̕���`��
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthMatrix);
	if (!result)
		return false;

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	bool result;//���ʏ��i�[�p

	char temp_string[16];
	char mouse_string[16];
	//mouseX�����𕶎���`���ɕϊ�
	_itoa_s(mouseX, temp_string, 10);

	//mouseX�������ݒ�
	strcpy_s(mouse_string, "MouseX:");
	strcpy_s(mouse_string, temp_string);

	//�����̒��_�o�b�t�@��V������������ōX�V
	result = UpdataSentence(m_sentence1, mouse_string, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	//mouseY�����𕶎���`���ɕϊ�
	_itoa_s(mouseY, temp_string, 10);

	//mouseY�������ݒ�
	strcpy_s(mouse_string, "MouseY:");
	strcpy_s(mouse_string, temp_string);

	//�����̒��_�o�b�t�@��V������������ōX�V
	result = UpdataSentence(m_sentence2, mouse_string, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;									//���_
	unsigned long* indices;									//�C���f�b�N�X
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//���_�A�C���f�b�N�X�o�b�t�@
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//���_�A�C���f�b�N�X�f�[�^
	HRESULT result;											//���ʏ��i�[�p

	//�V�������I�u�W�F�N�g���쐬
	*sentence = new SentenceType;
	if (!*sentence)
		return false;

	//���̃o�b�t�@��nulptr�ɏ�����
	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;

	//���̍ő咷��ݒ�
	(*sentence)->maxLength = maxLength;

	//���_�z��̒��_����ݒ�
	(*sentence)->vertexCount = 6 * maxLength;

	//�C���f�b�N�X�z����̃C���f�b�N�X�̐���ݒ�
	(*sentence)->indexCount = (*sentence)->vertexCount;

	//���_�z����쐬
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
		return false;

	//�C���f�b�N�X�z����쐬
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
		return false;

	//�ŏ��ɒ��_�z����[���ɏ���������
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	//�C���f�b�N�X�z���������
	for (int i = 0; i < (*sentence)->indexCount; i++)
		indices[i] = i;

	//���I���_�o�b�t�@�̋L�q��ݒ�
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;									//�o�b�t�@�őz�肳��Ă���ǂݍ��݋y�я������݂̕��@
	vertex_buffer_desc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;	//�o�b�t�@�̃T�C�Y
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;						//�o�b�t�@���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						//CPU�A�N�Z�X�t���O
	vertex_buffer_desc.MiscFlags = 0;												//���̑��̃t���O
	vertex_buffer_desc.StructureByteStride = 0;										//�\���̂��\�����o�b�t�@��\���ꍇ�A���̍\���̂̃T�C�Y

	//subresource�\���̂ɒ��_�f�[�^�̃|�C���^��^����
	vertex_data.pSysMem = vertices;	//�������f�[�^�ւ̃|�C���^
	vertex_data.SysMemPitch = 0;		//�e�N�X�`���ɂ���P�{�̐��̐�[����ׂ̐��܂ł̋���(�e�N�X�`���f�[�^�݂̂Ŏg�p�����)
	vertex_data.SysMemSlicePitch = 0;		//�P�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(3D�e�N�X�`���f�[�^�݂̂Ŏg����)

	//���_�o�b�t�@���쐬
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &(*sentence)->vertexBuffer);
	if (FAILED(result))
		return false;

	//���I�C���f�b�N�X�o�b�t�@�̋L�q��ݒ�
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;									//�o�b�t�@�őz�肳��Ă���ǂݍ��݋y�я������݂̕��@
	index_buffer_desc.ByteWidth = sizeof(unsigned long)* (*sentence)->indexCount;	//�o�b�t�@�̃T�C�Y
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;						//�o�b�t�@���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	index_buffer_desc.CPUAccessFlags = 0;											//CPU�A�N�Z�X�t���O
	index_buffer_desc.MiscFlags = 0;												//���̑��̃t���O
	index_buffer_desc.StructureByteStride = 0;										//�\���̂��\�����o�b�t�@��\���ꍇ�A���̍\���̂̃T�C�Y

	//subresource�\���̂ɒ��_�f�[�^�̃|�C���^��^����
	index_data.pSysMem = indices;	//�������f�[�^�ւ̃|�C���^
	index_data.SysMemPitch = 0;		//�e�N�X�`���ɂ���P�{�̐��̐�[����ׂ̐��܂ł̋���(�e�N�X�`���f�[�^�݂̂Ŏg�p�����)
	index_data.SysMemSlicePitch = 0;		//�P�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(3D�e�N�X�`���f�[�^�݂̂Ŏg����)

	//�C���f�b�N�X�o�b�t�@���쐬
	result = device->CreateBuffer(&index_buffer_desc, &index_data, &(*sentence)->indexBuffer);
	if (FAILED(result))
		return false;

	//���_�z�񂪕s�v�ɂȂ�������
	delete[] vertices;
	vertices = nullptr;

	//�C���f�b�N�X�z������
	delete[] indices;
	indices = nullptr;


	return true;
}

bool TextClass::UpdataSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
	ID3D11DeviceContext* deviceContext)
{
	int num_letters;			//text�̕�����
	VertexType* vertices;		//�㏑�����钸�_�z��
	float drawX, drawY;			//�`��ʒu �X�N���[�����W
	HRESULT result;				//���ʏ��i�[�p
	D3D11_MAPPED_SUBRESOURCE mapped_resource;//�V�F�[�_�̃f�[�^
	VertexType* vertices_ptr;	//�V�F�[�_�̒��_�z��ւ̃|�C���^

	//���̐F���i�[
	sentence->red	= red;
	sentence->green = green;
	sentence->blue	= blue;

	//���͒��̕��������擾
	num_letters = (int)strlen(text);

	//�\���̂���o�b�t�@�I�[�o�[�t���[���`�F�b�N
	if (num_letters > sentence->maxLength)
		return false;

	//���_�z����쐬
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
		return false;

	//�ŏ��ɒ��_�z����[���ɏ���������
	memset(vertices, 0, (sizeof(VertexType)* sentence->vertexCount));

	//�`����J�n�����ʏ��X�s�N�Z���ʒu��Y�s�N�Z���ʒu���v�Z����
	drawX = (float)(((m_screen_width / 2) * -1) + positionX);
	drawY = (float)((m_screen_height / 2) - positionY);

	//�t�H���g�N���X���g�p���āA���e�L�X�g�ƕ��`��ʒu���璸�_�z����\��
	m_font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	//�������ނ��Ƃ��ł����悤�ɒ��_�o�b�t�@�����b�N
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//���_�o�b�t�@���̃f�[�^�ւ̃|�C���^���擾
	vertices_ptr = (VertexType*)mapped_resource.pData;

	//�f�[�^�𒸓_�o�b�t�@�ɃR�s�[
	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType)* sentence->vertexCount));

	//���_�o�b�t�@�̃��b�N������
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	//���_�z�񂪕s�v�ɂȂ�������
	delete[] vertices;
	vertices = nullptr;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		//���̒��_�o�b�t�@�����
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = nullptr;
		}

		//���̃C���f�b�N�X�o�b�t�@�����
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = nullptr;
		}

		//�����������
		delete *sentence;
		*sentence = nullptr;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
	D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixel_color;	//�s�N�Z���J���[
	bool result;				//���ʏ��i�[�p

	//���_�o�b�t�@�̃X�g���C�h�ƃI�t�Z�b�g��ݒ�
	stride = sizeof(VertexType);
	offset = 0;

	//�����_�����O�ł���悤�ɁA���̓A�Z���u���Œ��_�o�b�t�@���A�N�e�B�u�ɐݒ�
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	//���̓A�Z���u���ŃC���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肵�āA�����_�����O�ł���悤�ɂ���
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���̒��_�o�b�t�@���烌���_�����O����v���~�e�B�u�̕���ݒ�B���̏ꍇ�͎O�p�`
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���͕��̐F�Ńs�N�Z���J���[�x�N�g�����쐬����
	pixel_color = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	//�t�H���g�V�F�[�_���g�p���ăe�L�X�g�������_�����O����
	result = m_font_shader->Render(deviceContext, sentence->indexCount, worldMatrix, m_base_view_matrix, orthoMatrix, m_font->GetTexture(),
				pixel_color);
	if (!result)
		return false;

	return true;
}