# 🌍 EcoEscape - Escape Game Éducatif Connecté

![Version](https://img.shields.io/badge/version-1.0.0-green)
![License](https://img.shields.io/badge/license-MIT-blue)
![Platform](https://img.shields.io/badge/platform-Web%20%2B%20IoT-orange)

**EcoEscape** est un escape game éducatif connecté qui sensibilise à l'environnement à travers deux énigmes interactives combinant technologies web et IoT.

## 🎯 Objectifs Pédagogiques

- **Sensibilisation écologique** : Apprendre les bonnes pratiques environnementales
- **Technologie IoT** : Découvrir l'Internet des Objets de manière ludique
- **Travail d'équipe** : Collaboration entre développement web et électronique
- **Résolution de problèmes** : Logique et réflexion pour résoudre les énigmes

## 🧩 Les Énigmes

### Énigme 1 : Tri Sélectif (Web)
- **Objectif** : Trier correctement 5 déchets dans 4 poubelles différentes
- **Technologie** : Drag & Drop HTML5 + JavaScript vanilla
- **Apprentissage** : Sensibilisation au tri des déchets

### Énigme 2 : Panneau Solaire (IoT)
- **Objectif** : Réparer le panneau solaire en l'exposant à la lumière
- **Technologie** : ESP32 + LDR + WebSocket temps réel
- **Apprentissage** : Énergie renouvelable et capteurs IoT

## 🏗️ Architecture Technique

```
┌─────────────────┐    WebSocket    ┌─────────────────┐
│   Navigateur    │◄────────────────┤   Serveur Web   │
│   (Frontend)    │                 │ (Node.js + IO)  │
└─────────────────┘                 └─────────────────┘
                                             ▲
                                             │ HTTP GET
                                             │
                                    ┌─────────────────┐
                                    │      ESP32      │
                                    │   (LDR + LEDs)  │
                                    └─────────────────┘
```

## 🚀 Installation et Lancement

### Prérequis
- **Node.js** 16+ et npm
- **Arduino IDE** pour l'ESP32
- **ESP32** + composants électroniques (voir liste matériel)

### 1. Installation du Serveur

```bash
cd server
npm install
cp .env.example .env
```

### 2. Configuration

Éditez le fichier `.env` si nécessaire :
```env
PORT=3000
HOST=0.0.0.0
```

### 3. Lancement du Serveur

```bash
npm run dev
```
Le serveur démarre sur `http://localhost:3000`

### 4. Configuration ESP32

1. Ouvrez `iot/ecoescape_esp32.ino` dans Arduino IDE
2. Modifiez les variables de configuration :
   ```cpp
   const char* WIFI_SSID = "VOTRE_SSID";
   const char* WIFI_PASSWORD = "VOTRE_MOT_DE_PASSE";
   const char* SERVER_URL = "http://IP_DE_VOTRE_PC:3000/update?status=ok";
   ```
3. Flashez le code sur l'ESP32

### 5. Câblage ESP32

| Composant | GPIO | Notes |
|-----------|------|-------|
| LDR | 34 | Avec résistance 10kΩ vers GND |
| LED Rouge | 5 | Avec résistance 220Ω |
| LED Verte | 18 | Avec résistance 220Ω |

## 🎮 Utilisation

1. **Ouvrez** `http://localhost:3000` dans votre navigateur
2. **Résolvez l'énigme 1** : Glissez-déposez les déchets dans les bonnes poubelles
3. **Passez à l'énigme 2** : La section panneau solaire s'affiche automatiquement
4. **Éclairez la LDR** : Allumez une lampe ou éclairez le capteur
5. **Victoire !** : Le panneau s'active et le jeu se termine

## 📋 Liste du Matériel IoT

### Composants Requis
- 1x ESP32 DevKit v1 ou similaire
- 1x LDR (Light Dependent Resistor)
- 1x Résistance 10kΩ
- 2x LEDs (1 rouge, 1 verte)
- 2x Résistances 220Ω
- 1x Breadboard
- Fils de connexion (jumper wires)

### Outils
- Arduino IDE
- Câble USB pour ESP32
- Multimètre (optionnel, pour tests)

## 🔧 Dépannage

### Problèmes Courants

**Le serveur ne démarre pas**
- Vérifiez que Node.js est installé : `node --version`
- Vérifiez que le port 3000 est libre

**L'ESP32 ne se connecte pas au WiFi**
- Vérifiez le SSID et mot de passe WiFi
- Assurez-vous que l'ESP32 est à portée du réseau

**Le panneau solaire ne s'active pas**
- Vérifiez le câblage de la LDR
- Ajustez la variable `SEUIL_LUMINOSITE` dans le code
- Consultez le moniteur série Arduino pour les logs

**Pas de communication ESP32 ↔ Serveur**
- Vérifiez l'IP du serveur dans `SERVER_URL`
- Testez la route `/health` : `http://localhost:3000/health`

### Debug

**Mode Debug Frontend** : Ajoutez `#debug` à l'URL
**Logs ESP32** : Moniteur série Arduino (115200 baud)
**Logs Serveur** : Console Node.js

## 👥 Organisation d'Équipe (5 Personnes)

### Rôles Suggérés

1. **Chef de Projet / Scénariste**
   - Coordination générale
   - Scénario et aspect pédagogique
   - Tests d'intégration

2. **Développeur Frontend**
   - Interface utilisateur (HTML/CSS/JS)
   - UX/UI et responsive design
   - Intégration WebSocket client

3. **Développeur Backend / Réseau**
   - Serveur Node.js + Express
   - WebSocket Server (Socket.io)
   - API et gestion des connexions

4. **Développeur IoT**
   - Code ESP32 Arduino
   - Câblage et tests électroniques
   - Communication HTTP/WiFi

5. **Designer / Présentateur**
   - Charte graphique et assets
   - Documentation utilisateur
   - Préparation démonstration

## 📊 API Documentation

Voir `docs/api.md` pour la documentation complète de l'API.

### Endpoints Principaux

- `GET /health` - Vérification de l'état du serveur
- `GET /update?status=ok` - Signal IoT (déclenche WebSocket)

### Événements WebSocket

- `connect` - Connexion client
- `energie` - Signal panneau solaire (payload: "ok")

## 📁 Structure du Projet

```
ecoescape/
├── README.md                 # Ce fichier
├── server/                   # Serveur Node.js
│   ├── package.json         # Dépendances npm
│   ├── .env.example         # Configuration exemple
│   ├── server.js            # Serveur principal
│   └── public/              # Frontend statique
│       ├── index.html       # Page principale
│       ├── style.css        # Styles CSS
│       └── app.js           # JavaScript client
├── iot/                     # Code IoT
│   ├── ecoescape_esp32.ino  # Code Arduino ESP32
│   └── fritzing_schema.png  # Schéma de câblage
└── docs/                    # Documentation
    ├── cahier_des_charges.md
    ├── architecture.md
    └── api.md
```

## 🔗 Liens Utiles

- [Node.js](https://nodejs.org/) - Runtime JavaScript
- [Socket.io](https://socket.io/) - WebSocket temps réel
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32) - Support ESP32
- [Fritzing](https://fritzing.org/) - Schémas électroniques

## 📄 Licence

Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.

## 🤝 Contribution

Les contributions sont les bienvenues ! N'hésitez pas à :
- Signaler des bugs
- Proposer de nouvelles fonctionnalités
- Améliorer la documentation
- Ajouter de nouvelles énigmes

## 📞 Support

Pour toute question ou problème :
1. Consultez la section Dépannage ci-dessus
2. Vérifiez les logs serveur et ESP32
3. Testez les connexions réseau

---

**Fait avec 💚 pour l'éducation environnementale et l'apprentissage des technologies IoT**