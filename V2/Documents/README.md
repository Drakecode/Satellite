# Documentation de la carte Satellite V2

## Introduction

Le rôle des cartes Satellite est de fournir des fonctions utilisées en grand nombre sur les réseau miniatures:

- La détection des trains;
- La manœuvre des aiguilles et de la signalisation mécanique;
- Les feux de signalisation.

Autant de Satellite que nécessaires sont implantés pour piloter les fonction nécessaires sur le réseau. La communication entre le calculateur qui héberge le logiciel de pilotage est assurée par un bus CAN.

La carte satellite V2 est une évolution de la carte Satellite V1. Les objectif sont :

- de diminuer la spécialisation ;
- d'augmenter la souplesse pour que la carte puisse s'adapter à un plus grand nombre de situations ;
- d'augmenter la modularité matérielle;
- de répondre à manque de la V1, à savoir l'absence d'entrées pour les contacts de fin de course des aiguilles.

### Diminution de la spécialisation

Aucune électronique spécifique à un type de détection n'est implanté sur la carte. La carte seule est donc meilleure marché que la Satellite V1 qui intégrait une électronique de détection par consommation de courant.

### Augmentation de la souplesse

La carte Satellite V1 associait une broche de l'Arduino à une fonction de manière exclusive. ce n'est pas le cas de la Satellite V2 ou une broche est partagée entre plusieurs fonctions. Bien évidemment quand un Satellite V2 est configuré une des fonction et une seule est choisie.

### Augmentation de la modularité matérielle

La carte Satellite V2 met en œuvre un système de cartes d'extension permettant de rajouter de l'électronique comme la détection par consommation par exemple.

### Ajout d'entrées pour les fins de course des servos

Une entrée pour la détection de fin de course a été ajoutée pour chacun des servos pilotés par la carte Satellite V2 

## Fonctions

La carte Satellite V2 permet de connecter :

- Jusqu'à 6 servomoteurs avec et sans détection de fin de course;
- Jusqu'à 15 LED pour la signalisation;
- Jusqu'à 8 détecteurs;
- Jusqu'à 4 cartes d'extension.

Evidemment on ne peut pas avoir simultanément 6 servomoteurs, 15 LED et 8 détecteurs. 

La carte Satellite V2 intègre un contrôleur CAN SPI MC2517FD de Microchip. 

### Affectation des broches

L'Arduino utilisé est un Arduino Nano. Ce type d'Arduino a l'intérêt d'avoir un brochage fixe, contrairement au Pro Mini qui existe en plusieurs variantes, ce qui le rend moins adapté pour l'implanter sur une carte. Il se trouve sur eBay à prix très bas. Il a de plus l'avantage d'embarquer un converstisseur USB-série qui permet de télécharger le programme et d'utiliser la liaison série sans matériel additionnel.

![Arduino Nano](https://it.emcelettronica.com/wp-content/uploads/2015/02/ArduinoNanoFront.jpg)

De plus, le contrôleur CAN MCP2517FD comporte 2 broches numériques supplémentaires appelées IO0 et IO1.

Les fonctions sont numérotées à partir de 0. Nous avons donc :

- 6 servos de S0 à S5
- 6 fins de course de F0 à F5
- 15 LEDs de L0 à L14
- 8 détection de B0 à B7
- 4 cartes d'extension de E0 à E3

Les fonctions possibles sur les broches de l'Arduino sont données dans la table ci-dessous.

Broche | Communication    | Servo | fin de course | détection | carte d'extension | LED 
-------|------------------|-------|---------------|-----------|-------------------|-----
RX0    |     série        |       |               |           |                   | L1
TX1    |     série        |       |               |           |                   | L0
D2     | Interruption CAN |       |               |           |                   |
D3     |                  |   S0  |               |           |                   | L9
D4     | Chip select CAN  |       |               |           |                   |
D5     |                  |   S1  |               |     B2    |         E2        | L10
D6     |                  |   S2  |               |     B3    |         E3        | L6
D7     |                  |   S3  |               |           |                   | L5
D8     |                  |   S4  |               |           |                   | L3
D9     |                  |       |               |     B4    |         E0        | L14
D10    |                  |       |               |     B5    |         E1        | L13
D11    | SPI MOSI         |       |               |           |                   |
D12    | SPI MISO         |       |               |           |                   |
D13    | SPI SCK          |       |               |           |                   |
A0     |                  |   S5  |               |     B6    |         E2        | L12
A1     |                  |       |               |     B7    |         E3        | L11
A2     |                  |       |      F5       |           |                   | L2
A3     |                  |       |      F4       |           |                   | L4
A4     | I2C SDA          |       |      F3       |           |                   | L8
A5     | I2C SCL          |       |      F2       |           |                   | L7
A6     |                  |       |      F1       |           |                   |
A7     |                  |       |      F0       |           |                   |  

Les fonctions possibles sur les deux broches numériques du MCP2517FD sont données dans la table ci-dessous

Broche | détection | carte d'extension 
-------|-----------|------------------
IO0    |   B0      |   E0
IO1    |   B1      |   E1

