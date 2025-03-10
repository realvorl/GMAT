//$Id: FuelTank.cpp 11483 2013-03-11 21:12:31Z djcinsb $
//------------------------------------------------------------------------------
//                               FuelTank
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Darrel J. Conway
// Created: 2004/11/08
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under MOMS Task
// Order 124.
//
/**
 * Class implementation for the Fuel Tanks.
 */
//------------------------------------------------------------------------------


#include "FuelTank.hpp"
#include "StringUtil.hpp"          // for GmatStringUtil
#include "HardwareException.hpp"
#include "MessageInterface.hpp"
#include "GmatConstants.hpp"
#include <sstream>


#define ZERO_REFERENCE_TEMPERATURE_THRESHOLD 0.01

//---------------------------------
// static data
//---------------------------------

/// Available pressure models
StringArray FuelTank::pressureModelList;

/// Labels used for the fuel tank parameters.
const std::string
FuelTank::PARAMETER_TEXT[FuelTankParamCount - HardwareParamCount] =
{
   "AllowNegativeFuelMass",
   "FuelMass",
   "Pressure", 
   "Temperature",
   "RefTemperature",
   "Volume",
   "FuelDensity",
   "PressureModel",
   "PressureRegulated",  // deprecated
};

/// Types of the parameters used by fuel tanks.
const Gmat::ParameterType
FuelTank::PARAMETER_TYPE[FuelTankParamCount - HardwareParamCount] =
{
   Gmat::BOOLEAN_TYPE,     // "AllowNegativeFuelMass"
   Gmat::REAL_TYPE,        // "FuelMass",
   Gmat::REAL_TYPE,        // "Pressure", 
   Gmat::REAL_TYPE,        // "Temperature",
   Gmat::REAL_TYPE,        // "RefTemperature",
   Gmat::REAL_TYPE,        // "Volume",
   Gmat::REAL_TYPE,        // "FuelDensity",
   Gmat::ENUMERATION_TYPE, // "PressureModel",
   Gmat::BOOLEAN_TYPE,     // deprecated
};


//------------------------------------------------------------------------------
//  FuelTank()
//------------------------------------------------------------------------------
/**
 * Fuel tank constructor.
 * 
 * @param nomme Name for the tank.
 */
//------------------------------------------------------------------------------
FuelTank::FuelTank(std::string nomme) :
   Hardware             (Gmat::FUEL_TANK, "FuelTank", nomme),
   fuelMass             (756.0),       // 0.6 m^3 of fuel
   pressure             (1500.0),
   temperature          (20.0),
   refTemperature       (20.0),
   volume               (0.75),
   density              (1260.0),      // Hydrazine/H2O2 mixture
   allowNegativeFuelMass(false),
   pressureModel        (TPM_PRESSURE_REGULATED)
{
   objectTypes.push_back(Gmat::FUEL_TANK);
   objectTypeNames.push_back("FuelTank");
   
   parameterCount = FuelTankParamCount;
   blockCommandModeAssignment = false;

   // Available pressure model list
   // Since it is static data, clear it first
   pressureModelList.clear();
   pressureModelList.push_back("PressureRegulated");
   pressureModelList.push_back("BlowDown");
}


//------------------------------------------------------------------------------
//  ~FuelTank()
//------------------------------------------------------------------------------
/**
 * Fuel tank destructor.
 */
//------------------------------------------------------------------------------
FuelTank::~FuelTank()
{
}


//------------------------------------------------------------------------------
//  FuelTank(const FuelTank& ft)
//------------------------------------------------------------------------------
/**
 * Copy constructor.
 * 
 * This method is called by the Clone method to replicate fuel tanks.
 * 
 * @param ft Reference to the tank that gets replicated.
 */
//------------------------------------------------------------------------------
FuelTank::FuelTank(const FuelTank& ft) :
   Hardware             (ft),
   fuelMass             (ft.fuelMass),
   pressure             (ft.pressure),
   temperature          (ft.temperature),
   refTemperature       (ft.refTemperature),
   volume               (ft.volume),
   density              (ft.density),
   allowNegativeFuelMass(ft.allowNegativeFuelMass),
   pressureModel        (ft.pressureModel),
   gasVolume            (ft.gasVolume),
   pvBase               (ft.pvBase)
{
   parameterCount = ft.parameterCount;
   isInitialized  = Initialize();
}


//------------------------------------------------------------------------------
//  FuelTank& operator=(const FuelTank& ft)
//------------------------------------------------------------------------------
/**
 * Assignment operator.
 * 
 * Sets the parameters for one tank equal to anothers.
 * 
 * @param ft Reference to the tank that gets replicated.
 */
//------------------------------------------------------------------------------
FuelTank& FuelTank::operator=(const FuelTank& ft)
{
   #ifdef DEBUG_TANK_SETTING
      MessageInterface::ShowMessage("Calling assignment operator on %s\n",
            instanceName.c_str());
   #endif
   if (&ft != this)
   {
      Hardware::operator=(ft);
   
      fuelMass              = ft.fuelMass;
      pressure              = ft.pressure;
      temperature           = ft.temperature;
      refTemperature        = ft.refTemperature;
      volume                = ft.volume;
      density               = ft.density;
      allowNegativeFuelMass = ft.allowNegativeFuelMass;
      pressureModel         = ft.pressureModel;
      gasVolume             = ft.gasVolume;
      pvBase                = ft.pvBase;

      isInitialized  = Initialize();
   }
   
   return *this;
}


//------------------------------------------------------------------------------
//  std::string  GetParameterText(const Integer id) const
//------------------------------------------------------------------------------
/**
 * This method returns the parameter text, given the input parameter ID.
 *
 * @param <id> Id for the requested parameter text.
 *
 * @return parameter text for the requested parameter.
 *
 */
//------------------------------------------------------------------------------
std::string FuelTank::GetParameterText(const Integer id) const
{
   if (id >= HardwareParamCount && id < FuelTankParamCount)
      return PARAMETER_TEXT[id - HardwareParamCount];
   return Hardware::GetParameterText(id);
}


//------------------------------------------------------------------------------
//  std::string  GetParameterUnit(const Integer id) const
//------------------------------------------------------------------------------
/**
 * @see GmatBase
 */
//------------------------------------------------------------------------------
std::string FuelTank::GetParameterUnit(const Integer id) const
{
   switch (id)
   {
   case FUEL_MASS:
      return "kg";
   case PRESSURE:
      return "kPa";
   case TEMPERATURE:
   case REFERENCE_TEMPERATURE:
      return "C";
   case VOLUME:
      return "m^3";
   case FUEL_DENSITY:
      return "kg/m^3";
   default:
      return Hardware::GetParameterUnit(id);
   }
}


//------------------------------------------------------------------------------
//  Integer  GetParameterID(const std::string &str) const
//------------------------------------------------------------------------------
/**
 * This method returns the parameter ID, given the input parameter string.
 *
 * @param <str> string for the requested parameter.
 *
 * @return ID for the requested parameter.
 *
 */
//------------------------------------------------------------------------------
Integer FuelTank::GetParameterID(const std::string &str) const
{
   for (Integer i = HardwareParamCount; i < FuelTankParamCount; i++)
   {
      if (str == PARAMETER_TEXT[i - HardwareParamCount])
         return i;
   }
   
   return Hardware::GetParameterID(str);
}


//------------------------------------------------------------------------------
//  Gmat::ParameterType  GetParameterType(const Integer id) const
//------------------------------------------------------------------------------
/**
 * This method returns the parameter type, given the input parameter ID.
 *
 * @param <id> ID for the requested parameter.
 *
 * @return parameter type of the requested parameter.
 *
 */
//------------------------------------------------------------------------------
Gmat::ParameterType FuelTank::GetParameterType(const Integer id) const
{
   if (id >= HardwareParamCount && id < FuelTankParamCount)
      return PARAMETER_TYPE[id - HardwareParamCount];
      
   return Hardware::GetParameterType(id);
}


//---------------------------------------------------------------------------
//  std::string GetParameterTypeString(const Integer id) const
//---------------------------------------------------------------------------
/**
 * Retrieve the string associated with a parameter.
 *
 * @param <id> The integer ID for the parameter.
 *
 * @return Text description for the type of the parameter, or the empty
 *         string ("").
 */
//---------------------------------------------------------------------------
std::string FuelTank::GetParameterTypeString(const Integer id) const
{
   return GmatBase::PARAM_TYPE_STRING[GetParameterType(id)];
}


//---------------------------------------------------------------------------
//  bool IsParameterReadOnly(const Integer id) const
//---------------------------------------------------------------------------
/**
 * Checks to see if the requested parameter is read only.
 *
 * @param <id> Description for the parameter.
 *
 * @return true if the parameter is read only, false (the default) if not,
 *         throws if the parameter is out of the valid range of values.
 */
//---------------------------------------------------------------------------
bool FuelTank::IsParameterReadOnly(const Integer id) const
{
   if ((id == DIRECTION_X) || (id == DIRECTION_Y) || (id == DIRECTION_Z))
      return true;
   
   if (id == PRESSURE_REGULATED)
      return true;
   
   return Hardware::IsParameterReadOnly(id);
}


//------------------------------------------------------------------------------
// bool IsParameterCommandModeSettable(const Integer id) const
//------------------------------------------------------------------------------
/**
 * Tests to see if an object property can be set in Command mode
 *
 * @param id The ID of the object property
 *
 * @return true if the property can be set in command mode, false if not.
 */
//------------------------------------------------------------------------------
bool FuelTank::IsParameterCommandModeSettable(const Integer id) const
{
   if ((id == ALLOW_NEGATIVE_FUEL_MASS) ||
       (id == PRESSURE_MODEL) ||
       (id == PRESSURE_REGULATED))
      return false;

   // Activate all of the other tank specific IDs
   if (id >= HardwareParamCount)
      return true;

   return Hardware::IsParameterCommandModeSettable(id);
}


//------------------------------------------------------------------------------
//  Real GetRealParameter(const Integer id) const
//------------------------------------------------------------------------------
/**
 * Retrieve the value for a Real parameter.
 *
 * @param <id> The integer ID for the parameter.
 *
 * @return The parameter's value.
 */
//------------------------------------------------------------------------------
Real FuelTank::GetRealParameter(const Integer id) const
{
   switch (id)
   {
      case FUEL_MASS:
         return fuelMass;
         
      case PRESSURE:
         return pressure;
         
      case TEMPERATURE:
         return temperature;
         
      case REFERENCE_TEMPERATURE:
         return refTemperature;
         
      case VOLUME:
         return volume;
         
      case FUEL_DENSITY:
         return density;
         
      default:
         break;   // Default just drops through
   }
   return Hardware::GetRealParameter(id);
}


//------------------------------------------------------------------------------
//  Real SetRealParameter(const Integer id, const Real value)
//------------------------------------------------------------------------------
/**
 * Set the value for a Real parameter.
 * 
 * This code checks the validity of selected tank parameters; specifically, the
 * fuel mass, density, and tank pressure and volume must all be non-negative 
 * numbers.
 *
 * @param id The integer ID for the parameter.
 * @param value The new parameter value.
 *
 * @return the parameter value at the end of this call, or throw an exception
 *         if the parameter id is invalid or the parameter type is not Real.
 */
//------------------------------------------------------------------------------
Real FuelTank::SetRealParameter(const Integer id, const Real value)
{
   #ifdef DEBUG_FUELTANK_SET
   MessageInterface::ShowMessage
      ("FuelTank::SetRealParameter(), id=%d, value=%f\n", id, value);
   #endif
   
   switch (id)
   {
      case FUEL_MASS:
         {
            #ifdef DEBUG_TANK_SETTING
               MessageInterface::ShowMessage("Updating fuel mass to %lf\n",
                     value);
            #endif
            if (value >= 0.0 || allowNegativeFuelMass)
            {
               fuelMass = value;
               UpdateTank();
               //isInitialized = false;
               return fuelMass;
            }
            else
            {
               HardwareException hwe("");
               hwe.SetDetails(errorMessageFormat.c_str(),
                              GmatStringUtil::ToString(value, 16).c_str(),
                              "FuelMass", "Real Number >= 0.0");
               throw hwe;
            }
         }
         
      case PRESSURE:
         
         if (value >= 0.0)
            pressure = value;
         else
         {
            HardwareException hwe("");
            hwe.SetDetails(errorMessageFormat.c_str(),
                           GmatStringUtil::ToString(value, 16).c_str(),
                           "Pressure", "Real Number >= 0.0");
            throw hwe;
         }
         isInitialized = false;
         return pressure;
         
      case TEMPERATURE:
         if (value > GmatPhysicalConstants::ABSOLUTE_ZERO_C)
            temperature = value;
         else
         {
            HardwareException hwe("");
            std::stringstream ss("");
            ss << "Real Number > " << GmatPhysicalConstants::ABSOLUTE_ZERO_C;
            hwe.SetDetails(errorMessageFormat.c_str(),
                           GmatStringUtil::ToString(value, 16).c_str(),
                           "Temperature", (ss.str()).c_str());
            throw hwe;
         }
         isInitialized = false;
         return temperature;
         
      case REFERENCE_TEMPERATURE:
         if (value > GmatPhysicalConstants::ABSOLUTE_ZERO_C)
         {
            if (GmatMathUtil::Abs(value) <= ZERO_REFERENCE_TEMPERATURE_THRESHOLD)
            {
               HardwareException hwe("");
               std::stringstream ss("");
               ss << "Real Number > " << ZERO_REFERENCE_TEMPERATURE_THRESHOLD
                  << " or Real Number < -"
                  << ZERO_REFERENCE_TEMPERATURE_THRESHOLD;
               hwe.SetDetails(errorMessageFormat.c_str(),
                              GmatStringUtil::ToString(value, 16).c_str(),
                              "RefTemperature", (ss.str()).c_str());
               throw hwe;
            }
            refTemperature = value;
         }
         else
         {
            HardwareException hwe("");
            std::stringstream ss("");
            ss << "Real Number > " << GmatPhysicalConstants::ABSOLUTE_ZERO_C;
            hwe.SetDetails(errorMessageFormat.c_str(),
                           GmatStringUtil::ToString(value, 16).c_str(),
                           "RefTemperature", (ss.str()).c_str());
            throw hwe;
         }
         isInitialized = false;
         return refTemperature;
         
      case VOLUME:
         
         if (value >= 0.0)
            volume = value;
         else
         {
            HardwareException hwe("");
            hwe.SetDetails(errorMessageFormat.c_str(),
                           GmatStringUtil::ToString(value, 16).c_str(),
                           "Volume", "Real Number >= 0.0");
            throw hwe;
         }
         isInitialized = false;
         return volume;

      case FUEL_DENSITY:
         
         if (value > 0.0)
            density = value;
         else
         {
            HardwareException hwe("");
            hwe.SetDetails(errorMessageFormat.c_str(),
                           GmatStringUtil::ToString(value, 16).c_str(),
                           "FuelDensity", "Real Number > 0.0");
            throw hwe;
         }
         isInitialized = false;
         return density;

      default:
         break;   // Default just drops through
   }
   
   return Hardware::SetRealParameter(id, value);
}


//---------------------------------------------------------------------------
// Real GetRealParameter(const std::string &label) const
//---------------------------------------------------------------------------
Real FuelTank::GetRealParameter(const std::string &label) const
{
   return GetRealParameter(GetParameterID(label));
}


//---------------------------------------------------------------------------
// Real SetRealParameter(const std::string &label, const Real value)
//---------------------------------------------------------------------------
Real FuelTank::SetRealParameter(const std::string &label, const Real value)
{
   return SetRealParameter(GetParameterID(label), value);
}


//---------------------------------------------------------------------------
//  bool GetBooleanParameter(const Integer id) const
//---------------------------------------------------------------------------
/**
 * Retrieve a boolean parameter.
 *
 * @param <id> The integer ID for the parameter.
 *
 * @return the boolean value for this parameter, or throw an exception if the
 *         parameter access in invalid.
 */
//------------------------------------------------------------------------------
bool FuelTank::GetBooleanParameter(const Integer id) const
{
   if (id == PRESSURE_REGULATED)
   {
      MessageInterface::ShowMessage
         ("*** WARNING *** \"PressureRegulated\" is deprecated and will be "
          "removed from a future build; please use \"PressureModel\" "
          "instead.\n");      
      return true;
   }
   else if (id == ALLOW_NEGATIVE_FUEL_MASS)
   {
      return allowNegativeFuelMass;
   }
   
   return Hardware::GetBooleanParameter(id);
}


//---------------------------------------------------------------------------
//  bool SetBooleanParameter(const Integer id, const bool value)
//---------------------------------------------------------------------------
/**
 * Sets the value for a boolean parameter.
 *
 * @param id The integer ID for the parameter.
 * @param value The new value.
 * 
 * @return the boolean value for this parameter, or throw an exception if the 
 *         parameter is invalid or not boolean.
 */
//------------------------------------------------------------------------------
bool FuelTank::SetBooleanParameter(const Integer id, const bool value)
{
   if (id == PRESSURE_REGULATED)
   {
      if (value)
         pressureModel = TPM_PRESSURE_REGULATED;
      else
         pressureModel = TPM_BLOW_DOWN;
      
      MessageInterface::ShowMessage
         ("*** WARNING *** \"PressureRegulated\" is deprecated and will be "
          "removed from a future build; please use \"PressureModel\" "
          "instead.\n");      
      return true;
   }
   else if (id == ALLOW_NEGATIVE_FUEL_MASS)
   {
      allowNegativeFuelMass = value;
      return true;
   }
   
   return Hardware::SetBooleanParameter(id, value);
}


//---------------------------------------------------------------------------
// std::string GetStringParameter(const Integer id) const
//---------------------------------------------------------------------------
std::string FuelTank::GetStringParameter(const Integer id) const
{
   if (id == PRESSURE_MODEL)
      return pressureModelList[pressureModel];
   
   return Hardware::GetStringParameter(id);
}


//---------------------------------------------------------------------------
// bool SetStringParameter(const Integer id, const std::string &value)
//---------------------------------------------------------------------------
bool FuelTank::SetStringParameter(const Integer id, const std::string &value)
{
   #ifdef DEBUG_FUELTANK_SET
   MessageInterface::ShowMessage
      ("FuelTank::SetStringParameter() entered, id=%d, value='%s'\n", id,
       value.c_str());
   #endif
   
   if (id == PRESSURE_MODEL)
   {
      if (find(pressureModelList.begin(), pressureModelList.end(), value) !=
          pressureModelList.end())
      {
         for (UnsignedInt i=0; i<pressureModelList.size(); i++)
            if (value == pressureModelList[i])
               pressureModel = i;
      }
      else
      {
         // write one warning per GMAT session
         static bool firstTimeWarning = true;
         std::string framelist = pressureModelList[0];
         for (UnsignedInt n = 1; n < pressureModelList.size(); ++n)
            framelist += ", " + pressureModelList[n];
         
         std::string msg =
            "The value of \"" + value + "\" for field \"PressureModel\""
            " on object \"" + instanceName + "\" is not an allowed value.\n"
            "The allowed values are: [ " + framelist + " ]. ";
         
         if (firstTimeWarning)
         {
            firstTimeWarning = false;
            throw HardwareException(msg);
         }
      }
      
      return true;
   }
   
   return Hardware::SetStringParameter(id, value);
}


//---------------------------------------------------------------------------
// std::string GetStringParameter(const std::string &label) const
//---------------------------------------------------------------------------
std::string FuelTank::GetStringParameter(const std::string &label) const
{
   return GetStringParameter(GetParameterID(label));
}


//---------------------------------------------------------------------------
// bool SetStringParameter(const std::string &label, const std::string &value)
//---------------------------------------------------------------------------
bool FuelTank::SetStringParameter(const std::string &label,
                                  const std::string &value)
{
   return SetStringParameter(GetParameterID(label), value);
}


//---------------------------------------------------------------------------
// const StringArray& GetPropertyEnumStrings(const Integer id) const
//---------------------------------------------------------------------------
const StringArray& FuelTank::GetPropertyEnumStrings(const Integer id) const
{
   if (id == PRESSURE_MODEL)
      return pressureModelList;
   
   return Hardware::GetPropertyEnumStrings(id);
}


//---------------------------------------------------------------------------
// const StringArray& GetPropertyEnumStrings(const std::string &label) const
//---------------------------------------------------------------------------
const StringArray& FuelTank::GetPropertyEnumStrings(const std::string &label) const
{
   return GetPropertyEnumStrings(GetParameterID(label));
}


//---------------------------------------------------------------------------
//  GmatBase* Clone() const
//---------------------------------------------------------------------------
/**
 * Provides a clone of this object by calling the copy constructor.
 *
 * @return A GmatBase pointer to the cloned fuel tank.
 */
//---------------------------------------------------------------------------
GmatBase* FuelTank::Clone() const
{
   return new FuelTank(*this);
}


//---------------------------------------------------------------------------
//  void Copy(const GmatBase* orig)
//---------------------------------------------------------------------------
/**
 * Sets this object to match another one.
 * 
 * @param orig The original that is being copied.
 */
//---------------------------------------------------------------------------
void FuelTank::Copy(const GmatBase* orig)
{
   operator=(*((FuelTank *)(orig)));
}


//------------------------------------------------------------------------------
//  bool Initialize()
//------------------------------------------------------------------------------
/**
 * Fills in the data needed for internal calculations of fuel depletion.
 */
//------------------------------------------------------------------------------
bool FuelTank::Initialize()
{
   isInitialized = false;

   if (Validate())
   {
      gasVolume = volume - fuelMass / density;
      pvBase = pressure * gasVolume;
      isInitialized = true;
   }
   
   return isInitialized;
}


// Protected methods

//------------------------------------------------------------------------------
//  void UpdateTank()
//------------------------------------------------------------------------------
/**
 * Updates pressure and volume data using the ideal gas law.
 *
 * GMAT fuel tanks can operate in a pressure-regulated mode (constant pressure,
 * constant temperature) or in a blow-down mode.  When the tank runs in 
 * blow-down mode, the pressure is calculated using the ideal gas law:
 * 
 *    \f[PV=nRT\f]
 * 
 * The right side of this equation is held constant.  Given an initial pressure
 * \f$P_i\f$ and an initial volume \f$V_i\f$, the new pressure is given by
 * 
 *    \f[P_f= \frac{P_i V_i}{V_f}\f]
 * 
 * The pressurant volume \f$V_G\f$ is calculated from the tank volume 
 * \f$V_T\f$, the fuel mass \f$M_F\f$, and the fuel density \f$\rho\f$ using
 * 
 *    \f[V_{G}=V_{T}-\frac{M_{F}}{\rho}\f]
 * 
 * Mass is depleted from the tank by integrating the mass flow over time, as
 * is described in the Thruster documentation.
 */
//------------------------------------------------------------------------------
void FuelTank::UpdateTank()
{
   if (pressureModel != TPM_PRESSURE_REGULATED)
   {
      #ifdef DEBUG_MASS_FLOW
         MessageInterface::ShowMessage("Vol = %.12lf, fuelMass = %.12lf, "
               "density = %.12lf, P0 = %.12lf, PV = %.12lf", volume, fuelMass,
               density, pressure, pvBase);
      #endif
      if (!isInitialized)
         Initialize();

      gasVolume = volume - fuelMass / density;

//      if (gasVolume < 0.0)
//         throw HardwareException("*** ERROR *** The fuel volume in the "
//               "tank \"" + instanceName + "\" exceeds the volume of the tank.");

      pressure = pvBase / gasVolume;

      #ifdef DEBUG_MASS_FLOW
         MessageInterface::ShowMessage("Gas Vol = %.12lf, Pf = %.12lf\n",
               gasVolume, pvBase);
      #endif
   }
}


//------------------------------------------------------------------------------
//  void DepleteFuel(Real dm)
//------------------------------------------------------------------------------
/**
 * Depletes fuel from the tank and updates the tank data as needed.
 * 
 * @todo Fill in the mathematics for this method.
 */
//------------------------------------------------------------------------------
void FuelTank::DepleteFuel(Real dm)
{
   fuelMass -= dm;
   
   if (fuelMass < 0.0)
      // For now, throw if the fuel goes below 0
      throw HardwareException("Fuel in tank " + instanceName + 
                              " completely exhausted.\n");
}

bool FuelTank::Validate()
{
   if (density <= 0.0)
	  return false;
   if ((volume - fuelMass / density) < 0.0)
	   throw HardwareException("Fuel volume exceeds tank capacity\n");
   return true;
}

