# 🔥 Solveur Thermique 1D en C++ Moderne

Modélisation mathématique, architecture logicielle orientée objet en **C++17** et validation numérique d'un **solveur 1D pour l'équation de la chaleur instationnaire** au sein d'un milieu homogène.

Ce projet met l'accent sur le découplage rigoureux des responsabilités physiques et numériques (géométrie, thermophysique, conditions aux limites, discrétisation spatiale et intégration temporelle implicite) via le **polymorphisme dynamique**, les principes **SOLID / RAII** et des structures matricielles creuses optimisées en $\mathcal{O}(N)$.

---

## 📖 Contexte et Modèle Mathématique

### 1. Équation continue de la chaleur

La conduction thermique instationnaire unidimensionnelle le long d'une barre homogène de longueur $L$, sans terme source interne, découle du principe de conservation de l'énergie :

$$\rho c_p \frac{\partial u}{\partial t}(x,t) - k \frac{\partial^2 u}{\partial x^2}(x,t) = 0, \qquad \forall x \in ]0, L[, \quad t > 0$$

En introduisant la **diffusivité thermique** du matériau $\alpha = \frac{k}{\rho c_p}$ (en $\text{m}^2\cdot\text{s}^{-1}$), où $k$ est la conductivité thermique ($\text{W}\cdot\text{m}^{-1}\cdot\text{K}^{-1}$), $\rho$ la masse volumique ($\text{kg}\cdot\text{m}^{-3}$) et $c_p$ la capacité thermique massique ($\text{J}\cdot\text{kg}^{-1}\cdot\text{K}^{-1}$), le problème se ramène à la forme canonique :

$$\frac{\partial u}{\partial t} = \alpha \frac{\partial^2 u}{\partial x^2}$$

---

### 2. Discrétisation spatiale (Différences Finies) et Conditions aux Limites

Le domaine $[0, L]$ est discrétisé en $N$ nœuds réguliers $x_i = i \Delta x$ avec $\Delta x = \frac{L}{N - 1}$. Aux nœuds internes, l'opérateur laplacien est approché par un schéma aux **différences finies centrées d'ordre 2** :

$$\left.\frac{\partial^2 u}{\partial x^2}\right\vert{}_{x_i} = \frac{u_{i+1} - 2u_i + u_{i-1}}{\Delta x^2} + \mathcal{O}(\Delta x^2)$$

Le système semi-discrétisé s'écrit sous forme matricielle globale :

$$M \frac{dU}{dt} + K U = F$$

- **$M$ (Matrice de masse)** : Identité aux nœuds intérieurs ($M_{i,i} = 1$), annulée aux bords pour imposer directement les contraintes algébriques des frontières.
- **$K$ (Matrice de rigidité)** : Tridiagonale de coefficients internes $K_{i,i} = \frac{2\alpha}{\Delta x^2}$ et $K_{i,i\pm 1} = -\frac{\alpha}{\Delta x^2}$.
- **$F$ (Vecteur second membre)** : Porte les contributions des conditions aux limites aux extrémités $x = 0$ et $x = L$ :
  - **Condition de Dirichlet (`DirichletDC`)** : Température prescrite aux parois ($u_0 = T_{\text{gauche}}$ ou $u_{N-1} = T_{\text{droite}}$).
  - **Condition de Neumann (`NeumannDC`)** : Flux thermique imposé selon la loi de Fourier ($\phi = -k \frac{\partial u}{\partial n}$), incluant le cas particulier d'une paroi parfaitement isolée / adiabatique ($\phi = 0$).

---

### 3. Intégration temporelle : Euler Implicite et Algorithme de Thomas

Afin de s'affranchir de la condition de stabilité restrictive de type CFL ($\Delta t \le \frac{\Delta x^2}{2\alpha}$) propre aux schémas explicites, l'intégration temporelle repose sur le schéma d'**Euler Implicite**, **inconditionnellement stable** :

$$(M + \Delta t \, K) U^{n+1} = M U^n + \Delta t \, F$$

À chaque pas de temps $\Delta t$, le système linéaire tridiagonal $A U^{n+1} = B$ (où $A = M + \Delta t \, K$ est assemblée une unique fois dans `setup(dt)`) est résolu exactement en **complexité temporelle et spatiale optimale $\mathcal{O}(N)$** via l'**algorithme de Thomas** (`solve_tridiagonal`), sans jamais allouer ni parcourir les coefficients nuls hors-diagonales.

---

## 🏗️ Architecture Logicielle et Conception C++

Le solveur est conçu selon les standards du **C++ moderne** afin de garantir modularité, extensibilité et sécurité mémoire :

1. **Abstractions pures et Principe *Open/Closed* (SOLID)** :
   - `SpatialDiscretization` définit le contrat d'assemblage des opérateurs ($M$, $K$, $F$). L'implémentation actuelle par Différences Finies (`DFDiscretization`) peut être remplacée par des Éléments Finis $\mathbb{P}_1$ sans modifier le solveur temporel.
   - `BoundaryCondition` abstrait les conditions aux bords (`DirichletDC`, `NeumannDC`) via `computeBoundaryValue`, permettant d'étendre le code (ex. condition de convection de Robin) de manière modulaire.
   - `TimeSolver` découple la boucle de marche en temps (`IE_Solver`) de la méthode spatiale.
2. **Stockage creux dédié (`TridiagonalMatrix`)** :
   - Représentation mémoire compacte à l'aide de trois vecteurs contigus (`lower` de taille $N-1$, `diag` de taille $N$, `upper` de taille $N-1$).
3. **Règle de Zéro, RAII et Agrégation** :
   - Aucune allocation dynamique manuelle (`new` / `delete`) : la gestion mémoire repose intégralement sur les conteneurs standards (`std::vector<double>`) et la synthèse automatique du compilateur.
   - Destruction polymorphique sécurisée (`virtual ~Class() = default`) sur toutes les interfaces.
   - `TimeSolver` maintient une référence constante (`const SpatialDiscretization&`) pour éviter toute copie superflue des matrices à chaque itération.

![Diagramme de classes UML](diagramme_uml.png)

---

## 📊 Campagne de Validation Numérique

Quatre scénarios physiques ont été menés sur une barre de longueur $L = 1.0\text{ m}$ afin de valider la précision numérique et le comportement physique aux frontières :

| Matériau | Conductivité $k$ ($\text{W}\cdot\text{m}^{-1}\cdot\text{K}^{-1}$) | Masse volumique $\rho$ ($\text{kg}\cdot\text{m}^{-3}$) | Capacité massique $c_p$ ($\text{J}\cdot\text{kg}^{-1}\cdot\text{K}^{-1}$) | Diffusivité $\alpha$ ($\text{m}^2\cdot\text{s}^{-1}$) | Comportement thermique |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Cuivre** | `400.0` | `8960.0` | `385.0` | $1.16 \times 10^{-4}$ | Très conducteur |
| **Inox** | `15.0` | `8000.0` | `500.0` | $3.75 \times 10^{-6}$ | Conducteur intermédiaire |
| **Béton** | `1.0` | `2300.0` | `880.0` | $4.94 \times 10^{-7}$ | Isolant thermique |

### Synthèse des cas tests

1. **Test A — Influence de la diffusivité ($t = 5000\text{ s}$)** : Barre initialement à $20\,^\circ\text{C}$ soumise à deux conditions de Dirichlet ($100\,^\circ\text{C}$ à gauche, $20\,^\circ\text{C}$ à droite). Le cuivre atteint déjà son régime permanent linéaire $u(x) = 100 - 80x$, l'inox est en régime transitoire au cœur du domaine, et l'échauffement du béton reste confiné à $x < 0.2\text{ m}$.
2. **Test B — Accumulation thermique avec paroi adiabatique** : Barre de cuivre ($20\,^\circ\text{C}$) chauffée à $100\,^\circ\text{C}$ à gauche et isolée à droite ($\phi = 0 \implies \left.\frac{\partial u}{\partial x}\right\vert{}_{x=L} = 0$). La chaleur ne pouvant s'échapper à droite, la barre s'homogénéise intégralement à $100\,^\circ\text{C}$ à $t = 20\,000\text{ s}$.
3. **Test C — Refroidissement asymétrique** : Barre de cuivre initialement à $100\,^\circ\text{C}$, refroidie à $20\,^\circ\text{C}$ à gauche et isolée à droite. Décroissance monotone des profils tout en conservant une tangente horizontale en $x = 1\text{ m}$ jusqu'à l'équilibre à $20\,^\circ\text{C}$.
4. **Test D — Injection continue d'un flux thermique (Neumann non homogène)** : Flux entrant $\phi = 5000\text{ W}\cdot\text{m}^{-2}$ à gauche dans une barre d'inox maintenue à $20\,^\circ\text{C}$ à droite. En raison de la faible conductivité de l'inox, une forte accumulation porte l'entrée au-delà de $2500\,^\circ\text{C}$ à $t = 50\,000\text{ s}$, convergeant vers la pente théorique de Fourier $\frac{\partial u}{\partial x} = -\frac{\phi}{k} \approx -333.33\text{ K}\cdot\text{m}^{-1}$.

![Résultats des Tests A et B](tests_A_B.png)
![Résultats des Tests C et D](tests_C_D.png)

---

## 📂 Structure du Dépôt

```text
├── BoundaryCondition.hpp       # Classe abstraite pure pour les conditions aux limites
├── DirichletDC.hpp             # Condition de Dirichlet (température imposée)
├── NeumannDC.hpp / .cpp        # Condition de Neumann (flux thermique imposé / paroi isolée)
├── SpatialDiscretization.hpp   # Interface d'assemblage spatial et structure TridiagonalMatrix
├── DFDiscretization.hpp / .cpp # Discrétisation par Différences Finies centrées d'ordre 2
├── TimeSolver.hpp              # Classe abstraite pour les schémas d'intégration temporelle
├── IE_Solver.hpp / .cpp        # Schéma d'Euler Implicite et solveur de Thomas O(N)
├── material.hpp / .cpp         # Propriétés thermophysiques et calcul de diffusivité
├── mesh_1D.hpp / .cpp          # Maillage 1D uniforme et stockage du champ de température
├── main.cpp                    # Point d'entrée : configuration et exécution d'une simulation
├── rapport_projet_edp.pdf      # Rapport technique complet (modèle, UML et validation)
└── README.md                   # Documentation du projet
```

---

## 🚀 Compilation et Utilisation

Le projet utilise exclusivement la bibliothèque standard **C++17** (aucune dépendance externe requise).

### 1. Cloner le dépôt

```bash
git clone [https://github.com/jamesduburcq/](https://github.com/jamesduburcq/)<Solveur_termique_unidimensionnel>.git
cd <Solveur_termique_unidimensionnel>
```

### 2. Compiler le projet

Avec `g++` (Linux, macOS ou MinGW sous Windows) :

```bash
g++ -std=c++17 -O2 main.cpp material.cpp mesh_1D.cpp NeumannDC.cpp DFDiscretization.cpp IE_Solver.cpp -o solveur_thermique
```

### 3. Lancer la simulation

```bash
./solveur_thermique
```

---

## 👥 Auteurs

Projet réalisé en **Septembre 2026** par :
- **James Duburcq** — [@jamesduburcq](https://github.com/jamesduburcq)
- **Orion Routhier** — [@OrionRouthier](https://github.com/OrionRouthier)
