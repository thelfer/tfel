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

#ifndef LIB_MTEST_MTESTEVOLUTION_H
#define LIB_MTEST_MTESTEVOLUTION_H

#include <map>
#include <vector>
#include <memory>

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      // forward declaration
      struct ExternalFunctionManager;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

namespace mtest {

  /*!
   * Base class for evolutions of external variables
   */
  struct MTEST_VISIBILITY_EXPORT Evolution {
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real operator()(const real) const = 0;
    /*!
     * \return true if the evolution
     * is constant
     */
    virtual bool isConstant() const = 0;
    /*!
     * \brief set the evolution value
     * \note most evolution will throw an exception (this
     * method only makes sense for constant evolutions)
     */
    virtual void setValue(const real) = 0;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     * \note most evolution will throw an exception (for example, this
     * method does not makes sense for constant evolutions)
     */
    virtual void setValue(const real, const real) = 0;
    //! destructor
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
    /*!
     * \return the value of the evolution
     * at the given time
     */
    real operator()(const real) const override;
    /*!
     * \return true if the evolution
     * is constant
     */
    bool isConstant() const override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] v  : value
     * \note most evolution will throw an exception (this
     * method only makes sense for constant evolutions)
     */
    void setValue(const real) override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    void setValue(const real, const real) override;
    //! destructor
    ~ConstantEvolution() override;

   protected:
    ConstantEvolution& operator=(const ConstantEvolution&) = delete;
    ConstantEvolution& operator=(ConstantEvolution&&) = delete;
    real value;
  };

  /*!
   * a linear per interval evolution
   */
  struct MTEST_VISIBILITY_EXPORT LPIEvolution final : public Evolution {
    /*!
     * constructor
     * \param[in] t : times
     * \param[in] v : values
     */
    LPIEvolution(const std::vector<real>&, const std::vector<real>&);
    /*!
     * \return the value of the evolution
     * at the given time
     */
    real operator()(const real) const override;
    /*!
     * \return true if the evolution
     * is constant
     */
    bool isConstant() const override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] v  : value
     * \note most evolution will throw an exception (this
     * method only makes sense for constant evolutions)
     */
    void setValue(const real) override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    void setValue(const real, const real) override;
    //! destructor
    ~LPIEvolution() override;

   private:
    std::map<real, real> values;
  };

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

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTEVOLUTION_H */
