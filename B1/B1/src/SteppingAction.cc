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
//
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
#include "G4AnalysisManager.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4OpticalPhoton.hh"
//#include "Analysis.hh"
#include "Randomize.hh"
#include <G4VProcess.hh>
#include <cmath>
#define PI 3.14159265

int Nphoton1 = 0;
int Nphoton2 = 0;
int Nphoton3 = 0; //счётчик фотоэлектронов
int Nphoton4 = 0;

namespace B1 {

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double efficiency(G4double energy) {

	G4double photonEnergy[] =

	{1.84,1.86,1.90,1.95,2.02,
    2.10,2.24,2.38,2.56,2.78,
    3.05,3.38,3.78,4.14,4.35,
    4.50,4.60,4.73,4.83,4.93};


  G4double efficiencyPMT[] =
  {0.003,0.005,0.011,0.022,0.041,
    0.077,0.141,0.236,0.272,0.248,
    0.198,0.134,0.095,0.058,0.042,
    0.028,0.015,0.009,0.005,0.003};

  
	for (size_t i=0; i<20; i++)
	{
	  if (energy >= photonEnergy[i] && energy <= photonEnergy[i+1])
	     {
          G4double x1 = photonEnergy[i];
          G4double x2 = photonEnergy[i+1];
          G4double y1 = efficiencyPMT[i];
          G4double y2 = efficiencyPMT[i+1];
          return y1 + (y2 - y1) * (energy - x1) / (x2 - x1);
       }
	}
	return 0.0;
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }
  G4Track *aTrack=step->GetTrack();

  // position z tracking to ntuple
  /*if (aTrack->GetTrackID()==1 && aTrack->GetCurrentStepNumber()==1)
  {
    G4StepPoint* preStep = step->GetPreStepPoint();
    G4ThreeVector startPosition = preStep -> GetPosition();
    G4double StartZ = startPosition.z();
    fEventAction->RecPos(StartZ);
  }*/

    /*if (aTrack->GetVolume()->GetName()=="NaIScint" &&
      aTrack->GetDynamicParticle()->GetParticleDefinition()->GetParticleName()=="opticalphoton" &&
      step->IsFirstStepInVolume() &&
      //step->GetPreStepPoint()->GetStepStatus()==fGeomBoundary
      aTrack->GetCurrentStepNumber()==1) {
        fEventAction->AddPhoton();
      }*/

    G4LogicalVolume* volume 
      = step->GetPreStepPoint()->GetTouchableHandle()
        ->GetVolume()->GetLogicalVolume();

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  
  if ((volume == fScoringVolume) && (aTrack->GetDynamicParticle()->GetCharge()!=0)){
    // collect energy deposited in this step
      G4double edepStep = step->GetTotalEnergyDeposit();
      fEventAction->AddEdep(edepStep);
  }

  if (aTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
      return;
    
    // get volume of the current step
    G4StepPoint* postPoint = step->GetPostStepPoint();
    G4VPhysicalVolume* postVolume = postPoint->GetPhysicalVolume();
      G4LogicalVolume* pvolume 
      = step->GetPostStepPoint()->GetTouchableHandle()
        ->GetVolume()->GetLogicalVolume();

    if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary 
        && postVolume->GetName()=="Photocathode")
    {
        G4double Ephoton = aTrack->GetKineticEnergy();
        G4double prob_photon = efficiency(Ephoton*1e6);
        if (G4UniformRand() < prob_photon) {
          fEventAction->AddPhoton();
        }    
    } 

}
}