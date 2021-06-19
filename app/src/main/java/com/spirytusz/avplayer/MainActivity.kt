package com.spirytusz.avplayer

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.spirytusz.avplayer.databinding.ActivityMainBinding
import com.spirytusz.lib.avplayer.AVPlayer

class MainActivity : AppCompatActivity() {

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

        avPlayer.init()
        binding.sampleText.text = AVPlayer.getFFmpegVersion()
        avPlayer.release()
    }
}