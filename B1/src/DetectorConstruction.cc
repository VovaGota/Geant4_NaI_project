//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
// Задачи: Вбить Дюралюминий, Добить спектры NaI
//
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"
#include "G4Scintillation.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4Material.hh"


namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 5 * cm, env_sizeZ = 30 * cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.0 * env_sizeXY;
  G4double world_sizeZ = 1.0 * env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld =
    new G4Box("World",  // its name
              1.0 * world_sizeXY, 1.0 * world_sizeXY, 1.0 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        world_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     checkOverlaps);  // overlaps checking

  //
  // Envelope
  //
  auto solidEnv = new G4Box("Envelope",  // its name
                            1.0 * env_sizeXY, 1.0 * env_sizeXY, 1.0 * env_sizeZ);  // its size

  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                      env_mat,  // its material
                                      "Envelope");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,  // its logical volume
                    "Envelope",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

//LAB scintallor               
 G4Element* elNa = nist->FindOrBuildElement("Na");
 G4Element* elI = nist->FindOrBuildElement("I");
 G4Element* elTl = nist->FindOrBuildElement("Tl");

 G4double density = 3.67*g/cm3;
 G4int ncomponents;
 G4Material* NaI = 
new G4Material("NaI"  , density, ncomponents=3);
 NaI->AddElement(elNa, 0.14025);
 NaI->AddElement(elI, 0.79475);
 NaI->AddElement(elTl, 0.065);

/*	 G4double scintilFast[] =
	{0.0,   0.0,   0.0,   0.0,
	 0.004, 0.026, 0.125, 0.380,
	 0.739};*/

/*	G4double scintilSlow[] =
	{0.0, 0.0, 0.0, 0.0,
	 0.0, 0.0, 0.0, 0.0,
	 0.0, 0.0, 0.0, 0.0,
	 0.0, 0.0, 0.0, 0.0,
	 0.0, 0.0, 0.0, 0.0,
	 0.0, 0.0, 0.0};*/

 const G4int NUMENTRIES = 20; // scint properties
 G4double NaI_PP[NUMENTRIES] = 
{1.0*eV,1.2*eV,1.5*eV,1.6*eV,1.8*eV, 1.9*eV,2.1*eV,2.2*eV,2.3*eV,2.5*eV,2.6*eV,
  2.8*eV,2.9*eV,3.1*eV,3.2*eV,3.3*eV,3.4*eV,3.6*eV,3.7*eV,3.8*eV}; // энергия выделившегося при сцинтилляции фотона
 G4double NaI_SCINT[NUMENTRIES] = 
{ 0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001, 0.017, 0.071, 
  0.246, 0.530, 0.746, 0.786, 0.698, 0.542, 0.393, 0.325, 0.217, 0.095};
  G4double NaI_RIND[NUMENTRIES] = 
{ 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 
  1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85}; // индекс преломления
 G4double NaI_ABSL[NUMENTRIES] = 
{ 91.*cm, 91.*cm, 91.*cm, 91.*cm, 84.2*cm, 73.1*cm, 63.5*cm, 59.6*cm, 51.8*cm, 47.1*cm, 
  42.3*cm, 36.1*cm, 30.9*cm, 27.3*cm, 24.8*cm, 15.5*cm, 6.3*cm, 1.5*cm, 0.7*cm, 0.7*cm}; // длина пробега волны

 G4MaterialPropertiesTable* NaI_MPT = new G4MaterialPropertiesTable();

//NaI_MPT->AddProperty("FASTCOMPONENT",NaI_PP,NaI_SCINT,NUMENTRIES);
NaI_MPT->AddProperty("RINDEX", NaI_PP, NaI_RIND, NUMENTRIES);
NaI_MPT->AddProperty("ABSLENGTH", NaI_PP, NaI_ABSL, NUMENTRIES);
NaI_MPT->AddConstProperty("SCINTILLATIONYIELD", 4500./MeV);
NaI_MPT->AddConstProperty("RESOLUTIONSCALE",1.0);
NaI_MPT->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
NaI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1",220.*ns);
NaI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500*ns);


NaI-> SetMaterialPropertiesTable(NaI_MPT);

G4Element* elSi = nist->FindOrBuildElement("Si");
G4Element* elCu = nist->FindOrBuildElement("Cu");
G4Element* elMn = nist->FindOrBuildElement("Mn");
G4Element* elMg = nist->FindOrBuildElement("Mg");
G4Element* elCr = nist->FindOrBuildElement("Cr");
G4Element* elZn = nist->FindOrBuildElement("Zn");
G4Element* elAl = nist->FindOrBuildElement("Al");

G4double densityDurAl = 2.74*g/cm3;
 G4Material* DurAl = 
new G4Material("DurAl"  , densityDurAl, ncomponents=7);
 DurAl->AddElement(elSi, 0.005);
 DurAl->AddElement(elCu, 0.025);
 DurAl->AddElement(elMn, 0.002);
 DurAl->AddElement(elMg, 0.004);
 DurAl->AddElement(elCr, 0.001);
 DurAl->AddElement(elZn, 0.001);
 DurAl->AddElement(elAl, 0.962);

  //
  // Shape 1
  //
  G4Material* shape1_mat = DurAl;
  G4ThreeVector pos1 = G4ThreeVector(0, 0 * cm, 0 * cm);

  // Conical section shape
  G4double shape1_rmina = 1.4 * cm, shape1_rmaxa = 1.5 * cm;
  G4double shape1_rminb = 1.4 * cm, shape1_rmaxb = 1.5 * cm;
  G4double shape1_hz = 15.2 * cm;
  G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
                                shape1_hz, shape1_phimin, shape1_phimax);

  auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
                                         shape1_mat,  // its material
                                         "Shape1");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos1,  // at position
                    logicShape1,  // its logical volume
                    "Shape1",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Shape 2  
  //
  G4Material* shape2_mat = NaI;
  G4ThreeVector pos2 = G4ThreeVector(0, 0 * cm, 0 * cm);

  // Trapezoid shape
  G4double shape2_rmina = 0. * cm, shape2_rmaxa = 1.27 * cm;
  G4double shape2_rminb = 0. * cm, shape2_rmaxb = 1.27 * cm;
  G4double shape2_hz = 15. * cm;
  G4double shape2_phimin = 0. * deg, shape2_phimax = 360. * deg;
  auto solidShape2 =
    new G4Cons("Shape2",  // its name
              shape2_rmina, shape2_rmaxa, shape2_rminb, shape2_rmaxb,
                                shape2_hz, shape2_phimin, shape2_phimax);  // its size

  auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
                                         shape2_mat,  // its material
                                         "Shape2");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos2,  // at position
                    logicShape2,  // its logical volume
                    "Shape2",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicShape1;

  //
  // always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
