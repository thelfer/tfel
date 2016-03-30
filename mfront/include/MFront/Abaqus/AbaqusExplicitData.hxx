/*!
 * \file   AbaqusExplicitData.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   29 mars 2016
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSEXPLICITDATA_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSEXPLICITDATA_HXX

namespace abaqus{

  template<typename T>
  struct AbaqusExplicitData{
    const int nblock;
    const int ndir;
    const int nshr;
    const int nstatev;
    const int nfieldv;
    const int nprops;
    const T dt;
    const T * const props;
    const T * const density;
    const T * const strainInc;
    const T * const relSpinInc;
    const T * const tempOld;
    const T * const stretchOld;
    const T * const defgradOld;
    const T * const fieldOld;
    const T * const stressOld;
    const T * const stateOld;
    const T * const enerInternOld;
    const T * const enerInelasOld;
    const T * const tempNew;
    const T * const stretchNew;
    const T * const defgradNew;
    const T * const fieldNew;
    const T * stressNew;
    const T * stateNew;
    const T * enerInternNew;
    const T * enerInelasNew;
  }; // end of struct AbaqusExplicitData
  
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSEXPLICITDATA_HXX_ */
