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
#include "G4MaterialPropertiesTable.hh"
#include "G4Material.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"


namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : fScoringVolume(nullptr),
      fMaterials(Materials::GetInstance())  // ← инициализация в списке инициализации
{
}

DetectorConstruction::~DetectorConstruction()
{ 
    fMaterials = Materials::GetInstance();// Materials управляется самостоятельно через Singleton
}

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

// 
G4Material* NaI = fMaterials->GetNaI();
G4Material* TiO2 = fMaterials->GetTiO2();
G4Material* myGlass = fMaterials->GetMyGlass();
G4Material* Bialkali = fMaterials->GetBialkali();
G4Material* DurAl = fMaterials->GetDurAl();
G4Material* Borosilicate_glass = fMaterials->GetBorosilicateGlass();

// Оптические поверхности:
G4OpticalSurface* TiO2Surface = fMaterials->GetTiO2Surface();
G4OpticalSurface* GlassSurface = fMaterials->GetGlassSurface();


  //----------------------------------------------------------------------------------------------------------------------------------
  // Shape 1 DURAL
  //
  G4Material* shape1_mat = DurAl;
  G4ThreeVector pos1 = G4ThreeVector(0, 0 * cm, 0 * cm);

  // Conical section shape
  G4double shape1_rmina = 0 * cm, shape1_rmaxa = 1.5 * cm;
  G4double shape1_rminb = 0 * cm, shape1_rmaxb = 1.5 * cm;
  G4double shape1_hz = 7.6 * cm;
  G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
                                shape1_hz, shape1_phimin, shape1_phimax);

  auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
                                         shape1_mat,  // its material
                                         "Shape1");  // its name

  auto Mother_of_Reflector = 
  new G4PVPlacement(nullptr,  // no rotation
                    pos1,  // at position
                    logicShape1,  // its logical volume
                    "Shape1",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0);  // copy number
                    //checkOverlaps  // overlaps checking

  //------------------------------------------------------------------------------------------------------------------
  // Shape 3  reflector
  //
  G4Material* shape3_mat = TiO2;
  G4ThreeVector pos3 = G4ThreeVector(0, 0 * cm, -0.05 * cm);

  // Trapezoid shape
  G4double shape3_rmina = 0 * cm, shape3_rmaxa = 1.4 * cm;
  G4double shape3_rminb = 0 * cm, shape3_rmaxb = 1.4 * cm;
  G4double shape3_hz = 7.55 * cm;
  G4double shape3_phimin = 0. * deg, shape3_phimax = 360. * deg;
  auto solidShape3 =
    new G4Cons("Shape3",  // its name
              shape3_rmina, shape3_rmaxa, shape3_rminb, shape3_rmaxb,
                                shape3_hz, shape3_phimin, shape3_phimax);  // its size

  auto LogReflectorTiO2 = new G4LogicalVolume(solidShape3,  // its solid
                                         shape3_mat,  // its material
                                         "Shape3");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos3,  // at position
                    LogReflectorTiO2,  // its logical volume
                    "Shape3",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0);  // copy number
                    //checkOverlaps  // overlaps checking


  //---------------------------------------------------------------------------------------------------------------
  // Shape 2  NaI
  //
  G4Material* shape2_mat = NaI;
  G4ThreeVector pos2 = G4ThreeVector(0, 0 * cm, -0.05 * cm);

  // Trapezoid shape
  G4double shape2_rmina = 0. * cm, shape2_rmaxa = 1.27 * cm;
  G4double shape2_rminb = 0. * cm, shape2_rmaxb = 1.27 * cm;
  G4double shape2_hz = 7.5 * cm;
  G4double shape2_phimin = 0. * deg, shape2_phimax = 360. * deg;
  auto solidShape2 =
    new G4Cons("Shape2",  // its name
              shape2_rmina, shape2_rmaxa, shape2_rminb, shape2_rmaxb,
                                shape2_hz, shape2_phimin, shape2_phimax);  // its size

  auto LogScintillatorNaI = new G4LogicalVolume(solidShape2,  // its solid
                                         shape2_mat,  // its material
                                         "Shape2");  // its name

  auto PhysScintNaI = 
  new G4PVPlacement(nullptr,  // no rotation
                    pos2,  // at position
                    LogScintillatorNaI,  // its logical volume
                    "Shape2",  // its name
                    LogReflectorTiO2,  // its mother  volume
                    false,  // no boolean operation
                    0);  // copy number
                    //checkOverlaps  // overlaps checking
  //-----------------------------------------------------------------------------------------------------
  // Shape 4  Glass
 //  G4Material* Glass = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
  G4Material* shape4_mat = myGlass;
  G4ThreeVector pos4 = G4ThreeVector(0, 0 * cm, -7.7 * cm);

  // Trapezoid shape
  G4double shape4_rmina = 0 * cm, shape4_rmaxa = 1.27 * cm;
  G4double shape4_rminb = 0 * cm, shape4_rmaxb = 1.27 * cm;
  G4double shape4_hz = 0.1 * cm;
  G4double shape4_phimin = 0. * deg, shape4_phimax = 360. * deg;
  auto solidShape4 =
   new G4Cons("Shape4",  // its name
              shape4_rmina, shape4_rmaxa, shape4_rminb, shape4_rmaxb,
                                shape4_hz, shape4_phimin, shape4_phimax);  // its size

  auto logicShape4 = new G4LogicalVolume(solidShape4,  // its solid
                                         shape4_mat,  // its material
                                         "Shape4");  // its name

  auto GlassPhys =  
  new G4PVPlacement(nullptr,  // no rotation
                    pos4,  // at position
                    logicShape4,  // its logical volume
                    "Shape4",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0);  // copy number
                   // checkOverlaps   overlaps checking
  //------------------------------------------------------------------------------------------
  // Shape 5  Photocathode
  //
  G4Material* shape5_mat = Bialkali; //Точный материал фотокатода
  G4ThreeVector pos5 = G4ThreeVector(0, 0 * cm, -7.8 * cm);

  // Trapezoid shape
  G4double shape5_rmina = 0. * cm, shape5_rmaxa = 1.1 * cm;
  G4double shape5_rminb = 0. * cm, shape5_rmaxb = 1.1 * cm;
  G4double shape5_hz = 0.05 * cm;
  G4double shape5_phimin = 0. * deg, shape5_phimax = 360. * deg;
  auto solidShape5 =
    new G4Cons("Shape5",  // its name
              shape5_rmina, shape5_rmaxa, shape5_rminb, shape5_rmaxb,
                                shape5_hz, shape5_phimin, shape5_phimax);  // its size

  auto LogPhotocathode = new G4LogicalVolume(solidShape5,  // its solid
                                         shape5_mat,  // its material
                                         "LogPhotocathode");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos5,  // at position
                    LogPhotocathode,  // its logical volume
                    "Photocathode",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0);  // copy number  // overlaps checking
   
  //------------------------------------------------------------------------------------------
  // Shape 6 PMT
  //                    
  G4Material* shape6_mat = Borosilicate_glass; 
  G4ThreeVector pos6 = G4ThreeVector(0, 0 * cm, -10.22 * cm);

  // Trapezoid shape
  G4double shape6_rmina = 0. * cm, shape6_rmaxa = 1.27 * cm;
  G4double shape6_rminb = 0. * cm, shape6_rmaxb = 1.27 * cm;
  G4double shape6_hz = 2.47 * cm;
  G4double shape6_phimin = 0. * deg, shape6_phimax = 360. * deg;
  auto solidShape6 =
    new G4Cons("Shape6",  // its name
              shape6_rmina, shape6_rmaxa, shape6_rminb, shape6_rmaxb,
                                shape6_hz, shape6_phimin, shape6_phimax);  // its size

  auto logicShape6 = new G4LogicalVolume(solidShape6,  // its solid
                                         shape6_mat,  // its material
                                         "Shape6");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos6,  // at position
                    logicShape6,  // its logical volume
                    "Shape6",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0);  // copy number  // overlaps checking
//---------------------------------------------------------------------------------
  G4Material* shape7_mat =  nist->FindOrBuildMaterial("G4_AIR"); 
  G4ThreeVector pos7 = G4ThreeVector(0, 0 * cm, 0 * cm);

  // Trapezoid shape
  G4double shape7_rmina = 0. * cm, shape7_rmaxa = 1.17 * cm;
  G4double shape7_rminb = 0. * cm, shape7_rmaxb = 1.17 * cm;
  G4double shape7_hz = 2.37 * cm;
  G4double shape7_phimin = 0. * deg, shape7_phimax = 360. * deg;
  auto solidShape7 =
    new G4Cons("Shape7",  // its name
              shape7_rmina, shape7_rmaxa, shape7_rminb, shape7_rmaxb,
                                shape7_hz, shape7_phimin, shape7_phimax);  // its size

  auto logicShape7 = new G4LogicalVolume(solidShape7,  // its solid
                                         shape7_mat,  // its material
                                         "Shape7");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos7,  // at position
                    logicShape7,  // its logical volume
                    "Shape7",  // its name
                    logicShape6,  // its mother  volume
                    false,  // no boolean operation
                    0);  // copy number  // overlaps checking

// Creating Optical Surface for TiO2/NaI Border

    // Создаем граничную поверхность между сцинтиллятором и отражателем
    new G4LogicalBorderSurface(
        "ScintillatorTiO2Surface",  // имя поверхности
        PhysScintNaI,           // физический объем сцинтиллятора  
        Mother_of_Reflector,   // физический объем мира (или родителя отражателя)
        TiO2Surface                // оптическая поверхность
    );
// Creating Optical Surface for Glass/NaI Border

    // Создаем граничную поверхность между сцинтиллятором и отражателем
    new G4LogicalBorderSurface(
        "ScintillatorGlassSurface",  // имя поверхности
        PhysScintNaI,           // физический объем сцинтиллятора  
        GlassPhys,   // физический объем стекла
        GlassSurface                // оптическая поверхность
    );  


  // Set LogScintillatorNaI as scoring volume
  //
  fScoringVolume = LogScintillatorNaI;

    // visualization attributes ------------------------------------------------

  G4VisAttributes invisible(G4VisAttributes::GetInvisible());
  G4VisAttributes invisibleBlue(false, G4Colour::Blue());
  G4VisAttributes invisibleGreen(false, G4Colour::Green());
  G4VisAttributes invisibleYellow(false, G4Colour::Yellow());
  G4VisAttributes blue(G4Colour::Blue());
  G4VisAttributes cgray(G4Colour::Gray());
  G4VisAttributes green(G4Colour::Green());
  G4VisAttributes red(G4Colour::Red());
  G4VisAttributes yellow(G4Colour::Yellow());

  logicShape1->SetVisAttributes(red);
  LogScintillatorNaI->SetVisAttributes(blue);
  LogPhotocathode->SetVisAttributes(yellow);
  logicShape6->SetVisAttributes(green);
  //
  // always return the physical World
  //
  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    // Пустая реализация - можно добавить SensitiveDetectors при необходимости
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
