//$Id: Factory.hpp 10639 2012-07-12 15:21:10Z djcinsb $
//------------------------------------------------------------------------------
//                             Factory
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: Wendy Shoan
// Created: 2003/08/28
//
/**
 *  This class is the base class for the factories.  Derived classes will be
 *  responsible for creating objects of a specific type.
 */
//------------------------------------------------------------------------------
#ifndef Factory_hpp
#define Factory_hpp

#include "gmatdefs.hpp"
#include "GmatBase.hpp"

// Forward references
class Spacecraft;
class SpaceObject;
class Parameter;
class Burn;
class Propagator;
class ODEModel;
class PhysicalModel;
class PropSetup;
class StopCondition;
class CalculatedPoint;
class CelestialBody;
class SolarSystem;
class Solver;
class Subscriber;
class EphemerisFile;
class GmatCommand;
class AtmosphereModel;
class Function;
class Hardware;
class AxisSystem;
class CoordinateSystem;
class MathNode;
class Attitude;
class SpacePoint;
class Event;
class EventLocator;
class Interface;

class MeasurementModel;
class CoreMeasurement;
class DataFile;
class ObType;
class TrackingSystem;
class TrackingData;


/// @todo Find a clever way to allow user types here when we don't know them

class GMAT_API Factory
{
public:
   // method to return objects as generic type
   virtual GmatBase*        CreateObject(const std::string &ofType,
                                         const std::string &withName = "");        

   // methods to return objects of specified types
   virtual SpaceObject*     CreateSpacecraft(const std::string &ofType,
                                             const std::string &withName = "");
   virtual SpacePoint*      CreateSpacePoint(const std::string &ofType,
                                             const std::string &withName = "");        
   virtual Propagator*      CreatePropagator(const std::string &ofType,
                                             const std::string &withName = "");
   virtual ODEModel*        CreateODEModel(const std::string &ofType,
                                             const std::string &withName = "");
   virtual PhysicalModel*   CreatePhysicalModel(const std::string &ofType,
                                                const std::string &withName = "");
   virtual PropSetup*       CreatePropSetup(const std::string &ofType,
                                            const std::string &withName = "");
   virtual Parameter*       CreateParameter(const std::string &ofType,
                                            const std::string &withName = "");
   virtual Burn*            CreateBurn(const std::string &ofType,
                                       const std::string &withName = "");
   virtual StopCondition*   CreateStopCondition(const std::string &ofType,
                                                const std::string &withName = "");
   virtual CalculatedPoint* CreateCalculatedPoint(const std::string &ofType,
                                                  const std::string &withName = "");
   virtual CelestialBody*   CreateCelestialBody(const std::string &ofType,
                                                const std::string &withName = "");
   virtual SolarSystem*     CreateSolarSystem(const std::string &ofType,
                                              const std::string &withName = "");
   virtual Solver*          CreateSolver(const std::string &ofType,
                                         const std::string &withName = ""); 
   virtual Subscriber*      CreateSubscriber(const std::string &ofType,
                                             const std::string &withName = "",
                                             const std::string &fileName = "");
   virtual EphemerisFile*   CreateEphemerisFile(const std::string &ofType,
                                                const std::string &withName = "");
   virtual GmatCommand*     CreateCommand(const std::string &ofType,
                                          const std::string &withName = "");
   virtual AtmosphereModel* CreateAtmosphereModel(const std::string &ofType,
                                                  const std::string &withName = "",
                                                  const std::string &forBody = "Earth");
   virtual Function*        CreateFunction(const std::string &ofType,
                                           const std::string &withName = "");
   virtual Hardware*        CreateHardware(const std::string &ofType,
                                           const std::string &withName = "");
   virtual AxisSystem*      CreateAxisSystem(const std::string &ofType,
                                             const std::string &withName = "");
   virtual CoordinateSystem* CreateCoordinateSystem(const std::string &ofType,
                                                    const std::string &withName = "");
   virtual MathNode*        CreateMathNode(const std::string &ofType,
                                           const std::string &withName = "");
   virtual Attitude*        CreateAttitude(const std::string &ofType,
                                           const std::string &withName = "");
   virtual MeasurementModel*
                            CreateMeasurementModel(const std::string &ofType,
                                           const std::string &withName = "");
   virtual CoreMeasurement* CreateMeasurement(const std::string &ofType,
                                           const std::string &withName = "");
   virtual DataFile*        CreateDataFile(const std::string &ofType,
                                           const std::string &withName = "");
   virtual ObType*          CreateObType(const std::string &ofType,
                                         const std::string &withName = "");
   virtual TrackingSystem*  CreateTrackingSystem(const std::string &ofType,
                                           const std::string &withName = "");
   virtual TrackingData*    CreateTrackingData(const std::string &ofType,
                                           const std::string &withName = "");
   virtual Event*           CreateEvent(const std::string &ofType,
                                        const std::string &withName = "");
   virtual EventLocator*    CreateEventLocator(const std::string &ofType,
                                        const std::string &withName = "");
   virtual Interface*       CreateInterface(const std::string &ofType,
                                            const std::string &withName = "");
   
   // method to return list of types of objects that this factory can create -
   // not const because the qualifier needs to adapt the return list
   virtual StringArray      GetListOfCreatableObjects(
                                  const std::string &qualifier = "");
   // method to check if a createable object type matches a subtype
   virtual bool             DoesObjectTypeMatchSubtype(
                                  const std::string &theType,
                                  const std::string &theSubtype);
   // method to return list of objects that can be viewed via GUI of this factory
   StringArray              GetListOfViewableObjects();
   // method to return list of objects that cannot be viewed via GUI of this factory
   StringArray              GetListOfUnviewableObjects() const;
   // method to set the types of objects that this factory can create
   bool                     SetListOfCreatableObjects(StringArray newList);
   // method to add types of objects that this factory can create
   bool                     AddCreatableObjects(StringArray newList);
   
   // method to return the type of factory this is
   Gmat::ObjectType         GetFactoryType() const;  
   bool                     IsTypeCaseSensitive() const;
   
   // destructor
   virtual ~Factory();

protected:
   // constructor specifying the type of objects creatable by the factory
   Factory(Gmat::ObjectType ofType = Gmat::UNKNOWN_OBJECT);
   // constructor specifying the type of objects creatable by the factory and the
   // specific types that this factory can create
   Factory(StringArray createList, Gmat::ObjectType ofType = Gmat::UNKNOWN_OBJECT);
   // copy constructor
   Factory(const Factory& fact);
   // assignment operator
   Factory& operator= (const Factory& fact);

   // protected data
   // the type of the factory (i.e. what type of objects it can create)
   Gmat::ObjectType         itsType;
   // a list of all of the specific types of objects (of type itsType) that
   // can be created by this factory.
   StringArray              creatables;
   // a list of qualified creatable objects thatcan be created by this factory.
   StringArray              qualifiedCreatables;
   // a list of all of the types of objects that can be viewed from the GUI
   // (This is automacally generated)
   StringArray              viewables;
   // a list of all of the types of objects that cannot be viewed from the GUI
   StringArray              unviewables;
   // is type name case sensitive
   bool                     isCaseSensitive;

private:
    
};

#endif // Factory_hpp
