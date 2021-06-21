package com.spirytusz.lib.avplayer

import android.os.Handler
import android.os.Looper
import android.os.Message
import android.util.Log
import androidx.annotation.AnyThread
import com.spirytusz.lib.avplayer.AVPlayerCode.MessageType.NATIVE_MESSAGE
import com.spirytusz.lib.avplayer.AVPlayerCode.Prepared.CODE_PREPARED
import com.spirytusz.lib.avplayer.AVPlayerCode.isErrorCode
import com.spirytusz.lib.avplayer.callback.OnErrorListener
import com.spirytusz.lib.avplayer.callback.OnPreparedListener

class AVPlayer {

    companion object {
        private const val TAG = "AVPlayer"

        init {
            System.loadLibrary("native-lib")
        }

        @JvmStatic
        external fun getFFmpegVersion(): String
    }

    var onPreparedListener: OnPreparedListener? = null
    var onErrorListener: OnErrorListener? = null

    private var nativePlayerPtr = 0L
    private val avPlayHandler by lazy {
        AVPlayHandler()
    }

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

    fun play() {
        nativePlay(nativePlayerPtr);
    }

    fun pause() {
        nativePause(nativePlayerPtr)
    }

    /**
     * will be called by native
     */
    @AnyThread
    private fun messageBridge(code: Int, msg: String) {
        Log.d(TAG, "messageBridge() >>> NativeMessage(code=$code, msg=$msg)")
        val message = Message.obtain()
        val nativeMessage = NativeMessage(code, msg)
        message.what = NATIVE_MESSAGE
        message.obj = nativeMessage
        avPlayHandler.sendMessage(message)
    }

    private fun handleNativeMessage(nativeMessage: NativeMessage) {
        when {
            isErrorCode(nativeMessage.code) -> {
                onErrorListener?.onError(this, nativeMessage.code, nativeMessage.msg)
            }
            nativeMessage.code == CODE_PREPARED -> {
                onPreparedListener?.onPrepared(this)
            }
        }
    }

    private external fun nativeInit(): Long

    private external fun nativePrepare(ptr: Long, uri: String)

    private external fun nativePlay(ptr: Long)

    private external fun nativePause(ptr: Long)

    private external fun nativeIsPlaying(ptr: Long): Boolean

    private external fun nativeReset(ptr: Long)

    private external fun nativeRelease(ptr: Long)

    private inner class AVPlayHandler : Handler(Looper.getMainLooper()) {

        override fun handleMessage(msg: Message) {
            super.handleMessage(msg)
            if (msg.what == NATIVE_MESSAGE) {
                handleNativeMessage(msg.obj as NativeMessage)
            }
        }
    }

    private data class NativeMessage(
        val code: Int,
        val msg: String
    )
}