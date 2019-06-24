

//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
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
	bool result;	//���ʏ��i�[�p

	//DirectSoundAPI�ƃv���C�}���T�E���h�o�b�t�@��������
	result = InitializeDirectSound(hwnd);
	if (!result)
		return false;

	//wave�I�[�f�B�I�t�@�C�����Z�J���_���o�b�t�@�Ƀ��[�h����
	result = LoadWaveFile(filename, &m_secondary_buffer);
	if (!result)
		return false;
}

void SoundClass::Shutdown()
{
	//�Z�J���_���o�b�t�@�����
	ShutdownWaveFile(&m_secondary_buffer);

	//DirectSoundAPI���V���b�g�_�E��
	ShutdownDirectSound();

	return;
}

//DirectSoundAPI�ƃv���C�}���T�E���h�o�b�t�@��������
bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;	//���ʏ��i�[�p
	DSBUFFERDESC buffer_desc;
	WAVEFORMATEX wave_format;

	//�f�t�H���g�̃T�E���h�f�o�C�X�̃_�C���N�g�T�E���h�C���^�[�t�F�C�X�|�C���^��������
	result = DirectSoundCreate8(NULL, &m_direct_sound, NULL);
	if (FAILED(result))
		return false;

	//�v���C�}���T�E���h�o�b�t�@�̃t�H�[�}�b�g��ύX�ł���悤�ɁA�������x�����v���C�I���e�B�ɐݒ�
	result = m_direct_sound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;

	//�v���C�}���o�b�t�@�̐�����ݒ�
	buffer_desc.dwSize			= sizeof(DSBUFFERDESC);	//�\���̂̃T�C�Y
	buffer_desc.dwFlags			= DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;	//�o�b�t�@�̔\��
	buffer_desc.dwBufferBytes	= 0;					//�V�����o�b�t�@�T�C�Y�@DSBCAPS_PRIMARYBUFFER���g�p���Ă���ꍇ�͂O
	buffer_desc.dwReserved		= 0;					//�\��ς݁B�O�łȂ���΂Ȃ�Ȃ�
	buffer_desc.lpwfxFormat		= NULL;					//�o�b�t�@�̃E�F�[�u�t�H�[���t�H�[�}�b�g
	buffer_desc.guid3DAlgorithm = GUID_NULL;			//DirectSound3D �̃n�[�h�E�F�A �G�~�����[�V�������g�� 2 �X�s�[�J���z���A���S���Y���̈�ӂȎ��ʎq�B

	//�f�t�H���g�T�E���h�f�o�C�X�̃v���C�}���T�E���h�o�b�t�@�̃R���g���[�����擾
	result = m_direct_sound->CreateSoundBuffer(&buffer_desc, &m_primary_buffer, NULL);
	if (FAILED(result))
		return false;

	//�v���C�}���T�E���h�o�b�t�@�̃t�H�[�}�b�g��ݒ�
	//���̏ꍇ�A16�r�b�g�X�e���I(cd�I�[�f�B�I�`��)��44100�T���v��/�a�ŋL�^���ꂽWAV�t�@�C��
	wave_format.wFormatTag		= WAVE_FORMAT_PCM;	//�E�F�[�u�t�H�[���I�[�f�B�I�t�H�[�}�b�g�̃^�C�v
	wave_format.nSamplesPerSec	= 44100;			//�W�{����(Hz)
	wave_format.wBitsPerSample	= 16;				//�ʎq����(�o�C�g)
	wave_format.nChannels		= 2;				//�`�F���l����
	wave_format.nBlockAlign		= (wave_format.wBitsPerSample / 8) * wave_format.nChannels;	//wFormatTag�̃f�[�^�̍ŏ��P��(�o�C�g�P��)
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;		//�K�v�ȕ��σf�[�^�]�����x(�P�b������̃o�C�g��)
	wave_format.cbSize			= 0;				//WAVEFORMATEX �\���̂̌��ɒǉ������t�H�[�}�b�g���T�C�Y(�o�C�g)wFormatTag�̒ǉ��������i�[�ł���

	//�v���C�}���o�b�t�@���w�肳�ꂽ�g�^�`���ɐݒ�
	result = m_primary_buffer->SetFormat(&wave_format);
	if (FAILED(result))
		return false;

	return true;
}

//DirectSoundAPI�ƃv���C�}���T�E���h�o�b�t�@�����
void SoundClass::ShutdownDirectSound()
{
	//�v���C�}���T�E���h�o�b�t�@�|�C���^�����
	if (m_primary_buffer)
	{
		m_primary_buffer->Release();
		m_primary_buffer = nullptr;
	}

	//�_�C���N�g�T�E���h�C���^�[�t�F�C�X�|�C���^�����
	if (m_direct_sound)
	{
		m_direct_sound->Release();
		m_direct_sound = nullptr;
	}
}

//wave�I�[�f�B�I�t�@�C�����Z�J���_���o�b�t�@�Ƀ��[�h
bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;						//�����������ǂ����m�F�p
	FILE* file_ptr;					//�I�[�f�B�I�t�@�C���ǂݍ��ݗp
	unsigned int count;				//�����������ǂ����m�F�p
	WaveHeaderType wave_file_header;//�E�F�[�u�t�@�C���̃n���h��
	WAVEFORMATEX wave_format;		//�E�F�[�u�t�H�[���t�H�[�}�b�g
	DSBUFFERDESC buffer_desc;		//�Z�J���_���T�E���h�o�b�t�@�̃o�b�t�@�L�q
	HRESULT result;					//�֐��̐��������s���m�F�p
	IDirectSoundBuffer* temp_buffer;//�Z�J���_���o�b�t�@���쐬�p�̈ꎞ�o�b�t�@
	unsigned char* wave_data;		//�E�F�[�u�t�@�C���f�[�^
	unsigned char* buffer_ptr;		//�Z�J���_���o�b�t�@�ɃE�F�[�u�f�[�^�������ݗp
	unsigned long buffer_size;		//�Z�J���_���o�b�t�@�ɃE�F�[�u�f�[�^�������ݗp

	//�o�C�i����wave�t�@�C�����J��
	error = fopen_s(&file_ptr, filename, "rb");
	if (error != 0)
		return false;

	//wave�t�@�C���̃w�b�_�[��ǂݍ���
	count = fread(&wave_file_header, sizeof(wave_file_header), 1, file_ptr);
	if (count != 1)
		return false;

	//�`�F�b�NID��RIFF�`���ł��邱�Ƃ��m�F
	if ((wave_file_header.chunkld[0] != 'R') || (wave_file_header.chunkld[1] != 'I') ||
		(wave_file_header.chunkld[2] != 'F') || (wave_file_header.chunkld[3] != 'F'))
		return false;

	//�t�@�C���`����WAVE�`���ł��邱�Ƃ��m�F
	if ((wave_file_header.format[0] != 'W') || (wave_file_header.format[1] != 'A') ||
		(wave_file_header.format[2] != 'V') || (wave_file_header.format[3] != 'E'))
		return false;

	//�T�u�`�����NID��fmt�`���ł��邱�Ƃ��m�F
	if ((wave_file_header.subChunkld[0] != 'f') || (wave_file_header.subChunkld[1] != 'm') ||
		(wave_file_header.subChunkld[2] != 't') || (wave_file_header.subChunkld[3] != ' '))
		return false;

	//�I�[�f�B�I�t�H�[�}�b�g��WAVE_FORMAT_PCM�ł��邱�Ƃ��m�F
	if (wave_file_header.audioFormat != WAVE_FORMAT_PCM)
		return false;

	//wave�t�@�C�����X�e���I�t�H�[�}�b�g�ŋL�^���ꂽ���Ƃ��m�F
	if (wave_file_header.unmChannels != 2)
		return false;

	//�E�F�[�u�t�H�[����44.1KH���̃T���v���O���[�g�ŋL�����ꂽ���Ƃ��m�F
	if (wave_file_header.sampleRate != 44100)
		return false;

	//wave�t�@�C����16�r�b�g�`���ŋL������Ă��邱�Ƃ��m�F
	if (wave_file_header.bitsPerSample != 16)
		return false;

	//�f�[�^�`�F�b�N�w�b�_�[���`�F�b�N
	/*if ((wave_file_header.dataChunkld[0] != 'd') || (wave_file_header.dataChunkld[1] != 'a') ||
		(wave_file_header.dataChunkld[2] != 't') || (wave_file_header.dataChunkld[3] != 'a'))
		return false;*/

	//����wave�t�@�C�������[�h�����Z�J���_���o�b�t�@�̃E�F�[�u�t�H�[���t�H�[�}�b�g��ݒ�
	wave_format.wFormatTag = WAVE_FORMAT_PCM;											//�t�H�[�}�b�g�@�^�O�@PCM�`����WAVE_FORMAT_PCM
	wave_format.nSamplesPerSec = 44100;													//�W�{�����g��
	wave_format.wBitsPerSample = 16;													//�ʎq���r�b�g��
	wave_format.nChannels = 2;															//�`�F���l�����@�P�`�F���l���Ȃ烂�m�����@�Q�`�����l���Ȃ�X�e���I
	wave_format.nBlockAlign = (wave_format.wBitsPerSample / 8) * wave_format.nChannels;	//��FormatTag�̃f�[�^�̍ŏ��P�ʁB�@�v�Z���@�͂�FormatTag�ɂ���Č��܂��Ă���
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;	//�t�H�[�}�b�g �^�O�ŕK�v�ȕ��σf�[�^�]�����x�B�P�ʂ́A1 �b������̃o�C�g���B�@�v�Z���@�͂�FormatTag�ɂ���Č��܂��Ă���
	wave_format.cbSize = 0;																//WAVEFORMATEX �\���̂̌��ɒǉ������t�H�[�}�b�g���̃T�C�Y (�o�C�g�P��)�B

	//�E�F�[�u�t�@�C�������[�h�����Z�J���_���T�E���h�o�b�t�@�̃o�b�t�@�L�q��ݒ�
	buffer_desc.dwSize = sizeof(DSBUFFERDESC);				//�\���̂̃T�C�Y
	buffer_desc.dwFlags = DSBCAPS_CTRLVOLUME;				//�o�b�t�@�̔\��
	buffer_desc.dwBufferBytes = wave_file_header.dataSize;	//�V�����o�b�t�@�T�C�Y�@DSBCAPS_PRIMARYBUFFER���g�p���Ă���ꍇ�͂O
	buffer_desc.dwReserved = 0;								//�\��ς݁B�O�łȂ���΂Ȃ�Ȃ�
	buffer_desc.lpwfxFormat = &wave_format;					//�o�b�t�@�̃E�F�[�u�t�H�[���t�H�[�}�b�g
	buffer_desc.guid3DAlgorithm = GUID_NULL;				//DirectSound3D �̃n�[�h�E�F�A �G�~�����[�V�������g�� 2 �X�s�[�J���z���A���S���Y���̈�ӂȎ��ʎq�B

	//����̃o�b�t�@�ݒ�ňꎞ�I�ȃT�E���h�o�b�t�@���쐬�B
	result = m_direct_sound->CreateSoundBuffer(&buffer_desc, &temp_buffer, NULL);
	if (FAILED(result))
		return false;

	//�_�C���N�g�T�E���h�W�C���^�[�t�F�C�X�ɑ΂��ăo�b�t�@�t�H�[�}�b�g���e�X�g���A�Z�J���_���o�b�t�@���쐬
	result = temp_buffer->QueryInterface(IID_IDirectSoundBuffer8, (void**) &* secondaryBuffer);
	if (FAILED(result))
		return false;

	//�ꎞ�o�b�t�@�����
	temp_buffer->Release();
	temp_buffer = 0;

	//�f�[�^�`�����N�w�b�_�[�̍Ō�Ŏn�܂�E�F�[�u�f�[�^�̐擪�Ɉړ��B
	fseek(file_ptr, sizeof(WaveHeaderType), SEEK_SET);

	//�E�F�[�u�t�@�C���f�[�^��ێ����邽�߂̈ꎞ�o�b�t�@���쐬
	wave_data = new unsigned char[wave_file_header.dataSize];
	if (!wave_data)
		return false;

	//�E�F�[�u�t�@�C���̃f�[�^��V�����쐬�����o�b�t�@�ɓǂݍ���
	count = fread(wave_data, 1, wave_file_header.dataSize, file_ptr);
	if (count != wave_file_header.dataSize)
		return false;

	//�ǂݏI�������t�@�C�������
	error = fclose(file_ptr);
	if (error != 0)
		return false;

	//�Z�J���_���o�b�t�@�����b�N���āA�E�F�[�u�f�[�^����������
	result = (*secondaryBuffer)->Lock(0, wave_file_header.dataSize, (void**)&buffer_ptr, (DWORD *)&buffer_size, NULL, 0, 0);
	if (FAILED(result))
		return false;

	//�E�F�[�u�f�[�^���o�b�t�@�ɃR�s�[
	result = (*secondaryBuffer)->Unlock((void*)buffer_ptr, buffer_size, NULL, 0);
	if (FAILED(result))
		return false;

	//�Z�J���_���o�b�t�@�ɃR�s�[���ꂽ�̂ŁA�E�F�[�u�f�[�^�����
	delete[] wave_data;
	wave_data = 0;

	return true;
 
}

//�Z�J���_���o�b�t�@�����
void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	//�Z�J���_���T�E���h�o�b�t�@�����
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

//�ǂݍ��܂ꂽ���y���Đ�
bool SoundClass::PlayWaveFile()
{
	HRESULT result;	//����

	//�T�E���h�o�b�t�@�̐擪�Ɉʒu��ݒ�
	result = m_secondary_buffer->SetCurrentPosition(0);
	if (FAILED(result))
		return false;

	//�o�b�t�@�̃{�����[����100%�ɐݒ�
	result = m_secondary_buffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
		return false;

	//�Z�J���_���T�E���h�o�b�t�@�̓��e���Đ�
	result = m_secondary_buffer->Play(0, 0, 0);
	if (FAILED(result))
		return false;
	
	return true;

}