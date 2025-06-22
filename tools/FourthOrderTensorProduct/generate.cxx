/*!
 * \file   tools/FourthOrderTensorProduct/generate.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  16 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <locale>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

enum TensorType {
  T2,
  ST2,
};

// first  : argument type
// second : result type
typedef std::pair<TensorType, TensorType> FourthOrderTensor;
typedef std::pair<FourthOrderTensor, FourthOrderTensor>
    FourthOrderTensorProduct;

static std::string getDimeToSizeMetaFunction(const TensorType& t) {
  if (t == T2) {
    return "TensorDimeToSize";
  }
  return "StensorDimeToSize";
}

static std::string getRowDimeToSizeMetaFunction(const FourthOrderTensor& t) {
  return getDimeToSizeMetaFunction(t.second);
}

static std::string getColumnDimeToSizeMetaFunction(const FourthOrderTensor& t) {
  return getDimeToSizeMetaFunction(t.first);
}

static unsigned short getSize(const TensorType& t, const unsigned short d) {
  if (t == T2) {
    if (d == 1u) {
      return 3u;
    }
    if (d == 2u) {
      return 5u;
    }
    return 9u;
  }
  if (d == 1u) {
    return 3u;
  }
  if (d == 2u) {
    return 4u;
  }
  return 6u;
}

static unsigned short getRowSize(const FourthOrderTensor& r,
                                 const unsigned short d) {
  return getSize(r.second, d);
}

static unsigned short getColumnSize(const FourthOrderTensor& r,
                                    const unsigned short d) {
  return getSize(r.first, d);
}

static std::string toupper(const std::string& s) {
  using namespace std;
  locale loc;
  string r;
  r.resize(s.size());
  transform(s.begin(), s.end(), r.begin(),
            [&loc](const char c) { return toupper(c, loc); });
  return r;
}

static std::string getName(const TensorType& t) {
  if (t == T2) {
    return "T2";
  }
  return "ST2";
}

static std::string getName(const FourthOrderTensor& t) {
  return getName(t.first) + "to" + getName(t.second);
}

static std::string getName(const FourthOrderTensorProduct& p) {
  return getName(p.first) + getName(p.second);
}

static FourthOrderTensor getResult(const FourthOrderTensorProduct& p) {
  using namespace std;
  const auto& t1 = p.first;
  const auto& t2 = p.second;
  return make_pair(t2.first, t1.second);
}

static bool isProductValid(const FourthOrderTensor& t1,
                           const FourthOrderTensor& t2) {
  return t2.second == t1.first;
}

void writeFileHeader(std::ostream& out, const FourthOrderTensorProduct& p) {
  using namespace std;
  const FourthOrderTensor r = getResult(p);
  out << "/*! " << endl;
  out << " * \\file  " << getName(p) << "ProductExpr.hxx" << endl;
  out << " * \\brief" << endl;
  out << " * \\author Thomas Helfer" << endl;
  out << " * \\date 16 juin 2014" << endl;
  out << " */" << endl;
  out << "" << endl;
  out << "#ifndef LIB_TFEL_MATH_" << toupper(getName(p)) << "PRODUCTEXPR_HXX"
      << endl;
  out << "#define LIB_TFEL_MATH_" << toupper(getName(p)) << "PRODUCTEXPR_HXX "
      << endl;
  out << "" << endl;
  out << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
  out << "" << endl;
  out << "#include\"TFEL/Math/General/EmptyRunTimeProperties.hxx\"" << endl;
  out << "#include\"TFEL/Math/" << getName(p.first) << "/" << getName(p.first)
      << "Concept.hxx\"" << endl;
  if (p.second != p.first) {
    out << "#include\"TFEL/Math/" << getName(p.second) << "/"
        << getName(p.second) << "Concept.hxx\"" << endl;
  }
  if ((r != p.first) && (r != p.second)) {
    out << "#include\"TFEL/Math/" << getName(r) << "/" << getName(r)
        << "Concept.hxx\"" << endl;
  }
  out << "#include\"TFEL/Math/" << getName(r) << "/" << getName(r)
      << "Expr.hxx\"" << endl;
  out << "" << endl;
  out << "namespace tfel" << endl;
  out << "{" << endl;
  out << "" << endl;
  out << "  namespace math" << endl;
  out << "  {" << endl;
  out << "" << endl;
  out << "    //! Empty structure used for partial specialisation of the"
      << endl;
  out << "    //! " << getName(r) << "Expr class" << endl;
  out << "    template<unsigned short N>" << endl;
  out << "    struct TFEL_VISIBILITY_LOCAL " << getName(p) << "ProductExpr"
      << endl;
  out << "    {}; // end of struct " << getName(p) << "ProductExpr" << endl;
  out << "" << endl;
}

void writePartialSpecialisation(std::ostream& out,
                                const FourthOrderTensorProduct& p,
                                const unsigned short d) {
  using namespace std;
  const FourthOrderTensor r = getResult(p);
  out << "/*!" << endl;
  out << " * Partial specialisation" << endl;
  out << " */" << endl;
  out << "template<typename " << getName(r) << "ResultType>" << endl;
  out << "struct TFEL_VISIBILITY_LOCAL " << getName(r) << "Expr<" << getName(r)
      << "ResultType," << getName(p) << "ProductExpr<" << d << "u> >" << endl;
  out << "  : public " << getName(r) << "Concept<" << getName(r) << "Expr<"
      << getName(r) << "ResultType," << getName(p) << "ProductExpr<" << d
      << "u> > >," << endl;
  out << "	public fsarray<" << getRowDimeToSizeMetaFunction(r) << "<"
      << getName(r) << "Traits<" << getName(r) << "ResultType>::dime>::value*"
      << endl;
  out << "		       " << getColumnDimeToSizeMetaFunction(r) << "<"
      << getName(r) << "Traits<" << getName(r) << "ResultType>::dime>::value,"
      << endl;
  out << "		       typename " << getName(r) << "Traits<"
      << getName(r) << "ResultType>::NumType>" << endl;
  out << "{" << endl;
  out << "  //! a simple alias" << endl;
  out << "  typedef EmptyRunTimeProperties RunTimeProperties;" << endl;
  out << "  //! a simple alias" << endl;
  out << "  typedef typename " << getName(r) << "Traits<" << getName(r)
      << "ResultType>::NumType value_type;" << endl;
  out << "  //! a simple check" << endl;
  out << "  TFEL_STATIC_ASSERT((" << getName(r) << "Traits<" << getName(r)
      << "ResultType>::dime==" << d << "u));" << endl;
  out << "  /*!" << endl;
  out << "   * \\param[in] a : first term of the product" << endl;
  out << "   * \\param[in] b : second term of the product" << endl;
  out << "   */" << endl;
  out << "  template<typename " << getName(p.first) << "Type," << endl;
  out << "	       typename " << getName(p.second) << "Type2>" << endl;
  out << "  TFEL_MATH_INLINE" << endl;
  out << "  " << getName(r) << "Expr(const " << getName(p.first) << "Type& a,"
      << endl;
  out << "		 const " << getName(p.second) << "Type2& b)" << endl;
  out << "  {" << endl;
  out << "	//! a simple check" << endl;
  out << "	TFEL_STATIC_ASSERT((tfel::meta::Implements<" << getName(p.first)
      << "Type," << getName(p.first) << "Concept>::cond));" << endl;
  out << "	//! a simple check" << endl;
  out << "	TFEL_STATIC_ASSERT((tfel::meta::Implements<"
      << getName(p.second) << "Type2," << getName(p.second)
      << "Concept>::cond));" << endl;
  out << "	//! a simple check" << endl;
  out << "	TFEL_STATIC_ASSERT((" << getName(p.first) << "Traits<"
      << getName(p.first) << "Type>::dime==" << d << "u));" << endl;
  out << "	//! a simple check" << endl;
  out << "	TFEL_STATIC_ASSERT((" << getName(p.second) << "Traits <"
      << getName(p.second) << "Type2>::dime==" << d << "u));" << endl;
  unsigned short o = 0;
  for (unsigned short i = 0; i != getRowSize(r, d); ++i) {
    for (unsigned short j = 0; j != getColumnSize(r, d); ++j, ++o) {
      out << "this->v[" << o << "]=";
      for (unsigned short k = 0; k != getColumnSize(p.first, d);) {
        out << "a(" << i << "," << k << ")*b(" << k << "," << j << ")";
        if (++k != getColumnSize(p.first, d)) {
          out << "+";
        }
      }
      out << ";" << endl;
    }
  }
  out << "  } // end of " << getName(r) << "Expr" << endl;
  out << "  /*!" << endl;
  out << "   * \\brief access operator" << endl;
  out << "   * \\param[in] i : line index" << endl;
  out << "   * \\param[in] j : column index" << endl;
  out << "   */" << endl;
  out << "  TFEL_MATH_INLINE const value_type&" << endl;
  out << "  operator()(const unsigned short i," << endl;
  out << "		 const unsigned short j) const" << endl;
  out << "  {" << endl;
  out << "	return this->v[i*" << getColumnSize(r, d) << "+j];" << endl;
  out << "  } // end of operator()" << endl;
  out << "  /*!" << endl;
  out << "   * \\return the runtime properties" << endl;
  out << "   */" << endl;
  out << "  TFEL_MATH_INLINE RunTimeProperties" << endl;
  out << "  getRunTimeProperties() const" << endl;
  out << "  {" << endl;
  out << "	return RunTimeProperties();" << endl;
  out << "  }" << endl;
  out << "}; // end of struct " << getName(r) << "Expr<" << getName(r)
      << "ResultType," << getName(p) << "ProductExpr>" << endl;
  out << endl;
}

void writeFileTrailer(std::ostream& out, const FourthOrderTensorProduct& p) {
  using namespace std;
  out << "} // end of namespace math" << endl;
  out << endl;
  out << "} // end of namespace tfel" << endl;
  out << endl;
  out << "#endif /* LIB_TFEL_MATH_" << toupper(getName(p)) << "PRODUCTEXPR_H */"
      << endl;
}  // end of writeFileTrailer

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  // all fouth order tensor
  const FourthOrderTensor t4[4u] = {make_pair(ST2, ST2), make_pair(ST2, T2),
                                    make_pair(T2, ST2), make_pair(T2, T2)};
  // all possible products
  vector<FourthOrderTensorProduct> products;
  for (unsigned short i = 0; i != 4; ++i) {
    for (unsigned short j = 0; j != 4; ++j) {
      if (isProductValid(t4[i], t4[j])) {
        products.push_back(make_pair(t4[i], t4[j]));
      }
    }
  }
  // writing outputs
  for (const auto& p : products) {
    cout << "treating : " << getName(p) << endl;
    string header = getName(p) + "ProductExpr.hxx";
    ofstream out(header);
    if (!out) {
      string msg("can't open file '" + header + "'");
      throw(runtime_error(msg));
    }
    writeFileHeader(out, p);
    writePartialSpecialisation(out, p, 1u);
    writePartialSpecialisation(out, p, 2u);
    writePartialSpecialisation(out, p, 3u);
    writeFileTrailer(out, p);
  }
  return EXIT_SUCCESS;
}  // end of main
