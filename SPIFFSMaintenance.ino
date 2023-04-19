#include <SPIFFS.h>

void setup() {
Serial.begin(115200);
if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

File root = SPIFFS.open("/");

File file = root.openNextFile();
while(file){
  Serial.println(file.name());
  file = root.openNextFile();
}

//To format SPIFFS and clear its cache: 

bool formatted = SPIFFS.format();
if(formatted){
    Serial.println("\n\nSuccess formatting");
}else{
    Serial.println("\n\nError formatting");
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
