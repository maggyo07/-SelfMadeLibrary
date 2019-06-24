
//����w�b�_�C���N���[�h
#include "RenderTextureClass.h"

#include "AccessController.h"

using Graphics::D3D;

RenderTextureClass::RenderTextureClass()
{
	m_render_rarget_texture = nullptr;
	m_render_target_view = nullptr;
	m_shader_resource_view = nullptr;
}

RenderTextureClass::~RenderTextureClass()
{

}
/* �������֐�
����1 texture_width	:�����_�����O���e�N�X�`���ɂ�������
����2 texture_height:�����_�����O���e�N�X�`���ɂ���������
����ʂ������_�����O���Ă���ꍇ�́A�e�N�X�`���ɑ΂��邱�̃����_�����O��
�@�c�������ʂ̏c����Ɠ����ɂ���K�v������B�łȂ��ƁA�T�C�Y�̂䂪�݂���������
 �����F
 ���̊֐��́A�ŏ��Ƀe�N�X�`���̐�����ݒ肵�Ă��炻�̃e�N�X�`�����쐬���邱�Ƃɂ���āA
 �����_�[�^�[�Q�b�g�e�N�X�`�����쐬���܂��B
 ���ɁA���̃e�N�X�`�����g�p���ă����_�[�^�[�Q�b�g�r���[��ݒ肵�A�e�N�X�`���������_�[�^�[�Q�b�g�Ƃ��ĕ`��ł���悤�ɂ��܂��B
 �Ō�ɍs�����Ƃ́A�����_�����O���ꂽ�f�[�^���Ăяo�����̃I�u�W�F�N�g�ɒ񋟂ł���悤�ɁA
 ���̃e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬���邱�Ƃł��B
 */
bool RenderTextureClass::Initialize(const int& texture_width, const int& texture_height)
{
	D3D11_TEXTURE2D_DESC texture_desc;	//�����_�����O�^�[�Q�b�g�e�N�X�`���̐���
	HRESULT result;						//�֐��̌��ʊm�F�p
	D3D11_RENDER_TARGET_VIEW_DESC  render_target_view_desc;		//�����_�����O�^�[�Q�b�g�r���[�̐���
	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;	//�V�F�[�_�[���\�[�X�r���[�̐���

	//�����_�����O�^�[�Q�b�g�e�N�X�`���̐�����������
	ZeroMemory(&texture_desc, sizeof(texture_desc));

	//�����_�����O�^�[�Q�b�g�e�N�X�`���̐�����ݒ�
	texture_desc.Width = texture_width;		//�e�N�X�`���̕�
	texture_desc.Height = texture_height;		//�e�N�X�`���̍���
	texture_desc.MipLevels = 1;					//�~�b�v�}�b�v���x���̍ő吔
	texture_desc.ArraySize = 1;					//�e�N�X�`���z����̃e�N�X�`����
	texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//�e�N�X�`���t�H�[�}�b�g
	texture_desc.SampleDesc.Count = 1;					//�s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
	texture_desc.Usage = D3D11_USAGE_DEFAULT;	//�e�N�X�`���̓ǂݏ������ݕ��@�����ʂ���l
	texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;//�p�C�v���C���X�e�[�W�ւ̃o�C���h�Ɋւ���t���O
	texture_desc.CPUAccessFlags = 0;					//������CPU�A�N�Z�X�̎�ނ��w�肷��t���O
	texture_desc.MiscFlags = 0;					//���̈�ʐ��̒Ⴂ���\�[�X�I�v�V���������ʂ���t���O

	//�����_�����O�^�[�Q�b�g�e�N�X�`�����쐬
	result = D3D().GetDevice()->CreateTexture2D(&texture_desc, NULL, &m_render_rarget_texture);
	if (FAILED(result))
		return false;


	//�����_�����O�^�[�Q�b�g�r���[�̐�����ݒ�
	render_target_view_desc.Format = texture_desc.Format;	//�f�[�^�`��
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;//���\�[�X�^�C�v
	render_target_view_desc.Texture2D.MipSlice = 0;			//�~�b�v �X���C�X���g�p����~�b�v�}�b�v ���x���̃C���f�b�N�X�ł��B

	//�����_�����O�^�[�Q�b�g���쐬
	result = D3D().GetDevice()->CreateRenderTargetView(m_render_rarget_texture, &render_target_view_desc, &m_render_target_view);
	if (FAILED(result))
		return false;


	//�V�F�[�_���\�[�X�r���[�̐�����ݒ�
	shader_resource_view_desc.Format = texture_desc.Format;	//�\���t�H�[�}�b�g
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;//�r���[�̃��\�[�X�^�C�v
	shader_resource_view_desc.Texture2D.MostDetailedMip = 0;					//�g�p����ł��ڍׂȃ~�b�v�}�b�v ���x���̃C���f�b�N�X
	shader_resource_view_desc.Texture2D.MipLevels = 1;					//�e�N�X�`���[���̃~�b�v�}�b�v ���x���̍ő吔

	//�V�F�[�_���\�[�X�r���[���쐬
	result = D3D().GetDevice()->CreateShaderResourceView(m_render_rarget_texture, &shader_resource_view_desc, &m_shader_resource_view);
	if (FAILED(result))
		return false;

	return true;
}

void RenderTextureClass::Shutdown()
{
	if (m_shader_resource_view)
	{
		m_shader_resource_view->Release();
		m_shader_resource_view = nullptr;
	}

	if (m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = nullptr;
	}

	if (m_render_rarget_texture)
	{
		m_render_rarget_texture->Release();
		m_render_rarget_texture = nullptr;
	}

}

/*
SetRenderTarget�֐��́A���̃N���X�̃����_�����O�^�[�Q�b�g�r���[���A
�����_�����O��̂��ׂẴO���t�B�b�N�̌��݂̃����_�����O�ʒu�Ƃ��Đݒ肵�܂��B
*/
void RenderTextureClass::SetRenderTarget()
{
	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���o�b�t�@���o�̓����_�[�p�C�v���C���Ƀo�C���h
	D3D().GetDeviceContext()->OMSetRenderTargets(1, &m_render_target_view, D3D().GetDepthStencilView());

}
/*
ClearRenderTarget�́A���̃N���X���̃����_�[�^�[�Q�b�g�r���[�ɑ΂��ċ@�\���邱�Ƃ������āA
D3DClass :: BeginScene�֐��̋@�\��͕킵�Ă��܂��B
���̃����_�[�^�[�Q�b�g�r���[�Ƀ����_�����O����O�ɁA
������e�t���[���ƌĂт܂��B
*/
void RenderTextureClass::ClearRenderTarget(const D3DXCOLOR& color)
{
	//�o�b�N�o�b�t�@���N���A
	D3D().GetDeviceContext()->ClearRenderTargetView(m_render_target_view, (float*)&color);

	//Z�o�b�t�@���N���A
	D3D().GetDeviceContext()->ClearDepthStencilView(D3D().GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

}

/*
GetShaderResourceView�֐��́A�����_�����O���V�F�[�_���\�[�X�r���[�Ƃ��ăe�N�X�`���f�[�^�ɕԂ��܂��B
���̕��@�ł́A�����_�[�^�[�Q�b�g�r���[�Ƀ����_�����O���ꂽ���̂͂��ׂāA
���̊֐����Ăяo�����܂��܂ȃV�F�[�_�Ńe�N�X�`���Ƃ��Ďg�p�ł��܂��B
�ʏ�A�e�N�X�`�����V�F�[�_�ɑ��M����ꏊ�ł́A����ɂ��̊֐��ւ̌Ăяo�������ɑ��M���邱�Ƃ��ł��A
�e�N�X�`���ւ̃����_�����O���g�p����܂��B
*/
ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return m_shader_resource_view;
}