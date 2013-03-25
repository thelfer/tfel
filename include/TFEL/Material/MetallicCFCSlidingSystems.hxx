/*! 
 * \file  MetallicCFCSlidingSystems.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mars 2013
 */

#ifndef _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_H_
#define _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel
{

  namespace material
  {

    /*!
     * An helper class which actually does the computation of the
     * tensor of directional sense.
     * Those tensors are computed as matrices.
     */
    template<unsigned short N>
    struct MetallicCFCSlidingSystemsBase
    {
      //! tensor of directional sense
      tfel::math::tvector<12,tfel::math::stensor<N,double> > mus;
    protected:
      /*!
       * constructor
       */
      MetallicCFCSlidingSystemsBase();
    private:
      // copy constructor (disabled)
      MetallicCFCSlidingSystemsBase(const MetallicCFCSlidingSystemsBase&);
      // assignement operator (disabled)
      MetallicCFCSlidingSystemsBase&
      operator=(const MetallicCFCSlidingSystemsBase&);
    };

    /*!
     * structure in charge of evaluation the tensor of directional
     * sense systems of MetallicCFC's cristals.
     *
     * We use the singleton pattern to ensure that this object is only
     * build once. This class is specialised in 1D, 2D and 3D but the
     * very sense of using those class in 1D or 2D has not been
     * investigated yet.
     */
    template<unsigned short N>
    struct MetallicCFCSlidingSystems;

    /*!
     * specialisation in 1D
     */
    template<>
    struct TFELMATERIALSINGLECRISTAL_VISIBILITY_EXPORT MetallicCFCSlidingSystems<1u>
      : public MetallicCFCSlidingSystemsBase<1u>
    {
      /*!
       * \return the uniq instance of this class
       */
      static MetallicCFCSlidingSystems&
      getMetallicCFCSlidingSystems();
    protected:
      // constructor
      MetallicCFCSlidingSystems();
    };

    /*!
     * specialisation in 2D
     */
    template<>
    struct TFELMATERIALSINGLECRISTAL_VISIBILITY_EXPORT MetallicCFCSlidingSystems<2u>
      : public MetallicCFCSlidingSystemsBase<2u>
    {
      /*!
       * \return the uniq instance of this class
       */
      static MetallicCFCSlidingSystems&
      getMetallicCFCSlidingSystems();
    protected:
      // constructor
      MetallicCFCSlidingSystems();
    };

    /*!
     * specialisation in 3D
     */
    template<>
    struct TFELMATERIALSINGLECRISTAL_VISIBILITY_EXPORT MetallicCFCSlidingSystems<3u>
      : public MetallicCFCSlidingSystemsBase<3u>
    {
      /*!
       * \return the uniq instance of this class
       */
      static MetallicCFCSlidingSystems&
      getMetallicCFCSlidingSystems();
    protected:
      // constructor
      MetallicCFCSlidingSystems();
    };

  } // end of namespace material

} // end of namespace tfel

#include"TFEL/Material/MetallicCFCSlidingSystems.ixx"

#endif /* _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_H */

