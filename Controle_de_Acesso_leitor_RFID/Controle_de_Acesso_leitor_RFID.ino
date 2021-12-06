//Programa : RFID - Controle de Acesso leitor RFID
//Autor : FILIPEFLOP
 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_AZ 3
#define LED_VM 2
#define BUZZ 4
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
LiquidCrystal lcd(6, 7, 5, 4, 3, 2); 
 
char st[20];
 
void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  //Define o número de colunas e linhas do LCD:
  lcd.begin(16, 2);
  mensageminicial();

  pinMode(BUZZ,OUTPUT);
  noTone(BUZZ);

  pinMode(LED_AZ, OUTPUT);
  pinMode(LED_VM, OUTPUT);
  digitalWrite(LED_AZ, LOW);
  digitalWrite(LED_VM, LOW);
}
 
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "39 E1 F9 C2") //UID 2 - Cartao
  {
    digitalWrite(LED_AZ, HIGH);
    Serial.println("Ola Cartao !");
    Serial.println();
    bip(2500);
  }else{
    digitalWrite(LED_VM, HIGH);
    Serial.println("Acesso Negado !");
    Serial.println();
    bip(1000);
  }

  delay(1000);
  digitalWrite(LED_AZ, LOW);
  digitalWrite(LED_VM, LOW);
} 

void bip(int freq)
{
  tone(BUZZ,freq);
  delay(200); 
  noTone(BUZZ);
  delay(200);
}
