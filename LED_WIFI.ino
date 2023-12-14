#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>           
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFiMulti.h>

// Create an instance of the server
ESP8266WebServer server(80);
ESP8266WiFiMulti wifiMulti;

// NeoPixel configuration
#define PIN 14
#define NUM_LEDS 56
#define MAXHUE 256*6
#define WiFi_Logo_width 60
#define WiFi_Logo_height 36

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define XPOS 0
#define YPOS 1
#define DELTAY 2

// Create an instance of the NeoPixel library
Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Variables to store RGB color values
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int hue = 0;

// Variable to store brightness value
int brightnessValue = 255;

// Function to handle the root page
void handleRootPage() {
  
  // Generate the HTML content
  String htmlContent = "<!DOCTYPE html><html><head>";
  htmlContent += "<style>";
  htmlContent += "body {font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; text-align: center;}";
  htmlContent += "h1 {color: #333;}";
  htmlContent += "h2 {color: #666;}";
  htmlContent += "form {background-color: #fff; border-radius: 5px; padding: 20px; margin: 20px auto; width: 80%; max-width: 400px;}";
  htmlContent += "label {display: block; margin-bottom: 10px; font-weight: bold;}";
  htmlContent += "input[type='number'] {width: 100%; padding: 8px; margin-bottom: 10px; border: 1px solid #ccc; border-radius: 4px;}";
  htmlContent += "input[type='submit'], button {background-color: #007BFF; color: #fff; border: none; border-radius: 4px; padding: 10px 20px; cursor: pointer;}";
  htmlContent += "input[type='submit']:hover, button:hover {background-color: #0056b3;}";
  htmlContent += ".effect-buttons button {margin: 5px;}";
  htmlContent += "</style>";
  htmlContent += "</head><body>";

  htmlContent += "<h1>Color Picker</h1>";
  htmlContent += "<h2>Color Selection</h2>";
  htmlContent += "<form method='post'>";
  htmlContent += "Red: <input type='number' name='red' min='0' max='255'><br>";
  htmlContent += "Green: <input type='number' name='green' min='0' max='255'><br>";
  htmlContent += "Blue: <input type='number' name='blue' min='0' max='255'><br>";
  htmlContent += "<input type='submit' name='colorSubmit' value='Update Color'>";
  htmlContent += "</form>";

  htmlContent += "<h2>Brightness Selection</h2>";
  htmlContent += "<form method='post'>";
  htmlContent += "Brightness: <br>";
  htmlContent += "<button type='submit' name='brightness' value='50'>Lv 1</button>";
  htmlContent += "<button type='submit' name='brightness' value='100'>Lv 2</button>";
  htmlContent += "<button type='submit' name='brightness' value='150'>Lv 3</button>";
  htmlContent += "<button type='submit' name='brightness' value='200'>Lv 4</button>";
  htmlContent += "<button type='submit' name='brightness' value='255'>Lv 5</button>";
  htmlContent += "<input type='hidden' name='brightnessSubmit' value='1'>";
  htmlContent += "</form>";
  
  htmlContent += "<h2>Color Effect Selection<h2>";
  htmlContent += "<form method='post'>";
  htmlContent += "Other options: <br>";
  htmlContent += "<button type='submit' name='colorWipe' value='Apply Effect'>Color Wipe</button>";
  htmlContent += "<button type='submit' name='theaterChase' value='Apply Effect'>Theater Chase</button>";
  htmlContent += "<button type='submit' name='rainbow' value='Apply Effect'>Rainbow</button>";
  htmlContent += "<button type='submit' name='rainbowWipe' value='Apply Effect'>Rainbow Wipe</button>";
  htmlContent += "<button type='submit' name='colorfulWipe' value='Apply Effect'>Colorful Wipe</button>";
  htmlContent += "<button type='submit' name='relaxedTransition' value='Apply Effect'>Relaxed Colors</button>";
  htmlContent += "<button type='submit' name='notSoRelaxedTransition' value='Apply Effect'>Party Colors</button>";    
  htmlContent += "<input type='hidden' name='effectSubmit' value='1'>";
  htmlContent += "<br><br>";

  htmlContent += "</body></html>";
  
  // Set the response content type
  server.sendHeader("Content-Type", "text/html", true);
  server.send(200, "text/html", htmlContent);
}

// Function to change the LED color
void changeColor() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(redValue, greenValue, blueValue));
  }
  strip.show();
}

// Function to change the brightness
void changeBrightness() {
  strip.setBrightness(brightnessValue);
  strip.show();
}

void colorfulWipe(){
  colorWipe(strip.Color(255,   0,   0), 25); // Red
  colorWipe(strip.Color(  0, 255,   0), 25); // Green
  colorWipe(strip.Color(  0,   0, 255), 25); // Blue
  colorWipe(strip.Color(255,   255,   0), 25); // Yellow
  colorWipe(strip.Color(  0, 255,   255), 25); // Turquoise
  colorWipe(strip.Color(  255,   0, 255), 25); // Purple
}

void rainbowWipe(){
  colorWipe(strip.Color(255,   0,   0), 25); // Red
  colorWipe(strip.Color(  255, 80,   0), 25);  // Orange
  colorWipe(strip.Color(  255,   255, 0), 25); // Yellow
  colorWipe(strip.Color(  180, 255,   0), 25); // 
  colorWipe(strip.Color(0,   128,   0), 25); // Green
  colorWipe(strip.Color(  0, 255,   125), 25); // 
  colorWipe(strip.Color(  0, 125,   125), 25); // 
  colorWipe(strip.Color(  0, 125,   255), 25); // 
  colorWipe(strip.Color(  0, 0,   125), 25); // 
  colorWipe(strip.Color(  45, 0,   200), 25); // 
  colorWipe(strip.Color(  75,   0, 130), 25); // Purple
  colorWipe(strip.Color(  200, 0,   200), 25); // 
  colorWipe(strip.Color(  238,  130, 138), 25); // Pink
}
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void smoothColorTransition(uint32_t startColor, uint32_t endColor, int duration) {
  int steps = 100; // Number of steps for the transition
  
  // Calculate the color increments for each step
  int rStep = ((endColor >> 16) & 0xFF) - ((startColor >> 16) & 0xFF);
  int gStep = ((endColor >> 8) & 0xFF) - ((startColor >> 8) & 0xFF);
  int bStep = (endColor & 0xFF) - (startColor & 0xFF);

  
  // Calculate the delay between each step based on the duration
  int delayTime = duration / steps;
  
  // Perform the color transition
  for (int i = 0; i < steps; i++) {
    int r = ((startColor >> 16) & 0xFF) + (rStep * i / steps);
    int g = ((startColor >> 8) & 0xFF) + (gStep * i / steps);
    int b = (startColor & 0xFF) + (bStep * i / steps);
    uint32_t color = (r << 16) | (g << 8) | b;
    
    strip.fill(color);
    strip.show();
    
    delay(delayTime);
  }
}

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);

    // Register multi WiFi networks
  wifiMulti.addAP("Lukscha", "sivonebo");
  wifiMulti.addAP("10701D", "pyrggfe3zo");
  wifiMulti.addAP("Mayas handy", "mamamama");

  Serial.println();
  Serial.println();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Connect to Wi-Fi
  if(wifiMulti.run()==WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.println("Connecting to: ");
    display.println(WiFi.SSID());
    display.display();
    delay(3000);
  } else display.println("Wifi not connected!");

  // Print the IP address
  Serial.println("WiFi connected");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(WiFi.SSID());
  display.setCursor(0,10);
  display.println("IP address:");
  display.setCursor(0,20);
  display.setTextSize(0.8);
  display.println(WiFi.localIP().toString());
  display.display();

  Serial.println(WiFi.localIP());

  // Initialize the NeoPixel strip
  strip.begin();
  strip.show(); // Initialize all pixels to off

  // Handle the root page request
  server.on("/", handleRootPage);

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Listen for incoming requests
  server.handleClient();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(WiFi.SSID());
  display.setCursor(0,10);
  display.println("IP address :");
  display.setCursor(0,20);
  display.setTextSize(0.8);
  display.println(WiFi.localIP().toString());
  display.display();

if (server.method() == HTTP_POST && server.arg("colorSubmit") != "") {
    // Read the RGB values from the form fields
    display.setCursor(0,40);
    display.println("Solid Color");
    display.display();
    redValue = server.arg("red").toInt();
    greenValue = server.arg("green").toInt();
    blueValue = server.arg("blue").toInt();
    changeColor();
  }

  // If the form was submitted for brightness selection
  if (server.method() == HTTP_POST && server.arg("brightnessSubmit") != "") {
    // Read the brightness value from the form field
    brightnessValue = server.arg("brightness").toInt();
    changeBrightness();
  }
  
  // Apply color effect if requested
  if (server.method() == HTTP_POST && server.arg("colorWipe") != "") {
    display.setCursor(0,40);
    display.println("Color Wipe");
    display.display();
    colorWipe(strip.Color(redValue, greenValue, blueValue), 35);
  }

  if (server.method() == HTTP_POST && server.arg("rainbow") != "") {
      display.setCursor(0,40);
      display.println("Rainbow");
      display.display();
      rainbow(50);
  }
  
  if (server.method() == HTTP_POST && server.arg("rainbowWipe") != "") {
      display.setCursor(0,40);
      display.println("Rainbow Wipe");
      display.display();
      rainbowWipe();

  }

  if (server.method() == HTTP_POST && server.arg("colorfulWipe") != "") {
    display.setCursor(0,40);
    display.println("Colorful Wipe");
    display.display();
    colorfulWipe();
  }

  if (server.method() == HTTP_POST && server.arg("theaterChase") != "") {
    display.setCursor(0,40);
    display.println("Theater Chase");
    display.display();
    theaterChase(strip.Color(redValue, greenValue, blueValue), 50);
  }

  if (server.method() == HTTP_POST && server.arg("relaxedTransition") != ""){
    display.setCursor(0,40);
    display.println("Relaxed");
    display.display();
    smoothColorTransition(strip.Color(255, 0, 0), strip.Color(0, 0, 255), 5000);
    smoothColorTransition(strip.Color(0, 0, 255), strip.Color(0, 255, 0), 5000);
    smoothColorTransition(strip.Color(0, 255, 0), strip.Color(255, 0, 0), 5000);

  }

  if (server.method() == HTTP_POST && server.arg("notSoRelaxedTransition") != ""){
    display.setCursor(0,40);
    display.println("Party");
    display.display();
    smoothColorTransition(strip.Color(255, 0, 0), strip.Color(0, 0, 255), 500);
    smoothColorTransition(strip.Color(0, 0, 255), strip.Color(0, 255, 0), 500);
    smoothColorTransition(strip.Color(0, 255, 0), strip.Color(255, 0, 0), 500);

  }

  delay(100);
}