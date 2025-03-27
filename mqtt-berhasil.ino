#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Credential
const char *ssid = "Jaetun";
const char *password = "kunyengcantik"; // Isi password WiFi jika ada

// MQTT Credential
const char *mqtt_broker = "test.mosquitto.org";
const char *mqtt_topic = "dinusrobotic/bbm";
const int mqtt_port = 1883;

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

// Variabel untuk pengukuran
const int dataPerPercobaan = 10;
const int maxPercobaan = 10;

int dataCount = 0;
int percobaanCount = 0;
float totalBandwidthPercobaan = 0;
unsigned long totalLatencyPercobaan = 0;

float rekapLatency[maxPercobaan];
float rekapBandwidth[maxPercobaan];

void wifiSetup();
void mqttReconnect();
void printTable();

void setup() {
  Serial.begin(115200);
  wifiSetup();
  client.setServer(mqtt_broker, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();

  int msg = random(1, 1001);
  String payload = "{\"data\":\"" + String(msg) + "\"}";
  int dataSize = payload.length();

  unsigned long startTime = micros(); // Waktu dalam mikrodetik
  if (client.publish(mqtt_topic, payload.c_str(), true)) { // QoS 1 agar menunggu ACK
    unsigned long endTime = micros();
    unsigned long latency = endTime - startTime; // Latensi dalam mikrodetik

    int dataSizeBits = dataSize * 8;
    float latencySeconds = latency / 1000000.0; // Konversi ke detik
    float bandwidth = (dataSizeBits / latencySeconds) / 1000.0;

    totalLatencyPercobaan += latency;
    totalBandwidthPercobaan += bandwidth;
    dataCount++;

    Serial.println("Data terkirim: " + payload);
    Serial.print("Ukuran Data: ");
    Serial.print(dataSize);
    Serial.println(" bytes");

    Serial.print("Latency: ");
    Serial.print(latency / 1000.0); // Konversi ke ms
    Serial.println(" ms");

    Serial.print("Bandwidth per data: ");
    Serial.print(bandwidth);
    Serial.println(" kbps");

    Serial.println("--------------------------");

    if (dataCount >= dataPerPercobaan) {
      float avgLatency = totalLatencyPercobaan / dataPerPercobaan;
      float avgBandwidth = totalBandwidthPercobaan / dataPerPercobaan;

      rekapLatency[percobaanCount] = avgLatency / 1000.0;
      rekapBandwidth[percobaanCount] = avgBandwidth;

      Serial.println("\n===== HASIL PERCOBAAN KE-" + String(percobaanCount + 1) + " =====");
      Serial.print("Rata-rata Latensi: ");
      Serial.print(avgLatency / 1000.0);
      Serial.println(" ms");

      Serial.print("Rata-rata Bandwidth: ");
      Serial.print(avgBandwidth);
      Serial.println(" kbps");
      Serial.println("================================\n");

      dataCount = 0;
      totalLatencyPercobaan = 0;
      totalBandwidthPercobaan = 0;
      percobaanCount++;

      if (percobaanCount >= maxPercobaan) {
        printTable();
        while (true);
      }
    }
  } else {
    Serial.println("Gagal mengirim data!");
  }
  delay(1000);
}

void wifiSetup() {
  Serial.print("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi Terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println(" Berhasil!");
    } else {
      Serial.print(" Gagal, kode: ");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik...");
      delay(5000);
    }
  }
}

void printTable() {
  Serial.println("\n===== RINGKASAN HASIL 10 PERCOBAAN =====");
  Serial.println("Percobaan | Rata-rata Latensi (ms) | Rata-rata Bandwidth (kbps)");
  Serial.println("-------------------------------------------------------------");
  for (int i = 0; i < maxPercobaan; i++) {
    Serial.print("    ");
    Serial.print(i + 1);
    Serial.print("     |         ");
    Serial.print(rekapLatency[i]);
    Serial.print("          |         ");
    Serial.println(rekapBandwidth[i]);
  }
  Serial.println("=============================================================");
  Serial.println("\nProgram selesai!");
}
