package com.theeasiestway.opus

import android.util.Log

class Opus {

    companion object {

        val TAG = "CodecOpus"

        init {
            try {
                System.loadLibrary("easyopus")
            } catch (e: Exception) {
                Log.e(TAG, "Couldn't load opus library: $e")
            }
        }
    }

    // pointer to codec instance
    private var codecPtr: Long = 0

    init {
        codecPtr = createCodec()
    }

    private external fun createCodec(): Long
    private external fun destroyCodec(codecPtr: Long)

    fun release() {
        destroyCodec(codecPtr)
    }

    // Encoder
    fun encoderInit(sampleRate: Constants.SampleRate, channels: Constants.Channels, application: Constants.Application): Int {
        return encoderInit(codecPtr, sampleRate.v, channels.v, application.v)
    }

    private external fun encoderInit(codecPtr: Long, sampleRate: Int, numChannels: Int, application: Int): Int

    fun encoderSetBitrate(bitrate: Constants.Bitrate): Int {
        return encoderSetBitrate(codecPtr, bitrate.v)
    }

    private external fun encoderSetBitrate(codecPtr: Long, bitrate: Int): Int

    fun encoderSetComplexity(complexity: Constants.Complexity): Int {
        return encoderSetComplexity(codecPtr, complexity.v)
    }

    private external fun encoderSetComplexity(codecPtr: Long, complexity: Int): Int

    fun encode(bytes: ByteArray, frameSize: Constants.FrameSize): ByteArray? {
        return encode(codecPtr, bytes, frameSize.v)
    }

    private external fun encode(codecPtr: Long, bytes: ByteArray, frameSize: Int): ByteArray?

    fun encode(shorts: ShortArray, frameSize: Constants.FrameSize): ShortArray? {
        return encode(codecPtr, shorts, frameSize.v)
    }

    private external fun encode(codecPtr: Long, shorts: ShortArray, frameSize: Int): ShortArray?

    fun encoderRelease() {
        encoderRelease(codecPtr)
    }

    private external fun encoderRelease(codecPtr: Long)

    // Decoder
    fun decoderInit(sampleRate: Constants.SampleRate, channels: Constants.Channels): Int {
        return decoderInit(codecPtr, sampleRate.v, channels.v)
    }

    private external fun decoderInit(codecPtr: Long, sampleRate: Int, numChannels: Int): Int

    fun decode(bytes: ByteArray, frameSize: Constants.FrameSize, fec: Int = 0): ByteArray? {
        return decode(codecPtr, bytes, frameSize.v, fec)
    }

    private external fun decode(codecPtr: Long, bytes: ByteArray, frameSize: Int, fec: Int): ByteArray?

    fun decode(shorts: ShortArray, frameSize: Constants.FrameSize, fec: Int = 0): ShortArray? {
        return decode(codecPtr, shorts, frameSize.v, fec)
    }

    private external fun decode(codecPtr: Long, shorts: ShortArray, frameSize: Int, fec: Int): ShortArray?

    fun decoderRelease() {
        decoderRelease(codecPtr)
    }

    private external fun decoderRelease(codecPtr: Long)

    // Utils
    fun convert(bytes: ByteArray): ShortArray? {
        return convert(codecPtr, bytes)
    }

    private external fun convert(codecPtr: Long, bytes: ByteArray): ShortArray?

    fun convert(shorts: ShortArray): ByteArray? {
        return convert(codecPtr, shorts)
    }

    private external fun convert(codecPtr: Long, shorts: ShortArray): ByteArray?
}
