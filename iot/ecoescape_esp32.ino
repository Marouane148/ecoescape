/*
  EcoEscape ESP32 - Panneau Solaire IoT
  
  Ce code fait partie du projet EcoEscape, un escape game éducatif connecté.
  L'ESP32 mesure la luminosité avec une LDR et communique avec le serveur web.
  
  Matériel requis:
  - ESP32 DevKit v1 ou similaire
  - LDR (Light Dependent Resistor)
  - Résistance 10kΩ
  - 2 LEDs (rouge et verte)
  - 2 résistances 220Ω pour les LEDs
  - Breadboard et fils de connexion
  
  Connexions:
  - LDR: GPIO 34 (avec pont diviseur 10kΩ vers GND)
  - LED Rouge: GPIO 5 (avec résistance 220Ω)
  - LED Verte: GPIO 18 (avec résistance 220Ω)
*/

#include <WiFi.h>
#include <HTTPClient.h>

// ===========================================
// CONFIGURATION RÉSEAU - MODIFIEZ CES 3 LIGNES !
// ===========================================
const char* WIFI_SSID = "VOTRE_SSID";          // ⚠️ REMPLACEZ par le nom de votre WiFi
const char* WIFI_PASSWORD = "VOTRE_MOT_DE_PASSE"; // ⚠️ REMPLACEZ par votre mot de passe WiFi
const char* SERVER_URL = "http://192.168.1.20:3000/update?status=ok"; // ⚠️ REMPLACEZ par l'IP de votre PC

// ===========================================
// CONFIGURATION MATÉRIEL
// ===========================================
const int PIN_LDR = 34;        // Entrée analogique pour la LDR
const int PIN_LED_ROUGE = 5;   // LED rouge (pas assez de lumière)
const int PIN_LED_VERTE = 18;  // LED verte (assez de lumière)

// ===========================================
// PARAMÈTRES DE FONCTIONNEMENT
// ===========================================
int SEUIL_LUMINOSITE = 1800;   // Seuil de luminosité (0-4095) à ajuster selon votre LDR
const int DELAI_LECTURE = 1000; // Délai entre les lectures en millisecondes
const int DELAI_HTTP = 5000;    // Délai minimum entre les requêtes HTTP
const int DELAI_WIFI_RETRY = 10000; // Délai entre les tentatives de reconnexion WiFi

// Variables globales
unsigned long derniere_requete_http = 0;
unsigned long derniere_tentative_wifi = 0;
bool panneau_actif = false;
bool wifi_connecte = false;

// ===========================================
// CONFIGURATION INITIALE
// ===========================================
void setup() {
  // Initialisation de la communication série
  Serial.begin(115200);
  Serial.println("\n🌍 EcoEscape ESP32 - Démarrage");
  Serial.println("================================");
  
  // Configuration des pins
  pinMode(PIN_LED_ROUGE, OUTPUT);
  pinMode(PIN_LED_VERTE, OUTPUT);
  pinMode(PIN_LDR, INPUT);
  
  // État initial: LED rouge allumée
  digitalWrite(PIN_LED_ROUGE, HIGH);
  digitalWrite(PIN_LED_VERTE, LOW);
  Serial.println("🔴 LED rouge activée (pas assez de lumière)");
  
  // Connexion WiFi
  connecterWiFi();
  
  // Affichage des informations de configuration
  afficherConfiguration();
}

// ===========================================
// BOUCLE PRINCIPALE
// ===========================================
void loop() {
  // Vérification de la connexion WiFi
  verifierConnexionWiFi();
  
  // Lecture de la luminosité
  int valeur_luminosite = analogRead(PIN_LDR);
  
  // Affichage des valeurs (debug)
  Serial.print("💡 Luminosité: ");
  Serial.print(valeur_luminosite);
  Serial.print("/4095 (seuil: ");
  Serial.print(SEUIL_LUMINOSITE);
  Serial.println(")");
  
  // Traitement selon le niveau de luminosité
  if (valeur_luminosite >= SEUIL_LUMINOSITE) {
    gererLuminositeSuffisante();
  } else {
    gererLuminositeInsuffisante();
  }
  
  delay(DELAI_LECTURE);
}

// ===========================================
// FONCTIONS WIFI
// ===========================================
void connecterWiFi() {
  Serial.println("📶 Connexion au WiFi...");
  Serial.print("Réseau: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int tentatives = 0;
  while (WiFi.status() != WL_CONNECTED && tentatives < 20) {
    delay(500);
    Serial.print(".");
    tentatives++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifi_connecte = true;
    Serial.println("");
    Serial.println("✅ WiFi connecté !");
    Serial.print("📍 Adresse IP: ");
    Serial.println(WiFi.localIP());
  } else {
    wifi_connecte = false;
    Serial.println("");
    Serial.println("❌ Échec de connexion WiFi");
  }
}

void verifierConnexionWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    if (!wifi_connecte || millis() - derniere_tentative_wifi > DELAI_WIFI_RETRY) {
      wifi_connecte = false;
      derniere_tentative_wifi = millis();
      Serial.println("⚠️ WiFi déconnecté, tentative de reconnexion...");
      connecterWiFi();
    }
  } else if (!wifi_connecte) {
    wifi_connecte = true;
    Serial.println("✅ WiFi reconnecté !");
  }
}

// ===========================================
// FONCTIONS DE GESTION DE LA LUMINOSITÉ
// ===========================================
void gererLuminositeSuffisante() {
  // Activation du panneau solaire
  if (!panneau_actif) {
    panneau_actif = true;
    digitalWrite(PIN_LED_VERTE, HIGH);
    digitalWrite(PIN_LED_ROUGE, LOW);
    Serial.println("🟢 Panneau solaire activé ! Assez de lumière détectée");
  }
  
  // Envoi de la requête HTTP (avec limitation de fréquence)
  if (wifi_connecte && millis() - derniere_requete_http > DELAI_HTTP) {
    envoyerRequeteHTTP();
    derniere_requete_http = millis();
  }
}

void gererLuminositeInsuffisante() {
  // Désactivation du panneau solaire
  if (panneau_actif) {
    panneau_actif = false;
    digitalWrite(PIN_LED_ROUGE, HIGH);
    digitalWrite(PIN_LED_VERTE, LOW);
    Serial.println("🔴 Panneau solaire désactivé - Pas assez de lumière");
  }
}

// ===========================================
// FONCTION HTTP
// ===========================================
void envoyerRequeteHTTP() {
  if (!wifi_connecte) {
    Serial.println("❌ Pas de connexion WiFi pour envoyer la requête");
    return;
  }
  
  HTTPClient http;
  http.begin(SERVER_URL);
  http.setTimeout(5000); // Timeout de 5 secondes
  
  Serial.println("📡 Envoi de la requête au serveur...");
  Serial.print("URL: ");
  Serial.println(SERVER_URL);
  
  int code_reponse = http.GET();
  
  if (code_reponse > 0) {
    Serial.printf("✅ Réponse HTTP: %d\n", code_reponse);
    
    if (code_reponse == 200) {
      String payload = http.getString();
      Serial.println("📦 Réponse du serveur: " + payload);
    }
  } else {
    Serial.printf("❌ Erreur HTTP: %s\n", http.errorToString(code_reponse).c_str());
  }
  
  http.end();
}

// ===========================================
// FONCTIONS UTILITAIRES
// ===========================================
void afficherConfiguration() {
  Serial.println("\n🔧 Configuration:");
  Serial.println("==================");
  Serial.printf("📍 LDR sur GPIO %d\n", PIN_LDR);
  Serial.printf("🔴 LED Rouge sur GPIO %d\n", PIN_LED_ROUGE);
  Serial.printf("🟢 LED Verte sur GPIO %d\n", PIN_LED_VERTE);
  Serial.printf("🌟 Seuil luminosité: %d/4095\n", SEUIL_LUMINOSITE);
  Serial.printf("⏱️ Délai lecture: %dms\n", DELAI_LECTURE);
  Serial.printf("🌐 URL serveur: %s\n", SERVER_URL);
  Serial.println("==================\n");
  
  Serial.println("💡 Instructions:");
  Serial.println("- Allumez une lampe ou éclairez la LDR");
  Serial.println("- La LED verte s'allumera si assez de lumière");
  Serial.println("- Le serveur sera notifié automatiquement");
  Serial.println("- Surveillez les messages sur le navigateur web\n");
}

// Fonction pour calibrer le seuil (optionnelle)
void calibrerSeuil() {
  Serial.println("🔧 Mode calibrage (30 secondes)");
  Serial.println("Variez l'éclairage de la LDR...");
  
  int valeur_min = 4095;
  int valeur_max = 0;
  
  unsigned long debut = millis();
  while (millis() - debut < 30000) {
    int valeur = analogRead(PIN_LDR);
    valeur_min = min(valeur_min, valeur);
    valeur_max = max(valeur_max, valeur);
    
    Serial.printf("Min: %d, Max: %d, Actuel: %d\n", valeur_min, valeur_max, valeur);
    delay(500);
  }
  
  SEUIL_LUMINOSITE = valeur_min + (valeur_max - valeur_min) * 0.7; // 70% de la plage
  Serial.printf("🎯 Nouveau seuil calculé: %d\n", SEUIL_LUMINOSITE);
}

/*
  SCHÉMA DE CÂBLAGE:
  ==================
  
  ESP32        Composant
  -----        ---------
  GPIO 34  --> LDR (+ résistance 10kΩ vers GND)
  GPIO 5   --> LED Rouge (+ résistance 220Ω)
  GPIO 18  --> LED Verte (+ résistance 220Ω)
  3.3V     --> LDR (autre extrémité)
  GND      --> Masse commune
  
  NOTES:
  - Ajustez SEUIL_LUMINOSITE selon votre environnement
  - Modifiez WIFI_SSID et WIFI_PASSWORD
  - Adaptez SERVER_URL avec l'IP de votre serveur
  - Utilisez le moniteur série pour le debug
*/