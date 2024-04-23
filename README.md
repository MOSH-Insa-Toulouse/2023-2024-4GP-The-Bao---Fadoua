# 2023-2024-4GP-The-Bao---Fadoua

# Introduction : Description du projet et Livrables

# 1. Description du projet  
Notre projet, inscrit dans l'UF "Du capteur au banc de test en open source hardware" de la 4ème année de Génie Physique à l’INSA Toulouse, vise à concevoir, fabriquer et caractériser un capteur de contrainte innovant utilisant une approche low-tech basée sur des capteurs en graphite. S'inspirant des travaux de chercheurs dans le domaine des capteurs, notre objectif est de développer un système de mesure complet, intégrant un shield Arduino avec un circuit analogique incluant un amplificateur transimpédance et des filtres, ainsi qu'une interface utilisateur via un écran OLED et une communication sans fil avec une application Android via un module Bluetooth. Nous avons également créé un banc de test pour évaluer les performances du capteur, en mesurant sa variation de résistance en fonction de la déformation induite par différents crayons graphites. Ces travaux aboutiront à la rédaction d'une datasheet détaillée du capteur, offrant ainsi une solution open source pour la mesure de contraintes basée sur une approche low-tech.

# 2. Livrables 
Les livrables de ce projet sont les suivants :
  
* Circuit Shield PCB avec Arduino UNO
     
   - Schéma électrique du circuit amplificateur transimpédance
   - Conception du PCB réalisée sous KiCad
   - Fabrication du PCB et soudure des composants
   - Intégration des éléments : amplificateur, module Bluetooth, écran OLED, encodeur rotatif
  
* Code Arduino     
   - Développement complet d'un code Arduino pour le fonctionnement du système.
   -  Fonctionnalités incluses :
   - Mesure de la déformation du capteur à base de graphite.
   -Transmission des données via le module Bluetooth.
   - Affichage des mesures sur l'écran OLED.
   - Utilisation de l'encodeur rotatif pour naviguer dans le menu de l'OLED.
  
* Application Android (APK)
* Banc de Test Dédié au Capteur
*  Datasheet Complète du Capteur
 ## 3. Matériel nécessaire
* Carte Arduino UNO
* Amplificateur opérationnel (LTC1050)
* Module Bluetooth (HC-05)
* Écran OLED
* Encodeur rotatif (KY-040)
* Flex Sensor
* 2 Résistance de 100kΩ
* 1 Résistance de 1kΩ
* 1 Résistance de 10kΩ
* 1 Potentiomètre digital MCP-41050
* 2 Condensateurs 100nF
* 1 Condensateurs 1µF

 # 4. Electronique Analogique:
 
Le circuit amplificateur transimpédance est une composante essentielle de notre projet, car il permet de convertir le courant provenant du capteur de déformation à base de graphite en une tension mesurable. Composé de plusieurs éléments interagissant de manière précise, ce circuit garantit des mesures fiables et précises tout au long du processus.

Au cœur du circuit se trouve l'amplificateur opérationnel LTC1050, auquel le capteur de déformation est connecté à l'entrée inverseuse. Ce capteur, conçu pour ajuster sa résistance en fonction de la déformation, induit ainsi un changement de courant à travers la résistance de rétroaction R1.

La précision de nos mesures est assurée par le choix du LTC1050 pour ses faibles valeurs d'offset et de dérive. En complément, nous avons intégré trois étages de filtres au circuit pour optimiser le signal. Un filtre passe-bas à l'entrée (R1,C1) de 16 Hz atténue les hautes fréquences indésirables. Un second filtre passe-bas (R4, C2) de 1,6 Hz élimine le bruit de 50 Hz du réseau électrique. Enfin, un filtre final (R5,C3) de 1,6 kHz traite le bruit de l'ADC pour garantir des mesures précises et stables.

Chaque composant du circuit a été soigneusement sélectionné et dimensionné pour optimiser les performances. Des simulations approfondies sous LTSpice ont validé le fonctionnement du circuit avant la conception du PCB final. Des tests sur une breadboard ont confirmé sa fiabilité en conditions réelles, avec des mesures de résistance et de déformation du capteur réalisées pour évaluer sa précision.

La formule utilisée pour calculer la résistance du capteur de contrainte au graphite est la suivante :

      𝑅𝑠𝑒𝑛𝑠𝑜𝑟 = (1 +𝑅4/𝑅2) ∗ 𝑅1 ∗ (𝑉𝐶𝐶/𝑉𝐴𝐷𝐶) − 𝑅1 − 𝑅3
      
Voici le schéma du circuit simulé :
   ![image](https://github.com/MOSH-Insa-Toulouse/2023-2024-4GP-The-Bao---Fadoua/assets/101217468/8d1cc3c1-9032-4561-9eb3-224b6efbe436)

 # 5. Arduino:
   ## 5.1 Intégration Arduino
   ## 5.2 Progamme Arduino:
   ![image](Images/ARDUINO_DIAGRAMME.png)
 # 6. KiCad:
   ![image](Images/pcb_schema-1.png)
   ![image](Images/pcb.png)
 # 7. Application mobile:
 # 8. Mesures et résultats:

  


