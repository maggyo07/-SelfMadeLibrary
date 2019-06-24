//Gameシステム用ヘッダー(自作)インクルード
#include "InputClass.h"

#include "AccessController.h"
using System::Input;

#pragma region InputButton
//コンストラクタ
InputButton::InputButton(int button_id):m_button_id(button_id)
{
	//キー・マウス・ジョイパッドのID配列を初期化
	memset(m_key_ids, INPUT_NOT_ID, sizeof(m_key_ids));
	memset(m_mouse_button_ids, INPUT_NOT_ID, sizeof(m_mouse_button_ids));
	memset(m_joypad_button_ids, INPUT_NOT_ID, sizeof(m_joypad_button_ids));
}

//ボタンを押している間は常に(連鎖状態)
bool InputButton::GetButton(int joypad_num)
{
	//キーの入力状態をチェック
	for each (int key_id in m_key_ids)
		if (key_id != INPUT_NOT_ID)
			if (Input().GetKey(key_id))
				return true;

	//マウスの入力状態をチェック
	for each (int mouse_button_id in m_mouse_button_ids)
		if (mouse_button_id != INPUT_NOT_ID)
			if (Input().GetMouseButton(mouse_button_id))
				return true;

	//ジョイパッドの入力状態をチェック
	for each (int joypad_button_id in m_joypad_button_ids)
		if (joypad_button_id != INPUT_NOT_ID)
			if (Input().GetJoypadButton(joypad_num, joypad_button_id))
				return true;

	return false;
}
//ボタンを押していない状態から押したとき
bool InputButton::GetButtonDown(int joypad_num)
{
	//キーの入力状態をチェック
	for each (int key_id in m_key_ids)
		if (key_id != INPUT_NOT_ID)
			if (Input().GetKeyDown(key_id))
				return true;

	//マウスの入力状態をチェック
	for each (int mouse_button_id in m_mouse_button_ids)
		if (mouse_button_id != INPUT_NOT_ID)
			if (Input().GetMouseButtonDown(mouse_button_id))
				return true;

	//ジョイパッドの入力状態をチェック
	for each (int joypad_button_id in m_joypad_button_ids)
		if (joypad_button_id != INPUT_NOT_ID)
			if (Input().GetJoypadButtonDown(joypad_num, joypad_button_id))
				return true;

	return false;
}
//ボタンが押している状態から離したとき
bool InputButton::GetButtonUp(int joypad_num)
{
	//キーの入力状態をチェック
	for each (int key_id in m_key_ids)
		if (key_id != INPUT_NOT_ID)
			if (Input().GetKeyUp(key_id))
				return true;

	//マウスの入力状態をチェック
	for each (int mouse_button_id in m_mouse_button_ids)
		if (mouse_button_id != INPUT_NOT_ID)
			if (Input().GetMouseButtonUp(mouse_button_id))
				return true;

	//ジョイパッドの入力状態をチェック
	for each (int joypad_button_id in m_joypad_button_ids)
		if (joypad_button_id != INPUT_NOT_ID)
			if (Input().GetJoypadButtonUp(joypad_num, joypad_button_id))
				return true;

	return false;
}

//キーボードIDを設定する(戻り値　成功＝true 失敗＝false)	
bool InputButton::SetKeyID(BYTE key_id)
{
	//引数のIDがなければ終了(一応INPUT_NOT_IDが来る前提なのでtrueを返す)
	if (key_id == INPUT_NOT_ID)
		return true;
	for each (auto& id in m_key_ids)
	{
		//すでに登録しているIDなら失敗
		if (id == key_id)
			return false;
		//登録しようとしている場所が設定されていなければ設定する
		else if (id == INPUT_NOT_ID)
		{
			id = key_id;
			return true;
		}
	}
	return false;
}
//マウスボタンIDを設定する(戻り値　成功＝true 失敗＝false)
bool InputButton::SetMouseButtonID(BYTE mouse_button_id)
{
	//引数のIDがなければ終了(一応INPUT_NOT_IDが来る前提なのでtrueを返す)
	if (mouse_button_id == INPUT_NOT_ID)
		return true;
	for each (auto& id in m_mouse_button_ids)
	{
		//すでに登録しているIDなら失敗
		if (id == mouse_button_id)
			return false;
		//登録しようとしている場所が設定されていなければ設定する
		else if (id == INPUT_NOT_ID)
		{
			id = mouse_button_id;
			return true;
		}
	}
	return false;
}
//ジョイパッドボタンIDを設定する(戻り値　成功＝true 失敗＝false)
bool InputButton::SetJoypadButtonID(BYTE jpypad_button_id)
{
	//引数のIDがなければ終了(一応INPUT_NOT_IDが来る前提なのでtrueを返す)
	if (jpypad_button_id == INPUT_NOT_ID)
		return true;
	for each (auto& id in m_joypad_button_ids)
	{
		//すでに登録しているIDなら失敗
		if (id == jpypad_button_id)
			return false;
		//登録しようとしている場所が設定されていなければ設定する
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
		//メンバ変数全てを０で初期化
		memset(buttons, 0x00, sizeof(JOYPAD_BUTTON_MAX));
		lX = lY = lZ = 0;
		lRz = lRy = lRz = 0;
		memset(rgl_slider, 0x00, sizeof(rgl_slider));
	}

	//引数のDIJOYSTATEを元に更新する
	void InputClass::Joypad::UpData(const DIJOYSTATE& joypad_stati)
	{
		if (&joypad_stati == nullptr)
			return;

		//ボタンの情報を更新
		for (int button = 0; button < JOYPAD_BUTTON_MAX; button++)
		{
			//ボタンが押されていたら
			if (joypad_stati.rgbButtons[button] & 0x80)
			{
				//押し続けていた状態だったら
				if ((buttons[button] & DownPressing) == DownPressing)
					//ボタンを押していない状態から押した時でないのでフラグをOFFにする
					buttons[button] &= ~DownMoment;
				//押し続けている状態でなかったら
				else//ボタンを押していない状態からおしたときなのでフラグをONにする
					buttons[button] |= DownMoment;


				//ボタンを押しているのでフラグをONにする
				buttons[button] |= DownPressing;
			}
			//ボタンが押されていなかったら
			else
			{
				//押し続けていた状態だったら
				if ((buttons[button] & DownPressing) == DownPressing)
					//ボタンを押し続けていた状態から離した時なのでフラグをONにする
					buttons[button] |= UpMoment;
				else//押し続けていない状態だったら
					//ボタンを押し続けていない状態から離したわけでないのでフラグをOFFにする
					buttons[button] &= ~UpMoment;

				//ボタンを押していないのでフラグをOFFにする
				buttons[button] &= ~DownPressing;
			}
		}

		//軸系の値を更新
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
						,m_mouseX(INIT_MOUSE_POS_X),m_mouseY(INIT_MOUSE_POS_Y)//初期マウス位置
{
	//キーボード・マウスボタンズの初期化
	memset(m_keyboard, 0x00, sizeof(m_keyboard));
	memset(m_mouse_buttons, 0x00, sizeof(m_mouse_buttons));
}
	
//初期化関数
bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd,int screenWidth,int screenHeight)
{
	HRESULT result;	//結果情報格納用

	//マウスカーソルの配置に使用する画面サイズを格納
	m_screen_width = screenWidth;
	m_screen_height = screenHeight;

	//メインダイレクトインプットインターフェイスを初期化
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);
	if (FAILED(result))
		return false;

	//キーボードの直接入力インターフェイスを初期化
	result = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_dev, NULL);
	if (FAILED(result))
		return false;

	//データ形式を設定する。この場合は、それはキーボードなので、あらかじめ定義されたデータ形式を使用できる
	result = m_keyboard_dev->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	//他のプログラムと共有しないようにキーボードの協調レベルを設定
	result = m_keyboard_dev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	//キーボードを取得
	do { result = m_keyboard_dev->Acquire(); } while (FAILED(result));

	if (FAILED(result))
		return false;

	//マウスの直接入力インターフェイスを初期化
	result = m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse_dev, NULL);
	if (FAILED(result))
		return false;

	//定義済みのマウスデータ形式を使用して、マウスのデータ形式を設定
	result = m_mouse_dev->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	//他のプログラムと共有するためのマウスの協調レベルを設定
	result = m_mouse_dev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	//マウスを取得
	do { result = m_mouse_dev->Acquire(); } while (FAILED(result));
	if (FAILED(result))
		return false;

	//ジョイスティックのデバイス作成
	result = m_direct_input->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
		NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(result))
		return false;

	//ジョイパッドが接続されていたら設定する
	if (m_joypad_dev != nullptr)
	{
		//定義済みのジョイスティックデータ形式を使用して、ジョイスティックのデータ形式を設定
		result = m_joypad_dev->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false;

		//他のプログラムと共有するためのマウスの協調レベルを設定
		result = m_joypad_dev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(result))
			return false;

		//デバイスの能力を格納する
		DIDEVCAPS dev_caps;
		//dwSizeを初期化しておかないと取得する際エラーが出る
		dev_caps.dwSize = sizeof(DIDEVCAPS);
		//デバイスの能力を取得
		//*使用していない
		result = m_joypad_dev->GetCapabilities(&dev_caps);
		if (FAILED(result))
			return false;

		//デバイスで利用可能な入力および出力オブジェクトを列挙します。
		result = m_joypad_dev->EnumObjects(EnumAxesCallback, (VOID*)hwnd, DIDFT_AXIS);
		if (FAILED(result))
			return false;

		//ポーリングを行う
		result = m_joypad_dev->Poll();
		if (FAILED(result))
		{
			do {//入力デバイスへのアクセス権を取得する。
				result = m_joypad_dev->Acquire();
			} while (result == DIERR_INPUTLOST);
		}
	}
	return true;
}

//シャットダウン関数
void InputClass::Shutdown()
{
	//ジョイパッドを解放
	if (m_joypad_dev)
	{
		m_joypad_dev->Unacquire();
		m_joypad_dev->Release();
		m_joypad_dev = nullptr;
	}
	//マウスを解放
	if (m_mouse_dev)
	{
		m_mouse_dev->Unacquire();
		m_mouse_dev->Release();
		m_mouse_dev = nullptr;
	}
	//キーボードを解放
	if (m_keyboard_dev)
	{
		m_keyboard_dev->Unacquire();
		m_keyboard_dev->Release();
		m_keyboard_dev = nullptr;
	}
	//入力を直接入力するためにメインインターフェイスを解放
	if (m_direct_input)
	{
		m_direct_input->Release();
		m_direct_input = nullptr;
	}

	//ボタンリストの破棄
	m_input_buttons->clear();
	delete m_input_buttons;

	return;
}

//フレーム関数(毎フレーム呼び出される)
bool InputClass::Frame()
{
	//キーボードの現在の状態を読み込む
	if (!ReadKeyboard())
		return false;

	//マウスの現在の状態を読み込む
	if (!ReadMouse())
		return false;

	//ジョイパッドの現在の状態を読み込む
	if (!ReadJoypad())
		return false;

	return true;
}

//キーボードの状態を読み込む関数
bool InputClass::ReadKeyboard()
{
	HRESULT result;	//結果情報格納用
	BYTE keys[KEY_MAX] = { 0 };//キーボードの押したか押していないかの情報取得用

							   //キーボードデバイスを読み込む
	result = m_keyboard_dev->GetDeviceState(sizeof(keys), (LPVOID)&keys);
	if (FAILED(result))
	{
		//キーボードがフォーカスを失った場合や取得されなかった場合は、コントロールをもとに戻せ
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_keyboard_dev->Acquire();
		else
			return false;
	}

	//キーボード情報更新
	for (int key = 0; key < KEY_MAX; key++)
	{
		//キーが押されていたら
		if (keys[key] & 0x80)
		{
			//押し続けていた状態だったら
			if ((m_keyboard[key] & DownPressing) == DownPressing)
				//キーを押していない状態から押した時でないのでフラグをOFFにする
				m_keyboard[key] &= ~DownMoment;
			//押し続けている状態でなかったら
			else//キーを押していない状態からおしたときなのでフラグをONにする
				m_keyboard[key] |= DownMoment;


			//キーを押しているのでフラグをONにする
			m_keyboard[key] |= DownPressing;
		}
		//キーが押されていなかったら
		else
		{
			//押し続けていた状態だったら
			if ((m_keyboard[key] & DownPressing) == DownPressing)
				//キーを押し続けていた状態から離した時なのでフラグをONにする
				m_keyboard[key] |= UpMoment;
			else//押し続けていない状態だったら
				//キーを押し続けていない状態から離したわけでないのでフラグをOFFにする
				m_keyboard[key] &= ~UpMoment;

			//キーを押していないのでフラグをOFFにする
			m_keyboard[key] &= ~DownPressing;
		}
	}
	return true;
}
//マウスの状態を読み込む関数
bool InputClass::ReadMouse()
{
	HRESULT result;	//結果情報格納用
	DIMOUSESTATE mouse_state;	//マウスの情報取得用

								//マウスデバイスを読み込む
	result = m_mouse_dev->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state);
	if (FAILED(result))
	{
		//マウスがフォーカスを失った場合や取得されなかった場合は、コントロールをもとに戻せ
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_mouse_dev->Acquire();
		else
			return false;
	}
	//フレーム中のマウスの位置の変化に基づいてマウスカーソルの位置を更新
	m_mouseX += mouse_state.lX;
	m_mouseY += mouse_state.lY;

	//ボタン情報更新
	for (int button = 0; button < MOUSE_BUTON_MAX; button++)
	{
		//マウスのボタンが押されたら
		if (mouse_state.rgbButtons[button] & 0x80)
		{
			//マウスを押し続けている状態なら
			if ((m_mouse_buttons[button] & DownPressing) == DownPressing)
				//マウスを押していない状態から押したわけではないのでフラグをOFFにする
				m_mouse_buttons[button] &= ~DownMoment;
			//マウスを押していない状態なら
			else//マウスを押していない状態から押したのでフラグをONにする
				m_mouse_buttons[button] |= DownMoment;

			//マウスを押したのでフラグをONにする
			m_mouse_buttons[button] |= DownPressing;

		}
		//マウスのボタンが押されてなければ
		else
		{
			//マウスを押し続けていた状態なら
			if ((m_mouse_buttons[button] & DownPressing) == DownPressing)
				//マウスを押している状態から離したのでフラグをONにする
				m_mouse_buttons[button] |= UpMoment;
			//マウスを離している状態なら
			else//マウスを押している状態から離したわけでないのでフラグをOFFにする
				m_mouse_buttons[button] &= ~UpMoment;

			//マウスを離したのでフラグをOFFにする
			m_mouse_buttons[button] &= ~DownPressing;

		}
	}

	return true;
}
//ジョイパッドの状態を読み込み関数
bool InputClass::ReadJoypad()
{
	//http://princess-tiara.biz/directx/?chapter=14
	HRESULT result;	//結果情報格納用
	DIJOYSTATE joypad_stati;	//ジョイパッドの情報格納用

								//ジョイパッドが接続されていなければ終了
	if (m_joypad_dev == nullptr)
		return true;	//エラーではないのでtrueを返す

	m_joypad_dev->Poll();
	result = m_joypad_dev->GetDeviceState(sizeof(DIJOYSTATE), &joypad_stati);

	if (FAILED(result))
	{
		//ジョイパッドがフォーカスを失った場合や取得されなかった場合は、コントロールをもとに戻せ
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_joypad_dev->Acquire();
		else
			return false;
	}

	//ボタンなどの情報を更新
	m_joypad.UpData(joypad_stati);

	return true;
}

//キーを押している間は常に(連射状態)
//引数 BYTE key	:DirectInput キー識別コード
bool InputClass::GetKey(BYTE key)
{
	if (key < 0 || key >= KEY_MAX) return false;

	if ((m_keyboard[key] & DownPressing) == DownPressing)
		return true;
	else
		return false;
}
//キーを押していない状態から押した時
//引数 BYTE key	:DirectInput キー識別コード
bool InputClass::GetKeyDown(BYTE key)
{
	if (key < 0 || key >= KEY_MAX) return false;

	if ((m_keyboard[key] & DownMoment) == DownMoment)
		return true;
	else
		return false;
}
//キーを押している状態から離した時
//引数 BYTE key	:DirectInput キー識別コード
bool InputClass::GetKeyUp(BYTE key)
{
	if (key < 0 || key >= KEY_MAX) return false;

	if ((m_keyboard[key] & UpMoment) == UpMoment)
		return true;
	else
		return false;
}

//マウスが押している間は常に(連射状態)
//引数 BYTE botton:0=左ボタン 1=右ボタン 2=中央ボタン
bool InputClass::GetMouseButton(BYTE button)
{
	if (button < 0 || button >= MOUSE_BUTON_MAX) return false;

	if ((m_mouse_buttons[button] & DownPressing) == DownPressing)
		return true;
	else
		return false;
}
//マウスをおしていない状態から押した時
//引数 BYTE botton:0=左ボタン 1=右ボタン 2=中央ボタン
bool InputClass::GetMouseButtonDown(BYTE button)
{
	if (button < 0 || button >= MOUSE_BUTON_MAX) return false;

	if ((m_mouse_buttons[button] & DownMoment) == DownMoment)
		return true;
	else
		return false;
}
//マウスを押している状態から離した時
//引数 BYTE botton:0=左ボタン 1=右ボタン 2=中央ボタン
bool InputClass::GetMouseButtonUp(BYTE button)
{
	if (button < 0 || button >= MOUSE_BUTON_MAX) return false;

	if ((m_mouse_buttons[button] & UpMoment) == UpMoment)
		return true;
	else
		return false;
}

//ジョイパッドを押している間が常に(連射状態)
bool InputClass::GetJoypadButton(BYTE button,int joypad_num)
{
	if (button < 0 || button >= JOYPAD_BUTTON_MAX)
		return false;

	if ((m_joypad.buttons[button] & DownPressing) == DownPressing)
		return true;
	else
		return false;
}
//ジョイパッドを押していない状態から押したとき
bool InputClass::GetJoypadButtonDown(BYTE button,int joypad_num)
{
	if (button < 0 || button >= JOYPAD_BUTTON_MAX)
		return false;

	if ((m_joypad.buttons[button] & DownMoment) == DownMoment)
		return true;
	else
		return false;
}
//ジョイパッドを押している状態から離した時
bool InputClass::GetJoypadButtonUp(BYTE button,int joypad_num)
{
	if (button < 0 || button >= JOYPAD_BUTTON_MAX)
		return false;

	if ((m_joypad.buttons[button] & UpMoment) == UpMoment)
		return true;
	else
		return false;
}

//ボタンの登録のみを行う(複数のボタンを一つのボタンのように扱う)
void InputClass::SetButton(BYTE button_id)
{
	//すでに登録済みであれば終了する
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if ((*itr)->m_button_id == button_id)
			return;
	}

	//ボタンを作成
	unique_ptr<InputButton> input_button(new InputButton(button_id));

	//リストに追加
	m_input_buttons->push_back(move(input_button));
}
//登録済みのボタンにキーボードのIDを登録する(戻り値　成功＝true 失敗＝false)
bool InputClass::SetButtonKeyID(BYTE button_id,BYTE key_id1,BYTE key_id2,BYTE key_id3,BYTE key_id4,BYTE key_id5)
{
	//登録済みであればキーボードIDを登録する
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
//登録済みのボタンにマウスボタンIDを登録する(戻り値　成功＝true 失敗＝false)
bool InputClass::SetButtonMouseButtonID(BYTE button_id,BYTE mouse_button_id1,BYTE mouse_button_id2,BYTE mouse_button_id3,BYTE mouse_button_id4,BYTE mouse_button_id5)
{
	//登録済みであればキーボードIDを登録する
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
//登録済みのボタンにジョイパッドボタンIDを登録する(戻り値　成功＝true 失敗＝false)
bool InputClass::SetButtonJoypadButtonID(BYTE button_id,BYTE joypad_button_di1,BYTE joypad_button_di2,BYTE joypad_button_di3,BYTE joypad_button_di4,BYTE joypad_button_di5)
{
	//登録済みであればキーボードIDを登録する
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

//ボタンを押している間は常に(連鎖状態)
bool InputClass::GetButton(BYTE button_id,int joypad_num)
{
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if (button_id == (*itr)->m_button_id)
			return (*itr)->GetButton(joypad_num);
	}
	return false;
}
//ボタンを押していない状態から押したとき
bool InputClass::GetButtonDown(BYTE button_id,int joypad_num)
{
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if (button_id == (*itr)->m_button_id)
			return (*itr)->GetButtonDown(joypad_num);
	}
	return false;
}
//ボタンが押している状態から離したとき
bool InputClass::GetButtonUp(BYTE button_id,int joypad_num)
{
	for (auto itr = m_input_buttons->begin(); itr != m_input_buttons->end(); itr++)
	{
		if (button_id == (*itr)->m_button_id)
			return (*itr)->GetButtonUp(joypad_num);
	}
	return false;
}


//現在のマウスの位置取得用関数
void InputClass::GetMouseLocation(int& out_mouseX, int& out_mouseY)
{
	out_mouseX = m_mouseX;
	out_mouseY = m_mouseY;

	//マウスの位置が画面の幅または高さを超えていないことを確認
	if (out_mouseX < 0) { out_mouseX = 0; }
	if (out_mouseY < 0) { out_mouseY = 0; }

	if (out_mouseX > m_screen_width) { out_mouseX = m_screen_width; }
	if (out_mouseY > m_screen_height) { out_mouseY = m_screen_height; }

	return;
}

//ジョイスティックデバイス作成用コールバック関数
BOOL CALLBACK InputClass::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;

	//ジョイスティックデバイス作成
	hr = Input().m_direct_input->CreateDevice(pdidInstance->guidInstance,
		&Input().m_joypad_dev, NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;


	return DIENUM_STOP;
}
//DirectInputDeviceオブジェクトを受け取るアプリケーション定義のコールバック関数
BOOL CALLBACK InputClass::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	//デバイス無いのオブジェクトの範囲に関する情報
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;	//dwObj メンバの解釈方法を指定する値
	diprg.diph.dwObj = pdidoi->dwType;//アクセスするプロパティが属するオブジェクト。このメンバに設定する値は、dwHow メンバに指定されている値によって異なる。
	diprg.lMin = -1;	//範囲の下限
	diprg.lMax = 1;		//範囲の上限
						//デバイスの動作を定義するプロパティを設定する。これらのプロパティには、入力バッファ サイズや軸モードなどがある。
	hr = Input().m_joypad_dev->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}
#pragma endregion