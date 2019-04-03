/*
  <form action="http://192.168.4.1/upload"method="post" enctype="multipart/form-data">
    <input type="file" name="name">
    <input class="button" type="submit" value="Upload">
</form>
*/
static const uint8_t D0   = 5;
static const uint8_t D1   = 4;
static const uint8_t D2   = 0;
static const uint8_t D3   = 2;
static const uint8_t D4   = 16;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

  char c;

#define COMMAND_SIZE 128
char word1[COMMAND_SIZE];
byte serial_count;
int no_data = 0;

int X_STEPS_PER_MM=128; 
int Y_STEPS_PER_MM=128;
int FAST_XY_FEEDRATE=200;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>   // Include the SPIFFS library

String ssid;
String password;

ESP8266WebServer server(80);

String responseHTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
  "be redirected here.</p></body></html>";


File fsUploadFile;              // a File object to temporarily store the received file

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
void handleFileUpload();                // upload a new file to the SPIFFS

void setup() {
  init_steppers();
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
//**************************************************************
SPIFFS.begin(); 


ssid=LoadDataFromFile("ssid.txt");
password=LoadDataFromFile("password.txt");

ESP.eraseConfig();
WiFi.disconnect(true);
WiFi.mode(WIFI_AP);
WiFi.softAP("Costycnc");
  //Serial.printf("Try connecting to %s ", ssid1);
  WiFi.begin(ssid, password);
  int attempt=0;
  while ((WiFi.status() != WL_CONNECTED) && attempt<10)//try 10 times to connect to router 
    // if not succeed ... connect locally
  {
    attempt ++;
    delay(500);
    Serial.print(attempt);

  }
  if(attempt < 10){
  Serial.println(" connected to router and local");}
    else{
      Serial.println(" connected only local");
}
                          // Start the SPI Flash Files System

readparam();

  server.begin();
  Serial.println("TCP server started");
  server.on("/upload", HTTP_GET, []() {                 // if the client requests the upload page
    if (!handleFileRead("/upload.html"))                // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.on("/upload", HTTP_POST,                       // if the client posts to the upload page
    [](){ server.send(200); },                          // Send status 200 (OK) to tell the client we are ready to receive
    handleFileUpload                                    // Receive and save the file
  );



  
/*
   server.on("/header", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request-&gt;beginResponse(200, "text/plain", "Ok");
    response-&gt;addHeader("Test-Header", "My header value");
    request-&gt;send(response);
  });
*/
  server.on("/costycnc",costycnc);
  server.on("/grbl",grbl);
  server.on("/speed1",speed1);
  server.on("/blink",blink1);
  server.on("/fileread",fileread);
  server.on("/list",list);
  server.on("/svg",svg);
  server.on("/potracex",potracex);
  server.on("/arguments",arguments);
  server.on("/absolute",absolute);
   server.on("/string",string);
    server.on("/rssi",rssi);
    server.on("/header",header);
     server.on("/costel",costel);
     server.on("/image",image);
     server.on("/xplus",xplus);
     server.on("/xminus",xminus);
     server.on("/yplus",yplus);
     server.on("/yminus",yminus);

 

 // replay to all requests with same HTML
 // server.onNotFound([]() {
 //   server.send(200, "text/html", responseHTML);
 // });
  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop() {
   //dnsServer.processNextRequest();
  server.handleClient();
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void handleFileUpload(){ // upload a new file to the SPIFFS
  
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Access-Control-Allow-Origin", "*");
      //server.sendHeader("Location","/success.html");      // Redirect the client to the success page
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}
