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
<<<<<<< HEAD
// $Id: $
=======
/// \file GB02/exampleGB02.cc
/// \brief Main program of the GB02 example
//
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
//

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "GB02ActionInitialization.hh"

#include "G4UImanager.hh"

#include "GB02DetectorConstruction.hh"
#include "GB02PrimaryGeneratorAction.hh"

#include "FTFP_BERT.hh"
#include "G4GenericBiasingPhysics.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " ./exampleGB02 [-m macro ] "
           << " [-b biasing {'on','off'}]"
           << "\n or\n ./exampleGB02 [macro.mac]"
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int main(int argc,char** argv)
{
  // Evaluate arguments
  //
  if ( argc > 5 ) {
    PrintUsage();
    return 1;
  }
  
  G4String macro("");
  G4String onOffBiasing("");
  if ( argc == 2 ) macro = argv[1];
  else
    {
      for ( G4int i=1; i<argc; i=i+2 )
        {
          if      ( G4String(argv[i]) == "-m" ) macro        = argv[i+1];
          else if ( G4String(argv[i]) == "-b" ) onOffBiasing = argv[i+1];
          else
            {
              PrintUsage();
              return 1;
            }
        }
    }
  
  if ( onOffBiasing == "" ) onOffBiasing = "on";

  // -- Construct the run manager : MT or sequential one
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  G4cout << "      ********** Run Manager constructed in MT mode ************ " << G4endl;
  // -- Choose 4 threads:
  runManager->SetNumberOfThreads(4);
#else
  G4RunManager * runManager = new G4RunManager;
  G4cout << "      ********** Run Manager constructed in sequential mode ************ " << G4endl;
#endif


  // -- Set mandatory initialization classes
  GB02DetectorConstruction* detector = new GB02DetectorConstruction();
  runManager->SetUserInitialization(detector);
  // -- Select a physics list
  FTFP_BERT* physicsList = new FTFP_BERT;
  // -- And augment it with biasing facilities:
  G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
  if ( onOffBiasing == "on" )
    {
      biasingPhysics->Bias("gamma");
      biasingPhysics->Bias("neutron");
      physicsList->RegisterPhysics(biasingPhysics);
      G4cout << "      ********************************************************* " << G4endl;
      G4cout << "      ********** processes are wrapped for biasing ************ " << G4endl;
      G4cout << "      ********************************************************* " << G4endl;
    }
  else
    {
      G4cout << "      ************************************************* " << G4endl;
      G4cout << "      ********** processes are not wrapped ************ " << G4endl;
      G4cout << "      ************************************************* " << G4endl;
    }
  runManager->SetUserInitialization(physicsList);
  // -- Action initialization:
  runManager->SetUserInitialization(new GB02ActionInitialization);

  // Initialize G4 kernel
  runManager->Initialize();
  

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( macro != "" )   // batch mode
    {
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command+macro);
    }
  else
    {  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      //      UImanager->ApplyCommand("/control/execute vis.mac"); 
#endif
      //      if (ui->IsGUI())
      //        UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
#endif
    }


#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
