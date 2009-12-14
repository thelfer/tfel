/*!
 * \file   UnitSamples.hxx
 * \brief  This file declares usual units.
 * \author Helfer Thomas
 * \date   07 Jun 2006
 */

#ifndef _LIB_TFEL_UNITSAMPLES_H_
#define _LIB_TFEL_UNITSAMPLES_H_ 

#include"Math/Quantity/Unit.hxx"

namespace tfel{

  namespace math{

    // Fondamental units

    /*!
     * \brief NoUnit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,0,0,0,0,0>::type  NoUnit;
    /*!
     * \brief Declares the Mass unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,0,0,0,0,0,0>::type  Mass;
    /*!
     * \brief Declares the Length unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,1,0,0,0,0,0>::type  Length;
    /*!
     * \brief Declares the Time unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,1,0,0,0,0>::type  Time;
    /*!
     * \brief Declares the Ampere unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,0,1,0,0,0>::type  Ampere;
    /*!
     * \brief Declares the Temperature unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,0,0,1,0,0>::type  Temperature;
    /*!
     * \brief Declares the Kelvin unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,0,0,1,0,0>::type  Kelvin;
    /*!
     * \brief Declares the Candela unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,0,0,0,1,0>::type  Candela;
    /*!
     * \brief Declares the Mole unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,0,0,0,0,1>::type  Mole;
    
    // Additional units

    /*!
     * \brief Declares the InvLength unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,-1,0,0,0,0,0>::type  InvLength; // m-1

    /*!
     * \brief Declares the InvTemperature unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,0,0,-1,0,0>::type  InvTemperature;

    /*!
     * \brief Declares the Frequency unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,0,-1,0,0,0,0>::type  Frequency;    // s-1

    /*!
     * \brief Declares the Velocity unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,1,-1,0,0,0,0>::type  Velocity;     // m.s-1

    /*!
     * \brief Declares the Acceleration unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<0,1,-2,0,0,0,0>::type  Acceleration; // m.s-2

    /*!
     * \brief Declares the Momentum unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,1,-1,0,0,0,0>::type  Momentum;     // kg.m.s-1

    /*!
     * \brief Declares the Momentum unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,1,-2,0,0,0,0>::type  Force;        // kg.m.s-2

    /*!
     * \brief Declares the Newton unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,1,-2,0,0,0,0>::type  Newton;       // kg.m.s-2

    /*!
     * \brief Declares the Stress unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,-1,-2,0,0,0,0>::type Stress;        // kg.m-1.s-2

    /*!
     * \brief Declares the StressRate unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,-1,-3,0,0,0,0>::type StressRate;     // kg.m-1.s-3


    /*!
     * \brief Declares the Pressure unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,-1,-2,0,0,0,0>::type Pressure;      // kg.m-1.s-2

    /*!
     * \brief Declares the Energy unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,2,-2,0,0,0,0>::type  Energy;         // kg.m2.s-2

    /*!
     * \brief Declares the EnergyDensity unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,-1,-2,0,0,0,0>::type EnergyDensity;  // kg.m-1.s-2
    /*!
     * \brief Declares the Density unit 
     * \see GenerateUnit
     */
    typedef GenerateUnit<1,-3,0,0,0,0,0>::type  Density; // k.m-3

    /*!
     * \brief Declares the ThermalConductivity unit
     */
    typedef GenerateUnit<1,1,-3,0,-1,0,0>::type ThermalConductivity;  // kg.m.s-3.K-1

    /*!
     * \brief Declares the HeatFluxDensity unit
     */
    typedef GenerateUnit<1,0,-3,0,0,0,0>::type HeatFluxDensity;  // kg.s-3

  } // end of Namesapce math

} // end of namespace tfel

namespace tfel{
  
  namespace utilities{

    /*!
     * \brief Partial specialisation for the NoUnit class. 
     */
    template<>
    struct Name<tfel::math::NoUnit>
    {
      /*!
       * \brief  Return the name of the class NoUnit.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[]");
      }
    };

    /*!
     * \brief Partial specialisation for the Mass class. 
     */
    template<>
    struct Name<tfel::math::Mass>
    {
      /*!
       * \brief  Return the name of the class Mass.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[kg]");
      }
    };

    /*!
     * \brief Partial specialisation for the Length class. 
     */
    template<>
    struct Name<tfel::math::Length>
    {
      /*!
       * \brief  Return the name of the class Length.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[m]");
      }
    };

    /*!
     * \brief Partial specialisation for the Time class. 
     */
    template<>
    struct Name<tfel::math::Time>
    {
      /*!
       * \brief  Return the name of the class Time.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[s]");
      }
    };

    /*!
     * \brief Partial specialisation for the Ampere class. 
     */
    template<>
    struct Name<tfel::math::Ampere>
    {
      /*!
       * \brief  Return the name of the class Ampere.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[A]");
      }
    };

    /*!
     * \brief Partial specialisation for the Temperature class. 
     */
    template<>
    struct Name<tfel::math::Temperature>
    {
      /*!
       * \brief  Return the name of the class Temperature.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[K]");
      }
    };

    /*!
     * \brief Partial specialisation for the Candela class. 
     */
    template<>
    struct Name<tfel::math::Candela>
    {
      /*!
       * \brief  Return the name of the class Candela.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[cd]");
      }
    };

    /*!
     * \brief Partial specialisation for the Mole class. 
     */
    template<>
    struct Name<tfel::math::Mole>
    {
      /*!
       * \brief  Return the name of the class Mole.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[mol]");
      }
    };

    /*!
     * \brief Partial specialisation for the Velocity class. 
     */
    template<>
    struct Name<tfel::math::Velocity>
    {
      /*!
       * \brief  Return the name of the class Velocity.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[Velocity]");
      }
    };

    /*!
     * \brief Partial specialisation for the Acceleration class. 
     */
    template<>
    struct Name<tfel::math::Acceleration>
    {
      /*!
       * \brief  Return the name of the class Acceleration.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[Acceleration]");
      }
    };

    /*!
     * \brief Partial specialisation for the Momentum class. 
     */
    template<>
    struct Name<tfel::math::Momentum>
    {
      /*!
       * \brief  Return the name of the class Momentum.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[Momentum]");
      }
    };

    /*!
     * \brief Partial specialisation for the Force class. 
     */
    template<>
    struct Name<tfel::math::Force>
    {
      /*!
       * \brief  Return the name of the class Force.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("[Force]");
      }
    };

  } // end of namespace utilities

} // end of namespace tfel


#endif /* _LIB_TFEL_UNITSAMPLES_H */
