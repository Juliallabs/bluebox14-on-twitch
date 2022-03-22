/*******************************************************************

    Connect to Twtich Chat with a Bot
   Created with code from TheOtherLoneStar (https://www.twitch.tv/theotherlonestar)
   Hackaday IO: https://hackaday.io/otherlonestar
   By Brian Lough (https://www.twitch.tv/brianlough)
   YouTube: https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
Created with code from noycebru www.twitch.tv/noycebru
 *******************************************************************/
#include <Arduino.h>
#include <FC16.h> // Biblioteca para controle da matriz de LED
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <IRCClient.h>
#include "senhas.h"

//define your default values here, if there are different values in config.json, they are overwritten.
#define secret_ssid "my ssid" 
#define IRC_SERVER   "irc.chat.twitch.tv"
#define IRC_PORT     6667
 
//------- Replace the following! ------

//The name of the channel that you want the bot to join
const String twitchChannelName = "julialabs";
 
//The name that you want the bot to have
#define TWITCH_BOT_NAME "julialabs"
 
//OAuth Key for your twitch bot
// https://twitchapps.com/tmi/


#define pino_CS D3 // Pino de conexão entre o display de LED e o Wemos
#define qte_displays 4 // Define o número de displays da matriz
#define scroll_delay 100 // Tempo (em milissegundos) para atualização dos LEDs 
const int tamanhoArray = 30;
char mensagem[tamanhoArray]; // Variável que armazena os caracteres da mensagem recebida
char msg_display[tamanhoArray];
unsigned long previousMillis = 0; // Variavel para armazenar o valor (tempo) da ultima atualização da mensagem
const long intervalo = 20000; //Intervalo de tempo entre checagem de mensagens (em milisegundos)

FC16 meuDisplay = FC16(pino_CS, qte_displays); // Criação e configuração do objeto display

String ircChannel = "";
 
WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);


void RemoveAccents (char Text[], int TextLength) {
  int i = 0;
  do {
    char c = Text[i];
    if ((int)c == 195) {
      char c2 = Text[i + 1];
      switch ((int)c2) {
        case 160 ... 166:
          Text[i] = 'a';
          break;
        case 167:
          Text[i] = 'c';
          break;
        case 168 ... 171:
          Text[i] = 'e';
          break;
        case 172 ... 175:
          Text[i] = 'i';
          break;
        case 177:
          Text[i] = 'n';
          break;
        case 178 ... 182:
          Text[i] = 'o';
          break;
        case 185 ... 188:
          Text[i] = 'u';
          break;
        case 189 ... 191:
          Text[i] = 'y';
          break;
        case 128 ... 134:
          Text[i] = 'A';
          break;
        case 135:
          Text[i] = 'C';
          break;
        case 136 ... 139:
          Text[i] = 'E';
          break;
        case 140 ... 143:
          Text[i] = 'I';
          break;
        case 146 ... 150:
          Text[i] = 'O';
          break;
        case 153 ... 156:
          Text[i] = 'U';
          break;
        default:
          break;
      }
      for (int j = i + 1; j < TextLength; j++) Text[j] = Text[j + 1];
      TextLength --;
    }
    i++;
  } while (i < TextLength);

}


void display_msg(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= intervalo)
  {
    previousMillis = currentMillis;; //Armazena o valor da ultima vez que foi feita a atualização da mensagem
    meuDisplay.setText(mensagem); // Atualiza o display com a nova mensagem
  }
  meuDisplay.update(); // Chama a rotina de scroll
  delay(scroll_delay); // Aguarda o tempo definido
}


 
// put your setup code here, to run once:
void setup() {

  delay(2000);
  Serial.begin(115200);
  Serial.println();
 
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
 
  ircChannel = "#" + twitchChannelName;
 
  client.setCallback(callback);

  meuDisplay.begin(); // Inicia o display
  meuDisplay.setIntensity(2); // Define o brilho do display
  meuDisplay.clearDisplay(); // Apaga qualquer informação que esteja no display
  meuDisplay.setText("JULIALABS"); // Texto inicial exibido no display


}

 
void loop() {
 
  // Try to connect to chat. If it loses connection try again
  if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel );
    // Attempt to connect
    // Second param is not needed by Twtich
    if (client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN)) {
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("connected and ready to rock");
      sendTwitchMessage("Ready to go Boss!");
    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }
  display_msg();
  client.loop();
}

 
void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}
 
 
void callback(IRCMessage ircMessage) {

  Serial.println(ircMessage.text[0]);

  if (ircMessage.text[0] == '*') {
    //Serial.println("Passed private message.");
   
    ircMessage.nick.toUpperCase();
    String msg(ircMessage.text);
    msg.toCharArray(mensagem, tamanhoArray);
    mensagem[0]=' ';
    RemoveAccents(mensagem, tamanhoArray);

  }
  if (ircMessage.text.indexOf("OI") > -1 && ircMessage.nick == "JULIALABS")
      {
     
      Serial.println("funcionaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
   
    }
 
    return;
  
} 