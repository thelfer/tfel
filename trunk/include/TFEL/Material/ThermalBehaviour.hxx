/*!
 * \file   ThermalBehaviour.hxx
 * \brief  This file declares the ThermalBehaviour class
 * \author Helfer Thomas
 * \date   22 Sep 2006
 */

#ifndef _LIB_TFEL_THERMALBEHAVIOUR_H_
#define _LIB_TFEL_THERMALBEHAVIOUR_H_ 

#include"TFEL/Material/ThermalBehaviourData.hxx"

namespace tfel{

  namespace material{

    /*!
     * \class ThermalBehaviour
     * \brief This class declares an interface for thermal behaviours.
     * \param unsigned short N, space dimension.
     * \param typename T, numerical type.
     * \param bool use_qt, says if one shall use quantities.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template<unsigned short N,typename NumType, bool use_qt>
    class ThermalBehaviour
    {

      typedef ThermalBehaviourData<N,NumType,use_qt> ThermalBehaviourData;

    public:

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("ThermalBehaviour");
      }

      // virtual void doThermalIntegration(ThermalBehaviourData&) = 0;

      virtual ~ThermalBehaviour(){};

    };

  } // end of namespace material 

} // end of namespace tfel

#endif /* _LIB_TFEL_THERMALBEHAVIOUR_H */

