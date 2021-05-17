# Groupe F10

## Disclaimer

Ce ReadMe est assez long mais non pensons qu'il est nécessaire à la compréhension de notre code :)
Bonne lecture!

## Structures

Les différentes structures que nous utilisons sont :
* ### point_t, groupant les coordonnées d'un point
  - (fournie, headers/structures/point.h)
* ### cluster_t, groupant le centroïde et la taille du cluster
  - (fournie, headers/structures/cluster.h)
* ### kmComputation_t, structure contenant les différentes informations résultant du calcul des kmeans pour une combinaison initiale
  - (headers/structures/kmComputation.h)
* ### args_t, groupant les différents arguments de la fonction globale kmeans, utile pour le parsing de ces arguments
  - (headers/file_headers/main.h)

## Threads

1 thread génère des combinaisons, n_threads exécutent l'algorithme pour chaque combinaison et 1 thread écrit dans le fichier CSV d'output

## Concurrence

Nous utilisons la méthode producteur-consommateur pour résoudre le problème :
 
- Un premier buffer (bufferInit) contient des combinaisons initiales, 1 thread y produit des combinaisons et n_threads les consomment.  Ce buffer est protégé par un mutex et est couplé à 2 sémaphores.

- Un deuxième buffer (bufferComp) contient des structures kmComputation_t résultantes de l'exécution de l'algorithme pour 1 combinaison initiale, n_threads y produisent des kmComputation_t, 1 thread les consomment pour imprimer les résultats dans le fichier CSV d'output.  Ce buffer est également protégé par un mutex et couplé à 2 sémaphores.

## Approche utilisée

 Afin d'optimiser le temps d'exécution de l'algorithme de Loyd, le programme travaille pour chaque combinaison avec une liste de cluster_t "clusters" et un tableau de uint32_t "clusterParPoint" de taille nbPts tel que clusterParPoint[i] est l'indice du cluster dans la liste de cluster_t pour pts[i] (ce qui donne que pts[i] appartient au cluster clusters[clusterParPoint[i]]).


## Fonctionnement du projet

Le point de départ du programme se situe dans la fonction main.c

1. Tout d'abord, sur base d'un fichier binaire contenant l'ensemble des points à répartir en clusters, la fonction `parseBin` (voir src/parseBinary.c) va stocker ces différents points dans notre variable globale `pts` (voir headers/main.h) et va initialiser les variables globales nbPts (nombre total de points) et dim (dimension des points).

2. Tous les threads producteurs et consommateurs sont lancés dans le fichier "launcher.csv" :

  A) La fonction "launchComputations" lance les threads puis les "join".

  B) La fonction "Combinations" est la fonction lancée pour le thread qui produit des combinaisons initiales.

  C) La fonction "lauch_kmeans" est la fonction lancée pour chaque thread calculant les kmeans de combinaisons initiales, cette fonction utilise une variable "trackerComputations" qui compte le nombre de combinaisons initiales déjà calculées afin de savoir quand s'arrêter.  Cette variable est protégée par un mutex "mutex_trackerComps" étant donné que plusieurs threads calculent les kmeans.

  D) La fonction "launch_CSV" est appelée avec le thread qui imprime les résultats dans le fichier d'output, elle travaille également avec une variable "trackerCSV" comptant le nombre de combinaions initiales déjà imprimée.

3. Le fichier "createCSV.c" contient des fonctions auxiliaires utilisées par la fonction "launch_CSV".  Le fichier "algorithme.c" contient les fonctions composant l'algorithme de Loyd.  Le fichier "combinations.c" contient des fonctions auxiliaires appelées par "Combinations".
Le reste des fichiers contenus dans src sont des fonctions tierces, telles que les fonctions de distance, de distortion, ...


## !! TOREAD !! Gestion des erreurs

Toutes les erreurs d'allocation de mémoire sont gérées, ce qui rend le code moins lisible mais plus robuste.  Le lecteur ne doit donc pas s'attarder sur les multiples free suivant chaque test de malloc.
Une variable "errorFlag" dans "launcher.c" est prend la valeur -3 quand un thread rencontre une erreur.

La stratégie utilisée en cas d'erreur pour un des threads est la suivante :

- Quand un thread rencontre une erreur, il modifie la valeur de "errorFlag" à -3, libère ses ressources et signale aux threads qui sont potentiellement entrain de l'attendre (producteur attendant une place ds un buffer ou consommateur attendant que le buffer ne soit plus vide)
qu'il a rencontré une erreur en utilisant sem_post(<sémaphore adéquat>).

- Après chaque appel de sem_wait, chaque thread vérifie qu'il n'a pas été réveillé à cause d'une erreur, si c'est le cas il libère ses ressources et signale aux threads pouvant l'attendre qu'il s'arrête avec sem_post.

- Après avoir "joined" les threads dans "launchComputations", le programme vérifie si une erreur est arrivée, si tel est le cas les buffers sont nettoyés (les éléments n'ayant pas été consommés sont free).
