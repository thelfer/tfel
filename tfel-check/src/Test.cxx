/*!
 * \file tfel-check/src/Test.cxx
 * \date 29 mai 2013
 * \author Remy Petkantchin
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Check/Test.hxx"

namespace tfel {

  namespace check {

    Test::Test() = default;
    Test::Test(Test&&) = default;
    Test::Test(const Test&) = default;
    Test& Test::operator=(Test&&) = default;
    Test& Test::operator=(const Test&) = default;
    Test::~Test() = default;

    void Test::compare() {
      this->comparison->setParameters(
          this->c1, this->c2, this->prec, this->precision2, this->ci,
          this->interpolation->getType(), this->interpolation->isConform(),
          this->colIntegralInterpolated, this->integralInterpolation);

      this->comparison->compare();
    }

    bool Test::hasSucceed() const { return this->comparison->hasSucceed(); }

    std::string Test::getMsgLog() {
      return this->comparison->getMsgLog() + "\n" + this->msgLog;
    }

    const std::shared_ptr<Comparison> Test::getComparison() const {
      return this->comparison;
    }

    void Test::setComparison(const std::shared_ptr<Comparison>& c) {
      this->comparison = c;
    }

    bool Test::isAllowLessResults() const { return this->allowLessResults; }

    void Test::setAllowLessResults(const bool b) { this->allowLessResults = b; }

    void Test::setColA(const std::shared_ptr<Column>& c) { this->c1 = c; }

    void Test::setColB(const std::shared_ptr<Column>& c) { this->c2 = c; }

    void Test::setColInterpolated(const std::shared_ptr<Column>& c) {
      this->ci = c;
    }

    const std::string& Test::getFileA() const { return this->f1; }

    void Test::setFileA(const std::string& f) { this->f1 = f; }

    const std::string& Test::getFileB() const { return this->f2; }

    void Test::setFileB(const std::string& f) { this->f2 = f; }

    const std::shared_ptr<Interpolation>& Test::getInterpolation() const {
      return this->interpolation;
    }

    void Test::setInterpolation(const std::shared_ptr<Interpolation>& i) {
      this->interpolation = i;
    }

    double Test::getPrec() const { return this->prec; }

    void Test::setPrec(const double v) { this->prec = v; }

    double Test::getPrecision2() const { return this->precision2; }

    void Test::setPrecision2(const double v) { this->precision2 = v; }

    const std::shared_ptr<Column>& Test::getColIntegralInterpolated() const {
      return this->ci;
    }

    void Test::setColIntegralInterpolated(const std::shared_ptr<Column>& c) {
      this->ci = c;
    }

    const std::shared_ptr<Interpolation>& Test::getIntegralInterpolation()
        const {
      return this->integralInterpolation;
    }

    void Test::setIntegralInterpolation(
        const std::shared_ptr<Interpolation>& i) {
      this->integralInterpolation = i;
    }

    const std::shared_ptr<Column>& Test::getColA() const { return this->c1; }

    const std::shared_ptr<Column>& Test::getColB() const { return this->c2; }

    void Test::applyInterpolation() {
      using namespace std;
      using size_type = std::vector<double>::size_type;

      this->c1->setFilename(this->f1);
      this->c2->setFilename(this->f2);

      std::ostringstream msg;
      msg.str("");  // wipe stream

      if (!(this->interpolation->isConform()) &&
          this->comparison->getName() != "area") {
        // tests if we can interpolate or not, based on columns' size and type
        // of interpolation
        if (this->c1->getValues().size() != this->c2->getValues().size()) {
          msg << "FAILED : ";
          msg << "columns ";
          msg << this->c1->getName();
          msg << " of files '" << this->c1->getFilename() << "' and '"
              << this->c2->getFilename()
              << "' don't have the same size, and interpolation is of type '"
              << this->interpolation->getType() << "' ("
              << this->c1->getValues().size() << " vs "
              << this->c2->getValues().size() << ")";
          this->msgLog.append(msg.str());
          raise(msg.str());
        }
      } else if (this->interpolation->isConform() &&
                 this->comparison->getName() != "area") {
        auto colInterpolatedTmpA = std::make_shared<Column>(*(this->ci));
        std::shared_ptr<Column> ci1(
            colInterpolatedTmpA); /**!< abscissa column for .ref */

        auto colInterpolatedTmpB = std::make_shared<Column>(*(this->ci));
        std::shared_ptr<Column> ci2(
            colInterpolatedTmpB); /**!< abscissa column for .res */

        // Opening the files to get the times columns
        ci1->setFilename(this->f1);
        ci2->setFilename(this->f2);

        if (this->c1->getValues().size() <= this->c2->getValues().size()) {
          vector<double> cXBTmp = ci2->getValues();
          vector<double> cbTmp = this->c2->getValues();
          vector<double> cXATmp = ci1->getValues();

          //      if (beforeInterFile)		// stores the before
          //      interpolation values
          //        for(size_type i = 0; i < ci2->getValues().size(); ++i)
          //          beforeInterFile << ci2->getValues().at(i) << " "
          //                          << this->c2->getValues().at(i) << endl;
          //      else
          //        msg << "\nProblem saving file\n";

          this->interpolation->interpolate(cXBTmp,
                                           cbTmp);  // interpolation of col B
          this->c2->clearValues();
          this->c2->resizeValues(this->c1->getValues().size());

          for (size_type i = 0; i < ci1->getValues().size(); ++i) {
            this->c2->setValue(
                i, this->interpolation->getValue(ci1->getValues().at(i)));
          }
          //      if (afterInterFile)		// stores the after
          //      interpolation values
          //        for(size_type i = 0; i < ci1->getValues().size(); ++i)
          //          afterInterFile << ci1->getValues().at(i) << " "
          //                         << this->c2->getValues().at(i) << endl;
          //      else
          //        msg << "\nProblem saving file\n";
          this->msgLog.append(msg.str());
          //      beforeInterFile.close();
          //      afterInterFile.close();
        } else if (this->c2->getValues() < this->c1->getValues() &&
                   this->allowLessResults) {
          //      if (beforeInterFile)		// stores the before
          //      interpolation values
          //        for(size_type i = 0; i < ci1->getValues().size(); ++i)
          //          beforeInterFile << ci1->getValues().at(i) << " "
          //                          << this->c1->getValues().at(i) << endl;
          //
          //      else
          //        msg << "\nProblem saving file\n";

          vector<double> cXATmp = ci1->getValues();
          vector<double> caTmp = this->c1->getValues();
          vector<double> cXBTmp = ci2->getValues();

          this->interpolation->interpolate(cXATmp,
                                           caTmp);  // interpolation of col A

          this->c1->clearValues();
          this->c1->resizeValues(this->c2->getValues().size());

          for (size_type i = 0; i < ci2->getValues().size(); ++i) {
            this->c1->setValue(
                i, this->interpolation->getValue(ci2->getValues().at(i)));
          }

          //      if (afterInterFile)
          //        for(size_type i = 0; i < ci2->getValues().size(); ++i)
          //          afterInterFile << ci2->getValues().at(i) << " "
          //                         << this->c1->getValues().at(i) << endl;
          //      else
          //        msg << "\nProblem saving file\n";
          this->msgLog.append(msg.str());
          //      beforeInterFile.close();
          //      afterInterFile.close();
        } else if (this->c2->getValues() < this->c1->getValues() &&
                   !(this->allowLessResults)) {
          msg << "\nResults have less values than reference.\n";
          this->msgLog.append(msg.str());
          raise(msg.str());
        }
      }  // else if (this->interpolation->isConform())
    }

  }  // end of namespace check

}  // end of namespace tfel
