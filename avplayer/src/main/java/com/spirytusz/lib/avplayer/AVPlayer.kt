package com.spirytusz.lib.avplayer

import android.util.Log

class AVPlayer {

    companion object {
        private const val TAG = "AVPlayer"

        init {
            System.loadLibrary("native-lib")
        }

        @JvmStatic
        external fun getFFmpegVersion(): String
    }

    private var nativePlayerPtr = 0L

    fun init() {
        nativePlayerPtr = nativeInit()
        Log.d(TAG, "init() >>> ptr=$nativePlayerPtr")
    }

    fun prepare(url: String) {
        nativePrepare(nativePlayerPtr, url);
    }

    fun release() {
        nativeRelease(nativePlayerPtr)
        Log.d(TAG, "release() >>> ptr=$nativePlayerPtr")
    }

    private external fun nativeInit(): Long

    private external fun nativePrepare(ptr: Long, uri: String)

    private external fun nativePlay(ptr: Long)

    private external fun nativePause(ptr: Long)

    private external fun nativeIsPlaying(ptr: Long): Boolean

    private external fun nativeReset(ptr: Long)

    private external fun nativeRelease(ptr: Long)
}