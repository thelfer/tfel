/*!
 * \file  mtest/include/MTest/Evolution.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTEVOLUTION_HXX
#define LIB_MTEST_MTESTEVOLUTION_HXX

#include <map>
#include <vector>
#include <memory>
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace tfel::math::parser {

  // forward declaration
  struct ExternalFunctionManager;

}  // end of namespace tfel::math::parser

namespace mtest {

  //! \brief base class for evolutions of external variables
  struct MTEST_VISIBILITY_EXPORT Evolution {
    /*!
     * \return the value of the evolution at the given time
     * \param[in] t: time
     */
    virtual real operator()(const real) const = 0;
    /*!
     * \return true if the evolution is constant
     */
    virtual bool isConstant() const = 0;
    /*!
     * \brief set the evolution value
     * \note most evolution will throw an exception (this
     * method only makes sense for constant evolutions)
     * \param[in] v  : value
     */
    virtual void setValue(const real) = 0;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t: time
     * \param[in] v: value
     * \note most evolution will throw an exception (for example, this
     * method does not makes sense for constant evolutions)
     */
    virtual void setValue(const real, const real) = 0;
    //! \brief destructor
    virtual ~Evolution();
  };

  /*!
   * a constant evolution
   */
  struct MTEST_VISIBILITY_EXPORT ConstantEvolution final : public Evolution {
    /*!
     * constructor
     * \param[in] v : value of the evolution
     */
    ConstantEvolution(const real);
    //
    real operator()(const real) const override;
    bool isConstant() const override;
    void setValue(const real) override;
    void setValue(const real, const real) override;
    //! \brief destructor
    ~ConstantEvolution() override;

   protected:
    ConstantEvolution& operator=(const ConstantEvolution&) = delete;
    ConstantEvolution& operator=(ConstantEvolution&&) = delete;
    real value;
  };

  //! \brief a linear per interval evolution
  struct MTEST_VISIBILITY_EXPORT LPIEvolution final : public Evolution {
    /*!
     * \return the interpolation in a set of values
     * \param[in] values: values use to create the interpolation
     * \param[in] t: absicssa
     */
    static real interpolate(const std::map<real, real>&, const real);
    /*!
     * constructor
     * \param[in] t : times
     * \param[in] v : values
     */
    LPIEvolution(const std::vector<real>&, const std::vector<real>&);
    //
    real operator()(const real) const override;
    bool isConstant() const override;
    void setValue(const real) override;
    void setValue(const real, const real) override;
    //! \brief destructor
    ~LPIEvolution() override;

   private:
    std::map<real, real> values;
  };  // end of struct LPIEvolution

  /*!
   * \brief build a constant evolution from a real value
   * \param[in] v: value
   */
  MTEST_VISIBILITY_EXPORT std::shared_ptr<Evolution> make_evolution(const real);
  /*!
   *  \brief build a linear per interval evolution from a map
   * associating a time and a value.
   * \param[in] v: values
   */
  MTEST_VISIBILITY_EXPORT std::shared_ptr<Evolution> make_evolution(
      const std::map<real, real>&);
  /*!
   * \return an external function manager from constant evolutions
   * \param[in] evm: evolution manager
   */
  MTEST_VISIBILITY_EXPORT
  std::shared_ptr<tfel::math::parser::ExternalFunctionManager>
  buildExternalFunctionManagerFromConstantEvolutions(const EvolutionManager&);
  /*!
   * \brief check if evolutions with given names have been declared
   */
  MTEST_VISIBILITY_EXPORT
  void checkIfDeclared(const std::vector<std::string>&,
                       const EvolutionManager&,
                       const std::string&);
  /*!
   * \brief check if evolutions with given names have been declared
   */
  MTEST_VISIBILITY_EXPORT
  void checkIfDeclared(const std::vector<std::string>&,
                       const EvolutionManager&,
                       const EvolutionManager&,
                       const std::string&);

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTEVOLUTION_HXX */
