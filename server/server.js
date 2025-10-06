import express from "express";
import http from "http";
import { Server } from "socket.io";
import dotenv from "dotenv";
import cors from "cors";
import path from "path";
import { fileURLToPath } from "url";

// Configuration pour ES modules
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

dotenv.config();

const app = express();

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, "public")));

// Création du serveur HTTP et Socket.io
const server = http.createServer(app);
const io = new Server(server, { 
  cors: { 
    origin: "*",
    methods: ["GET", "POST"]
  } 
});

// Gestion des connexions Socket.io
io.on("connection", (socket) => {
  console.log("✅ Client connecté:", socket.id);
  
  socket.on("disconnect", () => {
    console.log("❌ Client déconnecté:", socket.id);
  });
});

// Routes API
app.get("/health", (req, res) => {
  console.log("Health check demandé");
  res.json({ status: "ok", timestamp: new Date().toISOString() });
});

app.get("/update", (req, res) => {
  const status = (req.query.status || "").toString();
  console.log(`📡 Requête IoT reçue - Status: ${status}`);
  
  if (status === "ok") {
    io.emit("energie", "ok");
    console.log("🔋 Signal énergie OK reçu de l'ESP32 → diffusion WebSocket à tous les clients");
  }
  
  res.json({ 
    received: true, 
    status: status,
    timestamp: new Date().toISOString()
  });
});

// Route par défaut pour servir l'index.html
app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "public", "index.html"));
});

// Démarrage du serveur
const PORT = process.env.PORT || 3000;
const HOST = process.env.HOST || "0.0.0.0";

server.listen(PORT, HOST, () => {
  console.log(`🌍 EcoEscape serveur démarré sur http://${HOST}:${PORT}`);
  console.log(`📱 Interface web disponible sur http://localhost:${PORT}`);
  console.log(`🔌 WebSocket prêt pour les connexions IoT`);
});