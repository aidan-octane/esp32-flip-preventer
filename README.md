# esp32-flip-preventer
Code for the ESP32 AI-Thinker Camera and an MPU6050 Gyroscope that will detect when the device is flipped, take a picture, and email the picture to a recipient. 


The actual device is the ESP32 camera, the gyroscope, and an onboard LED light soldered to a 70x30mm perf-board and screwed into a 3D-printed housing container. This device - aptly named the KACHOW (it does not stand for anything, I just liked the name) - will attach to the dashboard of a racecar that I drive, a dirt-oval sprint car. These cars often are involved in violent crashes in which they flip. My mother does not like this fact. So, to motivate myself to not flip, I've developed the KACHOW, which will:
  A) Detect when I'm involved in an accident in which my car flips.
  B) Take a picture of myself in the car. I will probably be rather silly looking, as I am currently being chucked upside down at an estimated 90-110 miles an hour. 
  C) Save the picture in an onboard file repository named SPIFFS, then connect to Internet from my mobile hotspot.
  D) Send the picture to my mother.
  
Obviously, this contraption - which has taken three months of my life, is still not entirely complete (I screwed up the soldering), and cost me hundreds of dollars in total (though the finalized device costs approximately $15 to create), is incredibly useful, as it is designed to motivate me to not crash. I think it's working - it looks very scary, and it's bright red LED signal light that says "I'm ready to go. I'm just waiting for you to mess up, pal" is very scary. 

I will update this ReadMe when I crash, which will be the only time that I truly know if the code fully works to detect a flip.
  
