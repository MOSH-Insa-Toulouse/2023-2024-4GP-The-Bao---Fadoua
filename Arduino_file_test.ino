 #include <Adafruit_SSD1306.h> // lib de l'ecran OLED

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//Encodeur rotatif
#define encodeurpin_clk 2
#define encodeurpin_data 4
#define encodeurpin_switch 5
volatile unsigned int compteur = 0 ;
int etatprecedentDT;
int etatprecedentCLK;
void doEncoder();


//Flex sensor
const float VCC = 5;      // voltage at Ardunio 5V line
const float R_DIV = 10000.0;  // resistor used to create a voltage divider
const float flatResistance = 28000.0; // resistance when flat
const float bendResistance = 80000.0;  // resistance at 90 deg
const int flexpin = A1;





void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(38400);

   // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation

  byte tailleDeCaractere = 2;
  ecranOLED.setTextSize(tailleDeCaractere);    

  //encodeur_rotatoire

  pinMode(encodeurpin_clk, INPUT);
  digitalWrite(encodeurpin_clk,HIGH);

  pinMode(encodeurpin_data, INPUT);
  digitalWrite(encodeurpin_data,HIGH);

  etatprecedentDT = digitalRead(encodeurpin_data);
  etatprecedentCLK = digitalRead(encodeurpin_clk);

  //flex sensor
   pinMode(flexpin, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  //Lire la sens et la valeur de l'encodeur rotatoire
  doEncoder();
  Serial.println(compteur);
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  
  
  //mesure la resistance de flex sensor
  // Read the ADC, and calculate voltage and resistance from it

  int ADCflex = analogRead(flexpin);
  float Vflex = ADCflex * VCC / 1023.0;
  float Rflex = R_DIV * (VCC / Vflex - 1.0);
  
  // print out the value you read:

  //Serial.println(sensorValue);
  //Serial.println("Resistance: " + String(Rflex) + " ohms");

  // ***********************************************************************
  // Affichage de la resistancce
  // ***********************************************************************
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setCursor(0, 0); 
  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  if (compteur % 2 == 0){
  ecranOLED.print("Sensor: \n");
  ecranOLED.println(sensorValue);}
  else{
  ecranOLED.print("Flex: \n");
  ecranOLED.print(Rflex);}
  ecranOLED.display();                            // Transfert le buffer à l'écran
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
  
