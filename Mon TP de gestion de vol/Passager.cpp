#include "Passager.h"

#include <iostream>
#include <stdexcept>

Passager::Passager(int id, const std::string& nom, const std::string& prenom,
                   const std::string& telephone, const std::string& email)
    : id(id), nom(nom), prenom(prenom), telephone(telephone), email(email)
{
    if (nom.empty() || prenom.empty())
        throw std::invalid_argument("Le nom et le prenom du passager sont obligatoires.");
}

int         Passager::getId() const        { return id; }
std::string Passager::getNom() const       { return nom; }
std::string Passager::getPrenom() const    { return prenom; }
std::string Passager::getTelephone() const { return telephone; }
std::string Passager::getEmail() const     { return email; }
std::string Passager::getNomComplet() const { return prenom + " " + nom; }

void Passager::afficher() const
{
    std::cout << "Passager #" << id << " : " << getNomComplet()
              << " | Tel : " << telephone
              << " | Email : " << email << std::endl;
}
