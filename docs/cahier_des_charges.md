# Cahier des Charges - EcoEscape

## 1. Présentation du Projet

### 1.1 Contexte
**EcoEscape** est un escape game éducatif connecté développé dans le cadre d'un projet pédagogique associant développement web et Internet des Objets (IoT). Le projet vise à sensibiliser les utilisateurs aux enjeux environnementaux tout en découvrant les technologies modernes.

### 1.2 Objectifs
- **Objectif principal** : Créer une expérience ludique et éducative sur l'environnement
- **Objectif technique** : Maîtriser l'intégration Web ↔ IoT en temps réel
- **Objectif pédagogique** : Sensibiliser au tri des déchets et aux énergies renouvelables

## 2. Spécifications Fonctionnelles

### 2.1 Énigme 1 : Tri Sélectif
**Description** : Mini-jeu de drag & drop pour trier des déchets

**Fonctionnalités** :
- 5 déchets à trier (bouteille verre, canette, journal, pile, bouteille plastique)
- 4 types de poubelles (Verre, Plastique/Métal, Papier, Dangereux)
- Interface drag & drop native HTML5
- Validation en temps réel avec feedback visuel
- Progression affichée (score/total)
- Déverrouillage automatique de l'énigme 2

**Critères d'acceptation** :
- Tous les déchets doivent être triés correctement
- Interface responsive et intuitive
- Animations de feedback (succès/erreur)
- Passage automatique à l'énigme suivante

### 2.2 Énigme 2 : Panneau Solaire
**Description** : Activation d'un panneau solaire virtuel via capteur de lumière réel

**Fonctionnalités** :
- Mesure de luminosité en temps réel (ESP32 + LDR)
- Affichage du niveau d'énergie (barre de progression)
- LED virtuelle et physique synchronisées
- Communication bidirectionnelle Web ↔ IoT
- Instructions claires pour l'utilisateur

**Critères d'acceptation** :
- Détection de luminosité fiable et paramétrable
- Synchronisation temps réel via WebSocket
- Interface claire avec statut IoT
- Fin de jeu avec message de victoire

## 3. Spécifications Techniques

### 3.1 Architecture Globale
- **Frontend** : HTML5 + CSS3 + JavaScript vanilla
- **Backend** : Node.js + Express + Socket.io
- **IoT** : ESP32 + Arduino IDE
- **Communication** : HTTP REST + WebSocket

### 3.2 Technologies Imposées
- Pas de framework frontend obligatoire
- Socket.io pour le temps réel
- ESP32 avec WiFi
- Serveur de fichiers statiques intégré

### 3.3 Matériel IoT
- ESP32 DevKit v1 (ou compatible)
- LDR (Light Dependent Resistor)
- 2 LEDs (rouge/verte) + résistances
- Breadboard et câblage

## 4. Contraintes et Limitations

### 4.1 Contraintes Techniques
- **Performance** : Latence WebSocket < 500ms
- **Fiabilité** : Gestion des déconnexions réseau
- **Simplicité** : Code lisible et bien documenté
- **Portabilité** : Fonctionnement multi-plateforme

### 4.2 Contraintes Utilisateur
- **Accessibilité** : Interface intuitive sans formation
- **Compatibilité** : Navigateurs modernes (Chrome, Firefox, Safari)
- **Responsive** : Adaptation mobile/tablette/desktop

### 4.3 Contraintes Projet
- **Délai** : Développement sur une période définie
- **Équipe** : 5 personnes avec rôles spécialisés
- **Budget** : Composants électroniques économiques

## 5. Livrables

### 5.1 Code Source
- Serveur Node.js complet et fonctionnel
- Frontend responsive avec deux énigmes
- Code ESP32 Arduino documenté
- Structure de projet claire et organisée

### 5.2 Documentation
- README complet avec instructions d'installation
- Documentation technique (API, architecture)
- Schéma de câblage électronique
- Guide de dépannage

### 5.3 Démonstration
- Présentation fonctionnelle du projet
- Démonstration en temps réel des deux énigmes
- Explication technique de l'architecture IoT

## 6. Organisation de l'Équipe

### 6.1 Répartition des Rôles (5 personnes)

**Chef de Projet / Scénariste**
- Coordination générale du projet
- Définition du scénario pédagogique
- Validation des livrables
- Tests d'intégration finaux

**Développeur Frontend**
- Création de l'interface utilisateur
- Implémentation du drag & drop
- Responsive design et UX
- Intégration Socket.io côté client

**Développeur Backend / Réseau**
- Développement du serveur Express
- Configuration Socket.io serveur
- API REST et gestion des routes
- Déploiement et configuration réseau

**Développeur IoT**
- Programmation ESP32 Arduino
- Câblage et tests électroniques
- Communication WiFi et HTTP
- Calibrage des capteurs

**Designer / Présentateur**
- Charte graphique écologique
- Assets visuels et icônes
- Documentation utilisateur
- Préparation de la présentation finale

### 6.2 Jalons du Projet

**Phase 1 : Conception (Semaine 1)**
- Finalisation du cahier des charges
- Définition de l'architecture technique
- Répartition des tâches détaillées

**Phase 2 : Développement (Semaines 2-3)**
- Développement parallèle des composants
- Tests unitaires de chaque module
- Intégration progressive

**Phase 3 : Intégration (Semaine 4)**
- Assemblage de tous les composants
- Tests d'intégration complète
- Correction des bugs et optimisations

**Phase 4 : Finalisation (Semaine 5)**
- Documentation finale
- Préparation de la démonstration
- Tests de validation utilisateur

## 7. Critères de Réussite

### 7.1 Critères Techniques
- ✅ Les deux énigmes fonctionnent individuellement
- ✅ La communication IoT ↔ Web est fluide
- ✅ Le code est propre et documenté
- ✅ L'installation est simple et guidée

### 7.2 Critères Pédagogiques
- ✅ Le message écologique est clair
- ✅ L'expérience est engageante et ludique
- ✅ Les technologies IoT sont accessibles
- ✅ Le travail d'équipe est valorisé

### 7.3 Critères Utilisateur
- ✅ Interface intuitive sans manuel
- ✅ Temps de résolution raisonnable (5-10 min)
- ✅ Feedback satisfaisant lors de la réussite
- ✅ Robustesse face aux erreurs utilisateur

## 8. Gestion des Risques

### 8.1 Risques Techniques
- **Problème WiFi ESP32** → Solution de fallback avec simulation
- **Latence WebSocket** → Optimisation et mise en cache
- **Bugs navigateur** → Tests multi-plateformes

### 8.2 Risques Projet
- **Retard développement** → Priorisation des fonctionnalités core
- **Absence équipier** → Documentation et partage de code
- **Problème matériel** → Composants de rechange

## 9. Évolutions Futures

### 9.1 Fonctionnalités Additionnelles
- Système de scores et classements
- Énigmes supplémentaires (eau, transport, etc.)
- Mode multijoueur collaboratif
- Intégration de capteurs additionnels

### 9.2 Améliorations Techniques
- Application mobile native
- Base de données pour persistance
- Interface d'administration
- Déploiement cloud (Azure, AWS)

---

**Document validé par l'équipe EcoEscape**  
*Version 1.0 - Dernière mise à jour : Octobre 2025*