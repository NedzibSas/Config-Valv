#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <FirebaseArduino.h>

const char* base       = "myurl.firebaseio.com";     //Your Firebase url
const char* auth       = "********************"; //Your Firebase Auth
String ssid1,s;
String pass1;
char dat,o;
int addr = 1;
int ABR = 13; // GPIO13
int CIE = 12; // GPIO12
int bab = 4;
int bce = 5;
int bab1 = 15;
int bce1 = 14;
int con=0,con1=0,con2=0,con3=0,con4=0;
byte value=1;

void setup(){
  Serial.begin(9600);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ABR, OUTPUT);
  pinMode(10, INPUT);
  pinMode(CIE, OUTPUT);
  pinMode(bab, INPUT);
  pinMode(bce, INPUT);
  pinMode(bab1, INPUT);
  pinMode(bce1, INPUT);
  digitalWrite(ABR, LOW);
  digitalWrite(CIE, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  EEPROM.begin(512);
  Firebase.begin(base, auth); 
  
}

//===============================================================
void loop() {

if(digitalRead(10) == HIGH) {
  while(true){
  if(con1==0){
    
  Serial.println("Modo Config");
  while(true){
    Serial.print('c');
    delay(10);
    if (Serial.available()) {
      char c=Serial.read();
      if(c='c'){
        digitalWrite(ABR, HIGH);
        digitalWrite(CIE, HIGH);
        break;}
    }}
  Serial.println("\nPreparado para recibir datos");
  con1=1;}
  ////////////////////////////////////////Escritura eeprom////////////////////////////////////////
    if (Serial.available()) {
    dat = Serial.read();
    EEPROM.write(addr, dat);
    addr=addr+1;
    EEPROM.commit();
  }
      if(dat==']'){
      Serial.println("Configuracion completa.");
      Serial.println("Por favor reinicie el dispositivo");}
  delay(10);
  }}

  
  if(con==0){
  ///////////////////////////////////////////lectura eeprom////////////////////////////////////////
  while(con3==0){
    value = EEPROM.read(addr);
    o=value;
    if(o!='[' && con2==0){ssid1+=o;}
    if(o!=']' && con2==1){pass1+=o;}
    if(o=='['){con2=1;}
    if(o==']'){
      Serial.print("Ssid: \t");
      Serial.print(ssid1);
      Serial.println();
      Serial.print("Pass: \t");
      Serial.print(pass1);
      Serial.println();con3=1;}
    addr = addr + 1;
  if (addr == 512){addr = 0;}
  }    
  //////////////////////////////////////////////Conexion a WiFi//////////////////////////////
  Serial.println();
  Serial.println();
  Serial.print("Conectando a: ");
  const char*ssid=ssid1.c_str();
  const char*pass=pass1.c_str();
  
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado");
  con=1;
  }
////////////////////////////////////Peticion & Push - Firebase///////////////////////////////
        String path="estados";  
        String pathc="control";
        FirebaseObject valvula= Firebase.get(path);
        FirebaseObject control= Firebase.get(pathc);
        boolean t=valvula.getBool("Valvula1");
        int g=t;
        con4=control.getInt("var1");
        delay(50);
////////////////////////////////Apertura por red//////////////////////////        
        if(g==1 && con4==0){
          while(true){
          digitalWrite(ABR, HIGH);
          delay(10);
          if(digitalRead(bab) == HIGH) {
          digitalWrite(ABR, LOW);
          
          Firebase.setInt("control/var1", 1);
          if (Firebase.failed()) {
            Serial.print("failed");
            Serial.println(Firebase.error()); 
            return;
          }else{Serial.println("Var Ok");}

          break;
          }}}

          if(g==0 && con4==1){
          while(true){
          digitalWrite(CIE, HIGH);
          delay(10);
          if(digitalRead(bce) == HIGH) {
          digitalWrite(CIE, LOW);
          
          Firebase.setInt("control/var1", 0);
          if (Firebase.failed()) {
            Serial.print("failed");
            Serial.println(Firebase.error()); 
            return;
          }else{Serial.println("Var Ok");}

          break;
          }}}
///////////////////////////////////////////////Apertura Manual/////////////////
        if(digitalRead(bab1) == HIGH && con4==0){
          while(true){
          digitalWrite(ABR, HIGH);
          delay(10);
          if(digitalRead(bab) == HIGH) {
          digitalWrite(ABR, LOW);
          
          Firebase.setBool("estados/Valvula1", true);
          if (Firebase.failed()) {
            Serial.print("failed");
            Serial.println(Firebase.error()); 
            return;
          }else{Serial.println("Estado Ok");}
          
          Firebase.setInt("control/var1", 1);
          if (Firebase.failed()) {
            Serial.print("failed");
            Serial.println(Firebase.error()); 
            return;
          }else{Serial.println("Var Ok");}

          break;
          }}}

          if(digitalRead(bab1) == HIGH && con4==1){
          while(true){
          digitalWrite(CIE, HIGH);
          delay(10);
          if(digitalRead(bce) == HIGH) {
          digitalWrite(CIE, LOW);

          Firebase.setBool("estados/Valvula1", false);
          if (Firebase.failed()) {
            Serial.print("failed");
            Serial.println(Firebase.error()); 
            return;
          }else{Serial.println("Estado Ok");}
          
          Firebase.setInt("control/var1", 0);
          if (Firebase.failed()) {
            Serial.print("failed");
            Serial.println(Firebase.error()); 
            return;
          }else{Serial.println("Var Ok");}

          break;
          }}}
delay(10);
}
