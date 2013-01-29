/*!
 * \file   MechanicalBehaviour.hxx
 * \brief  This file declares the MechanicalBehaviour class
 * \author Helfer Thomas
 * \date   22 Sep 2006
 */

#ifndef _LIB_TFEL_MECHANICALBEHAVIOUR_H_
#define _LIB_TFEL_MECHANICALBEHAVIOUR_H_ 

#include"TFEL/Material/MechanicalBehaviourData.hxx"

namespace tfel{

  namespace material{

    /*!
     * \class MechanicalBehaviour
     * \brief This class declares an interface for mechanical behaviours.
     * \param unsigned short N, space dimension.
     * \param typename T, numerical type.
     * \param bool use_qt, says if one shall use quantities.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template<unsigned short N,typename NumType, bool use_qt>
    struct MechanicalBehaviour
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	return std::string("MechanicalBehaviour");
      }

      /*!
       * determine the value of the internal state variables at the
       * end of the time step
       * \param[in] b : compute the stiffness matrix
       * \return true if the integration is successfull
       */
      virtual bool
      integrate(const bool) = 0;

      /*!
       * \return the tangent operator
       */
      virtual const typename tfel::config::Types<N,NumType,use_qt>::StiffnessTensor&
      getTangentOperator(void) const = 0;

      /*!
       * destructor
       */
      virtual ~MechanicalBehaviour(){};

    };

  } // end of namespace material 

} // end of namespace tfel

#endif /* _LIB_TFEL_MECHANICALBEHAVIOUR_H */

