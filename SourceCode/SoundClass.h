#pragma once

//リンク
#pragma comment(lib, "dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

//システム系のヘッダーインクルード
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass
{
private:

	//.wavファイル形式読み込み用
	struct WaveHeaderType
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
public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND hwnd,char* filename);
	void Shutdown();

	//読み込まれた音楽を再生
	bool PlayWaveFile();
private:
	//DirectSoundAPIとプライマリサウンドバッファを初期化
	bool InitializeDirectSound(HWND);
	//DirectSoundAPIとプライマリサウンドバッファを解放
	void ShutdownDirectSound();

	//waveオーディオファイルをセカンダリバッファにロード
	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	//セカンダリバッファを解放
	void ShutdownWaveFile(IDirectSoundBuffer8**);

private:
	//バッファ オブジェクトの作成、デバイスの管理、環境の設定に使う。
	IDirectSound8* m_direct_sound;
	//プライマリバッファ
	IDirectSoundBuffer* m_primary_buffer;

	//個々のサウンドを格納して再生するための変数
	IDirectSoundBuffer8* m_secondary_buffer;
};