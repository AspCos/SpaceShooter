# SpaceShooter

Prototype de jeu de tir spatial réalisé avec Unreal Engine 5.8 et C++. Le projet s’ouvre avec `SpaceShooter.uproject`. La carte de démarrage configurée est `Content/MyMap.umap` (`/Game/MyMap`).

## Jouer

Au lancement, le menu permet de démarrer une partie ou de quitter le jeu. Le nom affiché dans le menu est « Alexis Chopin ».

- Déplacement du vaisseau : **ZQSD**
- Tir : **touches fléchées** ; la direction du tir suit la ou les flèches maintenues
- Interface : score et vies restantes

Les astéroïdes apparaissent à intervalles aléatoires de 0,8 à 1,8 seconde, sur l’un des quatre bords de la zone de jeu et à une position aléatoire. Ils se dirigent vers le vaisseau à 350 unités Unreal/s. Chaque astéroïde reçoit aléatoirement entre 1 et 3 points de vie ; sa destruction rapporte 100 points. Une collision avec le vaisseau lui fait perdre une vie. La partie se termine à zéro vie.

## Structure et personnalisation

- `Source/SpaceShooter/` : logique C++ du vaisseau, des projectiles, des astéroïdes, de leur apparition, du mode de jeu et de l’interface.
- `Content/` : carte et Blueprints dérivés des classes de jeu.
- `Config/` : paramètres du projet et carte/mode de jeu par défaut.

Les classes C++ exposent des paramètres modifiables dans les Blueprints, notamment la vitesse et les limites de déplacement, la cadence et la classe des projectiles, la fréquence d’apparition des astéroïdes, leur vitesse et leur nombre de points de vie, les vies de départ et les sons. Le son d’explosion d’une météorite (`ExplosionSound`) et son volume (`ExplosionSoundVolume`, de 0 à 1) sont réglables dans le Blueprint de l’astéroïde. Les maillages actuels utilisent des primitives de l’Engine. Les propriétés d’effet de tir et de destruction existent, mais les particules et sons correspondants doivent être affectés dans les Blueprints pour obtenir ces effets.

## Contrôle de version

- Dépôt GitHub public : [AspCos/SpaceShooter](https://github.com/AspCos/SpaceShooter).
- Les branches `main` et `dev` sont présentes. L’historique de `dev` contient le commit `9037d66` (« Merge main into dev and resolve README conflict »), qui documente la résolution du conflit de fusion avec `main`.
- `.gitignore` exclut les fichiers temporaires et répertoires générés.
- `.p4ignore` contient les règles d’exclusion destinées à Perforce. La présence et l’historique des streams `main` et `dev`, ainsi que la résolution d’un conflit Perforce, doivent être démontrés avec l’historique Perforce ; ces éléments ne sont pas vérifiables à partir du dépôt Git seul.
