package com.spirytusz.lib.avplayer.callback

import com.spirytusz.lib.avplayer.AVPlayer

interface OnErrorListener {
    fun onError(avPlayer: AVPlayer, code: Int, msg: String)
}