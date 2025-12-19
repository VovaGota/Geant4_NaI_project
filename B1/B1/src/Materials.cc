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
    {1.2*eV, 1.5*eV, 1.6*eV, 1.8*eV, 1.9*eV, 2.1*eV, 2.2*eV, 2.3*eV, 2.5*eV, 2.6*eV,
    2.8*eV, 2.9*eV, 3.01*eV, 3.2*eV, 3.3*eV, 3.4*eV, 3.6*eV, 3.8*eV, 3.9*eV, 4.0*eV};

    G4double NaI_PP1[29] = 
    {1.06*eV, 1.23*eV, 1.38*eV, 1.54*eV, 1.70*eV, 1.87*eV, 2.04*eV, 2.21*eV, 2.43*eV, 2.58*eV,
    2.73*eV, 2.91*eV, 3.07*eV, 3.20*eV, 3.33*eV, 3.39*eV, 3.45*eV, 3.50*eV, 3.56*eV, 3.60*eV,
    3.69*eV, 3.84*eV, 4.00*eV, 4.16*eV, 4.35*eV, 4.51*eV, 4.67*eV, 4.83*eV, 4.94*eV};      
    
    G4double NaI_SCINT[NUMENTRIES] = 
    {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.02, 0.04, 0.14, 0.45,
    0.66, 0.86, 0.98, 0.86, 0.68, 0.49, 0.39, 0.21, 0.07, 0.01};
      
    G4double NaI_RIND[NUMENTRIES] = 
    { 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 
      1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85, 1.85};
      
    G4double NaI_ABSL[NUMENTRIES] = 
    { 91.*cm, 91.*cm, 91.*cm, 91.*cm, 84.2*cm, 73.1*cm, 63.5*cm, 59.6*cm, 51.8*cm, 47.1*cm, 
      42.3*cm, 36.1*cm, 30.9*cm, 27.3*cm, 24.8*cm, 15.5*cm, 6.3*cm, 1.5*cm, 0.7*cm, 0.7*cm};
    G4double NaI_ABSL1[NUMENTRIES] = 
    { 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 
        1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m, 1.*m};
    G4double NaI_ABSL3[NUMENTRIES] = 
    { 91.*cm, 91.*cm, 91.*cm, 91.*cm, 84.2*cm, 73.1*cm, 68.5*cm, 65.6*cm, 60.8*cm, 55.*cm, // changed to +hz cm after 51.8 
      53.*cm, 45.*cm, 43.*cm, 40.*cm, 37.*cm, 35.*cm, 25.*cm, 20.*cm, 20.*cm, 20.*cm}; // +hz cm after hz cm
    G4double NaI_ABSL4[NUMENTRIES] = 
    { 70.*cm, 70.*cm, 70.*cm, 70.*cm, 70.2*cm, 70.1*cm, 68.5*cm, 65.6*cm, 60.8*cm, 55.*cm, // changed to +hz cm after 51.8 
      53.*cm, 45.*cm, 43.*cm, 40.*cm, 37.*cm, 35.*cm, 25.*cm, 20.*cm, 20.*cm, 20.*cm}; // +hz cm after hz cm
    G4double NaI_ABSL2[29] = 
    {367.15*mm, 368.34*mm, 368.34*mm, 371.93*mm, 365.97*mm, 346.39*mm, 334.29*mm, 320.52*mm, 295.63*mm, 281.63*mm,
    264.42*mm, 247.46*mm, 228.24*mm, 200.06*mm, 150.14*mm, 118.76*mm, 87.03*mm, 57.36*mm, 32.84*mm, 21.92*mm,
    12.99*mm, 11.13*mm, 10.81*mm, 11.02*mm, 11.02*mm, 11.0*mm2, 11.02*mm, 11.02*mm, 11.02*mm};

    G4MaterialPropertiesTable* NaI_MPT = new G4MaterialPropertiesTable();
    NaI_MPT->AddProperty("RINDEX", NaI_PP, NaI_RIND, NUMENTRIES);
    NaI_MPT->AddProperty("ABSLENGTH", NaI_PP, NaI_ABSL3, NUMENTRIES);
    NaI_MPT->AddProperty("SCINTILLATIONCOMPONENT1", NaI_PP, NaI_SCINT, NUMENTRIES);
    NaI_MPT->AddProperty("SCINTILLATIONCOMPONENT2", NaI_PP, NaI_SCINT, NUMENTRIES);
    NaI_MPT->AddConstProperty("SCINTILLATIONYIELD", 40000./MeV);
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

    G4MaterialPropertiesTable* Bialkali_MPT = new G4MaterialPropertiesTable();

    G4double photonEnergyBialkali_ABS[2] = {0.5*eV,6.5*eV};
    G4double absLengthBialkali[2] = {1.*nm,  1.*nm};

    G4double BialkaliphotonEnergy[62] = {1.79*eV, 1.80*eV, 1.82*eV, 1.84*eV, 1.87*eV, 1.89*eV, 1.91*eV, 1.93*eV, 1.96*eV, 1.98*eV,
    2.01*eV, 2.04*eV, 2.06*eV, 2.09*eV, 2.12*eV, 2.15*eV, 2.18*eV, 2.21*eV, 2.24*eV, 2.27*eV,
    2.31*eV, 2.34*eV, 2.36*eV, 2.38*eV, 2.39*eV, 2.42*eV, 2.45*eV, 2.49*eV, 2.53*eV, 2.58*eV,
    2.62*eV, 2.66*eV, 2.71*eV, 2.75*eV, 2.80*eV, 2.86*eV, 2.91*eV, 2.97*eV, 3.02*eV, 3.08*eV,
    3.15*eV, 3.21*eV, 3.28*eV, 3.35*eV, 3.42*eV, 3.50*eV, 3.58*eV, 3.66*eV, 3.75*eV, 3.85*eV,
    3.93*eV, 3.98*eV, 4.03*eV, 4.08*eV, 4.12*eV, 4.17*eV, 4.23*eV, 4.33*eV, 4.45*eV, 4.58*eV,
    4.72*eV, 4.87*eV};
    G4double rindexBialkali[62] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10, //Reflectivity of CsSbK
    0.11, 0.12, 0.13, 0.14, 0.14, 0.14, 0.14, 0.15, 0.15, 0.15,
    0.16, 0.17, 0.17, 0.18, 0.18, 0.19, 0.19, 0.19, 0.20, 0.21,
    0.22, 0.22, 0.21, 0.21, 0.21, 0.21, 0.21, 0.22, 0.23, 0.24,
    0.25, 0.26, 0.25, 0.25, 0.24, 0.24, 0.23, 0.23, 0.22, 0.22,
    0.22, 0.21, 0.21, 0.21, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20,
    0.20, 0.20};

    Bialkali_MPT->AddProperty("REFLECTIVITY", BialkaliphotonEnergy, rindexBialkali, 62);
    Bialkali_MPT->AddProperty("ABSLENGTH", photonEnergyBialkali_ABS, absLengthBialkali, 2);
    
    fBialkali->SetMaterialPropertiesTable(Bialkali_MPT);
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
    
    G4double photonEnergy[2] = {0.3 * eV, 6.0 * eV};
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

    fPMTSurface = new G4OpticalSurface("PMTSurface");
    fPMTSurface->SetType(dielectric_metal);
    fPMTSurface->SetFinish(ground);
    fPMTSurface->SetModel(unified);
    fTiO2Surface->SetSigmaAlpha(0.1);
}