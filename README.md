# costycnc-wifi
Now costycnc serial program grbl decoder for 2 unipolar steppers in in wifi.

Open wifi a try costycnc... connect it ... in case that your phone say " not internet" ... connect anyway.
open upload.html and load all files from folder "toate" ... one by one
You can verify files uploadet to esp8266 with http://192.168.4.1/list
http://192.168.4.1   for index page
test.nc neeed to be your gcode file that you want cut.
So... create your gcode file ,rename to test.nc and upload to esp.
Begin cut with http://192.168.4.1/fileread
test.nc that actually is in this folder is a test cut.
This variant is in acces point mode that mean that you not can use your internet in same time.
need to disconnect from costycnc wifi and connect to your wifi ... but in future i work to resolve this problem.

The program can be uploaded in esp8266 with arduino settings:
Board:"Generic ESP8266 Module"
Flash Mode: "DIO"
Flash Size: "4M(3M SPIFFS)"
.
.
.
Reset Method: "nodemcu"
.
.
.
Upload Speed: "921600"   (maxim is your pc permit!!!!)
Erase Flash : "Only Sketch"  ( files uploaded remain in esp module)
www.costycnc.it






