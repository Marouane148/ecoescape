# Architecture Technique - EcoEscape

## Vue d'Ensemble

EcoEscape implémente une architecture hybride combinant technologies web modernes et Internet des Objets (IoT) pour créer une expérience interactive temps réel.

## Schéma d'Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        ÉCOSYSTÈME ECOESCAPE                     │
├─────────────────┬─────────────────┬─────────────────────────────┤
│                 │                 │                             │
│   FRONTEND      │   BACKEND       │         IOT                 │
│   (Browser)     │   (Server)      │       (ESP32)               │
│                 │                 │                             │
│  ┌─────────────┐│  ┌─────────────┐│  ┌─────────────────────────┐│
│  │ index.html  ││  │  server.js  ││  │   ecoescape_esp32.ino   ││
│  │ style.css   ││  │             ││  │                         ││
│  │ app.js      ││  │ Express.js  ││  │ ┌─────────────────────┐ ││
│  │             ││  │ Socket.io   ││  │ │        WiFi         │ ││
│  │ Socket.io   ││  │             ││  │ │ ┌─────────────────┐ │ ││
│  │ Client      ││  │             ││  │ │ │      LDR        │ │ ││
│  └─────────────┘│  └─────────────┘│  │ │ │  (GPIO 34)      │ │ ││
│                 │                 │  │ │ └─────────────────┘ │ ││
│                 │                 │  │ │ ┌─────────────────┐ │ ││
│                 │                 │  │ │ │   LED Rouge     │ │ ││
│                 │                 │  │ │ │   (GPIO 5)      │ │ ││
│                 │                 │  │ │ └─────────────────┘ │ ││
│                 │                 │  │ │ ┌─────────────────┐ │ ││
│                 │                 │  │ │ │   LED Verte     │ │ ││
│                 │                 │  │ │ │   (GPIO 18)     │ │ ││
│                 │                 │  │ │ └─────────────────┘ │ ││
│                 │                 │  │ └─────────────────────┘ ││
│                 │                 │  └─────────────────────────┘│
└─────────────────┴─────────────────┴─────────────────────────────┘
```

## Flux de Communication

### 1. Communication Web (Frontend ↔ Backend)

```
Navigateur                     Serveur Node.js
    │                              │
    ├─────── HTTP GET / ─────────→ │ (page d'accueil)
    │←──── HTML + CSS + JS ────────┤
    │                              │
    ├──── WebSocket Connect ─────→ │ (Socket.io)
    │←─── Connection ACK ──────────┤
    │                              │
    │         ... Jeu ...          │
    │                              │
    │←─── event: "energie" ────────┤ (signal IoT)
    │        data: "ok"            │
```

### 2. Communication IoT (ESP32 → Backend)

```
ESP32                          Serveur Node.js
  │                                │
  ├─ analogRead(LDR) ─→ luminosité │
  │                                │
  ├─ if (luminosité >= seuil)      │
  │   └─ digitalWrite(LED_VERTE)   │
  │                                │
  ├── HTTP GET /update?status=ok ─→│
  │                                ├─ io.emit("energie", "ok")
  │←──── JSON Response ────────────┤
  │                                │
  │         Boucle...              │
```

## Composants Détaillés

### Frontend (Client Web)

**Technologies** : HTML5, CSS3, JavaScript ES6+, Socket.io Client

**Fichiers** :
- `index.html` : Structure de la page et éléments DOM
- `style.css` : Styles responsives avec thème écologique
- `app.js` : Logique client, drag & drop, WebSocket

**Responsabilités** :
- Interface utilisateur interactive
- Gestion du drag & drop (énigme 1)
- Réception des événements temps réel (énigme 2)
- Feedback visuel et animations

**Architecture Interne** :
```javascript
// Modules principaux
├── Gestion Drag & Drop
│   ├── initDragAndDrop()
│   ├── handleCorrectDrop()
│   └── handleIncorrectDrop()
├── WebSocket Client
│   ├── initWebSocket()
│   ├── socket.on("connect")
│   └── socket.on("energie")
└── Utilitaires
    ├── testServerConnection()
    └── showDebugInfo()
```

### Backend (Serveur Node.js)

**Technologies** : Node.js, Express.js, Socket.io, dotenv, cors

**Fichiers** :
- `server.js` : Serveur principal avec routes et WebSocket
- `package.json` : Configuration npm et dépendances
- `.env` : Variables d'environnement

**Responsabilités** :
- Servir les fichiers statiques (frontend)
- API REST pour l'IoT
- Gestion des WebSockets
- Coordination Frontend ↔ IoT

**Routes API** :
```javascript
GET /              → Serve index.html
GET /health        → Status check
GET /update        → IoT endpoint
```

**Architecture Interne** :
```javascript
// Structure serveur
├── Configuration
│   ├── Express middleware
│   ├── CORS policy
│   └── Static files
├── WebSocket Server
│   ├── Connection handling
│   ├── Event broadcasting
│   └── Error management
└── HTTP Routes
    ├── Health check
    ├── IoT webhook
    └── Frontend serving
```

### IoT (ESP32)

**Technologies** : Arduino C++, ESP32 WiFi, HTTPClient

**Fichiers** :
- `ecoescape_esp32.ino` : Code Arduino principal
- Configuration WiFi et hardware

**Responsabilités** :
- Lecture capteur LDR
- Contrôle LEDs physiques
- Communication WiFi
- Envoi requêtes HTTP

**Architecture Matérielle** :
```
ESP32 DevKit v1
├── WiFi Module (intégré)
├── GPIO 34 ─────→ LDR + 10kΩ résistance
├── GPIO 5  ─────→ LED Rouge + 220Ω
├── GPIO 18 ─────→ LED Verte + 220Ω
└── Alimentation USB/5V
```

**Cycle de Fonctionnement** :
```cpp
void loop() {
    // 1. Lecture luminosité
    int lumiere = analogRead(PIN_LDR);
    
    // 2. Décision selon seuil
    if (lumiere >= SEUIL) {
        // 3a. Activation panneau
        digitalWrite(LED_VERTE, HIGH);
        
        // 4a. Notification serveur
        httpGET("/update?status=ok");
    } else {
        // 3b. Désactivation panneau
        digitalWrite(LED_ROUGE, HIGH);
    }
    
    delay(1000);
}
```

## Protocoles de Communication

### HTTP REST (ESP32 → Serveur)

**Endpoint** : `GET /update?status=ok`

**Request** :
```http
GET /update?status=ok HTTP/1.1
Host: 192.168.1.20:3000
User-Agent: ESP32HTTPClient/1.0
```

**Response** :
```json
{
  "received": true,
  "status": "ok",
  "timestamp": "2025-10-06T10:30:00.000Z"
}
```

### WebSocket (Serveur ↔ Frontend)

**Événements** :

1. **Connexion Client**
```javascript
// Client
socket.on("connect", () => {
    console.log("Connecté:", socket.id);
});

// Serveur
io.on("connection", (socket) => {
    console.log("Client connecté:", socket.id);
});
```

2. **Signal Énergie**
```javascript
// Serveur (émission)
io.emit("energie", "ok");

// Client (réception)
socket.on("energie", (status) => {
    if (status === "ok") {
        activateSolarPanel();
    }
});
```

## Sécurité et Gestion d'Erreurs

### Sécurité

**Frontend** :
- Validation côté client (drag & drop)
- Gestion des déconnexions WebSocket
- Protection contre les injections

**Backend** :
- CORS configuré pour développement
- Validation des paramètres query
- Rate limiting implicite (IoT)

**IoT** :
- WiFi WPA2 (minimum)
- Timeout sur requêtes HTTP
- Gestion reconnexion automatique

### Gestion d'Erreurs

**Scénarios de Panne** :

1. **Déconnexion WiFi ESP32**
```cpp
if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi perdu, reconnexion...");
    WiFi.begin(SSID, PASSWORD);
}
```

2. **Serveur Indisponible**
```javascript
socket.on("connect_error", (error) => {
    console.error("Erreur serveur:", error);
    showErrorBanner("Serveur temporairement indisponible");
});
```

3. **Capteur Défaillant**
```cpp
int valeur = analogRead(PIN_LDR);
if (valeur < 0 || valeur > 4095) {
    Serial.println("Erreur lecture LDR");
    return; // Skip cette itération
}
```

## Performance et Scalabilité

### Optimisations Frontend
- CSS optimisé (animations hardware-accelerated)
- JavaScript vanilla (pas de framework lourd)
- Ressources servies localement

### Optimisations Backend
- Express.js léger
- Socket.io avec rooms (si multi-utilisateurs)
- Logs structurés

### Optimisations IoT
- Délais optimisés (1s lecture, 5s HTTP)
- Buffer WiFi intelligent
- Gestion mémoire ESP32

### Métriques Cibles
- **Latence WebSocket** : < 200ms
- **Temps réponse HTTP** : < 100ms
- **Consommation ESP32** : < 100mA
- **Stabilité** : 99% uptime sur 1h

## Extensions Futures

### Architecture Microservices
```
Frontend → Load Balancer → API Gateway
                            ├── Game Service
                            ├── IoT Service
                            └── User Service
```

### Base de Données
```
├── MongoDB (scores, parties)
├── InfluxDB (métriques IoT)
└── Redis (cache temps réel)
```

### Déploiement Cloud
```
├── Frontend → CDN (Cloudflare)
├── Backend → Container (Docker + K8s)
└── IoT → MQTT Broker (AWS IoT)
```

---

**Cette architecture assure une séparation claire des responsabilités tout en maintenant une communication fluide entre tous les composants du système EcoEscape.**