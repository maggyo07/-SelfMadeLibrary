//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "BitmapClass.h"

BitmapClass::BitmapClass()
{
	m_vertex_buffer = nullptr;
	m_index_buffer  = nullptr;
	m_texture = nullptr;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{

}

BitmapClass::~BitmapClass()
{

}

//�������֐�
bool BitmapClass::Initialize(ID3D11Device* device,int screenWidth,int screenHeight,WCHAR* textureFilename,int bitmapWidth,int bitmapHeight)
{
	//�摜�T�C�Y��ۑ�����
	m_screen_width = screenWidth;
	m_screen_height = screenHeight;

	//���̃r�b�g�}�b�v�������_�����O����s�N�Z���P�ʂ̃T�C�Y���i�[
	m_bitmap_width = bitmapWidth;
	m_bitmap_height = bitmapHeight;

	//�O�̃����_�����O�ʒu�𕉂̒l�ɏ�����
	m_previous_posX = -1;
	m_previous_posY = -1;

	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@��������
	if (!InitalizeBuffers(device))
		return false;

	//���̃��f���̃e�N�X�`�������[�h����
	if (!LoadTexture(device, textureFilename))
		return false;

	return true;
}

//�V���b�g�_�E���֐�
void BitmapClass::Shutdown()
{
	//���f���e�N�X�`�������
	ReleaseTexture();

	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�����
	ShutdownBuffers();

	return;
}

//�`��֐�
 bool BitmapClass::Render(ID3D11DeviceContext* deviceContext,int positionX, int positionY)
{
	//�����_�����O�̂��߂ɓ��I���_�o�b�t�@���č\�z���āA�����炭��ʏ�̕ʂ̏ꏊ�ɂ��܂��B
	if (!UpdateBuffers(deviceContext, positionX, positionY))
		return false;

	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���O���t�B�b�N�p�C�v���C���ɔz�u���āA�`��̏���
	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_index_count;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_texture->GetTexture();
}

//�������֐�(���f���̓ǂݍ��݁A�o�b�t�@���쐬)
bool BitmapClass::InitalizeBuffers(ID3D11Device* device)
{
	VertexType* vertexs;	//���_���i�[�p
	unsigned long* indices;	//�C���f�b�N�X���i�[�p
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//���_�A�C���f�b�N�X�o�b�t�@���쐬����̂Ɏg�p����ϐ�
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//���_�A�C���f�b�N�X�o�b�t�@���쐬����̂Ɏg�p����ϐ�
	HRESULT result;			//����ɍs��ꂽ���ǂ����̊m�F�p

	//-----------4�p�`�̔|���S���쐬-----------
	//���_�z��̒��_�̐���ݒ�
	m_vertex_count = 6;
	//�C���f�b�N�X�z����̃C���f�b�N�X�̐���ݒ�
	m_index_count = m_vertex_count;

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
	for (int i = 0; i < m_index_count; i++)
		indices[i] = i;
	//-------------------------------------------
	//���I���_�o�b�t�@�̐�����ݒ�
	vertex_buffer_desc.Usage				= D3D11_USAGE_DYNAMIC;					//�o�b�t�@�őz�肳��Ă���ǂݍ��݋y�я������݂̕��@
	vertex_buffer_desc.ByteWidth			= sizeof(VertexType)* m_vertex_count;	//�o�b�t�@�̃T�C�Y
	vertex_buffer_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;				//�o�b�t�@���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	vertex_buffer_desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;				//CPU�A�N�Z�X�t���O
	vertex_buffer_desc.MiscFlags			= 0;									//���̑��̃t���O
	vertex_buffer_desc.StructureByteStride	= 0;									//�\���̂��\�����o�b�t�@��\���ꍇ�A���̍\���̂̃T�C�Y

	//subresource�\���̂ɒ��_�f�[�^�̃|�C���^��^����
	vertex_data.pSysMem				= vertexs;	//�������f�[�^�ւ̃|�C���^
	vertex_data.SysMemPitch			= 0;		//�e�N�X�`���ɂ���P�{�̐��̐�[����ׂ̐��܂ł̋���(�e�N�X�`���f�[�^�݂̂Ŏg�p�����)
	vertex_data.SysMemSlicePitch	= 0;		//�P�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(3D�e�N�X�`���f�[�^�݂̂Ŏg����)

	//���_�o�b�t�@���쐬
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
	if (FAILED(result))
		return false;

	//�ÓI�C���f�b�N�X�o�b�t�@�̐�����ݒ�
	index_buffer_desc.Usage					= D3D11_USAGE_DEFAULT;					//�o�b�t�@�őz�肳��Ă���ǂݍ��݋y�я������݂̕��@
	index_buffer_desc.ByteWidth				= sizeof(unsigned long)* m_index_count;	//�o�b�t�@�̃T�C�Y
	index_buffer_desc.BindFlags				= D3D11_BIND_INDEX_BUFFER;				//�o�b�t�@���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	index_buffer_desc.CPUAccessFlags		= 0;									//CPU�A�N�Z�X�t���O
	index_buffer_desc.MiscFlags				= 0;									//���̑��̃t���O
	index_buffer_desc.StructureByteStride	= 0;									//�\���̂��\�����o�b�t�@��\���ꍇ�A���̍\���̂̃T�C�Y

	//�T�u���\�[�X�\���̂ɃC���f�b�N�X�f�[�^�ւ̃|�C���^��Ԃ��܂��B
	index_data.pSysMem			= indices;	//�������f�[�^�ւ̃|�C���^
	index_data.SysMemPitch		= 0;		//�e�N�X�`���ɂ���P�{�̐��̐�[����ׂ̐��܂ł̋���(�e�N�X�`���f�[�^�݂̂Ŏg�p�����)
	index_data.SysMemSlicePitch	= 0;		//�P�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(3D�e�N�X�`���f�[�^�݂̂Ŏg����)

	//�C���f�b�N�X�o�b�t�@���쐬
	result = device->CreateBuffer(&index_buffer_desc, &index_data, &m_index_buffer);
	if (FAILED(result))
		return false;

	//���_�ƃC���f�b�N�X�̃o�b�t�@���쐬���ꃍ�[�h���ꂽ�̂ŁA�z������
	delete[] vertexs;
	vertexs = 0;

	delete[] indices;
	indices = 0;

	return true;
}

//�V���b�g�_�E���֐�(�o�b�t�@��p)
void BitmapClass::ShutdownBuffers()
{
	//�C���f�b�N�X�o�b�t�@�����
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = 0;
	}

	//���_�o�b�t�@�����
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = 0;
	}

	return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left,right, top, bottom;				//�r�b�g�}�b�v�̊e��ʂ̍��W
	VertexType* vertices;						//�X�V���钸�_���
	D3D11_MAPPED_SUBRESOURCE mapped_resource;	//���_�o�b�t�@�̏��
	VertexType* vertices_ptr;					//���_�o�b�t�@���ւ̃|�C���^
	HRESULT result;								//����m�F�p

	//�O�̃t���[���ƈʒu�ɕω����Ȃ���΁A���_�o�b�t�@���X�V���Ȃ�
	if ((positionX == m_previous_posX) && (positionY == m_previous_posY))
		return true;

	//�ύX����Ă���ꍇ�́A�����_�����O�����ʒu���X�V
	m_previous_posX = positionX;
	m_previous_posY = positionY;

	//�r�b�g�}�b�v�̍����̉�ʍ��W���v�Z
	left = (float)((m_screen_width / 2)*-1) + (float)positionX;

	//�r�b�g�}�b�v�̉E���̉�ʍ��W���v�Z
	right = left + (float)m_bitmap_width;

	//�r�b�g�}�b�v�̏㕔�̉�ʍ��W���v�Z
	top = (float)(m_screen_height / 2) - (float)positionY;

	//�r�b�g�}�b�v�̉����̉摜���W���v�Z
	bottom = top - (float)m_bitmap_height;

	//���_�z����쐬
	vertices = new VertexType[m_vertex_count];
	if (!vertices)
		return false;

	//���_�z��Ƀf�[�^�����[�h����
	//�ŏ��̎O�p�`
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);	//����
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);//�E��
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);	//����
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	//�Q�Ԗڂ̎O�p�`
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);	//����
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);	//�E��
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);//�E��
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	//�������ނ��Ƃ��ł���悤�ɒ��_�o�b�t�@�����b�N
	result = deviceContext->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//���_�o�b�t�@���̃f�[�^�ւ̃|�C���^���擾
	vertices_ptr = (VertexType*)mapped_resource.pData;

	//�f�[�^�𒸓_�o�b�t�@�ɃR�s�[����
	memcpy(vertices_ptr, (void*)vertices, (sizeof(VertexType)*m_vertex_count));

	//���_�o�b�t�@�̃��b�N������
	deviceContext->Unmap(m_vertex_buffer, 0);

	//���_�z�񂪕s�v�ɂȂ�������
	delete[] vertices;
	vertices = nullptr;

	return true;
}
//�`��֐�(GPU�̓��̓A�Z���u���Œ��_�A�C���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肷��)
void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//�X�g���C�h
	unsigned int stride;
	//�I�t�Z�b�g(��_���狗���ŕ\�����ʒu)
	unsigned int offset;

	//���_�o�b�t�@�̃X�g���C�h�ƃI�t�Z�b�g��ݒ�
	stride = sizeof(VertexType);
	offset = 0;

	//�����_�����O�ł���悤�ɁA���̓A�Z���u���Œ��_�o�b�t�@���A�N�e�B�u�ɐݒ�
	deviceContext->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	//���̓A�Z���u���ŃC���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肵�āA�����_�����O�ł���悤�ɂ���
	deviceContext->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//���̒��_�o�b�t�@���烌���_�����O����v���~�e�B�u�̌^��ݒ肷��B���̏ꍇ�͎O�p�`�ł�
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return;

}
//�t�@�C��������e�N�X�`����ǂݍ���
bool BitmapClass::LoadTexture(ID3D11Device* device,WCHAR* filename)
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
void BitmapClass::ReleaseTexture()
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
