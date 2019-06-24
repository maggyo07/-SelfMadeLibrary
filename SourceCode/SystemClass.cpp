//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "SystemClass.h"

#include "AccessController.h"

using namespace System;

SystemClass::SystemClass()	:m_graphics(nullptr), m_move_camera(nullptr)
							,m_input(&Input()), m_audio(&Audio()),m_time(&Time())
							,m_FPS(&FPS()),m_CPU(&CPU())
{
}

SystemClass::SystemClass(const SystemClass &other)
{

}

SystemClass::~SystemClass()
{

}

//�A�v���P�[�V�����̂��ׂĂ̐ݒ���s���֐�
//�߂�l bool	:���́A�O���t�B�b�N�X�I�u�W�F�N�g�̍쐬�y�я������̐�����true�@���s��false
bool SystemClass::Initialize()
{
	bool result;	//���ʏ��i�[�p
	
	//��ʂ̑傫��
	//�ϐ����֐��ɑ���O�ɁA��ʂ̕��ƍ������[���ɏ���������
	int screenWidth = 0, screenHeight = 0;

	//Windows API������������
	InitializeWindows(screenWidth, screenHeight);

	//���̓I�u�W�F�N�g�����������܂�
	result = m_input->Initialize(m_hinstance,m_hwnd,screenWidth,screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"���̓I�u�W�F�N�g���������ł��܂���ł����B", L"Error", MB_OK);
		return false;
	}

	//�O���t�B�b�N�X�I�u�W�F�N�g���쐬���܂��B���̃I�u�W�F�N�g�́A���̃A�v���P�[�V�����̂��ׂẴO���t�B�b�N�̃����_�����O���������܂��B
	m_graphics = new GraphicsClass;
	//�쐬�Ɏ��s������false��Ԃ�
	if (!m_graphics)
		return false;

	//�O���t�B�b�N�I�u�W�F�N�g�����������܂��B
	result = m_graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	//�������Ɏ��s������false��Ԃ�
	if (!result)
		return false;

	//�J��������p�I�u�W�F�N�g���쐬
	m_move_camera = new MoveCameraClass();
	if (!m_move_camera)
		return false;

	//Audio��������
	m_audio->Initialize();

	//FPS��������
	m_FPS->Initialize();

	//CPU��������
	m_CPU->Initialize();

	//�^�C�}�[��������
	m_time->Initialize();

	//���ׂĂ�����������true��Ԃ�
	return true;
}

//�V���b�g�_�E���֐�
//���́A�O���t�B�b�N�X�I�u�W�F�N�g�����
//�n���h�����N���[���A�b�v����
void SystemClass::Shutdown()
{
	//�O���t�B�b�N�I�u�W�F�N�g���������
	if (m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = nullptr;
	}

	//�J��������p�I�u�W�F�N�g���
	if (m_move_camera)
	{
		delete m_move_camera;
		m_move_camera = nullptr;
	}

	//���̓I�u�W�F�N�g�����
	m_input->Shutdown();

	//Audio�����
	m_audio->Delete();
	//FPS�����
	m_FPS->Delete();
	//CPU�����
	m_CPU->Delete();
	//�^�C�}�[�����
	m_time->Delete();

	//�E�B���h�E���V���b�g�_�E������
	ShutdownWindows();

	//�V���O���g���폜
	SingletonFinalizer::Finalize();
	return;
}

//�A�v���P�[�V���������[�v���ďI������܂ł̂��ׂĂ̏������s��
void SystemClass::Run()
{
	/*���b�Z�[�W�\��
	�����o�[1 hwnd		:�E�B���h�E�n���h��
	�����o�[2 lParam	:lParam���b�Z�[�W�̒l
	�����o�[3 message	:���b�Z�[�WID
	�����o�[4 pt.x		:�J�[�\���ʒu�� x ���W�B
	�����o�[5 pt.y		:�J�[�\���ʒu�� y ���W�B
	�����o�[6 time		:���b�Z�[�W���|�X�g���ꂽ�����B
	�����o�[7 wParam	:wParam���b�Z�[�W�̒l
	*/
	MSG msg;

	//���b�Z�[�W�\����������
	ZeroMemory(&msg, sizeof(MSG));
	
	//�E�B���h�E�܂��̓��[�U�[����̏I�����b�Z�[�W������܂Ń��[�v����
	bool done = false;
	while (!done)
	{
		//�E�B���h�E���b�Z�[�W���������܂�
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
			DispatchMessage(&msg);	//�擾�������b�Z�[�W���f�B�X�p�b�`�i���o�j����
		}

		//�E�B���h�E���A�v���P�[�V�������I������悤�Ɏw��������A�I������
		if (msg.message == WM_QUIT)
			done = true;
		else
		{
			//����ȊO�̏ꍇ�̓t���[���������s��
			if (!Frame())
				done = true;
		}

		//���[�U�[���G�X�P�[�v�������ďI���������ǂ������m�F
		if (m_input->GetKey(DIK_ESCAPE))
			done = true;
		if (m_input->GetJoypadButton(0))
			done = true;
	}

	return;
}

//�A�v���P�[�V�����̂��ׂĂ̏������s���֐�
bool SystemClass::Frame()
{
	//�V�X�e�����v���X�V
	m_time->Frame();
	m_FPS->Frame();
	m_CPU->Frame();
	//���̓t���[���������s��
	if (!m_input->Frame())
		return false;

	//���݂̃J������]�x(Y��)
	float rotationY;
	//�X�V
	m_move_camera->Frame();
	m_move_camera->GetRotation(rotationY);

	//�O���t�B�b�N�X�I�u�W�F�N�g�̃t���[���������s��
	if (!m_graphics->Frame(rotationY))
		return false;

	//�Ō�ɃO���t�B�b�N���X�N���[���Ƀ����_�����O
	if (!m_graphics->Render())
		return false;

	return true;
}
//Windows�V�X�e�����b�Z�[�W�𑗐M����֐�
//�K�v�ȏ�񂾂���m��
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd,umsg,wparam,lparam);
}

//�E�B���h�E���쐬����֐�
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	//�E�B���h�E�N���X�̃p�����^
	WNDCLASSEX wc;
	//�v�����^�h���C�o
	DEVMODE dmScreenSettings;
	//�E�B���h�E�̈ʒu�@
	int posx, posy;

	//���̃I�u�W�F�N�g�ւ̊O���|�C���^���擾
	ApplicationHandle = this;

	//���̃A�v���P�[�V�����̃C���X�^���X���擾
	m_hinstance = GetModuleHandle(NULL);

	//�A�v���P�[�V�����̖��O��t����
	m_application_name = L"�G���W��";
	
	//�f�t�H���g�̐ݒ��Windows�N���X��ݒ�
	//http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-3wnddetail.html#back
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			//�E�B���h�E�X�^�C��
	wc.lpfnWndProc = WndProc;								//�R�[���o�b�N�֐��ւ̃|�C���^
	wc.cbClsExtra = 0;										//�E�B���h�E�q�Í\���̂̌��Ɋ��蓖�Ă�⑫�o�C�g���B���ʂO
	wc.cbWndExtra = 0;										//�E�B���h�E�E�B�X�^���X�̌��Ɋ��蓖�Ă�⑫�o�C�g���B���ʂO
	wc.hInstance = m_hinstance;								//���̃N���X�̂��߂̃E�B���h�E�v���V�[�W��������C���X�^���X�n���h��
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);					//�A�C�R���̃n���h��
	wc.hIconSm = wc.hIcon;									//16x16�̏������T�C�Y�̃A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//�}�E�X�J�[�\���̃n���h��
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//�E�B���h�E�w�i�F(�w�i�`��p�u���V�̃n���h��)
	wc.lpszMenuName = NULL;									//�f�t�H���g���j���[��
	wc.lpszClassName = m_application_name;					//���̃E�B���h�E�N���X�ɂ��閼�O
	wc.cbSize = sizeof(WNDCLASSEX);							//�\���̂̃T�C�Y

	//�E�B���h�E�N���X��o�^����
	RegisterClassEx(&wc);

	//�N���C�A���g�̃f�X�N�g�b�v��ʂ̉𑜓x�����肷��
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//�t���X�N���[���ŕ\������Ă���̂��A�E�B���h�E�ŕ\������Ă���̂��ɉ����āA��ʂ̐ݒ���s��
	if (FULL_SCREEN)
	{
		//�t���X�N���[���̏ꍇ�A���[�U�[�̃f�X�N�g�b�v��32�r�b�g�̍ő�T�C�Y�ɉ�ʂ�ݒ肷��
		//http://wisdom.sakura.ne.jp/system/winapi/win32/win154.html
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);							//���̍\���̂̃o�C�g�T�C�Y
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;					//���f�o�C�X�̕\�ʂ̕�
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;				//���f�o�C�X�̕\�ʂ̍���
		dmScreenSettings.dmBitsPerPel = 32;											//�f�B�X�v���C�f�o�C�X�̃J���[�r�b�g��
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;	//���̍\���̂̂ǂ̃����o���g������\���t���O
		//�\���ݒ���t���X�N���[���ɕύX����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		
		//�E�B���h�E�̈ʒu��������ɐݒ肷��
		posx = posy = 0;
	}
	else
	{
		//�E�B���h�E�̏ꍇ��800x600�𑜓x�ɐݒ肷��
		screenWidth = 800;
		screenHeight = 600;

		//�E�B���h�E����ʒ����ɔz�u����
		posx = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	
	//��ʐݒ�ŃE�B���h�E���쐬���A�n���h�����擾����
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_application_name, m_application_name,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posx, posy, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	
	//�E�B���h�E����ʏ�Ɏ����グ�A���C���t�H�[�J�X�Ƃ��Đݒ肷��
	ShowWindow(m_hwnd, SW_SHOW);//�\����Ԃ�ݒ�
	SetForegroundWindow(m_hwnd);//�A�N�e�B�u�ɂ��A�t�H�A�O���E���h�ɂ���
	SetFocus(m_hwnd);			//�L�[�{�[�h�t�H�[�J�X��ݒ�

	//�}�E�X�J�[�\�����\���ɂ���
	ShowCursor(false);

	return;
}

//��ʂ̐ݒ��ʏ�ɖ߂��A����Ɋ֘A�t����ꂽ�E�B���h�E�ƃn���h�����������֐�
void SystemClass::ShutdownWindows()
{
	//�}�E�X�J�[�\����\������
	ShowCursor(true);

	//�t���X�N���[�����[�h���I������ꍇ�́A�\���ݒ���C�����܂�
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	//�E�B���h�E����������
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//�A�v���P�[�V�����C���X�^���X����������
	UnregisterClass(m_application_name, m_hinstance);
	m_hinstance = NULL;

	//���̃N���X�ւ̃|�C���^���������
	ApplicationHandle = NULL;

	return;
}
//�R�[���o�b�N�֐�
LRESULT CALLBACK WndProc(HWND hwnd,UINT umessage,WPARAM wparam,LPARAM lparam)
{
	switch (umessage)
	{
		//�E�B���h�E���j������Ă��邩�ǂ������`�F�b�N
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		//�E�B���h�E�������Ă��邩�ǂ������m�F
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		//���̂��ׂẴ��b�Z�[�W�́A�V�X�e���N���X�̃��b�Z�[�W�n���h���ɓn�����
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}