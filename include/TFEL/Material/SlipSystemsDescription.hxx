/*!
 * \file   SlipSystemsDescription.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 juin 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATERIAL_SLIPSYSTEMSDESCRIPTION_HXX
#define LIB_TFEL_MATERIAL_SLIPSYSTEMSDESCRIPTION_HXX

#include<array>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/GenTypeBase.hxx"
#include"TFEL/Material/CrystalStructure.hxx"

namespace tfel{

  namespace material{

    /*!
     * \brief a class handling the description of a slip system
     * This is a small wrapper around the TFELNUMODIS library.
     */
    struct TFELMATERIAL_VISIBILITY_EXPORT SlipSystemsDescription
    {
      //! a simple alias
      using vec3d  = std::array<int,3u>;
      //! a simple alias
      using vec4d  = std::array<int,4u>;
      //! a simple alias
      using tensor = std::array<long double,9u>;
      //! 
      struct system3d{
	vec3d burgers;
	vec3d plane;
      };
      //! 
      struct system4d{
	vec4d burgers;
	vec4d plane;
      };
      //! a simple alias
      using system = tfel::utilities::GenType<system3d,system4d>;
      //! a simple alias
      using size_type = std::vector<system3d>::size_type;
      //! list of all supported interaction matrix model
      enum InteractionMatrixModel{
	NONE,
	DUPUY2017
      }; // end of InteractionMatrixModel
      /*!
       * \brief structure describing the structure of the interaction
       * matrix.
       */
      struct TFELMATERIAL_VISIBILITY_EXPORT InteractionMatrixStructure
      {
	/*!
	 * \brief structure descrbing the interaction of two sliding
	 * systems.
	 */
	struct SlidingSystemsInteraction{
	  system g1;
	  system g2;
	};
	using SSIContainer = std::vector<std::vector<SlidingSystemsInteraction>>;
	//! constructor
	InteractionMatrixStructure(const SSIContainer&);
	//! move constructor
	InteractionMatrixStructure(InteractionMatrixStructure&&);
	//! copy constructor
	InteractionMatrixStructure(const InteractionMatrixStructure&);
	//! \return the number of independant coefficients 
	size_type rank() const;
	//! \return get the rank of a pair of systems
	size_type getRank(const system&,
			  const system&) const;
	//! \return the sliding system interaction, sorted by rank
	const SSIContainer& getSlidingSystemsInteraction() const;
	//! destructor
	~InteractionMatrixStructure();
      private:
	//! \brief sliding system sorted by rank
	const SSIContainer ranks;
      };
      /*!
       * \param[in] s:  crystal structure
       */
      SlipSystemsDescription(const CrystalStructure);
      /*!
       * copy constructor
       * \param[in] src: object copied
       */
      SlipSystemsDescription(const SlipSystemsDescription&);
      /*!
       * move constructor
       * \param[in] src: object moved
       */
      SlipSystemsDescription(SlipSystemsDescription&&);
      //! \return the crystal structure
      CrystalStructure getCrystalStructure() const;
      /*!
       * \param[in] p: plane
       * \param[in] b: Burgers' vector
       */
      void addSlipSystemsFamily(const vec3d&,const vec3d&);
      /*!
       * \param[in] p: plane normal
       * \param[in] b: Burgers' vector
       */
      void addSlipSystemsFamily(const vec4d&,const vec4d&);
      //! \return the number of slip systems families
      size_type getNumberOfSlipSystemsFamilies() const;
      /*!
       * \return the ith slip system family
       * \param[in] i: index
       */
      system getSlipSystemFamily(const size_type) const;
      /*!
       * \param[in] i: family index
       * \return the number of slip systems for the given family
       */
      size_type getNumberOfSlipSystems(const size_type) const;
      /*!
       * \param[in] i: family index
       * \return the number of slip systems for the given family
       */
      size_type getNumberOfSlipSystems() const;
      /*!
       * \return all the slip systems associated with the ith family
       * \param[in] i: index
       */
      std::vector<system> getSlipSystems(const size_type) const;
      //! \return all the slip systems, sorted by family
      std::vector<std::vector<system>> getSlipSystems() const;
      //! \return the orientation tensors, sorted by family
      std::vector<std::vector<tensor>> getOrientationTensors() const;
      /*!
       * \return the orientation tensors associated with the ith family
       * \param[in] i: index
       */
      std::vector<tensor> getOrientationTensors(const size_type) const;
      /*!
       * \return a structure defining the structure of the
       * interaction matrix
       */
      InteractionMatrixStructure getInteractionMatrixStructure() const;
      //! destructor
      ~SlipSystemsDescription();
    private:
      //! gliding systems
      tfel::utilities::GenType<std::vector<system3d>,
			       std::vector<system4d>> gs;
      //! the crystal structure
      const CrystalStructure cs;
      //! InteractionMatrix 
    }; // end of SlipSystemsDescription
    
  } // end of namespace material

} // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_SLIPSYSTEMSDESCRIPTION_HXX */
