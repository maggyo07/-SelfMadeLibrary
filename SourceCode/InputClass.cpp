//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "InputClass.h"

#include "AccessController.h"
using System::Input;

#pragma region InputButton
//�R���X�g���N�^
InputButton::InputButton(int button_id):m_button_id(button_id)
{
	//�L�[�E�}�E�X�E�W���C�p�b�h��ID�z���������
	memset(m_key_ids, INPUT_NOT_ID, sizeof(m_key_ids));
	memset(m_mouse_button_ids, INPUT_NOT_ID, sizeof(m_mouse_button_ids));
	memset(m_joypad_button_ids, INPUT_NOT_ID, sizeof(m_joypad_button_ids));
}

//�{�^���������Ă���Ԃ͏��(�A�����)
bool InputButton::GetButton(int joypad_num)
{
	//�L�[�̓��͏�Ԃ��`�F�b�N
	for each (int key_id in m_key_ids)
		if (key_id != INPUT_NOT_ID)
			if (Input().GetKey(key_id))
				return true;

	//�}�E�X�̓��͏�Ԃ��`�F�b�N
	for each (int mouse_button_id in m_mouse_button_ids)
		if (mouse_button_id != INPUT_NOT_ID)
			if (Input().GetMouseButton(mouse_button_id))
				return true;

	//�W���C�p�b�h�̓��͏�Ԃ��`�F�b�N
	for each (int joypad_button_id in m_joypad_button_ids)
		if (joypad_button_id != INPUT_NOT_ID)
			if (Input().GetJoypadButton(joypad_num, joypad_button_id))
				return true;

	return false;
}
//�{�^���������Ă��Ȃ���Ԃ��牟�����Ƃ�
bool InputButton::GetButtonDown(int joypad_num)
{
	//�L�[�̓��͏�Ԃ��`�F�b�N
	for each (int key_id in m_key_ids)
		if (key_id != INPUT_NOT_ID)
			if (Input().GetKeyDown(key_id))
				return true;

	//�}�E�X�̓��͏�Ԃ��`�F�b�N
	for each (int mouse_button_id in m_mouse_button_ids)
		if (mouse_button_id != INPUT_NOT_ID)
			if (Input().GetMouseButtonDown(mouse_button_id))
				return true;

	//�W���C�p�b�h�̓��͏�Ԃ��`�F�b�N
	for each (int joypad_button_id in m_joypad_button_ids)
		if (joypad_button_id != INPUT_NOT_ID)
			if (Input().GetJoypadButtonDown(joypad_num, joypad_button_id))
				return true;

	return false;
}
//�{�^���������Ă����Ԃ��痣�����Ƃ�
bool InputButton::GetButtonUp(int joypad_num)
{
	//�L�[�̓��͏�Ԃ��`�F�b�N
	for each (int key_id in m_key_ids)
		if (key_id != INPUT_NOT_ID)
			if (Input().GetKeyUp(key_id))
				return true;

	//�}�E�X�̓��͏�Ԃ��`�F�b�N
	for each (int mouse_button_id in m_mouse_button_ids)
		if (mouse_button_id != INPUT_NOT_ID)
			if (Input().GetMouseButtonUp(mouse_button_id))
				return true;

	//�W���C�p�b�h�̓��͏�Ԃ��`�F�b�N
	for each (int joypad_button_id in m_joypad_button_ids)
		if (joypad_button_id != INPUT_NOT_ID)
			if (Input().GetJoypadButtonUp(joypad_num, joypad_button_id))
				return true;

	return false;
}

//�L�[�{�[�hID��ݒ肷��(�߂�l�@������true ���s��false)	
bool InputButton::SetKeyID(BYTE key_id)
{
	//������ID���Ȃ���ΏI��(�ꉞINPUT_NOT_ID������O��Ȃ̂�true��Ԃ�)
	if (key_id == INPUT_NOT_ID)
		return true;
	for each (auto& id in m_key_ids)
	{
		//���łɓo�^���Ă���ID�Ȃ玸�s
		if (id == key_id)
			return false;
		//�o�^���悤�Ƃ��Ă���ꏊ���ݒ肳��Ă��Ȃ���ΐݒ肷��
		else if (id == INPUT_NOT_ID)
		{
			id = key_id;
			return true;
		}
	}
	return false;
}
//�}�E�X�{�^��ID��ݒ肷��(�߂�l�@������true ���s��false)
bool InputButton::SetMouseButtonID(BYTE mouse_button_id)
{
	//������ID���Ȃ���ΏI��(�ꉞINPUT_NOT_ID������O��Ȃ̂�true��Ԃ�)
	if (mouse_button_id == INPUT_NOT_ID)
		return true;
	for each (auto& id in m_mouse_button_ids)
	{
		//���łɓo�^���Ă���ID�Ȃ玸�s
		if (id == mouse_button_id)
			return false;
		//�o�^���悤�Ƃ��Ă���ꏊ���ݒ肳��Ă��Ȃ���ΐݒ肷��
		else if (id == INPUT_NOT_ID)
		{
			id = mouse_button_id;
			return true;
		}
	}
	return false;
}
//�W���C�p�b�h�{�^��ID��ݒ肷��(�߂�l�@������true ���s��false)
bool InputButton::SetJoypadButtonID(BYTE jpypad_button_id)
{
	//������ID���Ȃ���ΏI��(�ꉞINPUT_NOT_ID������O��Ȃ̂�true��Ԃ�)
	if (jpypad_button_id == INPUT_NOT_ID)
		return true;
	for each (auto& id in m_joypad_button_ids)
	{
		//���łɓo�^���Ă���ID�Ȃ玸�s
		if (id == jpypad_button_id)
			return false;
		//�o�^���悤�Ƃ��Ă���ꏊ���ݒ肳��Ă��Ȃ���ΐݒ肷��
		else if (id == INPUT_NOT_ID)
		{
			id = jpypad_button_id;
			return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region InputClass

#pragma region struct Joypad

	InputClass::Joypad::Joypad()
	{
		//�����o�ϐ��S�Ă��O�ŏ�����
		memset(buttons, 0x00, sizeof(JOYPAD_BUTTON_MAX));
		lX = lY = lZ = 0;
		lRz = lRy = lRz = 0;
		memset(rgl_slider, 0x00, sizeof(rgl_slider));
	}

	//������DIJOYSTATE�����ɍX�V����
	void InputClass::Joypad::UpData(const DIJOYSTATE& joypad_stati)
	{
		if (&joypad_stati == nullptr)
			return;

		//�{�^���̏����X�V
		for (int button = 0; button < JOYPAD_BUTTON_MAX; button++)
		{
			//�{�^����������Ă�����
			if (joypad_stati.rgbButtons[button] & 0x80)
			{
				//���������Ă�����Ԃ�������
				if ((buttons[button] & DownPressing) == DownPressing)
					//�{�^���������Ă��Ȃ���Ԃ��牟�������łȂ��̂Ńt���O��OFF�ɂ���
					buttons[button] &= ~DownMoment;
				//���������Ă����ԂłȂ�������
				else//�{�^���������Ă��Ȃ���Ԃ��炨�����Ƃ��Ȃ̂Ńt���O��ON�ɂ���
					buttons[button] |= DownMoment;


				//�{�^���������Ă���̂Ńt���O��ON�ɂ���
				buttons[button] |= DownPressing;
			}
			//�{�^����������Ă��Ȃ�������
			else
			{
				//���������Ă�����Ԃ�������
				if ((buttons[button] & DownPressing) == DownPressing)
					//�{�^�������������Ă�����Ԃ��痣�������Ȃ̂Ńt���O��ON�ɂ���
					buttons[button] |= UpMoment;
				else//���������Ă��Ȃ���Ԃ�������
					//�{�^�������������Ă��Ȃ���Ԃ��痣�����킯�łȂ��̂Ńt���O��OFF�ɂ���
					buttons[button] &= ~UpMoment;

				//�{�^���������Ă��Ȃ��̂Ńt���O��OFF�ɂ���
				buttons[button] &= ~DownPressing;
			}
		}

		//���n�̒l���X�V
		lX = joypad_stati.lX;
		lY = joypad_stati.lY;
		lZ = joypad_stati.lZ;
		lRx = joypad_stati.lRx;
		lRy = joypad_stati.lRy;
		lRz = joypad_stati.lRz;

		rgl_slider[0] = joypad_stati.rglSlider[0];
		rgl_slider[1] = joypad_stati.rglSlider[1];
	}

#pragma endregion
InputClass::InputClass():m_input_buttons(new list<unique_ptr<InputButton>>)
						,m_direct_input(nullptr),m_keyboard_dev(nullptr),m_mouse_dev(nullptr)
						,m_mouseX(INIT_MOUSE_POS_X),m_mouseY(INIT_MOUSE_POS_Y)//�����}�E�X�ʒu
{
	//�L�[�{�[�h�E�}�E�X�{�^���Y�̏�����
	memset(m_keyboard, 0x00, sizeof(m_keyboard));
	memset(m_mouse_buttons, 0x00, sizeof(m_mouse_buttons));
}
	
//�������֐�
bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd,int screenWidth,int screenHeight)
{
	HRESULT result;	//���ʏ��i�[�p

	//�}�E�X�J�[�\���̔z�u�Ɏg�p�����ʃT�C�Y���i�[
	m_screen_width = screenWidth;
	m_screen_height = screenHeight;

	//���C���_�C���N�g�C���v�b�g�C���^�[�t�F�C�X��������
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);
	if (FAILED(result))
		return false;

	//�L�[�{�[�h�̒��ړ��̓C���^�[�t�F�C�X��������
	result = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_dev, NULL);
	if (FAILED(result))
		return false;

	//�f�[�^�`����ݒ肷��B���̏ꍇ�́A����̓L�[�{�[�h�Ȃ̂ŁA���炩���ߒ�`���ꂽ�f�[�^�`�����g�p�ł���
	result = m_keyboard_dev->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	//���̃v���O�����Ƌ��L���Ȃ��悤�ɃL�[�{�[�h�̋������x����ݒ�
	result = m_keyboard_dev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	//�L�[�{�[�h���擾
	do { result = m_keyboard_dev->Acquire(); } while (FAILED(result));

	if (FAILED(result))
		return false;

	//�}�E�X�̒��ړ��̓C���^�[�t�F�C�X��������
	result = m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse_dev, NULL);
	if (FAILED(result))
		return false;

	//��`�ς݂̃}�E�X�f�[�^�`�����g�p���āA�}�E�X�̃f�[�^�`����ݒ�
	result = m_mouse_dev->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	//���̃v���O�����Ƌ��L���邽�߂̃}�E�X�̋������x����ݒ�
	result = m_mouse_dev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	//�}�E�X���擾
	do { result = m_mouse_dev->Acquire(); } while (FAILED(result));
	if (FAILED(result))
		return false;

	//�W���C�X�e�B�b�N�̃f�o�C�X�쐬
	result = m_direct_input->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
		NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(result))
		return false;

	//�W���C�p�b�h���ڑ�����Ă�����ݒ肷��
	if (m_joypad_dev != nullptr)
	{
		//��`�ς݂̃W���C�X�e�B�b�N�f�[�^�`�����g�p���āA�W���C�X�e�B�b�N�̃f�[�^�`����ݒ�
		result = m_joypad_dev->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false;

		//���̃v���O�����Ƌ��L���邽�߂̃}�E�X�̋������x����ݒ�
		result = m_joypad_dev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(result))
			return false;

		//�f�o�C�X�̔\�͂��i�[����
		DIDEVCAPS dev_caps;
		//dwSize�����������Ă����Ȃ��Ǝ擾����ۃG���[���o��
		dev_caps.dwSize = sizeof(DIDEVCAPS);
		//�f�o�C�X�̔\�͂��擾
		//*�g�p���Ă��Ȃ�
		result = m_joypad_dev->GetCapabilities(&dev_caps);
		if (FAILED(result))
			return false;

		//�f�o�C�X�ŗ��p�\�ȓ��͂���яo�̓I�u�W�F�N�g��񋓂��܂��B
		result = m_joypad_dev->EnumObjects(EnumAxesCallback, (VOID*)hwnd, DIDFT_AXIS);
		if (FAILED(result))
			return false;

		//�|�[�����O���s��
		result = m_joypad_dev->Poll();
		if (FAILED(result))
		{
			do {//���̓f�o�C�X�ւ̃A�N�Z�X�����擾����B
				result = m_joypad_dev->Acquire();
			} while (result == DIERR_INPUTLOST);
		}
	}
	return true;
}

//�V���b�g�_�E���֐�
void InputClass::Shutdown()
{
	//�W���C�p�b�h�����
	if (m_joypad_dev)
	{
		m_joypad_dev->Unacquire();
		m_joypad_dev->Release();
		m_joypad_dev = nullptr;
	}
	//�}�E�X�����
	if (m_mouse_dev)
	{
		m_mouse_dev->Unacquire();
		m_mouse_dev->Release();
		m_mouse_dev = nullptr;
	}
	//�L�[�{�[�h�����
	if (m_keyboard_dev)
	{
		m_keyboard_dev->Unacquire();
		m_keyboard_dev->Release();
		m_keyboard_dev = nullptr;
	}
	//���͂𒼐ړ��͂��邽�߂Ƀ��C���C���^�[�t�F�C�X�����
	if (m_direct_input)
	{
		m_direct_input->Release();
		m_direct_input = nullptr;
	}

	//�{�^�����X�g�̔j��
	m_input_buttons->clear();
	delete m_input_buttons;

	return;
}

//�t���[���֐�(���t���[���Ăяo�����)
bool InputClass::Frame()
{
	//�L�[�{�[�h�̌��݂̏�Ԃ�ǂݍ���
	if (!ReadKeyboard())
		return false;

	//�}�E�X�̌��݂̏�Ԃ�ǂݍ���
	if (!ReadMouse())
		return false;

	//�W���C�p�b�h�̌��݂̏�Ԃ�ǂݍ���
	if (!ReadJoypad())
		return false;

	return true;
}

//�L�[�{�[�h�̏�Ԃ�ǂݍ��ފ֐�
bool InputClass::ReadKeyboard()
{
	HRESULT result;	//���ʏ��i�[�p
	BYTE keys[KEY_MAX] = { 0 };//�L�[�{�[�h�̉������������Ă��Ȃ����̏��擾�p

							   //�L�[�{�[�h�f�o�C�X��ǂݍ���
	result = m_keyboard_dev->GetDeviceState(sizeof(keys), (LPVOID)&keys);
	if (FAILED(result))
	{
		//�L�[�{�[�h���t�H�[�J�X���������ꍇ��擾����Ȃ������ꍇ�́A�R���g���[�������Ƃɖ߂�
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_keyboard_dev->Acquire();
		else
			return false;
	}

	//�L�[�{�[�h���X�V
	for (int key = 0; key < KEY_MAX; key++)
	{
		//�L�[��������Ă�����
		if (keys[key] & 0x80)
		{
			//���������Ă�����Ԃ�������
			if ((m_keyboard[key] & DownPressing) == DownPressing)
				//�L�[�������Ă��Ȃ���Ԃ��牟�������łȂ��̂Ńt���O��OFF�ɂ���
				m_keyboard[key] &= ~DownMoment;
			//���������Ă����ԂłȂ�������
			else//�L�[�������Ă��Ȃ���Ԃ��炨�����Ƃ��Ȃ̂Ńt���O��ON�ɂ���
				m_keyboard[key] |= DownMoment;


			//�L�[�������Ă���̂Ńt���O��ON�ɂ���
			m_keyboard[key] |= DownPressing;
		}
		//�L�[��������Ă��Ȃ�������
		else
		{
			//���������Ă�����Ԃ�������
			if ((m_keyboard[key] & DownPressing) == DownPressing)
				//�L�[�����������Ă�����Ԃ��痣�������Ȃ̂Ńt���O��ON�ɂ���
				m_keyboard[key] |= UpMoment;
			else//���������Ă��Ȃ���Ԃ�������
				//�L�[�����������Ă��Ȃ���Ԃ��痣�����킯�łȂ��̂Ńt���O��OFF�ɂ���
				m_keyboard[key] &= ~UpMoment;

			//�L�[�������Ă��Ȃ��̂Ńt���O��OFF�ɂ���
			m_keyboard[key] &= ~DownPressing;
		}
	}
	return true;
}
//�}�E�X�̏�Ԃ�ǂݍ��ފ֐�
bool InputClass::ReadMouse()
{
	HRESULT result;	//���ʏ��i�[�p
	DIMOUSESTATE mouse_state;	//�}�E�X�̏��擾�p

								//�}�E�X�f�o�C�X��ǂݍ���
	result = m_mouse_dev->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state);
	if (FAILED(result))
	{
		//�}�E�X���t�H�[�J�X���������ꍇ��擾����Ȃ������ꍇ�́A�R���g���[�������Ƃɖ߂�
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_mouse_dev->Acquire();
		else
			return false;
	}
	//�t���[�����̃}�E�X�̈ʒu�̕ω��Ɋ�Â��ă}�E�X�J�[�\���̈ʒu���X�V
	m_mouseX += mouse_state.lX;
	m_mouseY += mouse_state.lY;

	//�{�^�����X�V
	for (int button = 0; button < MOUSE_BUTON_MAX; button++)
	{
		//�}�E�X�̃{�^���������ꂽ��
		if (mouse_state.rgbButtons[button] & 0x80)
		{
			//�}�E�X�����������Ă����ԂȂ�
			if ((m_mouse_buttons[button] & DownPressing) == DownPressing)
				//�}�E�X�������Ă��Ȃ���Ԃ��牟�����킯�ł͂Ȃ��̂Ńt���O��OFF�ɂ���
				m_mouse_buttons[button] &= ~DownMoment;
			//�}�E�X�������Ă��Ȃ���ԂȂ�
			else//�}�E�X�������Ă��Ȃ���Ԃ��牟�����̂Ńt���O��ON�ɂ���
				m_mouse_buttons[button] |= DownMoment;

			//�}�E�X���������̂Ńt���O��ON�ɂ���
			m_mouse_buttons[button] |= DownPressing;

		}
		//�}�E�X�̃{�^����������ĂȂ����
		else
		{
			//�}�E�X�����������Ă�����ԂȂ�
			if ((m_mouse_buttons[button] & DownPressing) == DownPressing)
				//�}�E�X�������Ă����Ԃ��痣�����̂Ńt���O��ON�ɂ���
				m_mouse_buttons[button] |= UpMoment;
			//�}�E�X�𗣂��Ă����ԂȂ�
			else//�}�E�X�������Ă����Ԃ��痣�����킯�łȂ��̂Ńt���O��OFF�ɂ���
				m_mouse_buttons[button] &= ~UpMoment;

			//�}�E�X�𗣂����̂Ńt���O��OFF�ɂ���
			m_mouse_buttons[button] &= ~DownPressing;

		}
	}

	return true;
}
//�W���C�p�b�h�̏�Ԃ�ǂݍ��݊֐�
bool InputClass::ReadJoypad()
{
	//http://princess-tiara.biz/directx/?chapter=14
	HRESULT result;	//���ʏ��i�[�p
	DIJOYSTATE joypad_stati;	//�W���C�p�b�h�̏��i�[�p

								//�W���C�p�b�h���ڑ�����Ă��Ȃ���ΏI��
	if (m_joypad_dev == nullptr)
		return true;	//�G���[�ł͂Ȃ��̂�true��Ԃ�

	m_joypad_dev->Poll();
	result = m_joypad_dev->GetDeviceState(sizeof(DIJOYSTATE), &joypad_stati);

	if (FAILED(result))
	{
		//�W���C�p�b�h���t�H�[�J�X���������ꍇ��擾����Ȃ������ꍇ�́A�R���g���[�������Ƃɖ߂�
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_joypad_dev->Acquire();
		else
			return false;
	}

	//�{�^���Ȃǂ̏����X�V
	m_joypad.UpData(joypad_stati);

	return true;
}

//�L�[�������Ă���Ԃ͏��(�A�ˏ��)
//���� BYTE key	:DirectInput �L�[���ʃR�[�h
bool InputClass::GetKey(BYTE key)
{
	if (key < 0 || key >= KEY_MAX) return false;

	if ((m_keyboard[key] & DownPressing) == DownPressing)
		return true;
	else
		return false;
}
//�L�[�������Ă��Ȃ���Ԃ��牟������
//���� BYTE key	:DirectInput �L�[���ʃR�[�h
bool InputClass::GetKeyDown(BYTE key)
{
	if (key < 0 || key >= KEY_MAX) return false;

	if ((m_keyboard[key] & DownMoment) == DownMoment)
		return true;
	else
		return false;
}
//�L�[�������Ă����Ԃ��痣������
//���� BYTE key	:DirectInput �L�[���ʃR�[�h
bool InputClass::GetKeyUp(BYTE key)
{
	if (key < 0 || key >= KEY_MAX) return false;

	if ((m_keyboard[key] & UpMoment) == UpMoment)
		return true;
	else
		return false;
}

//�}�E�X�������Ă���Ԃ͏��(�A�ˏ��)
//���� BYTE botton:0=���{�^�� 1=�E�{�^�� 2=�����{�^��
bool InputClass::GetMouseButton(BYTE button)
{
	if (button < 0 || button >= MOUSE_BUTON_MAX) return false;

	if ((m_mouse_buttons[button] & DownPressing) == DownPressing)
		return true;
	else
		return false;
}
//�}�E�X�������Ă��Ȃ���Ԃ��牟������
//���� BYTE botton:0=���{�^�� 1=�E�{�^�� 2=�����{�^��
bool InputClass::GetMouseButtonDown(BYTE button)
{
	if (button < 0 || button >= MOUSE_BUTON_MAX) return false;

	if ((m_mouse_buttons[button] & DownMoment) == DownMoment)
		return true;
	else
		return false;
}
//�}�E�X�������Ă����Ԃ��痣������
//���� BYTE botton:0=���{�^�� 1=�E�{�^�� 2=�����{�^��
bool InputClass::GetMouseButtonUp(BYTE button)
{
	if (button < 0 || button >= MOUSE_BUTON_MAX) return false;

	if ((m_mouse_buttons[button] & UpMoment) == UpMoment)
		return true;
	else
		return false;
}

//�W���C�p�b�h�������Ă���Ԃ����(�A�ˏ��)
bool InputClass::GetJoypadButton(BYTE button,int joypad_num)
{
	if (button < 0 || button >= JOYPAD_BUTTON_MAX)
		return false;

	if ((m_joypad.buttons[button] & DownPressing) == DownPressing)
		return true;
	else
		return false;
}
//�W���C�p�b�h�������Ă��Ȃ���Ԃ��牟�����Ƃ�
bool InputClass::GetJoypadButtonDown(BYTE button,int joypad_num)
{
	if (button < 0 || button >= JOYPAD_BUTTON_MAX)
		return false;

	if ((m_joypad.buttons[button] & DownMoment) == DownMoment)
		return true;
	else
		return false;
}
//�W���C�p�b�h�������Ă����Ԃ��痣������
bool InputClass::GetJoypadButtonUp(BYTE button,int joypad_num)
{
	if (button < 0 || button >= JOYPAD_BUTTON_MAX)
		return false;

	if ((m_joypad.buttons[button] & UpMoment) == UpMoment)
		return true;
	else
		return false;
}

//�{�^���̓o�^�݂̂��s��(�����̃{�^������̃{�^���̂悤�Ɉ���)
void InputClass::SetButton(BYTE button_id)
{
	//���łɓo�^�ς݂ł���ΏI������
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if ((*itr)->m_button_id == button_id)
			return;
	}

	//�{�^�����쐬
	unique_ptr<InputButton> input_button(new InputButton(button_id));

	//���X�g�ɒǉ�
	m_input_buttons->push_back(move(input_button));
}
//�o�^�ς݂̃{�^���ɃL�[�{�[�h��ID��o�^����(�߂�l�@������true ���s��false)
bool InputClass::SetButtonKeyID(BYTE button_id,BYTE key_id1,BYTE key_id2,BYTE key_id3,BYTE key_id4,BYTE key_id5)
{
	//�o�^�ς݂ł���΃L�[�{�[�hID��o�^����
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if ((*itr)->m_button_id == button_id)
		{
			if (!(*itr)->SetKeyID(key_id1))return false;
			if (!(*itr)->SetKeyID(key_id2))return false;
			if (!(*itr)->SetKeyID(key_id3))return false;
			if (!(*itr)->SetKeyID(key_id4))return false;
			if (!(*itr)->SetKeyID(key_id5))return false;

			break;
		}
	}
	return true;
}
//�o�^�ς݂̃{�^���Ƀ}�E�X�{�^��ID��o�^����(�߂�l�@������true ���s��false)
bool InputClass::SetButtonMouseButtonID(BYTE button_id,BYTE mouse_button_id1,BYTE mouse_button_id2,BYTE mouse_button_id3,BYTE mouse_button_id4,BYTE mouse_button_id5)
{
	//�o�^�ς݂ł���΃L�[�{�[�hID��o�^����
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if ((*itr)->m_button_id == button_id)
		{
			if (!(*itr)->SetMouseButtonID(mouse_button_id1))return false;
			if (!(*itr)->SetMouseButtonID(mouse_button_id2))return false;
			if (!(*itr)->SetMouseButtonID(mouse_button_id3))return false;
			if (!(*itr)->SetMouseButtonID(mouse_button_id4))return false;
			if (!(*itr)->SetMouseButtonID(mouse_button_id5))return false;
		}
	}
	return true;
}
//�o�^�ς݂̃{�^���ɃW���C�p�b�h�{�^��ID��o�^����(�߂�l�@������true ���s��false)
bool InputClass::SetButtonJoypadButtonID(BYTE button_id,BYTE joypad_button_di1,BYTE joypad_button_di2,BYTE joypad_button_di3,BYTE joypad_button_di4,BYTE joypad_button_di5)
{
	//�o�^�ς݂ł���΃L�[�{�[�hID��o�^����
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if ((*itr)->m_button_id == button_id)
		{
			if (!(*itr)->SetJoypadButtonID(joypad_button_di1))return false;
			if (!(*itr)->SetJoypadButtonID(joypad_button_di2))return false;
			if (!(*itr)->SetJoypadButtonID(joypad_button_di3))return false;
			if (!(*itr)->SetJoypadButtonID(joypad_button_di4))return false;
			if (!(*itr)->SetJoypadButtonID(joypad_button_di5))return false;
		}
	}
}

//�{�^���������Ă���Ԃ͏��(�A�����)
bool InputClass::GetButton(BYTE button_id,int joypad_num)
{
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if (button_id == (*itr)->m_button_id)
			return (*itr)->GetButton(joypad_num);
	}
	return false;
}
//�{�^���������Ă��Ȃ���Ԃ��牟�����Ƃ�
bool InputClass::GetButtonDown(BYTE button_id,int joypad_num)
{
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if (button_id == (*itr)->m_button_id)
			return (*itr)->GetButtonDown(joypad_num);
	}
	return false;
}
//�{�^���������Ă����Ԃ��痣�����Ƃ�
bool InputClass::GetButtonUp(BYTE button_id,int joypad_num)
{
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if (button_id == (*itr)->m_button_id)
			return (*itr)->GetButtonUp(joypad_num);
	}
	return false;
}


//���݂̃}�E�X�̈ʒu�擾�p�֐�
void InputClass::GetMouseLocation(int& out_mouseX, int& out_mouseY)
{
	out_mouseX = m_mouseX;
	out_mouseY = m_mouseY;

	//�}�E�X�̈ʒu����ʂ̕��܂��͍����𒴂��Ă��Ȃ����Ƃ��m�F
	if (out_mouseX < 0) { out_mouseX = 0; }
	if (out_mouseY < 0) { out_mouseY = 0; }

	if (out_mouseX > m_screen_width) { out_mouseX = m_screen_width; }
	if (out_mouseY > m_screen_height) { out_mouseY = m_screen_height; }

	return;
}

//�W���C�X�e�B�b�N�f�o�C�X�쐬�p�R�[���o�b�N�֐�
BOOL CALLBACK InputClass::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;

	//�W���C�X�e�B�b�N�f�o�C�X�쐬
	hr = Input().m_direct_input->CreateDevice(pdidInstance->guidInstance,
		&Input().m_joypad_dev, NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;


	return DIENUM_STOP;
}
//DirectInputDevice�I�u�W�F�N�g���󂯎��A�v���P�[�V������`�̃R�[���o�b�N�֐�
BOOL CALLBACK InputClass::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	//�f�o�C�X�����̃I�u�W�F�N�g�͈̔͂Ɋւ�����
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;	//dwObj �����o�̉��ߕ��@���w�肷��l
	diprg.diph.dwObj = pdidoi->dwType;//�A�N�Z�X����v���p�e�B��������I�u�W�F�N�g�B���̃����o�ɐݒ肷��l�́AdwHow �����o�Ɏw�肳��Ă���l�ɂ���ĈقȂ�B
	diprg.lMin = -1;	//�͈͂̉���
	diprg.lMax = 1;		//�͈͂̏��
						//�f�o�C�X�̓�����`����v���p�e�B��ݒ肷��B�����̃v���p�e�B�ɂ́A���̓o�b�t�@ �T�C�Y�⎲���[�h�Ȃǂ�����B
	hr = Input().m_joypad_dev->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}
#pragma endregion