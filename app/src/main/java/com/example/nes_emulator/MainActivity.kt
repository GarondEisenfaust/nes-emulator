package com.example.nes_emulator

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.ParcelFileDescriptor
import androidx.activity.enableEdgeToEdge
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.documentfile.provider.DocumentFile


class MainActivity : AppCompatActivity() {

    private fun getFdOfRom(uri: Uri, romName: String): ParcelFileDescriptor {
        val uriToFile = DocumentFile.fromTreeUri(baseContext, uri)?.listFiles()
            ?.find { file -> file.name == romName }!!.uri
        val fd = contentResolver.openFileDescriptor(
            uriToFile, "r"
        )!!
        return fd
    }

    private fun openDirectory(pickerInitialUri: Uri) {
        val listDir = registerForActivityResult(ActivityResultContracts.OpenDocumentTree()) { uri ->
            val fd = getFdOfRom(uri!!, "smb.nes")
            val myIntent = Intent(
                this, NesActivity::class.java
            )
            myIntent.putExtra("romFd", fd.detachFd())
            startActivity(myIntent)
        }
        listDir.launch(pickerInitialUri)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
        openDirectory(Uri.parse("Downloads"))
    }
}