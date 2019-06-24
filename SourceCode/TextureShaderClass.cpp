//�V�X�e���n�̃w�b�_�[�C���N���[�h

//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass()
{
	m_vertex_shader = nullptr;
	m_pixel_shader = nullptr;
	m_layout = nullptr;
	m_matrix_buffer = nullptr;
	m_sample_state = nullptr;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{

}

TextureShaderClass::~TextureShaderClass()
{

}

//�������֐�
bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	//���_�V�F�[�_�ƃs�N�Z���V�F�[�_������������
	if (!InitializeShader(device, hwnd, L"Texture.vs", L"Texture.ps"))
		return false;

	return true;
}

//�V���b�g�_�E���֐�
void TextureShaderClass::Shutdown()
{
	//���_�V�F�[�_�ƃs�N�Z���V�F�[�_�A����ъ֘A����I�u�W�F�N�g���V���b�g�_�E��
	ShutdownShader();

	return;
}

//�`��֐�
bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix,
				D3DXMATRIX viewMatrix, D3DXMATRIX ortho_matrix,ID3D11ShaderResourceView* texture)
{
	//�����_�����O�Ɏg�p����V�F�[�_�p�����[�^��ݒ�
	if (!SetShaderVariables(deviceContext, worldMatrix, viewMatrix, ortho_matrix,texture))
		return false;

	//���������o�b�t�@���V�F�[�_�Ń����_�����O
	RenderShader(deviceContext, indexCount);
	
	return true;
}
//�������֐�(�V�F�[�_�t�@�C�������[�h���ADirectX��GPU�Ŏg�p�ł���悤�ɂ���)
//����1 ID3D11Device* device	:�f�o�C�X�̃|�C���^
//����2 HWND hwnd				:�E�B���h�E�n���h��
//����3 WCHAR* vsFilename		:���_�V�F�[�_�R�[�h���i�[����Ă���t�@�C����(�p�X)
//����4 WCHAR* psFilename		:�s�N�Z���V�F�[�_�R�[�h���i�[����Ă���t�@�C����(�p�X)
bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;	//����m�F�p
	ID3D10Blob* error_message;					//�G���[���b�Z�[�W
	ID3D10Blob* vertex_shader_buffer;			//���_�V�F�[�_���i�[�p�o�b�t�@
	ID3D10Blob* pixel_shader_buffer;			//�s�N�Z���V�F�[�_���i�[�p�o�b�t�@
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];	//�|���S�����C�A�E�g
	unsigned int num_elements;					//�|���S�����C�A�E�g�̗v�f��
	D3D11_SAMPLER_DESC sampler_desc;			//�e�N�X�`���̋L�q��ێ�����
	D3D11_BUFFER_DESC matrix_buffer_desc;		//���_�V�F�[�_�̍s��萔�o�b�t�@

	//�|�C���^�ϐ��̏�����
	error_message = nullptr;
	vertex_shader_buffer = nullptr;
	pixel_shader_buffer = nullptr;

	//���_�V�F�[�_�R�[�h���R���p�C��
	//�֐����->//https://msdn.microsoft.com/ja-jp/library/ee416856(v=vs.85).aspx
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertex_shader_buffer, &error_message, NULL);
	if (FAILED(result))
	{
		//�V�F�[�_���R���p�C�����s�����ꍇ�́A�G���[���b�Z�[�W�ɉ����������܂��͂�
		if (error_message)
			OutputShaderErrorMessage(error_message, hwnd, vsFilename);
		//�G���[���b�Z�[�W�ɉ����Ȃ������ꍇ�A�P�ɃV�F�[�_�t�@�C�����̂������邱�Ƃ��ł��Ȃ�����
		else
			MessageBox(hwnd, vsFilename, L"���_�V�F�[�_�t�@�C��������܂���B", MB_OK);

		return false;
	}

	//�s�N�Z���V�F�[�_�R�[�h���R���p�C��
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixel_shader_buffer, &error_message, NULL);
	if (FAILED(result))
	{
		//�V�F�[�_���R���p�C�����s�����ꍇ�́A�G���[���b�Z�[�W�ɉ����������܂��͂�
		if (error_message)
			OutputShaderErrorMessage(error_message, hwnd, psFilename);
		//�G���[���b�Z�[�W�ɉ����Ȃ������ꍇ�A�P�ɃV�F�[�_�t�@�C�����̂������邱�Ƃ��ł��Ȃ�����
		else
			MessageBox(hwnd, psFilename, L"�s�N�Z���V�F�[�_�t�@�C��������܂���B", MB_OK);

		return false;
	}

	//�o�b�t�@���璸�_�V�F�[�_���쐬
	result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), NULL, &m_vertex_shader);
	if (FAILED(result))
		return false;

	//�o�b�t�@����s�N�Z���V�F�[�_���쐬
	result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), NULL,&m_pixel_shader);
	if (FAILED(result))
		return false;

	//�V�F�[�_�ɓ���f�[�^�̃��C�A�E�g��ݒ�
	//���̐ݒ�́AModelClass�ƃV�F�[�_��VertexType�\���Ɉ�v����K�v������
	polygon_layout[0].SemanticName = "POSITION";						//�V�F�[�_�[���͏����ł��̗v�f�Ɋ֘A�t�����Ă��� HLSL �Z�}���e�B�N�X
	polygon_layout[0].SemanticIndex = 0;								//�v�f�̃Z�}���e�B�N�X �C���f�b�N�X
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				//�v�f�f�[�^�̃f�[�^�^
	polygon_layout[0].InputSlot = 0;									//���̓A�Z���u���[�����ʂ��鐮���l
	polygon_layout[0].AlignedByteOffset = 0;							//�e�v�f�Ԃ̃I�t�Z�b�g 
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;		//�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������
	polygon_layout[0].InstanceDataStepRate = 0;							//�o�b�t�@�[�̒��ŗv�f�� 1 ���i�ޑO�ɁA�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐��ł�

	polygon_layout[1].SemanticName = "TEXCOORD";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

	//���C�A�E�g���̗v�f�����擾
	num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

	//���_���̓��C�A�E�g���쐬
	result = device->CreateInputLayout(polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(),
		vertex_shader_buffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
		return false;

	//���_�V�F�[�_�o�b�t�@�ƃs�N�Z���V�F�[�_�o�b�t�@�́A�����K�v�Ȃ��̂ŉ��
	vertex_shader_buffer->Release();
	vertex_shader_buffer = 0;

	pixel_shader_buffer->Release();
	pixel_shader_buffer = 0;

	//�e�N�X�`���T���v���̏�ԋL�q���쐬
	sampler_desc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//�t�B���^�����O
	sampler_desc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;		//�O�`�P�͈̔͊O�ɂ���U�e�N�X�`�����W�̉������@
	sampler_desc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;		//�O�`�P�͈̔͊O�ɂ���V�e�N�X�`�����W�̉������@
	sampler_desc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;		//�O�`�P�͈̔͊O�ɂ���W	�e�N�X�`�����W�̉������@
	sampler_desc.MipLODBias		= 0.0f;								//�v�Z���ꂽ�~�b�v�}�b�v���x������̃I�t�Z�b�g
	sampler_desc.MaxAnisotropy	= 1;								//����̃t�B���^�����O�̎��Ɏg�p�����N�����v�l
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;			//�����̃T���v�����O�f�[�^�ɑ΂��ăf�[�^���r����֐�	
	sampler_desc.BorderColor[0] = 0;								//AddressU,V,W��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p����t���E�̐F
	sampler_desc.BorderColor[1] = 0;								//		
	sampler_desc.BorderColor[2] = 0;								//
	sampler_desc.BorderColor[3] = 0;								//
	sampler_desc.MinLOD			= 0;								//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̉���
	sampler_desc.MaxLOD			= D3D11_FLOAT32_MAX;				//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̏��

	//�e�N�X�`���T���v���̏�Ԃ��쐬
	result = device->CreateSamplerState(&sampler_desc, &m_sample_state);
	if (FAILED(result))
		return false;

	//���_�V�F�[�_�ɂ���s��萔�o�b�t�@�̋L�q���Z�b�g�A�b�v
	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;			//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@
	matrix_buffer_desc.ByteWidth = sizeof(MatrixBufferType);		//�o�b�t�@�̃T�C�Y
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�o�b�t�@�[���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//CPU �A�N�Z�X�̃t���O
	matrix_buffer_desc.MiscFlags = 0;							//���̑��̃t���O
	matrix_buffer_desc.StructureByteStride = 0;							//�\���̂��\�����o�b�t�@�[��\���ꍇ�A���̍\���̂̃T�C�Y

	//���̃N���X�����璸�_�V�F�[�_�萔�o�b�t�@�ɃA�N�Z�X�ł���悤�ɁA�萔�o�b�t�@�|�C���^���쐬
	result = device->CreateBuffer(&matrix_buffer_desc, NULL, &m_matrix_buffer);
	if (FAILED(result))
		return false;

	return true;
}
//�V���b�g�_�E���֐�(�V�F�[�_�֘A�̃��������)
void TextureShaderClass::ShutdownShader()
{
	//�s��萔�o�b�t�@�����
	if (m_matrix_buffer)
	{
		m_matrix_buffer->Release();
		m_matrix_buffer = nullptr;
	}

	//�T���v���[�̏�Ԃ����
	if (m_sample_state)
	{
		m_sample_state->Release();
		m_sample_state = nullptr;
	}

	//���C�A�E�g�����
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	//�s�N�Z���V�F�[�_���
	if (m_pixel_shader)
	{
		m_pixel_shader->Release();
		m_pixel_shader = nullptr;
	}

	//���_�V�F�[�_�����
	if (m_vertex_shader)
	{
		m_vertex_shader->Release();
		m_vertex_shader = nullptr;
	}

	return;
}

//�V�F�[�_���R���p�C������Ƃ��ɐ��������G���[���b�Z�[�W���������ފ֐�
//����1 ID3D10Blob* errorMessage	:�G���[���b�Z�[�W
//����2 HWND		hwnd			:�E�B���h�E�n���h��
//����3 WVHAR*		shaderFilename	:�V�F�[�_�t�@�C���̖��O
void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMassage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compile_errors;			//���b�Z�[�W(������)
	unsigned long buffer_size, i;	//���b�Z�[�W�̒����ƃ��[�v�p
	ofstream fout;					//�t�@�C���̏������ݗp

	//�G���[���b�Z�[�W�̃e�L�X�g�o�b�t�@�ւ̃|�C���^���擾
	compile_errors = (char*)(errorMassage->GetBufferPointer());

	//���b�Z�[�W�̒������擾
	buffer_size = errorMassage->GetBufferSize();

	//�G���[���b�Z�[�W���������ރt�@�C�����J��
	//�Ȃ���ΐ��������
	fout.open("shader-error.txt");

	//�G���[���b�Z�[�W���o��
	for (i = 0; i < buffer_size; i++)
		fout << compile_errors[i];

	//�t�@�C�������
	fout.close();

	//�G���[���b�Z�[�W�����
	errorMassage->Release();
	errorMassage = 0;

	//��ʏ�Ƀ��b�Z�[�W���|�b�v�A�b�v���āA�e�L�X�g�t�@�C���̃R���p�C���G���[���`�F�b�N����悤�Ƀ��[�U�[�ɒʒm����
	MessageBox(hwnd, L"�V�F�[�_�[�̃R���p�C�����ɃG���[���������܂����Bshader-error.txt�Ń��b�Z�[�W���m�F���Ă��������B", shaderFilename, MB_OK);

	return;
}

//�V�F�[�_�[���̃O���[�o���ϐ��̐ݒ��e�Ղɂ��邽�߂̊֐�
bool TextureShaderClass::SetShaderVariables(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
								D3DXMATRIX viewMatrix, D3DXMATRIX projextionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;								//����m�F�p
	D3D11_MAPPED_SUBRESOURCE mapped_resource;	//�}�b�v���ꂽ�T�u���\�[�X�ւ̃|�C���^�[
	unsigned int buffer_number;					//���_�V�F�[�_���̒萔�o�b�t�@�̈ʒu
	MatrixBufferType* data_ptr;					//�s��萔�o�b�t�@���̃f�[�^�ւ̃|�C���^

	//�s����ڒ����ăV�F�[�_�̂��߂ɏ����B�����DirectX11�̗v���ł�
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projextionMatrix, &projextionMatrix);

	//�������ނ��Ƃ��ł���悤�ɒ萔�o�b�t�@���b�N����
	result = deviceContext->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return false;

	//�s��萔�o�b�t�@���̃f�[�^�ւ̃|�C���^���擾
	data_ptr = (MatrixBufferType*)mapped_resource.pData;

	//�s���萔�o�b�t�@�ɃR�s�[����
	data_ptr->world = worldMatrix;
	data_ptr->view = viewMatrix;
	data_ptr->projection = projextionMatrix;

	//�萔�o�b�t�@�̃��b�N������
	deviceContext->Unmap(m_matrix_buffer, 0);

	//���_�V�F�[�_���̒萔�o�b�t�@�̈ʒu��ݒ�
	buffer_number = 0;

	//�X�V���ꂽ�l�Œ��_�V�F�[�_�̍s��萔�o�b�t�@��Finanly�ɐݒ�
	deviceContext->VSSetConstantBuffers(buffer_number, 1, &m_matrix_buffer);

	//�s�N�Z���V�F�[�_�ɃV�F�[�_�e�N�X�`�����\�[�X��ݒ�
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

//���̓A�Z���u���œ��̓��C�A�E�g���A�N�e�B�u�ɐݒ肵�A
//���_�o�b�t�@�̃����_�����O�Ɏg�p���钸�_�A�s�N�Z���V�F�[�_��ݒ�
void TextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//���_�̓��̓��C�A�E�g��ݒ�
	deviceContext->IASetInputLayout(m_layout);

	//���̎O�p�`��`�悷�邽�߂Ɏg�p����钸�_�V�F�[�_�ƃs�N�Z���V�F�[�_��ݒ�
	deviceContext->VSSetShader(m_vertex_shader, NULL, 0);
	deviceContext->PSSetShader(m_pixel_shader, NULL, 0);

	//�s�N�Z���V�F�[�_�ŃT���v���̏�Ԃ�ݒ�
	deviceContext->PSSetSamplers(0, 1, &m_sample_state);

	//�����_�����O���܂�
	deviceContext->DrawIndexed(indexCount, 0, 0);
}



