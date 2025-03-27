// #include <WiFi.h>
// #include <HTTPClient.h>

// // WiFi Credential
// const char *ssid = "Jaetun";
// const char *password = "kunyengcantik";

// // HTTP Server
// const char *serverURL = "http://192.168.1.16/api-data/store_data.php"; // Ganti dengan URL server tujuan

// void wifiSetup();

// void setup() {
//   Serial.begin(115200);
//   wifiSetup();
// }

// void loop() {
//   float summaryBandwidth[10];
//   float summaryLatency[10];

//   for (int j = 0; j < 10; j++) { // 10 percobaan
//     Serial.printf("Percobaan %d:\n", j + 1);
//     float totalBandwidth = 0;
//     float totalLatency = 0;

//     for (int i = 0; i < 10; i++) { // 10 data per percobaan
//       int randomValue = random(1, 1001);
//       String payload = "{\"value\": " + String(randomValue) + "}";

//       HTTPClient http;
//       http.begin(serverURL);
//       http.addHeader("Content-Type", "application/json");

//       unsigned long startTime = micros();
//       int httpResponseCode = http.POST(payload);
//       unsigned long endTime = micros();

//       float latency = (endTime - startTime) / 1000.0; // Konversi ke ms
//       float latencySeconds = latency / 1000.0; // Konversi ke detik

//       // Konversi ukuran data ke bit
//       int dataSizeBits = payload.length() * 8;

//       // Hitung bandwidth dalam kbps
//       float bandwidth = (dataSizeBits / latencySeconds) / 1000.0;

//       totalBandwidth += bandwidth;
//       totalLatency += latency;

//       Serial.printf("Data %d: %s | Latency: %.2f ms | Bandwidth: %.2f kbps | HTTP Code: %d\n", 
//                     i + 1, payload.c_str(), latency, bandwidth, httpResponseCode);

//       http.end();
//       delay(500);
//     }

//     summaryBandwidth[j] = totalBandwidth / 10; // Rata-rata bandwidth
//     summaryLatency[j] = totalLatency / 10; // Rata-rata latency

//     Serial.printf("Rata-rata Bandwidth: %.2f kbps | Rata-rata Latency: %.2f ms\n\n", summaryBandwidth[j], summaryLatency[j]);
//     delay(1000);
//   }

//   Serial.println("Ringkasan Percobaan:");
//   Serial.println("Percobaan | Rata-rata Bandwidth (kbps) | Rata-rata Latency (ms)");
//   for (int j = 0; j < 10; j++) {
//     Serial.printf("%d        | %.2f                   | %.2f\n", j + 1, summaryBandwidth[j], summaryLatency[j]);
//   }
  
//   while (true) {} // Hentikan loop setelah 10 percobaan
// }

// // Function untuk koneksi WiFi
// void wifiSetup() {
//   Serial.print("Menghubungkan ke WiFi...");
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(1000);
//   }

//   Serial.println("\nWiFi Terhubung!");
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());
// }
