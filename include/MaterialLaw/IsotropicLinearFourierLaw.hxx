/*!
 * \file   IsotropicLinearFourierLaw.hxx
 * \brief  This file declares the IsotropicLinearFourierLaw class
 * \author Helfer Thomas
 * \date   25 Sep 2006
 */

#ifndef _LIB_TFEL_ISOTROPICLINEARFOURIERLAW_H_
#define _LIB_TFEL_ISOTROPICLINEARFOURIERLAW_H_ 

#include"MaterialLaw/ThermalBehaviour.hxx"
#include"MaterialLaw/ThermalBehaviourData.hxx"

namespace tfel{

  namespace materiallaw{

    /*!
     * \class IsotropicLinearFourierLaw
     * \brief This class implements the lorentz damage law.
     * \param N, space dimension.
     * \param T, numerical type.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template<unsigned short N,typename T,bool use_qt>
    class IsotropicLinearFourierLaw
      : public ThermalBehaviour<N,T,use_qt>
    {

      TFEL_STATIC_ASSERT(N==1||N==2||N==3);

      typedef tfel::config::Types<N,T,use_qt> Types;
 
      typedef typename Types::real        real;
      typedef typename Types::temperature temperature;

    public:

      /*
       * Default Constructor
       */
      IsotropicLinearFourierLaw(){};
    
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static const std::string getName(void){
	return std::string("IsotropicLinearElasticLaw");
      }

      /*!
       * Law integration
       */
      void doThermalIntegration(ThermalBehaviourData<N,T,use_qt>& data){

	CheckThermalDataValidity<IsotropicLinearFourierLaw>::exe(data);

	
	
      }

    };

  } // end of namespace materiallaw

} // end of namespace  tfel

#endif /* _LIB_TFEL_ISOTROPICLINEARFOURIERLAW_H */

