#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

namespace {

constexpr char DEFAULT_WIFI_SSID[] = "Repeater-9924";
constexpr char DEFAULT_WIFI_PASSWORD[] = "nino2222";
constexpr char PREFERENCES_NAMESPACE[] = "device-config";
constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000;

Preferences preferences;
WebServer server(80);
String wifiSsid;
String wifiPassword;
String rtdbUrl;
String rtdbToken;
bool connectedToWifi = false;

String htmlEscape(const String &value) {
  String escaped;
  escaped.reserve(value.length());
  for (size_t i = 0; i < value.length(); ++i) {
    switch (value[i]) {
      case '&':
        escaped += F("&amp;");
        break;
      case '<':
        escaped += F("&lt;");
        break;
      case '>':
        escaped += F("&gt;");
        break;
      case '"':
        escaped += F("&quot;");
        break;
      case '\'':
        escaped += F("&#39;");
        break;
      default:
        escaped += value[i];
    }
  }
  return escaped;
}

String configurationPage() {
  String page;
  page.reserve(1800);
  page += F(
      "<!doctype html><html lang=\"id\"><head><meta charset=\"utf-8\">"
      "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
      "<title>Pengaturan ESP32</title><style>"
      "body{font:16px sans-serif;max-width:520px;margin:2em auto;padding:0 1em;"
      "background:#f4f6f8;color:#20242a}form{background:white;padding:1.2em;"
      "border-radius:10px}label{display:block;margin:1em 0 .35em}input{box-sizing:"
      "border-box;width:100%;padding:.7em;border:1px solid #bbc2ca;border-radius:"
      "5px}button{margin-top:1.4em;padding:.75em 1em;background:#0969da;color:white;"
      "border:0;border-radius:5px;font-size:1em}.note{color:#57606a;font-size:.9em}"
      "</style></head><body><h1>Pengaturan perangkat</h1>"
      "<p>ESP32 belum tersambung ke Wi-Fi. Simpan pengaturan untuk mencoba kembali."
      "</p><form method=\"post\" action=\"/save\">"
      "<label for=\"ssid\">SSID Wi-Fi</label><input id=\"ssid\" name=\"ssid\" "
      "required maxlength=\"32\" value=\"");
  page += htmlEscape(wifiSsid);
  page += F("\"><label for=\"wifi_password\">Kata sandi Wi-Fi</label>"
            "<input id=\"wifi_password\" name=\"wifi_password\" type=\"password\" "
            "maxlength=\"63\" autocomplete=\"new-password\" placeholder=\"Kosongkan "
            "jika tidak ingin mengubah\">"
            "<label for=\"rtdb_url\">Firebase RTDB URL</label>"
            "<input id=\"rtdb_url\" name=\"rtdb_url\" type=\"url\" "
            "placeholder=\"https://contoh-default-rtdb.firebaseio.com\" value=\"");
  page += htmlEscape(rtdbUrl);
  page += F("\"><label for=\"rtdb_token\">Firebase RTDB auth token (opsional)</label>"
            "<input id=\"rtdb_token\" name=\"rtdb_token\" type=\"password\" "
            "autocomplete=\"new-password\" placeholder=\"Kosongkan jika tidak ingin "
            "mengubah\"><p class=\"note\">Kata sandi dan token tidak ditampilkan. "
            "Kolom kosong mempertahankan nilai tersimpan.</p>"
            "<button type=\"submit\">Simpan dan sambungkan ulang</button>"
            "</form></body></html>");
  return page;
}

void handleConfiguration() {
  server.send(200, "text/html; charset=utf-8", configurationPage());
}

void handleSave() {
  String newSsid = server.arg("ssid");
  if (newSsid.isEmpty()) {
    server.send(400, "text/plain; charset=utf-8", "SSID tidak boleh kosong.");
    return;
  }

  preferences.begin(PREFERENCES_NAMESPACE, false);
  preferences.putString("ssid", newSsid);
  if (!server.arg("wifi_password").isEmpty()) {
    preferences.putString("password", server.arg("wifi_password"));
  }
  preferences.putString("rtdb_url", server.arg("rtdb_url"));
  if (!server.arg("rtdb_token").isEmpty()) {
    preferences.putString("rtdb_token", server.arg("rtdb_token"));
  }
  preferences.end();

  server.send(200, "text/html; charset=utf-8",
              F("<!doctype html><html lang=\"id\"><meta charset=\"utf-8\">"
                "<p>Pengaturan tersimpan. Perangkat akan menyambung ulang...</p>"
                "</html>"));
  delay(800);
  ESP.restart();
}

void startConfigurationAccessPoint() {
  WiFi.mode(WIFI_AP);
  const uint64_t chipId = ESP.getEfuseMac();
  char apSsid[32];
  char apPassword[13];
  snprintf(apSsid, sizeof(apSsid), "Wemos32-Setup-%04X",
           static_cast<unsigned int>(chipId & 0xFFFF));
  snprintf(apPassword, sizeof(apPassword), "%010lu",
           static_cast<unsigned long>(esp_random() % 10000000000ULL));

  if (!WiFi.softAP(apSsid, apPassword)) {
    Serial.println(F("Gagal membuat access point konfigurasi."));
    return;
  }

  Serial.printf("Mode konfigurasi: SSID %s, password %s, buka http://%s\n",
                apSsid, apPassword, WiFi.softAPIP().toString().c_str());
  server.on("/", HTTP_GET, handleConfiguration);
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound(handleConfiguration);
  server.begin();
}

void startOta() {
  ArduinoOTA.setHostname("wemos-lolin32");
  ArduinoOTA.onStart([]() { Serial.println(F("OTA dimulai.")); });
  ArduinoOTA.onEnd([]() { Serial.println(F("\nOTA selesai.")); });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Kesalahan OTA [%u]\n", error);
  });
  ArduinoOTA.begin();
  Serial.println(F("OTA siap."));
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(100);

  preferences.begin(PREFERENCES_NAMESPACE, true);
  wifiSsid = preferences.getString("ssid", DEFAULT_WIFI_SSID);
  wifiPassword = preferences.getString("password", DEFAULT_WIFI_PASSWORD);
  rtdbUrl = preferences.getString("rtdb_url", "");
  rtdbToken = preferences.getString("rtdb_token", "");
  preferences.end();

  WiFi.mode(WIFI_STA);
  WiFi.setHostname("wemos-lolin32");
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
  Serial.printf("Menyambung ke Wi-Fi: %s\n", wifiSsid.c_str());

  const uint32_t startedAt = millis();
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startedAt < WIFI_CONNECT_TIMEOUT_MS) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  connectedToWifi = WiFi.status() == WL_CONNECTED;
  if (connectedToWifi) {
    Serial.printf("Wi-Fi tersambung. IP: %s\n",
                  WiFi.localIP().toString().c_str());
    startOta();
  } else {
    Serial.println(F("Wi-Fi gagal tersambung; memulai AP konfigurasi."));
    startConfigurationAccessPoint();
  }
}

void loop() {
  if (connectedToWifi) {
    ArduinoOTA.handle();
  } else {
    server.handleClient();
  }
}
