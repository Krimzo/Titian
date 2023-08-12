#pragma once

#include <jni.h>

#pragma comment(lib, "jawt.lib")
#pragma comment(lib, "jvm.lib")


namespace jdk {
    inline JavaVM* jvm = nullptr;
    inline JNIEnv* env = nullptr;
}

namespace jdk {
    void init();
    void uninit();
}
