 #include <Adafruit_SSD1306.h> // lib de l'ecran OLED
 #include <SPI.h>
 #define Resolution_vertical 0.0048828125
 int sensorValue_analog;
// ================ Initialisation Ecran OLED =================== ///
#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de écran OLED sur le bus i2c 
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// ================ Initialisation Encodeur Rotatoire =================== ///
#define encodeurpin_clk 2
#define encodeurpin_data 4
#define encodeurpin_switch 5
volatile unsigned int compteur = 0 ;
int etatprecedentDT;
int etatprecedentCLK;
void doEncoder();

// ================ Initialisation Flex Sensor =================== ///
const float VCC = 5;      
const float R_DIV = 9800.0;  // Resustance utilise pour diviser la tension 
const float flatResistance = 28000.0; // resistance when flat
const float bendResistance = 80000.0;  // resistance at 90 deg
const int flexpin = A1;
int bit_mask_pot = 16;
float R2;
float Gain;

// ==================== Initalisation Potentionmetre digitale ===================//
#define MCP_NOP 0b00000000 //Aucune commande execute
#define MCP_WRITE 0b00010001 //écrire les données contenues dans l'octet de données dans le potentionmètre 
#define MCP_SHTDWN 0b00100001 
const int ssMCPin = 10;
bool check_overload();

void SPIWrite(uint8_t cmd, uint8_t data, uint8_t ssPin) // SPI write the command and data to the MCP IC connected to the ssPin
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //https://www.arduino.cc/en/Reference/SPISettings
  
  digitalWrite(ssPin, LOW); // SS pin low to select chip
  
  SPI.transfer(cmd);        // Send command code
  SPI.transfer(data);       // Send associated value
  
  digitalWrite(ssPin, HIGH);// SS pin high to de-select chip
  SPI.endTransaction();
}
// =========== Initilisation Module Bluetooth ========== //
  #include <stdlib.h>
  #include <string.h>
  #include <stdio.h>
  //#include <SoftwareSerial.h>
  
  byte serialRX; // variable de reception de donnée via RX
  byte serialTX; // variable de transmission de données via TX
  volatile byte RX = 0;
  int Gain_bit_mask;
  byte Option = 0;
  const int pushButton = 2;
  //#define rxPin 1  // Broche 10 en tant que RX, à raccorder sur TX du HC-05
  //#define txPin 0  // Broche 11 en tant que TX, à raccorder sur RX du HC-05
  //SoftwareSerial bluetooth(rxPin,txPin); // Définition du software serial pour le bluetooth


void setup() {
  // ========== Setup Port Série ========== //
  Serial.begin(9600); // initialize serial communication at 38400 bits per second:
  //Serial.println(F("Initialize System"));

  // ========== SETUP L'ECRAN OLED ==========//
  ecranOLED.clearDisplay(); 
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)){ while(1);  }    // Arrêt du programme (boucle infinie) en cas d'échec de l'initialisation
  ecranOLED.clearDisplay();   // Effaçage de la mémoire tampon de l'écran OLED
  delay(200);                            
  //Serial.println(F("OLED OK"));

  byte tailleDeCaractere = 1;
  ecranOLED.setTextSize(tailleDeCaractere);    

  //encodeur_rotatoire

  pinMode(encodeurpin_clk, INPUT);
  digitalWrite(encodeurpin_clk,HIGH);

  pinMode(encodeurpin_data, INPUT);
  digitalWrite(encodeurpin_data,HIGH);

  etatprecedentDT = digitalRead(encodeurpin_data);
  etatprecedentCLK = digitalRead(encodeurpin_clk);
  //Serial.println(F("ENCODEUR OK"));
  //flex sensor
   pinMode(flexpin, INPUT);

  // ============= SET UP POTENTIOMETRE DIGITAL ===========//
  pinMode (ssMCPin, OUTPUT); //select pin output
  digitalWrite(ssMCPin, HIGH); //SPI chip disabled
  SPI.begin(); 

  // ========== SET UP BLUETOOTH =========//
  pinMode(8, OUTPUT);

  
}


void loop() {
  //Lire la sens et la valeur de la compteur de l'encodeur rotatoire
  //Serial.println(compteur);
  doEncoder();
  // ==================== Potentiometre ===============
  SPIWrite(MCP_WRITE, bit_mask_pot, ssMCPin);
  // ***********************************************************************
  // Mesure la resistance de flex sensor et Lire la tension ADC à A0
  // ***********************************************************************
 
  int ADCflex = analogRead(flexpin);             // Lire la tension ADC de la pin A1
  float Vflex = ADCflex * Resolution_vertical;          // Calcul la tension ADC
  float Rflex = R_DIV * Vflex * (1 / (VCC - Vflex));     // Calcul la resistance de flex sensor
  sensorValue_analog = analogRead(A0);              // Lire la tension ADC de la pin A0 
  float sensorValue = sensorValue_analog * Resolution_vertical; // La tension ADC de la capteur graphite
  R2 = (50000 / 256) * bit_mask_pot + 125; 
  Gain = (1 + 100000 / R2) * 10 / 11;
  float R_sensor = (VCC / sensorValue) * 98000 * (1 + (98000/R2)) - 98000 - 9800;

  // ***********************************************************************
  // Envoyer les valeur de resistance à la module Bluetooth
  // ***********************************************************************
  int i=0;
  char received_message[32]={0};
  char gain[4] = {0};    
  do{
    received_message[i++]=Serial.read();
    delay(4);       
    }while (Serial.available() >0) ;
  
  // Ajuster le gain de l'AmpliOp 
  if (strstr(received_message,"Gain"))
  {  
    for (int k = 0 ;k < 4; k++){
    gain[k] = received_message[k + 4];
  }
    bit_mask_pot = atoi(gain);
  }

  // Mettre à jour le gain actuel sur l'interface
   if (strstr(received_message,"Update"))
  {  
    Serial.print("update ");
    Serial.println(bit_mask_pot);
  }

  if (strstr(received_message,"Flex")){
    if (check_overload(ADCflex)){
       Serial.print("over");
    }
    else{
    Serial.print(Rflex/1000);
    Serial.print(" ");
    Serial.println(ADCflex);
    Serial.print(' ');
    }
  }

  if (strstr(received_message, "Capteur"))
  {
    
    if( check_overload( sensorValue_analog )){
      Serial.print("over");
    }
    else{
    Serial.print(R_sensor/1000000);
    Serial.print(' ');
    Serial.println(sensorValue_analog);
    Serial.print(' ');
    }
  }

  // ***********************************************************************
  // Affichage OLED selon Encodeur Rotatoire
  // ***********************************************************************
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setCursor(0, 0); 
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  if (compteur % 2 == 0){
  ecranOLED.print("CAPTEUR: \n");
  ecranOLED.println("");
  ecranOLED.println("Resistance: ");
  ecranOLED.print(R_sensor/1000);
  ecranOLED.println(" kOhm");
  ecranOLED.println("");
  ecranOLED.print("Tension ADC: ");
  ecranOLED.println(sensorValue);
  ecranOLED.println("");
  ecranOLED.print("Gain ");
  ecranOLED.println (1 +(98000/(bit_mask_pot*195.31+125)));


  }
  else{
  ecranOLED.print("Flex: \n");
  ecranOLED.println("");
  ecranOLED.println("Resistance: ");
  ecranOLED.print(Rflex);}
  ecranOLED.display();        
  }                    


void doEncoder() {
  int etatactuelleDT = digitalRead(encodeurpin_data);
  int etatactuelleCLK = digitalRead(encodeurpin_clk);
  if (etatactuelleCLK != etatprecedentCLK){
    etatprecedentCLK  = etatactuelleCLK;
    if (etatactuelleCLK == LOW){
      if(etatactuelleCLK != etatactuelleDT){
        compteur ++;
      }
      else{
        compteur --;
        }
      }
    }
  }

bool check_overload(int bit_check){
  if  (bit_check > 980 && bit_check < 1024 ){
    Serial.print("over");
  }
}  
