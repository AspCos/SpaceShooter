# Space Shooter

Prototype C++ pour Unreal Engine 5.8. Le vaisseau, les projectiles, les asteroides, le spawn, le score, les vies et l'interface sont implementes dans le module `SpaceShooter`. Les assets de mesh peuvent etre remplaces dans des Blueprints derives.

## Commandes

- Deplacement : WASD ou fleches
- Tir : Espace ou clic gauche

## Build Windows

Depuis PowerShell, dans le dossier du projet :

```powershell
& 'C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat' SpaceShooterEditor Win64 Development '-Project=SpaceShooter.uproject'
& 'C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat' SpaceShooter Win64 Shipping '-Project=SpaceShooter.uproject'
```

Le packaging est configure en Shipping avec pak et compression. Le prototype utilise actuellement la map moteur `Entry` pour le menu et le jeu ; les maps distinctes Menu et Niveau de jeu restent a creer dans le contenu du projet.

## Contenu a fournir

Les meshes sont des primitives de l'Engine. Les effets de tir et de destruction sont des proprietes Blueprint a renseigner. Le nom des membres de l'equipe, les captures d'historique Git/Perforce et la video de demonstration sont egalement a ajouter.

Les streams Perforce `Main` et `dev` existent deja sur le serveur, mais le workspace local `AspergeV2` est mappe vers un autre depot. Aucun depot Git n'est initialise dans ce dossier.