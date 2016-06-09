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
// $Id: G4AdjointPosOnPhysVolGenerator.cc,v 1.2 2009/11/18 17:57:59 gcosmo Exp $
// GEANT4 tag $Name: geant4-09-03 $
//
/////////////////////////////////////////////////////////////////////////////
//      Class Name:	G4AdjointCrossSurfChecker
//	Author:       	L. Desorgher
// 	Organisation: 	SpaceIT GmbH
//	Contract:	ESA contract 21435/08/NL/AT
// 	Customer:     	ESA/ESTEC
/////////////////////////////////////////////////////////////////////////////

#include "G4AdjointPosOnPhysVolGenerator.hh"
#include "G4VSolid.hh"
#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"
#include "Randomize.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

G4AdjointPosOnPhysVolGenerator* G4AdjointPosOnPhysVolGenerator::theInstance = 0;

////////////////////////////////////////////////////
//
G4AdjointPosOnPhysVolGenerator* G4AdjointPosOnPhysVolGenerator::GetInstance()
{
  if(theInstance == 0) {
    static G4AdjointPosOnPhysVolGenerator manager;
    theInstance = &manager;
  }
  return theInstance;
}

////////////////////////////////////////////////////
//
G4AdjointPosOnPhysVolGenerator::~G4AdjointPosOnPhysVolGenerator()
{ 
}

////////////////////////////////////////////////////
//
G4AdjointPosOnPhysVolGenerator::G4AdjointPosOnPhysVolGenerator()
{ 
  theSolid=0;
  NStat =1000000;
  epsilon=0.001;
  ModelOfSurfaceSource = "OnSolid"; //OnSolid, ExternalSphere, ExternalBox
  thePhysicalVolume = 0;
  theTransformationFromPhysVolToWorld = G4AffineTransform();
  UseSphere =true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
G4VPhysicalVolume* G4AdjointPosOnPhysVolGenerator::DefinePhysicalVolume(const G4String& aName)
{
  thePhysicalVolume = 0;
  theSolid =0;
  G4PhysicalVolumeStore* thePhysVolStore =G4PhysicalVolumeStore::GetInstance();
  for ( unsigned int i=0; i< thePhysVolStore->size();i++){
  	G4String vol_name =(*thePhysVolStore)[i]->GetName();
	if (vol_name == ""){
		vol_name = (*thePhysVolStore)[i]->GetLogicalVolume()->GetName();
	}
	if (vol_name == aName){
		thePhysicalVolume = (*thePhysVolStore)[i];
	}
  }
  if (thePhysicalVolume){
  	theSolid = thePhysicalVolume->GetLogicalVolume()->GetSolid();
	ComputeTransformationFromPhysVolToWorld();
	/*AreaOfExtSurfaceOfThePhysicalVolume=ComputeAreaOfExtSurface(1.e-3);
	G4cout<<"Monte Carlo  Estimate of the  area of the external surface :"<<AreaOfExtSurfaceOfThePhysicalVolume/m/m<<" m2"<<std::endl;*/
  }
  else {
  	G4cout<<"The physical volume with name "<<aName<<" does not exist!!"<<std::endl;
	G4cout<<"Before generating a source on an external surface of a volume you should select another physical volume"<<std::endl; 
  }
  return thePhysicalVolume;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
void G4AdjointPosOnPhysVolGenerator::DefinePhysicalVolume1(const G4String& aName)
{
   thePhysicalVolume = DefinePhysicalVolume(aName);
}
////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurface()
{
   return ComputeAreaOfExtSurface(theSolid); 
}
////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurface(G4int NStat)
{
   return ComputeAreaOfExtSurface(theSolid,NStat); 
}
////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurface(G4double epsilon)
{
  return ComputeAreaOfExtSurface(theSolid,epsilon); 
}
////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurface(G4VSolid* aSolid)
{
  return ComputeAreaOfExtSurface(aSolid,1.e-3); 
}
////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurface(G4VSolid* aSolid,G4int NStat)
{
  if (ModelOfSurfaceSource == "OnSolid" ){
	if (UseSphere){
		return ComputeAreaOfExtSurfaceStartingFromSphere(aSolid,NStat);
	
	}
	else {
		return ComputeAreaOfExtSurfaceStartingFromBox(aSolid,NStat);
	}
  }
  else {
  	G4ThreeVector p,dir;
	if (ModelOfSurfaceSource == "ExternalSphere" ) return GenerateAPositionOnASphereBoundary(aSolid, p,dir);
  	return GenerateAPositionOnABoxBoundary(aSolid, p,dir);
  }
}
////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurface(G4VSolid* aSolid,G4double epsilon)
{
  G4int Nstat = G4int(1./(epsilon*epsilon));
  return ComputeAreaOfExtSurface(aSolid,Nstat);
}
////////////////////////////////////////////////////
void G4AdjointPosOnPhysVolGenerator::GenerateAPositionOnTheExtSurfaceOfASolid(G4VSolid* aSolid,G4ThreeVector& p, G4ThreeVector& direction)
{
  G4double area;
  area =1.;
  if (ModelOfSurfaceSource == "OnSolid" ){
	return GenerateAPositionOnASolidBoundary(aSolid, p,direction);
  }
  if (ModelOfSurfaceSource == "ExternalSphere" ) {
  	area = GenerateAPositionOnASphereBoundary(aSolid, p, direction);
  	return;
  }	
  	area = GenerateAPositionOnABoxBoundary(aSolid, p, direction);
	return;
}
////////////////////////////////////////////////////
void G4AdjointPosOnPhysVolGenerator::GenerateAPositionOnTheExtSurfaceOfTheSolid(G4ThreeVector& p, G4ThreeVector& direction)
{
  GenerateAPositionOnTheExtSurfaceOfASolid(theSolid,p,direction);
}
////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurfaceStartingFromBox(G4VSolid* aSolid,G4int Nstat)
{
  G4double area=1.;
  G4int i=0;
  G4int j=0;
  while (i<Nstat){
  	G4ThreeVector p, direction;
  	area = GenerateAPositionOnABoxBoundary( aSolid,p, direction);
	G4double dist_to_in = aSolid->DistanceToIn(p,direction);
	if (dist_to_in<kInfinity/2.) i++;
	j++;
 }
 area=area*double(i)/double(j);
 return area;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::ComputeAreaOfExtSurfaceStartingFromSphere(G4VSolid* aSolid,G4int Nstat)
{
  G4double area=1.;
  G4int i=0;
  G4int j=0;
  while (i<Nstat){
  	G4ThreeVector p, direction;
  	area = GenerateAPositionOnASphereBoundary( aSolid,p, direction);
	G4double dist_to_in = aSolid->DistanceToIn(p,direction);
	if (dist_to_in<kInfinity/2.) i++;
	j++;
 }
 area=area*double(i)/double(j);
 
 return area;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
void G4AdjointPosOnPhysVolGenerator::GenerateAPositionOnASolidBoundary(G4VSolid* aSolid,G4ThreeVector& p, G4ThreeVector&  direction)
{ 
  G4bool find_pos =false;
  G4double area=1.;
  while (!find_pos){
  	 if (UseSphere) area = GenerateAPositionOnASphereBoundary( aSolid,p, direction);
  	 else  area = GenerateAPositionOnABoxBoundary( aSolid,p, direction);
	 G4double dist_to_in = aSolid->DistanceToIn(p,direction);
	 if (dist_to_in<kInfinity/2.) {
		find_pos =true;
		G4ThreeVector p1=p+ 0.99999*direction*dist_to_in;
		G4ThreeVector norm =aSolid->SurfaceNormal(p1);
		p+= 0.999999*direction*dist_to_in;
		CosThDirComparedToNormal=direction.dot(-norm);
		//std::cout<<CosThDirComparedToNormal<<std::endl;
		
		return;
	}
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::GenerateAPositionOnASphereBoundary(G4VSolid* aSolid,G4ThreeVector& p, G4ThreeVector&  direction)
{
  G4double minX,maxX,minY,maxY,minZ,maxZ;
  G4bool yesno;

  // values needed for CalculateExtent signature

  G4VoxelLimits limit;                // Unlimited
  G4AffineTransform origin;

  // min max extents of pSolid along X,Y,Z

  yesno = aSolid->CalculateExtent(kXAxis,limit,origin,minX,maxX);
  yesno = aSolid->CalculateExtent(kYAxis,limit,origin,minY,maxY);
  yesno = aSolid->CalculateExtent(kZAxis,limit,origin,minZ,maxZ);

  G4ThreeVector center = G4ThreeVector((minX+maxX)/2.,(minY+maxY)/2.,(minZ+maxZ)/2.);
  
  G4double dX=(maxX-minX)/2.;
  G4double dY=(maxY-minY)/2.;
  G4double dZ=(maxZ-minZ)/2.;
  G4double scale=1.01;
  G4double r=scale*std::sqrt(dX*dX+dY*dY+dZ*dZ);

  G4double cos_th2 = G4UniformRand();
  G4double theta = std::acos(std::sqrt(cos_th2));
  G4double phi=G4UniformRand()*3.1415926*2;
  direction.setRThetaPhi(1.,theta,phi);
  direction=-direction;
  G4double cos_th = (1.-2.*G4UniformRand());
  theta = std::acos(cos_th);
  if (G4UniformRand() <0.5) theta=3.1415926-theta;
  phi=G4UniformRand()*3.1415926*2;
  p.setRThetaPhi(r,theta,phi);
  p+=center;
  direction.rotateY(theta);
  direction.rotateZ(phi);
  return 4.*3.1415926*r*r;;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
G4double G4AdjointPosOnPhysVolGenerator::GenerateAPositionOnABoxBoundary(G4VSolid* aSolid,G4ThreeVector& p, G4ThreeVector&  direction)
{

  G4double ran_var,px,py,pz,minX,maxX,minY,maxY,minZ,maxZ;
  G4bool yesno;
  
  // values needed for CalculateExtent signature

  G4VoxelLimits limit;                // Unlimited
  G4AffineTransform origin;

  // min max extents of pSolid along X,Y,Z

  yesno = aSolid->CalculateExtent(kXAxis,limit,origin,minX,maxX);
  yesno = aSolid->CalculateExtent(kYAxis,limit,origin,minY,maxY);
  yesno = aSolid->CalculateExtent(kZAxis,limit,origin,minZ,maxZ);
  
  G4double scale=.1;
  minX-=scale*std::abs(minX);
  minY-=scale*std::abs(minY);
  minZ-=scale*std::abs(minZ);
  maxX+=scale*std::abs(maxX);
  maxY+=scale*std::abs(maxY);
  maxZ+=scale*std::abs(maxZ);
  
  G4double dX=(maxX-minX);
  G4double dY=(maxY-minY);
  G4double dZ=(maxZ-minZ);

  G4double XY_prob=2.*dX*dY;
  G4double YZ_prob=2.*dY*dZ;
  G4double ZX_prob=2.*dZ*dX;
  G4double area=XY_prob+YZ_prob+ZX_prob;
  XY_prob/=area;
  YZ_prob/=area;
  ZX_prob/=area;
  
  ran_var=G4UniformRand();
  G4double cos_th2 = G4UniformRand();
  G4double sth = std::sqrt(1.-cos_th2);
  G4double cth = std::sqrt(cos_th2);
  G4double phi=G4UniformRand()*3.1415926*2;
  G4double dirX = sth*std::cos(phi);
  G4double dirY = sth*std::sin(phi);
  G4double dirZ = cth;
  if (ran_var <=XY_prob){ //on the XY faces
	G4double ran_var1=ran_var/XY_prob;
	G4double ranX=ran_var1;
	if (ran_var1<=0.5){
		pz=minZ;
		direction=G4ThreeVector(dirX,dirY,dirZ);
		ranX=ran_var1*2.;
	} 
	else{
		pz=maxZ;
		direction=-G4ThreeVector(dirX,dirY,dirZ);
		ranX=(ran_var1-0.5)*2.;
	}
	G4double ranY=G4UniformRand();
	px=minX+(maxX-minX)*ranX;
	py=minY+(maxY-minY)*ranY;
  }
  else if (ran_var <=(XY_prob+YZ_prob)){ //on the YZ faces 
	G4double ran_var1=(ran_var-XY_prob)/YZ_prob;
	G4double ranY=ran_var1;
	if (ran_var1<=0.5){
		px=minX;
		direction=G4ThreeVector(dirZ,dirX,dirY);
		ranY=ran_var1*2.;
	} 
  	else{
		px=maxX;
		direction=-G4ThreeVector(dirZ,dirX,dirY);
		ranY=(ran_var1-0.5)*2.;
  	}
	G4double ranZ=G4UniformRand();
	py=minY+(maxY-minY)*ranY;
	pz=minZ+(maxZ-minZ)*ranZ;
		
  }
  else{ //on the ZX faces 
	G4double ran_var1=(ran_var-XY_prob-YZ_prob)/ZX_prob;
	G4double ranZ=ran_var1;
	if (ran_var1<=0.5){
		py=minY;
		direction=G4ThreeVector(dirY,dirZ,dirX);
		ranZ=ran_var1*2.;
	} 
	else{
		py=maxY;
		direction=-G4ThreeVector(dirY,dirZ,dirX);
		ranZ=(ran_var1-0.5)*2.;
	}
	G4double ranX=G4UniformRand();
	px=minX+(maxX-minX)*ranX;
	pz=minZ+(maxZ-minZ)*ranZ;
  }
  
  p=G4ThreeVector(px,py,pz);
  return area;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
void G4AdjointPosOnPhysVolGenerator::GenerateAPositionOnTheExtSurfaceOfThePhysicalVolume(G4ThreeVector& p, G4ThreeVector&  direction)
{
  if (!thePhysicalVolume) {
  	G4cout<<"Before generating a source on an external surface of volume you should select a physical volume"<<std::endl; 
  	return;
  };
  GenerateAPositionOnTheExtSurfaceOfTheSolid(p,direction);
  p = theTransformationFromPhysVolToWorld.TransformPoint(p);
  direction = theTransformationFromPhysVolToWorld.TransformAxis(direction);
}
/////////////////////////////////////////////////////////////////////////////////////////
//
void G4AdjointPosOnPhysVolGenerator::GenerateAPositionOnTheExtSurfaceOfThePhysicalVolume(G4ThreeVector& p, G4ThreeVector&  direction,
  										G4double& costh_to_normal)
{
  GenerateAPositionOnTheExtSurfaceOfThePhysicalVolume(p, direction);
  costh_to_normal = CosThDirComparedToNormal;
}										
/////////////////////////////////////////////////////////////////////////////////////////
//
void G4AdjointPosOnPhysVolGenerator::ComputeTransformationFromPhysVolToWorld()
{
  G4VPhysicalVolume* daughter =thePhysicalVolume;
  G4LogicalVolume* mother = thePhysicalVolume->GetMotherLogical();
  theTransformationFromPhysVolToWorld = G4AffineTransform();
  G4PhysicalVolumeStore* thePhysVolStore =G4PhysicalVolumeStore::GetInstance();
  while (mother){
 	theTransformationFromPhysVolToWorld *=
	G4AffineTransform(daughter->GetFrameRotation(),daughter->GetObjectTranslation());
 	for ( unsigned int i=0; i< thePhysVolStore->size();i++){
		if ((*thePhysVolStore)[i]->GetLogicalVolume() == mother){
			daughter = (*thePhysVolStore)[i];
			mother =daughter->GetMotherLogical();
			break;
		};		
	}
  }
}
