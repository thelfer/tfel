/*!
 * \file   SlipSystemsDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   09 juin 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_SLIPSYSTEMSDESCRIPTION_HXX
#define LIB_TFEL_MATERIAL_SLIPSYSTEMSDESCRIPTION_HXX

#include <array>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "TFEL/Material/CrystalStructure.hxx"

namespace tfel::material {

  /*!
   * \brief a class handling the description of a slip system
   * This is a small wrapper around the TFELNUMODIS library.
   */
  struct TFELMATERIAL_VISIBILITY_EXPORT SlipSystemsDescription {
    //! \brief a simple alias
    using vec3d = std::array<int, 3u>;
    //! \brief a simple alias
    using vec4d = std::array<int, 4u>;
    //! \brief a simple alias
    using vector = std::array<long double, 3u>;
    //! \brief a simple alias
    using tensor = std::array<long double, 9u>;
    //!
    struct system3d {
      vec3d burgers;
      vec3d plane;
    };
    //!
    struct system4d {
      vec4d burgers;
      vec4d plane;
    };
    //! \brief a simple alias
    using vec = tfel::utilities::GenType<vec3d, vec4d>;
    //! \brief a simple alias
    using system = tfel::utilities::GenType<system3d, system4d>;
    //! \brief a simple alias
    using size_type = std::vector<system3d>::size_type;
    //! \brief list of all supported interaction matrix model
    enum InteractionMatrixModel {
      NONE,
      DUPUY2017
    };  // end of InteractionMatrixModel
    /*!
     * \brief structure describing the structure of the interaction
     * matrix.
     */
    struct TFELMATERIAL_VISIBILITY_EXPORT InteractionMatrixStructure {
      /*!
       * \brief structure descrbing the interaction of two sliding
       * systems.
       */
      struct SlidingSystemsInteraction {
        system g1;
        system g2;
      };
      using SSIContainer = std::vector<std::vector<SlidingSystemsInteraction>>;
      //! \brief constructor
      explicit InteractionMatrixStructure(const SSIContainer&);
      //! \brief move constructor
      InteractionMatrixStructure(InteractionMatrixStructure&&) noexcept;
      //! \brief copy constructor
      InteractionMatrixStructure(const InteractionMatrixStructure&);
      //! \return the number of independant coefficients
      [[nodiscard]] size_type rank() const;
      //! \return get the rank of a pair of systems
      [[nodiscard]] size_type getRank(const system&, const system&) const;
      //! \return the sliding system interaction, sorted by rank
      [[nodiscard]] const SSIContainer& getSlidingSystemsInteraction() const;
      //! \brief destructor
      ~InteractionMatrixStructure();

     private:
      //! \brief sliding system sorted by rank
      const SSIContainer ranks;
    };
    /*!
     * \param[in] s:  crystal structure
     */
    explicit SlipSystemsDescription(const CrystalStructure);
    /*!
     * copy constructor
     * \param[in] src: object copied
     */
    SlipSystemsDescription(const SlipSystemsDescription&);
    /*!
     * move constructor
     * \param[in] src: object moved
     */
    SlipSystemsDescription(SlipSystemsDescription&&) noexcept;
    //! \return the crystal structure
    [[nodiscard]] CrystalStructure getCrystalStructure() const;
    /*!
     * \param[in] b: Burgers' vector
     * \param[in] p: plane
     */
    void addSlipSystemsFamily(const vec3d&, const vec3d&);
    /*!
     * \param[in] b: Burgers' vector
     * \param[in] p: plane normal
     */
    void addSlipSystemsFamily(const vec4d&, const vec4d&);
    //! \return the number of slip systems families
    [[nodiscard]] size_type getNumberOfSlipSystemsFamilies() const;
    /*!
     * \return the ith slip system family
     * \param[in] i: index
     */
    [[nodiscard]] system getSlipSystemFamily(const size_type) const;
    /*!
     * \param[in] i: family index
     * \return the number of slip systems for the given family
     */
    [[nodiscard]] size_type getNumberOfSlipSystems(const size_type) const;
    //! \return the number of slip systems
    [[nodiscard]] size_type getNumberOfSlipSystems() const;
    /*!
     * \return all the slip systems associated with the ith family
     * \param[in] i: index
     */
    [[nodiscard]] std::vector<system> getSlipSystems(const size_type) const;
    //! \return all the slip systems, sorted by family
    [[nodiscard]] std::vector<std::vector<system>> getSlipSystems() const;
    //! \return the normals to the slip planes, sorted by family
    [[nodiscard]] std::vector<std::vector<vector>> getSlipPlaneNormals() const;
    /*!
     * \return the normals to the slip plane associated with the ith family
     * \param[in] i: index
     */
    [[nodiscard]] std::vector<vector> getSlipPlaneNormals(
        const size_type) const;
    //! \return the unit vectors giving the slip direction, sorted by family
    [[nodiscard]] std::vector<std::vector<vector>> getSlipDirections() const;
    /*!
     * \return the  unit vectors giving the slip direction the ith family
     * \param[in] i: index
     */
    [[nodiscard]] std::vector<vector> getSlipDirections(const size_type) const;
    //! \return the orientation tensors, sorted by family
    [[nodiscard]] std::vector<std::vector<tensor>> getOrientationTensors()
        const;
    /*!
     * \return the orientation tensors associated with the ith family
     * \param[in] i: index
     */
    [[nodiscard]] std::vector<tensor> getOrientationTensors(
        const size_type) const;
    //! \return the climb tensors, sorted by family
    [[nodiscard]] std::vector<std::vector<tensor>> getClimbTensors() const;
    /*!
     * \return the climb tensors associated with the ith family
     * \param[in] i: index
     */
    [[nodiscard]] std::vector<tensor> getClimbTensors(const size_type) const;
    /*!
     * \return the Schmid factor, sorted by family
     * \param[in] d: direction
     */
    [[nodiscard]] std::vector<std::vector<long double>> getSchmidFactors(
        const vec) const;
    /*!
     * \return the Schmid factors associated with the ith family
     * \param[in] d: direction
     * \param[in] i: index
     */
    [[nodiscard]] std::vector<long double> getSchmidFactors(
        const vec, const size_type) const;
    /*!
     * \return a structure defining the structure of the
     * interaction matrix
     */
    [[nodiscard]] InteractionMatrixStructure getInteractionMatrixStructure()
        const;
    //! \return if an interaction matrix has been defined
    [[nodiscard]] bool hasInteractionMatrix() const;
    /*!
     * \brief set the interaction matrix
     * \param[in] v: values
     */
    void setInteractionMatrix(const std::vector<long double>&);
    //! \return the interaction matrix
    [[nodiscard]] const std::vector<long double>& getInteractionMatrix() const;
    /*!
     * \return if an interaction matrix for defining the effects of
     * the dislocations on the mean free path of a dislocation has
     * been defined.
     */
    [[nodiscard]] bool hasDislocationsMeanFreePathInteractionMatrix() const;
    /*!
     * \brief set the interaction matrix
     * \param[in] v: values
     */
    void setDislocationsMeanFreePathInteractionMatrix(
        const std::vector<long double>&);
    //! \return the interaction matrix
    [[nodiscard]] const std::vector<long double>&
    getDislocationsMeanFreePathInteractionMatrix() const;
    //! \brief destructor
    ~SlipSystemsDescription();

   private:
    //! \brief gliding systems
    tfel::utilities::GenType<std::vector<system3d>, std::vector<system4d>> gs;
    //! \brief the crystal structure
    const CrystalStructure cs;
    //! \brief the interaction matrix
    std::vector<long double> m;
    /*! \brief matrix standing for the effects of the dislocations on the mean
     * free path of a dislocation.
     */
    std::vector<long double> mfpm;
  };  // end of SlipSystemsDescription

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_SLIPSYSTEMSDESCRIPTION_HXX */
