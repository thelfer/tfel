/*!
 * \file   CxxTokenizerOffsetTest.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   24 août 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<tuple>
#include<vector>
#include<cstdlib>
#include<iostream>
#include<stdexcept>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

struct CxxTokenizerOffsetTest final
  : public tfel::tests::TestCase
{
  CxxTokenizerOffsetTest()
    : tfel::tests::TestCase("TFEL/Utilities","CxxTokenizerOffsetTest")
  {} // end of MyTest
  tfel::tests::TestResult execute() override{
    this->check("  void",{{"void",2u,Token::Standard}});
    this->check(R"("my test" //toto   "second test")",
    		{{R"("my test")",0u,Token::String},
    		 {R"(toto   "second test")",12u,Token::Comment}});
    this->check(R"("my test" /*toto*/ "second test")",
    		{{R"("my test")",0u,Token::String},
    		 {R"(toto)",12u,Token::Comment},
    		 {R"("second test")",19u,Token::String}});
    this->check("void readBlock(std::vector<std::string>& b,\n"
    		"               const_iterator& p,\n"
    		"               const const_iterator pe)",
    		{{"void",0u,Token::Standard},
    		 {"readBlock",5u,Token::Standard},
    		 {"(",14u,Token::Standard},
    		 {"std",15u,Token::Standard},
    		 {"::",18u,Token::Standard},
    		 {"vector",20u,Token::Standard},
    		 {"<",26u,Token::Standard},
    		 {"std",27u,Token::Standard},
    		 {"::",30u,Token::Standard},
    		 {"string",32u,Token::Standard},
    		 {">",38u,Token::Standard},
    		 {"&",39u,Token::Standard},
    		 {"b",41u,Token::Standard},
    		 {",",42u,Token::Standard},
    		 {"const_iterator",15u,Token::Standard},
    		 {"&",29u,Token::Standard},
    		 {"p",31u,Token::Standard},
    		 {",",32u,Token::Standard},
    		 {"const",15u,Token::Standard},	
    		 {"const_iterator",21u,Token::Standard},
    		 {"pe",36u,Token::Standard},
    		 {")",38u,Token::Standard}});
    return this->result;
  } // end of execute()
 private:
  //! a simple alias
  using size_type = tfel::utilities::Token::size_type;
  using Token = tfel::utilities::Token;
  using flags = Token::TokenFlag;
  /*!
   * \param[in] s:  string to be parsed
   * \param[in] et: expected tokens and offset
   */
  void check(const char * const s,
	     const std::vector<std::tuple<std::string,size_type,flags>>& et){
    tfel::utilities::CxxTokenizer tokenizer;
    tokenizer.parseString(s);
    TFEL_TESTS_ASSERT(tokenizer.size()==et.size());
    if(tokenizer.size()!=et.size()){
      return;
    }
    auto pt  = tokenizer.begin();
    auto pet = et.begin();
    while(pt!=tokenizer.end()){
      TFEL_TESTS_ASSERT(pt->value==std::get<0>(*pet));
      TFEL_TESTS_ASSERT(pt->offset==std::get<1>(*pet));
      TFEL_TESTS_ASSERT(pt->flag==std::get<2>(*pet));
      ++pt;
      ++pet;
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerOffsetTest,
			  "CxxTokenizerOffset");

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CxxTokenizer.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
