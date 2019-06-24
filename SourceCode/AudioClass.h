#pragma once

#include <XAudio2.h>
#include <list>
#include <memory>

using namespace std;
 
//���y�f�[�^�i�[�p
struct AudioData
{
	IXAudio2SourceVoice* source_voice;	//�Đ��E�ꎞ��~�Ȃǂ��s��
	XAUDIO2_BUFFER buffer;				//�t�@�C���f�[�^����̏��(��~�p)
	BYTE* data;						//���y�f�[�^
	int id;							//�o�^�ԍ�(ID)
}; 

class AudioClass
{
public :
	AudioClass();
	bool Initialize();	//�������֐�
	void Delete();	//�폜�֐�
	bool LoodFile(const wchar_t* file_name,int id, bool loop_flag =false);	//���y�f�[�^�̓o�^
	bool Start(int id);	//���y�Đ�
	bool Stop(int id);	//���y��~
private:
	/* wave�`�����y�f�[�^�̓o�^
	����1 file_name	:�t�@�C����
	����2 out_data		:�E�F�[�u�f�[�^(���y�̖{�f�[�^)�@
	����3 out_data_size	:�E�F�[�u�f�[�^�̃T�C�Y
	����4 out_format	:���y�f�[�^�̃t�H�[�}�b�g(nChannels�EwBitsPerSample�EnSamplesPerSec������ݒ肷��΂悢)
	wave �o�C�i��//http://norte.coron.jp/sound/wav01.html
	*/
	bool LoodWaveFile(const wchar_t* file_name, BYTE*& out_data, UINT32* out_data_size, WAVEFORMATEX* out_format);
	//Ogg�`�����y�f�[�^�̓o�^s
	//������LoodWaveFile�Ɠ���
	bool LoodOggFile(const wchar_t* file_name, BYTE*& out_data, UINT32* out_data_size, WAVEFORMATEX* out_format);//Ogg�`�����y�f�[�^�̓o�^

private:
	//��{�^.wav�t�@�C���`���ǂݍ��ݗp
	struct BasicWaveHeaderType
	{
		char			chunkld[4];		//�`�����(RIFF�t�@�C���ł��邱��)
		unsigned long	chunkSize;		//�t�@�C���T�C�Y�i�o�C�g�j
		char			format[4];		//WAVE�w�b�_(RIFF�`����WAVE�t�@�C���ł��邱�Ƃ�\��������)
		char			subChunkld[4];	//�t�H�[�}�b�g�`�����N(fmt�`�����N�̒�`)
		unsigned long	sudChunkSize;	//�t�H�[�}�b�g�̒�`�֘A�̃f�[�^�T�C�Y
		unsigned short	audioFormat;	//���k�̃t�H�[�}�b�gID
		unsigned short	unmChannels;	//�`�����l����
		unsigned long	sampleRate;		//�T���v���O���[�g
		unsigned long	butesPerSecond;	//�f�[�^���M���x(�o�C�g)
		unsigned short	blockAlign;		//�T���v��������̃o�C�g��
		unsigned short	bitsPerSample;	//�ʎq���r�b�g��
		char			dataChunkld[4];	//data�w�b�_(RIFF�`��WAVE�t�@�C����data)
		unsigned long	dataSize;		//�f�[�^�`�����N�̃T�C�Y(�o�C�g)
	};
private:
	IXAudio2* m_xaudio2;						//XAudio2�C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_mastering_voice;	//�}�X�^�[�{�C�X
	list<unique_ptr<AudioData>>* m_source_voice_list;	//���y�f�[�^���X�g
	bool m_speaker_flag;			//���y�o�͂ł����Ԃ��ۂ�
};