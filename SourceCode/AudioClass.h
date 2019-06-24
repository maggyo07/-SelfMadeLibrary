#pragma once

#include <XAudio2.h>
#include <list>
#include <memory>

using namespace std;
 
//音楽データ格納用
struct AudioData
{
	IXAudio2SourceVoice* source_voice;	//再生・一時停止などを行う
	XAUDIO2_BUFFER buffer;				//ファイルデータからの情報(停止用)
	BYTE* data;						//音楽データ
	int id;							//登録番号(ID)
}; 

class AudioClass
{
public :
	AudioClass();
	bool Initialize();	//初期化関数
	void Delete();	//削除関数
	bool LoodFile(const wchar_t* file_name,int id, bool loop_flag =false);	//音楽データの登録
	bool Start(int id);	//音楽再生
	bool Stop(int id);	//音楽停止
private:
	/* wave形式音楽データの登録
	引数1 file_name	:ファイル名
	引数2 out_data		:ウェーブデータ(音楽の本データ)　
	引数3 out_data_size	:ウェーブデータのサイズ
	引数4 out_format	:音楽データのフォーマット(nChannels・wBitsPerSample・nSamplesPerSecだけを設定すればよい)
	wave バイナリ//http://norte.coron.jp/sound/wav01.html
	*/
	bool LoodWaveFile(const wchar_t* file_name, BYTE*& out_data, UINT32* out_data_size, WAVEFORMATEX* out_format);
	//Ogg形式音楽データの登録s
	//引数はLoodWaveFileと同じ
	bool LoodOggFile(const wchar_t* file_name, BYTE*& out_data, UINT32* out_data_size, WAVEFORMATEX* out_format);//Ogg形式音楽データの登録

private:
	//基本型.wavファイル形式読み込み用
	struct BasicWaveHeaderType
	{
		char			chunkld[4];		//形式情報(RIFFファイルであること)
		unsigned long	chunkSize;		//ファイルサイズ（バイト）
		char			format[4];		//WAVEヘッダ(RIFF形式のWAVEファイルであることを表す文字列)
		char			subChunkld[4];	//フォーマットチャンク(fmtチャンクの定義)
		unsigned long	sudChunkSize;	//フォーマットの定義関連のデータサイズ
		unsigned short	audioFormat;	//圧縮のフォーマットID
		unsigned short	unmChannels;	//チャンネル数
		unsigned long	sampleRate;		//サンプルグレート
		unsigned long	butesPerSecond;	//データ送信速度(バイト)
		unsigned short	blockAlign;		//サンプル当たりのバイト数
		unsigned short	bitsPerSample;	//量子化ビット数
		char			dataChunkld[4];	//dataヘッダ(RIFF形式WAVEファイルのdata)
		unsigned long	dataSize;		//データチャンクのサイズ(バイト)
	};
private:
	IXAudio2* m_xaudio2;						//XAudio2インターフェイス
	IXAudio2MasteringVoice* m_mastering_voice;	//マスターボイス
	list<unique_ptr<AudioData>>* m_source_voice_list;	//音楽データリスト
	bool m_speaker_flag;			//音楽出力できる状態か否か
};