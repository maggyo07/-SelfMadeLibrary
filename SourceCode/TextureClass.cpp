

//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "TextureClass.h"

#include "AccessController.h"

using Graphics::D3D;

#pragma region DrawTexture2DClass
DrawTexture2DClass::DrawTexture2DClass()	:m_vertex_buffer(nullptr),m_index_buffer(nullptr),m_texture_shader(nullptr)
											,m_texture_list(new vector<shared_ptr<TextureData>>)
{

}
//������
bool DrawTexture2DClass::Initialize(int window_width,int window_height)
{
	//�E�B���h�E�̃T�C�Y����ݒ�
	m_screen_width = window_width;
	m_screen_height = window_height;

	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@��������
	if (!InitalizeBuffers())
		return false;

	return true;
}
//�������֐�(�o�b�t�@���쐬)
bool DrawTexture2DClass::InitalizeBuffers()
{
	VertexType* vertexs;	//���_���i�[�p
	unsigned long* indices;	//�C���f�b�N�X���i�[�p
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//���_�A�C���f�b�N�X�o�b�t�@���쐬����̂Ɏg�p����ϐ�
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//���_�A�C���f�b�N�X�o�b�t�@���쐬����̂Ɏg�p����ϐ�
	HRESULT result;			//����ɍs��ꂽ���ǂ����̊m�F�p

	//-----------4�p�`�̔|���S���쐬-----------
	//���_�z��̒��_�̐���ݒ�
	m_vertex_count = 4;
	//�C���f�b�N�X�z����̃C���f�b�N�X�̐���ݒ�
	m_index_count = 6;

	//���_�z����쐬
	vertexs = new VertexType[m_vertex_count];
	if (!vertexs)
		return false;

	//�C���f�b�N�X�z����쐬
	indices = new unsigned long[m_index_count];
	if (!indices)
		return false;

	//�ŏ��ɒ��_�z����[���ɏ���������
	memset(vertexs, 0, (sizeof(VertexType) * m_vertex_count));

	//�C���f�b�N�X�z����f�[�^�Ń��[�h����
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 1; indices[4] = 2; indices[5] = 3;
	//-------------------------------------------
	//���I���_�o�b�t�@�̐�����ݒ�
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;					//�o�b�t�@�őz�肳��Ă���ǂݍ��݋y�я������݂̕��@
	vertex_buffer_desc.ByteWidth = sizeof(VertexType)* m_vertex_count;	//�o�b�t�@�̃T�C�Y
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//�o�b�t�@���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//CPU�A�N�Z�X�t���O
	vertex_buffer_desc.MiscFlags = 0;									//���̑��̃t���O
	vertex_buffer_desc.StructureByteStride = 0;									//�\���̂��\�����o�b�t�@��\���ꍇ�A���̍\���̂̃T�C�Y

																				//subresource�\���̂ɒ��_�f�[�^�̃|�C���^��^����
	vertex_data.pSysMem = vertexs;	//�������f�[�^�ւ̃|�C���^
	vertex_data.SysMemPitch = 0;		//�e�N�X�`���ɂ���P�{�̐��̐�[����ׂ̐��܂ł̋���(�e�N�X�`���f�[�^�݂̂Ŏg�p�����)
	vertex_data.SysMemSlicePitch = 0;		//�P�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(3D�e�N�X�`���f�[�^�݂̂Ŏg����)

											//���_�o�b�t�@���쐬
	result = D3D().GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
	if (FAILED(result))
		return false;

	//�ÓI�C���f�b�N�X�o�b�t�@�̐�����ݒ�
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;					//�o�b�t�@�őz�肳��Ă���ǂݍ��݋y�я������݂̕��@
	index_buffer_desc.ByteWidth = sizeof(unsigned long)* m_index_count;	//�o�b�t�@�̃T�C�Y
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;				//�o�b�t�@���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	index_buffer_desc.CPUAccessFlags = 0;									//CPU�A�N�Z�X�t���O
	index_buffer_desc.MiscFlags = 0;									//���̑��̃t���O
	index_buffer_desc.StructureByteStride = 0;									//�\���̂��\�����o�b�t�@��\���ꍇ�A���̍\���̂̃T�C�Y

																				//�T�u���\�[�X�\���̂ɃC���f�b�N�X�f�[�^�ւ̃|�C���^��Ԃ��܂��B
	index_data.pSysMem = indices;	//�������f�[�^�ւ̃|�C���^
	index_data.SysMemPitch = 0;		//�e�N�X�`���ɂ���P�{�̐��̐�[����ׂ̐��܂ł̋���(�e�N�X�`���f�[�^�݂̂Ŏg�p�����)
	index_data.SysMemSlicePitch = 0;		//�P�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(3D�e�N�X�`���f�[�^�݂̂Ŏg����)

	//�C���f�b�N�X�o�b�t�@���쐬
	result = D3D().GetDevice()->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
	if (FAILED(result))
		return false;

	//���_�ƃC���f�b�N�X�̃o�b�t�@���쐬���ꃍ�[�h���ꂽ�̂ŁA�z������
	delete[] vertexs;
	vertexs = 0;

	delete[] indices;
	indices = 0;

	return true;
}
//�폜�֐�
void DrawTexture2DClass::Delete()
{
	//�o�b�t�@��j������
	DeleteBuffer();
	//�o�^�����e�N�X�`���f�[�^��j��
	DeleteTextures();
}
//�폜�֐�(�o�b�t�@��p)
void DrawTexture2DClass::DeleteBuffer()
{
	//�C���f�b�N�X�o�b�t�@�����
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = nullptr;
	}

	//���_�o�b�t�@�����
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = nullptr;
	}
}
//�o�^�����e�N�X�`������j������
void DrawTexture2DClass::DeleteTextures()
{
	if (m_texture_list)
	{
		m_texture_list->clear();
		delete m_texture_list;
	}
}
//���_�o�b�t�@�̓��e���X�V���A�K�v�ɉ�����2D�r�b�g�}�b�v�C���[�W����ʏ�ɍĔz�u
bool DrawTexture2DClass::UpdateBuffers(int id,float positionX,float positionY,float sizeX,float sizeY, const D3DXCOLOR& color,float r,int dev_id)
{
	float left, right, top, bottom;				//�r�b�g�}�b�v�̊e��ʂ̍��W
	int dev_all_num, dev_x_num, dev_y_num, dev_x_size, dev_y_size;//�����f�[�^
	bool dev_flag;								//�����f�[�^�����邩�ǂ����̃t���O
	VertexType* vertices;						//�X�V���钸�_���
	D3D11_MAPPED_SUBRESOURCE mapped_resource;	//���_�o�b�t�@�̏��
	VertexType* vertices_ptr;					//���_�o�b�t�@���ւ̃|�C���^
	HRESULT result;								//����m�F�p
	weak_ptr<TextureData> texture_data;			//id�Ō������ăq�b�g�����e�N�X�`���f�[�^
	//float texture_sizeX, texture_sizeY;			//�e�N�X�`���̃T�C�Y

	//����
	for each (auto itr in *m_texture_list)
	{
		if (itr->GetID() == id)
		{
			texture_data = itr;
			break;
		}
	}
	//�������s
	if (texture_data.expired())
		return false;

	//�r�b�g�}�b�v�̍����̉�ʍ��W���v�Z
	left = (float)((m_screen_width / 2)*-1) + positionX;

	//�r�b�g�}�b�v�̉E���̉�ʍ��W���v�Z
	if (abs(sizeX) > 1.0f)
		right = left + sizeX;
	else
		right = left + texture_data.lock()->GetTextureClass()->GetWidth();

	//�r�b�g�}�b�v�̏㕔�̉�ʍ��W���v�Z
	top = (float)(m_screen_height / 2) - positionY;

	//�r�b�g�}�b�v�̉����̉摜���W���v�Z
	if(abs(sizeY) > 1.0f)
		bottom = top - sizeY;
	else
		bottom = top - texture_data.lock()->GetTextureClass()->GetHeight();

	//���_�z����쐬
	vertices = new VertexType[m_vertex_count];
	if (!vertices)
		return false;
	//�����f�[�^�Ǎ�
	dev_flag = texture_data.lock()->GetSplit(&dev_all_num, &dev_x_num, &dev_y_num, &dev_x_size, &dev_y_size);

	//���_�z��̈ʒu�f�[�^��ݒ�
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);	//����
	vertices[1].position = D3DXVECTOR3(right, top, 0.0f);	//�E��
	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);	//����
	vertices[3].position = D3DXVECTOR3(right, bottom, 0.0f);//�E��

	//���_�z���UV�f�[�^��ݒ�
	if (dev_flag)
	{
		double aa = (double)(dev_id % (dev_x_num)*dev_x_size) /(double)texture_data.lock()->GetTextureClass()->GetWidth();
		float a1 = dev_id % (10);
		float a = ((float)(dev_id % (dev_x_num)*dev_x_size) / (float)texture_data.lock()->GetTextureClass()->GetWidth());
		vertices[0].texture = D3DXVECTOR2((float)(dev_id % (dev_x_num)*dev_x_size / (float)texture_data.lock()->GetTextureClass()->GetWidth()),
										 (float)(dev_id / (dev_x_num)*dev_y_size / (float)texture_data.lock()->GetTextureClass()->GetHeight()));	//����
		vertices[1].texture = vertices[0].texture + D3DXVECTOR2((float)dev_x_size / (float)texture_data.lock()->GetTextureClass()->GetWidth(), 0.0f);  //�E��
		vertices[2].texture = vertices[0].texture + D3DXVECTOR2(0.0f, (float)dev_y_size / (float)texture_data.lock()->GetTextureClass()->GetHeight());	//����
		vertices[3].texture = vertices[0].texture + D3DXVECTOR2((float)dev_x_size / (float)texture_data.lock()->GetTextureClass()->GetWidth(), (float)dev_y_size / (float)texture_data.lock()->GetTextureClass()->GetHeight());	//�E��
		for (int i = 0; i < 4; i++)
		{
			vertices[i].texture.x = min(max(0.0f, vertices[i].texture.x), 1.0f);
			vertices[i].texture.y = min(max(0.0f, vertices[i].texture.y), 1.0f);
		}
	}
	else
	{
		vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);	//����
		vertices[1].texture = D3DXVECTOR2(1.0f, 0.0f);  //�E��
		vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);	//����
		vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);	//�E��
		
	}

	//�������ނ��Ƃ��ł���悤�ɒ��_�o�b�t�@�����b�N
	result = D3D().GetDeviceContext()->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//���_�o�b�t�@���̃f�[�^�ւ̃|�C���^���擾
	vertices_ptr = (VertexType*)mapped_resource.pData;

	//�f�[�^�𒸓_�o�b�t�@�ɃR�s�[����
	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType)*m_vertex_count));

	//���_�o�b�t�@�̃��b�N������
	D3D().GetDeviceContext()->Unmap(m_vertex_buffer, 0);

	//���_�z�񂪕s�v�ɂȂ�������
	delete[] vertices;
	vertices = nullptr;

	return true;
}
//�`��֐�(GPU�̓��̓A�Z���u���Œ��_�A�C���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肷��)
void DrawTexture2DClass::RenderBuffers()
{
	//�X�g���C�h
	unsigned int stride;
	//�I�t�Z�b�g(��_���狗���ŕ\�����ʒu)
	unsigned int offset;

	//���_�o�b�t�@�̃X�g���C�h�ƃI�t�Z�b�g��ݒ�
	stride = sizeof(VertexType);
	offset = 0;

	//�����_�����O�ł���悤�ɁA���̓A�Z���u���Œ��_�o�b�t�@���A�N�e�B�u�ɐݒ�
	D3D().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	//���̓A�Z���u���ŃC���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肵�āA�����_�����O�ł���悤�ɂ���
	D3D().GetDeviceContext()->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//���̒��_�o�b�t�@���烌���_�����O����v���~�e�B�u�̌^��ݒ肷��B���̏ꍇ�̓g���C�A���O�� �X�g���b�v�ł�
	D3D().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return;

}
//�`��֐�
bool DrawTexture2DClass::Render(int id,float positionX,float positionY,float sizeX,float sizeY,const D3DXCOLOR& color,float r,int dev_id)
{
	//�����_�����O�̂��߂ɓ��I���_�o�b�t�@���č\�z���āA�����炭��ʏ�̕ʂ̏ꏊ�ɂ��܂��B
	if (!UpdateBuffers(id, positionX, positionY,sizeX,sizeY,color,r,dev_id))
		return false;

	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���O���t�B�b�N�p�C�v���C���ɔz�u���āA�`��̏���
	RenderBuffers();

	return true;
}

//�e�N�X�`������o�^
bool DrawTexture2DClass::LoadTexture(int id,const WCHAR* file_name)
{
	//���łɓo�^���Ă���ID�ƈꏏ�Ȃ�I��
	for each (auto texture in *m_texture_list)
	{
		if (texture->GetID() == id)
			return false;
	}

	//���X�g�ɒǉ�����e�N�X�`���f�[�^
	shared_ptr<TextureData> add_texture_data(new TextureData(id));

	//�e�N�X�`���f�[�^������
	if (!add_texture_data->Initialize(file_name))
		return false;

	//���X�g�ɒǉ�
	m_texture_list->push_back(move(add_texture_data));

	return true;
}
/*�@�e�N�X�`������o�^
	����3 int	all_num	:�摜�̕�������
	����4 int	x_num	:�摜�̉������ɑ΂��镪����
	����5 int	y_num	:�摜�̏c�����ɑ΂��镪����
	����6 int	x_size	:�������ꂽ1�̑傫��(X)
	����7 int	y_size	:�������ꂽ1�̑傫��(Y)
	�������������ꍇ�A�\�����邽�߂ɕK�v�ȕ���ID�͈ȉ��̂悤�Ɋ���U����
	XNum = 4 ; YNum = 4 ; AllNum = 14 ;
	0	1	2	3
	4	5	6	7
	8	9	10	11
	12	13
*/
bool DrawTexture2DClass::LoadDevTexture(int id, const WCHAR* file_name, int all_num, int x_num, int y_num, int x_size, int y_size)
{
	//���łɓo�^���Ă���ID�ƈꏏ�Ȃ�I��
	for each (auto texture in *m_texture_list)
	{
		if (texture->GetID() == id)
			return false;
	}

	//���X�g�ɒǉ�����e�N�X�`���f�[�^
	shared_ptr<TextureData> add_texture_data(new TextureData(id));

	//�e�N�X�`���f�[�^������
	if (!add_texture_data->Initialize(file_name))
		return false;
	
	//��������ݒ�
	add_texture_data->SetSplit(all_num, x_num, y_num, x_size, y_size);

	//���X�g�ɒǉ�
	m_texture_list->push_back(move(add_texture_data));

	return true;
}
int DrawTexture2DClass::GetIndexCount()
{
    return m_index_count;
}
ID3D11ShaderResourceView* DrawTexture2DClass::GetTexture(int id)
{
	for each (auto itr in *m_texture_list)
	{
		if (itr->GetID() == id)
			return itr->GetTextureClass()->GetTexture();
	}

	return nullptr;
}
#pragma endregion

#pragma region TextureData

TextureData::TextureData(int id)
{
	m_id = id;
	m_split_data = nullptr;
}
TextureData::~TextureData()
{
	//�e�N�X�`�������폜
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}

	//�����p�ϐ����폜
	if (m_split_data)
	{
		delete m_split_data;
	}
}
bool TextureData::Initialize(const WCHAR* file_name)
{
	//�e�N�X�`���I�u�W�F�N�g���쐬
	m_texture = new TextureClass;
	if (!m_texture)
		return false;

	//�e�N�X�`���I�u�W�F�N�g��������
	if (!m_texture->Initialize(file_name))
		return false;

	return true;
}
/*�@�����f�[�^��ݒ肷��
����1 all_num	:�摜�̕�������
����2 x_num		:��������ۂ�X�����ɂ����������邩�̐�
����3 y_num		:��������ۂ�Y�����ɂ����������邩�̐�
����4 x_size	:�������ꂽ1�̑傫��(X)
����5 y_size	:�������ꂽ1�̑傫��(Y)
*/
void TextureData::SetSplit(int all_num, int x_num, int y_num, int x_size, int y_size)
{
	if (m_split_data == nullptr)
		m_split_data = new SplitData;

	m_split_data->all_num = all_num;
	m_split_data->x_num = x_num;
	m_split_data->y_num = y_num;
	m_split_data->x_size = x_size;
	m_split_data->y_size = y_size;
}
//�߂�l bool	:true = �����f�[�^������ :false = �����f�[�^������
bool TextureData::GetSplit(int* out_all_num, int* out_x_num, int* out_y_num, int* out_x_size, int* out_y_size)
{
	if (m_split_data == nullptr)
		return false;

	if (out_all_num)
		*out_all_num = m_split_data->all_num;
	if (out_x_num)
		*out_x_num = m_split_data->x_num;
	if (out_y_num)
		*out_y_num = m_split_data->y_num;
	if (out_x_size)
		*out_x_size = m_split_data->x_size;
	if (out_y_size)
		*out_y_size = m_split_data->y_size;

	return true;
}
#pragma endregion

#pragma region TextureClass

TextureClass::TextureClass()
{
	m_texture = nullptr;
	m_texture_file_name = nullptr;
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(const WCHAR* filename)
{
	HRESULT result;	//����m�F�p�ϐ�

	//�e�N�X�`���t�@�C������ۑ�
	m_texture_file_name = new WCHAR[100];
	wcscpy_s(m_texture_file_name,100, filename);

	//�e�N�X�`�������[�h����
	result = D3DX11CreateShaderResourceViewFromFile(D3D().GetDevice(), filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, L"�e�N�X�`����������܂���B", filename, MB_OK);
		return false;
	}
	//�e�N�X�`���̃T�C�Y���擾����
	// ���̍\���̂̒��ɉ摜�̏�񂪓���
	D3DX11_IMAGE_INFO info = {0};
	//�摜�̏����擾
	result = D3DX11GetImageInfoFromFile(filename, NULL,&info,NULL);
	if (FAILED(result))	//�������ɖ����Ƃ͎v�����ꉞ
	{
		MessageBox(NULL, L"�e�N�X�`����������܂���B", filename, MB_OK);
		return false;
	}
	//�T�C�Y���i�[
	m_width = info.Width;
	m_height = info.Height;

	return true;
}

void TextureClass::Shutdown()
{
	//�e�N�X�`�����\�[�X�����
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	if (m_texture_file_name)
	{
		delete[] m_texture_file_name;
		m_texture_file_name = nullptr;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}
#pragma endregion

