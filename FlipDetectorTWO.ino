//Trying to fix the code.

/*
Notes for LEDs
  LED 1: Signifies unit being initialized / picture being taken.
    Turns on when unit is initialized properly & active - blinks rapidly once picture is taken. Turns off when e-mail is sent.
  LED 2: Signifies WiFi connection
    When a picture is taken, and LED 1 is blinking rapidly, LED 2 begins slowly blinking, implying that it is searching for a signal.
    When it successfully connects to WiFi, it will stay constantly on.
*/

//Definitions for E-MAIL portions: 
#include "esp_camera.h"
#include "SPI.h"
#include "driver/rtc_io.h"
#include "ESP32_MailClient.h"
#include "FS.h"               
#include <SPIFFS.h>
#include <WiFi.h>
//#include <EEPROM.h>

int pictureNumber = 0;

// REPLACE WITH YOUR NETWORK CREDENTIALS
// const char* ssid = "umd-iot";
// const char* password = "zqa8cb2pu5xy";
// const char* ssid = "Lindsay WiFi";
// const char* password = "pwd611493";
// const char* ssid = "MyAltice adb1cd";
// const char* password = "70-cobalt-5376";
const char* ssid = "aidan";
const char* password = "poopsock";

// To send Emails using Gmail on port 465 (SSL), you need to create an app password: https://support.google.com/accounts/answer/185833
#define emailSenderAccount    "aidanjustflipped@gmail.com"
#define emailSenderPassword   "gggysfhmslqhajfj"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "Ouch!!!!ooof ouchie ow ow ow !"
#define emailRecipient        "aidanaborden@gmail.com"

#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22
  #define FLASH 4
  #define RED_LED 2
#else
  #error "Camera model not selected"
#endif

// The Email Sending data object contains config and data to send
SMTPData smtpData;

// Photo File Name to save in SPIFFS
String FILE_PHOTO;

//Definitions for GYROSCOPE portions: 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Math.h>

Adafruit_MPU6050 mpu;
const int ledPin = 8;
bool didChange = false, initializedProperly = true;
int value, orientation; 

//Formats GYROSCOPE to ESP32 pins
#define I2C_SDA 15
#define I2C_SCL 14

TwoWire I2CMPU = TwoWire(0);

void setup() {
   Serial.begin(115200);
   Serial.println("Initialized");
   pinMode (RED_LED, OUTPUT);

    //CAMERA
  //Initialize SPIFFS 
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    initializedProperly = false;
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

  //Labels the correct name for the file - Useful in case need to go back into SPIFFS
  File root = SPIFFS.open("/");

  File count = root.openNextFile();
  while(count){
    pictureNumber++;
    count = root.openNextFile();
  }

  FILE_PHOTO = "/photo" + String(pictureNumber) + ".jpg";
  Serial.println("FILE_PHOTO: " + FILE_PHOTO);

  //Defines camera 
   camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 5;
  config.fb_count = 2;
 
 //Initializes camera 

  esp_err_t err = esp_camera_init(&config);
  while (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    err = esp_camera_init(&config);
    initializedProperly = false;
  }
  Serial.println("Camera Initialized");
  //Changes sensor - improves picture quality
  sensor_t * s = esp_camera_sensor_get();
  s->set_gain_ctrl(s, 1);                       // auto gain on
  s->set_exposure_ctrl(s, 1);                   // auto exposure on
  s->set_awb_gain(s, 1);                        // Auto White Balance enable (0 or 1)
  s->set_brightness(s, 1);                     // (-2 to 2) - set brightness

    //GYROSCOPE:
  //Initializes gyroscope
 I2CMPU.begin(I2C_SDA, I2C_SCL, 100000);
  bool status;
  status = mpu.begin(0x68, &I2CMPU);  

  if (!status) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
    initializedProperly = false;
  }

  //Not sure if these setters are necessary but they were in sample code :3
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  //Gets the sensor reading on startup to intiialize the a/g values
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //Determines whether the orientation of gyroscope is H or V based on whether Z or Y values are bigger
  if ((fabs(a.acceleration.z) > fabs(a.acceleration.y) && fabs(a.acceleration.z) > fabs(a.acceleration.x))) {
    orientation = 3;  //3 = z orientation, or 'flat'
    //Sets the start value to the Z acceleration value if is horizontal
    value = a.acceleration.z;
  } else if (fabs(a.acceleration.y) > fabs(a.acceleration.x)) {
    orientation = 2;
    //Sets the start value to the Y acceleration value if is vertical
    value = a.acceleration.y;
  } else {
    orientation = 1;
    value = a.acceleration.x;
  }

  String addedString = orientation == 3 ? "Flat" : orientation == 2 ? "Vertical" : "Horizontal";
  Serial.println("Orientation: " + addedString);

  if(initializedProperly){
    Serial.println("Turn on Red LED!");
    digitalWrite(RED_LED, HIGH);
  }
}

void loop() {
  //Gets new sensor events with the readings
  sensors_event_t a, g, temp;
  if(!mpu.getEvent(&a, &g, &temp)){
    digitalWrite(RED_LED, LOW);
  }

  //Gets value
  double currValue = orientation == 3 ? a.acceleration.z : orientation == 2 ? a.acceleration.y : a.acceleration.x;
  //Prints value of what we're looking for - debugging purposes
  Serial.println("Value of orientation: " + String(currValue));


  //Determines whether the gyroscope flips simply based off of whether or not the current value - associated with whichever orientation
    //it is defined to be in - changes signs. 
    if (value > 0) {
      if (currValue < 0) {
        didChange = true;
      }
    } else {
      if (currValue > 0) {
        didChange = true;
      }
    }

  if (didChange) {
    Serial.println("Triggered!");
    //Capture photo at the current moment & save it in SPIFFS.
    pinMode(FLASH, OUTPUT);
    digitalWrite(FLASH, HIGH);
    while(!capturePhoto());
    //CaptureFiveMorePhotosJustForFun();
    //Connects to WiFi - WAITS an INFINITE amound of time until it connects
    connectToWifi();
    //E-mails photo
    sendPhoto();    
   
    //Stops execution of code
    esp_deep_sleep_start();
  }
  //Serial.print(".");
  delay(500);  //wait every half second to check for a flip
}


bool capturePhoto(){
  bool isSuccessful=true;; 
  camera_fb_t * fb = NULL; // pointer
  Serial.println("Taking a photo...");
    //activates flash, waits a little
    delay(50);
    //takes picture
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      isSuccessful=false;
    }else{
    Serial.println("Camera capture successful!");
    }
    delay(50);
    digitalWrite(FLASH, LOW);
    // Photo file name
    Serial.printf("Picture file name: %s\n", FILE_PHOTO);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);

    // Insert the data in the photo file
    if (!file) {
      Serial.println("Failed to open file in writing mode");
      isSuccessful=false;
    } else {
      file.write(fb->buf, fb->len);  // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    // Close the file
    file.close();
    esp_camera_fb_return(fb);
    return isSuccessful;
}

void connectToWifi(){
  WiFi.mode(WIFI_STA); // SETS TO STATION MODE!
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(RED_LED, LOW);
    Serial.print(".");
    WiFi.begin(ssid, password);
    delay(2000);
    digitalWrite(RED_LED, HIGH);
    delay(25);
  }
  Serial.println("WiFi connected!");
  digitalWrite(RED_LED, LOW);
  for(int i=0; i<3; i++){
  digitalWrite(FLASH, HIGH);
  delay(250);
  digitalWrite(FLASH, LOW);
  delay(250);
  }
  digitalWrite(RED_LED, LOW);
}

void sendPhoto() {
  digitalWrite(RED_LED, HIGH);
  bool isSuccessful=true;
  // Preparing email
  Serial.println("Sending email...");
  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  
  // Set the sender name and Email
  smtpData.setSender("ESP32-CAM", emailSenderAccount);
  
  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);
    
  // Set the email message in HTML format
  smtpData.setMessage("<h2>If you are recieving this, Mom, it sadly means that I have just flipped violently in the sprint car. I am probably okay. Check out this picture of me!</h2>", true);
  // Set the email message in text format
  //smtpData.setMessage("Photo captured with ESP32-CAM and attached in this email.", false);

  // Add recipients, can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient(emailRecipient2);

  // Add attach files from SPIFFS
  smtpData.addAttachFile(FILE_PHOTO, "image/jpg");
  // Set the storage type to attach files in your email (SPIFFS)
  smtpData.setFileStorageType(MailClientStorageType::SPIFFS);

  smtpData.setSendCallback(sendCallback);
  
  // Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData)){
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    isSuccessful=false;
  }
  if(isSuccessful){
    for(int i=0; i<5; i++){
    digitalWrite(FLASH, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(250);
    digitalWrite(FLASH, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(250);
    }
  }
  // Clear all data from Email object to free memory
  smtpData.empty();
}

// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  //Print the current status
  Serial.println(msg.info());
}


