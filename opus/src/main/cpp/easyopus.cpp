//
// Created by Loboda Alexey on 21.05.2020.
//

#include <string>
#include <jni.h>
#include "codec/CodecOpus.h"
#include "utils/SamplesConverter.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_theeasiestway_opus_Opus_createCodec(JNIEnv *env, jobject thiz) {
    return reinterpret_cast<jlong>(new CodecOpus());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_opus_Opus_destroyCodec(JNIEnv *env, jobject thiz, jlong codecPtr) {
auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);
delete codec;
}

//
// Encoding
//

extern "C"
JNIEXPORT jint JNICALL
        Java_com_theeasiestway_opus_Opus_encoderInit(JNIEnv *env, jobject thiz, jlong codecPtr, jint sample_rate, jint num_channels, jint application) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);
    return codec->encoderInit(sample_rate, num_channels, application);
}

extern "C"
JNIEXPORT jint JNICALL
        Java_com_theeasiestway_opus_Opus_encoderSetBitrate(JNIEnv *env, jobject thiz, jlong codecPtr, jint bitrate) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);
    return codec->encoderSetBitrate(bitrate);
}

extern "C"
JNIEXPORT jint JNICALL
        Java_com_theeasiestway_opus_Opus_encoderSetComplexity(JNIEnv *env, jobject thiz, jlong codecPtr, jint complexity) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);
    return codec->encoderSetComplexity(complexity);
}

extern "C"
JNIEXPORT jshortArray JNICALL
        Java_com_theeasiestway_opus_Opus_encode___3SI(JNIEnv *env, jobject thiz, jlong codecPtr, jshortArray shorts, jint frame_size) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);

    jshort *nativeShorts = env->GetShortArrayElements(shorts, 0);
    jint length = env->GetArrayLength(shorts);

    std::vector<short> encodedData = codec->encode(nativeShorts, length, frame_size);
    int encodedSize = encodedData.size();

    if (encodedSize <= 0) return nullptr;

    jshortArray result = env->NewShortArray(encodedSize);
    env->SetShortArrayRegion(result, 0, encodedSize, encodedData.data());
    env->ReleaseShortArrayElements(shorts, nativeShorts, 0);

    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_theeasiestway_opus_Opus_decoderRelease(JNIEnv *env, jobject thiz, jlong codecPtr) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);
    codec->decoderRelease();
}

//
// Decoding
//

extern "C"
JNIEXPORT jint JNICALL
        Java_com_theeasiestway_opus_Opus_decoderInit(JNIEnv *env, jobject thiz, jlong codecPtr, jint sample_rate, jint num_channels) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);
    return codec->decoderInit(sample_rate, num_channels);
}


extern "C"
JNIEXPORT jbyteArray JNICALL
        Java_com_theeasiestway_opus_Opus_decode___3BII(JNIEnv *env, jobject thiz, jlong codecPtr, jbyteArray bytes, jint frame_size, jint fec) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);

    jbyte *nativeBytes = env->GetByteArrayElements(bytes, 0);
    jint length = env->GetArrayLength(bytes);

    std::vector<uint8_t> decodedData = codec->decode((uint8_t *) nativeBytes, length, frame_size, fec);
    int decodedSize = decodedData.size();

    if (decodedSize <= 0) return nullptr;

    jbyteArray result = env->NewByteArray(decodedSize);
    env->SetByteArrayRegion(result, 0, decodedSize, (jbyte *) decodedData.data());
    env->ReleaseByteArrayElements(bytes, nativeBytes, 0);

    return result;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_theeasiestway_opus_Opus_decode__J_3BII(JNIEnv *env, jobject thiz, jlong codecPtr, jbyteArray bytes, jint frameSize, jint fec) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);
    jbyte *nativeBytes = env->GetByteArrayElements(bytes, 0);
    jint length = env->GetArrayLength(bytes);

    std::vector<uint8_t> decodedData = codec->decode((uint8_t *)nativeBytes, length, frameSize, fec);
    int decodedSize = decodedData.size();

    if (decodedSize <= 0) return nullptr;

    jbyteArray result = env->NewByteArray(decodedSize);
    env->SetByteArrayRegion(result, 0, decodedSize, (jbyte *)decodedData.data());
    env->ReleaseByteArrayElements(bytes, nativeBytes, 0);

    return result;
}



extern "C"
JNIEXPORT jshortArray JNICALL
        Java_com_theeasiestway_opus_Opus_decode___3SII(JNIEnv *env, jobject thiz, jlong codecPtr, jshortArray shorts, jint frame_size, jint fec) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);

    jshort *nativeShorts = env->GetShortArrayElements(shorts, 0);
    jint length = env->GetArrayLength(shorts);

    std::vector<short> decodedData = codec->decode(nativeShorts, length, frame_size, fec);
    int decodedSize = decodedData.size();

    if (decodedSize <= 0) return nullptr;

    jshortArray result = env->NewShortArray(decodedSize);
    env->SetShortArrayRegion(result, 0, decodedSize, decodedData.data());
    env->ReleaseShortArrayElements(shorts, nativeShorts, 0);

    return result;
}


//
// Utils
//

extern "C"
JNIEXPORT jshortArray JNICALL
        Java_com_theeasiestway_opus_Opus_convert___3B(JNIEnv *env, jobject thiz, jlong codecPtr, jbyteArray bytes) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);

    uint8_t *nativeBytes = (uint8_t *) env->GetByteArrayElements(bytes, 0);
    jint length = env->GetArrayLength(bytes);

    std::vector<short> shorts = SamplesConverter::convert(&nativeBytes, length);
    int size = shorts.size();

    if (!size) return nullptr;

    jshortArray result = env->NewShortArray(size);
    env->SetShortArrayRegion(result, 0, size, shorts.data());
    env->ReleaseByteArrayElements(bytes, (jbyte *) nativeBytes, 0);

    return result;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
        Java_com_theeasiestway_opus_Opus_convert___3S(JNIEnv *env, jobject thiz, jlong codecPtr, jshortArray shorts) {
    auto *codec = reinterpret_cast<CodecOpus *>(codecPtr);

    short *nativeShorts = env->GetShortArrayElements(shorts, 0);
    jint length = env->GetArrayLength(shorts);

    std::vector<uint8_t> bytes = SamplesConverter::convert(&nativeShorts, length);
    int size = bytes.size();

    if (!size) return nullptr;


    jbyteArray result = env->NewByteArray(size);
    env->SetByteArrayRegion(result, 0, size, (jbyte *) bytes.data());
    env->ReleaseShortArrayElements(shorts, nativeShorts, 0); // Libera la memoria nativa

    return result;
}
