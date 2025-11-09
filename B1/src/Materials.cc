#include "Materials.hh"
#include "G4Element.hh"

// Initialize static pointer
Materials* Materials::fInstance = 0;

Materials* Materials::GetInstance() {
    if (fInstance == 0) {
        fInstance = new Materials();
    }
    return fInstance;
}

Materials::Materials() 
    : fNaI(0), fDurAl(0), fMyGlass(0), fBialkali(0), fBorosilicateGlass(0), 
      fTiO2(0), fSiO2(0), fB2O3(0), fNa2O(0), fAl2O3(0), fK20(0),
      fTiO2Surface(0), fGlassSurface(0) {
    DefineMaterials();
}

Materials::~Materials() {
    // Geant4 manages materials automatically
}

void Materials::DefineMaterials() {
    G4NistManager* nist = G4NistManager::Instance();
    
    // Create elements first
    CreateElements();
    
    // Create custom materials with properties
    CreateNaIWithProperties();
    CreateDurAlWithProperties();
    CreateMyGlassWithProperties();
    CreateBialkaliWithProperties();
    CreateBorosilicateGlassWithProperties();
    CreateTiO2WithProperties();
    CreateOpticalSurfaces();
}

void Materials::CreateElements() {
    // Elements are managed by Geant4, we just need to get them
    G4NistManager* nist = G4NistManager::Instance();
    // Elements will be retrieved when needed in material creation methods
}

void Materials::CreateNaIWithProperties() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* elNa = nist->FindOrBuildElement("Na");
    G4Element* elI = nist->FindOrBuildElement("I");
    G4Element* elTl = nist->FindOrBuildElement("Tl");
    
    G4double density = 3.67*g/cm3;
    G4int ncomponents;
    fNaI = new G4Material("NaI", density, ncomponents=3);
    fNaI->AddElement(elNa, 0.14025);
    fNaI->AddElement(elI, 0.79475);
    fNaI->AddElement(elTl, 0.065);

    const G4int NUMENTRIES = 20;
    G4double NaI_PP[NUMENTRIES] = 
    {1.0*eV,1.2*eV,1.5*eV,1.6*eV,1.8*eV, 1.9*eV,2.1*eV,2.2*eV,2.3*eV,2.5*eV,2.6*eV,
      2.8*eV,2.9*eV,3.1*eV,3.2*eV,3.3*eV,3.4*eV,3.6*eV,3.7*eV,3.8*eV};
    
    G4double NaI_SCINT[NUMENTRIES] = 
    { 0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001,  0.001, 0.017, 0.071, 
      0.246, 0.530, 0.746, 0.786, 0.698, 0.542, 0.393, 0.325, 0.217, 0.095};
      
    G4double NaI_RIND[NUMENTRIES] = 
    { 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 
      1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85};
      
    G4double NaI_ABSL[NUMENTRIES] = 
    { 91.*cm, 91.*cm, 91.*cm, 91.*cm, 84.2*cm, 73.1*cm, 63.5*cm, 59.6*cm, 51.8*cm, 47.1*cm, 
      42.3*cm, 36.1*cm, 30.9*cm, 27.3*cm, 24.8*cm, 15.5*cm, 6.3*cm, 1.5*cm, 0.7*cm, 0.7*cm};

    G4MaterialPropertiesTable* NaI_MPT = new G4MaterialPropertiesTable();
    NaI_MPT->AddProperty("RINDEX", NaI_PP, NaI_RIND, NUMENTRIES);
    NaI_MPT->AddProperty("ABSLENGTH", NaI_PP, NaI_ABSL, NUMENTRIES);
    NaI_MPT->AddProperty("SCINTILLATIONCOMPONENT1", NaI_PP, NaI_SCINT, NUMENTRIES);
    NaI_MPT->AddProperty("SCINTILLATIONCOMPONENT2", NaI_PP, NaI_SCINT, NUMENTRIES);
    NaI_MPT->AddConstProperty("SCINTILLATIONYIELD", 4500./MeV);
    NaI_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    NaI_MPT->AddConstProperty("SCINTILLATIONYIELD1", 0.96);
    NaI_MPT->AddConstProperty("SCINTILLATIONYIELD2", 0.04);
    NaI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 220.*ns);
    NaI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500.*ns);

    fNaI->SetMaterialPropertiesTable(NaI_MPT);
}

void Materials::CreateDurAlWithProperties() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* elSi = nist->FindOrBuildElement("Si");
    G4Element* elCu = nist->FindOrBuildElement("Cu");
    G4Element* elMn = nist->FindOrBuildElement("Mn");
    G4Element* elMg = nist->FindOrBuildElement("Mg");
    G4Element* elCr = nist->FindOrBuildElement("Cr");
    G4Element* elZn = nist->FindOrBuildElement("Zn");
    G4Element* elAl = nist->FindOrBuildElement("Al");
    
    G4double densityDurAl = 2.74*g/cm3;
    fDurAl = new G4Material("DurAl", densityDurAl, 7);
    fDurAl->AddElement(elSi, 0.005);
    fDurAl->AddElement(elCu, 0.025);
    fDurAl->AddElement(elMn, 0.002);
    fDurAl->AddElement(elMg, 0.004);
    fDurAl->AddElement(elCr, 0.001);
    fDurAl->AddElement(elZn, 0.001);
    fDurAl->AddElement(elAl, 0.962);
}

void Materials::CreateMyGlassWithProperties() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* elO = nist->FindOrBuildElement("O");
    G4Element* elSi = nist->FindOrBuildElement("Si");
    G4Element* elNa = nist->FindOrBuildElement("Na");
    G4Element* elCa = nist->FindOrBuildElement("Ca");
    
    fMyGlass = new G4Material("MyGlass", 2.5*g/cm3, 4);
    fMyGlass->AddElement(elO,  0.460);
    fMyGlass->AddElement(elSi, 0.333);
    fMyGlass->AddElement(elNa, 0.118);
    fMyGlass->AddElement(elCa, 0.089);

    G4MaterialPropertiesTable* glassMPT = new G4MaterialPropertiesTable();
    const G4int numPoints = 5;
    G4double photonEnergyGLASS[numPoints] = {
        1.0 * eV, 2.0 * eV, 3.0 * eV, 4.0 * eV, 6.0 * eV
    };
    
    G4double rindexGlass[numPoints] = {1.5, 1.5, 1.5, 1.5, 1.5};
    G4double absLengthGLASS[numPoints] = {
        100.0 * m, 100.0 * m, 100.0 * m, 10.0 * m, 1.0 * m
    };
    G4double reflectivityGLASS[numPoints] = {0.08, 0.08, 0.08, 0.08, 0.08};
    
    glassMPT->AddProperty("REFLECTIVITY", photonEnergyGLASS, reflectivityGLASS, numPoints);
    glassMPT->AddProperty("RINDEX", photonEnergyGLASS, rindexGlass, numPoints);
    glassMPT->AddProperty("ABSLENGTH", photonEnergyGLASS, absLengthGLASS, numPoints);
    
    fMyGlass->SetMaterialPropertiesTable(glassMPT);
}

void Materials::CreateBialkaliWithProperties() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* elCs = nist->FindOrBuildElement("Cs");
    G4Element* elSb = nist->FindOrBuildElement("Sb");
    G4Element* elK = nist->FindOrBuildElement("K");
    
    fBialkali = new G4Material("Bialkali", 2.0*g/cm3, 3);
    fBialkali->AddElement(elCs, 0.452);
    fBialkali->AddElement(elSb, 0.415);
    fBialkali->AddElement(elK, 0.133);
}

void Materials::CreateBorosilicateGlassWithProperties() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* elSi = nist->FindOrBuildElement("Si");
    G4Element* elO = nist->FindOrBuildElement("O");
    G4Element* elB = nist->FindOrBuildElement("B");
    G4Element* elNa = nist->FindOrBuildElement("Na");
    G4Element* elAl = nist->FindOrBuildElement("Al");
    G4Element* elK = nist->FindOrBuildElement("K");
    
    // Create component materials
    fSiO2 = new G4Material("SiO2", 2.65*g/cm3, 2);
    fSiO2->AddElement(elSi, 0.467);
    fSiO2->AddElement(elO, 0.533);
    
    fB2O3 = new G4Material("B2O3", 2.46*g/cm3, 2);
    fB2O3->AddElement(elB, 0.314);
    fB2O3->AddElement(elO, 0.686);
    
    fNa2O = new G4Material("Na2O", 2.27*g/cm3, 2);
    fNa2O->AddElement(elNa, 0.742);
    fNa2O->AddElement(elO, 0.258);
    
    fAl2O3 = new G4Material("Al2O3", 3.95*g/cm3, 2);
    fAl2O3->AddElement(elAl, 0.529);
    fAl2O3->AddElement(elO, 0.471);
    
    fK20 = new G4Material("K20", 2.35*g/cm3, 2);
    fK20->AddElement(elK, 0.83);
    fK20->AddElement(elO, 0.17);
    
    // Create Borosilicate glass
    fBorosilicateGlass = new G4Material("Borosilicate_glass", 2.23*g/cm3, 5);
    fBorosilicateGlass->AddMaterial(fSiO2, 81.*perCent);
    fBorosilicateGlass->AddMaterial(fB2O3, 12.5*perCent);
    fBorosilicateGlass->AddMaterial(fNa2O, 2.09*perCent);
    fBorosilicateGlass->AddMaterial(fAl2O3, 2.32*perCent);
    fBorosilicateGlass->AddMaterial(fK20, 2.09*perCent);
}

void Materials::CreateTiO2WithProperties() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* elTi = nist->FindOrBuildElement("Ti");
    G4Element* elO = nist->FindOrBuildElement("O");
    
    G4double densityTiO2 = 4.235*g/cm3;
    fTiO2 = new G4Material("TiO2", densityTiO2, 2);
    fTiO2->AddElement(elTi, 0.6);
    fTiO2->AddElement(elO, 0.4);

    G4MaterialPropertiesTable* TiO2_MPT = new G4MaterialPropertiesTable();
    
    G4double photonEnergy_RI[6] = {1.38*eV, 1.55*eV, 1.77*eV, 2.07*eV, 2.48*eV, 2.76*eV};
    G4double rindexTiO2[6] = {2.5, 2.52, 2.55, 2.6, 2.71, 2.81};
    TiO2_MPT->AddProperty("RINDEX", photonEnergy_RI, rindexTiO2, 6);
    
    G4double photonEnergy[2] = {1.3 * eV, 4.0 * eV};
    G4double reflectivity[2] = {0.95, 0.95};
    TiO2_MPT->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, 2);
    
    G4double photonEnergy_ABS[6] = {1.49*eV, 2.13*eV, 2.38*eV, 3.10*eV, 3.63*eV, 4.13*eV};
    G4double absLengthTiO2[6] = {0.000135*mm, 0.000198*mm, 0.000241*mm, 0.000294*mm, 0.000347*mm, 0.000411*mm};
    TiO2_MPT->AddProperty("ABSLENGTH", photonEnergy_ABS, absLengthTiO2, 6);
    
    fTiO2->SetMaterialPropertiesTable(TiO2_MPT);
}

void Materials::CreateOpticalSurfaces() {
    // Create TiO2 optical surface
    fTiO2Surface = new G4OpticalSurface("TiO2Surface");
    fTiO2Surface->SetType(dielectric_metal);
    fTiO2Surface->SetFinish(ground);
    fTiO2Surface->SetModel(unified);
    fTiO2Surface->SetSigmaAlpha(0.1);
    
    // Create Glass optical surface
    fGlassSurface = new G4OpticalSurface("GlassSurface");
    fGlassSurface->SetType(dielectric_dielectric);
    fGlassSurface->SetFinish(polished);
    fGlassSurface->SetModel(unified);
}