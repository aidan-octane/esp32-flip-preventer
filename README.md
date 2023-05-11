# esp32-flip-preventer
My project for the ESP32 AI-Thinker Camera and an MPU6050 Gyroscope that will detect when the device is flipped, take a picture, and email the picture to a recipient. 

The finished product:
I designed my own PCB for it, and the physical product consists of an ESP-32 Ai Thinker microcontroller-camera, an MPU6050 I2C gyroscope, multiple header pins for the primary purpose of uploading code, a 9v battery with independent power switch, and a surface-mounted signal LED. This is all housed inside of a 3d-printed container with a laser-cut acrylic lid that will allow the camera to see through and will allow me to see the signal lights. For all of this, I used the program EasyEDA to design the PCB, JCLPCB to order/assemble it, and Inventor to design the box/lasercut the acrylic. The code was developed in ArduinoIDE in C++. 

This device - aptly named the KACHOW (it does not stand for anything, I just liked the name) - will attach to the dashboard of a racecar that I drive, a dirt-oval sprint car. 
These cars often are involved in violent crashes in which they flip. My mother does not like this fact. So, to motivate myself to not flip, I've developed the KACHOW, which will:
  A) Detect when I'm involved in an accident in which my car flips.
  B) Take a picture of myself in the car. I will probably be rather silly looking, as I am currently being chucked upside down at an estimated 90-110 miles an hour. 
  C) Save the picture in an onboard file repository named SPIFFS, then connect to Internet from my mobile hotspot.
  D) Send the picture to my mother.
  
Obviously, this contraption - which has taken three months of my life, is still not entirely complete (I screwed up the soldering), and cost me hundreds of dollars in total (though the finalized device costs approximately $15 to create) - is incredibly useful, as it is designed to motivate me to not crash. I think it's working - it looks very scary, and it's bright red LED signal light that says "I'm ready to go. I'm just waiting for you to mess up, pal" is ominous to say the least while I drift a 1500-lb high-octane monster into a corner at 110 miles an hour. 

4/19/23:
I will update this ReadMe when I crash, which will be the only time that I truly know if the code fully works to detect a flip.
If you are reading this before I update it, assume that I am a very good racecar driver who never crashes.

