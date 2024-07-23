#include "fuze.h"


titian::Track::Track()
{
	for (int i = 0; i < 1; i++) {
		Ref<Media> med = new Media();
		med->name = kl::random::gen_string(10);
		med->type = MediaType(kl::random::gen_int(3));
		med->duration = kl::random::gen_float(1.0f, 20.0f);
		this->insert_media(kl::random::gen_float(7.0f), med);
	}
}

void titian::Track::insert_media(const float offset, const Ref<Media>& new_med)
{
	if (media.contains(offset)) {
		return;
	}
	media[offset] = new_med;
	readjust_media();
}

void titian::Track::remove_media(const Ref<Media>& new_med)
{
	for (auto it = media.begin(); it != media.end(); it++) {
		if (it->second == new_med) {
			media.erase(it);
			break;
		}
	}
}

void titian::Track::readjust_media()
{
	std::map copy = media;
	media.clear();

	float last_offset = 0.0f;
	for (auto& [offset, med] : copy) {
		const float new_offset = (offset < last_offset) ? last_offset : offset;
		media[new_offset] = med;
		last_offset = new_offset + med->duration;
	}
}
