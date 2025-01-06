plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.jetbrains.kotlin.android)
}

android {
    namespace = "com.example.nes_emulator"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.example.nes_emulator"
        minSdk = 30
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
        setProperty("archivesBaseName", applicationId)
        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                targets += listOf("nes-android", "nes-lib", "common")
                arguments("-DCMAKE_TOOLCHAIN_FILE=cmake/conan_android_toolchain.cmake")
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro"
            )
            signingConfig = signingConfigs.getByName("debug")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }
    buildFeatures {
        prefab = true
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}


abstract class ConanInstall : DefaultTask() {

    @get:Input
    abstract var rootProject: String

    @get:Input
    abstract var pythonPath: String

    @TaskAction
    fun run() {
        val buildTypes = listOf("Debug", "Release", "RelWithDebInfo")
        val architectures = listOf("armv7", "armv8", "x86", "x86_64")
        val permutations = buildTypes.flatMap { buildType ->
            architectures.map { architecture ->
                Pair(
                    buildType, architecture
                )
            }
        }
        permutations.forEach {
            val command = listOf(
                pythonPath,
                "prepare.py",
                "--build-type=${it.first}",
                "--profile=android",
                "--architecture=${it.second}"
            )

            println(rootProject)
            println(command)

            project.exec {
                workingDir = File(rootProject)
                commandLine = command
            }
        }
    }
}

tasks.register<ConanInstall>("conanInstall") {
    pythonPath = "python"
    rootProject = projectDir.path + "/../"
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.androidx.games.activity)
    implementation(libs.androidx.activity)
    implementation(libs.androidx.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}