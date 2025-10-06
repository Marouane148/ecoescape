// ===========================================
// ECOESCAPE - Application Frontend
// ===========================================

// Variables globales
let score = 0;
const totalWastes = 5;
let socket;

// ===========================================
// ÉNIGME 1 : TRI SÉLECTIF
// ===========================================

// Sélection des éléments DOM
const dechets = document.querySelectorAll(".dechet");
const poubelles = document.querySelectorAll(".poubelle");
const msg = document.getElementById("msg");
const step1 = document.getElementById("step1");
const step2 = document.getElementById("step2");

// Initialisation du drag & drop
function initDragAndDrop() {
    // Configuration des déchets (draggable)
    dechets.forEach(dechet => {
        dechet.addEventListener("dragstart", (e) => {
            e.dataTransfer.setData("text/plain", dechet.dataset.type);
            dechet.style.opacity = "0.5";
        });

        dechet.addEventListener("dragend", (e) => {
            dechet.style.opacity = "1";
        });
    });

    // Configuration des poubelles (drop targets)
    poubelles.forEach(poubelle => {
        poubelle.addEventListener("dragover", (e) => {
            e.preventDefault();
            poubelle.style.background = "rgba(74, 170, 153, 0.3)";
        });

        poubelle.addEventListener("dragleave", (e) => {
            poubelle.style.background = "";
        });

        poubelle.addEventListener("drop", (e) => {
            e.preventDefault();
            poubelle.style.background = "";
            
            const wasteType = e.dataTransfer.getData("text/plain");
            const binType = poubelle.dataset.type;
            
            if (wasteType === binType) {
                handleCorrectDrop(poubelle);
            } else {
                handleIncorrectDrop(poubelle);
            }
        });
    });
}

// Gestion d'un tri correct
function handleCorrectDrop(poubelle) {
    score++;
    poubelle.classList.add("success");
    
    // Animation de succès
    setTimeout(() => {
        poubelle.classList.remove("success");
    }, 1000);
    
    console.log(`✅ Tri correct! Score: ${score}/${totalWastes}`);
    
    if (score === totalWastes) {
        completeFirstPuzzle();
    } else {
        msg.textContent = `✅ Bon tri ! (${score}/${totalWastes})`;
        msg.className = "message success";
    }
}

// Gestion d'un tri incorrect
function handleIncorrectDrop(poubelle) {
    poubelle.classList.add("error");
    
    setTimeout(() => {
        poubelle.classList.remove("error");
    }, 600);
    
    msg.textContent = "❌ Mauvais tri, réessaie !";
    msg.className = "message error";
    
    console.log("❌ Tri incorrect");
}

// Complétion de la première énigme
function completeFirstPuzzle() {
    msg.textContent = "🎉 Tri réactivé ! Passe à l'énergie solaire…";
    msg.className = "message success";
    
    setTimeout(() => {
        step1.classList.add("hidden");
        step2.classList.remove("hidden");
        console.log("🔄 Passage à l'énigme 2");
    }, 2000);
}

// ===========================================
// ÉNIGME 2 : PANNEAU SOLAIRE & WEBSOCKET
// ===========================================

// Éléments DOM pour l'énigme 2
const led = document.getElementById("led");
const energyBar = document.getElementById("energy");
const finalMessage = document.getElementById("final");
const connectionStatus = document.getElementById("connection");
const energyPercentage = document.querySelector(".energy-percentage");

// Initialisation de Socket.io
function initWebSocket() {
    console.log("🔌 Initialisation de la connexion WebSocket...");
    
    socket = io();

    // Événements de connexion
    socket.on("connect", () => {
        console.log("✅ Socket connecté:", socket.id);
        connectionStatus.textContent = "🟢 Connecté";
        connectionStatus.style.color = "#4caf50";
    });

    socket.on("disconnect", () => {
        console.log("❌ Socket déconnecté");
        connectionStatus.textContent = "🔴 Déconnecté";
        connectionStatus.style.color = "#f44336";
    });

    // Réception du signal d'énergie de l'ESP32
    socket.on("energie", (status) => {
        console.log("🔋 Signal énergie reçu:", status);
        
        if (status === "ok") {
            activateSolarPanel();
        }
    });

    // Gestion des erreurs
    socket.on("connect_error", (error) => {
        console.error("❌ Erreur de connexion WebSocket:", error);
        connectionStatus.textContent = "🔴 Erreur de connexion";
        connectionStatus.style.color = "#f44336";
    });
}

// Activation du panneau solaire
function activateSolarPanel() {
    console.log("☀️ Activation du panneau solaire");
    
    // LED verte
    led.textContent = "🟢";
    led.style.transform = "scale(1.2)";
    
    // Animation de la barre d'énergie
    energyBar.style.width = "100%";
    energyPercentage.textContent = "100%";
    
    // Message final
    finalMessage.textContent = "🎉 Panneau solaire réparé ! La planète est sauvée 🌍";
    finalMessage.className = "message success";
    
    // Effet visuel additionnel
    setTimeout(() => {
        document.body.style.background = "linear-gradient(135deg, #4caf50 0%, #8bc34a 50%, #cddc39 100%)";
    }, 1000);
    
    console.log("✅ Énigme 2 complétée !");
}

// ===========================================
// FONCTIONS UTILITAIRES
// ===========================================

// Test de connexion serveur
async function testServerConnection() {
    try {
        const response = await fetch('/health');
        const data = await response.json();
        console.log("🏥 Test de santé serveur:", data);
        return data.status === 'ok';
    } catch (error) {
        console.error("❌ Erreur de connexion serveur:", error);
        return false;
    }
}

// Affichage d'informations de debug
function showDebugInfo() {
    if (window.location.hash === '#debug') {
        const debugInfo = document.createElement('div');
        debugInfo.style.cssText = `
            position: fixed;
            top: 10px;
            right: 10px;
            background: rgba(0,0,0,0.8);
            color: white;
            padding: 10px;
            border-radius: 5px;
            font-family: monospace;
            font-size: 12px;
            z-index: 1000;
        `;
        debugInfo.innerHTML = `
            <strong>Debug Info</strong><br>
            Score: ${score}/${totalWastes}<br>
            Socket: ${socket ? socket.connected ? 'Connecté' : 'Déconnecté' : 'Non initialisé'}<br>
            Step 2 visible: ${!step2.classList.contains('hidden')}<br>
        `;
        document.body.appendChild(debugInfo);
        
        setInterval(() => {
            debugInfo.innerHTML = `
                <strong>Debug Info</strong><br>
                Score: ${score}/${totalWastes}<br>
                Socket: ${socket ? socket.connected ? 'Connecté' : 'Déconnecté' : 'Non initialisé'}<br>
                Step 2 visible: ${!step2.classList.contains('hidden')}<br>
            `;
        }, 1000);
    }
}

// ===========================================
// INITIALISATION
// ===========================================

// Démarrage de l'application
document.addEventListener("DOMContentLoaded", async () => {
    console.log("🚀 Initialisation d'EcoEscape");
    
    // Test de connexion serveur
    const serverOk = await testServerConnection();
    if (!serverOk) {
        console.warn("⚠️ Problème de connexion serveur");
    }
    
    // Initialisation des composants
    initDragAndDrop();
    initWebSocket();
    showDebugInfo();
    
    console.log("✅ EcoEscape initialisé avec succès");
    console.log("🎮 Commencez par trier les déchets !");
});

// Gestion des erreurs globales
window.addEventListener('error', (event) => {
    console.error('❌ Erreur JavaScript:', event.error);
});

window.addEventListener('unhandledrejection', (event) => {
    console.error('❌ Promise rejetée:', event.reason);
});