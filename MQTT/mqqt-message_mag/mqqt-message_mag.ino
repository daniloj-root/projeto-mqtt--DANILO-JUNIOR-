#include <PubSubClient.h>

#include <UIPEthernet.h>
#include <utility/logging.h>

#include <SPI.h>

//Define o endereço MAC que será utilizado
byte mac[] = {0x18, 0x51, 0xE9, 0xD8, 0x79, 0xB0};

bool resposta, estadoSensorMag;

const int pinoSensorMag = 2;

const char MENSAGEM_ABERTO[6] = "ABERTO";
const char MENSAGEM_FECHADO[7] = "FECHADO";

//Inicia o cliente Ethernet
EthernetClient client;

//Inicia o cliente MQQT
PubSubClient mqttClient(client);

void setup() {
    //Inicia o controlador Ethernet e solicita um IP para o servidor de DHCP
    Ethernet.begin(mac);

    //Inicia o monitor Serial
    Serial.begin(9600);

    mqttClient.setServer("54.161.191.80", 1883);

    //Exibe no Monitor Serial as informações sobre o IP do Arduino
    Serial.print("O IP do Arduino e: ");
    Serial.println(Ethernet.localIP());

    //Exibe no Monitor Serial as informações sobre a Máscara de Rede do Arduino
    Serial.print("A Mascara de Rede do Arduino e: ");
    Serial.println(Ethernet.subnetMask());

    //Exibe no Monitor Serial as informações sobre o Gateway do Arduino
    Serial.print("O Gateway do Arduino e: ");
    Serial.println(Ethernet.gatewayIP());

    //Exibe uma linha em branco
    Serial.print('\n');

    // Inicializa o sensor magnético
    pinMode(pinoSensorMag, INPUT_PULLUP);
}

void loop() {  
  estadoSensorMag = digitalRead(pinoSensorMag);
  
  // Conecta o cliente como 'daniloj'
  mqttClient.connect("daniloj");

  delay(100);
    
  if(estadoSensorMag == 1){
    resposta = mqttClient.publish("daniloj-t", MENSAGEM_ABERTO);
  }
  else{
    resposta = mqttClient.publish("daniloj-t", MENSAGEM_FECHADO);
  }

  if (resposta == 1)
  {
    Serial.println("O servidor MQQT recebeu a mensagem"); 
  }
  else
  {
    Serial.println("O servidor MQQT não recebeu a mensagem"); 
  }
  
  mqttClient.loop();
}
