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
//#include "Analysis.hh"
#include <G4VProcess.hh>
#include <cmath>
#define PI 3.14159265

int Nphoton1 = 0;
int Nphoton2 = 0;
int Nphoton3 = 0;
int Nphoton4 = 0;

using namespace B1;

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

        G4double dE = abs(photonEnergy[0]-energy);
	int j=0;
	for (int i=1; i<20; i++)
	{
	  if (abs(photonEnergy[i]-energy)<dE)
	     {
		dE = photonEnergy[i]-energy;
        	j = i;
      	     }
	}
	return efficiencyPMT[j];
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

    // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  // collect energy deposited in this step
  G4Track *aTrack=step->GetTrack();
  G4double Energy = aTrack->GetTotalEnergy();
  G4double time = aTrack->GetGlobalTime();
	
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  
  if (aTrack->GetTrackStatus() == fStopAndKill) {
    if (aTrack->GetDefinition()->GetParticleName() == "opticalphoton") {
        if(volume->GetName()=="Photocathode"){
        G4double Ephoton = aTrack->GetKineticEnergy();
        G4double prob_photon = efficiency(Ephoton);
				Nphoton3++; 
  		}
	  }
  }

  if (volume == fScoringVolume){
    // collect energy deposited in this step
    G4double edepStep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edepStep);  

  // Сохраняем только если есть депонирование энергии (взаимодействие)
    if (edepStep > 0) {
      G4double Energy = aTrack->GetTotalEnergy();
      G4double time = aTrack->GetGlobalTime();
      
      man->FillNtupleDColumn(0, 0, Energy);    // Полная энергия
      man->FillNtupleDColumn(0, 1, edepStep);  // Депонированная энергия  
      man->FillNtupleDColumn(0, 2, time);      // Время
      man->AddNtupleRow(0);
    }  
  }
}