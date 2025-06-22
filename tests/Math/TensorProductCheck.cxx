/*!
 * \file  tests/Math/TensorProductCheck.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 16 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/st2tot2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"

template <unsigned short N>
struct TensorProductCheck final : public tfel::tests::TestCase {
  TensorProductCheck()
      : tfel::tests::TestCase("TFEL/Math", "TensorProductCheck") {
  }  // end of TensorProductCheck
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double values1[] = {
        0.08302032231654488,  0.5181596568814956,    0.006263984638196551,
        0.5365285235863536,   0.0021413062196622423, 0.4270927330213501,
        0.3110170556277837,   0.7509628766804228,    0.9854039107001845,
        0.4721305501776071,   0.09930390806279887,   0.2669966197084469,
        0.033078235345435636, 0.9533648983310657,    0.9239164792710177,
        0.6832587741777069,   0.8151251326491792,    0.02936511483292048,
        0.35636910363511987,  0.007587022652430386,  0.8680934206770248,
        0.1907135148032405,   0.5892653800856216,    0.7429271293358384,
        0.13265866162438145,  0.34483979753267546,   0.7573014793686119,
        0.17720288304735032,  0.7457915507475924,    0.4897451182960191,
        0.3665741905561034,   0.7590854727462344,    0.2349306726468402,
        0.7889411016787455,   0.807318457050991,     0.7238017330118321,
        0.4444575778956925,   0.9966749380807296,    0.12900087091803603,
        0.3534060386305171,   0.7972788268614268,    0.03199701743931771,
        0.4764007900564017,   0.6975418331150337,    0.19771844505482228,
        0.3488230144458574,   0.9726184448247054,    0.6097105347904597,
        0.42327578421506085,  0.5293649723354253,    0.26244224443918296,
        0.7755559672515373,   0.7433920754697674,    0.9286504749590663,
        0.6292562358570254,   0.7814515671794035,    0.3422952597501965,
        0.3522032139834862,   0.09849641808733434,   0.8489213794094593,
        0.8583870135802967,   0.8086192818265622,    0.4824014703705718,
        0.6162902116495417,   0.9590547964728655,    0.08084288066991208,
        0.14622865838623889,  0.7740346450702899,    0.9810620400137333,
        0.8289080242710049,   0.920700036972557,     0.6394112046711389,
        0.23845993484927852,  0.8588182519574407,    0.09759834568323555,
        0.8219655560259856,   0.31623882070757614,   0.26925900927551216,
        0.5793984254589885,   0.3868720893855282,    0.9425806412742042};
    const double values2[] = {
        0.34377959815555115,  0.35405603868715907,   0.34803761968529245,
        0.4250480304491224,   0.6247908159154605,    0.50877708123782,
        0.024456206409153358, 0.27418095580794877,   0.39853705992132193,
        0.8682710479471252,   0.4809618573933587,    0.8815882998492386,
        0.7331233372044301,   0.8415120998800362,    0.12932642096836544,
        0.32901587740635463,  0.09030676640358382,   0.9464950000669002,
        0.9025362835108338,   0.47877492420310186,   0.9506221507106539,
        0.8360036952970094,   0.7700327434780886,    0.6647928798268372,
        0.5583897931040324,   0.6014845978506491,    0.22184642917876518,
        0.7778675714209037,   0.7794356899993372,    0.2345586852342506,
        0.43327445878076243,  0.4776669918735924,    0.16922784753958675,
        0.8499506933717526,   0.6525969904067269,    0.6996474572904141,
        0.34298472956602344,  0.569937863290134,     0.9356229318238083,
        0.9905261357587236,   0.961672094075648,     0.7533918956750197,
        0.533295065733455,    0.9382180054644846,    0.3524055314726311,
        0.38911618228482425,  0.125381281615632,     0.8782134194387512,
        0.7938874550603823,   0.9173442769025875,    0.1377953890101855,
        0.6550662996389561,   0.9507769638713305,    0.1403606210216961,
        0.7826701823407461,   0.6873242436024496,    0.8068679785183894,
        0.8979953434434187,   0.7546479441641072,    0.28414905481611163,
        0.5044811252526411,   0.9329575610820637,    0.6895996756858518,
        0.24979071807257502,  0.0563532304240536,    0.8895809990668933,
        0.5248723514575786,   0.4711492718328274,    0.25986275772164147,
        0.08707958051317732,  0.9805233740977812,    0.9190857887459923,
        0.34154099478907485,  0.24787612066857634,   0.2905513104069153,
        0.187509866673372,    0.0023550955528973683, 0.733521199422858,
        0.20811005281104267,  0.18344092535290435,   0.19940469230665936};
    const double values3[] = {
        0.15461974105342813, 0.13924644516783613, 0.43117665944930983,
        0.07202701278173496, 0.8054329144110002,  0.36336467442067855,
        0.1715059351414837,  0.9239779569444945,  0.8447301031948172};
    const double values4[] = {
        0.9729554328337576, 0.5188383738972479,  0.589053493642629,
        0.9549030317226025, 0.31308843322319646, 0.6868578117931654,
        0.5422770128505602, 0.4381973611955209,  0.5943223452967757};
    st2tost2<N, double> sf1;
    copy(values1, values1 + sf1.size(), sf1.begin());
    st2tost2<N, double> sf2;
    copy(values2, values2 + sf2.size(), sf2.begin());
    t2tot2<N, double> sf3;
    copy(values1, values1 + sf3.size(), sf3.begin());
    t2tot2<N, double> sf4;
    copy(values2, values2 + sf4.size(), sf4.begin());
    t2tost2<N, double> sf5;
    copy(values1, values1 + sf5.size(), sf5.begin());
    st2tot2<N, double> sf6;
    copy(values2, values2 + sf6.size(), sf6.begin());
    const tensor<N, double> s1(values3);
    const stensor<N, double> s2(values4);
    /* Matrix-Vector operations */
    checkMatrixVectorProduct(sf1, s2);
    checkMatrixVectorProduct(sf2, s2);
    checkMatrixVectorProduct(sf3, s1);
    checkMatrixVectorProduct(sf4, s1);
    checkMatrixVectorProduct(sf5, s1);
    checkMatrixVectorProduct(sf6, s2);
    /* Vector-Matrix operations -operator |) */
    checkVectorMatrixProduct(s2, sf1);
    checkVectorMatrixProduct(s2, sf2);
    checkVectorMatrixProduct(s1, sf3);
    checkVectorMatrixProduct(s1, sf4);
    checkVectorMatrixProduct(s2, sf5);
    checkVectorMatrixProduct(s1, sf6);
    /* Matrix-Matrix operations */
    checkMatrixMatrixProduct(sf1, sf2);
    checkMatrixMatrixProduct(sf3, sf4);
    checkMatrixMatrixProduct(sf5, sf6);
    checkMatrixMatrixProduct(sf6, sf5);
    checkMatrixMatrixProduct(sf4, sf6);
    checkMatrixMatrixProduct(sf6, sf1);
    checkMatrixMatrixProduct(sf1, sf5);
    checkMatrixMatrixProduct(sf5, sf3);
    /* Diadic Product */
    checkDiadicProduct(s1, s1);
    checkDiadicProduct(s2, s2);
    checkDiadicProduct(s1, s2);
    checkDiadicProduct(s2, s1);
    return this->result;
  }  // end of execute
 private:
  unsigned short getSize(const tfel::math::stensor<N, double>&) {
    return tfel::math::StensorDimeToSize<N>::value;
  }
  unsigned short getSize(const tfel::math::tensor<N, double>&) {
    return tfel::math::TensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfRows(const tfel::math::st2tost2<N, double>&) {
    return tfel::math::StensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfRows(const tfel::math::st2tot2<N, double>&) {
    return tfel::math::TensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfRows(const tfel::math::t2tost2<N, double>&) {
    return tfel::math::StensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfRows(const tfel::math::t2tot2<N, double>&) {
    return tfel::math::TensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfColumns(const tfel::math::st2tost2<N, double>&) {
    return tfel::math::StensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfColumns(const tfel::math::st2tot2<N, double>&) {
    return tfel::math::StensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfColumns(const tfel::math::t2tost2<N, double>&) {
    return tfel::math::TensorDimeToSize<N>::value;
  }
  unsigned short getNumberOfColumns(const tfel::math::t2tot2<N, double>&) {
    return tfel::math::TensorDimeToSize<N>::value;
  }
  template <typename MatrixType, typename MatrixType2>
  void checkMatrixMatrixProduct(const MatrixType& m, const MatrixType2& m2) {
    using namespace tfel::math;
    using std::vector;
    typedef typename ResultType<MatrixType, MatrixType2, OpMult>::type Result;
    const Result r = m * m2;
    vector<double> nr(getNumberOfRows(m) * getNumberOfColumns(m2));
    for (unsigned short i = 0; i != getNumberOfRows(m); ++i) {
      for (unsigned short j = 0; j != getNumberOfColumns(m2); ++j) {
        double v = 0;
        for (unsigned short k = 0; k != getNumberOfRows(m2); ++k) {
          v += m(i, k) * m2(k, j);
        }
        nr[i * getNumberOfColumns(m2) + j] = v;
      }
    }
    TFEL_TESTS_CHECK_EQUAL(r.size(), nr.size());
    for (unsigned short i = 0; i != getNumberOfRows(m); ++i) {
      for (unsigned short j = 0; j != getNumberOfColumns(m2); ++j) {
        const double v = nr[i * getNumberOfColumns(m2) + j];
        TFEL_TESTS_ASSERT(abs(v - r(i, j)) < 1.e-14);
      }
    }
  }
  template <typename MatrixType, typename VectorType>
  void checkMatrixVectorProduct(const MatrixType& m, const VectorType& v) {
    using namespace tfel::math;
    using std::vector;
    using Result = typename ResultType<MatrixType, VectorType, OpMult>::type;
    const Result r = m * v;
    vector<double> nr(r.size());
    for (unsigned short i = 0; i != nr.size(); ++i) {
      nr[i] = 0.;
      for (unsigned short j = 0; j != v.size(); ++j) {
        nr[i] += m(i, j) * v(j);
      }
    }
    for (unsigned short i = 0; i != nr.size(); ++i) {
      TFEL_TESTS_ASSERT(abs(nr[i] - r[i]) < 1.e-14);
    }
  }
  template <typename VectorType, typename MatrixType>
  void checkVectorMatrixProduct(const VectorType& v, const MatrixType& m) {
    using namespace std;
    using namespace tfel::math;
    using std::vector;
    typedef typename ResultType<VectorType, MatrixType, OpMult>::type Result;
    const Result r = v | m;
    vector<double> nr(r.size());
    for (unsigned short i = 0; i != nr.size(); ++i) {
      nr[i] = 0.;
      for (unsigned short j = 0; j != v.size(); ++j) {
        nr[i] += v(j) * m(j, i);
      }
    }
    for (unsigned short i = 0; i != nr.size(); ++i) {
      TFEL_TESTS_ASSERT(abs(nr[i] - r[i]) < 1.e-14);
    }
  }
  template <typename TensorType, typename TensorType2>
  void checkDiadicProduct(const TensorType& t1, const TensorType2& t2) {
    using namespace std;
    using namespace tfel::math;
    using std::vector;
    typedef typename ResultType<TensorType, TensorType2, OpDiadicProduct>::type
        Result;
    const Result r = t1 ^ t2;
    vector<double> nr(getSize(t1) * getSize(t2));
    for (unsigned short i = 0; i != getSize(t1); ++i) {
      for (unsigned short j = 0; j != getSize(t2); ++j) {
        nr[i * getSize(t2) + j] = t1[i] * t2[j];
      }
    }
    TFEL_TESTS_CHECK_EQUAL(r.size(), nr.size());
    for (unsigned short i = 0; i != getSize(t1); ++i) {
      for (unsigned short j = 0; j != getSize(t2); ++j) {
        const double v = nr[i * getSize(t2) + j];
        TFEL_TESTS_ASSERT(abs(v - r(i, j)) < 1.e-14);
      }
    }
  }
};

using TensorProductCheck1D = TensorProductCheck<1U>;
using TensorProductCheck2D = TensorProductCheck<2U>;
using TensorProductCheck3D = TensorProductCheck<3U>;
TFEL_TESTS_GENERATE_PROXY(TensorProductCheck1D, "TensorProductCheck1D");
TFEL_TESTS_GENERATE_PROXY(TensorProductCheck2D, "TensorProductCheck2D");
TFEL_TESTS_GENERATE_PROXY(TensorProductCheck3D, "TensorProductCheck3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ResultOf.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
