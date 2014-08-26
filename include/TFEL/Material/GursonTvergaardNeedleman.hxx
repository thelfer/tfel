/*! 
 * \file   GURSONTVERGAARDNEEDLEMAN.hxx
 * \brief
 * \author Jeremy Hure
 * \brief  04 Septembre 2013
 */

#ifndef _LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_H_
#define _LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_H_ 

#include<cmath>

namespace tfel
{

  namespace material
  {

    /*!
     * \brief This function computes the implicit flow stress sigma_star
     * of the Gurson-Tvergaard-Needleman model
     * \param[out] sst:          Implicit flow stress
     * \param[in]  seq:          Mises stress
     * \param[in]  skk:          Stress trace
     * \param[in]  fst:          Void volume fraction
     * \param[in]  q1:           Parameter q1
     * \param[in]  q2:           Parameter q2
     */
    template<typename T>
    T computeFlowStressGTN(const T seq,
			   const T skk,
			   const T fst,
			   const T q1,
			   const T q2)
    {
      using namespace std;
      T sst = seq;
      T S = pow((seq/sst),2) + 2*q1*fst*cosh((q2*skk)/(2*sst)) - 1 - pow((q1*fst),2);
      T SP = (-2*pow((seq/sst),2)/(sst)) - ((q1*q2*fst*skk)/(sst*sst))*sinh((q2*skk)/(2*sst));
      while(S>1.e-10){
	sst = sst - S/SP;
	S =  pow((seq/sst),2) + 2*q1*fst*cosh((q2*skk)/(2*sst)) - 1 - pow((q1*fst),2);
	SP = (-2*pow((seq/sst),2)/(sst)) - ((q1*q2*fst*skk)/(sst*sst))*sinh((q2*skk)/(2*sst));   
      }
      return sst;
    } // end of computeFlowStressGTN

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_H */

