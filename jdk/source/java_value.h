#pragma once

#include "java.h"


namespace java {
	struct value
	{
        union
        {
            jbool   z;
            jbyte   b;
            jchar   c;
            jshort  s;
            jint    i;
            jlong   j = {};
            jfloat  f;
            jdouble d;
            jobject l;
        };

        value();
        value(jbool val);
        value(jbyte val);
        value(jchar val);
        value(jshort val);
        value(jint val);
        value(jlong val);
        value(jfloat val);
        value(jdouble val);
        value(jobject val);
	};
}
