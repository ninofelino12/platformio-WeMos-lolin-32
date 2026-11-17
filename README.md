# PlatformIO WeMos LOLIN32

Proyek Arduino untuk ESP32 WeMos LOLIN32 dengan OTA dan pengaturan Wi-Fi melalui
halaman web lokal.

[![Build firmware](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml/badge.svg)](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml)
[![Jalankan build](https://img.shields.io/badge/Jalankan%20build-GitHub%20Actions-2088FF?logo=githubactions&logoColor=white)](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml)

**Download firmware:** buka tab [Actions](https://github.com/ninofelino12/platformio-WeMos-lolin-32/actions/workflows/build.yml),
pilih build terbaru yang berhasil, lalu unduh artifact **`firmware-lolin32`**
di bagian *Artifacts*. File hasil compile ada di `firmware.bin`. Artifact
tersedia selama 30 hari.

## Penggunaan

1. Buka proyek ini di PlatformIO dan unggah firmware melalui USB.
2. Secara default perangkat mencoba Wi-Fi `Repeater-9924` dengan kata sandi
   `nino2222`.
3. Jika tidak tersambung dalam 20 detik, perangkat membuat access point
   `Wemos32-Setup-XXXX`. Nama access point, kata sandi acak, dan alamat halaman
   konfigurasi (biasanya `http://192.168.4.1`) ditampilkan di Serial Monitor
   115200 baud.
4. Sambungkan ponsel/komputer ke access point itu, buka alamat konfigurasi,
   lalu simpan SSID/kata sandi Wi-Fi dan konfigurasi Firebase RTDB. Kata sandi
   atau token yang dikosongkan tidak diubah. Setelah disimpan, perangkat reboot.
5. Setelah perangkat tersambung ke Wi-Fi, host OTA adalah `wemos-lolin32`.
   Pilih environment PlatformIO `lolin32` untuk upload melalui jaringan.

SSID/kata sandi Wi-Fi bawaan ditetapkan di `src/main.cpp`. Pengaturan yang
disimpan melalui halaman web menggantikan nilai bawaan dan disimpan di NVS ESP32.
URL dan auth token RTDB hanya disimpan sebagai konfigurasi; integrasi baca/tulis
database belum ditambahkan.
