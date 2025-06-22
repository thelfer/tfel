/*!
 * \file   mfront/include/MFront/ModelDSLBase.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMODELPARSERBASE_IXX
#define LIB_MFRONTMODELPARSERBASE_IXX

#include <sstream>
#include <stdexcept>
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  template <typename Child>
  void ModelDSLBase<Child>::registerDefaultCallBacks() {
    this->registerNewCallBack(";", &Child::treatLonelySeparator);
    this->registerNewCallBack("@DSL", &Child::treatDSL);
    this->registerNewCallBack("@Parser", &Child::treatDSL);
    this->registerNewCallBack("@Model", &Child::treatModel);
    this->registerNewCallBack("@Material", &Child::treatMaterial);
    this->registerNewCallBack("@Author", &Child::treatAuthor);
    this->registerNewCallBack("@Date", &Child::treatDate);
    this->registerNewCallBack("@UnitSystem", &Child::treatUnitSystem);
    this->registerNewCallBack("@UseQt", &Child::treatUseQt);
    this->registerNewCallBack("@Includes", &Child::treatIncludes);
    this->registerNewCallBack("@StaticVar", &Child::treatStaticVar);
    this->registerNewCallBack("@StaticVariable", &Child::treatStaticVar);
    this->registerNewCallBack("@Description", &Child::treatDescription);
    this->registerNewCallBack("@Import", &Child::treatImport);
    this->registerNewCallBack("@Bounds", &Child::treatBounds);
    this->registerNewCallBack("@PhysicalBounds", &Child::treatPhysicalBounds);
    this->registerNewCallBack("@ConstantMaterialProperty",
                              &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Coef", &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@MaterialProperty",
                              &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Parameter", &Child::treatParameter);
    this->registerNewCallBack("@LocalParameter", &Child::treatLocalParameter);
    this->registerNewCallBack("@ConstantMaterialProperty",
                              &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Domain", &Child::treatDomain);
    this->registerNewCallBack("@Domains", &Child::treatDomains);
    this->registerNewCallBack("@Material", &Child::treatMaterial);
    this->registerNewCallBack("@Output", &Child::treatOutput);
    this->registerNewCallBack("@StateVariable", &Child::treatOutput);
    this->registerNewCallBack("@Input", &Child::treatInput);
    this->registerNewCallBack("@ExternalStateVariable", &Child::treatInput);
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
      const std::string& fn,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->fd.fileName = fn;
    this->openFile(this->fd.fileName, ecmds, s);
    this->analyse();
  }  // end of ModelDSLBase<Child>::importFile

  template <typename Child>
  void ModelDSLBase<Child>::analyseString(const std::string& s) {
    this->fd.fileName = "user defined string";
    this->parseString(s);
    this->analyse();
    // Adding some stuff
    this->endsInputFileProcessing();
  }  // end of ModelDSLBase<Child>::analyseString

  template <typename Child>
  void ModelDSLBase<Child>::analyse() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "ModelDSLBase<Child>::analyse: "
                     << "begin of analysis of file '" << this->fd.fileName
                     << "'\n";
    }
    MemberFuncPtr handler = nullptr;
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->tokens.begin();
    while (this->current != this->tokens.end()) {
      const auto c = tfel::unicode::getMangledString(this->current->value);
      const auto p = this->callBacks.find(c);
      if (p == this->callBacks.end()) {
        if (this->md.outputs.contains(c)) {
          this->currentVar = c;
          handler = &Child::treatOutputMethod;
        } else if (this->md.inputs.contains(c)) {
          this->currentVar = c;
          handler = &Child::treatInputMethod;
        } else if (this->md.parameters.contains(c)) {
          this->currentVar = c;
          handler = &Child::treatParameterMethod;
        } else if (this->md.constantMaterialProperties.contains(c)) {
          this->currentVar = c;
          handler = &Child::treatConstantMaterialPropertyMethod;
        } else {
          handler = &Child::treatUnknownKeyword;
        }
      } else {
        if (getVerboseMode() >= VERBOSE_LEVEL2) {
          getLogStream() << "ModelDSLBase<Child>::analyse: "
                         << "treating keywork '" << c << "'\n";
        }
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
                     << "end of analysis of file '" << this->fd.fileName
                     << "'\n";
    }
  }  // end of ModelDSLBase<Child>::analyse

  template <typename Child>
  void ModelDSLBase<Child>::analyseFile(
      const std::string& fileName_,
      const std::vector<std::string>& ecmds,
      const std::map<std::string, std::string>& s) {
    this->importFile(fileName_, ecmds, s);
    // Adding some stuff
    this->endsInputFileProcessing();
    //
    for (const auto& i : this->interfaces) {
      i.second->getTargetsDescription(this->td, md);
    }
    this->completeTargetsDescription();
  }

  template <typename Child>
  void ModelDSLBase<Child>::registerNewCallBack(const std::string& keyword,
                                                const MemberFuncPtr f) {
    this->callBacks.insert({keyword, f});
    this->registredKeyWords.insert(keyword);
  }  // end of registerNewCallBack

  template <typename Child>
  ModelDSLBase<Child>::ModelDSLBase(const DSLOptions& opts)
      : ModelDSLCommon(opts) {
    this->registerDefaultCallBacks();
  }

}  // end of namespace mfront

#endif /* LIB_MFRONTMODELPARSERBASE_IXX */
