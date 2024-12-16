package com.example.nes_emulator

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.ParcelFileDescriptor
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class OpenWithActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_open_with)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
        val uri = this.intent.data

        val fd = getFdOfRom(uri!!)
        val myIntent = Intent(
            this, NesActivity::class.java
        )
        myIntent.putExtra("romFd", fd.detachFd())
        startActivity(myIntent)
    }

    private fun getFdOfRom(uri: Uri): ParcelFileDescriptor {
        val fd = contentResolver.openFileDescriptor(
            uri, "r"
        )!!
        return fd
    }
}