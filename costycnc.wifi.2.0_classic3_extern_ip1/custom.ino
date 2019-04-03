
char movex[]="X10\n";
char movey[]="Y10\n";
char a[128]="COSTYCNC";



int x=0;



bool fileOpenFail;

String LoadDataFromFile(String fileNameForSave)
{
  String WhatIwillReturn;
  //SPIFFS.begin();
  File f = SPIFFS.open(String("/"+fileNameForSave), "r");
  if (!f)
  {
    fileOpenFail = 1;
    Serial.print("file open failed  :");
    Serial.println(fileNameForSave);
  }
  else
  {
    fileOpenFail = 0;
    WhatIwillReturn =  f.readStringUntil('\r');
    WhatIwillReturn.replace("\n", "");
    f.close();
    return WhatIwillReturn;
  }
}

void sendresp() {
  
String text= "<!DOCTYPE html><html><head>";
  text +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>";
  text +="<p>Until cnc finished moving you can not make another commands\n"; 
  text +="If cnc finished moving you can make this commands:</p>";
  text +="<p><h1><a href='/xplus'>X+ </a>";
  text +="<a href='/xminus'>X- </a>";
  text +="<a href='/yplus'>Y+ </a>";
  text +="<a href='/yminus'>Y- </a></h1></p>";
  text +="<p><a href='index.html'>index</a></p></body></html>";
   server.send(200, "text/html", text);
}

void xplus() {
  sendresp();
  set_position(0.0, 0.0, 0.0);
  String message="G01 X10 Y0\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
  disable_steppers();
}

void xminus() {
  sendresp();
  set_position(10.0, 0.0, 0.0);
  String message="G01 X0 Y0\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
  disable_steppers();
}


void yplus() {
  sendresp();
  set_position(0.0, 0.0, 0.0);
  String message="G01 X0 Y10\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
  disable_steppers();
}

void yminus() {
  sendresp();
  set_position(0.0, 10.0, 0.0);
  String message="G01 X0 Y0\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
  disable_steppers();
}


void readparam() {
  
String message="";
byte param=0;
char t;
 File f = SPIFFS.open("/settings.txt", "r");
 while(f.available()) {
     t = f.read();
     message += t;
     if(t=='\n'){ //if line end
      param++;
      if(param==1){
      X_STEPS_PER_MM=message.toInt(); 
      Serial.print("parametru1=");
      Serial.println( X_STEPS_PER_MM);
      
      }
      if(param==2){
      Y_STEPS_PER_MM=message.toInt();
      Serial.print("parametru2="); 
      Serial.println( Y_STEPS_PER_MM); 
      } 
      if(param==3){
      FAST_XY_FEEDRATE=message.toInt();
      Serial.print("parametru3=");
      Serial.println(FAST_XY_FEEDRATE);  
      }
      message="";       //empty gcode string
       }
    }
f.close();
}


void fileread() { 
  set_position(0.0, 0.0, 0.0);
String message="";
char t;

init_process_string();
server.send(200, "text/html", "until cnc not finished cut you not can not make any command!!!" );
 File f = SPIFFS.open("/test.nc", "r");
 while(f.available()) {
     t = f.read();
     message += t;
     if(t=='\n'){ //if line end
       process_string(message,message.length()); //send gcode line to gcode interpreter
       message="";       //empty gcode string
       }
    }
f.close();
disable_steppers();
} 

 void arranqueweb()
{
   server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
   server.sendHeader("Pragma", "no-cache");
   server.sendHeader("Expires", "-1");
   //server.sendHeader("Content-Length", "-1");
   server.setContentLength(CONTENT_LENGTH_UNKNOWN);
   server.send(200, "text/html", "");
   server.sendContent("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /><meta name=\"apple-mobile-web-app-capable\" content=\"yes\" /><meta name=\"apple-mobile-web-app-status-bar-style\" content=\"black-translucent\" /><style>body {font-family:arial;} </style>");
   server.sendContent("</head><body><h1>Node 1</h1>");
   server.sendContent("<h6>Node test v 0.5</h6></body></html>");
   server.client().stop();
}


/*
//first create a fixed buffer
const int bufferSize = 6000;
uint8_t _buffer[6000];

//a little counter to know at which position we are in our buffer
int bufferCounter = 0;
  
int fileSize = 80000;

//now send an empty string but with the header field Content-Length to let the browser know how much data it should expect
server.sendHeader("Content-Length", (String)fileSize);
server.send(200, "text/html", "");

for(int i=0;i<fileSize;i++){
  _buffer[bufferCounter] = random(256); //write some random bytes into our buffer
  bufferCounter++;
  
  if(bufferCounter >= bufferSize){ //when the buffer is full...
    server.sendContent_P(_buffer, bufferCounter); //send the current buffer
    bufferCounter = 0; //reset the counter
  }
  
}

//send the rest bytes if there are some
if(bufferCounter > 0){
  server.sendContent_P(_buffer, bufferCounter);
  bufferCounter = 0;
}*/

void image() { 
String message="";
String gcodex="";
String gcodey="";
char t;
int a;
String puncte="";

   server.setContentLength(CONTENT_LENGTH_UNKNOWN);
   server.send(200, "text/html", "");

server.sendContent("<!DOCTYPE html><html><head><script>document.body.innerHTML ='';</script>");
server.sendContent("<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>");
server.sendContent("<svg height='300' width='1500'><g transform='scale(2)'><polyline points='");
 File f = SPIFFS.open("/test.nc", "r");
 while(f.available()) {
     t = f.read();
     if(t=='/'){message = f.readStringUntil('\n');}
     // G01 X123.456 Y567.890\n
     //message += t;
     if(t=='X'){a=1;gcodex="";} //read line until 'X' (attention X high!!!)
     if(t=='Y'){a=2;gcodey="";} //read line until 'Y' (attention Y high!!!)
     if(t=='\n'){ //if line end
       a=0;
       
       puncte +=gcodex;
       puncte +=",";
       puncte +=gcodey;
       server.sendContent(puncte);
       puncte =" ";
       
       //message="";       //empty gcode string
       }
     if(a==1) if(t !='X' && t !=' ') gcodex +=t; //write svg coordonate without 'X' and space
     if(a==2) if(t !='Y' && t !=' ') gcodey +=t; //write svg coordonate without 'X' and space
     }
f.close();


server.sendContent("' style='fill:white;stroke:red;stroke-width:.5' />");
server.sendContent("Sorry, your browser does not support inline SVG.</svg></body></html>");
server.client().stop();
} 



void costel() { 
String message="";
String gcodex="";
String gcodey="";
char t;
int a;
String inceput= "<!DOCTYPE html><html><head><script>document.body.innerHTML ='';</script>";
inceput +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body><svg height='500' width='500'>";
inceput +="<g transform='scale(2)'><polyline points='";
String sfarsit ="' style='fill:white;stroke:red;stroke-width:.5' />";
sfarsit +="Sorry, your browser does not support inline SVG.</svg></body></html>";
String puncte="";

 File f = SPIFFS.open("/test.nc", "r");
 while(f.available()) {
     t = f.read();
     // G01 X123.456 Y567.890\n
     message += t;
     if(t=='X'){a=1;gcodex="";} //read line until 'X' (attention X high!!!)
     if(t=='Y'){a=2;gcodey="";} //read line until 'Y' (attention Y high!!!)
     if(t=='\n'){ //if line end
       a=0;
       puncte +=gcodex+','+gcodey+' ';
       server.send(100, "text/html", inceput+puncte+sfarsit); //on any new line response to server with svg + new coordinate
       process_string(message,message.length()); //send gcode line to gcode interpreter
       message="";       //empty gcode string
       
       }
     if(a==1) if(t !='X' && t !=' ') gcodex +=t; //write svg coordonate without 'X' and space
     if(a==2) if(t !='Y' && t !=' ') gcodey +=t; //write svg coordonate without 'X' and space
     }
f.close();
server.send(200, "text/html", "ok" );
} 

void svg()
{
  String gcode="G01 X1.27 Y2.00";
  char b;
  String x2;
  String y2;


 String svginceput = "<div><svg height='210' width='500'><line x1='0' y1='0' x2='";

 String svgy2 ="' y2='";
 String svgsfarsit="' style='stroke:rgb(255,0,0);stroke-width:2' />Sorry, your browser does not support inline SVG.</svg></div>";




  
   for (int i=1; i<=13; i++){
    b=gcode[i];
        if (b=='X'){
      x2="";
x2 +=gcode[i+1];
x2 +=gcode[i+2];
x2 +=gcode[i+3];
    }
        if (b=='Y'){
      y2="";
y2 +=gcode[i+1];
y2 +=gcode[i+2];
y2 +=gcode[i+3];
    }
    }
    /*


  String message = "<!DOCTYPE html><head>";
  if(x<3){ 
  message +="<meta http-equiv='refresh' content='1' />";
  }
  message +="</head><body>x=";
  message +=a;
  message += "<svg height='210' width='500'>";
  message += "<line x1='0' y1='0' x2='200' y2='200' style='stroke:rgb(255,0,0);stroke-width:2' />";
  message += "Sorry, your browser does not support inline SVG.</svg></body></html>";

   server.send(200, "text/html", message ); */
   server.send(200, "text/html", svginceput+x2+svgy2+y2+svgsfarsit);

}

void potracex() { 
 String message = "";
 String mes = "costycnc";

  String gcode=server.arg(0);
  int lastStringLength = gcode.length(); 
 
 //Use /potracex?textg=......gcode in textarea....

  if (server.arg(0)== ""){     //Parameter not found

  message = "Feedback Argument not found";

  }else{     //Parameter found






  
  message = "Gcode1 = " ;
  message +=lastStringLength;


message += "<textarea rows='4' cols='50' name='textg'>";
  
  message += gcode;     //Gets the value of the query parameter
  message += "</textarea>";
   message += "<p>go to <a href='http://192.168.4.1'>index</a></p>";

 }

  //server.send(100, "text/html", message);  
 // server.send(100, "text/html", "\r\nbla\r\n");//Returns the HTTP response
 // server.send(100, "text/html", "\r\naltul\r\n");
  //server.send(200, "text/html", "ok");
  
//*******************************************************
//Serial.println(server.arg(0));
 String svg="";
 String svginceput = "<div><svg height='500' width='500'><g transform='scale(10)'><polyline stroke='black' fill='none' stroke-width:.1 points='";
 String svgsfarsit="' />Sorry, your browser does not support inline SVG.</g></svg></div>";

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//<script>document.body.innerHTML = '';</script>
String inceput = "<html><head><script>document.body.innerHTML = '<html></html>';</script>";
inceput +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body><div><textarea>";
String dupaa="</textarea></div>";
String progress="<progress value='";
String sfarsit="' max='85'></progress></body></html>";
char b;
int c=0;

int j=0;
String x="0",y="0";
b=server.arg(0)[1];
 Serial.println("test=");
 Serial.println(b);
 Serial.println("=sfarsit");


int u=0;
  for (int i=1; i<=lastStringLength; i++){
    
    
    b=gcode[i];

    //***************************************************
    if (b=='X'){
      x="";
x +=gcode[i+1];
x +=gcode[i+2];
if(gcode[i+3]=='.'){
x +=",";  
}else{
x +=gcode[i+3]; 
x +=","; 
}
    }
    if (b=='Y'){
      y="";
 y +=gcode[i+1];
 y +=gcode[i+2];
if(gcode[i+3]=='.'){
y +=" ";  
}else{
y +=gcode[i+3]; 
y +=" "; 
}
    }


    //*****************************************************
   a[j]=b;
  if(b=='\n')
  {
    a[j]=0;
    
    c++;
    svg +=x;
    svg +=y;
    
server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(100, "text/html", inceput+a+dupaa+svginceput+svg+svgsfarsit+progress+c+sfarsit);
    Serial.println(a);
    process_string(a,j);
    j=-1;
  
   }
j++;
  }
  
  disable_steppers();
 // server.send(200, "text/html",String("<script>location.replace('/svg')</script>"));
  server.send(200, "text/html",String("ok")+c);


}
//*****************************************************************

void list()
{
  
  String str = "<p>go to <a href='index.html'>index</a></p>";
  String strnumar;
  int intnumar=0;
Dir dir = SPIFFS.openDir("/");
while (dir.next()) {
    str += dir.fileName();
    str += "-------";
    str += dir.fileSize();
    intnumar +=dir.fileSize();
   
    Serial.println(intnumar);
    str += " bytes<br>";
   
    
}
str +="<br><br>Free space ";
str +=500-(intnumar/1024);
str +=" Kb<br>";
str +="Used space ";
str +=intnumar/1024;
str +=" Kb";
Serial.print(str);  
server.send(200, "text/html", str);
}

//-------------------------------------------------------------------------------------------------------------
void speed1()
{

String str= "<!DOCTYPE html><html><head>";
str +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>";

 str += "<h1><p>go to <a href='index.html'>index</a></p>";
 str +=" X step/mm=";
 str +=X_STEPS_PER_MM ;

 
 str +=" Y step/mm=";
 str +=Y_STEPS_PER_MM ;


 str +=" Feedrate=";
 str +=FAST_XY_FEEDRATE ;

   //server.send(200, "text/plain", "este un test");
str +="<br>";
str +="<form action='arguments'>";

str +="Feedrate: <input type='text' name='feedrate' value=";
str +=FAST_XY_FEEDRATE;
str +="><br>";

str +="X step/mm: <input type='text' name='stepx' value=";
str +=X_STEPS_PER_MM;
str +="><br>";

str +="Y step/mm: <input type='text' name='stepy' value=";
str +=Y_STEPS_PER_MM;
str +="><br>";

str +="<input type='submit' value='Set parameters'>";
str +="</form></h1></body></html>";

   /*  <form action="/arguments">
    Username: <input type="text" name="feedrate" value=FAST_XY_FEEDRATE><br>
    <input type="submit" value="Set parameters">
</form>*/
server.send(200, "text/html", str);
}

void blink1()
{
 digitalWrite(2, HIGH); 
 delay(1000);
 digitalWrite(2, LOW); 
 delay(1000); 
  server.send(200, "text/html",String("<script>location.replace('/potracex')</script>"));
}
void grbl()
{

String message = "";

/*     //server.send(200, "text/plain", "sfarsit");
    //newlines are ends of commands.
    if (c != '\n') 
    {
       
      word1[serial_count] = c;
      serial_count++;
    }
  if (serial_count && c == '\n' )
  {
     server.send(200, "text/plain", "este n si count");
    //process our command!
    */ 
  message = "<p>go to <a href='http://192.168.4.1'>index</a></p>";
    message = "Again-> <p><a href='/grbl'>/grbl</a></p>";
   server.send(200, "text/html", word1 + message);
    process_string(movex, strlen(movex));
    process_string(movey, strlen(movey));
  
/*
    //clear command.
   */ 
    init_process_string();
     
 // }
   
}

void arguments() { 
 String message = "<h1>";

 if (server.arg("feedrate")== ""){
  }else{     //Parameter found
  message += "feedrate = ";
  String valx=server.arg("feedrate");
  FAST_XY_FEEDRATE=valx.toInt();
  message += FAST_XY_FEEDRATE;     //Gets the value of the query parameter
  ;//Use /arguments?feedrate=100
 }

 if (server.arg("stepx")== ""){
  }else{     //Parameter found
  message += "stepx = ";
  String valx=server.arg("stepx");
  X_STEPS_PER_MM=valx.toInt();
  message += X_STEPS_PER_MM;     //Gets the value of the query parameter
  ;//Use /arguments?stepx=100
 }

 if (server.arg("stepy")== ""){
  }else{     //Parameter found
  message += "stepy = ";
  String valx=server.arg("stepy");
  Y_STEPS_PER_MM=valx.toInt();
  message += Y_STEPS_PER_MM;     //Gets the value of the query parameter
  ;//Use /arguments?stepy=100
 }

 
 message += "<p>go to <a href='index.html'>index</a></p></h1>";
  server.send(200, "text/html", message);          //Returns the HTTP response


}

void costycnc() { 
 String message = "";

  if (server.arg("Feedback")== ""){     //Parameter not found

  message = "Feedback Argument not found";

  }else{     //Parameter found

  message = "Feedback Argument = ";
  message += server.arg("Feedback");     //Gets the value of the query parameter
  ;//Use /costycnc?Feedback=100

 message += "<p>go to <a href='http://192.168.4.1'>index</a></p>";
 }

  server.send(200, "text/html", message);          //Returns the HTTP response


}

bool hFileRead1() { // send the right file to the client (if it exists)
 String message = "";

  // open file for reading
 File f = SPIFFS.open("/test.nc", "r");

char *p;
char b;
char a[128];
int j=0;
  if (!f) {
      Serial.println("file open failed");
       server.send(200, "text/plain", "file open failed");
      return 0;
  }  Serial.println("====== Reading from SPIFFS file =======");
    message = "<h1>====== Reading from SPIFFS file =======</h1>";
   message += " <p>You need wait until cnc finished moving</p>";
   message += "go to index -><a href='/index.html'>/index.html</a></p>";
server.sendHeader("Access-Control-Allow-Origin", "*");
server.send(200, "text/html",message );
  // write 10 strings to file
  for (int i=1; i<=f.size(); i++){
    b=f.read();
   a[j]=b;  
  if(b=='\n')
  {
    a[j]=0;
    Serial.println(a);
    process_string(a,j);
    j=-1;
  
   }
j++;
  }
  f.close();
  disable_steppers();
 
}

bool hFileRead() { // send the right file to the client (if it exists)
 String message = "";
 String message1= "";
 String message2= "";

  // open file for reading
 File f = SPIFFS.open("/test.nc", "r");

  if (!f) {
      Serial.println("file open failed");
       server.send(200, "text/plain", "file open failed");
      return 0;
  }  Serial.println("====== Reading from SPIFFS file =======");
    message = "<html><head>";
    message +="<script>document.body.innerHTML = '';</script></head><body><h1>====== Reading from SPIFFS file =======</h1>";
   message += " <p>You need wait until cnc finished moving</p>";
   message += "go to index -><a href='/index.html'>/index.html</a></p></body></html>";
server.sendHeader("Access-Control-Allow-Origin", "*");




message1="";
 message = "<html><head>";
    message +="<script>document.body.innerHTML = '';</script></head><body><h1>====== Reading from SPIFFS file =======</h1>";
      message +="<svg xmlns='http://www.w3.org/2000/svg' version='1.1'";
      message +="width='250' height='250'><rect x='25' y='25' width='200'";
      message +="height='200' fill='lime' stroke-width='4' stroke='pink'></rect></svg></html>";

char b;
int i=0;
 while(f.available()) {

      //Lets read line by line from the file
     
      //message1 = f.readStringUntil('\n');
      b=f.read();
      i++;
      message1 +=b;
       if(b=='\n')
  {
    process_string(message1,message1.length());
    server.send(100, "text/html",message+message1);
    message1="";
    i=0;
   }
      
      process_string(message1,message1.length());
        }
  f.close();
  disable_steppers();
      server.send(200, "text/html",message1+message +" "+message1.length());
 
}

bool hFileRead2() { // send the right file to the client (if it exists)
String message;
  // open file for reading
 File f = SPIFFS.open("/test.nc", "r");
 /*
  while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('\n');
      Serial.println(line);
    }
    */ 
      while(f.available()) {
      //Lets read line by line from the file
      message += f.readStringUntil('\n');
        }
      
f.close();
server.send(200, "text/html", message );
disable_steppers();
} 



void absolute()
{
  String message="asolute=";
 float delta_x;
 float target_x=2.07;
 float current_x=1.03;

 

delta_x = fabs(target_x - current_x);
 message +=delta_x;
 message +="<br><p>go to <a href='http://192.168.4.1'>index</a></p>";
 
   server.send(200, "text/html", message ); 

}



bool string() { // send the right file to the client (if it exists)
String message;
  // open file for reading
 File f = SPIFFS.open("/test.nc", "r");
 /*
  while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('\n');
      Serial.println(line);
    }
    */ 
      while(f.available()) {
      //Lets read line by line from the file
      message += f.readStringUntil('\n');
        }
      
f.close();
server.send(200, "text/html", message );
disable_steppers();
} 

void rssi()
{
long rssi = WiFi.RSSI();
 
   server.send(200, "text/html", String(rssi)); 

}

void header()
{
 server.sendHeader("bla","bla"); 
  server.send(200,"text/html","bla bla bla");
}
