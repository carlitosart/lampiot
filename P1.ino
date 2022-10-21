// Importamos las librerías
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
// Sustituye los datos de tu red WIFI ( el nombre y la contraseña )
#include "config.h"
int page = 0;
//
#define ADC_VREF_mV    3300.0 // 3.3v en millivoltios
#define ADC_RESOLUTION 4096.0
#define LIGHT_SENSOR_PIN       36 // ESP32 pin GIOP36 (ADC0) conectado al LDR                
int datoADC;
float porcentaje=0.0;
float factor=100.0/ADC_RESOLUTION;


#define LED_PIN           19  

#define PWM2_Ch    1
#define PWM2_Res   8
#define PWM2_Freq  1000
int PWM2_DutyCycle = 0;


// Puesta de LED GPIO
const int ledPin = 2;
int PinLedR = 22;
int PinLedG = 18;
int PinLedB = 19;  
// Para guardar el estado del LED                
int estadoR = LOW;                          //Definimos la variable que va a recoger el estado del LED
int estadoG = LOW; 
int estadoB = LOW; 
String ledStateG;
int estado = LOW;                         //Definimos la variable que va a recoger el estado del LED
int pulsador = 23;                        // Estado del pulsador
int contador = 0;
const int potPin=34;

// Creamos el servidor AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// leemos la temperatura y la mostramos
String getTemperature() {
    float adc = analogRead(A0);
    Serial.println(adc);
    return String(adc);
}
// leemos la humedad y la mostramos
String getHumidity() {
    float boton = digitalRead(pulsador);
    Serial.println(boton);
    return String(boton);
}
// leemos la presion y la mostramos
String getPressure() {
    float rssi = WiFi.RSSI();
    Serial.println(rssi);
    return String(rssi);
}


// Remplazamos el marcador con el estado del  LED
String processor(const String& var)
{
    Serial.print(var+" : ");
    //esta función primero verifica si el marcador de posición es el ESTADO que hemos creado en el archivo HTML.
    if(var == "ESTADO")
    {   //Si lo está, entonces, de acuerdo con el estado del LED, ponemos la variable ledState en ON u OFF.
        if(digitalRead(PinLedG))
            { ledStateG = "ON"; }
        else{ ledStateG = "OFF";  }
       // Imprimimos el estado del led ( por el COM activo )
       //Finalmente, se devuelve la variable ledState. Esto reemplaza el marcador de posición STATE con el valor de cadena ledState.
       Serial.println(ledStateG);
       return ledStateG;
    }
    else if (var == "ADC"){
      return getTemperature();
    }
    else if (var == "PULSADOR"){
       return getHumidity();
    }
    else if (var == "RSSI"){
       return getPressure();
    }
}

void setup()
{
  // Establecemos la velocidad de conexión por el puerto serie
  Serial.begin(115200);
  //PWM
  Serial.println("Ejemplo de Sensor LDR ");
  pinMode(LED_PIN,OUTPUT );
  digitalWrite(LED_PIN, LOW); 
  ledcAttachPin(19, 0);

    ledcAttachPin(LED_PIN, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);
  //PWM
  // Ponemos a  ledPin  como salida
  pinMode(ledPin, OUTPUT);
  pinMode(PinLedG, OUTPUT);                //Inicializamos el GPIO2 como salida
  digitalWrite(PinLedG, LOW);              //Dejamos inicialmente el GPIO2 apagado
  pinMode(pulsador,INPUT);
  // Iniciamos  SPIFFS
  if(!SPIFFS.begin())
     { Serial.println("ha ocurrido un error al montar SPIFFS");
       return; }
  // conectamos al Wi-Fi
  WiFi.begin(ssid, password);
  // Mientras no se conecte, mantenemos un bucle con reintentos sucesivos
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      // Esperamos un segundo
      Serial.println("Conectando a la red WiFi..");
    }
  Serial.println();
  Serial.println(WiFi.SSID());
  Serial.print("Direccion IP:\t");
  // Imprimimos la ip que le ha dado nuestro router
  Serial.println(WiFi.localIP());
  
  // Ruta para cargar el archivo index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 0;
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/opcion_b.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 1;
    request->send(SPIFFS, "/opcion_b.html", String(), false);
  });

  server.on("/opcion_c.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 2;
    request->send(SPIFFS, "/opcion_c.html", String(), false);
  });
  server.on("/opcion_d.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 3;
    request->send(SPIFFS, "/opcion_d.html", String(), false);
  });

  server.on("/opcion_e.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 4;
    request->send(SPIFFS, "/opcion_e.html", String(), false);
  });

  server.on("/opcion_f.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 5;
    request->send(SPIFFS, "/opcion_f.html", String(), false);
  });
  // Ruta para cargar el archivo style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/style.css", "text/css");
            });
            
// lectura de botones  
  // Ruta para poner el GPIO alto
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
            digitalWrite(PinLedG, HIGH);
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });
  
  // Ruta para poner el GPIO bajo
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
            digitalWrite(PinLedG, LOW);
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });
// Refresco de datos enla pagina mediante script  
  server.on("/adc", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send_P(200, "text/plain", getTemperature().c_str());
            });
  server.on("/pulsador", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send_P(200, "text/plain", getHumidity().c_str());
            });
  server.on("/RSSI", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send_P(200, "text/plain", getPressure().c_str());
            });
// Inicio del servidor
  server.begin();
}

void loop(){
  int sensorValue = analogRead(A0)/4;
  digitalWrite(PinLedG, sensorValue);
   // lectura del dato analogico (valor entre 0 y 4095)
  datoADC = analogRead(LIGHT_SENSOR_PIN);
  porcentaje=factor*datoADC;
  Serial.print("Valor Analogico = ");
  Serial.print(datoADC);   
  Serial.print("  Porcentaje = ");
  Serial.print(porcentaje);  
  if (datoADC < 40) {
    Serial.println("%  => Oscuro");
    ledcWrite(PWM2_Ch,  100); 
  } else if (datoADC < 800) {
    Serial.println("% => Tenue");
    ledcWrite(PWM2_Ch,  400); 
  } else if (datoADC < 2000) {
    Serial.println("% => Claro");
    ledcWrite(PWM2_Ch,  600); 
  } else if (datoADC < 3200) {
    Serial.println("% => Luminoso");
    ledcWrite(PWM2_Ch,  800); 
  } else {
    Serial.println("% => Muy Luminoso");
    ledcWrite(PWM2_Ch,  1000); 
  }
  delay(500);
}
