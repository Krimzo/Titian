#include "klibrary.h"


kl::Audio::Audio()
{}

kl::Audio::Audio(const std::string_view& path)
{
	load_from_file(path);
}

// Alter
uint64_t kl::Audio::duration_100ns() const
{
	return (size() * 10'000'000) / sample_rate;
}

float kl::Audio::duration_seconds() const
{
	return size() / (float) sample_rate;
}

void kl::Audio::set_duration(const float duration)
{
	this->resize((size_t) (duration * sample_rate));
}

void kl::Audio::increase_volume(const float amount)
{
	for (auto& sample : *this)
		sample *= amount;
}

void kl::Audio::decrease_volume(const float amount)
{
	const float inv_amount = 1.0f / amount;
	for (auto& sample : *this)
		sample *= inv_amount;
}

// Helper
float kl::Audio::sample_time(const int at_index) const
{
	return at_index / (float) sample_rate;
}

int kl::Audio::sample_index(const float at_time) const
{
	return (int) (at_time * sample_rate);
}

// Decoding
bool kl::Audio::load_from_memory(const byte* data, const uint64_t byte_size)
{
	HRESULT hr = 0;

	ComPtr<IStream> stream = SHCreateMemStream(data, (UINT) byte_size);
	if (!stream) {
		return false;
	}

	ComPtr<IMFByteStream> byte_stream;
	hr = MFCreateMFByteStreamOnStream(stream.Get(), &byte_stream);
	if (FAILED(hr)) {
		return false;
	}

	ComPtr<IMFSourceReader> reader;
	hr = MFCreateSourceReaderFromByteStream(byte_stream.Get(), nullptr, &reader);
	if (FAILED(hr)) {
		return false;
	}

	ComPtr<IMFMediaType> new_media_type;
	hr = MFCreateMediaType(&new_media_type);
	if (FAILED(hr)) {
		return false;
	}
	new_media_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	new_media_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_Float);
	new_media_type->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1);
	new_media_type->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 32);
	new_media_type->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, 4);
	new_media_type->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, sample_rate > 0 ? sample_rate : 48000);
	new_media_type->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, (sample_rate > 0 ? sample_rate : 48000) * sizeof(float));

	hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, new_media_type.Get());
	if (FAILED(hr)) {
		return false;
	}

	ComPtr<IMFMediaType> media_type;
	hr = reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &media_type);
	if (FAILED(hr)) {
		return false;
	}
	media_type->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, (uint32_t*) &sample_rate);

	this->clear();
	while (true) {
		// Read sample
		DWORD flags = NULL;
		LONGLONG time_stamp = 0;
		ComPtr<IMFSample> sample;
		if (FAILED(reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, nullptr, &flags, &time_stamp, &sample)) || !sample) {
			break;
		}

		// Convert to array
		ComPtr<IMFMediaBuffer> media_buffer;
		if (FAILED(sample->ConvertToContiguousBuffer(&media_buffer)) || !media_buffer) {
			break;
		}

		// Copy data
		BYTE* sample_data = nullptr;
		DWORD sample_byte_size = 0;
		media_buffer->Lock(&sample_data, nullptr, &sample_byte_size) >> verify_result;
		const int old_size = (int) this->size();
		this->resize(old_size + (sample_byte_size / sizeof(float)));
		memcpy(this->data() + old_size, sample_data, sample_byte_size);
		media_buffer->Unlock() >> verify_result;
	}
	return true;
}

bool kl::Audio::load_from_vector(const std::vector<byte>& buffer)
{
	return load_from_memory(buffer.data(), buffer.size());
}

bool kl::Audio::load_from_file(const std::string_view& filepath)
{
	const auto file_data = read_file(filepath);
	return load_from_vector(file_data);
}

// Encoding
bool kl::Audio::save_to_vector(std::vector<byte>* buffer, const AudioType type) const
{
	HRESULT hr = 0;

	ComPtr<IStream> stream = SHCreateMemStream(nullptr, 0);
	if (!stream) {
		return false;
	}

	ComPtr<IMFByteStream> byte_stream;
	hr = MFCreateMFByteStreamOnStream(stream.Get(), &byte_stream);
	if (FAILED(hr)) {
		return false;
	}

	ComPtr<IMFMediaSink> sink;
	if (type == AudioType::WAV) {
		assert(false, "WAV saving is not supported yet");
	}
	else if (type == AudioType::MP3) {
		MFCreateMP3MediaSink(byte_stream.Get(), &sink);
	}
	else {
		return false;
	}
	
	ComPtr<IMFSinkWriter> writer;
	hr = MFCreateSinkWriterFromMediaSink(sink.Get(), nullptr, &writer);
	if (FAILED(hr)) {
		return false;
	}

	ComPtr<IMFMediaType> input_type;
	hr = MFCreateMediaType(&input_type);
	if (FAILED(hr)) {
		return false;
	}
	input_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	input_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_Float);
	input_type->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1);
	input_type->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 32);
	input_type->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, 4);
	input_type->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, sample_rate);
	input_type->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, sample_rate * sizeof(float));
	writer->SetInputMediaType(0, input_type.Get(), nullptr) >> verify_result;

	writer->BeginWriting() >> verify_result;
	for (int i = 0; i < (int) size();) {
		const int sample_size = std::min(sample_rate, (int) size() - i);
		const int sample_byte_size = sample_size * sizeof(float);

		ComPtr<IMFMediaBuffer> media_buffer;
		MFCreateMemoryBuffer(sample_byte_size, &media_buffer) >> verify_result;
		media_buffer->SetCurrentLength(sample_byte_size) >> verify_result;

		ComPtr<IMFSample> media_sample;
		MFCreateSample(&media_sample) >> verify_result;
		media_sample->AddBuffer(media_buffer.Get()) >> verify_result;

		BYTE* out_buffer = nullptr;
		media_buffer->Lock(&out_buffer, nullptr, nullptr) >> verify_result;
		memcpy(out_buffer, data() + i, sample_byte_size);
		media_buffer->Unlock() >> verify_result;

		media_sample->SetSampleDuration((sample_size * 10'000'000LL) / sample_rate) >> verify_result;
		media_sample->SetSampleTime((i * 10'000'000LL) / sample_rate) >> verify_result;
		writer->WriteSample(0, media_sample.Get()) >> verify_result;
		i += sample_size;
	}
	writer->Finalize() >> verify_result;

	QWORD length = 0;
	byte_stream->GetLength(&length) >> verify_result;
	buffer->resize((size_t) length);
	byte_stream->SetCurrentPosition(0) >> verify_result;
	ULONG ignored = 0;
	byte_stream->Read(buffer->data(), (ULONG) buffer->size(), &ignored) >> verify_result;
	return true;
}

bool kl::Audio::save_to_file(const std::string_view& filepath, const AudioType type) const
{
	std::vector<byte> buffer{};
	if (!save_to_vector(&buffer, type)) {
		return false;
	}
	return write_file(filepath, buffer);
}

bool kl::Audio::get_audio(const float time, const float duration, Audio& out) const
{
	if (time < 0.0f || duration <= 0.0f || (duration_seconds() - time) < duration) {
		return false;
	}
	const int start = sample_index(time);
	const int end = sample_index(time + duration);
	out.resize((size_t) end - start);
	memcpy(out.data(), data() + start, out.size() * sizeof(float));
	return true;
}
