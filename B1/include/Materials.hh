#ifndef MATERIALS_HH
#define MATERIALS_HH

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"

class Materials {
public:
    // Static method to get instance (Singleton pattern)
    static Materials* GetInstance();
    
    // Destructor
    virtual ~Materials();
    
    // Methods to get materials
    G4Material* GetNaI() const { return fNaI; }
    G4Material* GetDurAl() const { return fDurAl; }
    G4Material* GetMyGlass() const { return fMyGlass; }
    G4Material* GetBialkali() const { return fBialkali; }
    G4Material* GetBorosilicateGlass() const { return fBorosilicateGlass; }
    G4Material* GetTiO2() const { return fTiO2; }
    
    // Methods to get optical surfaces
    G4OpticalSurface* GetTiO2Surface() const { return fTiO2Surface; }
    G4OpticalSurface* GetGlassSurface() const { return fGlassSurface; }
    
    // Method to initialize all materials
    void DefineMaterials();

private:
    // Private constructor (Singleton)
    Materials();
    
    // Static pointer to instance
    static Materials* fInstance;
    
    // Materials
    G4Material* fNaI;
    G4Material* fDurAl;
    G4Material* fMyGlass;
    G4Material* fBialkali;
    G4Material* fBorosilicateGlass;
    G4Material* fTiO2;
    
    // Component materials for Borosilicate glass
    G4Material* fSiO2;
    G4Material* fB2O3;
    G4Material* fNa2O;
    G4Material* fAl2O3;
    G4Material* fK20;
    
    // Optical surfaces
    G4OpticalSurface* fTiO2Surface;
    G4OpticalSurface* fGlassSurface;
    
    // Helper methods
    void CreateElements();
    void CreateNaIWithProperties();
    void CreateDurAlWithProperties();
    void CreateMyGlassWithProperties();
    void CreateBialkaliWithProperties();
    void CreateBorosilicateGlassWithProperties();
    void CreateTiO2WithProperties();
    void CreateOpticalSurfaces();
};

#endif // MATERIALS_HH