/*!
 * \file   dianafea_external_behaviour_call.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/01/2019
 */

#include <array>
#include <utility>
#include <vector>
#include <string>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#include <windows.h>
#else /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
#include <dlfcn.h>
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
// code retrieved from
// http://www.codeproject.com/Tips/479880/GetLastError-as-std-string
static std::string getLastWin32Error() {
  const DWORD error = GetLastError();
  if (error) {
    LPVOID lpMsgBuf;
    DWORD bufLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, nullptr);
    if (bufLen) {
      LPCSTR lpMsgStr = (LPTSTR)lpMsgBuf;
      std::string result(lpMsgStr, lpMsgStr + bufLen);
      LocalFree(lpMsgBuf);
      return result;
    }
  }
  return std::string();
}
#endif /*  (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

static std::string getErrorMessage() {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  return getLastWin32Error();
#else
  const auto e = ::dlerror();
  if (e != nullptr) {
    return std::string(e);
  }
  return "";
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
}  // end of  getErrorMessage

static void usrmat_log(const std::string &m) {
  static std::ofstream log("mfront-usrmat.log");
  log << m << std::endl;
  std::clog << m << std::endl;
}

static std::vector<std::string> tokenize(const std::string &s,
                                         const char c = ' ') {
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

static void usrmat_exit(const std::string &m) {
  usrmat_log(m);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  MessageBox(nullptr, m.c_str(), "mfront", 0);
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  std::exit(-1);
}  // end of usrmat_exit

namespace dianafea {

  /*!
   * \brief structure in charge of loading external behaviours
   */
  struct ExternalBehavioursHandler {
    //! a simple alias
    using fct = void (*)(double *const,
                         double *const,
                         double *const,
                         const int *const,
                         const int *const,
                         const int *const,
                         const double *const,
                         const double *const,
                         const double *const,
                         const double *const,
                         const double *const,
                         const double *const);
    //! \brief default constructor
    ExternalBehavioursHandler();
    /*!
     * \return the function pointer associated with the given identifier
     * \param[in] id: identifer
     */
    fct get(const char *const) const;
    //! \brief destructor
    ~ExternalBehavioursHandler();

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
    struct BehaviourHandler {
      //! material identifier
      const std::array<char, 6> id;
      //! pointer to the MFront behaviour
      const fct f;
    };  // end of struct BehaviourHandler
    //! internal data structure storing pointers to external libraries
    struct ExternalLibraryHandler {
      //! library name
      const std::string name;
      //! library handler
      const libhandler l;
    };  // end of struct ExternalLibraryHandler
    /*!
     * \return the function pointer associated with the given identifier if
     * declared. nullptr is returned otherwise.
     * \param[in] id: identifer
     */
    fct find(const char *const) const;
    //! \brief list of all loaded behaviours and associated identifiers
    std::vector<BehaviourHandler> behaviours;
    //! \brief list of loaded libraries
    std::vector<ExternalLibraryHandler> libraries;
    /* deleted methods */
    ExternalBehavioursHandler(ExternalBehavioursHandler &&) = delete;
    ExternalBehavioursHandler(const ExternalBehavioursHandler &) = delete;
    ExternalBehavioursHandler &operator=(ExternalBehavioursHandler &&) = delete;
    ExternalBehavioursHandler &operator=(const ExternalBehavioursHandler &) =
        delete;
  };  // end of struct ExternalBehavioursHandler

  ExternalBehavioursHandler::ExternalBehavioursHandler() {
    auto find_library = [this](const std::string &n) {
      return std::find_if(
          this->libraries.begin(), this->libraries.end(),
          [&n](const ExternalLibraryHandler &l) { return l.name == n; });
    };
    auto exit_if = [](const bool b, const std::string &m) {
      if (b) {
        usrmat_exit("ExternalBehavioursHandler::ExternalBehavioursHandler: " +
                    m);
      }
    };
    auto emsg = std::string{};
    auto open_library = [&emsg](const std::string &lib) -> libhandler {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      for (const auto prefix : {"", "lib"}) {
        for (const auto suffix : {"", ".dll"}) {
          const auto library = prefix + lib + suffix;
          const auto l = ::LoadLibrary(TEXT(library.c_str()));
          if (l != nullptr) {
            return l;
          } else {
            emsg +=
                "ExternalBehavioursHandler::ExternalBehavioursHandler: "
                "failed to open library '" +
                library + "' (" + getErrorMessage() + ")\n";
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
          } else {
            emsg +=
                "ExternalBehavioursHandler::ExternalBehavioursHandler: "
                "failed to open library '" +
                library + "' (" + getErrorMessage() + ")\n";
          }
        }
      }
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      return nullptr;
    };
    std::ifstream in("mfront-usrmat.dat");
    exit_if(!in, "unable to open file 'mfront-usrmat.dat'");
    std::string line;
    size_t ln = 1u;
    while (std::getline(in, line)) {
      if (line[0] == '*') {
        continue;
      }
      auto tokens = tokenize(line);
      exit_if(tokens.size() != 3u, "invalid line " + std::to_string(ln));
      exit_if(tokens[0].size() > 6u,
              "invalid identifier at line" + std::to_string(ln));
      const auto id = tokens[0];
      const auto lib = tokens[1];
      const auto fct = tokens[2];
      auto ptr = find_library(lib);
      if (ptr == this->libraries.end()) {
        const auto l = open_library(lib);
        exit_if(l == nullptr, "unable to load library '" + lib + "'\n" + emsg);
        this->libraries.push_back(ExternalLibraryHandler{lib, l});
        ptr = std::prev(this->libraries.end());
      }
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      auto f = reinterpret_cast<ExternalBehavioursHandler::fct>(
          ::GetProcAddress(ptr->l, fct.c_str()));
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      auto f = reinterpret_cast<ExternalBehavioursHandler::fct>(
          ::dlsym(ptr->l, fct.c_str()));
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      exit_if(f == nullptr, "could not load behaviour '" + fct +
                                "' in library '" + lib + "' (" +
                                getErrorMessage() + ")");
      std::array<char, 6> i = {' ', ' ', ' ', ' ', ' ', ' '};
      std::copy(id.begin(), id.end(), i.begin());
      this->behaviours.push_back(std::move(BehaviourHandler{i, f}));
      ++ln;
    }
    exit_if(in.bad(), "error while reading file 'mfront-usrmat.dat'");
    in.close();
  }  // end of ExternalBehavioursHandler::ExternalBehavioursHandler

  ExternalBehavioursHandler::fct ExternalBehavioursHandler::get(
      const char *const n) const {
    const auto f = this->find(n);
    if (f == nullptr) {
      usrmat_exit(
          "ExternalBehavioursHandler::get: "
          "no behaviour associated with identifier '" +
          std::string(n, 6) + "'");
    }
    return f;
  }  // end of ExternalBehavioursHandler::get

  ExternalBehavioursHandler::fct ExternalBehavioursHandler::find(
      const char *const n) const {
    const auto p =
        std::find_if(this->behaviours.begin(), this->behaviours.end(),
                     [n](const BehaviourHandler &b) {
                       return std::strncmp(b.id.data(), n, 6) == 0;
                     });
    return (p == this->behaviours.end()) ? nullptr : p->f;
  }  // end of ExternalBehavioursHandler::find

  ExternalBehavioursHandler::~ExternalBehavioursHandler() = default;

}  // end of namespace dianafea

extern "C" {

/*!
 * \param[out] sig: stresses
 * \param[out] ddsdde: stiffness matrix
 * \param[in,out] statev: internal state variables
 * \param[in] cname: name of the behaviour
 * \param[in] ntens: number of component of the stress tensor
 * \param[in] nprops: number of material properties
 * \param[in] nstatv: number of state variables
 * \param[in] eto: strain tensor at the beginning of the time step
 * \param[in] deto: strain tensor increment
 * \param[in] dt: time increment
 * \param[in] props: material properties
 * \param[in] temp: temperature at the beginning of the time step
 * \param[in] dtemp: temperature increment
 */
void dianafea_external_behaviour_call(double *const sig,
                                      double *const ddsdde,
                                      double *const statev,
                                      const char *const cname,
                                      const int ntens,
                                      const int nprops,
                                      const int nstatv,
                                      const double *const eto,
                                      const double *const deto,
                                      const double dt,
                                      const double *const props,
                                      const double temp,
                                      const double dtemp) {
  static dianafea::ExternalBehavioursHandler h;
  const auto f = h.get(cname);
  f(sig, ddsdde, statev,       //
    &ntens, &nprops, &nstatv,  //
    eto, deto, &dt, props, &temp, &dtemp);
}  // end of dianafea_external_behaviour_call

}  // end of extern "C"
