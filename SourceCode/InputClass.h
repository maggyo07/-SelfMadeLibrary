#pragma once

//プリプロセッサの指示
//ヘッダーで使用しているダイレクトインプットのバージョンを定義
//そうしないと、コンパイラはデフォルトでバージョン8になっている迷惑なメッセージを作成する
#define DIRECTINPUT_VERSION 0x0800

//リンク
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

//システム系のヘッダーインクルード
#include <dinput.h>
#include <list>
#include <memory>

constexpr auto KEY_MAX					{ 256 };	//キーボードのキーの最大数
constexpr auto MOUSE_BUTON_MAX			{ 4 };	//マウスのボタンの最大数
constexpr auto JOYPAD_BUTTON_MAX		{ 32 };	//ジョイパッドの最大ボタン数(32以下でないとエラー)
constexpr auto SAME_TERMINAL_BUTTON_MAX	{ 5 };	//同じ端末のボタン登録最大数
constexpr auto INPUT_NOT_ID				{ 255 };	//IDが設定されていない状態

constexpr auto INIT_MOUSE_POS_X			{0};
constexpr auto INIT_MOUSE_POS_Y			{0};

using namespace std;

//前方宣言
class InputClass;

//入力情報
enum PushInfo
{
	DownPressing = 1,	//キーを押している(連射状態)
	DownMoment = 1 << 1,	//キーを押していない状態から押した時
	UpMoment = 1 << 2,	//キーを押している状態から離した時
};

//ボタンID
namespace ButtonID
{
	enum
	{
		NOT,
		LEFT,		//左ボタン
		RIGHT,		//右ボタン
		UP,			//上ボタン
		DOWN,		//下ボタン
		ATTACK,		//攻撃ボタン
		ITEM_USE,	//アイテム使用ボタン
	};
}

//ボタン登録用(複数のボタンを一つのボタンのようにする)
struct InputButton
{
	friend class InputClass;
public:
	InputButton(int button_id);

	//ボタンを押している間は常に(連鎖状態)
	bool GetButton(int joypad_num = 0);
	//ボタンを押していない状態から押したとき
	bool GetButtonDown(int joypad_num = 0);
	//ボタンが押している状態から離したとき
	bool GetButtonUp(int joypad_num = 0);

	//キーボードIDを設定する(戻り値　成功＝true 失敗＝false)	
	bool SetKeyID(BYTE key_id);
	//マウスボタンIDを設定する(戻り値　成功＝true 失敗＝false)
	bool SetMouseButtonID(BYTE mouse_button_id);
	//ジョイパッドボタンIDを設定する(戻り値　成功＝true 失敗＝false)
	bool SetJoypadButtonID(BYTE jpypad_button_id);
private:
	BYTE m_key_ids[SAME_TERMINAL_BUTTON_MAX];			//キーボードのID
	BYTE m_mouse_button_ids[SAME_TERMINAL_BUTTON_MAX];	//マウスのID
	BYTE m_joypad_button_ids[SAME_TERMINAL_BUTTON_MAX];	//ジョイパッドのID
	int m_button_id;			//登録番号
};

//入力クラス
class InputClass
{
	struct Joypad
	{
		Joypad();
		//引数のDIJOYSTATEを元に更新する
		void UpData(const DIJOYSTATE& joypad_stati);

		BYTE buttons[JOYPAD_BUTTON_MAX];	//ボタンの配列
		int lX;	//通常はスティックの左右の動きを表す x 軸。
		int lY;	//通常はスティックの前後の動きを表す y 軸。
		int lZ;	//z 軸。スロットル制御を表す場合が多い。ジョイスティックにこの軸がない場合、値はゼロである。
		int lRx;//x 軸回転。ジョイスティックにこの軸がない場合、値はゼロである。
		int lRy;//y 軸回転。ジョイスティックにこの軸がない場合、値はゼロである。
		int lRz;//z 軸回転 (方向舵と呼ばれる)。ジョイスティックにこの軸がない場合、値はゼロである。
		int rgl_slider[2];//2 つの追加軸 (以前には u 軸と v 軸と呼ばれていた)。この値の意味はジョイスティックで異なる。
	};
public:
	InputClass();
	//初期化関数
	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	//シャットダウン関数
	void Shutdown();
	//フレーム関数(毎フレーム呼び出される)
	bool Frame();

	//キーを押している間は常に(連射状態)
	//引数 BYTE key	:DirectInput キー識別コード
	bool GetKey(BYTE key);
	//キーを押していない状態から押した時
	//引数 BYTE key	:DirectInput キー識別コード
	bool GetKeyDown(BYTE key);
	//キーを押している状態から離した時
	//引数 BYTE key	:DirectInput キー識別コード
	bool GetKeyUp(BYTE key);

	//マウスを押している間は常に(連射状態)
	//引数 BYTE botton:0=左ボタン 1=右ボタン 2=中央ボタン
	bool GetMouseButton(BYTE button);
	//マウスをおしていない状態から押した時
	//引数 BYTE botton:0=左ボタン 1=右ボタン 2=中央ボタン
	bool GetMouseButtonDown(BYTE button);
	//マウスを押している状態から離した時
	//引数 BYTE botton:0=左ボタン 1=右ボタン 2=中央ボタン
	bool GetMouseButtonUp(BYTE button);

	//ジョイパッドを押している間が常に(連射状態)
	bool GetJoypadButton(BYTE button, int joypad_num = 0);
	//ジョイパッドを押していない状態から押したとき
	bool GetJoypadButtonDown(BYTE button, int joypad_num = 0);
	//ジョイパッドを押している状態から離した時
	bool GetJoypadButtonUp(BYTE button, int joypad_num = 0);

	//ボタンの登録のみを行う(複数のボタンを一つのボタンのように扱う)
	void SetButton(BYTE button_id);
	//登録済みのボタンにキーボードのIDを登録する(戻り値　成功＝true 失敗＝false)
	bool SetButtonKeyID(BYTE button_id, BYTE key_id1, BYTE key_id2 = INPUT_NOT_ID, BYTE key_id3 = INPUT_NOT_ID, BYTE key_id4 = INPUT_NOT_ID, BYTE key_id5 = INPUT_NOT_ID);
	//登録済みのボタンにマウスボタンIDを登録する(戻り値　成功＝true 失敗＝false)
	bool SetButtonMouseButtonID(BYTE button_id, BYTE mouse_button_id1, BYTE mouse_button_id2 = INPUT_NOT_ID, BYTE mouse_button_id3 = INPUT_NOT_ID, BYTE mouse_button_id4 = INPUT_NOT_ID, BYTE mouse_button_id5 = INPUT_NOT_ID);
	//登録済みのボタンにジョイパッドボタンIDを登録する(戻り値　成功＝true 失敗＝false)
	bool SetButtonJoypadButtonID(BYTE button_id, BYTE joypad_button_di1, BYTE joypad_button_di2 = INPUT_NOT_ID, BYTE joypad_button_di3 = INPUT_NOT_ID, BYTE joypad_button_di4 = INPUT_NOT_ID, BYTE joypad_button_di5 = INPUT_NOT_ID);

	//ボタンを押している間は常に(連鎖状態)
	bool GetButton(BYTE button_id, int joypad_num = 0);
	//ボタンを押していない状態から押したとき
	bool GetButtonDown(BYTE button_id, int joypad_num = 0);
	//ボタンが押している状態から離したとき
	bool GetButtonUp(BYTE button_id, int joypad_num = 0);

	//現在のマウスの位置取得用関数
	void GetMouseLocation(int& out_mouseX, int& out_mouseY);

	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
private:
	//キーボードの状態を読み込む関数
	bool ReadKeyboard();
	//マウスの状態を読み込む関数
	bool ReadMouse();
	//ジョイパッドの状態を読み込み関数
	bool ReadJoypad();
private:
	IDirectInput8* m_direct_input;		//メインインターフェイス
	IDirectInputDevice8* m_keyboard_dev;	//キーボードの入力デバイス
	IDirectInputDevice8* m_mouse_dev;		//マウスの入力デバイス

	IDirectInputDevice8* m_joypad_dev;	//ジョイパッドの入力デバイス

	BYTE m_keyboard[KEY_MAX];//キーボード配列
	BYTE m_mouse_buttons[MOUSE_BUTON_MAX];	//マウスのボタン
	Joypad m_joypad;			//ジョイパッド
	list<unique_ptr<InputButton>>* m_input_buttons;		//入力ボタン登録用リスト

	int m_screen_width, m_screen_height;
	int m_mouseX, m_mouseY;
};



