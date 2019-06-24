////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#include "FrustumClass.h"

#include "AccessController.h"


using namespace Graphics;

GraphicsClass::GraphicsClass()	:m_camera(nullptr),m_model(nullptr),m_light(nullptr)
								,m_render_texture(nullptr),m_texture_shader(nullptr),m_map_shader(nullptr),m_frustum(nullptr)
								,m_D3D(&D3D()),m_drawTexture2D(&DrawTexture2D()),m_modelList(&ModelList())
{

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{

}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	D3DXMATRIX base_view_matrix;	//�x�[�X�r���[�s��(�e�L�X�g�`��p)
	
	//Direct3D�I�u�W�F�N�g������������
	bool result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Direct3D���������ł��܂���ł���", L"Error", MB_OK);
		return false;
	}

	//�J�����I�u�W�F�N�g���쐬
	m_camera = new CameraClass;
	if (!m_camera)
		return false;

	//�����_�[�`�F�b�N�I�u�W�F�N�g���쐬
	m_frustum = new FrustumClass;
	if (!m_frustum)
		return false;

	//�J�����̏����ʒu��ݒ�
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_camera->Render();
	m_camera->GetViewMatrix(base_view_matrix);

	//�e�L�X�g�I�u�W�F�N�g���쐬
	m_text = new TextClass;
	if (!m_text)
		return false;

	//�e�L�X�g�I�u�W�F�N�g��������
	result = m_text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, base_view_matrix);
	if (!result)
	{
		MessageBox(hwnd, L"�e�L�X�g�I�u�W�F�N�g���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//���f���I�u�W�F�N�g���쐬
	m_model = new ModelClass;
	if (!m_model)
		return false;

	//���f���I�u�W�F�N�g������������
	if (!m_model->Initialize(L"untitled.obj"))
		return false;
	//�e�N�X�`����ǂݍ���
	if (!m_model->LoadTexture(L"Image/specular&bump1.gif"))
		return false;
	if (!m_model->LoadTexture(L"Image/specular&bump2.gif"))
		return false;
	if (!m_model->LoadTexture(L"Image/specular&bump3.gif"))
		return false;

	//�V�F�[�_�I�u�W�F�N�g�쐬
	m_shader = new ShaderClass;
	if (!m_shader)
		return false;

	//�V�F�[�_�I�u�W�F�N�g������
	if (!m_shader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"�V�F�[�_�I�u�W�F�N�g���������ł��܂���ł���", L"Error", MB_OK);
		return false;
	}

	//�e�N�X�`���V�F�[�_�I�u�W�F�N�g�쐬(2D�p)
	m_texture_shader = new TextureShaderClass;
	if (!m_texture_shader)
		return false;

	//�e�N�X�`���V�F�[�_�I�u�W�F�N�g������(2D�p)
	result = m_texture_shader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"�e�N�X�`���V�F�[�_�I�u�W�F�N�g���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//�}���`�e�N�X�`���V�F�[�_�[�I�u�W�F�N�g���쐬
	m_multi_texture_shader = new MultiTextureShaderClass;
	if (!m_multi_texture_shader)
		return false;

	//�}���`�e�N�X�`���V�F�[�_�[�I�u�W�F�N�g������
	result = m_multi_texture_shader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"�}���`�e�N�X�`���V�F�[�_�[���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//�o���v�}�b�v�V�F�[�_�[���쐬
	m_bump_map_shader = new BumpMapShaderClass;
	if (!m_bump_map_shader)
		return false;

	//�o���v�}�b�v�V�F�[�_�[��������
	result = m_bump_map_shader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"�o���v�}�b�v�V�F�[�_�[���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//�}�b�s���O�V�F�[�_�[���쐬
	m_map_shader = new MapShaderClass;
	if (!m_map_shader)
		return false;

	//�}�b�s���O�V�F�[�_������
	result = m_map_shader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"�}�b�s���O�V�F�[�_���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//2D�e�N�X�`���\���p�N���X��������
	result = m_drawTexture2D->Initialize(screenWidth, screenHeight);
	if (!result)
		return false;
	//�e�X�g�p�e�N�X�`���ǂݍ���
	m_drawTexture2D->LoadTexture(0, L"dog.jpg");
	m_drawTexture2D->LoadDevTexture(1, L"Player.png", 16, 4, 4, 40, 40);

	//���C�g�I�u�W�F�N�g���쐬
	m_light = new LightClass;
	if (!m_light)
		return false;

	//���C�g�I�u�W�F�N�g������������
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetSoecularPower(16.0f);
	m_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���I�u�W�F�N�g�ւ̃����_�����O���쐬
	m_render_texture = new RenderTextureClass;
	if (!m_render_texture)
		return false;

	//�e�N�X�`���I�u�W�F�N�g��������
	result = m_render_texture->Initialize(screenWidth, screenHeight);
	if (!result)
		return false;

	//���f�����X�g�̏�����
	result = m_modelList->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"���f�����X�g���������ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}
	//�e�X�g�p���f���̍쐬
	m_modelList->TestRandomBllCreation(25);

	return true;
}


void GraphicsClass::Shutdown()
{
	//���f�����X�g�N���X�����
	m_modelList->Delete();

	//�e�N�X�`���I�u�W�F�N�g�Ƀ����_�����O�����
	if (m_render_texture)
	{
		m_render_texture->Shutdown();
		delete m_render_texture;
		m_render_texture = nullptr;
	}

	//�e�L�X�g�I�u�W�F�N�g�����
	if (m_text)
	{
		m_text->Shutdown();
		delete m_text;
		m_text = nullptr;
	}
	//���C�g�I�u�W�F�N�g�����
	if (m_light)
	{
		delete m_light;
		m_light = nullptr;
	}
	//�V�F�[�_�I�u�W�F�N�g�����
	if (m_shader)
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = nullptr;
	}
	//�e�N�X�`���V�F�[�_�I�u�W�F�N�g�����
	if (m_texture_shader)
	{
		m_texture_shader->Shutdown();
		delete m_texture_shader;
		m_texture_shader = nullptr;
	}
	//�}���`�e�N�X�`���V�F�[�_�[�I�u�W�F�N�g�����
	if (m_multi_texture_shader)
	{
		m_multi_texture_shader->Shutdown();
		delete m_multi_texture_shader;
		m_multi_texture_shader = nullptr;
	}
	//�o���v�}�b�v�V�F�[�_�[�����
	if (m_bump_map_shader)
	{
		m_bump_map_shader->Shutdown();
		delete m_bump_map_shader;
		m_bump_map_shader = nullptr;
	}
	//�}�b�s���O�V�F�[�_�����
	if (m_map_shader)
	{
		m_map_shader->Shutdown();
		delete m_map_shader;
		m_map_shader = nullptr;
	}
	//���f���I�u�W�F�N�g�����
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = nullptr;
	}

	//�J�����I�u�W�F�N�g�����
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
	
	//�����_�[�`�F�b�N�I�u�W�F�N�g���J��
	if (m_frustum)
	{
		delete m_frustum;
		m_frustum = nullptr;
	}

	//2D�`��p�N���X�����
	m_drawTexture2D->Delete();
	//Direct3D�I�u�W�F�N�g�����
	m_D3D->Shutdown();

	return;
}


bool GraphicsClass::Frame(float rotationY)
{
	//�J�����̈ʒu��ݒ�
	m_camera->SetPosition(0.0f, 0.0f, -5.0f);

	//�J�����̉�]��ݒ�
	m_camera->SetRotation(0.0f, rotationY, 0.0f);
	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX view_matrix, projection_matrix, world_matrix, ortho_matrix;
	bool result;		//���ʏ��i�[�p�ϐ�

						//�V�[���S�̂��ŏ��Ƀe�N�X�`���Ƀ����_�����O
	result = RenderToTexture();
	if (!result)
		return false;

	//�o�b�t�@���N���A���ăV�[�����J�n����
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//�o�b�N�o�b�t�@�ɑ΂��Ēʏ�ʂ�V�[���������_�����O
	result = RenderScene();
	if (!result)
		return false;

	//�J�����̈ʒu�Ɋ�Â��ăr���[�s����쐬����
	m_camera->Render();

	//�J������D3D�I�u�W�F�N�g���烏�[���h�A�r���[�A���e�s����擾
	m_camera->GetViewMatrix(view_matrix);
	m_D3D->GetWorldMatrix(world_matrix);
	m_D3D->GetProjectionMatrix(projection_matrix);
	m_D3D->GetOrthoMatrix(ortho_matrix);

	//�~�����g�ݗ��Ă�
	m_frustum->ConstructFrustum(SCREEN_DEPTH, projection_matrix, view_matrix);
	//�����_�����O���J�E���g�p
	int render_count = 0;

	//���ׂẴ��f���𒲂ׂāA�J�����r���[�ŕ\���ł���ꍇ�ɂ݂̂����������_�����O���܂��B
	for (int index = 0; index < m_modelList->GetModelCount(); index++)
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
		//���̃C���f�b�N�X�ɂ����鋅�̃��f���̈ʒu�ƐF���擾
		m_modelList->GetData(index, position, color);

		//���̔��a��1.0�ɐݒ�B
		float radius = 1.0f;

		//���̃��f����������ɂ��邩�ǂ������m�F
		if (m_frustum->ChechCube(position.x, position.y, position.z, radius))
		{
			//���f���������_�����O����ʒu�Ɉړ�
			D3DXMatrixTranslation(&world_matrix, position.x, position.y, position.z);

			//���f���̒��_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���O���t�B�b�N�p�C�v���C���ɔz�u���āA�`��̏���������
			m_model->Render(m_D3D->GetDeviceContext());

			//�V�F�[�_���g�p���ă��f���������_�����O
			/*result = m_shader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
			m_model->GetTextureArray()[0], m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor(),
			m_camera->GetPosition(), m_light->GetSpecularColor(), m_light->GetSpecularPower());*/

			//���̃��[���h�}�g���b�N�X�Ƀ��Z�b�g
			m_D3D->GetWorldMatrix(world_matrix);

			//���̃��f���̓����_�����O���ꂽ�̂ŁA���̃t���[���̐����t���X
			render_count++;

		}
	}

	//-------------------------2D�I�u�W�F�N�g�������_�����O-------------------------
	//���ׂĂ�2D�����_�����O���J�n���邽�߂�Z�o�b�t�@��OFF�ɂ���
	m_D3D->TurnZBufferOff();
	//�e�L�X�g�������_�����O����O�ɃA���t�@�u�����f�B���O��ON�ɂ���
	m_D3D->TurnOnAlphaBlending();

	//�e�L�X�g������������_�����O
	result = m_text->Render(m_D3D->GetDeviceContext(), world_matrix, ortho_matrix);
	if (!result)
		return false;

	//�ړ��e�X�g�p
	static int x = 100, y = 100;
	x += 1;
	result = m_drawTexture2D->Render(0, x, y, 255, 255, D3DXCOLOR(), 0);
	if (!result)
		return false;

	//�r�b�g�}�b�v���e�N�X�`���V�F�[�_�Ń����_�����O����
	result = m_texture_shader->Render(m_D3D->GetDeviceContext(), m_drawTexture2D->GetIndexCount(), world_matrix, view_matrix, ortho_matrix, m_drawTexture2D->GetTexture(0));
	if (!result)
		return false;

	//�e�X�g
	/*m_drawTexture2D->Render(1, 1, 1,50,50,D3DXCOLOR(),0,1);
	m_texture_shader->Render(m_D3D->GetDeviceContext(), m_drawTexture2D->GetIndexCount(), world_matrix, view_matrix, ortho_matrix, m_drawTexture2D->GetTexture(1));*/


	//�e�N�X�`�������_�����O
	result = m_drawTexture2D->Render(0, 0, 0, 100, 100, D3DXCOLOR(), 0.0f);
	if (!result)
		return false;
	result = m_texture_shader->Render(m_D3D->GetDeviceContext(), m_drawTexture2D->GetIndexCount(), world_matrix, view_matrix, ortho_matrix, m_render_texture->GetShaderResourceView());
	if (!result)
		return false;

	//�e�L�X�g�̃����_�����O��ɃA���t�@�u�����f�B���O���I�t�ɂ���
	m_D3D->TurnOffAlphaBlending();
	//���ׂĂ�2D�����_�����O�����������̂ŁAZ�o�b�t�@�����Ƃɖ߂�
	m_D3D->TurnZBufferOn();
	//------------------------------------------------------------------------------
	//-------------------------3D�I�u�W�F�N�g�������_�����O-------------------------
	//�e�X�g�p�Ő��E����----------
	//�e�t���[���̍X�V���ꂽ��]�l
	static float rotation = 0.0f;

	//�e�t���[����rotation�ϐ����X�V
	rotation += (float)D3DX_PI * 0.001f;
	if (rotation > 360.0f)
		rotation -= 360.0f;
	D3DXMatrixRotationY(&world_matrix, rotation);
	//------------------------------
	//���f���̒��_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���O���t�B�b�N�X�p�C�v���C���ɒu���A�`��̏���������
	m_model->Render(m_D3D->GetDeviceContext());

	//�V�F�[�_���g�p���ă��f���������_�����O
	/*
	result = m_shader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
	m_model->GetTextureArray()[0], m_light->GetDirection(), m_light->GetDiffuseColor(), m_light->GetAmbientColor(),
	m_camera->GetPosition(), m_light->GetSpecularColor(),m_light->GetSpecularPower());
	*/
	/*result = m_multi_texture_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
	m_model->GetTextureArray(), 2.0f);
	*/
	/*
	result = m_bump_map_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
	m_model->GetTextureArray(), m_light->GetDirection(), m_light->GetDiffuseColor());
	*/
	/*result = m_map_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
		m_model->GetTextureArray(), m_light->GetDirection(), m_light->GetDiffuseColor(), m_camera->GetPosition(),
		m_light->GetSpecularColor(), m_light->GetSpecularPower());*/
	if (!result)
		return false;

	//------------------------------------------------------------------------------
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


bool GraphicsClass::RenderToTexture()
{
	//�����_�����O�^�[�Q�b�g���e�N�X�`���ւ̃����_�����O�ɐݒ�
	m_render_texture->SetRenderTarget();

	//�e�N�X�`���ւ̃����_�����O���N���A
	m_render_texture->ClearRenderTarget(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	//�V�[���������������_�����O����ƁA�o�b�N�o�b�t�@�ł͂Ȃ��e�N�X�`���Ƀ����_�����O�����
	if (!RenderScene())
		return false;

	//�����_�[�^�[�Q�b�g�����̃o�b�N�o�b�t�@�Ƀ��Z�b�g����
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


bool GraphicsClass::RenderScene()
{
	D3DXMATRIX view_matrix, projection_matrix, world_matrix;
	bool result = true;	//����

						//�J�����̈ʒu�Ɋ�Â��ăr���[�s����쐬
	m_camera->Render();

	//�J�����I�u�W�F�N�g��D3D�I�u�W�F�N�g���烏�[���h�A�r���[�A���e�s����擾
	m_D3D->GetWorldMatrix(world_matrix);
	m_camera->GetViewMatrix(view_matrix);
	m_D3D->GetProjectionMatrix(projection_matrix);

	//�e�t���[���ŉ�]�ϐ���ύX
	static float rotati;
	rotati += (float)D3DX_PI * 0.005f;
	if (rotati > 360.0f)
		rotati -= 360.0f;

	D3DXMatrixRotationY(&world_matrix, rotati);

	//���f���̒��_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���O���t�B�b�N�X�p�C�v���C���ɐݒu�A�`��̏���
	m_model->Render(m_D3D->GetDeviceContext());

	//���C�g�V�F�[�_���g�p���ă��f���������_�����O
	result = m_map_shader->Render(m_model->GetIndexCount(), world_matrix, view_matrix,
		projection_matrix, m_model->GetTextureArray(), m_light->GetDirection(),
		m_light->GetDiffuseColor(), m_camera->GetPosition(), m_light->GetSpecularColor(), m_light->GetSpecularPower());

	if (!result)
		return false;

	return true;
}