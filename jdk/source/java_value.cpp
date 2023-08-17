#include "java.h"
#include "java_value.h"


java::value::value()
{}

java::value::value(jbool val)
	: z(val)
{}

java::value::value(jbyte val)
	: b(val)
{}

java::value::value(jchar val)
	: c(val)
{}

java::value::value(jshort val)
	: s(val)
{}

java::value::value(jint val)
	: i(val)
{}

java::value::value(jlong val)
	: j(val)
{}

java::value::value(jfloat val)
	: f(val)
{}

java::value::value(jdouble val)
	: d(val)
{}

java::value::value(jobject val)
	: l(val)
{}
