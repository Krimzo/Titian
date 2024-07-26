#include "klibrary.h"


kl::AudioDevice::AudioDevice(const UINT id)
    : id(id)
{}

bool kl::AudioDevice::record_audio(Audio* audio, const std::function<bool()>& should_record) const
{
	WAVEFORMATEX wave_format{};
	wave_format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	wave_format.nChannels = 1;
	wave_format.wBitsPerSample = 32;
	wave_format.nSamplesPerSec = audio->sample_rate;
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nChannels * wave_format.wBitsPerSample / 8;
	wave_format.nBlockAlign = wave_format.nChannels * wave_format.wBitsPerSample / 8;

	HWAVEIN wave_in{};
	if (waveInOpen(&wave_in, id, &wave_format, NULL, NULL, WAVE_FORMAT_DIRECT)) {
		return false;
	}

	WAVEHDR wave_in_hdr{};
	wave_in_hdr.lpData = reinterpret_cast<char*>(audio->data());
	wave_in_hdr.dwBufferLength = (DWORD) audio->size() * sizeof(float);
	if (waveInPrepareHeader(wave_in, &wave_in_hdr, sizeof(WAVEHDR))) {
		waveInClose(wave_in);
		return false;
	}
	if (waveInAddBuffer(wave_in, &wave_in_hdr, sizeof(WAVEHDR))) {
		waveInClose(wave_in);
		return false;
	}

	if (waveInStart(wave_in)) {
		waveInClose(wave_in);
		return false;
	}
	while (waveInUnprepareHeader(wave_in, &wave_in_hdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) {
		if (!should_record()) {
			waveInStop(wave_in);
		}
	}

	waveInClose(wave_in);
    return true;
}

bool kl::AudioDevice::play_audio(const Audio& audio, const std::function<bool()>& should_play) const
{
	WAVEFORMATEX wave_format{};
	wave_format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	wave_format.nChannels = 1;
	wave_format.wBitsPerSample = 32;
	wave_format.nSamplesPerSec = audio.sample_rate;
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nChannels * wave_format.wBitsPerSample / 8;
	wave_format.nBlockAlign = wave_format.nChannels * wave_format.wBitsPerSample / 8;

	HWAVEOUT wave_out{};
	if (waveOutOpen(&wave_out, id, &wave_format, NULL, NULL, WAVE_FORMAT_DIRECT)) {
		return false;
	}

	WAVEHDR wave_in_hdr{};
	wave_in_hdr.lpData = reinterpret_cast<char*>(const_cast<float*>(audio.data()));
	wave_in_hdr.dwBufferLength = (DWORD) audio.size() * sizeof(float);
	if (waveOutPrepareHeader(wave_out, &wave_in_hdr, sizeof(WAVEHDR))) {
		waveOutClose(wave_out);
		return false;
	}

	if (waveOutWrite(wave_out, &wave_in_hdr, sizeof(WAVEHDR))) {
		waveOutClose(wave_out);
		return false;
	}
	while (waveOutUnprepareHeader(wave_out, &wave_in_hdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) {
		if (!should_play()) {
			waveOutReset(wave_out);
		}
	}

	waveOutClose(wave_out);
    return true;
}

UINT kl::AudioDevice::input_devices_count()
{
    return waveInGetNumDevs();
}

UINT kl::AudioDevice::output_devices_count()
{
    return waveOutGetNumDevs();
}
