package com.spirytusz.avplayer

import android.annotation.SuppressLint
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.spirytusz.avplayer.databinding.ActivityMainBinding
import com.spirytusz.lib.avplayer.AVPlayer
import com.spirytusz.lib.avplayer.callback.OnPreparedListener

class MainActivity : AppCompatActivity(), OnPreparedListener {

    companion object {
        private const val TAG = "MainActivity"
    }

    private lateinit var binding: ActivityMainBinding

    private val avPlayer by lazy {
        AVPlayer()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        initView()
        initListener()

        avPlayer.init()
        avPlayer.prepare("http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8")
    }


    @SuppressLint("SetTextI18n")
    private fun initView() {
        binding.sampleText.text = "FFmpeg Version ${AVPlayer.getFFmpegVersion()}"
    }

    private fun initListener() {
        avPlayer.onPreparedListener = this

        binding.play.setOnClickListener {
            avPlayer.play()
        }

        binding.pause.setOnClickListener {
            avPlayer.pause()
        }

        binding.release.setOnClickListener {
            avPlayer.release()
        }
    }

    override fun onPrepared(avPlayer: AVPlayer) {
        Log.d(TAG, "onPrepared")
        binding.play.isEnabled = true
    }
}