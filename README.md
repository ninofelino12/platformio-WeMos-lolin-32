# PlatformIO WeMos LOLIN32

Firmware Arduino untuk ESP32 WeMos LOLIN32 dengan OTA dan halaman konfigurasi
Wi-Fi lokal.

[![Build firmware](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml/badge.svg)](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml)
[![Jalankan build](https://img.shields.io/badge/Jalankan%20build-GitHub%20Actions-2088FF?logo=githubactions&logoColor=white)](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml)

## Build dan unduh firmware

GitHub Actions otomatis meng-compile firmware saat ada push atau pull request.
Untuk menjalankan build secara manual, klik **Jalankan build** di atas, lalu
pilih **Run workflow**.

Setelah build berhasil:

1. Buka tab [Actions](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml)
   dan pilih hasil build terbaru.
2. Pada bagian **Artifacts**, unduh **`firmware-lolin32`**.
3. Ekstrak file ZIP untuk mendapatkan `firmware.bin`. Artifact tersimpan selama
   30 hari.

Untuk compile lokal dengan PlatformIO, jalankan `pio run`. File firmware akan
dibuat di `.pio/build/lolin32/firmware.bin`.

## Penggunaan

1. Unggah firmware pertama kali melalui USB.
2. Perangkat mencoba tersambung ke Wi-Fi bawaan `Repeater-9924` dengan kata sandi
   `nino2222`.
3. Jika gagal tersambung selama 20 detik, perangkat membuat access point
   `Wemos32-Setup-XXXX`. Nama dan kata sandi acak access point ditampilkan di
   Serial Monitor pada 115200 baud.
4. Sambungkan ponsel/komputer ke access point tersebut, lalu buka
   `http://192.168.4.1`. Isi SSID, kata sandi, dan URL/token Firebase RTDB.
   Kolom kata sandi atau token yang kosong tidak mengubah nilai tersimpan.
   Setelah disimpan, perangkat akan reboot.
5. Saat tersambung ke Wi-Fi, perangkat menyediakan OTA dengan hostname
   `wemos-lolin32`.

Pengaturan disimpan di NVS ESP32. Nilai URL dan auth token RTDB disimpan sebagai
konfigurasi; integrasi baca/tulis database belum ditambahkan.
