//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "D3DClass.h"

D3DClass::D3DClass()
{
	m_swap_chain = nullptr;
	m_device = nullptr;
	m_device_context = nullptr;
	m_render_target_view = nullptr;
	m_depth_stencil_buffer = nullptr;
	m_depth_stencil_state = nullptr;
	m_depth_disabled_stencil_state = nullptr;
	m_depth_stencil_view = nullptr;
	m_raster_state = nullptr;
	m_alpha_enable_blending_state = nullptr;
	m_alpha_disable_blending_state = nullptr;
}

/*�������֐�
����1 int screenWidth	:�E�B���h�E�̕�
����2 int screenHeight	:�E�B���h�E�̍���
����3 bool vsync		:true = �Q�[���̃t���[�����[�g�����j�^�[�̃��t���b�V�����[�g�Ɠ����@false = �ł��邾������
����4 HWND hwnd			:�E�B���h�E�n���h��
����5 bool fullscreen	:true = �t���X�N���[���@false = �E�B���h�E���[�h
����6 float screenDepth	:�����r���[���ʂ�Z�l
����7 float screenNear	:�߂��r���[���ʂ�Z�l
*/
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	D3DClass();
	HRESULT result;											//����ɍs��ꂽ���ǂ����̊m�F�p
	IDXGIFactory* factory;									//�t���X�N���[���؂�ւ�����������N���X
	IDXGIAdapter* adapter;									//�f�B�X�v���C �T�u�V�X�e��
	IDXGIOutput* adapter_output;							//�A�_�v�^�[�o�� (���j�^�[�Ȃ�) 
	unsigned int num_modes;									//�\�����[�h�̐�
	unsigned int numerator = 0;									//���t���b�V�� ���[�g (Hz �P��) ��\�����q
	unsigned int denominator = 0;								//���t���b�V�� ���[�g (Hz �P��) ��\������
	unsigned int string_length;								//�r�f�I�J�[�h�̖��O�̕�����
	DXGI_MODE_DESC* display_mode_list;						//�\�����[�h
	DXGI_ADAPTER_DESC adapter_desc;							//�A�_�v�^�[
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;					//�X���b�v�`�F�[��(�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@���g���āA��ʂ��X�V����d�g�݁B)
	D3D_FEATURE_LEVEL feature_level;						//�^�[�Q�b�g�ƂȂ�@�\�Z�b�g(DirectX�̃o�[�W����)
	ID3D11Texture2D* back_buffer_ptr;						//�\�������ꂽ�������[�ł���e�N�Z�� �f�[�^���Ǘ�(�o�b�N�o�b�t�@�ւ̃|�C���^)
	D3D11_TEXTURE2D_DESC depth_buffer_desc;					//2D �e�N�X�`���[(�[�x�o�b�t�@)
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;			//�[�x�X�e���V�� �X�e�[�g
	D3D11_DEPTH_STENCIL_DESC depth_disabied_stencil_desc;	//�[�x�X�e���V�� �X�e�[�g(Z�o�b�t�@OFF)
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;	//�[�x�X�e���V�� �r���[����A�N�Z�X�\�ȃe�N�X�`���[�̃T�u���\�[�X
	D3D11_RASTERIZER_DESC raster_desc;						//���X�^���C�U�[ �X�e�[�g(�ǂ̂悤�Ƀ|���S����`�悷�邩�����肷�郉�X�^�L�q)
	D3D11_VIEWPORT viewport;								//�r���[�|�[�g
	D3D11_BLEND_DESC blend_state_description;				//�u�����h��Ԃ�ݒ肷�Ã����̐V�����L�q�ϐ�

															//vsync�̐ݒ��ۑ�����
	m_vsync_enabled = vsync;

	//---------------�r�f�I�J�[�h/���j�^���烊�t���b�V�����[�g���擾---------------
	//DirectX�O���t�B�b�N�X�C���^�[�t�F�C�X�t�@�N�g�����쐬
	/*
	CreateDXGIFactory�́A�A�_�v�^�[�̗񋓁A�X���b�v �`�F�[���̍쐬�A
	����ёS��ʕ\�����[�h�Ƃ̊Ԃ̐؂�ւ��Ɏg�p�����
	Alt + Enter �L�[ �V�[�P���X�Ƃ̃E�B���h�E�̊֘A�t�����s���I�u�W�F�N�g�𐶐����邽�߂Ɏg�p���܂��B
	*/
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
		return false;

	//�t�@�N�g�����g�p���āA�v���C�}���O���t�B�b�N�X�C���^�[�t�F�C�X(�r�f�I�J�[�h)�̃A�_�v�^���쐬
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
		return false;

	//�v���C�}���A�_�v�^�o��(���j�^)���
	result = adapter->EnumOutputs(0, &adapter_output);
	if (FAILED(result))
		return false;

	//�A�_�v�^�o��(���j�^)��DXGI_FORMAT_R8G8B8A8_UNORM�\���`���ɓK�����郂�[�h�̐����擾
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
	if (FAILED(result))
		return false;

	//���̃��j�^�[/�r�f�I�J�[�h�̑g�ݍ��킹�ŉ\�Ȃ��ׂĂ̕\�����[�h��ێ����郊�X�g���쐬
	display_mode_list = new DXGI_MODE_DESC[num_modes];
	if (!display_mode_list)
		return false;

	//�\�����[�h�̃��X�g�\�����L��
	result = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
	if (FAILED(result))
		return false;

	//���ׂĂ̕\�����[�h���s���āA��ʂ̕��ƍ�������v����\�����[�h��������
	//��v������̂�������ƁA���̃��j�^�[�̃��t���b�V�����[�g�̕��q�ƕ�����i�[
	for (unsigned int i = 0; i < num_modes; i++)
	{
		if (display_mode_list[i].Width == (unsigned int)screenWidth)
		{
			if (display_mode_list[i].Height == (unsigned int)screenHeight)
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denominator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}
	if (numerator == 0 && denominator == 0)
	{
		MessageBox(NULL, L"��ʂ̕��ƍ������������擾�ł��܂���ł���", NULL, MB_OK);
	}
	//-----------------------------------------------------------------------------
	//------------------�r�f�I�J�[�h�̖��O�ƃ������ʂ��擾-------------------------
	//�A�_�v�^(�r�f�I�J�[�h)�̐������擾
	result = adapter->GetDesc(&adapter_desc);
	if (FAILED(result))
		return false;

	//��p�̃r�f�I�J�[�h�̃����������K�o�C�g�P�ʂŕۑ�
	m_video_card_memory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);

	//�r�f�I�J�[�h�̖��O�𕶎���ɕϊ����ĕۑ�
	int error = wcstombs_s(&string_length, m_video_card_description, 128, adapter_desc.Description, 128);
	if (error != 0)
		return false;
	//-----------------------------------------------------------------------------
	//���t���b�V�����[�g�ƃr�f�I�J�[�h���̕��q�A�����ۑ������̂ŁA�\�����[�h���X�g�ƃA�_�v�^�ƃt�@�N�g�������------
	//�\�����[�h���X�g�����
	delete[] display_mode_list;
	display_mode_list = 0;

	//�A�_�v�^�o�͂����
	adapter_output->Release();
	adapter_output = 0;

	//�A�_�v�^�����
	adapter->Release();
	adapter = 0;

	//�t�@�N�g�������
	factory->Release();
	factory = 0;
	//------------------------------------------------------------------------------------------------------------------
	//-----------------------------DirectX�̏�����-----------------------------
	//�X���b�v�`�F�[���̐�����������
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

	//�P��̃o�b�N�o�b�t�@�ɐݒ�
	swap_chain_desc.BufferCount = 1;

	//�o�b�N�o�b�t�@�̕��ƍ�����ݒ�
	swap_chain_desc.BufferDesc.Width = screenWidth;
	swap_chain_desc.BufferDesc.Height = screenHeight;

	//�o�b�N�o�b�t�@�p�ɒʏ�̂R�Q�r�b�g�T�[�t�F�X��ݒ�
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//�o�b�N�o�b�t�@�̃��t���b�V�����[�g��ݒ�
	if (m_vsync_enabled)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//�o�b�N�o�b�t�@�̎g�p���@��ݒ�
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//�����_�����O����E�B���h�E�n���h����ݒ�
	swap_chain_desc.OutputWindow = hwnd;

	//�}���`�T���v�����O���I�t�ɂ���(�I���ɂ���ƃ|���S���̒[�����̃M�U�M�U���X���[�Y�ɂł���)
	swap_chain_desc.SampleDesc.Count = 1;	//�s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
	swap_chain_desc.SampleDesc.Quality = 0;	//�C���[�W�̕i�����x�� 0�`-1

											//�t���X�N���[�����[�h�܂��̓E�B���h�E���[�h�ɐݒ�
	if (fullscreen)
		swap_chain_desc.Windowed = false;
	else
		swap_chain_desc.Windowed = true;

	//�������̏����ƃX�P�[�����O���w�肵�Ȃ��悤�ɐݒ�
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//�v���[���e�[�V������Ƀo�b�N�o�b�t�@�̓��e��j���ݒ�
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//�X���b�v �`�F�[���̓���̐ݒ�
	swap_chain_desc.Flags = 0;

	//�@�\���x��(�o�[�W����)��DirectX 11�ɐݒ�
	feature_level = D3D_FEATURE_LEVEL_11_0;

	//�X���b�v�`�F�[���ADirect3D�f�o�C�X�ADirect3D�f�o�C�X�R���e�L�X�g���쐬
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &feature_level, 1,
		D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, NULL, &m_device_context);
	if (FAILED(result))
		return false;	//DirectX 11�r�f�I�J�[�h�������Ă��Ȃ��ꍇ�AD3D11CreateDeviceAndSwapChain�֐��Ăяo���̓f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̍쐬�Ɏ��s���܂�
						//-------------------------------------------------------------------------

						//--------------------------�o�b�N�o�b�t�@�|�C���^����X���b�v�`�F�[���ɃA�^�b�`����-----------------
						//�o�b�N�o�b�t�@�ւ̃|�C���^���擾
	result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);
	if (FAILED(result))
		return false;


	//�o�b�N�o�b�t�@�|�C���^�Ń����_�[�^�[�Q�b�g�r���[���쐬
	result = m_device->CreateRenderTargetView(back_buffer_ptr, NULL, &m_render_target_view);
	if (FAILED(result))
		return false;

	//�g���I������o�b�N�o�b�t�@�|�C���^���������
	back_buffer_ptr->Release();
	back_buffer_ptr = 0;
	//---------------------------------------------------------------------------------------------------

	//------------�[�x�o�b�t�@���쐬���A�|���S����3D��ԂœK�؂Ƀ����_�����O�ł���悤�ɂ���-------------
	//�[�x�o�b�t�@�̋L�q������������
	ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

	//�[�x�o�b�t�@�̋L�q��ݒ�
	depth_buffer_desc.Width = screenWidth;					//�e�N�X�`���[�̕�(�e�N�Z���P��)
	depth_buffer_desc.Height = screenHeight;					//�e�N�X�`���[�̍���(�e�N�Z���P��)
	depth_buffer_desc.MipLevels = 1;							//�~�b�v�}�b�v���x���̍ő吔
	depth_buffer_desc.ArraySize = 1;							//�e�N�X�`���[�z����̃e�N�X�`���[�̐�
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//�e�N�X�`���[�t�H�[�}�b�g
	depth_buffer_desc.SampleDesc.Count = 1;							//�s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
	depth_buffer_desc.SampleDesc.Quality = 0;							//�C���[�W�̕i�����x���B�O�`�[�P
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;			//�e�N�X�`���[�̓ǂݍ��݂���я������ݕ��@�����ʂ���l
	depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//�p�C�v���C�� �X�e�[�W�ւ̃o�C���h�Ɋւ���t���O
	depth_buffer_desc.CPUAccessFlags = 0;							//������ CPU �A�N�Z�X�̎�ނ��w�肷��t���O
	depth_buffer_desc.MiscFlags = 0;							//���̈�ʐ��̒Ⴂ���\�[�X �I�v�V���������ʂ���t���O

																//�L�q���ꂽ�������g�p���Đ[�x�o�b�t�@�̃e�N�X�`�����쐬����
	result = m_device->CreateTexture2D(&depth_buffer_desc, NULL, &m_depth_stencil_buffer);
	if (FAILED(result))
		return false;
	//---------------------------------------------------------------------------------------------------

	//-------------------------------�[�x�X�e���V����Ԃ�ݒ�E�쐬����----------------------------------
	//�X�e���V����Ԃ̋L�q������������
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

	//�X�e���V����Ԃ̐�����ݒ�
	depth_stencil_desc.DepthEnable = true;							//�[�x�e�X�g���g�p�\�ɂ��邩�ǂ���
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//�[�x�X�e���V���o�b�t�@�̒��ŁA�[�x�f�[�^�ɂ��ύX���\�ȕ���������
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;		//�[�x�f�[�^�������̐[�x�f�[�^�Ƃǂ���r����

	depth_stencil_desc.StencilEnable = true;							//�X�e���V���e�X�g���g�p�\�ɂ��邩�ǂ���
	depth_stencil_desc.StencilReadMask = 0xFF;							//�[�x�X�e���V���o�b�t�@�̒��ŁA�X�e���V���f�[�^��ǂݎ�镔��������
	depth_stencil_desc.StencilWriteMask = 0xFF;							//�[�x�X�e���V���o�b�t�@�̒��ŁA�X�e���V���f�[�^���������ޕ���������

																		//�s�N�Z�����O�ʂɌ����Ă���ꍇ�̃X�e���V������
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//�X�e���V���e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;	//�X�e���V���e�X�g�ɍ��i���A�[�x�e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//�X�e���V���e�X�g�Ɛ[�x�e�X�g�̗����ɍ��i�����Ƃ��A���s����X�e���V������
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//�X�e���V���f�[�^�������̃X�e���V���f�[�^�Ƃǂ���r����

																		//�s�N�Z�����w�ʂɌ����Ă���ꍇ�̃X�e���V�����Z
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//�X�e���V���e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;	//�X�e���V���e�X�g�ɍ��i���A�[�x�e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//�X�e���V���e�X�g�Ɛ[�x�e�X�g�̗����ɍ��i�����Ƃ��A���s����X�e���V������
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//�X�e���V���f�[�^�������̃X�e���V���f�[�^�Ƃǂ���r����

																		//�[�x�X�e���V����Ԃ��쐬
	result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depth_stencil_state);
	if (FAILED(result))
		return false;

	//�[�x�̃X�e���V����Ԃ�ݒ�
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
	//---------------------------------------------------------------------------------------------------

	//-----�[�x�X�e���V���r���[�̍쐬�A�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���o�b�t�@���o�̓����_�����O�p�C�v���C���Ƀo�C���h-----
	//�[�x�X�e���V���r���[��������
	ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

	//�[�x�X�e���V���r���[�̐�����ݒ�
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//���\�[�X �f�[�^�̃t�H�[�}�b�g
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//���\�[�X�̃^�C�v
	depth_stencil_view_desc.Texture2D.MipSlice = 0;								//�e�N�X�`���[���ǂݎ���p���ǂ������L�q����l

																				//�[�x�X�e���V���r���[���쐬
	result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
	if (FAILED(result))
		return false;

	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���o�b�t�@���o�̓����_�����O�p�C�v���C���Ƀo�C���h����
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
	//--------------------------------------------------------------------------------------------------------------------------------
	//----------------------���X�^���C�U�̏�Ԃ��쐬----------------------
	//�ǂ̂悤�Ƀ|���S����`�悷�邩�����肷�郉�X�^�L�q��ݒ�
	raster_desc.AntialiasedLineEnable = false;			//���̃A�����G�C���A�V���O�̗L���B���̕`�撆��MultisampleEnable��false�̏ꍇ�ɂ̂ݓK�p�����
	raster_desc.CullMode = D3D11_CULL_BACK;	//����̕����������Ă���O�p�`�̕`��̗L��
	raster_desc.DepthBias = 0;				//�w��̃s�N�Z���ɉ��Z����[�x�l
	raster_desc.DepthBiasClamp = 0.0f;				//�s�N�Z���̍ő�[�x�o�C�A�X
	raster_desc.DepthClipEnable = true;				//�����Ɋ�Â��ăN���b�s���O�̗L��
	raster_desc.FillMode = D3D11_FILL_SOLID;	//�����_�����O���Ɏg�p����`�惂�[�h
	raster_desc.FrontCounterClockwise = false;			//�O�p�`���O��������������������
	raster_desc.MultisampleEnable = false;			//�}���`�T���v�����O�̃A���`�G�C���A�V���O�̗L��
	raster_desc.ScissorEnable = false;			//�V�U�[�Z�`�J�����O�̗L���B�A�N�e�B�u�ȃV�U�[�Z�`�̊O���̃s�N�Z���͂��ׂăJ�����O����܂��B
	raster_desc.SlopeScaledDepthBias = 0.0f;				//�w��̃s�N�Z���̃X���[�v�ɑ΂���X�J��

															//��قǋL�������������烉�X�^���C�U�̏�Ԃ��쐬
	result = m_device->CreateRasterizerState(&raster_desc, &m_raster_state);
	if (FAILED(result))
		return false;

	//���X�^���C�U�̏�Ԃ�ݒ�
	m_device_context->RSSetState(m_raster_state);
	//--------------------------------------------------------------------

	//-----------------------�r���[�{�[�h�̍쐬---------------------------
	//�����_�����O�p�̃r���[�{�[�h��ݒ�
	viewport.Width = (float)screenWidth;	//�r���[�{�[�h�̕�
	viewport.Height = (float)screenHeight;	//�r���[�{�[�h�̍���
	viewport.MinDepth = 0.0f;					//�r���[�{�[�h�̍ŏ��[�x�B
	viewport.MaxDepth = 1.0f;					//�r���[�{�[�h�̍ő�[�x�B
	viewport.TopLeftX = 0.0f;					//�r���[�{�[�h�̍�����X�ʒu
	viewport.TopLeftY = 0.0f;					//�r���[�{�[�h�̏㕔��Y�ʒu

												//�r���[�{�[�h���쐬
	m_device_context->RSSetViewports(1, &viewport);
	//--------------------------------------------------------------------

	//---------------------------�s��̍쐬�Ȃ�---------------------------
	//���e�s���ݒ�
	float field_of_view = (float)D3DX_PI / 4.0f;					//Y�����ւ̎���
	float screen_aspect = (float)screenWidth / (float)screenHeight;	//�A�X�y�N�g��B�r���[��Ԃ̍����𕝂ŏ��Z�����l

																	//3D�����_�����O�p�̓��e�s����쐬
	D3DXMatrixPerspectiveFovLH(&m_projection_matrix, field_of_view, screen_aspect, screenNear, screenDepth);

	//���[���h�s���P�ʍs��ɏ�����
	D3DXMatrixIdentity(&m_world_matrix);

	//2D�����_�����O�p�̐��ˉe���e�s����쐬
	D3DXMatrixOrthoLH(&m_ortho_matrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	//--------------------------------------------------------------------

	//--------------------------�[�x�X�e���V�����(Z�o�b�t�@OFF)��ݒ�E�쐬����------------------------
	//�p�����[�^��ݒ肷��O�ɁA2�Ԗڂ̐[�x�̃X�e���V����Ԃ�k���A����
	ZeroMemory(&depth_disabied_stencil_desc, sizeof(depth_disabied_stencil_desc));

	//2D�����_�����O�̂��߂�Z�o�b�t�@��OFF�ɂ���2�Ԗڂ̐[�x�X�e���V����Ԃ��쐬�B
	//�B��̈Ⴂ��DepthEnable��false�ɐݒ肳��Ă���ꍇ�A���̂��ׂẴp�����[�^�͑��̐[�x�X�e���V����Ԃ𓯂�
	depth_disabied_stencil_desc.DepthEnable = false;							//�[�x�e�X�g���g�p�\�ɂ��邩�ǂ���
	depth_disabied_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//�[�x�X�e���V���o�b�t�@�̒��ŁA�[�x�f�[�^�ɂ��ύX���\�ȕ���������
	depth_disabied_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;				//�[�x�f�[�^�������̐[�x�f�[�^�Ƃǂ���r����

	depth_disabied_stencil_desc.StencilEnable = true;							//�X�e���V���e�X�g���g�p�\�ɂ��邩�ǂ���
	depth_disabied_stencil_desc.StencilReadMask = 0xFF;							//�[�x�X�e���V���o�b�t�@�̒��ŁA�X�e���V���f�[�^��ǂݎ�镔��������
	depth_disabied_stencil_desc.StencilWriteMask = 0xFF;						//�[�x�X�e���V���o�b�t�@�̒��ŁA�X�e���V���f�[�^���������ޕ���������

																				//�s�N�Z�����O�ʂɌ����Ă���ꍇ�̃X�e���V������
	depth_disabied_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		//�X�e���V���e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_disabied_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;	//�X�e���V���e�X�g�ɍ��i���A�[�x�e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_disabied_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		//�X�e���V���e�X�g�Ɛ[�x�e�X�g�̗����ɍ��i�����Ƃ��A���s����X�e���V������
	depth_disabied_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		//�X�e���V���f�[�^�������̃X�e���V���f�[�^�Ƃǂ���r����

																						//�s�N�Z�����w�ʂɌ����Ă���ꍇ�̃X�e���V�����Z
	depth_disabied_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			//�X�e���V���e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_disabied_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;	//�X�e���V���e�X�g�ɍ��i���A�[�x�e�X�g�ŕs���i�ɂȂ����Ƃ��Ɏ��s����X�e���V������
	depth_disabied_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;			//�X�e���V���e�X�g�Ɛ[�x�e�X�g�̗����ɍ��i�����Ƃ��A���s����X�e���V������
	depth_disabied_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;			//�X�e���V���f�[�^�������̃X�e���V���f�[�^�Ƃǂ���r����

																						//�f�o�C�X���g�p���ď�Ԃ��쐬
	result = m_device->CreateDepthStencilState(&depth_disabied_stencil_desc, &m_depth_disabled_stencil_state);
	if (FAILED(result))
		return false;
	//--------------------------------------------------------------------------------------------------

	//-------------�u�����h���(�A���t�@�u�����f�B���OON)��ݒ�E�쐬-------------
	//�u�����h�X�e�[�g�̐������N���A����
	ZeroMemory(&blend_state_description, sizeof(D3D11_BLEND_DESC));

	//�A���t�@�Ή��u�����h�X�e�[�g�̋L�q���쐬
	blend_state_description.RenderTarget[0].BlendEnable = TRUE;						//�u�����f�B���O��L��
	blend_state_description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;			//�u�����f�B���O�I�v�V����(�ŏ���RGB�f�[�^�\�[�X���w��A�u�����f�B���O�O�̏���)
	blend_state_description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//�u�����f�B���O�I�v�V����(�Q�Ԗڂ�RGB�f�[�^�\�[�X���w��A�u�����f�B���O�O�̏���)
	blend_state_description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;		//�u�����f�B���O����(RGB�f�[�^�\�[�X�̑g�ݍ������@���`)
	blend_state_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//�u�����f�B���O�I�v�V����(�ŏ��̃A���t�@�f�[�^�\�[�X���w��A�u�����f�B���O�O�̏���)
	blend_state_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//�u�����f�B���O�I�v�V����(�Q�Ԗڂ̃A���t�@�f�[�^�\�[�X���w��A�u�����f�B���O�O�̏���)
	blend_state_description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		//�u�����f�B���O����(�A���t�@�f�[�^�\�[�X�̑g�ݍ��킹���@���`)
	blend_state_description.RenderTarget[0].RenderTargetWriteMask = 0x0f;						//�������݃}�X�N

																								//�L�q���g�p���ău�����h��Ԃ��쐬
	result = m_device->CreateBlendState(&blend_state_description, &m_alpha_disable_blending_state);
	if (FAILED(result))
		return false;
	//----------------------------------------------------------------------------
	//------------�u�����h���(�A���t�@�u�����f�B���OOFF)��ݒ�E�쐬------------
	//�A���t�@�s�g�p�u�����h��Ԃ��L�q���쐬���邽�߂ɋL�q��ύX
	blend_state_description.RenderTarget[0].BlendEnable = FALSE;
	//�L�q���g�p���ău�����h��Ԃ��쐬
	result = m_device->CreateBlendState(&blend_state_description, &m_alpha_enable_blending_state);
	if (FAILED(result))
		return false;
	//---------------------------------------------------------------------------
	return true;
}

//�V���b�g�_�E���֐�
void D3DClass::Shutdown()
{
	//�t���X�N���[�����[�h�ŃX���b�v�`�F�[��������ł��Ȃ����Ƃ�����̂�
	//�E�B���h�E���[�h�ɂ��ĉ�����s��
	if (m_swap_chain)
	{
		m_swap_chain->SetFullscreenState(false, NULL);
		m_swap_chain->Release();
		m_swap_chain = 0;
	}

	if (m_alpha_enable_blending_state)
	{
		m_alpha_enable_blending_state->Release();
		m_alpha_enable_blending_state = 0;
	}

	if (m_alpha_disable_blending_state)
	{
		m_alpha_disable_blending_state->Release();
		m_alpha_disable_blending_state = 0;
	}

	if (m_raster_state)
	{
		m_raster_state->Release();
		m_raster_state = 0;
	}

	if (m_depth_stencil_view)
	{
		m_depth_stencil_view->Release();
		m_depth_stencil_view = 0;
	}

	if (m_depth_stencil_state)
	{
		m_depth_stencil_state->Release();
		m_depth_stencil_state = 0;
	}

	if (m_depth_disabled_stencil_state)
	{
		m_depth_disabled_stencil_state->Release();
		m_depth_disabled_stencil_state = 0;
	}

	if (m_depth_stencil_buffer)
	{
		m_depth_stencil_buffer->Release();
		m_depth_stencil_buffer = 0;
	}

	if (m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = 0;
	}

	if (m_device_context)
	{
		m_device_context->Release();
		m_device_context = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	return;
}
/*�e�t���[���̎n�߂ɐV����3D�V�[����`�悷���тɌĂяo�����֐�
����1 float red	:�o�b�t�@���N���A����Ƃ��̐F���@R
����2 float green	:�o�b�t�@���N���A����Ƃ��̐F���@G
����3 float bluea	:�o�b�t�@���N���A����Ƃ��̐F���@B
����4 float alpha	:�o�b�t�@���N���A����Ƃ��̐F���@A*/
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	//�o�b�t�@���N���A����F���RGBA
	float color[4];

	//�o�b�t�@���N���A����F��ݒ�
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//�o�b�N�o�b�t�@���N���A
	m_device_context->ClearRenderTargetView(m_render_target_view, color);

	//�[�x�o�b�t�@���N���A
	m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

//�e�t���[���̍Ō�ɂ��ׂĂ̕`�悪����������3D�V�[����\������悤�ɃX���b�v�`�F�[���Ɏw������֐�
void D3DClass::EndScene()
{
	//�����_�����O�����������̂ŁA�o�b�N�o�b�t�@����ʂɕ\������
	if (m_vsync_enabled)
	{
		//��ʂ̃��t���b�V�����[�g�Ƀ��b�N
		m_swap_chain->Present(1, 0);
	}
	else
	{
		//�ł��邾�������f������
		m_swap_chain->Present(0, 0);
	}
	return;
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}
ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_device_context;
}
void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectonMatrix)
{
	projectonMatrix = m_projection_matrix;
	return;
}
void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_world_matrix;
	return;
}
void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMateix)
{
	orthoMateix = m_ortho_matrix;
	return;
}
void D3DClass::TurnZBufferOn()
{
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
	return;
}
void D3DClass::TurnZBufferOff()
{
	m_device_context->OMSetDepthStencilState(m_depth_disabled_stencil_state, 1);
	return;
}

void D3DClass::TurnOnAlphaBlending()
{
	float blend_factor[4];

	//�u�����h�W����ݒ�
	blend_factor[0] = 0.0f;
	blend_factor[1] = 0.0f;
	blend_factor[2] = 0.0f;
	blend_factor[3] = 0.0f;

	//�A���t�@�u�����f�B���O���I���ɂ���
	m_device_context->OMSetBlendState(m_alpha_enable_blending_state, blend_factor, 0xffffffff);

	return;
}

void D3DClass::TurnOffAlphaBlending()
{
	float blend_factor[4];

	//�u�����h�W����ݒ�
	blend_factor[0] = 0.0f;
	blend_factor[1] = 0.0f;
	blend_factor[2] = 0.0f;
	blend_factor[3] = 0.0f;

	//�A���t�@�u�����f�B���O�I�t�ɂ���
	m_device_context->OMSetBlendState(m_alpha_disable_blending_state, blend_factor, 0xffffffff);

	return;
}

//�[�x�X�e���V���r���[�ɃA�N�Z�X�p
ID3D11DepthStencilView* D3DClass::GetDepthStencilView()
{
	return m_depth_stencil_view;
}
//�o�b�N�o�b�t�@�����_�[�^�[�Q�b�g���A�N�e�B�u�ɂ���p
void D3DClass::SetBackBufferRenderTarget()
{
	//�����_�����O�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���o�b�t�@���o�̓����_�����O�p�C�v���C���Ƀo�C���h
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

}