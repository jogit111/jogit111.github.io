#include <WiFi.h>
#include <PubSubClient.h> 
#include <Adafruit_NeoPixel.h>

// --- Configuration ---
#define WIFI_SSID       "YOUR WIFI SSID"      // Replace with your network SSID
#define WIFI_PASSWORD   "YOUR WIFI PASSWORD"  // Replace with your network password

#define MQTT_SERVER     "test.mosquitto.org"
#define MQTT_TOPIC      "joriskerst/color"          // The topic to subscribe to for RGB values

#define LED_PIN         4                     // The GPIO pin connected to the Data IN of the WS2812 strip (e.g., GPIO 4)
#define LED_COUNT       200                   // Number of LEDs in your strip

// --- Global Objects ---
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BGR + NEO_KHZ800);

// --- Function Prototypes ---
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);
void setStripColor(int r, int g, int b);

// --- LED Control Function ---
// Sets all pixels to a single color and displays the change.
void setStripColor(int r, int g, int b) {
  // Ensure values are within the 0-255 range
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  Serial.printf("Setting color to: R=%d, G=%d, B=%d\n", r, g, b);

  uint32_t color = strip.Color(r, g, b);

  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show(); // Push the data to the strip
}

// --- MQTT Callback Function ---
// This function is called when a message is received on a subscribed topic.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Payload: ");

  // Convert payload (byte array) to a String for easier parsing.
  String payloadStr;
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  // Convert to uppercase to correctly parse 'a' through 'f' hex characters
  payloadStr.toUpperCase(); 
  
  Serial.println(payloadStr);

  // Expected payload format: "RRGGBB" (6-character hex, e.g., "FF00A5")
  
  if (length == 7) {
    // Extract Red, Green, and Blue hex components (2 chars each)
  
  } else {
    Serial.printf("Error: Invalid payload length (%u). Expected 6-character hex 'RRGGBB'.\n", length);
  }
  if (length == 9)  {
    if (payloadStr == "1")  {
      sparkle();
    }
    if (payloadStr == "2")  {
      fade();
    }
    if (payloadStr == "3")  {
      chase();
    }
    if (payloadStr == "4")  {
      blink();
    }
    if (payloadStr == "5")  {
      wave();
    }
    if (payloadStr == "6")  {
      cycle();
    }
    if (payloadStr == "7")  {
      strobe();
    }
    if (payloadStr == "8")   {

    }
    if (payloadStr == "9")   {
      funcstatic();
    }
  }
}

// --- MQTT Reconnect Logic ---
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect using a unique client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, subscribe to the required topic
      if (client.subscribe(MQTT_TOPIC)) {
        Serial.print("Subscribed to topic: ");
        Serial.println(MQTT_TOPIC);
      } else {
        Serial.println("Subscription failed.");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// --- Arduino Setup ---
void setup() {
  Serial.begin(115200);

  // 1. Initialize NeoPixel strip
  strip.begin();
  setStripColor(0, 0, 0); // Turn LEDs off initially
  strip.setBrightness(100);

  // 2. Connect to WiFi
  Serial.printf("Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 3. Setup MQTT Client
  client.setServer(MQTT_SERVER, 1883); // Default MQTT port is 1883
  client.setCallback(callback);
}

// --- Arduino Loop ---
void loop() {
  // Keep the MQTT client connected
  if (!client.connected()) {
    reconnect();
  }
  
  // MQTT client loop must be called frequently to process incoming messages
  client.loop();
  
  // Add a small delay to prevent watchdog timer from tripping and to allow other tasks to run
  delay(10);
}


void sparkle()  {
  setStripColor(0, 0, 0); // Turn LEDs off initially
  for(int i=0; i < strip.numPixels(); i++) {
    setStripColor(0, 0, 0); // Turn LEDs off initially
    strip.show();
    for(int c=0; c < 50;   c++)  {
      strip.setPixelColor(random(0, strip.numPixels()), 255, 0, 0);   //red
      
    }
    strip.show();
    delay(200);


  }
  strip.show(); // Push the data to the strip
}


void fade()  {
  int brightnessfade = 255;
  setStripColor(0, 0, 0); // Turn LEDs off initially

  for(int i=0; i < strip.numPixels(); i++) {
    setStripColor(0, brightnessfade, 0);  //Green
    strip.show();

    delay(10);
    brightnessfade = brightnessfade - 1;

  }
  strip.show(); // Push the data to the strip
}


void chase()  {
  for(int q=0; q < 1000; q++)
  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 255, 255, 0);   //yellow
    strip.setPixelColor(i - 1, 0, 0, 0);   //yellow

    strip.show(); // Push the data to the strip

  }
}


void blink()  {
  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 255);  //Blue
    delay(250);
    strip.setPixelColor(i, 0, 0, 0);
    delay(250);
  }
  strip.show(); // Push the data to the strip
}


void wave()  {
  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 255, 0, 255);  //magenta
  }
  strip.show(); // Push the data to the strip
}


void cycle()  {
  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 255, 255);  //cyan
  }
  strip.show(); // Push the data to the strip
}


void strobe()  {
  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 255, 255, 255); //white
  }
  strip.show(); // Push the data to the strip
}

void funcstatic()  {
  String payloadStr;

  String rHex = payloadStr.substring(1, 3);
  String gHex = payloadStr.substring(3, 5);
  String bHex = payloadStr.substring(5, 7);
  // Convert hex string to decimal integer (base 16) using strtol
  int r = (int)strtol(rHex.c_str(), NULL, 16);
  int g = (int)strtol(gHex.c_str(), NULL, 16);
  int b = (int)strtol(bHex.c_str(), NULL, 16);
  
  // Set the new color on the LED strip
  setStripColor(r, g, b);
}
