#pragma once

//�����N
#pragma comment(lib, "dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass
{
private:

	//.wav�t�@�C���`���ǂݍ��ݗp
	struct WaveHeaderType
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
public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND hwnd,char* filename);
	void Shutdown();

	//�ǂݍ��܂ꂽ���y���Đ�
	bool PlayWaveFile();
private:
	//DirectSoundAPI�ƃv���C�}���T�E���h�o�b�t�@��������
	bool InitializeDirectSound(HWND);
	//DirectSoundAPI�ƃv���C�}���T�E���h�o�b�t�@�����
	void ShutdownDirectSound();

	//wave�I�[�f�B�I�t�@�C�����Z�J���_���o�b�t�@�Ƀ��[�h
	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	//�Z�J���_���o�b�t�@�����
	void ShutdownWaveFile(IDirectSoundBuffer8**);

private:
	//�o�b�t�@ �I�u�W�F�N�g�̍쐬�A�f�o�C�X�̊Ǘ��A���̐ݒ�Ɏg���B
	IDirectSound8* m_direct_sound;
	//�v���C�}���o�b�t�@
	IDirectSoundBuffer* m_primary_buffer;

	//�X�̃T�E���h���i�[���čĐ����邽�߂̕ϐ�
	IDirectSoundBuffer8* m_secondary_buffer;
};