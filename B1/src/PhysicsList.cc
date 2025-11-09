#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{
    //EM Physics
    RegisterPhysics(new G4EmStandartPhysics());

    //Radioactive Decay physics
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    //Decay physics
    RegisterPhysics(new G4DecayPhysics());
}

PhysicsList::~PhysicsList()
{
}