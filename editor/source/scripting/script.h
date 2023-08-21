#pragma once

#include "jdk.h"


class script
{
public:
	java::holder<jobject> loader = { nullptr };
	java::holder<jclass> clazz = { nullptr };
	java::holder<jobject> instance = { nullptr };

	jmethod start_method = {};
	jmethod update_method = {};

	script();
	script(jobject loader, jclass clazz);

	bool valid() const;

	bool call_start() const;
	bool call_update() const;
};
