#pragma once

//�v���v���Z�b�T�̎w��
//�w�b�_�[�Ŏg�p���Ă���_�C���N�g�C���v�b�g�̃o�[�W�������`
//�������Ȃ��ƁA�R���p�C���̓f�t�H���g�Ńo�[�W����8�ɂȂ��Ă�����f�ȃ��b�Z�[�W���쐬����
#define DIRECTINPUT_VERSION 0x0800

//�����N
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <dinput.h>
#include <list>
#include <memory>

constexpr auto KEY_MAX					{ 256 };	//�L�[�{�[�h�̃L�[�̍ő吔
constexpr auto MOUSE_BUTON_MAX			{ 4 };	//�}�E�X�̃{�^���̍ő吔
constexpr auto JOYPAD_BUTTON_MAX		{ 32 };	//�W���C�p�b�h�̍ő�{�^����(32�ȉ��łȂ��ƃG���[)
constexpr auto SAME_TERMINAL_BUTTON_MAX	{ 5 };	//�����[���̃{�^���o�^�ő吔
constexpr auto INPUT_NOT_ID				{ 255 };	//ID���ݒ肳��Ă��Ȃ����

constexpr auto INIT_MOUSE_POS_X			{0};
constexpr auto INIT_MOUSE_POS_Y			{0};

using namespace std;

//�O���錾
class InputClass;

//���͏��
enum PushInfo
{
	DownPressing = 1,	//�L�[�������Ă���(�A�ˏ��)
	DownMoment = 1 << 1,	//�L�[�������Ă��Ȃ���Ԃ��牟������
	UpMoment = 1 << 2,	//�L�[�������Ă����Ԃ��痣������
};

//�{�^��ID
namespace ButtonID
{
	enum
	{
		NOT,
		LEFT,		//���{�^��
		RIGHT,		//�E�{�^��
		UP,			//��{�^��
		DOWN,		//���{�^��
		ATTACK,		//�U���{�^��
		ITEM_USE,	//�A�C�e���g�p�{�^��
	};
}

//�{�^���o�^�p(�����̃{�^������̃{�^���̂悤�ɂ���)
struct InputButton
{
	friend class InputClass;
public:
	InputButton(int button_id);

	//�{�^���������Ă���Ԃ͏��(�A�����)
	bool GetButton(int joypad_num = 0);
	//�{�^���������Ă��Ȃ���Ԃ��牟�����Ƃ�
	bool GetButtonDown(int joypad_num = 0);
	//�{�^���������Ă����Ԃ��痣�����Ƃ�
	bool GetButtonUp(int joypad_num = 0);

	//�L�[�{�[�hID��ݒ肷��(�߂�l�@������true ���s��false)	
	bool SetKeyID(BYTE key_id);
	//�}�E�X�{�^��ID��ݒ肷��(�߂�l�@������true ���s��false)
	bool SetMouseButtonID(BYTE mouse_button_id);
	//�W���C�p�b�h�{�^��ID��ݒ肷��(�߂�l�@������true ���s��false)
	bool SetJoypadButtonID(BYTE jpypad_button_id);
private:
	BYTE m_key_ids[SAME_TERMINAL_BUTTON_MAX];			//�L�[�{�[�h��ID
	BYTE m_mouse_button_ids[SAME_TERMINAL_BUTTON_MAX];	//�}�E�X��ID
	BYTE m_joypad_button_ids[SAME_TERMINAL_BUTTON_MAX];	//�W���C�p�b�h��ID
	int m_button_id;			//�o�^�ԍ�
};

//���̓N���X
class InputClass
{
	struct Joypad
	{
		Joypad();
		//������DIJOYSTATE�����ɍX�V����
		void UpData(const DIJOYSTATE& joypad_stati);

		BYTE buttons[JOYPAD_BUTTON_MAX];	//�{�^���̔z��
		int lX;	//�ʏ�̓X�e�B�b�N�̍��E�̓�����\�� x ���B
		int lY;	//�ʏ�̓X�e�B�b�N�̑O��̓�����\�� y ���B
		int lZ;	//z ���B�X���b�g�������\���ꍇ�������B�W���C�X�e�B�b�N�ɂ��̎����Ȃ��ꍇ�A�l�̓[���ł���B
		int lRx;//x ����]�B�W���C�X�e�B�b�N�ɂ��̎����Ȃ��ꍇ�A�l�̓[���ł���B
		int lRy;//y ����]�B�W���C�X�e�B�b�N�ɂ��̎����Ȃ��ꍇ�A�l�̓[���ł���B
		int lRz;//z ����] (�����ǂƌĂ΂��)�B�W���C�X�e�B�b�N�ɂ��̎����Ȃ��ꍇ�A�l�̓[���ł���B
		int rgl_slider[2];//2 �̒ǉ��� (�ȑO�ɂ� u ���� v ���ƌĂ΂�Ă���)�B���̒l�̈Ӗ��̓W���C�X�e�B�b�N�ňقȂ�B
	};
public:
	InputClass();
	//�������֐�
	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	//�V���b�g�_�E���֐�
	void Shutdown();
	//�t���[���֐�(���t���[���Ăяo�����)
	bool Frame();

	//�L�[�������Ă���Ԃ͏��(�A�ˏ��)
	//���� BYTE key	:DirectInput �L�[���ʃR�[�h
	bool GetKey(BYTE key);
	//�L�[�������Ă��Ȃ���Ԃ��牟������
	//���� BYTE key	:DirectInput �L�[���ʃR�[�h
	bool GetKeyDown(BYTE key);
	//�L�[�������Ă����Ԃ��痣������
	//���� BYTE key	:DirectInput �L�[���ʃR�[�h
	bool GetKeyUp(BYTE key);

	//�}�E�X�������Ă���Ԃ͏��(�A�ˏ��)
	//���� BYTE botton:0=���{�^�� 1=�E�{�^�� 2=�����{�^��
	bool GetMouseButton(BYTE button);
	//�}�E�X�������Ă��Ȃ���Ԃ��牟������
	//���� BYTE botton:0=���{�^�� 1=�E�{�^�� 2=�����{�^��
	bool GetMouseButtonDown(BYTE button);
	//�}�E�X�������Ă����Ԃ��痣������
	//���� BYTE botton:0=���{�^�� 1=�E�{�^�� 2=�����{�^��
	bool GetMouseButtonUp(BYTE button);

	//�W���C�p�b�h�������Ă���Ԃ����(�A�ˏ��)
	bool GetJoypadButton(BYTE button, int joypad_num = 0);
	//�W���C�p�b�h�������Ă��Ȃ���Ԃ��牟�����Ƃ�
	bool GetJoypadButtonDown(BYTE button, int joypad_num = 0);
	//�W���C�p�b�h�������Ă����Ԃ��痣������
	bool GetJoypadButtonUp(BYTE button, int joypad_num = 0);

	//�{�^���̓o�^�݂̂��s��(�����̃{�^������̃{�^���̂悤�Ɉ���)
	void SetButton(BYTE button_id);
	//�o�^�ς݂̃{�^���ɃL�[�{�[�h��ID��o�^����(�߂�l�@������true ���s��false)
	bool SetButtonKeyID(BYTE button_id, BYTE key_id1, BYTE key_id2 = INPUT_NOT_ID, BYTE key_id3 = INPUT_NOT_ID, BYTE key_id4 = INPUT_NOT_ID, BYTE key_id5 = INPUT_NOT_ID);
	//�o�^�ς݂̃{�^���Ƀ}�E�X�{�^��ID��o�^����(�߂�l�@������true ���s��false)
	bool SetButtonMouseButtonID(BYTE button_id, BYTE mouse_button_id1, BYTE mouse_button_id2 = INPUT_NOT_ID, BYTE mouse_button_id3 = INPUT_NOT_ID, BYTE mouse_button_id4 = INPUT_NOT_ID, BYTE mouse_button_id5 = INPUT_NOT_ID);
	//�o�^�ς݂̃{�^���ɃW���C�p�b�h�{�^��ID��o�^����(�߂�l�@������true ���s��false)
	bool SetButtonJoypadButtonID(BYTE button_id, BYTE joypad_button_di1, BYTE joypad_button_di2 = INPUT_NOT_ID, BYTE joypad_button_di3 = INPUT_NOT_ID, BYTE joypad_button_di4 = INPUT_NOT_ID, BYTE joypad_button_di5 = INPUT_NOT_ID);

	//�{�^���������Ă���Ԃ͏��(�A�����)
	bool GetButton(BYTE button_id, int joypad_num = 0);
	//�{�^���������Ă��Ȃ���Ԃ��牟�����Ƃ�
	bool GetButtonDown(BYTE button_id, int joypad_num = 0);
	//�{�^���������Ă����Ԃ��痣�����Ƃ�
	bool GetButtonUp(BYTE button_id, int joypad_num = 0);

	//���݂̃}�E�X�̈ʒu�擾�p�֐�
	void GetMouseLocation(int& out_mouseX, int& out_mouseY);

	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
private:
	//�L�[�{�[�h�̏�Ԃ�ǂݍ��ފ֐�
	bool ReadKeyboard();
	//�}�E�X�̏�Ԃ�ǂݍ��ފ֐�
	bool ReadMouse();
	//�W���C�p�b�h�̏�Ԃ�ǂݍ��݊֐�
	bool ReadJoypad();
private:
	IDirectInput8* m_direct_input;		//���C���C���^�[�t�F�C�X
	IDirectInputDevice8* m_keyboard_dev;	//�L�[�{�[�h�̓��̓f�o�C�X
	IDirectInputDevice8* m_mouse_dev;		//�}�E�X�̓��̓f�o�C�X

	IDirectInputDevice8* m_joypad_dev;	//�W���C�p�b�h�̓��̓f�o�C�X

	BYTE m_keyboard[KEY_MAX];//�L�[�{�[�h�z��
	BYTE m_mouse_buttons[MOUSE_BUTON_MAX];	//�}�E�X�̃{�^��
	Joypad m_joypad;			//�W���C�p�b�h
	list<unique_ptr<InputButton>>* m_input_buttons;		//���̓{�^���o�^�p���X�g

	int m_screen_width, m_screen_height;
	int m_mouseX, m_mouseY;
};



