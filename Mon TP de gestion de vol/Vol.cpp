#include "Vol.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

Vol::Vol(const std::string& numero, const std::string& villeDepart,
         const std::string& villeArrivee, const std::string& dateDepart,
         const std::string& heureDepart, int capacite, double prix)
    : numero(numero), villeDepart(villeDepart), villeArrivee(villeArrivee),
      dateDepart(dateDepart), heureDepart(heureDepart),
      capacite(capacite), placesReservees(0), prix(prix)
{
    if (numero.empty() || villeDepart.empty() || villeArrivee.empty())
        throw std::invalid_argument("Le numero, la ville de depart et la ville d'arrivee sont obligatoires.");
    if (capacite <= 0)
        throw std::invalid_argument("La capacite d'un vol doit etre strictement positive.");
    if (prix < 0)
        throw std::invalid_argument("Le prix ne peut pas etre negatif.");
}

std::string Vol::getNumero() const       { return numero; }
std::string Vol::getVilleDepart() const  { return villeDepart; }
std::string Vol::getVilleArrivee() const { return villeArrivee; }
std::string Vol::getDateDepart() const   { return dateDepart; }
std::string Vol::getHeureDepart() const  { return heureDepart; }
int         Vol::getCapacite() const     { return capacite; }
int         Vol::getPlacesReservees() const { return placesReservees; }
double      Vol::getPrix() const         { return prix; }

int  Vol::getPlacesDisponibles() const { return capacite - placesReservees; }
bool Vol::estComplet() const           { return placesReservees >= capacite; }

void Vol::reserverPlace()
{
    if (estComplet())
        throw std::logic_error("Le vol " + numero + " est complet.");
    ++placesReservees;
}

void Vol::libererPlace()
{
    if (placesReservees > 0)
        --placesReservees;
}

void Vol::afficherEntete()
{
    std::cout << std::left
              << std::setw(8)  << "Vol"
              << std::setw(14) << "Depart"
              << std::setw(14) << "Arrivee"
              << std::setw(12) << "Date"
              << std::setw(8)  << "Heure"
              << std::setw(12) << "Prix(FCFA)"
              << "Places dispo" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
}

void Vol::afficherLigne() const
{
    std::cout << std::left
              << std::setw(8)  << numero
              << std::setw(14) << villeDepart
              << std::setw(14) << villeArrivee
              << std::setw(12) << dateDepart
              << std::setw(8)  << heureDepart
              << std::setw(12) << std::fixed << std::setprecision(0) << prix
              << getPlacesDisponibles() << "/" << capacite << std::endl;
}
