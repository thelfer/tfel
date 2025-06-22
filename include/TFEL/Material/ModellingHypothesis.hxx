/*!
 * \file  include/TFEL/Material/ModellingHypothesis.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 16 mar 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_HXX
#define LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_HXX

#include <string>
#include <vector>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::material {

  //! \brief a helper structure defining the Hypothesis enum.
  struct TFELMATERIAL_VISIBILITY_EXPORT ModellingHypothesis {
    /*!
     * the list of supported modelling hypothesis
     */
    enum Hypothesis {
      AXISYMMETRICALGENERALISEDPLANESTRAIN,
      AXISYMMETRICALGENERALISEDPLANESTRESS,
      AXISYMMETRICAL,
      PLANESTRESS,
      PLANESTRAIN,
      GENERALISEDPLANESTRAIN,
      TRIDIMENSIONAL,
      UNDEFINEDHYPOTHESIS
    };  // end of enum ModellingHypothesis

    static const std::vector<Hypothesis>& getModellingHypotheses();

    /*!
     * convert a string to a modelling hypothesis
     *
     * AXISYMMETRICALGENERALISEDPLANESTRAIN <=>
     * AxisymmetricalGeneralisedPlaneStrain AXISYMMETRICALGENERALISEDPLANESTRESS
     * <=> AxisymmetricalGeneralisedPlaneStress, AXISYMMETRICAL <=>
     * Axisymmetrical PLANESTRESS                          <=> PlaneStress
     * PLANESTRAIN                          <=> PlaneStrain
     * GENERALISEDPLANESTRAIN               <=> GeneralisedPlaneStrain
     * TRIDIMENSIONAL                       <=> Tridimensional
     */
    static Hypothesis fromString(const std::string&);
    /*!
     * \return a string correpsonding to the modelling hypothesis
     * AXISYMMETRICALGENERALISEDPLANESTRAIN <=>
     * AxisymmetricalGeneralisedPlaneStrain AXISYMMETRICALGENERALISEDPLANESTRESS
     * <=> AxisymmetricalGeneralisedPlaneStress, AXISYMMETRICAL <=>
     * Axisymmetrical PLANESTRESS                          <=> PlaneStress
     * PLANESTRAIN                          <=> PlaneStrain
     * GENERALISEDPLANESTRAIN               <=> GeneralisedPlaneStrain
     * TRIDIMENSIONAL                       <=> Tridimensional
     */
    static std::string toString(const Hypothesis);
    /*!
     * \return an upper case string correpsonding to the modelling
     * hypothesis
     */
    static std::string toUpperCaseString(const Hypothesis);
    /*!
     * \return true if the given string stands for a modelling
     * hypothesis
     */
    static bool isModellingHypothesis(const std::string&);
  };  // end of struct ModellingHypothesis

  /*!
   * a metafunction returning the space dimension of a modelling
   * hypothesis.
   */
  template <ModellingHypothesis::Hypothesis>
  struct ModellingHypothesisToSpaceDimension;

  /*!
   * partial specialisation for
   * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToSpaceDimension<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 1u;
  };

  /*!
   * partial specialisation for
   * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS
   */
  template <>
  struct ModellingHypothesisToSpaceDimension<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 1u;
  };

  /*!
   * partial specialisation for
   * ModellingHypothesis::AXISYMETRICAL
   */
  template <>
  struct ModellingHypothesisToSpaceDimension<
      ModellingHypothesis::AXISYMMETRICAL> {
    //! the return of the metafunction
    static constexpr unsigned short value = 2u;
  };

  /*!
   * partial specialisation for
   * ModellingHypothesis::PLANESTRESS
   */
  template <>
  struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::PLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 2u;
  };

  /*!
   * partial specialisation for
   * ModellingHypothesis::PLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::PLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 2u;
  };

  /*!
   * partial specialisation for
   * ModellingHypothesis::GENERALISEDPLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToSpaceDimension<
      ModellingHypothesis::GENERALISEDPLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 2u;
  };

  /*!
   * partial specialisation for
   * ModellingHypothesis::TRIDIMENSIONAL
   */
  template <>
  struct ModellingHypothesisToSpaceDimension<
      ModellingHypothesis::TRIDIMENSIONAL> {
    //! the return of the metafunction
    static constexpr unsigned short value = 3u;
  };

  /*!
   * \brief a metafunction returning the size of a symmetric tensor for the
   * given modelling hypothesis.
   */
  template <ModellingHypothesis::Hypothesis>
  struct ModellingHypothesisToStensorSize;

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToStensorSize<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 3u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS
   */
  template <>
  struct ModellingHypothesisToStensorSize<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 3u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::AXISYMETRICAL
   */
  template <>
  struct ModellingHypothesisToStensorSize<ModellingHypothesis::AXISYMMETRICAL> {
    //! the return of the metafunction
    static constexpr unsigned short value = 4u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::PLANESTRESS
   */
  template <>
  struct ModellingHypothesisToStensorSize<ModellingHypothesis::PLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 4u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::PLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToStensorSize<ModellingHypothesis::PLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 4u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::GENERALISEDPLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToStensorSize<
      ModellingHypothesis::GENERALISEDPLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 4u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::TRIDIMENSIONAL
   */
  template <>
  struct ModellingHypothesisToStensorSize<ModellingHypothesis::TRIDIMENSIONAL> {
    //! the return of the metafunction
    static constexpr unsigned short value = 6u;
  };

  /*!
   * \brief a metafunction returning the size of a non-symmetric tensor for the
   * given modelling hypothesis.
   */
  template <ModellingHypothesis::Hypothesis>
  struct ModellingHypothesisToTensorSize;

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToTensorSize<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 3u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS
   */
  template <>
  struct ModellingHypothesisToTensorSize<
      ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 3u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::AXISYMETRICAL
   */
  template <>
  struct ModellingHypothesisToTensorSize<ModellingHypothesis::AXISYMMETRICAL> {
    //! the return of the metafunction
    static constexpr unsigned short value = 5u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::PLANESTRESS
   */
  template <>
  struct ModellingHypothesisToTensorSize<ModellingHypothesis::PLANESTRESS> {
    //! the return of the metafunction
    static constexpr unsigned short value = 5u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::PLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToTensorSize<ModellingHypothesis::PLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 5u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::GENERALISEDPLANESTRAIN
   */
  template <>
  struct ModellingHypothesisToTensorSize<
      ModellingHypothesis::GENERALISEDPLANESTRAIN> {
    //! the return of the metafunction
    static constexpr unsigned short value = 5u;
  };

  /*!
   * \brief partial specialisation for
   * ModellingHypothesis::TRIDIMENSIONAL
   */
  template <>
  struct ModellingHypothesisToTensorSize<ModellingHypothesis::TRIDIMENSIONAL> {
    //! the return of the metafunction
    static constexpr unsigned short value = 9u;
  };

  /*!
   * \return the space dimension associated to a modelling hypothesis
   * \param[in] h: modelling hypothesis
   */
  TFELMATERIAL_VISIBILITY_EXPORT unsigned short getSpaceDimension(
      const ModellingHypothesis::Hypothesis);

  /*!
   * \return the size of a symmetric tensor for a given modelling
   * hypothesis
   * \param[in] h: modelling hypothesis
   */
  TFELMATERIAL_VISIBILITY_EXPORT unsigned short getStensorSize(
      const ModellingHypothesis::Hypothesis);

  /*!
   * \return the size of a tensor for a given modelling hypothesis
   * \param[in] h: modelling hypothesis
   */
  TFELMATERIAL_VISIBILITY_EXPORT unsigned short getTensorSize(
      const ModellingHypothesis::Hypothesis);

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_HXX */
