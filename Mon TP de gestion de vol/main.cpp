#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "Agence.h"

using namespace std;

// ---------- Fonctions de saisie securisee ----------
string lireLigne(const string& invite)
{
    string s;
    cout << invite;
    if (!getline(cin, s)) {                 // fin du flux (Ctrl+D / Ctrl+Z)
        cout << "\nFin de la saisie. Au revoir." << endl;
        exit(0);
    }
    return s;
}

int lireEntier(const string& invite)
{
    while (true) {
        string s = lireLigne(invite);
        try {
            size_t pos = 0;
            int v = stoi(s, &pos);
            if (pos != s.size()) throw invalid_argument("reste");
            return v;
        } catch (const exception&) {
            cout << "  [Erreur] Veuillez saisir un nombre entier valide." << endl;
        }
    }
}

double lireReel(const string& invite)
{
    while (true) {
        string s = lireLigne(invite);
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos != s.size()) throw invalid_argument("reste");
            return v;
        } catch (const exception&) {
            cout << "  [Erreur] Veuillez saisir un nombre valide." << endl;
        }
    }
}

// ---------- Actions du menu ----------
void actionEnregistrerPassager(Agence& agence)
{
    cout << "\n--- Enregistrer un passager ---" << endl;
    string nom    = lireLigne("Nom       : ");
    string prenom = lireLigne("Prenom    : ");
    string tel    = lireLigne("Telephone : ");
    string email  = lireLigne("Email     : ");
    Passager& p = agence.enregistrerPassager(nom, prenom, tel, email);
    cout << "[OK] Passager enregistre avec l'identifiant " << p.getId() << "." << endl;
}

void actionAjouterVol(Agence& agence)
{
    cout << "\n--- Ajouter un vol ---" << endl;
    string numero  = lireLigne("Numero du vol (ex. AF123)  : ");
    string depart  = lireLigne("Ville de depart            : ");
    string arrivee = lireLigne("Ville d'arrivee            : ");
    string date    = lireLigne("Date de depart (JJ/MM/AAAA): ");
    string heure   = lireLigne("Heure de depart (HH:MM)    : ");
    int    cap     = lireEntier("Capacite (nombre de places) : ");
    double prix    = lireReel("Prix du billet (FCFA)       : ");
    Vol& v = agence.ajouterVol(numero, depart, arrivee, date, heure, cap, prix);
    cout << "[OK] Vol " << v.getNumero() << " ajoute." << endl;
}

void actionVolsDisponibles(Agence& agence)
{
    cout << "\n--- Vols disponibles ---" << endl;
    agence.afficherVolsDisponibles();
}

void actionRechercherVol(Agence& agence)
{
    cout << "\n--- Rechercher un vol (laisser vide pour ignorer un critere) ---" << endl;
    string depart  = lireLigne("Ville de depart  : ");
    string arrivee = lireLigne("Ville d'arrivee  : ");
    agence.rechercherVols(depart, arrivee);
}

void actionReserver(Agence& agence)
{
    cout << "\n--- Effectuer une reservation ---" << endl;
    int    idP   = lireEntier("Identifiant du passager : ");
    string numVol = lireLigne("Numero du vol           : ");
    Reservation& r = agence.effectuerReservation(idP, numVol);
    cout << "[OK] Reservation #" << r.getId() << " creee (statut EN_ATTENTE)." << endl;

    string rep = lireLigne("Valider maintenant la reservation ? (o/n) : ");
    if (!rep.empty() && (rep[0] == 'o' || rep[0] == 'O')) {
        agence.validerReservation(r.getId());
        cout << "[OK] Reservation #" << r.getId() << " validee : place definitivement reservee." << endl;
    } else {
        cout << "[INFO] Reservation laissee EN_ATTENTE (aucune place consommee)." << endl;
    }
}

void actionPaiement(Agence& agence)
{
    cout << "\n--- Enregistrer un paiement ---" << endl;
    int id = lireEntier("Identifiant de la reservation : ");
    cout << "Mode de paiement : 1) Especes  2) Carte  3) Mobile Money" << endl;
    int choix = lireEntier("Votre choix : ");
    ModePaiement mode;
    switch (choix) {
        case 1: mode = ModePaiement::ESPECES;      break;
        case 2: mode = ModePaiement::CARTE;        break;
        case 3: mode = ModePaiement::MOBILE_MONEY; break;
        default: throw invalid_argument("Mode de paiement invalide.");
    }
    Paiement& p = agence.enregistrerPaiement(id, mode);
    cout << "[OK] "; p.afficher();
}

void actionAnnuler(Agence& agence)
{
    cout << "\n--- Annuler une reservation ---" << endl;
    int id = lireEntier("Identifiant de la reservation : ");
    double rembourse = agence.annulerReservation(id);
    cout << "[OK] Reservation #" << id << " annulee." << endl;
    if (rembourse > 0)
        cout << "[INFO] Remboursement de " << static_cast<long>(rembourse)
             << " FCFA (" << static_cast<int>(Paiement::TAUX_REMBOURSEMENT * 100)
             << "% du montant paye)." << endl;
    else
        cout << "[INFO] Aucun paiement a rembourser." << endl;
}

void actionConsulterReservations(Agence& agence)
{
    cout << "\n--- Liste des reservations ---" << endl;
    agence.afficherReservations();
}

void actionInfosReservation(Agence& agence)
{
    cout << "\n--- Informations d'une reservation ---" << endl;
    int id = lireEntier("Identifiant de la reservation : ");
    agence.afficherInfosReservation(id);
}

void afficherMenu()
{
    cout << "\n=========================================" << endl;
    cout << "   AGENCE DE VOYAGE - MENU PRINCIPAL"        << endl;
    cout << "=========================================" << endl;
    cout << " 1. Enregistrer un passager"                 << endl;
    cout << " 2. Ajouter un vol"                          << endl;
    cout << " 3. Afficher les vols disponibles"           << endl;
    cout << " 4. Rechercher un vol"                       << endl;
    cout << " 5. Effectuer une reservation"               << endl;
    cout << " 6. Enregistrer un paiement"                 << endl;
    cout << " 7. Annuler une reservation"                 << endl;
    cout << " 8. Consulter les reservations"              << endl;
    cout << " 9. Afficher les informations d'une reservation" << endl;
    cout << "10. Quitter"                                 << endl;
    cout << "-----------------------------------------"  << endl;
}

int main()
{
    Agence agence("Agence de Voyage de Cotonou");
    bool continuer = true;

    while (continuer) {
        afficherMenu();
        int choix = lireEntier("Votre choix : ");
        try {
            switch (choix) {
                case 1:  actionEnregistrerPassager(agence);   break;
                case 2:  actionAjouterVol(agence);            break;
                case 3:  actionVolsDisponibles(agence);       break;
                case 4:  actionRechercherVol(agence);         break;
                case 5:  actionReserver(agence);              break;
                case 6:  actionPaiement(agence);              break;
                case 7:  actionAnnuler(agence);               break;
                case 8:  actionConsulterReservations(agence); break;
                case 9:  actionInfosReservation(agence);      break;
                case 10: continuer = false;                   break;
                default: cout << "[Erreur] Choix invalide (1 a 10)." << endl;
            }
        } catch (const exception& e) {
            cout << "[Erreur] " << e.what() << endl;
        }
    }
    cout << "Merci d'avoir utilise l'application. Au revoir !" << endl;
    return 0;
}
