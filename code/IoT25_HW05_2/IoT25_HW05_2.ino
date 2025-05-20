/*********
  Rui Santos
  Adapted by ChatGPT for DHT11
  Original: https://RandomNerdTutorials.com/esp32-ble-server-client/
*********/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>               // ← DHT library

// Set temperature unit (comment out for Fahrenheit)
#define temperatureCelsius

// BLE server name
#define bleServerName "ESP32_BLE_Server"

// ── DHT11 setup ───────────────────────────────────────────────────────────
#define DHTPIN   21            // DHT11 DATA pin → ESP32 GPIO21
#define DHTTYPE  DHT11         // DHT11 type

DHT dht(DHTPIN, DHTTYPE);
// ──────────────────────────────────────────────────────────────────────────

float temp;
float tempF;
float hum;

// BLE notify timer
unsigned long lastTime   = 0;
unsigned long timerDelay = 30000;  // 30-second interval

bool deviceConnected = false;

// Service UUID (unchanged)
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// Temperature characteristic / descriptor
#ifdef temperatureCelsius
  BLECharacteristic bmeTemperatureCelsiusCharacteristics(
    "cba1d466-344c-4be3-ab3f-189f80dd7518",
    BLECharacteristic::PROPERTY_NOTIFY
  );
  BLEDescriptor bmeTemperatureCelsiusDescriptor(
    BLEUUID((uint16_t)0x2902)
  );
#else
  BLECharacteristic bmeTemperatureFahrenheitCharacteristics(
    "f78ebbff-c8b7-4107-93de-889a6a06d408",
    BLECharacteristic::PROPERTY_NOTIFY
  );
  BLEDescriptor bmeTemperatureFahrenheitDescriptor(
    BLEUUID((uint16_t)0x2902)
  );
#endif

// Humidity characteristic / descriptor
BLECharacteristic bmeHumidityCharacteristics(
  "ca73b3ba-39f6-4ab3-91ae-186dc9577d99",
  BLECharacteristic::PROPERTY_NOTIFY
);
BLEDescriptor bmeHumidityDescriptor(BLEUUID((uint16_t)0x2903));

// Client connection callbacks
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer)    { deviceConnected = true; }
  void onDisconnect(BLEServer* pServer) { deviceConnected = false; }
};

void setup() {
  Serial.begin(115200);
  // Initialize DHT11
  dht.begin();

  // BLE device, server, and service setup (same as original)
  BLEDevice::init(bleServerName);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *bmeService = pServer->createService(SERVICE_UUID);

  // Temperature characteristic + descriptor
  #ifdef temperatureCelsius
    bmeService->addCharacteristic(&bmeTemperatureCelsiusCharacteristics);
    bmeTemperatureCelsiusDescriptor.setValue("Temperature (°C)");
    bmeTemperatureCelsiusCharacteristics.addDescriptor(&bmeTemperatureCelsiusDescriptor);
  #else
    bmeService->addCharacteristic(&bmeTemperatureFahrenheitCharacteristics);
    bmeTemperatureFahrenheitDescriptor.setValue("Temperature (°F)");
    bmeTemperatureFahrenheitCharacteristics.addDescriptor(&bmeTemperatureFahrenheitDescriptor);
  #endif

  // Humidity characteristic + descriptor
  bmeService->addCharacteristic(&bmeHumidityCharacteristics);
  bmeHumidityDescriptor.setValue("Humidity (%)");
  bmeHumidityCharacteristics.addDescriptor(new BLE2902());

  // Start service & start advertising
  bmeService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("Waiting for client connection to notify...");
}

void loop() {
  if (!deviceConnected) return;

  if ((millis() - lastTime) > timerDelay) {
    // Read temperature (°C), Fahrenheit (°F), and humidity (%) from DHT11
    temp  = dht.readTemperature();
    tempF = dht.readTemperature(true);
    hum   = dht.readHumidity();

    // Notify temperature
    #ifdef temperatureCelsius
      char temperatureCTemp[6];
      dtostrf(temp, 6, 2, temperatureCTemp);
      bmeTemperatureCelsiusCharacteristics.setValue(temperatureCTemp);
      bmeTemperatureCelsiusCharacteristics.notify();
      Serial.print("Temperature Celsius: ");
      Serial.print(temp);
      Serial.print(" °C");
    #else
      char temperatureFTemp[6];
      dtostrf(tempF, 6, 2, temperatureFTemp);
      bmeTemperatureFahrenheitCharacteristics.setValue(temperatureFTemp);
      bmeTemperatureFahrenheitCharacteristics.notify();
      Serial.print("Temperature Fahrenheit: ");
      Serial.print(tempF);
      Serial.print(" °F");
    #endif

    // Notify humidity
    char humidityTemp[6];
    dtostrf(hum, 6, 2, humidityTemp);
    bmeHumidityCharacteristics.setValue(humidityTemp);
    bmeHumidityCharacteristics.notify();
    Serial.print(" - Humidity: ");
    Serial.print(hum);
    Serial.println(" %");

    lastTime = millis();
  }
}
