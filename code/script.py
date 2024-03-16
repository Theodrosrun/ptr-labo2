import sys
import matplotlib.pyplot as plt
import numpy as np
import os

def generate_histogram(data_path, output_name):
    # Lecture des données depuis le fichier
    with open(data_path, 'r') as file:
        donnees = [int(line.strip()) for line in file if line.strip()]

    # Conversion des données en array numpy
    data_array = np.array(donnees)

    # Création de l'histogramme
    plt.figure(figsize=(14, 7))
    plt.hist(data_array, bins=50, color='blue', alpha=0.7)
    plt.title('Histogramme des Fréquences des Données')
    plt.xlabel('Valeur des Données')
    plt.ylabel('Fréquence')

    # Vérification de l'existence du dossier 'doc' au niveau supérieur et création si nécessaire
    output_dir = os.path.join(os.path.dirname(__file__), '..', 'doc')
    os.makedirs(output_dir, exist_ok=True)

    # Construction du chemin complet de l'image de sortie
    output_path = os.path.join(output_dir, output_name)

    # Sauvegarde de l'histogramme dans le fichier de sortie
    plt.savefig(output_path)
    plt.close()

    print(f"L'image a été sauvegardée sous : {output_path}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <chemin_fichier_donnees.dat> <nom_fichier_sortie.png>")
        sys.exit(1)

    data_path = sys.argv[1]
    output_name = sys.argv[2]

    generate_histogram(data_path, output_name)

