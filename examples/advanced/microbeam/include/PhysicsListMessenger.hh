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
/// \file electromagnetic/TestEm5/include/PhysicsListMessenger.hh
/// \brief Definition of the PhysicsListMessenger class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PhysicsListMessenger_h
#define PhysicsListMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsListMessenger: public G4UImessenger
{
public:
  
<<<<<<< HEAD
    PhysicsListMessenger(PhysicsList* );
   ~PhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
=======
  PhysicsListMessenger(PhysicsList* );
 ~PhysicsListMessenger();
    
  virtual void SetNewValue(G4UIcommand*, G4String);

  inline G4double GetMaxChargedStep() const { return fMaxChargedStep; }
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    
private:
  
<<<<<<< HEAD
    PhysicsList* fPhysicsList;
    
    G4UIdirectory*             fPhysDir;        
    G4UIcmdWithADoubleAndUnit* fGammaCutCmd;
    G4UIcmdWithADoubleAndUnit* fElectCutCmd;
    G4UIcmdWithADoubleAndUnit* fProtoCutCmd;    
    G4UIcmdWithADoubleAndUnit* fAllCutCmd;    
    G4UIcmdWithAString*        fListCmd;
=======
  PhysicsList*               fPhysicsList;
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    
  G4UIdirectory*             fPhysDir;    
  G4UIcmdWithAString*        fListCmd;    
  G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
  G4double                   fMaxChargedStep;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

