// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4NURBSbox.hh,v 1.3 1999/05/19 08:33:38 stesting Exp $
// GEANT4 tag $Name: geant4-00-01 $
//
// 
// Olivier Crumeyrolle  12 September 1996

// Box builder prototype
// OC 060996

#include "G4NURBS.hh"

#ifndef __C_G4NURBSbox__

#define __C_G4NURBSbox__ 1 

class	G4NURBSbox : public G4NURBS
	{
        public:
	  G4NURBSbox(G4double DX, G4double DY, G4double DZ);
	  virtual G4Visible&  operator = (const G4Visible& right);
	  virtual G4VVisPrim& operator = (const G4VVisPrim& right);
	  const char*	Whoami() const;
	};

#endif
// end of __C_G4NURBSbox__