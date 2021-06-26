package com.spirytusz.avplayer

import android.annotation.SuppressLint
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import com.spirytusz.avplayer.databinding.ActivityMainBinding
import com.spirytusz.lib.avplayer.AVPlayer
import com.spirytusz.lib.avplayer.callback.OnErrorListener
import com.spirytusz.lib.avplayer.callback.OnPreparedListener

class MainActivity : AppCompatActivity(), OnPreparedListener, OnErrorListener,
    SurfaceHolder.Callback {

    companion object {
        private const val TAG = "MainActivityTest"
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

        avPlayer.prepare("http://220.161.87.62:8800/hls/0/index.m3u8")
    }


    @SuppressLint("SetTextI18n")
    private fun initView() {
        binding.sampleText.text = "FFmpeg Version ${AVPlayer.getFFmpegVersion()}"
    }

    private fun initListener() {
        avPlayer.onPreparedListener = this
        avPlayer.onErrorListener = this

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
        if (binding.surfaceView.holder.isCreating) {
            binding.surfaceView.holder.addCallback(this)
        } else {
            avPlayer.setSurface(binding.surfaceView.holder.surface)
            binding.play.isEnabled = true
        }
    }

    override fun onError(avPlayer: AVPlayer, code: Int, msg: String) {
        Log.e(TAG, "onError code=$code, msg=$msg")
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        Log.d(TAG, "surfaceChanged")
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        Log.d(TAG, "surfaceDestroyed")
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        Log.d(
            TAG,
            "surfaceCreated width=${binding.surfaceView.width} height=${binding.surfaceView.height}"
        )
        avPlayer.setSurface(holder.surface)

        binding.play.isEnabled = true
    }
}