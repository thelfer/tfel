/*!
 * \file   IsotropicLinearFourierHookLaw.hxx
 * \brief  This file declares the IsotropicLinearFourierHookLaw class
 * \author Helfer Thomas
 * \date   27 Sep 2006
 */

#ifndef _LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_H_
#define _LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_H_ 

#include"Material/IsotropicLinearFourierLaw.hxx"
#include"Material/IsotropicLinearHookLaw.hxx"

namespace tfel{

  namespace material{

    /*!
     * \class IsotropicLinearFourierHookLaw
     * \brief This class implements the hook law.
     * \param N, space dimension.
     * \param T, numerical type.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template<unsigned short N,typename T,bool use_qt>
    class IsotropicLinearFourierHookLaw
      : public IsotropicLinearFourierLaw<N,T,use_qt>,
	public IsotropicLinearHookLaw<N,T,use_qt>
    {};

  } // end of namespace material

} // end of namespace  tfel


#endif /* _LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_H */

