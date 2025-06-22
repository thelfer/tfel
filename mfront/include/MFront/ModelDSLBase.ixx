/*!
 * \file   mfront/include/MFront/ModelDSLBase.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMODELPARSERBASE_IXX_
#define LIB_MFRONTMODELPARSERBASE_IXX_

#include <sstream>
#include <stdexcept>
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  template <typename Child>
  void ModelDSLBase<Child>::registerDefaultCallBacks(void) {
    this->registerNewCallBack(";", &Child::treatLonelySeparator);
    this->registerNewCallBack("@DSL", &Child::treatParser);
    this->registerNewCallBack("@Parser", &Child::treatParser);
    this->registerNewCallBack("@Model", &Child::treatModel);
    this->registerNewCallBack("@Material", &Child::treatMaterial);
    this->registerNewCallBack("@Author", &Child::treatAuthor);
    this->registerNewCallBack("@Date", &Child::treatDate);
    this->registerNewCallBack("@Includes", &Child::treatIncludes);
    this->registerNewCallBack("@StaticVar", &Child::treatStaticVar);
    this->registerNewCallBack("@StaticVariable", &Child::treatStaticVar);
    this->registerNewCallBack("@Description", &Child::treatDescription);
    this->registerNewCallBack("@Import", &Child::treatImport);
    this->registerNewCallBack("@Bounds", &Child::treatBounds);
    this->registerNewCallBack("@PhysicalBounds", &Child::treatPhysicalBounds);
    this->registerNewCallBack("@ConstantMaterialProperty",
                              &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Parameter", &Child::treatParameter);
    this->registerNewCallBack("@LocalParameter", &Child::treatLocalParameter);
    this->registerNewCallBack("@ConstantMaterialProperty",
                              &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Domain", &Child::treatDomain);
    this->registerNewCallBack("@Domains", &Child::treatDomains);
    this->registerNewCallBack("@Material", &Child::treatMaterial);
    this->registerNewCallBack("@Output", &Child::treatOutput);
    this->registerNewCallBack("@Input", &Child::treatInput);
    this->registerNewCallBack("@Function", &Child::treatFunction);
    this->registerNewCallBack("@MaterialLaw", &Child::treatMaterialLaw);
  }  // end of ModelDSLBase<Child>::registerDefaultCallBacks()

  template <typename Child>
  void ModelDSLBase<Child>::getKeywordsList(std::vector<std::string>& k) const {
    for (auto c : this->callBacks) {
      k.push_back(c.first);
    }
  }  // end of ModelDSLBase<Child>::getKeywordsList

  template <typename Child>
  void ModelDSLBase<Child>::importFile(
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->fileName = fileName_;
    this->openFile(this->fileName, ecmds, s);
    this->analyse();
  }  // end of ModelDSLBase<Child>::importFile

  template <typename Child>
  void ModelDSLBase<Child>::analyseString(const std::string& s) {
    this->fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  }  // end of ModelDSLBase<Child>::analyseString

  template <typename Child>
  void ModelDSLBase<Child>::analyse() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "ModelDSLBase<Child>::analyse: "
                     << "begin of analysis of file '" << this->fileName
                     << "'\n";
    }
    MemberFuncPtr handler = nullptr;
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->tokens.begin();
    while (this->current != this->tokens.end()) {
      auto p = this->callBacks.find(this->current->value);
      if (p == this->callBacks.end()) {
        if (this->outputs.contains(this->current->value)) {
          this->currentVar = this->current->value;
          handler = &Child::treatOutputMethod;
        } else if (this->inputs.contains(this->current->value)) {
          this->currentVar = this->current->value;
          handler = &Child::treatInputMethod;
        } else if (this->parameters.contains(this->current->value)) {
          this->currentVar = this->current->value;
          handler = &Child::treatParameterMethod;
        } else if (this->constantMaterialProperties.contains(
                       this->current->value)) {
          this->currentVar = this->current->value;
          handler = &Child::treatConstantMaterialPropertyMethod;
        } else {
          handler = &Child::treatUnknownKeyword;
        }
      } else {
        handler = p->second;
      }
      this->currentComment = this->current->comment;
      ++(this->current);
      try {
        (static_cast<Child*>(this)->*handler)();
      } catch (...) {
        this->currentComment.clear();
        throw;
      }
      this->currentComment.clear();
    }
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "ModelDSLBase<Child>::analyse: "
                     << "end of analysis of file '" << this->fileName << "'\n";
    }
  }  // end of ModelDSLBase<Child>::analyse

  template <typename Child>
  void ModelDSLBase<Child>::analyseFile(
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->importFile(fileName_, ecmds, s);
    for (const auto& i : this->interfaces) {
      i.second->getTargetsDescription(this->td, *this);
    }
    this->completeTargetsDescription();
  }

  template <typename Child>
  void ModelDSLBase<Child>::registerNewCallBack(const std::string& keyword,
                                                const MemberFuncPtr f) {
    using namespace std;
    this->callBacks.insert(make_pair(keyword, f));
    this->registredKeyWords.insert(keyword);
  }  // end of registerNewCallBack

  template <typename Child>
  ModelDSLBase<Child>::ModelDSLBase() {
    this->registerDefaultCallBacks();
  }

}  // end of namespace mfront

#endif /* LIB_MFRONTMODELPARSERBASE_IXX_ */
