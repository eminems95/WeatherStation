#include<DHTStable\dht.h>
#include<LPS\LPS.h>
#include<Wire\Wire.h>
#include<ESP8266WiFi\src\ESP8266WiFi.h>
#include<ArduinoJson\ArduinoJson.h>

#define DHT_PIN 4

LPS barometer;
dht thermo;
double temperature, humidity, pressuremb, pressuremmHg;
struct DHT11
{
	int chk;
	double temperature;
	double humidity;
};

void setup()
{
	
	Serial.begin(115200);
	WiFi.mode(WIFI_AP_STA);
	SetupLPS25H();

}

void loop()
{
	while (!Serial);
	int chk = thermo.read11(DHT_PIN);
	CheckErrors(chk);
		temperature = thermo.temperature;
		humidity = thermo.humidity;


		pressuremb = barometer.readPressureMillibars();
		pressuremmHg = barometer.readPressureInchesHg();
		GenerateJSONObject();
	delay(4000);
	
				ESP.deepSleep(WAKE_RF_DEFAULT);
	//delay(3000);
}

void SetupLPS25H() {
	Serial.begin(9600);
	Wire.begin(13, 12);
	if (!barometer.init())
	{
		Serial.println("Failed to autodetect pressure sensor!");
		while (1);
	}

	barometer.enableDefault();
}

void GenerateJSONObject() {
	StaticJsonBuffer<200> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
		root["temperature"] = temperature;
		root["humidity"] = humidity;

	JsonObject& data = root.createNestedObject("pressure");
		data["hPa"] = pressuremb;
		data["mmHg"] = pressuremmHg;

	root.prettyPrintTo(Serial);
	// This prints:
	// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
}

void CheckErrors(int chk) {
	switch (chk)
	{
	case DHTLIB_OK:
		Serial.print("OK,\t");
		break;
	case DHTLIB_ERROR_CHECKSUM:
		Serial.print("Checksum error,\t");
		break;
	case DHTLIB_ERROR_TIMEOUT:
		Serial.print("Time out error,\t");
		break;
	default:
		Serial.print("Unknown error,\t");
		break;
	}
}