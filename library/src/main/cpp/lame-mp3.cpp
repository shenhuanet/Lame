//
// Created by shenhua on 2018/4/25.
//
#include"stdio.h"
#include"jni.h"
#include"android/log.h"
#include "lamemp3/lame.h"
#include "lamemp3/version.h"

#define LOG_TAG "lame-mp3"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define BUFFER_SIZE 8192

static lame_global_flags *lame = NULL;
long currentConvertBytes = 0;

/**
 * rest lame
 */
void resetLame() {
    if (lame != NULL) {
        lame_close(lame);
        lame = NULL;
    }
}

/**
 * init
 * @param inSampleRate
 * @param channel
 * @param mode
 * @param outSampleRate
 * @param outBitRate
 * @param quality
 */
void lameInit(jint inSampleRate, jint channel, jint mode, jint outSampleRate,
              jint outBitRate, jint quality) {
    resetLame();
    lame = lame_init();
    lame_set_in_samplerate(lame, inSampleRate);
    lame_set_num_channels(lame, channel);
    lame_set_out_samplerate(lame, outSampleRate);
    lame_set_brate(lame, outBitRate);
    lame_set_quality(lame, quality);
    switch (mode) {
        case 0:
            lame_set_VBR(lame, vbr_default);
            break;
        case 1:
            lame_set_VBR(lame, vbr_abr);
            break;
        default:
            lame_set_VBR(lame, vbr_mtrh);
            break;
    }
    lame_init_params(lame);

}

extern "C" JNIEXPORT void JNICALL
Java_com_shenhua_lame_library_LameMp3Utils_init(JNIEnv *env, jclass type, jint
inSampleRate, jint channel, jint mode, jint outSampleRate, jint outBitRate, jint quality) {
    lameInit(inSampleRate, channel, mode, outSampleRate, outBitRate, quality);
}

extern "C" JNIEXPORT void JNICALL
Java_com_shenhua_lame_library_LameMp3Utils_convertMp3(JNIEnv *env, jclass type, jstring input_,
                                                      jstring mp3_) {
    const char *input = env->GetStringUTFChars(input_, 0);
    const char *mp3 = env->GetStringUTFChars(mp3_, 0);

    FILE *fInput = fopen(input, "rb");
    FILE *fMp3 = fopen(mp3, "wb");
    short int inputBuffer[BUFFER_SIZE * 2];
    unsigned char mp3Buffer[BUFFER_SIZE];
    int read = 0;
    int write = 0;
    int total = 0;
    currentConvertBytes = 0;
    if (lame == NULL) {
        lameInit(44100, 2, 0, 44100, 96, 7);
    }
    do {
        read = static_cast<int>(fread(inputBuffer, sizeof(short int) * 2, BUFFER_SIZE, fInput));
        total += read * sizeof(short int) * 2;
        currentConvertBytes = total;
        if (read != 0) {
            write = lame_encode_buffer_interleaved(lame, inputBuffer, read, mp3Buffer, BUFFER_SIZE);
            fwrite(mp3Buffer, sizeof(unsigned char), static_cast<size_t>(write), fMp3);
        }
        if (read == 0) {
            lame_encode_flush(lame, mp3Buffer, BUFFER_SIZE);
        }
    } while (read != 0);

    resetLame();
    fclose(fInput);
    fclose(fMp3);
    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(mp3_, mp3);
    currentConvertBytes = -1;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_shenhua_lame_library_LameMp3Utils_getConvertBytes(JNIEnv *env, jclass type) {
    return currentConvertBytes;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_shenhua_lame_library_LameMp3Utils_getLameVersion(JNIEnv *env, jclass type) {
    return env->NewStringUTF(get_lame_version());
}