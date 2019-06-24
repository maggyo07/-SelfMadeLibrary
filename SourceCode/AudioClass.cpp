#include "AudioClass.h"

AudioClass::AudioClass()	:m_mastering_voice(nullptr),m_speaker_flag(true)
							,m_source_voice_list(new list<unique_ptr<AudioData>>)
{

}
bool AudioClass::Initialize()
{
	HRESULT result;					//関数の成功か失敗か確認用

	result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(result))return false;

	//AudioClass2インターフェイス作成
	unsigned XAudio2CreateFlags = 0;
	#if _DEBUG
	XAudio2CreateFlags |= XAUDIO2_DEBUG_ENGINE;
	#endif

	//XAudio初期化
	result = XAudio2Create(&m_xaudio2, XAudio2CreateFlags, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(result))return false;

	//マスターボイス作成
	result = m_xaudio2->CreateMasteringVoice(&m_mastering_voice);
	if (FAILED(result))
	{
		//音楽出力ができない(スピーカーが無い)ならフラグをOFFにする
		if (result == XAUDIO2_E_INVALID_CALL)
			m_speaker_flag = false;
		else
			return false;
	}	

	return true;

}

void AudioClass::Delete()
{
	if (m_source_voice_list)
	{
		//ソースボイス,データの破棄
		for (auto itr = m_source_voice_list->begin(); itr != m_source_voice_list->end(); itr++)
		{
			if (itr->get()->source_voice != nullptr)
				itr->get()->source_voice->DestroyVoice();
			delete itr->get()->data;
		}

		m_source_voice_list->clear();
		delete m_source_voice_list;
	}
	//マスターボイス破棄
	if(m_mastering_voice)m_mastering_voice->DestroyVoice();
	//XAudioインターフェイス破棄
	if (m_xaudio2) m_xaudio2->Release();
	CoUninitialize();

}

//音楽データの登録
bool AudioClass::LoodFile(const wchar_t* file_name, int id,bool loop_flag)
{
	//既に登録しているものに同じIDがあれば終了
	for (auto itr = m_source_voice_list->begin(); itr != m_source_voice_list->end(); itr++)
	{
		if (itr->get()->id == id)
			return false;
	}
	//リストに追加するデータ
	unique_ptr<AudioData> add_audio_data(new AudioData);

	//id設定
	add_audio_data.get()->id = id;
	//バッファのAudioBytes・pAudioData以外を設定
	//https://msdn.microsoft.com/ja-jp/library/bb633483(v=vs.85).aspx
	add_audio_data.get()->buffer.Flags = XAUDIO2_END_OF_STREAM;	//オーディオバッファに関する追加情報を提供するフラグ
	add_audio_data.get()->buffer.PlayBegin = 0;		//再生される、バッファ内の最初のサンプル
	add_audio_data.get()->buffer.PlayLength = 0;	//再生される領域の長さ(サンプル数単位)。値が０の場合、バッファ全体が再生される。この場合がはPlayerBeginを０にする必要がある
	add_audio_data.get()->buffer.pContext = nullptr;//コールバックでクライアントに返されるコンテキスト値。NULLでもOK
	//ループフラグがONならループするバッファを生成する
	if (loop_flag)
	{
		add_audio_data.get()->buffer.LoopBegin = 0;	//ループされる領域の最初のサンプル
		add_audio_data.get()->buffer.LoopLength = 0;//ループ領域の長さ(サンプル数単位)
		add_audio_data.get()->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;	//ループ領域を通してループする回数
	}
	else
	{
		add_audio_data.get()->buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;
		add_audio_data.get()->buffer.LoopLength = 0;
		add_audio_data.get()->buffer.LoopCount = 0;
	}


	
	//音楽データのフォーマット
	WAVEFORMATEX format;
	//形式の名前情報
	const wchar_t* format_name;
	//形式の名前情報を取得(「.」を含む)
	format_name = wcschr(file_name,L'.');

	//形式による読み込みを行う
	if (wcscmp(&format_name[1], L"wav") == 0) 
	{
		if (!LoodWaveFile(file_name, add_audio_data.get()->data, &add_audio_data.get()->buffer.AudioBytes, &format))
			return false;
	}
	else if (wcscmp(&format_name[1], L"ogg") == 0) 
	{
		if (!LoodOggFile(file_name, add_audio_data.get()->data, &add_audio_data.get()->buffer.AudioBytes, &format))
			return false;
	}
	else
		//読み込めない形式
		return false;

	//バッファのデータを設定
	add_audio_data.get()->buffer.pAudioData = add_audio_data.get()->data;

	//フォーマットの残りを設定
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize = 0;

	add_audio_data.get()->source_voice = nullptr;
	//音楽が出力できる状態であればソースボイス設定・バッファを作成する
	if (m_speaker_flag)
	{
		//読み込んだ音楽ファイルをソースボイスに設定
		m_xaudio2->CreateSourceVoice(&add_audio_data.get()->source_voice, &format);
		//バッファを設定
		add_audio_data.get()->source_voice->SubmitSourceBuffer(&add_audio_data.get()->buffer);
	}

	//リストに追加
	m_source_voice_list->push_back(move(add_audio_data));

	//読み込み成功
	return true;
}

//音楽再生
bool AudioClass::Start(int id)
{
	for (auto itr = m_source_voice_list->begin(); itr != m_source_voice_list->end(); itr++)
	{
		if (itr->get()->id == id)
		{
			if (itr->get()->source_voice != nullptr)
			{
				Stop(id);
				itr->get()->source_voice->Start();
				return true;
			}
		}
	}
	return false;
}

//音楽停止
bool AudioClass::Stop(int id)
{
	for (auto itr = m_source_voice_list->begin(); itr != m_source_voice_list->end(); itr++)
	{
		if (itr->get()->id == id)
		{
			if (itr->get()->source_voice != nullptr)
			{
				itr->get()->source_voice->Stop();
				itr->get()->source_voice->FlushSourceBuffers();
				itr->get()->source_voice->SubmitSourceBuffer(&itr->get()->buffer);
				return true;
			}
		}
	}
	return false;
}

/* wave形式音楽データの登録
引数1 file_name	:ファイル名
引数2 out_data		:ウェーブデータ(音楽の本データ)　
引数3 out_data_size	:ウェーブデータのサイズ
引数4 out_format	:音楽データのフォーマット(nChannels・wBitsPerSample・nSamplesPerSecだけを設定すればよい)
http://norte.coron.jp/sound/wav01.html
*/
bool AudioClass::LoodWaveFile(const wchar_t* file_name,BYTE*& out_data,UINT32* out_data_size, WAVEFORMATEX* out_format)
{
	//Waveファイルのオープン
	FILE* fp;
	_wfopen_s(&fp,file_name, L"rb");

	BasicWaveHeaderType wave_file_header;//ウェーブファイルのハンドル
	//ファイルの読み込み失敗
	if (fp == nullptr)
		return false;

	//ファイル読み込み
	fread(&wave_file_header, sizeof(wave_file_header), 1,fp);

	//チェックIDがRIFF形式であることを確認
	if ((wave_file_header.chunkld[0] != 'R') || (wave_file_header.chunkld[1] != 'I') ||
		(wave_file_header.chunkld[2] != 'F') || (wave_file_header.chunkld[3] != 'F'))
	{
		fclose(fp);
		return false;
	}
	//ファイル形式がWAVE形式であることを確認
	if ((wave_file_header.format[0] != 'W') || (wave_file_header.format[1] != 'A') ||
		(wave_file_header.format[2] != 'V') || (wave_file_header.format[3] != 'E'))
	{
		fclose(fp);
		return false;
	}

	//サブチャンクIDがfmt形式であることを確認
	if ((wave_file_header.subChunkld[0] != 'f') || (wave_file_header.subChunkld[1] != 'm') ||
		(wave_file_header.subChunkld[2] != 't') || (wave_file_header.subChunkld[3] != ' '))
	{
		fclose(fp);
		return false;
	}

	//オーディオフォーマットがWAVE_FORMAT_PCMであることを確認
	if (wave_file_header.audioFormat != WAVE_FORMAT_PCM)
	{
		fclose(fp);
		return false;
	}
	//waveファイルがステレオフォーマットで記録されたことを確認
	if (wave_file_header.unmChannels != 2)
	{
		fclose(fp);
		return false;
	}
	//ウェーブフォームが44.1KHｚのサンプルグレートで記憶されたことを確認
	if (wave_file_header.sampleRate != 44100)
	{
		fclose(fp);
		return false;
	}
	//waveファイルが16ビット形式で記憶されていることを確認
	if (wave_file_header.bitsPerSample != 16)
	{
		fclose(fp);
		return false;
	}

	out_format->nChannels = wave_file_header.unmChannels;//フォーマットのチャンネル数設定
	out_format->wBitsPerSample = wave_file_header.bitsPerSample;//フォーマットの1サンプルあたりのビット数設定
	out_format->nSamplesPerSec = wave_file_header.sampleRate;//フォーマットのサンプリングレート設定

	//データチェックヘッダーをチェック
	if ((wave_file_header.dataChunkld[0] == 'd') && (wave_file_header.dataChunkld[1] == 'a') &&
		(wave_file_header.dataChunkld[2] == 't') && (wave_file_header.dataChunkld[3] == 'a'))
	{
		//waveが基本型である 動作確認中

		//サイズ情報格納
		*out_data_size = wave_file_header.dataSize;

		//メモリセット
		out_data = new BYTE[*out_data_size];

		fread(out_data, *out_data_size, 1, fp);

	}
	else if ((wave_file_header.dataChunkld[0] == 'L') && (wave_file_header.dataChunkld[1] == 'I') &&
		(wave_file_header.dataChunkld[2] == 'S') && (wave_file_header.dataChunkld[3] == 'T'))
	{
		//waveがLIST型である　動作確認済み

		//必要ないデータ分読み込む
		BYTE garbage;
		for(int i = 0 ; i < 30 ; i++)
			fread(&garbage, 1,1, fp);

		//データのサイズ情報を取得
		fread(out_data_size, sizeof(UINT32), 1, fp);

		//メモリセット
		out_data = new BYTE[*out_data_size];
		
		//データ読み込み
		fread(out_data, *out_data_size, 1, fp);
	}
	else
	{
		//予想外の形式である
		return false;
	}

	//ファイルを閉じる
	fclose(fp);
	return true;
}

//Ogg形式音楽データの登録
bool AudioClass::LoodOggFile(const wchar_t* file_name, BYTE*& out_data, UINT32* out_data_size, WAVEFORMATEX* out_format)
{
	//Oggファイルのオープン
	FILE* fp;
	_wfopen_s(&fp, file_name, L"rd");

	//ファイルの読み込み失敗
	if (fp == nullptr)
		return false;

	fclose(fp);
	return true;
}