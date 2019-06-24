

//Gameシステム用ヘッダー(自作)インクルード
#include "SoundClass.h"

SoundClass::SoundClass()
{
	m_direct_sound = nullptr;
	m_primary_buffer = nullptr;
	m_secondary_buffer = nullptr;
}

SoundClass::SoundClass(const SoundClass& other)
{

}

SoundClass::~SoundClass()
{

}

bool SoundClass::Initialize(HWND hwnd,char* filename)
{
	bool result;	//結果情報格納用

	//DirectSoundAPIとプライマリサウンドバッファを初期化
	result = InitializeDirectSound(hwnd);
	if (!result)
		return false;

	//waveオーディオファイルをセカンダリバッファにロードする
	result = LoadWaveFile(filename, &m_secondary_buffer);
	if (!result)
		return false;
}

void SoundClass::Shutdown()
{
	//セカンダリバッファを解放
	ShutdownWaveFile(&m_secondary_buffer);

	//DirectSoundAPIをシャットダウン
	ShutdownDirectSound();

	return;
}

//DirectSoundAPIとプライマリサウンドバッファを初期化
bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;	//結果情報格納用
	DSBUFFERDESC buffer_desc;
	WAVEFORMATEX wave_format;

	//デフォルトのサウンドデバイスのダイレクトサウンドインターフェイスポインタを初期化
	result = DirectSoundCreate8(NULL, &m_direct_sound, NULL);
	if (FAILED(result))
		return false;

	//プライマリサウンドバッファのフォーマットを変更できるように、協調レベルをプライオリティに設定
	result = m_direct_sound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;

	//プライマリバッファの説明を設定
	buffer_desc.dwSize			= sizeof(DSBUFFERDESC);	//構造体のサイズ
	buffer_desc.dwFlags			= DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;	//バッファの能力
	buffer_desc.dwBufferBytes	= 0;					//新しいバッファサイズ　DSBCAPS_PRIMARYBUFFERを使用している場合は０
	buffer_desc.dwReserved		= 0;					//予約済み。０でなければならない
	buffer_desc.lpwfxFormat		= NULL;					//バッファのウェーブフォームフォーマット
	buffer_desc.guid3DAlgorithm = GUID_NULL;			//DirectSound3D のハードウェア エミュレーションが使う 2 スピーカ仮想化アルゴリズムの一意な識別子。

	//デフォルトサウンドデバイスのプライマリサウンドバッファのコントロールを取得
	result = m_direct_sound->CreateSoundBuffer(&buffer_desc, &m_primary_buffer, NULL);
	if (FAILED(result))
		return false;

	//プライマリサウンドバッファのフォーマットを設定
	//この場合、16ビットステレオ(cdオーディオ形式)で44100サンプル/病で記録されたWAVファイル
	wave_format.wFormatTag		= WAVE_FORMAT_PCM;	//ウェーブフォームオーディオフォーマットのタイプ
	wave_format.nSamplesPerSec	= 44100;			//標本化数(Hz)
	wave_format.wBitsPerSample	= 16;				//量子化数(バイト)
	wave_format.nChannels		= 2;				//チェンネル数
	wave_format.nBlockAlign		= (wave_format.wBitsPerSample / 8) * wave_format.nChannels;	//wFormatTagのデータの最小単位(バイト単位)
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;		//必要な平均データ転送速度(１秒あたりのバイト数)
	wave_format.cbSize			= 0;				//WAVEFORMATEX 構造体の後ろに追加されるフォーマット情報サイズ(バイト)wFormatTagの追加属性を格納できる

	//プライマリバッファを指定された波型形式に設定
	result = m_primary_buffer->SetFormat(&wave_format);
	if (FAILED(result))
		return false;

	return true;
}

//DirectSoundAPIとプライマリサウンドバッファを解放
void SoundClass::ShutdownDirectSound()
{
	//プライマリサウンドバッファポインタを解放
	if (m_primary_buffer)
	{
		m_primary_buffer->Release();
		m_primary_buffer = nullptr;
	}

	//ダイレクトサウンドインターフェイスポインタを解放
	if (m_direct_sound)
	{
		m_direct_sound->Release();
		m_direct_sound = nullptr;
	}
}

//waveオーディオファイルをセカンダリバッファにロード
bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;						//成功したかどうか確認用
	FILE* file_ptr;					//オーディオファイル読み込み用
	unsigned int count;				//成功したかどうか確認用
	WaveHeaderType wave_file_header;//ウェーブファイルのハンドル
	WAVEFORMATEX wave_format;		//ウェーブフォームフォーマット
	DSBUFFERDESC buffer_desc;		//セカンダリサウンドバッファのバッファ記述
	HRESULT result;					//関数の成功か失敗か確認用
	IDirectSoundBuffer* temp_buffer;//セカンダリバッファを作成用の一時バッファ
	unsigned char* wave_data;		//ウェーブファイルデータ
	unsigned char* buffer_ptr;		//セカンダリバッファにウェーブデータ書き込み用
	unsigned long buffer_size;		//セカンダリバッファにウェーブデータ書き込み用

	//バイナリでwaveファイルを開く
	error = fopen_s(&file_ptr, filename, "rb");
	if (error != 0)
		return false;

	//waveファイルのヘッダーを読み込む
	count = fread(&wave_file_header, sizeof(wave_file_header), 1, file_ptr);
	if (count != 1)
		return false;

	//チェックIDがRIFF形式であることを確認
	if ((wave_file_header.chunkld[0] != 'R') || (wave_file_header.chunkld[1] != 'I') ||
		(wave_file_header.chunkld[2] != 'F') || (wave_file_header.chunkld[3] != 'F'))
		return false;

	//ファイル形式がWAVE形式であることを確認
	if ((wave_file_header.format[0] != 'W') || (wave_file_header.format[1] != 'A') ||
		(wave_file_header.format[2] != 'V') || (wave_file_header.format[3] != 'E'))
		return false;

	//サブチャンクIDがfmt形式であることを確認
	if ((wave_file_header.subChunkld[0] != 'f') || (wave_file_header.subChunkld[1] != 'm') ||
		(wave_file_header.subChunkld[2] != 't') || (wave_file_header.subChunkld[3] != ' '))
		return false;

	//オーディオフォーマットがWAVE_FORMAT_PCMであることを確認
	if (wave_file_header.audioFormat != WAVE_FORMAT_PCM)
		return false;

	//waveファイルがステレオフォーマットで記録されたことを確認
	if (wave_file_header.unmChannels != 2)
		return false;

	//ウェーブフォームが44.1KHｚのサンプルグレートで記憶されたことを確認
	if (wave_file_header.sampleRate != 44100)
		return false;

	//waveファイルが16ビット形式で記憶されていることを確認
	if (wave_file_header.bitsPerSample != 16)
		return false;

	//データチェックヘッダーをチェック
	/*if ((wave_file_header.dataChunkld[0] != 'd') || (wave_file_header.dataChunkld[1] != 'a') ||
		(wave_file_header.dataChunkld[2] != 't') || (wave_file_header.dataChunkld[3] != 'a'))
		return false;*/

	//このwaveファイルがロードされるセカンダリバッファのウェーブフォームフォーマットを設定
	wave_format.wFormatTag = WAVE_FORMAT_PCM;											//フォーマット　タグ　PCM形式はWAVE_FORMAT_PCM
	wave_format.nSamplesPerSec = 44100;													//標本化周波数
	wave_format.wBitsPerSample = 16;													//量子化ビット数
	wave_format.nChannels = 2;															//チェンネル数　１チェンネルならモノラル　２チャンネルならステレオ
	wave_format.nBlockAlign = (wave_format.wBitsPerSample / 8) * wave_format.nChannels;	//ｗFormatTagのデータの最小単位。　計算方法はｗFormatTagによって決まっている
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;	//フォーマット タグで必要な平均データ転送速度。単位は、1 秒あたりのバイト数。　計算方法はｗFormatTagによって決まっている
	wave_format.cbSize = 0;																//WAVEFORMATEX 構造体の後ろに追加されるフォーマット情報のサイズ (バイト単位)。

	//ウェーブファイルがロードされるセカンダリサウンドバッファのバッファ記述を設定
	buffer_desc.dwSize = sizeof(DSBUFFERDESC);				//構造体のサイズ
	buffer_desc.dwFlags = DSBCAPS_CTRLVOLUME;				//バッファの能力
	buffer_desc.dwBufferBytes = wave_file_header.dataSize;	//新しいバッファサイズ　DSBCAPS_PRIMARYBUFFERを使用している場合は０
	buffer_desc.dwReserved = 0;								//予約済み。０でなければならない
	buffer_desc.lpwfxFormat = &wave_format;					//バッファのウェーブフォームフォーマット
	buffer_desc.guid3DAlgorithm = GUID_NULL;				//DirectSound3D のハードウェア エミュレーションが使う 2 スピーカ仮想化アルゴリズムの一意な識別子。

	//特定のバッファ設定で一時的なサウンドバッファを作成。
	result = m_direct_sound->CreateSoundBuffer(&buffer_desc, &temp_buffer, NULL);
	if (FAILED(result))
		return false;

	//ダイレクトサウンド８インターフェイスに対してバッファフォーマットをテストし、セカンダリバッファを作成
	result = temp_buffer->QueryInterface(IID_IDirectSoundBuffer8, (void**) &* secondaryBuffer);
	if (FAILED(result))
		return false;

	//一時バッファを解放
	temp_buffer->Release();
	temp_buffer = 0;

	//データチャンクヘッダーの最後で始まるウェーブデータの先頭に移動。
	fseek(file_ptr, sizeof(WaveHeaderType), SEEK_SET);

	//ウェーブファイルデータを保持するための一時バッファを作成
	wave_data = new unsigned char[wave_file_header.dataSize];
	if (!wave_data)
		return false;

	//ウェーブファイルのデータを新しく作成したバッファに読み込む
	count = fread(wave_data, 1, wave_file_header.dataSize, file_ptr);
	if (count != wave_file_header.dataSize)
		return false;

	//読み終わったらファイルを閉じる
	error = fclose(file_ptr);
	if (error != 0)
		return false;

	//セカンダリバッファをロックして、ウェーブデータを書き込む
	result = (*secondaryBuffer)->Lock(0, wave_file_header.dataSize, (void**)&buffer_ptr, (DWORD *)&buffer_size, NULL, 0, 0);
	if (FAILED(result))
		return false;

	//ウェーブデータをバッファにコピー
	result = (*secondaryBuffer)->Unlock((void*)buffer_ptr, buffer_size, NULL, 0);
	if (FAILED(result))
		return false;

	//セカンダリバッファにコピーされたので、ウェーブデータを解放
	delete[] wave_data;
	wave_data = 0;

	return true;
 
}

//セカンダリバッファを解放
void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	//セカンダリサウンドバッファを解放
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

//読み込まれた音楽を再生
bool SoundClass::PlayWaveFile()
{
	HRESULT result;	//結果

	//サウンドバッファの先頭に位置を設定
	result = m_secondary_buffer->SetCurrentPosition(0);
	if (FAILED(result))
		return false;

	//バッファのボリュームを100%に設定
	result = m_secondary_buffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
		return false;

	//セカンダリサウンドバッファの内容を再生
	result = m_secondary_buffer->Play(0, 0, 0);
	if (FAILED(result))
		return false;
	
	return true;

}