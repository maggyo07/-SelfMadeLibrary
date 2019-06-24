//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "ModelClass.h"

#include "AccessController.h"

using Graphics::D3D;

ModelClass::ModelClass()
{
	m_vertex_buffer = nullptr;
	m_index_buffer  = nullptr;
	m_model = nullptr;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

//�������֐�
bool ModelClass::Initialize(WCHAR* modelFilename)
{
	//���f����ǂݍ��݁A���_�A�C���f�b�N�X�o�b�t�@���쐬
	if (!InitalizeBuffers(modelFilename))
		return false;
	
	return true;
}

//�V���b�g�_�E���֐�
void ModelClass::Shutdown()
{
	//���f���e�N�X�`�������
	ReleaseTexture();

	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�����
	ShutdownBuffers();

	//���f���f�[�^��j��
	ReleaseModel();

	return;
}

//�`��֐�
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���O���t�B�b�N�p�C�v���C���ɔz�u���āA�`��̏���
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_index_count;
}

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	if (m_texture_array != nullptr)
		return m_texture_array->GetTextureArray(nullptr);
	else
		return nullptr;
}

//�������֐�(���f���̓ǂݍ��݁A�o�b�t�@���쐬)
bool ModelClass::InitalizeBuffers(const WCHAR* modelFilename)
{
	VertexType* vertexs;	//���_���i�[�p
	unsigned long* indices;	//�C���f�b�N�X���i�[�p
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;//���_�A�C���f�b�N�X�o�b�t�@���쐬����̂Ɏg�p����ϐ�
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;			//���_�A�C���f�b�N�X�o�b�t�@���쐬����̂Ɏg�p����ϐ�
	HRESULT result;			//����ɍs��ꂽ���ǂ����̊m�F�p

	//-----------�ʏ�A���f����ǂݍ��ނƂ���B-----------
	vertexs = nullptr; indices = nullptr;
	if (!LoadModel(modelFilename, vertexs, indices))
		return false;
	//���f���̖@���x�N�g���A�ڐ��x�N�g���A����ёo���K�x�N�g�����v�Z
	CalculateModelVectors();
	//--------------------------------------------------------------------
	//���_�z��ƃC���f�b�N�X�z��Ƀf�[�^�����[�h
	for (int i = 0; i < m_index_count; i++)
	{
		vertexs[i].position = D3DXVECTOR3(m_model[i].x,m_model[i].y, m_model[i].z);
		vertexs[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertexs[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertexs[i].tangent = D3DXVECTOR3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertexs[i].binormal = D3DXVECTOR3(m_model[i].bx, m_model[i].by, m_model[i].bz);

		//indices[i] = i;
	}

	//�ÓI���_�o�b�t�@�̐�����ݒ�
	vertex_buffer_desc.Usage				= D3D11_USAGE_DEFAULT;					//�o�b�t�@�őz�肳��Ă���ǂݍ��݋y�я������݂̕��@
	vertex_buffer_desc.ByteWidth			= sizeof(VertexType)* m_vertex_count;	//�o�b�t�@�̃T�C�Y
	vertex_buffer_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;				//�o�b�t�@���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	vertex_buffer_desc.CPUAccessFlags		= 0;									//CPU�A�N�Z�X�t���O
	vertex_buffer_desc.MiscFlags			= 0;									//���̑��̃t���O
	vertex_buffer_desc.StructureByteStride	= 0;									//�\���̂��\�����o�b�t�@��\���ꍇ�A���̍\���̂̃T�C�Y

	//subresource�\���̂ɒ��_�f�[�^�̃|�C���^��^����
	vertex_data.pSysMem				= vertexs;	//�������f�[�^�ւ̃|�C���^
	vertex_data.SysMemPitch			= 0;		//�e�N�X�`���ɂ���P�{�̐��̐�[����ׂ̐��܂ł̋���(�e�N�X�`���f�[�^�݂̂Ŏg�p�����)
	vertex_data.SysMemSlicePitch	= 0;		//�P�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(3D�e�N�X�`���f�[�^�݂̂Ŏg����)

	//���_�o�b�t�@���쐬
	result = D3D().GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_vertex_buffer);
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

//�V���b�g�_�E���֐�(�o�b�t�@��p)
void ModelClass::ShutdownBuffers()
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

//�`��֐�(GPU�̓��̓A�Z���u���Œ��_�A�C���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肷��)
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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
bool ModelClass::LoadTexture(WCHAR* filename)
{
	//�e�N�X�`���f�[�^�z�񂪍쐬����Ă��Ȃ���΍쐬����
	if (!m_texture_array)
		m_texture_array = new TextureArrayClass;

	if (!m_texture_array)
		return false;

	//�e�N�X�`�������[�h����
	if (!m_texture_array->LoadTexture(filename))
		return false;

	return true;
}
void ModelClass::ReleaseTexture()
{
	//�e�N�X�`���I�u�W�F�N�g�����
	if (m_texture_array)
	{
		delete m_texture_array;
		m_texture_array = nullptr;
	}

	return;
}

//�t�@�C�����烂�f������ǂݍ���
//���݂̓t�@�C���`���@obj�̂�
bool ModelClass::LoadModel(const WCHAR* filename, VertexType*& vertexs,unsigned long*& indices)
{
	ifstream fin;			//�t�@�C���̓ǂݍ��ݗp	
	char input;				//�t�@�C���̕�����ǂݍ��ݗp
	char input2;			//OBJ�t�@�C���̍Ō�̖ʏ���'/'��ǂݍ��ނ��߂̕ϐ�
	D3DXVECTOR3* v_vertexs, *v_texcoords, *v_normal;	//���_�A�e�N�X�`�����W�A�@�����i�[�p
	int vertex_index, texcoord_index, normal_index, face_index;//���_�A�e�N�X�`�����W�A�@���A�ʂ̏��擾���̃J�E���g
	int vertex_count, texture_count, normal_count, face_count;//���_�A�e�N�X�`�����W�A�@���A�ʂ̐�

	//�ʏ��i�[�p(�ԍ�)
	typedef struct
	{
		int vindex1, vindex2, vindex3;
		int tindex1, tindex2, tindex3;
		int nindex1, nindex2, nindex3;
	}FaceType;
	FaceType* faces;
	//�J�E���g��������
	vertex_count = 0;
	texture_count = 0;
	normal_count = 0;
	face_count = 0;
	vertex_index = 0;
	texcoord_index = 0;
	normal_index = 0;
	face_index = 0;

	//���f���t�@�C�����J��
	fin.open(filename);

	//�t�@�C�����I�[�v���ł��Ȃ������ꍇ�͏I��
	if (fin.fail())
		return false;

	//�t�@�C������ǂݍ��݁A�t�@�C���̏I���ɒB����܂œǂݍ��ݑ����܂�
	fin.get(input);
	while (!fin.eof())
	{
		//�s��'v'�Ŏn�܂��Ă���ꍇ�́A���_�A�e�N�X�`�����W�A�܂��͖@���x�N�g���̂����ꂩ�𐔂���
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertex_count++; }
			if (input == 't') { texture_count++; }
			if (input == 'n') { normal_count++; }
		}

		//�s�����Ŏn�܂��Ă���΁A�ʐ����C���N�������g
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { face_count++; }
		}

		//�����łȂ���΁A�s�̎c��̕�����ǂݍ���
		while (input != '\n')
			fin.get(input);

		//���̍s�̐�[����ǂݎn�߂�
		fin.get(input);
	}
	//�t�@�C�������
	fin.close();
	
	//�U�̃f�[�^��������
	v_vertexs = new D3DXVECTOR3[vertex_count];
	if (!v_vertexs)
		return false;
	
	v_texcoords = new D3DXVECTOR3[texture_count];
	if (!v_texcoords)
		return false;

	v_normal = new D3DXVECTOR3[normal_count];
	if (!v_normal)
		return false;

	vertexs = new VertexType[face_count*3];
	if (!vertexs)
		return false;

	indices = new unsigned long[face_count * 3];
	if (!indices)
		return false;

	faces = new FaceType[face_count];
	if (!faces)
		return false;

	//�����o�̒��_�A�C���f�b�N�X����ݒ�
	m_vertex_count = m_index_count = face_count * 3;

	//�t�@�C�����J��
	fin.open(filename);

	//�t�@�C���I�[�v���ɐ����������ǂ������m�F
	if (fin.fail())
		return false;

	//���_�A�e�N�X�`�����W�A�@�����f�[�^�ɓǂݍ���
	//�d�v�Fobj�t�@�C�����E����W�n���g�p����̂ŁA������W�n�ɕϊ�
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			//���_��ǂݍ���

			if (input == ' ')
			{

				fin >> v_vertexs[vertex_index].x >> v_vertexs[vertex_index].y >> v_vertexs[vertex_index].z;

				//Z���_�𔽓]�����č���n�ɕϊ�����
				v_vertexs[vertex_index].z = v_vertexs[vertex_index].z * -1.0f;
				vertex_index++;
			}

			//�e�N�X�`����uv���W��ǂݍ���
			if (input == 't')
			{
				fin >> v_texcoords[texcoord_index].x >> v_texcoords[texcoord_index].y;

				//V�e�N�X�`�����W������n�ɔ��]����
				v_texcoords[texcoord_index].y = 1.0f - v_texcoords[texcoord_index].y;
				texcoord_index++;
			}

			//�@����ǂݍ���
			if (input == 'n')
			{
				fin >> v_normal[normal_index].x >> v_normal[normal_index].y >> v_normal[normal_index].z;

				//Z�@���𔽓]���č���n�ɕϊ�
				v_normal[normal_index].z = v_normal[normal_index].z * -1.0f;
				normal_index++;
			}
		}

		//�ʂ�ǂݍ���
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				//�E��n���獶��n�ɕϊ����邽�߁A�ʃf�[�^������ɓǂݍ���
				fin >> faces[face_index].vindex3 >> input2 >> faces[face_index].tindex3 >> input2 >> faces[face_index].nindex3
					>> faces[face_index].vindex2 >> input2 >> faces[face_index].tindex2 >> input2 >> faces[face_index].nindex2
					>> faces[face_index].vindex1 >> input2 >> faces[face_index].tindex1 >> input2 >> faces[face_index].nindex1;

				//�ԍ����P�`�Ȃ̂łO�`�ɂ��邽��-1����
				faces[face_index].vindex1 -= 1; faces[face_index].tindex1 -= 1; faces[face_index].nindex1 -= 1;
				faces[face_index].vindex2 -= 1; faces[face_index].tindex2 -= 1; faces[face_index].nindex2 -= 1;
				faces[face_index].vindex3 -= 1; faces[face_index].tindex3 -= 1; faces[face_index].nindex3 -= 1;
				face_index++;
			}
		}

		//�c��̍s��ǂݍ���
		while (input != '\n')
			fin.get(input);

		//���̍s�̐�[����ǂݍ��ݎn�߂�
		fin.get(input);
	}
	//�t�@�C�������
	fin.close();

	//���_���ƃC���f�b�N�X������������
	for (int fc=0,vc = 0; fc < face_count; fc++,vc += 3)//fc=�ʐ��@vc=���_��
	{
		vertexs[vc].position = v_vertexs[faces[fc].vindex1];
		vertexs[vc].texture = D3DXVECTOR2(v_texcoords[faces[fc].tindex1]);
		vertexs[vc].normal = v_normal[faces[fc].nindex1];

		vertexs[vc+1].position = v_vertexs[faces[fc].vindex2];
		vertexs[vc+1].texture = D3DXVECTOR2(v_texcoords[faces[fc].tindex2]);
		vertexs[vc+1].normal = v_normal[faces[fc].nindex2];

		vertexs[vc+2].position = v_vertexs[faces[fc].vindex3];
		vertexs[vc+2].texture = D3DXVECTOR2(v_texcoords[faces[fc].tindex3]);
		vertexs[vc+2].normal = v_normal[faces[fc].nindex3];

		indices[vc] = vc;
		indices[vc+1] = vc+1;
		indices[vc+2] = vc+2;
	}
	//���������
	delete[] v_vertexs;
	v_vertexs = nullptr;

	delete[] v_texcoords;
	v_texcoords = nullptr;

	delete[] v_normal;
	v_normal = nullptr;

	delete[] faces;
	faces = nullptr;


	//m_model�Ƀf�[�^���i�[
	//�������ݒ�
	if (m_model == nullptr)
		m_model = new ModelType[m_index_count];

	for (int index_count = 0; index_count < m_index_count; index_count++)
	{
		m_model[index_count].x = vertexs[index_count].position.x;
		m_model[index_count].y = vertexs[index_count].position.y;
		m_model[index_count].z = vertexs[index_count].position.z;

		m_model[index_count].nx = vertexs[index_count].normal.x;
		m_model[index_count].ny = vertexs[index_count].normal.y;
		m_model[index_count].nz = vertexs[index_count].normal.z;

		m_model[index_count].tu = vertexs[index_count].texture.x;
		m_model[index_count].tv = vertexs[index_count].texture.y;
	}
	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
}

//���f���̐ڐ���2�����K���z�A����эČv�Z���ꂽ�@���x�N�g�����쐬
//���f�����ɂ����̖�(�O�p�`)�����邩���v�Z�A�����̎O�p�`��
//���ꂼ��ɂ��Ă����3�̒��_�𓾂āA������g���Đ��ځA2���K�A�����Ė@�����v�Z�B
//�����3�̖@���x�N�g�����v�Z������A���������f���\���ɕۑ�
void ModelClass::CalculateModelVectors()
{
	int face_count,index;
	TempVertexType vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal, normal;

	//���f�����̖ʂ̐����v�Z
	face_count = m_vertex_count / 3;

	//���f���f�[�^�ւ̃C���f�b�N�X��������
	index = 0;

	//�S�Ă̖ʂ𒲂ׁA�ڐ��x�N�g���A�񍀃x�N�g���B����і@���x�N�g�����v�Z
	for (int count = 0; count < face_count; count++)
	{
		//���f�����炱�̖ʂ�3�̒��_���擾����
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		//���̖ʂ̐��ڂƓ񍀐��K���z���v�Z
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		//�^���W�F���g�ƃo�C�m�[�}�����g�p���ĐV�����@�����v�Z
		CalculateNormal(tangent, binormal, normal);

		//���̖ʂ̖@���A�ڐ��A����ѓ񍀐��K���z�����f���\���Ɋi�[
		m_model[index - 1].nx = normal.x;
		m_model[index - 1].ny = normal.y;
		m_model[index - 1].nz = normal.z;
		m_model[index - 1].tx = tangent.x;
		m_model[index - 1].ty = tangent.y;
		m_model[index - 1].tz = tangent.z;
		m_model[index - 1].bx = binormal.x;
		m_model[index - 1].by = binormal.y;
		m_model[index - 1].bz = binormal.z;

		m_model[index - 2].nx = normal.x;
		m_model[index - 2].ny = normal.y;
		m_model[index - 2].nz = normal.z;
		m_model[index - 2].tx = tangent.x;
		m_model[index - 2].ty = tangent.y;
		m_model[index - 2].tz = tangent.z;
		m_model[index - 2].bx = binormal.x;
		m_model[index - 2].by = binormal.y;
		m_model[index - 2].bz = binormal.z;

		m_model[index - 3].nx = normal.x;
		m_model[index - 3].ny = normal.y;
		m_model[index - 3].nz = normal.z;
		m_model[index - 3].tx = tangent.x;
		m_model[index - 3].ty = tangent.y;
		m_model[index - 3].tz = tangent.z;
		m_model[index - 3].bx = binormal.x;
		m_model[index - 3].by = binormal.y;
		m_model[index - 3].bz = binormal.z;

	}
}

//3�̒��_����荞��ł���A�����3�̒��_�̐ڐ��Ƒo���K���v�Z���A�Ԃ�
void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	D3DXVECTOR3& tangent, D3DXVECTOR3& binormal)
{
	D3DXVECTOR3 vector1, vector2;
	D3DXVECTOR2 tu_vector, tv_vector;
	float den,length;

	//���̖ʂ�2�̃x�N�g�����v�Z
	vector1.x = vertex2.x - vertex1.x;
	vector1.y = vertex2.y - vertex1.y;
	vector1.z = vertex2.z - vertex1.z;

	vector2.x = vertex3.x - vertex1.x;
	vector2.y = vertex3.y - vertex1.y;
	vector2.z = vertex3.z - vertex1.z;

	//tu��tv�̃e�N�X�`����ԃx�N�g�����v�Z
	tu_vector.x = vertex2.tu - vertex1.tu;
	tv_vector.x = vertex2.tv - vertex1.tv;

	tu_vector.y = vertex3.tu - vertex1.tu;
	tv_vector.y = vertex3.tv - vertex1.tv;

	//�^���W�F���g/�o�C�m�[�}���������̕�����v�Z
	den = 1.0f / (tu_vector.x * tv_vector.y - tu_vector.y * tv_vector.x);

	//�O�ς��v�Z���A�W���������Đ��ڂƓ񐳋K���z�����߂�
	tangent = (tv_vector.y * vector1 - tv_vector.x * vector2) * den;

	binormal = (tu_vector.x * vector2 - tu_vector.y * vector1) * den;

	//���̖@���̒������v�Z
	length = D3DXVec3Length(&tangent);

	//�@���𐳋K�����Ă���A������i�[
	if (length > 0.0001f || length < -0.0001f)
		binormal = binormal / length;
	else
		binormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}
//���ڂƓ񍀐��K���Ƃ肱��ł���A�O�ς����s���Ė@���x�N�g����Ԃ�
void ModelClass::CalculateNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal)
{
	float length;

	//�@���x�N�g����������ڐ��Ɠ񍀐��K���z�̊O�ς��v�Z
	D3DXVec3Cross(&normal, &tangent, &binormal);

	//�@���̒������v�Z
	length = D3DXVec3Length(&normal);

	//�@���𐳋K������
	normal = normal / length;
}