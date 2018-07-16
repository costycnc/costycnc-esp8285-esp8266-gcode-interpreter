
char movex[]="X10\n";
char movey[]="Y10\n";


void list()
{
  String str = "<p>go to <a href='http://192.168.4.1'>index</a></p>";
Dir dir = SPIFFS.openDir("/");
while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\r\n";
   
    
}
Serial.print(str);  
server.send(200, "text/html", str);
}


void test()
{


 String str = "<p>go to <a href='http://192.168.4.1'>index</a></p>";
 str +="<h1> X step/mm=";
 str +=X_STEPS_PER_MM ;
 str +="</h1>";
 
 str +="<h1> Y step/mm=";
 str +=Y_STEPS_PER_MM ;
 str +="</h1>";

 str +="<h1> Feedrate=";
 str +=FAST_XY_FEEDRATE ;
 str +="</h1>";
   //server.send(200, "text/plain", "este un test");
str +="<br>";
str +="<h1><form action='http://192.168.4.1/arguments'>";

str +="feedrate: <input type='text' name='feedrate' value=";
str +=FAST_XY_FEEDRATE;
str +="><br>";

str +="feedrate: <input type='text' name='stepx' value=";
str +=X_STEPS_PER_MM;
str +="><br>";

str +="feedrate: <input type='text' name='stepy' value=";
str +=Y_STEPS_PER_MM;
str +="><br>";

str +="<input type='submit' value='Set parameters'>";
str +="</form></h1>";

   /*  <form action="http://192.168.4.1/arguments">
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
   server.send(200, "text/html", String("<p>go to <a href='http://192.168.4.1'>index</a></p>") ); 

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
    message = "Again-> <p><a href='http://192.168.4.1/grbl'>http://192.168.4.1/grbl</a></p>";
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
  ;//Use http://192.168.4.1/arguments?feedrate=100
 }

 if (server.arg("stepx")== ""){
  }else{     //Parameter found
  message += "stepx = ";
  String valx=server.arg("stepx");
  X_STEPS_PER_MM=valx.toInt();
  message += X_STEPS_PER_MM;     //Gets the value of the query parameter
  ;//Use http://192.168.4.1/arguments?stepx=100
 }

 if (server.arg("stepy")== ""){
  }else{     //Parameter found
  message += "stepy = ";
  String valx=server.arg("stepy");
  Y_STEPS_PER_MM=valx.toInt();
  message += Y_STEPS_PER_MM;     //Gets the value of the query parameter
  ;//Use http://192.168.4.1/arguments?stepy=100
 }

 
 message += "<p>go to <a href='http://192.168.4.1'>index</a></p></h1>";
  server.send(200, "text/html", message);          //Returns the HTTP response


}

void costycnc() { 
 String message = "";

  if (server.arg("Feedback")== ""){     //Parameter not found

  message = "Feedback Argument not found";

  }else{     //Parameter found

  message = "Feedback Argument = ";
  message += server.arg("Feedback");     //Gets the value of the query parameter
  ;//Use http://192.168.4.1/costycnc?Feedback=100

 message += "<p>go to <a href='http://192.168.4.1'>index</a></p>";
 }

  server.send(200, "text/html", message);          //Returns the HTTP response


}

bool hFileRead() { // send the right file to the client (if it exists)
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
   message += "go to index -><a href='http://192.168.4.1/index.html'>http://192.168.4.1/index.html</a></p>";

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

  


