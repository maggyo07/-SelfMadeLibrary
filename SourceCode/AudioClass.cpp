#include "AudioClass.h"

AudioClass::AudioClass()	:m_mastering_voice(nullptr),m_speaker_flag(true)
							,m_source_voice_list(new list<unique_ptr<AudioData>>)
{

}
bool AudioClass::Initialize()
{
	HRESULT result;					//�֐��̐��������s���m�F�p

	result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(result))return false;

	//AudioClass2�C���^�[�t�F�C�X�쐬
	unsigned XAudio2CreateFlags = 0;
	#if _DEBUG
	XAudio2CreateFlags |= XAUDIO2_DEBUG_ENGINE;
	#endif

	//XAudio������
	result = XAudio2Create(&m_xaudio2, XAudio2CreateFlags, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(result))return false;

	//�}�X�^�[�{�C�X�쐬
	result = m_xaudio2->CreateMasteringVoice(&m_mastering_voice);
	if (FAILED(result))
	{
		//���y�o�͂��ł��Ȃ�(�X�s�[�J�[������)�Ȃ�t���O��OFF�ɂ���
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
		//�\�[�X�{�C�X,�f�[�^�̔j��
		for (auto itr = m_source_voice_list->begin(); itr != m_source_voice_list->end(); itr++)
		{
			if (itr->get()->source_voice != nullptr)
				itr->get()->source_voice->DestroyVoice();
			delete itr->get()->data;
		}

		m_source_voice_list->clear();
		delete m_source_voice_list;
	}
	//�}�X�^�[�{�C�X�j��
	if(m_mastering_voice)m_mastering_voice->DestroyVoice();
	//XAudio�C���^�[�t�F�C�X�j��
	if (m_xaudio2) m_xaudio2->Release();
	CoUninitialize();

}

//���y�f�[�^�̓o�^
bool AudioClass::LoodFile(const wchar_t* file_name, int id,bool loop_flag)
{
	//���ɓo�^���Ă�����̂ɓ���ID������ΏI��
	for (auto itr = m_source_voice_list->begin(); itr != m_source_voice_list->end(); itr++)
	{
		if (itr->get()->id == id)
			return false;
	}
	//���X�g�ɒǉ�����f�[�^
	unique_ptr<AudioData> add_audio_data(new AudioData);

	//id�ݒ�
	add_audio_data.get()->id = id;
	//�o�b�t�@��AudioBytes�EpAudioData�ȊO��ݒ�
	//https://msdn.microsoft.com/ja-jp/library/bb633483(v=vs.85).aspx
	add_audio_data.get()->buffer.Flags = XAUDIO2_END_OF_STREAM;	//�I�[�f�B�I�o�b�t�@�Ɋւ���ǉ�����񋟂���t���O
	add_audio_data.get()->buffer.PlayBegin = 0;		//�Đ������A�o�b�t�@���̍ŏ��̃T���v��
	add_audio_data.get()->buffer.PlayLength = 0;	//�Đ������̈�̒���(�T���v�����P��)�B�l���O�̏ꍇ�A�o�b�t�@�S�̂��Đ������B���̏ꍇ����PlayerBegin���O�ɂ���K�v������
	add_audio_data.get()->buffer.pContext = nullptr;//�R�[���o�b�N�ŃN���C�A���g�ɕԂ����R���e�L�X�g�l�BNULL�ł�OK
	//���[�v�t���O��ON�Ȃ烋�[�v����o�b�t�@�𐶐�����
	if (loop_flag)
	{
		add_audio_data.get()->buffer.LoopBegin = 0;	//���[�v�����̈�̍ŏ��̃T���v��
		add_audio_data.get()->buffer.LoopLength = 0;//���[�v�̈�̒���(�T���v�����P��)
		add_audio_data.get()->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;	//���[�v�̈��ʂ��ă��[�v�����
	}
	else
	{
		add_audio_data.get()->buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;
		add_audio_data.get()->buffer.LoopLength = 0;
		add_audio_data.get()->buffer.LoopCount = 0;
	}


	
	//���y�f�[�^�̃t�H�[�}�b�g
	WAVEFORMATEX format;
	//�`���̖��O���
	const wchar_t* format_name;
	//�`���̖��O�����擾(�u.�v���܂�)
	format_name = wcschr(file_name,L'.');

	//�`���ɂ��ǂݍ��݂��s��
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
		//�ǂݍ��߂Ȃ��`��
		return false;

	//�o�b�t�@�̃f�[�^��ݒ�
	add_audio_data.get()->buffer.pAudioData = add_audio_data.get()->data;

	//�t�H�[�}�b�g�̎c���ݒ�
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize = 0;

	add_audio_data.get()->source_voice = nullptr;
	//���y���o�͂ł����Ԃł���΃\�[�X�{�C�X�ݒ�E�o�b�t�@���쐬����
	if (m_speaker_flag)
	{
		//�ǂݍ��񂾉��y�t�@�C�����\�[�X�{�C�X�ɐݒ�
		m_xaudio2->CreateSourceVoice(&add_audio_data.get()->source_voice, &format);
		//�o�b�t�@��ݒ�
		add_audio_data.get()->source_voice->SubmitSourceBuffer(&add_audio_data.get()->buffer);
	}

	//���X�g�ɒǉ�
	m_source_voice_list->push_back(move(add_audio_data));

	//�ǂݍ��ݐ���
	return true;
}

//���y�Đ�
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

//���y��~
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

/* wave�`�����y�f�[�^�̓o�^
����1 file_name	:�t�@�C����
����2 out_data		:�E�F�[�u�f�[�^(���y�̖{�f�[�^)�@
����3 out_data_size	:�E�F�[�u�f�[�^�̃T�C�Y
����4 out_format	:���y�f�[�^�̃t�H�[�}�b�g(nChannels�EwBitsPerSample�EnSamplesPerSec������ݒ肷��΂悢)
http://norte.coron.jp/sound/wav01.html
*/
bool AudioClass::LoodWaveFile(const wchar_t* file_name,BYTE*& out_data,UINT32* out_data_size, WAVEFORMATEX* out_format)
{
	//Wave�t�@�C���̃I�[�v��
	FILE* fp;
	_wfopen_s(&fp,file_name, L"rb");

	BasicWaveHeaderType wave_file_header;//�E�F�[�u�t�@�C���̃n���h��
	//�t�@�C���̓ǂݍ��ݎ��s
	if (fp == nullptr)
		return false;

	//�t�@�C���ǂݍ���
	fread(&wave_file_header, sizeof(wave_file_header), 1,fp);

	//�`�F�b�NID��RIFF�`���ł��邱�Ƃ��m�F
	if ((wave_file_header.chunkld[0] != 'R') || (wave_file_header.chunkld[1] != 'I') ||
		(wave_file_header.chunkld[2] != 'F') || (wave_file_header.chunkld[3] != 'F'))
	{
		fclose(fp);
		return false;
	}
	//�t�@�C���`����WAVE�`���ł��邱�Ƃ��m�F
	if ((wave_file_header.format[0] != 'W') || (wave_file_header.format[1] != 'A') ||
		(wave_file_header.format[2] != 'V') || (wave_file_header.format[3] != 'E'))
	{
		fclose(fp);
		return false;
	}

	//�T�u�`�����NID��fmt�`���ł��邱�Ƃ��m�F
	if ((wave_file_header.subChunkld[0] != 'f') || (wave_file_header.subChunkld[1] != 'm') ||
		(wave_file_header.subChunkld[2] != 't') || (wave_file_header.subChunkld[3] != ' '))
	{
		fclose(fp);
		return false;
	}

	//�I�[�f�B�I�t�H�[�}�b�g��WAVE_FORMAT_PCM�ł��邱�Ƃ��m�F
	if (wave_file_header.audioFormat != WAVE_FORMAT_PCM)
	{
		fclose(fp);
		return false;
	}
	//wave�t�@�C�����X�e���I�t�H�[�}�b�g�ŋL�^���ꂽ���Ƃ��m�F
	if (wave_file_header.unmChannels != 2)
	{
		fclose(fp);
		return false;
	}
	//�E�F�[�u�t�H�[����44.1KH���̃T���v���O���[�g�ŋL�����ꂽ���Ƃ��m�F
	if (wave_file_header.sampleRate != 44100)
	{
		fclose(fp);
		return false;
	}
	//wave�t�@�C����16�r�b�g�`���ŋL������Ă��邱�Ƃ��m�F
	if (wave_file_header.bitsPerSample != 16)
	{
		fclose(fp);
		return false;
	}

	out_format->nChannels = wave_file_header.unmChannels;//�t�H�[�}�b�g�̃`�����l�����ݒ�
	out_format->wBitsPerSample = wave_file_header.bitsPerSample;//�t�H�[�}�b�g��1�T���v��������̃r�b�g���ݒ�
	out_format->nSamplesPerSec = wave_file_header.sampleRate;//�t�H�[�}�b�g�̃T���v�����O���[�g�ݒ�

	//�f�[�^�`�F�b�N�w�b�_�[���`�F�b�N
	if ((wave_file_header.dataChunkld[0] == 'd') && (wave_file_header.dataChunkld[1] == 'a') &&
		(wave_file_header.dataChunkld[2] == 't') && (wave_file_header.dataChunkld[3] == 'a'))
	{
		//wave����{�^�ł��� ����m�F��

		//�T�C�Y���i�[
		*out_data_size = wave_file_header.dataSize;

		//�������Z�b�g
		out_data = new BYTE[*out_data_size];

		fread(out_data, *out_data_size, 1, fp);

	}
	else if ((wave_file_header.dataChunkld[0] == 'L') && (wave_file_header.dataChunkld[1] == 'I') &&
		(wave_file_header.dataChunkld[2] == 'S') && (wave_file_header.dataChunkld[3] == 'T'))
	{
		//wave��LIST�^�ł���@����m�F�ς�

		//�K�v�Ȃ��f�[�^���ǂݍ���
		BYTE garbage;
		for(int i = 0 ; i < 30 ; i++)
			fread(&garbage, 1,1, fp);

		//�f�[�^�̃T�C�Y�����擾
		fread(out_data_size, sizeof(UINT32), 1, fp);

		//�������Z�b�g
		out_data = new BYTE[*out_data_size];
		
		//�f�[�^�ǂݍ���
		fread(out_data, *out_data_size, 1, fp);
	}
	else
	{
		//�\�z�O�̌`���ł���
		return false;
	}

	//�t�@�C�������
	fclose(fp);
	return true;
}

//Ogg�`�����y�f�[�^�̓o�^
bool AudioClass::LoodOggFile(const wchar_t* file_name, BYTE*& out_data, UINT32* out_data_size, WAVEFORMATEX* out_format)
{
	//Ogg�t�@�C���̃I�[�v��
	FILE* fp;
	_wfopen_s(&fp, file_name, L"rd");

	//�t�@�C���̓ǂݍ��ݎ��s
	if (fp == nullptr)
		return false;

	fclose(fp);
	return true;
}