/*!
 * \file   umat41.cxx
 * \brief
 * \author th202608
 * \date   13/06/2018
 */

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#include <windows.h>
#else /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
#include <dlfcn.h>
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

#include <array>
#include <vector>
#include <string>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace lsdyna {

  //! pointer to a MFront Behaviour
  using LSDynaBehaviourPointer = void (*)(const double* const,
                                          const double* const,
                                          double* const,
                                          double* const,
                                          const double* const);
  /*!
   * A behaviour is associated to two pointer:
   * - one for the 3D case
   * - one for the plane stress case
   */
  using BehaviourContainer = std::array<LSDynaBehaviourPointer, 2u>;

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
  }    // end of  getErrorMessage

  static void log(const std::string& m) {
    static std::ofstream log("mfront-lsdyna.log");
    log << m << std::endl;
    std::clog << m << std::endl;
  }

  static std::vector<std::string> tokenize(const std::string& s, const char c) {
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

  static std::string strim_front(const std::string& s) {
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

  static std::string strim_back(const std::string& s) {
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

  static std::string strim(const std::string& s) {
    return strim_front(strim_back(s));
  }  // end of tokenize

  static std::vector<std::string> strim(const std::vector<std::string>& v) {
    auto r = std::vector<std::string>{};
    r.reserve(v.size());
    for (const auto& s : v) {
      auto ss = strim(s);
      if (!ss.empty()) {
        r.push_back(std::move(ss));
      }
    }
    return r;
  }  // end of tokenize

  static void exit(const std::string& m) {
    log(m);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    MessageBox(nullptr, m.c_str(), "mfront", 0);
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    std::exit(-1);
  }  // end of exit

  struct BehaviourLoader {
    BehaviourLoader();
    BehaviourLoader(BehaviourLoader&&) = delete;
    BehaviourLoader(const BehaviourLoader&) = delete;
    BehaviourLoader& operator=(BehaviourLoader&&) = delete;
    BehaviourLoader& operator=(const BehaviourLoader&) = delete;
    ~BehaviourLoader();
    BehaviourContainer operator[](const BehaviourContainer::size_type);

   private:
//! a simple alias
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    using libhandler = HINSTANCE__*;
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    using libhandler = void*;
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    //! internal data structure storing pointers to external libraries
    struct ExternalLibraryHandler {
      //! library name
      const std::string name;
      //! library handler
      const libhandler l;
    };  // end of struct ExternalLibraryHandler
    //! list of all registred behaviours
    std::array<BehaviourContainer, 10> behaviours;
    //! list of all loaded library
    std::vector<ExternalLibraryHandler> libraries;
  };

  BehaviourLoader::BehaviourLoader() {
    auto find_library = [this](const std::string& n) {
      return std::find_if(
          this->libraries.begin(), this->libraries.end(),
          [&n](const ExternalLibraryHandler& l) { return l.name == n; });
    };
    auto exit_if = [](const bool b, const std::string& m) {
      if (b) {
        exit("BehaviourLoader::BehaviourLoader: " + m);
      }
    };
    auto open_library = [](const std::string& lib) -> libhandler {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      for (const auto prefix : {"", "lib"}) {
        for (const auto suffix : {"", ".dll"}) {
          const auto library = prefix + lib + suffix;
          const auto l = ::LoadLibrary(TEXT(library.c_str()));
          if (l != nullptr) {
            return l;
          } else {
            log("BehaviourLoader::BehaviourLoader: "
                "failed to open library '" +
                library + "' (" + getErrorMessage() + ")");
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
            log("BehaviourLoader::BehaviourLoader: "
                "failed to open library '" +
                library + "' (" + getErrorMessage() + ")");
          }
        }
      }
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      return nullptr;
    };
    for (auto& b : this->behaviours) {
      b = {nullptr, nullptr};
    }
    std::ifstream in("mfront-lsdyna.k");
    exit_if(!in, "unable to open file 'mfront-lsdyna.dat'");
    std::string line;
    size_t ln = 1u;
    while (getline(in, line)) {
      auto tokens = strim(tokenize(line, ' '));
      if ((tokens.empty()) || (tokens[0][0] == '#')) {
        ++ln;
        continue;
      }
      exit_if(tokens.size() != 3, "invalid line '" + std::to_string(ln) +
                                      "' in file 'mfront-lsdyna.k', "
                                      "expected 3 tokens");
      const auto id = std::stoi(tokens[0]);
      const auto lib = tokens[1];
      const auto fct = tokens[2];
      exit_if((id < 41) || (id > 50), "invalid indentifier (" + tokens[0] +
                                          ") at line '" + std::to_string(ln) +
                                          "'");
      auto& f = this->behaviours[id - 41];
      exit_if((f[0] != nullptr) && (f[1] != nullptr),
              "a behaviour has already been associated to material "
              "identifier '" +
                  std::to_string(id) + "'");
      log("BehaviourLoader::BehaviourLoader: "
          "associating material '" +
          std::to_string(id) + "' to behaviour '" + fct + "' in library '" +
          lib);
      auto ptr = find_library(lib);
      if (ptr == this->libraries.end()) {
        const auto l = open_library(lib);
        exit_if(l == nullptr, "unable to load library '" + lib + "'");
        this->libraries.push_back(ExternalLibraryHandler{lib, l});
        ptr = std::prev(this->libraries.end());
      }
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      f = {reinterpret_cast<LSDynaBehaviourPointer>(
               ::GetProcAddress(ptr->l, (fct + "_3D").c_str())),
           reinterpret_cast<LSDynaBehaviourPointer>(
               ::GetProcAddress(ptr->l, (fct + "_PSTRESS").c_str()))};
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      f = {reinterpret_cast<LSDynaBehaviourPointer>(
               ::dlsym(ptr->l, (fct + "_3D").c_str())),
           reinterpret_cast<LSDynaBehaviourPointer>(
               ::dlsym(ptr->l, (fct + "_PSTRESS").c_str()))};
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      exit_if((f[0] == nullptr) && (f[1] == nullptr),
              "could not load behaviour '" + fct + "' in library '" + lib +
                  "' (" + getErrorMessage() + ")");
      ++ln;
    }
    exit_if(in.bad(), "error while reading file 'mfront-lsdyna.dat'");
    in.close();
  }  // end of BehaviourLoader::BehaviourLoader

  BehaviourContainer BehaviourLoader::operator[](
      const BehaviourContainer::size_type id) {
    if (id >= 10) {
      exit("BehaviourLoader::operator: invalid index");
    }
    const auto f = this->behaviours[id];
    if ((f[0] == nullptr) && (f[1] == nullptr)) {
      exit(
          "BehaviourLoader::operator: no behaviour associated with identifier "
          "'" +
          std::to_string(id) + "'");
    }
    return f;
  }  // end of BehaviourLoader::operator[]

  BehaviourLoader::~BehaviourLoader() = default;

  void getBehaviours(BehaviourContainer& c,
                     const BehaviourContainer::size_type i) {
    static BehaviourLoader l;
    c = l[i];
  }  // end of getBehaviours

  struct LSDynaBehaviour {
    //! array containing pointers to behaviours pointer
    BehaviourContainer c;
    /*!
     * \param[in] i: function index
     * - umat41 has index 0
     * - umat42 has index 1
     * - etc
     */
    LSDynaBehaviour(const BehaviourContainer::size_type);
    LSDynaBehaviour() = delete;
    LSDynaBehaviour(LSDynaBehaviour&&) = delete;
    LSDynaBehaviour(const LSDynaBehaviour&) = delete;
    LSDynaBehaviour& operator=(LSDynaBehaviour&&) = delete;
    LSDynaBehaviour& operator=(const LSDynaBehaviour&) = delete;
    ~LSDynaBehaviour();
  };  // end of struct LSDynaBehaviour

  LSDynaBehaviour::LSDynaBehaviour(const BehaviourContainer::size_type i) {
    getBehaviours(c, i);
  }  // end of LSDynaBehaviour::LSDynaBehaviour

  LSDynaBehaviour::~LSDynaBehaviour() = default;

  /*!
   * \param[in] l: structure containing the behaviour pointers
   * \param[in] m: material properties
   * \param[in] de: strain increment
   * \param[in] hisv: history variable
   * \param[in] dt: time increment
   * \param[in] capa: reduction factor for transverse shear
   * \param[in] etype: type of element, used to get the modelling hypothesis
   * \param[in] time: current time (unused)
   * \param[in] size: size of tye etype string (unused)
   */
  void call_umat(LSDynaBehaviour& l,
                 const double* const m,
                 const double* const de,
                 double* const sig,
                 double* const hisv,
                 const double* const dt,
                 const char* const etype,
                 const int s) {
    if (::strncmp(etype, "brick", std::max(5, s)) == 0) {
      const auto f = l.c[0];
      f(m, de, sig, hisv, dt);
    } else if (::strncmp(etype, "shell", std::max(5, s)) == 0) {
      const auto f = l.c[1];
      f(m, de, sig, hisv, dt);
    } else if (::strncmp(etype, "beam", std::max(5, s)) == 0) {
      const auto f = l.c[2];
      f(m, de, sig, hisv, dt);
    } else {
      exit("lsdyna::call_umat: unsupported element type '" +
           std::string(etype, s) + "'");
    }
  }  // end of call_umat

}  // end of namespace lsdyna

/* !
 * \def LSDYNA_PP_JOIN
 * \brief this macro joins joins its two arguments together.
 *
 * This macro was taken from the boost library:
 * - http://boost.org/
 *
 * The following piece of macro magic joins the two
 * arguments together, even when one of the arguments is
 * itself a macro (see 16.3.1 in C++ standard).  The key
 * is that macro expansion of macro arguments does not
 * occur in LSDYNA_PP_DO_JOIN2 but does in LSDYNA_PP_DO_JOIN.
 *
 * \author Thomas Helfer
 * \date   28 Aug 2006
 */
#define LSDYNA_PP_JOIN(X, Y) LSDYNA_PP_DO_JOIN(X, Y)

/*!
 * \def LSDYNA_PP_DO_JOIN
 * \brief An helper macro for LSDYNA_PP_JOIN
 * \see LSDYNA_PP_JOIN
 * \author Thomas Helfer
 * \date   28 Aug 2006
 */
#define LSDYNA_PP_DO_JOIN(X, Y) LSDYNA_PP_DO_JOIN2(X, Y)

/*!
 * \def LSDYNA_PP_DO_JOIN2
 * \brief An helper macro for LSDYNA_PP_JOIN
 * \see LSDYNA_PP_JOIN
 * \author Thomas Helfer
 * \date   28 Aug 2006
 */
#define LSDYNA_PP_DO_JOIN2(X, Y) X##Y

#define UMATFCT2(fct) LSDYNA_PP_JOIN(LSDYNA_PP_JOIN(umat, fct), _)

#define UMATFCT(fct, id)                                                     \
  /*!                                                                        \
   * \param[in] m: material properties                                       \
   * \param[in] de: strain increment                                         \
   * \param[in] hisv: history variable                                       \
   * \param[in] dt: time increment                                           \
   * \param[in] capa: reduction factor for transverse shear                  \
   * \param[in] etype: type of element, used to get the modelling hypothesis \
   * \param[in] time: current time (unused)                                  \
   * \param[in] size: size of tye etype string (unused)                      \
   */                                                                        \
  void UMATFCT2(fct)(                                                        \
      const double* const m, const double* const de, double* const sig,      \
      double* const hisv, const double* const dt, const double* const,       \
      const char* const etype, const double* const, const int s) {           \
    static lsdyna::LSDynaBehaviour l(id);                                    \
    call_umat(l, m, de, sig, hisv, dt, etype, s);                            \
  }  // end of UMATFCT2(fct)

extern "C" {

UMATFCT(41, 0)
UMATFCT(42, 1)
UMATFCT(43, 2)
UMATFCT(44, 3)
UMATFCT(45, 4)
UMATFCT(46, 5)
UMATFCT(47, 6)
UMATFCT(48, 7)
UMATFCT(49, 8)
UMATFCT(50, 9)

}  // end of extern "C"