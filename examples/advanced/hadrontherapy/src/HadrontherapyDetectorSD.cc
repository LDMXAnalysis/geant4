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
// Hadrontherapy advanced example for Geant4
// See more at: https://twiki.cern.ch/twiki/bin/view/Geant4/AdvancedExamplesHadrontherapy

#include "HadrontherapyDetectorSD.hh"
#include "HadrontherapyDetectorHit.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "HadrontherapyMatrix.hh"
#include "HadrontherapyLet.hh"
#include "G4Track.hh"
#include "HadrontherapyAnalysisManager.hh"
#include "G4SystemOfUnits.hh"

/////////////////////////////////////////////////////////////////////////////
HadrontherapyDetectorSD::HadrontherapyDetectorSD(G4String name):
    G4VSensitiveDetector(name)
{ 
    G4String HCname;
    collectionName.insert(HCname="HadrontherapyDetectorHitsCollection");
    HitsCollection = NULL; 
    sensitiveDetectorName = name;

}

/////////////////////////////////////////////////////////////////////////////
HadrontherapyDetectorSD::~HadrontherapyDetectorSD()
<<<<<<< HEAD
{ 
}
=======
{}
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c

/////////////////////////////////////////////////////////////////////////////
void HadrontherapyDetectorSD::Initialize(G4HCofThisEvent*)
{
  
    HitsCollection = new HadrontherapyDetectorHitsCollection(sensitiveDetectorName,
	    collectionName[0]);
}

/////////////////////////////////////////////////////////////////////////////
G4bool HadrontherapyDetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{

 
  if (aStep -> GetPreStepPoint() -> GetPhysicalVolume() -> GetName() != "RODetectorZDivisionPhys") return false;
	

    // Get kinetic energy
    G4Track * theTrack = aStep  ->  GetTrack();
<<<<<<< HEAD
    G4double kineticEnergy =  theTrack -> GetKineticEnergy();  

    G4ParticleDefinition *particleDef = theTrack -> GetDefinition();
    //Get particle name  
    G4String particleName =  particleDef -> GetParticleName();  

=======
    
    G4ParticleDefinition *particleDef = theTrack -> GetDefinition();
    //Get particle name
    G4String particleName =  particleDef -> GetParticleName();
    
    // Get particle PDG code
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    G4int pdg = particleDef ->GetPDGEncoding();

    // Get unique track_id (in an event)
    G4int trackID = theTrack -> GetTrackID();

    G4double energyDeposit = aStep -> GetTotalEnergyDeposit();

    G4double DX = aStep -> GetStepLength();
    G4int Z = particleDef-> GetAtomicNumber();
    G4int A = particleDef-> GetAtomicMass();
<<<<<<< HEAD

  
 // Read voxel indexes: i is the x index, k is the z index
 const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
 G4int k  = touchable->GetReplicaNumber(0);
 G4int i  = touchable->GetReplicaNumber(2);
 G4int j  = touchable->GetReplicaNumber(1);

=======
    G4StepPoint* PreStep = aStep->GetPreStepPoint();
    
    // Read voxel indexes: i is the x index, k is the z index
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int k  = touchable->GetReplicaNumber(0);
    G4int i  = touchable->GetReplicaNumber(2);
    G4int j  = touchable->GetReplicaNumber(1);
    
    G4TouchableHandle touchPreStep = PreStep->GetTouchableHandle();
    G4VPhysicalVolume* volumePre = touchPreStep->GetVolume();
    G4String namePre = volumePre->GetName();
    
    
    
    
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    

#ifdef G4ANALYSIS_USE_ROOT
    HadrontherapyAnalysisManager* analysis = HadrontherapyAnalysisManager::GetInstance();
#endif

    HadrontherapyMatrix* matrix = HadrontherapyMatrix::GetInstance();
    HadrontherapyLet* let = HadrontherapyLet::GetInstance();
    
<<<<<<< HEAD
=======
    G4int* hitTrack = matrix -> GetHitTrack(i,j,k);
    
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    
  //  ******************** let ***************************
     if (let)
    {
     if ( !(Z==0 && A==1) ) // All but not neutrons 
    {
<<<<<<< HEAD
	  if( energyDeposit>0. && DX >0. )
	    {
	      if (pdg !=22) // not gamma
		{
		  let -> FillEnergySpectrum(trackID, particleDef,energyDeposit, DX, i, j, k);
		}
	      else if (kineticEnergy > 50.*keV) // gamma cut
		{
		    let -> FillEnergySpectrum(trackID, particleDef,energyDeposit, DX, i , j, k);
		}

	    }
	}
=======
        if ( !(Z==0 && A==1) ) // All but not neutrons
        {
            if( energyDeposit>0. && DX >0. )// calculate only energy deposit
            {
                if (pdg !=22 && pdg !=11) // not gamma and electrons
                {
                    
                    // Get the pre-step kinetic energy
                    G4double eKinPre = aStep -> GetPreStepPoint() -> GetKineticEnergy();
                    // Get the post-step kinetic energy
                    G4double eKinPost = aStep -> GetPostStepPoint() -> GetKineticEnergy();
                    // Get the step average kinetic energy
                    G4double eKinMean = (eKinPre + eKinPost) * 0.5;
                    
                    // get the material
                    G4Material * materialStep = aStep -> GetPreStepPoint() -> GetMaterial();
                    
                    // get the secondary paticles
                    G4Step fstep = *theTrack -> GetStep();
                    // store all the secondary partilce in current step
                    const std::vector<const G4Track*> * secondary = fstep.GetSecondaryInCurrentStep();
                    
                    size_t SecondarySize = (*secondary).size();
                    G4double EnergySecondary = 0.;
                    
                    // get secondary electrons energy deposited
                    if (SecondarySize) // calculate only secondary particles
                    {
                        for (size_t numsec = 0; numsec< SecondarySize ; numsec ++)
                        {
                            //Get the PDG code of every secondaty particles in current step
                            G4int PDGSecondary=(*secondary)[numsec]->GetDefinition()->GetPDGEncoding();
                            
                            if(PDGSecondary == 11) // calculate only secondary electrons
                            {
                                // calculate the energy deposit of secondary electrons in current step
                                EnergySecondary += (*secondary)[numsec]->GetKineticEnergy();
                            }
                        }
                        
                    }
                    
                    // call the let filldatas function to calculate let
                    let -> FillEnergySpectrum(trackID, particleDef, eKinMean, materialStep,
                                              energyDeposit,EnergySecondary,DX, i, j, k);
                }
            }
        }
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    }
    
    
    
    //  ******************** let ***************************
  
    
    if (matrix)
    {
<<<<<<< HEAD

	// Increment Fluences & accumulate energy spectra
	// Hit voxels are marked with track_id throught hitTrack matrix
	G4int* hitTrack = matrix -> GetHitTrack(i,j,k); // hitTrack MUST BE cleared at every eventAction!
	if ( *hitTrack != trackID )
	{
                 *hitTrack = trackID;
		/*
		 * Fill FLUENCE data for every single nuclide 
		 * Exclude e-, neutrons, gamma, ...
		 */
		if ( Z >= 1) matrix -> Fill(trackID, particleDef, i, j, k, 0, true);
		    

#ifdef G4ANALYSIS_USE_ROOT
/*
	    // Fragments kinetic energy (ntuple)
	    if (trackID !=1 && Z>=1) 
	    {
		// First step kinetic energy for every fragment 
		 analysis -> FillKineticFragmentTuple(i, j, k, A, Z, kineticEnergy/MeV);
	    }	 
	    // Kinetic energy spectra for primary particles 

	    if ( trackID == 1 && i == 0) 
	    {
		// First step kinetic energy for primaries only
		analysis -> FillKineticEnergyPrimaryNTuple(i, j, k, kineticEnergy/MeV);
	    }
*/
#endif
	}	 

	if(energyDeposit != 0)
	{
/*
 *  This method will fill a dose matrix for every single nuclide. 
 *  A method of the HadrontherapyMatrix class (StoreDoseFluenceAscii())
 *  is called automatically at the end of main (or via the macro command /analysis/writeDoseFile.
 *  It permits to store all dose/fluence data into a single plane ASCII file. 
*/	    
	    // if (A==1 && Z==1) // primary and sec. protons 
	    if ( Z>=1 )    //  exclude e-, neutrons, gamma, ...
		    matrix -> Fill(trackID, particleDef, i, j, k, energyDeposit);
	    /*
	     * Create a hit with the information of position is in the detector     
	     */
	    HadrontherapyDetectorHit* detectorHit = new HadrontherapyDetectorHit();       
	    detectorHit -> SetEdepAndPosition(i, j, k, energyDeposit); 
	    HitsCollection -> insert(detectorHit);
	}
    }

#ifdef G4ANALYSIS_USE_ROOT
    if(energyDeposit != 0)
    {  
	if(trackID != 1)
	{
	    if (particleName == "proton")
		analysis -> SecondaryProtonEnergyDeposit(i, energyDeposit/MeV);

	    else if (particleName == "neutron")
		analysis -> SecondaryNeutronEnergyDeposit(i, energyDeposit/MeV);

	    else if (particleName == "alpha")
		analysis -> SecondaryAlphaEnergyDeposit(i, energyDeposit/MeV);

	    else if (particleName == "gamma")
		analysis -> SecondaryGammaEnergyDeposit(i, energyDeposit/MeV);

	    else if (particleName == "e-")
		analysis -> SecondaryElectronEnergyDeposit(i, energyDeposit/MeV);

	    else if (particleName == "triton")
		analysis -> SecondaryTritonEnergyDeposit(i, energyDeposit/MeV);

	    else if (particleName == "deuteron")
		analysis -> SecondaryDeuteronEnergyDeposit(i, energyDeposit/MeV);

	    else if (particleName == "pi+" || particleName == "pi-" ||  particleName == "pi0")
		analysis -> SecondaryPionEnergyDeposit(i, energyDeposit/MeV);   	
	}
=======
        
        // Increment Fluences & accumulate energy spectra
        // Hit voxels are marked with track_id throught hitTrack matrix
        //G4int* hitTrack = matrix -> GetHitTrack(i,j,k); // hitTrack MUST BE cleared at every eventAction!
        if ( *hitTrack != trackID )
        {
            *hitTrack = trackID;
            /*
             * Fill FLUENCE data for every single nuclide
             * Exclude e-, neutrons, gamma, ...
             */
            if ( Z >= 1) matrix -> Fill(trackID, particleDef, i, j, k, 0, true);
            
        }
        
        if(energyDeposit != 0)
        {
            /*
             *  This method will fill a dose matrix for every single nuclide.
             *  A method of the HadrontherapyMatrix class (StoreDoseFluenceAscii())
             *  is called automatically at the end of main (or via the macro command /analysis/writeDoseFile.
             *  It permits to store all dose/fluence data into a single plane ASCII file.
             */
            // if (A==1 && Z==1) // primary and sec. protons
            if ( Z>=1 )    //  exclude e-, neutrons, gamma, ...
                matrix -> Fill(trackID, particleDef, i, j, k, energyDeposit);
            /*
             * Create a hit with the information of position is in the detector
             */
            HadrontherapyDetectorHit* detectorHit = new HadrontherapyDetectorHit();
            detectorHit -> SetEdepAndPosition(i, j, k, energyDeposit);
            HitsCollection -> insert(detectorHit);
        }
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    }
#endif

    return true;
}

/////////////////////////////////////////////////////////////////////////////
void HadrontherapyDetectorSD::EndOfEvent(G4HCofThisEvent* HCE)
{
 
    static G4int HCID = -1;
    if(HCID < 0)
<<<<<<< HEAD
    { 
	HCID = GetCollectionID(0); 
=======
    {
        HCID = GetCollectionID(0);
>>>>>>> 5baee230e93612916bcea11ebf822756cfa7282c
    }

    HCE -> AddHitsCollection(HCID,HitsCollection);
}

