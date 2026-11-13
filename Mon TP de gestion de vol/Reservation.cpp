#include "Reservation.h"
#include "Passager.h"
#include "Vol.h"
#include "Paiement.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

std::string statutReservationVersTexte(StatutReservation statut)
{
    switch (statut) {
        case StatutReservation::EN_ATTENTE: return "EN_ATTENTE";
        case StatutReservation::VALIDEE:    return "VALIDEE";
        case StatutReservation::PAYEE:      return "PAYEE";
        case StatutReservation::ANNULEE:    return "ANNULEE";
    }
    return "INCONNU";
}

Reservation::Reservation(int id, Passager* passager, Vol* vol)
    : id(id), passager(passager), vol(vol),
      statut(StatutReservation::EN_ATTENTE), paiement(nullptr)
{
    // Regle 4 : une reservation est associee a un seul passager ET a un seul vol
    if (passager == nullptr || vol == nullptr)
        throw std::invalid_argument("Une reservation doit etre associee a un passager et a un vol.");
}

int               Reservation::getId() const       { return id; }
Passager*         Reservation::getPassager() const { return passager; }
Vol*              Reservation::getVol() const      { return vol; }
StatutReservation Reservation::getStatut() const   { return statut; }
Paiement*         Reservation::getPaiement() const { return paiement; }

bool Reservation::occupeUnePlace() const
{
    return statut == StatutReservation::VALIDEE || statut == StatutReservation::PAYEE;
}

void Reservation::valider()
{
    if (statut != StatutReservation::EN_ATTENTE)
        throw std::logic_error("Seule une reservation en attente peut etre validee.");
    vol->reserverPlace();                 // regle 2 : leve une exception si le vol est complet
    statut = StatutReservation::VALIDEE;  // regle 6 : place definitivement reservee
}

void Reservation::verifierPayable() const
{
    switch (statut) {
        case StatutReservation::ANNULEE:     // regle 5
            throw std::logic_error("Une reservation annulee ne peut plus etre utilisee pour effectuer un paiement.");
        case StatutReservation::EN_ATTENTE:
            throw std::logic_error("La reservation doit d'abord etre validee avant le paiement.");
        case StatutReservation::PAYEE:
            throw std::logic_error("Cette reservation est deja payee.");
        case StatutReservation::VALIDEE:
            break;
    }
}

void Reservation::associerPaiement(Paiement* p)
{
    verifierPayable();
    if (p == nullptr)
        throw std::invalid_argument("Paiement invalide.");
    paiement = p;
    statut   = StatutReservation::PAYEE;
}

double Reservation::annuler()
{
    if (statut == StatutReservation::ANNULEE)
        throw std::logic_error("Cette reservation est deja annulee.");

    double rembourse = 0.0;
    // Regle 7 : traitement selon le statut de remboursement
    if (statut == StatutReservation::PAYEE && paiement != nullptr)
        rembourse = paiement->rembourser();

    if (occupeUnePlace())
        vol->libererPlace();              // la place redevient disponible

    statut = StatutReservation::ANNULEE;
    return rembourse;
}

void Reservation::afficherEntete()
{
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(24) << "Passager"
              << std::setw(8)  << "Vol"
              << std::setw(24) << "Trajet"
              << "Statut" << std::endl;
    std::cout << std::string(75, '-') << std::endl;
}

void Reservation::afficherLigne() const
{
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(24) << passager->getNomComplet()
              << std::setw(8)  << vol->getNumero()
              << std::setw(24) << (vol->getVilleDepart() + " -> " + vol->getVilleArrivee())
              << statutReservationVersTexte(statut) << std::endl;
}

void Reservation::afficherDetails() const
{
    std::cout << "===== Reservation #" << id << " =====" << std::endl;
    std::cout << "Statut   : " << statutReservationVersTexte(statut) << std::endl;
    std::cout << "Passager : "; passager->afficher();
    std::cout << "Vol      : " << vol->getNumero() << " | "
              << vol->getVilleDepart() << " -> " << vol->getVilleArrivee()
              << " | " << vol->getDateDepart() << " a " << vol->getHeureDepart()
              << " | " << std::fixed << std::setprecision(0) << vol->getPrix() << " FCFA" << std::endl;
    if (paiement != nullptr) {
        std::cout << "Paiement : "; paiement->afficher();
    } else {
        std::cout << "Paiement : aucun" << std::endl;
    }
}
