import matplotlib.pyplot as plt
import numpy as np
import sys

def generer_histogramme(fichier_donnees, nom_fichier_sortie):
    # Chargement des données depuis le fichier
    try:
        with open(fichier_donnees, 'r') as file:
            donnees = file.read().splitlines()
        donnees = [int(d) for d in donnees if d.isdigit()]
    except Exception as e:
        print(f"Erreur lors de la lecture du fichier {fichier_donnees}: {e}")
        sys.exit(1)

    # Conversion de la liste en un array numpy pour un traitement plus efficace
    data_array = np.array(donnees)

    # Création de l'histogramme
    plt.figure(figsize=(14, 7))
    plt.hist(data_array, bins=50, color='blue', alpha=0.7)
    plt.title('Histogramme des Fréquences des Données')
    plt.xlabel('Valeur des Données')
    plt.ylabel('Fréquence')

    # Sauvegarde de l'histogramme dans un fichier
    plt.savefig(nom_fichier_sortie)
    print(f"L'histogramme a été sauvegardé sous {nom_fichier_sortie}")

# Utilisation du script
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <chemin_fichier_donnees.dat> <nom_fichier_sortie.png>")
        sys.exit(1)

    fichier_donnees = sys.argv[1]
    nom_fichier_sortie = sys.argv[2]

    generer_histogramme(fichier_donnees, nom_fichier_sortie)

