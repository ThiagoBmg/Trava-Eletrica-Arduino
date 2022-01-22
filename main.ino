#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);
SoftwareSerial mySerial(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define trava 7 /* Verificar esses pinos */
#define buzzer 6 /* Verificar esses pinos */
/*#define btn 5  Verificar esses pinos */

/*--------------------------------------------------------------------------------------------------- VARIAVEIS------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*int estado_btn = 0;*/
/*-----------------*/

/**/
int id;
uint8_t ida;
int t = 1;
/**/
uint8_t modoGravacaoID(uint8_t ida);

/*-----------------------------------------------------------------------------------------------------void setup------------------------------------------------------------------------------------------------------------------------------------------------------*/
void setup() {

  pinMode(trava, OUTPUT);
  digitalWrite(trava, LOW);
  
  /*pinMode(btn, INPUT);*/
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight(); 

  Serial.begin(9600);
  finger.begin(57600);

  if(finger.verifyPassword()){/*---------------------------------------------------------------/////-verifica se o sistema esta ok.-\\\\\\-------------------------------------*/
    Serial.println("Sensor biometrico encontrado!");
    lcd.setCursor(3,0);
    lcd.print("Sistema Ok!!");
    tone(buzzer, 2500, 250);  
    delay(250);
    tone(buzzer, 3000, 250);  
    delay(250);
    tone(buzzer, 4000, 250);
    delay(250);
    tone(buzzer, 3500, 250);  
    delay(2000);
    LcdMessage();
    /*---------------------------------------------------------------/////-sistema esta ok.-\\\\\\-------------------------------------*/
    /*noTone(buzzer);*/ 
    /*---------------------------------------------------------------/////-sistema não esta ok.-\\\\\\-------------------------------------*/
  } else {
    Serial.println("Sensor não biometrico encontrado!");
    lcd.clear(); 
    lcd.setCursor(3,1);
    lcd.print("Sistema Off!!");
    tone(buzzer, 1000, 1000);  
    while(true) { 
      delay(1); 
    }
  }/*---------------------------------------------------------------/////-verifica se o sistema esta ok.-\\\\\\-------------------------------------*/

}
/*-----------------------------------------------------------------------------------------------------void loop------------------------------------------------------------------------------------------------------------------------------------------------------*/
void loop() {
  // put your main code here, to run repeatedly:
    getFingerprintIDez();
}

void LcdMessage(){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Coloque ");
    lcd.setCursor(4,1);
    lcd.print("sua digital");
    lcd.setCursor(5,2);
    lcd.print(" para entrar");
    lcd.setCursor(5,3);
    lcd.print("THPS.HOME");
}

/*---------------------------------------------------------------------------------------------------MODO SCANER LOOP------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------MODO SCANER LOOP------------------------------------------------------------------------------------------------------------------------------------------------------*/
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  //Encontrou uma digital!
  if (finger.fingerID == 1) {
     Serial.println("Sensor biometrico encontrado!");
     lcd.clear();
     lcd.setCursor(1,0);
     lcd.print("Modo Administrador");
     lcd.setCursor(5,2);
     lcd.print("Aguarde...");
     delay (1000);
     finger.getTemplateCount(); 
     id = (finger.templateCount);
     ida = id + t;
     modoGravacaoID(ida);
     return 0; 
  } else if (finger.fingerID < 10 ||finger.fingerID == 10) {/*---------------------------------------------------------------/EU\-----------------------*/
    digitalWrite(trava, HIGH);
    Serial.println("Sensor biometrico encontrado!");
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("BEM VINDO!");
    lcd.setCursor(2,2);
    lcd.print("SR. THIAGO GOMES");
    lcd.setCursor(5,3);
    lcd.print("THPS.HOME");
    delay(10000);
    LcdMessage();/*---------------------------------------------------------------/////-ajuste tempo que a porta ira ficar liberada-\\\\\\-----------------------*/
    digitalWrite(trava, LOW);
    return finger.fingerID; 
  }
  else {/*---------------------------------------------------------------/////-ACESSO DEMAIS PESSOAS-\\\\\\--------------------------------------------------------*/
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("lIBERANDO ACESSO EM ...");
    lcd.setCursor(5,2);
    lcd.print("5");
    tone(buzzer, 1000, 1000);
    delay(1000);
    lcd.setCursor(5,2);
    lcd.print("4");
    tone(buzzer, 1000, 1000);
    delay(1000);
    lcd.setCursor(5,2);
    lcd.print("3");
    tone(buzzer, 1000, 1000);
    delay(1000);
    lcd.setCursor(5,2);
    lcd.print("2");
    tone(buzzer, 1000, 1000);
    delay(1000);
    lcd.setCursor(5,2);
    lcd.print("1");
    tone(buzzer, 1000, 1000);
    delay(1000);
    lcd.setCursor(5,2);
    lcd.print("0");
    tone(buzzer, 1000, 1000);
    delay(1000);
     lcd.clear();
     lcd.setCursor(3,0);
     lcd.print("Acesso Liberado.");
     lcd.setCursor(1,1);
     lcd.print("Seja bem vindo.");
     digitalWrite(trava, HIGH);
     delay(10000);/*---------------------------------------------------------------/////-ajuste tempo que a porta ira ficar liberada-\\\\\\-----------------------*/
     digitalWrite(trava, LOW);
     return finger.fingerID;
     LcdMessage();
  } 
}

/*---------------------------------------------------------------------------------------------------MODO GRAVAÇÃO------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------MODO GRAVAÇÃO------------------------------------------------------------------------------------------------------------------------------------------------------*/
 uint8_t modoGravacaoID(uint8_t ida) {
  int p = -1;
  lcd.clear();
  lcd.print("Esperando...");lcd.print("ID");lcd.setCursor(2,2);lcd.print(ida);
  delay(2000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Leitura concluída");
      break;
      
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("Coloque o  dedo");
     finger.getImage();
     delay(1000);
     finger.getImage();
      delay(1000);
     finger.getImage();
      delay(1000);
     finger.getImage();
      delay(1000);
     finger.getImage();
      delay(1000);
      LcdMessage();
      return;
      break;

    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Erro comunicação");

      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Erro de leitura");

      break;
    default:
      lcd.clear();
      lcd.print("Erro desconhecido");

      break;
    }
  }

  // OK successo!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Erro desconhecido");

      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Leitura suja");

      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Erro de comunicação");

      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("Não foi possível encontrar propriedade da digital");

      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("Não foi possível encontrar propriedade da digital");

      return p;
    default:
      lcd.clear();
      lcd.print("Erro desconhecido");

      return p;
  }
  
  lcd.clear();
  lcd.print("Remova o dedo");

  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  lcd.clear();
  lcd.print("ID");lcd.setCursor(2,2);lcd.print(ida);
  
  p = -1;
  lcd.clear();
  lcd.print("Coloque o Mesmo dedo novamente");
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Leitura concluída");
     
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("Coloque o  dedo");
      finger.getImage();
     delay(1000);
     finger.getImage();
      delay(1000);
     finger.getImage();
      delay(1000);
     finger.getImage();
      delay(1000);
     finger.getImage();
      delay(1000);
      LcdMessage();
      return;
      break;

    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Erro de comunicação");
      
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Erro de Leitura");
      
      break;
    default:
      lcd.clear();
      lcd.print("Erro desconhecido");
      
      break;
    }
  }

  // OK successo!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Leitura convertida");
      
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Leitura suja");
      
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Erro de comunicação");
      
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("Não foi possível encontrar as propriedades da digital");
      
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("Não foi possível encontrar as propriedades da digital");
      
      return p;
    default:
      lcd.clear();
      lcd.print("Erro desconhecido");
      
      return p;
  }
  
  // OK convertido!
  lcd.clear();
  lcd.print("...");
  
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("As digitais batem!");
    delay(500);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("Erro de comunicação");
  
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.print("As digitais não batem");
   
    return p;
  } else {
    lcd.clear();
    lcd.print("Erro desconhecido");
    
    return p;
  }  

  
  p = finger.storeModel(ida);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0,0);lcd.print("ID");lcd.setCursor(4,0);lcd.print(ida);lcd.setCursor(0,8);lcd.print("Armazenado!");
    
    
    delay(1500);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("Erro de comunicação");

    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.print("Não foi possível gravar neste local da memória");

    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.print("Erro durante escrita na memória flash");

    return p;
  } else {
    lcd.clear();
    lcd.print("Erro desconhecido");

    return p;
  }   
  LcdMessage();
}
