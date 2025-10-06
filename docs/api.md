# API Documentation - EcoEscape

## Vue d'ensemble

L'API EcoEscape fournit une interface de communication entre les composants web et IoT du système. Elle utilise une combinaison de REST HTTP pour les requêtes IoT et WebSocket pour la communication temps réel.

## Base URL

```
http://localhost:3000
```

## Authentification

Aucune authentification requise pour cette version éducative.

## Routes HTTP REST

### 1. Health Check

Vérification de l'état du serveur

**Endpoint :** `GET /health`

**Description :** Retourne l'état de santé du serveur

**Paramètres :** Aucun

**Réponse :**
```json
{
  "status": "ok",
  "timestamp": "2025-10-06T10:30:00.000Z"
}
```

**Codes de statut :**
- `200` : Serveur opérationnel
- `500` : Erreur serveur

**Exemple curl :**
```bash
curl -X GET http://localhost:3000/health
```

---

### 2. IoT Update Endpoint

Signal de mise à jour depuis l'ESP32

**Endpoint :** `GET /update`

**Description :** Reçoit les signaux IoT et les diffuse via WebSocket

**Paramètres Query :**
- `status` (string, requis) : État du panneau solaire
  - Valeurs acceptées : `"ok"`, `"error"`, `"reset"`

**Réponse :**
```json
{
  "received": true,
  "status": "ok",
  "timestamp": "2025-10-06T10:30:00.000Z"
}
```

**Comportement :**
- Si `status=ok` : Diffuse l'événement `"energie"` via WebSocket
- Logs l'événement côté serveur
- Retourne la confirmation de réception

**Codes de statut :**
- `200` : Signal reçu et traité
- `400` : Paramètre manquant ou invalide

**Exemples :**

Succès (ESP32) :
```bash
curl -X GET "http://localhost:3000/update?status=ok"
```

Test manuel :
```bash
curl -X GET "http://localhost:3000/update?status=error"
```

---

### 3. Frontend Serving

Service des fichiers statiques

**Endpoint :** `GET /`

**Description :** Sert la page principale de l'application

**Paramètres :** Aucun

**Réponse :** Page HTML complète avec CSS et JavaScript

**Fichiers servis :**
- `/` → `public/index.html`
- `/style.css` → `public/style.css`
- `/app.js` → `public/app.js`
- `/socket.io/socket.io.js` → Bibliothèque Socket.io

---

## WebSocket API

### Configuration

**Namespace :** `/` (racine)
**Protocole :** Socket.io v4.x
**CORS :** Autorisé pour tous les domaines (développement)

### Événements Client → Serveur

#### Connection

**Événement :** `connect`

**Description :** Connexion initiale du client

**Payload :** Aucun

**Réponse serveur :** Log de connexion avec ID socket

**Exemple JavaScript :**
```javascript
const socket = io();

socket.on("connect", () => {
    console.log("Connecté avec ID:", socket.id);
});
```

#### Disconnection

**Événement :** `disconnect`

**Description :** Déconnexion du client

**Payload :** Raison de déconnexion

**Exemple :**
```javascript
socket.on("disconnect", (reason) => {
    console.log("Déconnecté:", reason);
});
```

### Événements Serveur → Client

#### Signal Énergie

**Événement :** `energie`

**Description :** Signal d'activation du panneau solaire

**Payload :** 
```javascript
"ok" // String
```

**Déclencheur :** Requête IoT sur `/update?status=ok`

**Exemple réception :**
```javascript
socket.on("energie", (status) => {
    if (status === "ok") {
        console.log("Panneau solaire activé !");
        // Mettre à jour l'interface
    }
});
```

#### Gestion d'erreurs

**Événement :** `connect_error`

**Description :** Erreur de connexion

**Payload :** Objet d'erreur

**Exemple :**
```javascript
socket.on("connect_error", (error) => {
    console.error("Erreur de connexion:", error.message);
});
```

## Codes d'Erreur

### HTTP Status Codes

| Code | Signification | Description |
|------|---------------|-------------|
| 200 | OK | Requête traitée avec succès |
| 400 | Bad Request | Paramètres manquants ou invalides |
| 404 | Not Found | Route non trouvée |
| 500 | Internal Server Error | Erreur interne du serveur |

### WebSocket Error Events

| Événement | Cause | Action recommandée |
|-----------|-------|-------------------|
| `connect_error` | Serveur indisponible | Retry automatique |
| `disconnect` | Perte de connexion | Tentative de reconnexion |
| `error` | Erreur générale | Log et notification utilisateur |

## Exemples d'Intégration

### Frontend JavaScript

```javascript
// Initialisation
const socket = io();
let connected = false;

// Gestion de la connexion
socket.on("connect", () => {
    connected = true;
    updateConnectionStatus("🟢 Connecté");
});

socket.on("disconnect", () => {
    connected = false;
    updateConnectionStatus("🔴 Déconnecté");
});

// Réception signal IoT
socket.on("energie", (status) => {
    if (status === "ok") {
        activateSolarPanel();
    }
});

// Test de santé serveur
async function checkServerHealth() {
    try {
        const response = await fetch('/health');
        const data = await response.json();
        return data.status === 'ok';
    } catch (error) {
        console.error('Serveur inaccessible:', error);
        return false;
    }
}
```

### Arduino ESP32

```cpp
#include <WiFi.h>
#include <HTTPClient.h>

const char* SERVER_URL = "http://192.168.1.20:3000/update?status=ok";

void sendSolarSignal() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(SERVER_URL);
        
        int httpCode = http.GET();
        
        if (httpCode == 200) {
            String payload = http.getString();
            Serial.println("Réponse serveur: " + payload);
        } else {
            Serial.printf("Erreur HTTP: %d\n", httpCode);
        }
        
        http.end();
    }
}
```

### Test avec curl

```bash
# Test de santé
curl -i http://localhost:3000/health

# Simulation signal IoT
curl -i "http://localhost:3000/update?status=ok"

# Test route inexistante
curl -i http://localhost:3000/nonexistent
```

## Rate Limiting et Performance

### Limitations

**HTTP Endpoints :**
- Pas de rate limiting strict (confiance IoT)
- Timeout côté ESP32 : 5 secondes
- Délai minimum entre requêtes : 1 seconde

**WebSocket :**
- Connexions simultanées : Illimitées (développement)
- Heartbeat automatique : 30 secondes
- Reconnexion automatique côté client

### Performances attendues

| Métrique | Valeur cible |
|----------|--------------|
| Latence HTTP | < 50ms |
| Latence WebSocket | < 100ms |
| Throughput | 100 req/sec |
| Temps de reconnexion | < 3 secondes |

## Sécurité

### CORS Policy

```javascript
// Configuration actuelle (développement)
{
    origin: "*",
    methods: ["GET", "POST"]
}
```

**⚠️ Production :** Restreindre les origines autorisées

### Validation des données

**Query Parameters :**
- `status` : Validation par whitelist
- Échappement automatique des caractères dangereux

**WebSocket :**
- Pas de données sensibles transmises
- Validation côté client recommandée

## Monitoring et Logs

### Logs Serveur

```
[2025-10-06 10:30:00] INFO: Serveur démarré sur port 3000
[2025-10-06 10:30:15] INFO: Client connecté: socket_abc123
[2025-10-06 10:30:45] INFO: Signal IoT reçu: status=ok
[2025-10-06 10:30:45] INFO: Diffusion WebSocket: energie=ok
```

### Métriques recommandées

- Nombre de connexions WebSocket actives
- Fréquence des signaux IoT
- Temps de réponse par endpoint
- Taux d'erreur par type

## Évolutions Futures

### API v2 (Proposée)

```
POST /api/v2/iot/signals
{
  "device_id": "esp32_001",
  "sensor": "ldr",
  "value": 2048,
  "threshold": 1800,
  "status": "ok"
}
```

### Authentification

```
Authorization: Bearer jwt_token_here
```

### Webhooks

```
POST /api/v2/webhooks/game-completed
{
  "game_id": "uuid",
  "user_id": "uuid", 
  "score": 100,
  "completion_time": 300
}
```

---

**Cette API est conçue pour être simple d'utilisation tout en offrant une communication fiable entre les composants web et IoT d'EcoEscape.**