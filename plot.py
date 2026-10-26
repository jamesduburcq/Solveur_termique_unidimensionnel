import pandas as pd
import matplotlib.pyplot as plt

# ---------------------------------------------------------
# PRÉPARATION DES DONNÉES
# ---------------------------------------------------------
# Lecture de tous les fichiers
df_cuivre = pd.read_csv("testA_cuivre.csv", index_col=0)
df_inox   = pd.read_csv("testA_inox.csv", index_col=0)
df_beton  = pd.read_csv("testA_beton.csv", index_col=0)
df_b      = pd.read_csv("testB_accu.csv", index_col=0)
df_c      = pd.read_csv("testC_refroid.csv", index_col=0)
df_d      = pd.read_csv("testD_flux.csv", index_col=0)

# Les coordonnées x sont les mêmes pour tous les tests
x_coords = [float(col) for col in df_cuivre.columns]

# ---------------------------------------------------------
# TEST A : Influence du Matériau
# ---------------------------------------------------------
plt.figure(figsize=(8, 6)) # Création d'une nouvelle figure
plt.plot(x_coords, df_cuivre.iloc[-1], label="Cuivre (très conducteur)", linewidth=2)
plt.plot(x_coords, df_inox.iloc[-1], label="Inox (moyen)", linewidth=2)
plt.plot(x_coords, df_beton.iloc[-1], label="Béton (isolant)", linewidth=2)

plt.title("TEST A : Matériaux à t = 5000s (Dirichlet 100°C et 20°C)")
plt.xlabel("Position (m)")
plt.ylabel("Température (°C)")
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend(fontsize=10, loc="best")
plt.tight_layout()
plt.savefig("graphique_Test_A.png", dpi=300) # Sauvegarde en haute résolution (300 dpi)
plt.close() # On ferme la figure courante
print("Graphique du Test A sauvegardé.")

# ---------------------------------------------------------
# TEST B : Accumulation (Neumann 0)
# ---------------------------------------------------------
plt.figure(figsize=(8, 6))
for time, temps in df_b.iterrows():
    plt.plot(x_coords, temps, label=f"t = {time} s")
    
plt.title("TEST B : Accumulation Cuivre (100°C à G, Isolé à D)")
plt.xlabel("Position (m)")
plt.ylabel("Température (°C)")
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend(fontsize=8, loc="lower right", ncol=2)
plt.tight_layout()
plt.savefig("graphique_Test_B.png", dpi=300)
plt.close()
print("Graphique du Test B sauvegardé.")

# ---------------------------------------------------------
# TEST C : Refroidissement Asymétrique
# ---------------------------------------------------------
plt.figure(figsize=(8, 6))
for time, temps in df_c.iterrows():
    plt.plot(x_coords, temps, label=f"t = {time} s")
    
plt.title("TEST C : Refroidissement Cuivre (Init 100°C, 20°C à G, Isolé à D)")
plt.xlabel("Position (m)")
plt.ylabel("Température (°C)")
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend(fontsize=8, loc="upper right", ncol=2)
plt.tight_layout()
plt.savefig("graphique_Test_C.png", dpi=300)
plt.close()
print("Graphique du Test C sauvegardé.")

# ---------------------------------------------------------
# TEST D : Injection de Flux
# ---------------------------------------------------------
plt.figure(figsize=(8, 6))
for time, temps in df_d.iterrows():
    plt.plot(x_coords, temps, label=f"t = {time} s")
    
plt.title("TEST D : Flux entrant (Inox, Chaleur injectée à G, 20°C à D)")
plt.xlabel("Position (m)")
plt.ylabel("Température (°C)")
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend(fontsize=8, loc="upper right", ncol=2)
plt.tight_layout()
plt.savefig("graphique_Test_D.png", dpi=300)
plt.close()
print("Graphique du Test D sauvegardé.")

print("Tous les graphiques ont été générés avec succès !")