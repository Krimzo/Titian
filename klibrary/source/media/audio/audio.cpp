#include "klibrary.h"


kl::Audio::Audio()
{}

kl::Audio::Audio(const int sample_rate)
	: sample_rate(sample_rate)
{}

kl::Audio::Audio(const std::string_view& path)
{
	load_from_file(path);
}

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

float kl::Audio::index_to_time(const int at_index) const
{
	return at_index / (float) sample_rate;
}

int kl::Audio::time_to_index(const float at_time) const
{
	return int(at_time * sample_rate);
}

float kl::Audio::sample_at_index(const int index) const
{
	if (index >= 0 && index < (int) size()) {
		return (*this)[index];
	}
	return 0.0f;
}

float kl::Audio::sample_at_time(const float time) const
{
	const int first_index = time_to_index(time);
	const int second_index = first_index + 1;
	const float first_time = index_to_time(first_index);
	const float second_time = index_to_time(second_index);
	const float first_sample = sample_at_index(first_index);
	const float second_sample = sample_at_index(second_index);
	return lerp(unlerp(time, first_time, second_time), first_sample, second_sample);
}

bool kl::Audio::load_from_memory(const void* data, const uint64_t byte_size)
{
	HRESULT hr = 0;

	ComRef<IStream> stream{ SHCreateMemStream(reinterpret_cast<const BYTE*>(data), UINT(byte_size)) };
	if (!stream) {
		return false;
	}

	ComRef<IMFByteStream> byte_stream;
	hr = MFCreateMFByteStreamOnStream(stream.get(), &byte_stream);
	if (FAILED(hr)) {
		return false;
	}

	ComRef<IMFSourceReader> reader;
	hr = MFCreateSourceReaderFromByteStream(byte_stream.get(), nullptr, &reader);
	if (FAILED(hr)) {
		return false;
	}

	ComRef<IMFMediaType> media_type;
	hr = reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &media_type);
	if (FAILED(hr)) {
		return false;
	}
	media_type->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, (uint32_t*) &sample_rate);

	ComRef<IMFMediaType> new_media_type;
	hr = MFCreateMediaType(&new_media_type);
	if (FAILED(hr)) {
		return false;
	}
	new_media_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	new_media_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_Float);
	new_media_type->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1);
	new_media_type->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 32);
	new_media_type->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, 4);
	new_media_type->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, sample_rate);
	new_media_type->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, sample_rate * sizeof(float));

	hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, new_media_type.get());
	if (FAILED(hr)) {
		return false;
	}

	this->clear();
	while (true) {
		DWORD flags = NULL;
		LONGLONG time_stamp = 0;
		ComRef<IMFSample> sample;
		if (FAILED(reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, nullptr, &flags, &time_stamp, &sample)) || !sample) {
			break;
		}
		ComRef<IMFMediaBuffer> media_buffer;
		if (FAILED(sample->ConvertToContiguousBuffer(&media_buffer)) || !media_buffer) {
			break;
		}

	 	const int64_t sample_index = int64_t(time_stamp * 1e-7 * sample_rate);
		if (sample_index >= (int64_t) this->size()) {
			this->resize(sample_index);
		}

		BYTE* sample_data = nullptr;
		DWORD sample_byte_size = 0;
		media_buffer->Lock(&sample_data, nullptr, &sample_byte_size) >> verify_result;
		const int old_size = (int) this->size();
		this->resize(old_size + (sample_byte_size / sizeof(float)));
		copy<byte>(this->data() + old_size, sample_data, sample_byte_size);
	}
	return true;
}

bool kl::Audio::load_from_buffer(const std::string_view& buffer)
{
	return load_from_memory(buffer.data(), buffer.size());
}

bool kl::Audio::load_from_file(const std::string_view& filepath)
{
	const std::string data = read_file(filepath);
	return load_from_buffer(data);
}

bool kl::Audio::save_to_buffer(std::string& buffer, const AudioType type) const
{
	HRESULT hr = 0;

	ComRef<IStream> stream{ SHCreateMemStream(nullptr, 0) };
	if (!stream) {
		return false;
	}

	ComRef<IMFByteStream> byte_stream;
	hr = MFCreateMFByteStreamOnStream(stream.get(), &byte_stream);
	if (FAILED(hr)) {
		return false;
	}

	ComRef<IMFMediaSink> sink;
	if (type == AudioType::WAV) {
		assert(false, "WAV saving is not supported yet");
	}
	else if (type == AudioType::MP3) {
		MFCreateMP3MediaSink(byte_stream.get(), &sink);
	}
	else {
		return false;
	}
	
	ComRef<IMFSinkWriter> writer;
	hr = MFCreateSinkWriterFromMediaSink(sink.get(), nullptr, &writer);
	if (FAILED(hr)) {
		return false;
	}

	ComRef<IMFMediaType> input_type;
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
	writer->SetInputMediaType(0, input_type.get(), nullptr) >> verify_result;

	writer->BeginWriting() >> verify_result;
	for (int i = 0; i < (int) size();) {
		const int sample_size = min(sample_rate, (int) size() - i);
		const int sample_byte_size = sample_size * sizeof(float);

		ComRef<IMFMediaBuffer> media_buffer;
		MFCreateMemoryBuffer(sample_byte_size, &media_buffer) >> verify_result;
		media_buffer->SetCurrentLength(sample_byte_size) >> verify_result;

		ComRef<IMFSample> media_sample;
		MFCreateSample(&media_sample) >> verify_result;
		media_sample->AddBuffer(media_buffer.get()) >> verify_result;

		BYTE* out_buffer = nullptr;
		media_buffer->Lock(&out_buffer, nullptr, nullptr) >> verify_result;
		copy<byte>(out_buffer, data() + i, sample_byte_size);
		media_buffer->Unlock() >> verify_result;

		media_sample->SetSampleDuration((sample_size * 10'000'000LL) / sample_rate) >> verify_result;
		media_sample->SetSampleTime((i * 10'000'000LL) / sample_rate) >> verify_result;
		writer->WriteSample(0, media_sample.get()) >> verify_result;
		i += sample_size;
	}
	writer->Finalize() >> verify_result;

	QWORD length = 0;
	byte_stream->GetLength(&length) >> verify_result;
	buffer.resize((size_t) length);
	byte_stream->SetCurrentPosition(0) >> verify_result;
	ULONG ignored = 0;
	byte_stream->Read((BYTE*) buffer.data(), (ULONG) buffer.size(), &ignored) >> verify_result;
	return true;
}

bool kl::Audio::save_to_file(const std::string_view& filepath, const AudioType type) const
{
	std::string buffer;
	if (save_to_buffer(buffer, type)) {
		return write_file(filepath, buffer);
	}
	return false;
}
