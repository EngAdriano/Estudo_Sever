/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Importe as bibliotecas necessárias
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Substitua pelas suas credenciais de rede
const char* ssid = "Lu e Deza";
const char* password = "liukin1208";

// Definir LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Crie o objeto AsyncWebServer na porta 80
AsyncWebServer server(80);

// Substitui o espaço reservado pelo valor do estado do LED
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}
 
void setup(){
  // Porta serial para fins de depuração
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Inicializa o sistema de arquivos SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("Ocorreu um erro ao montar o SPIFFS");
    return;
  }

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando-se ao WiFi..");
  }

  // Mostra o endereço IP local ESP32
  Serial.println(WiFi.localIP());

  // Rota para raiz/página da web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Rota para carregar o arquivo style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Rota para definir GPIO como HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Rota para definir GPIO como LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Iniciar servidor
  server.begin();
}
 
void loop(){
  
}