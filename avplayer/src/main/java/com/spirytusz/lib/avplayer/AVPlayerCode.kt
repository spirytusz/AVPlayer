package com.spirytusz.lib.avplayer

object AVPlayerCode {

    object MessageType {
        const val NATIVE_MESSAGE = 0
    }

    object Prepared {
        const val CODE_PREPARED = 1
    }

    object Error {
        const val ERROR_BEGIN = 0xFFF0
        const val PREPARED_ERROR = ERROR_BEGIN + 1
    }

    fun isErrorCode(code: Int): Boolean = (code and Error.ERROR_BEGIN) != 0
}