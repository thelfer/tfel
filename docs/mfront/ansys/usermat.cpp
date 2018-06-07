#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#include <windows.h>
#else /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
#include <dlfcn.h>
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

static std::vector<std::string> tokenize(const std::string &s, const char c) {
  std::vector<std::string> res;
  std::string::size_type b = 0u;
  std::string::size_type e = s.find_first_of(c, b);
  while (std::string::npos != e || std::string::npos != b) {
    // Found a token, add it to the vector.
    res.push_back(s.substr(b, e - b));
    b = s.find_first_not_of(c, e);
    e = s.find_first_of(c, b);
  }
  return res;
}  // end of tokenize

static std::string strim_front(const std::string &s) {
  std::string::size_type b = 0u;
  std::string::size_type e = s.size();
  while (b != e) {
    if (!std::isspace(s[b])) {
      return s.substr(b);
    }
    ++b;
  }
  return "";
}

static std::string strim_back(const std::string &s) {
  std::string::size_type b = 0u;
  std::string::size_type e = s.size();
  while (b != e) {
    if (std::isspace(s[b])) {
      return s.substr(0, b);
    }
    ++b;
  }
  return s;
}

static std::string strim(const std::string &s) {
  return strim_front(strim_back(s));
}  // end of tokenize

static std::vector<std::string> strim(const std::vector<std::string> &v) {
  auto r = std::vector<std::string>{};
  r.reserve(v.size());
  for (const auto &s : v) {
    auto ss = strim(s);
    if (!ss.empty()) {
      r.push_back(std::move(ss));
    }
  }
  return r;
}  // end of tokenize

static void usermat_exit(const std::string &m) {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  MessageBox(nullptr, m.c_str(), "mfront", 0);
#else /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  std::cerr << m << '\n';
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  std::exit(-1);
} // end of usermat_exit

namespace ansys {

  /*!
   * structure in charge of associating a MFront behaviour to a
   * material by reading the `mfront-usermat.dat` file in the current
   * directory.
   */
  struct UserMaterialManager {
    //! a simple alias
    using AnsysIntegerType = int;
    //! a simple alias
    using AnsysRealType = double;
    //! a simple alias
    using AnsysFctPtr = void (*)(const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysIntegerType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 AnsysRealType *const,
                                 AnsysRealType *const,
                                 AnsysRealType *const,
                                 AnsysRealType *const,
                                 AnsysRealType *const,
                                 AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const,
                                 const AnsysRealType *const);
    //! default constructor
    UserMaterialManager();
    /*!
     * \return the function associated to a material identifier
     * \param[in] id: material identifier
     */
    AnsysFctPtr getBehaviour(const AnsysIntegerType) const;
    //! destructor
    ~UserMaterialManager();

   private:
//! a simple alias
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    using libhandler = HINSTANCE__ *;
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    using libhandler = void *;
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
       /*!
        * \brief internal data structure storing pointers to MFront
        * behaviour and the associated material identifier
        */
    struct UserMaterialHander {
      //! material identifier
      const int id;
      //! pointer to the MFront behaviour
      const AnsysFctPtr f;
    };  // end of struct UserMaterialHander
    //! internal data structure storing pointers to external libraries
    struct ExternalLibraryHandler {
      //! library name
      const std::string name;
      //! library handler
      const libhandler l;
    };  // end of struct ExternalLibraryHandler
    //! list of all registred behaviours
    std::vector<UserMaterialHander> handlers;
    //! list of all loaded library
    std::vector<ExternalLibraryHandler> libraries;
    /* deleted methods */
    UserMaterialManager(UserMaterialManager &&) = delete;
    UserMaterialManager(const UserMaterialManager &) = delete;
    UserMaterialManager &operator=(UserMaterialManager &&) = delete;
    UserMaterialManager &operator=(const UserMaterialManager &) = delete;
  };  // end of struct UserMaterialManager

  UserMaterialManager::UserMaterialManager() {
    auto find_library = [this](const std::string &n) {
      return std::find_if(
          this->libraries.begin(), this->libraries.end(),
          [&n](const ExternalLibraryHandler &l) { return l.name == n; });
    };
    auto exit_if = [](const bool b, const std::string &m) {
      if (b) {
        usermat_exit("UserMaterialManager::UserMaterialManager: " + m);
      }
    };
    auto open_library = [](const std::string &lib) -> libhandler {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      for (const auto prefix : {"", "lib"}) {
        for (const auto suffix : {"", ".dll"}) {
          const auto library = prefix + lib + suffix;
          const auto l = ::LoadLibrary(TEXT(library.c_str()));
          if (l != nullptr) {
            return l;
          }
        }
      }
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      for (const auto prefix : {"", "lib"}) {
        for (const auto suffix : {"", ".so", ".dylib"}) {
          const auto library = prefix + lib + suffix;
          const auto l = ::dlopen(library.c_str(), RTLD_NOW);
          if (l != nullptr) {
            return l;
          }
        }
      }
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      return nullptr;
    };
    std::ifstream in("mfront-usermat.dat");
    exit_if(!in, "unable to open file 'mfront-usermat.dat'");
    std::string line;
    size_t ln = 1u;
    while (getline(in, line)) {
      auto tokens = strim(tokenize(line, ','));
      if ((tokens.empty()) || (tokens[0] == "/com")) {
        continue;
      }
      exit_if(tokens[0] != "tb", "invalid line");
      exit_if(tokens.size() < 2, "invalid line '" + std::to_string(ln) +
                                     "' in file 'mfront-usermat.dat'");
      exit_if(tokens[1] != "mfront", "invalid line '" + std::to_string(ln) +
                                         "' in file 'mfront-usermat.dat'");
      exit_if(tokens.size() != 5, "invalid line '" + std::to_string(ln) +
                                      "' in file 'mfront-usermat.dat'");
      const auto id = std::stoi(tokens[2]);
      const auto lib = tokens[3];
      const auto fct = tokens[4];
      std::cout << "UserMaterialManager::UserMaterialManager: "
                << "associating material '" << id << "' to behaviour '" << fct
                << "' in library '" << lib << "'\n";
      auto ptr = find_library(lib);
      if (ptr == this->libraries.end()) {
        const auto l = open_library(lib);
        exit_if(l == nullptr, "unable to load library '" + lib + "'");
        this->libraries.push_back(ExternalLibraryHandler{lib, l});
        ptr = std::prev(this->libraries.end());
      }
      exit_if(this->getBehaviour(id) != nullptr,
              "a behaviour has already been associated to material "
              "identifier '" +
                  std::to_string(id) + "'");
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      auto f =
          reinterpret_cast<AnsysFctPtr>(::GetProcAddress(ptr->l, fct.c_str()));
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      auto f = reinterpret_cast<AnsysFctPtr>(::dlsym(ptr->l, fct.c_str()));
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      exit_if(f == nullptr,
              "could not load behaviour '" + fct + "' in library '" + lib + "'");
      this->handlers.push_back(std::move(UserMaterialHander{id, f}));
      ++ln;
    }
    exit_if(in.bad(), "error while reading file 'mfront-usermat.dat'");
    in.close();
  }  // end of UserMaterialManager::UserMaterialManager

  UserMaterialManager::AnsysFctPtr UserMaterialManager::getBehaviour(
      const AnsysIntegerType id) const {
    for (const auto &h : this->handlers) {
      if (h.id == id) {
        return h.f;
      }
    }
    return nullptr;
  }

  UserMaterialManager::~UserMaterialManager() {
    for (auto &l : this->libraries) {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      ::FreeLibrary(l.l);
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      ::dlclose(l.l);
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    }
  }  // end of UserMaterialManager::~UserMaterialManager

}  // end of namespace ansys

extern "C" {

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
__declspec(dllexport) void USERMAT(const int *const matId,
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
void usermat(const int *const matId,
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
                                   const int *const elemId,
                                   const int *const kDomIntPt,
                                   const int *const kLayer,
                                   const int *const kSectPt,
                                   const int *const ldsetp,
                                   const int *const isubst,
                                   int *const keycut,
                                   const int *const nDirect,
                                   const int *const nShear,
                                   const int *const ncomp,
                                   const int *const nSatev,
                                   const int *const nProp,
                                   const double *const Time,
                                   const double *const dTime,
                                   const double *const Temp,
                                   const double *const dTemp,
                                   double *const stress,
                                   double *const statev,
                                   double *const dsdePl,
                                   double *const sedEl,
                                   double *const sedPl,
                                   double *const epseq,
                                   const double *const Strain,
                                   const double *const dStrain,
                                   double *const epsPl,
                                   const double *const prop,
                                   const double *const coords,
                                   const double *const rotateM,
                                   const double *const defGrad_t,
                                   const double *const defGrad,
                                   const double *const tsstif,
                                   double *const epsZZ,
                                   const double *const var1,
                                   const double *const var2,
                                   const double *const var3,
                                   const double *const var4,
                                   const double *const var5,
                                   const double *const var6,
                                   const double *const var7,
                                   const double *const var8) {
  static ansys::UserMaterialManager m;
  auto f = m.getBehaviour(*matId);
  if(f == nullptr){
    usermat_exit("usermat: no behaviour associated to material id '" +
                 std::to_string(*matId) + "'\n");
  }
  f(matId, elemId, kDomIntPt, kLayer, kSectPt, ldsetp, isubst, keycut, nDirect,
    nShear, ncomp, nSatev, nProp, Time, dTime, Temp, dTemp, stress, statev,
    dsdePl, sedEl, sedPl, epseq, Strain, dStrain, epsPl, prop, coords, rotateM,
    defGrad_t, defGrad, tsstif, epsZZ, var1, var2, var3, var4, var5, var6, var7,
    var8);
}  // end of usermat

}  // end of extern "C"
