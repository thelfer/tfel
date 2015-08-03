/*!
 * \file   mfront/src/JavaMaterialPropertyInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>
#include<sstream>
#include<stdexcept>
#include<iterator>
#include<string>
#include<set>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#include"TFEL/System/System.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/CMaterialPropertyInterface.hxx"
#include"MFront/JavaMaterialPropertyInterface.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront
{

  static std::string
  getJavaClassName(const MaterialPropertyDescription& mpd){
    if(mpd.library.empty()){
      if(!mpd.material.empty()){
	return mpd.material;
      } else {
	return "UnknownMaterial";
      }
    }
    return mpd.library;
  } // end of getJavaClassName

  static std::string
  getJavaClassFileName(const MaterialPropertyDescription& mpd,
		       const std::string& package){
    if(package.empty()){
      return getJavaClassName(mpd);
    }
    return package+"/"+getJavaClassName(mpd);
  } // end of getJavaClassFileName
  
  std::string
  JavaMaterialPropertyInterface::getName(void)
  {
    return "java";
  }

  JavaMaterialPropertyInterface::JavaMaterialPropertyInterface()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  JavaMaterialPropertyInterface::treatKeyword(const std::string& key,
					      tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					      const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using namespace std;
    if(key=="@Package"){
      if(!this->package.empty()){
	throw(runtime_error("JavaMaterialPropertyInterface::treatKeyword : "
			    "package name already defined"));
      }
      if(current==end){
	throw(runtime_error("JavaMaterialPropertyInterface::treatKeyword : "
			    "unexpected end of file"));
      }
      auto p = current->value;
      if(!tfel::utilities::CxxTokenizer::isValidIdentifier(p,true)){
	throw(runtime_error("JavaMaterialPropertyInterface::treatKeyword : "
			    "invalid package name '"+p+"'"));
      }
      for(const auto c : p){
	if(isalpha(c)){
	  if(!islower(c)){	
	    throw(runtime_error("JavaMaterialPropertyInterface::treatKeyword : "
				"invalid package name '"+p+"'"));
    	  }
	}
      }
      ++(current);
      if(current==end){
	throw(runtime_error("JavaMaterialPropertyInterface::treatKeyword : "
			    "unexpected end of file"));
      }  
      if(current->value!=";"){
	throw(runtime_error("JavaMaterialPropertyInterface::treatKeyword : "
			    "expected ';', read '"+current->value+"'"));
      }
      ++(current);
      this->package = p;
      return make_pair(true,current);      
    }
    return {false,current};
  } // end of treatKeyword

  JavaMaterialPropertyInterface::~JavaMaterialPropertyInterface()
  {}

  void
  JavaMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
						       const MaterialPropertyDescription& mpd)
  {
    const auto lib = "lib"+getMaterialLawLibraryNameBase(mpd.library,mpd.material)+"-java";
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    d.dependencies[lib].push_back("-lm");    
    // the jni part
    d.cppflags[lib].push_back(TFEL_JAVA_INCLUDES);
    d.sources[lib].push_back(name+"-java.cxx");
    if(this->package.empty()){
      d.epts[lib].push_back(getJavaClassName(mpd)+"."+mpd.law);
    } else {
      d.epts[lib].push_back(this->package+"."+getJavaClassName(mpd)+"."+mpd.law);
    }
    // the java class
    const auto jfname = getJavaClassFileName(mpd,this->package);
    const auto src    = "../java/"+jfname+".java";
    const auto target = "../java/"+jfname+".class";
    auto cmd = std::string{};
    const char * java = ::getenv("JAVAC");
    if(java==nullptr){
      cmd += "javac";
    } else {
      cmd += java;
    }
    cmd += " "+src;
    auto& res = d.specific_targets;
    res[target].first.push_back(src);
    res[target].second.push_back(cmd);
    res["all"].first.push_back(target);
  } // end of JavaMaterialPropertyInterface::getTargetsDescription

  void
  JavaMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						  const FileDescription& fd)
  {
    using namespace std;
    using tfel::utilities::replace_all;
    tfel::system::systemCall::mkdir("java");
    if(!this->package.empty()){
      tfel::system::systemCall::mkdir("java/"+this->package);
    }
    const auto lib   = getMaterialLawLibraryNameBase(mpd.library,mpd.material)+"-java";
    const auto cname = (!mpd.material.empty()) ? mpd.material : "UnknowMaterial";
    const auto name  = (!mpd.material.empty()) ? mpd.material+"_"+mpd.law : mpd.law;
    const auto srcFileName     = "src/" + name + "-java.cxx";
    ofstream srcFile{srcFileName};
    if(!srcFile){
      throw(runtime_error("JavaMaterialPropertyInterface::writeOutputFiles : "
			  "unable to open '"+ srcFileName+"' for writing output file."));
    }
    srcFile.exceptions(ios::badbit|ios::failbit);
    // writing source
    srcFile << "/*!" << endl;
    srcFile << "* \\file   " << srcFileName  << endl;
    srcFile << "* \\brief  " << "this file implements the java interface "
	    << "for the " << name << " material law.\n";
    srcFile << "*         File generated by ";
    srcFile << MFrontHeader::getVersionName() << " ";
    srcFile << "version " << MFrontHeader::getVersionNumber();
    srcFile << endl;
    if(!fd.authorName.empty()){
      srcFile << "* \\author " << fd.authorName << endl;
    }
    if(!fd.date.empty()){
      srcFile << "* \\date   " << fd.date       << endl;
    }
    srcFile << " */\n\n";
    // headers 
    srcFile << "#include<algorithm>\n"
	    << "#include<iostream>\n"
	    << "#include<sstream>\n"
      	    << "#include<cstring>\n"
	    << "#include<cstdlib>\n"
	    << "#include<string>\n"
	    << "#include<cmath>\n\n"
	    << "#include <jni.h>\n\n";
    if(!mpd.includes.empty()){
      srcFile << mpd.includes << "\n\n";
    }
    srcFile << "#ifdef __cplusplus\n";
    srcFile << "extern \"C\"{\n";
    srcFile << "#endif /* __cplusplus */\n\n";
    srcFile << "JNIEXPORT jdouble JNICALL\n";
    if(this->package.empty()){
      srcFile << "Java_" << replace_all(getJavaClassName(mpd),"_","_1");
    } else {
      srcFile << "Java_" << replace_all(this->package,"_","_1") << "_"
	      << replace_all(getJavaClassName(mpd),"_","_1");
    }
    srcFile << "_" <<  replace_all(mpd.law,"_","_1") << "(";
    srcFile << "JNIEnv *java_env,jclass";
    for(const auto i:mpd.inputs){
      srcFile << ", const jdouble " << i.name;
    }
    srcFile << ")\n{\n";
    srcFile << "using namespace std;\n";
    srcFile << "using real = jdouble;\n";
    // handle java exceptions
    srcFile << "auto throwJavaRuntimeException = [java_env](const string& msg){\n"
	    << "  auto jexcept = java_env->FindClass(\"java/lang/RuntimeException\");\n"
	    << "  if (jexcept==nullptr){\n"
	    << "    cerr << \"Internal error : can't find the java \"\n"
	    << "         << \"RuntimeException class\" << endl;\n"
	    << "    ::exit(EXIT_FAILURE);\n"
	    << "  }\n"
	    << "  java_env->ThrowNew(jexcept,msg.c_str());\n"
	    << "  java_env->DeleteLocalRef(jexcept);\n"
	    << "  return jdouble{};\n"
	    << "};\n";
    // material laws
    writeMaterialLaws("JavaMaterialPropertyInterface::writeOutputFile",
		      srcFile,mpd.materialLaws);
    // static variables
    writeStaticVariables("JavaMaterialPropertyInterface::writeOutputFile",
			 srcFile,mpd.staticVars,fd.fileName);
    // parameters
    for(const auto& p : mpd.parameters){
      const auto p6 = mpd.parametersValues.find(p);
      if(p6==mpd.parametersValues.end()){
	throw(runtime_error("JavaMaterialPropertyInterface::writeOutputFile : "
			    "internal error (can't find value of parameter '"+p+"')"));
      }
      srcFile << "static " << constexpr_c << " double " << p << " = " << p6->second << ";\n";
    }
    if((!mpd.physicalBoundsDescriptions.empty())||(!mpd.boundsDescriptions.empty())){
      srcFile << "#ifndef JAVA_NO_BOUNDS_CHECK\n";
      if(!mpd.physicalBoundsDescriptions.empty()){
	srcFile << "// treating physical bounds\n";
	for(const auto& b : mpd.physicalBoundsDescriptions){
	  if(b.boundsType==VariableBoundsDescription::Lower){
	    srcFile << "if(" << b.varName<< " < "<< b.lowerBound << "){\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is below its physical lower bound (\"\n << "
	    	    << b.varName << " << \"<" << b.lowerBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "}\n";
	  } else if(b.boundsType==VariableBoundsDescription::Upper){
	    srcFile << "if(" << b.varName<< " > "<< b.upperBound << "){\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is beyond its physical upper bound (\"\n << "
	    	    << b.varName << " << \">" << b.upperBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "}\n";
	  } else {
	    srcFile << "if((" << b.varName<< " < "<< b.lowerBound << ")||"
	    	    << "(" << b.varName<< " > "<< b.upperBound << ")){\n";
	    srcFile << "if(" << b.varName<< " < " << b.lowerBound << "){\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is below its physical lower bound (\"\n << "
	    	    << b.varName << " << \"<" << b.lowerBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "} else {\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is beyond its physical upper bound (\"\n << "
	    	    << b.varName << " << \">" << b.upperBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "}\n";
	    srcFile << "}\n";
	  }
	}
	if(!mpd.boundsDescriptions.empty()){
	  srcFile << "// treating standard bounds\n";
	  for(const auto& b : mpd.boundsDescriptions){
	    if((b.boundsType==VariableBoundsDescription::Lower)||
	       (b.boundsType==VariableBoundsDescription::LowerAndUpper)){
	      srcFile << "if(" << b.varName<< " < "<< b.lowerBound << "){\n";
	      srcFile << "auto policy = "
		      << "::getenv(\"JAVA_OUT_OF_BOUND_POLICY\");\n";
	      srcFile << "if(policy!=nullptr){\n";
	      srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
		      << "(strcmp(policy,\"WARNING\")==0)){\n";
	      srcFile << "ostringstream msg;\n";
	      srcFile << "msg << \"" << name << " : "
		      << b.varName << " is below its lower bound (\"\n << "
		      << b.varName << " << \"<" << b.lowerBound << ").\";\n";
	      srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      srcFile << "return throwJavaRuntimeException(msg.str());\n";
	      srcFile << "} else {\n";
	      srcFile << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	    }
	    if((b.boundsType==VariableBoundsDescription::Upper)||
	       (b.boundsType==VariableBoundsDescription::LowerAndUpper)){
	      srcFile << "if(" << b.varName<< " > "<< b.upperBound << "){\n";
	      srcFile << "auto policy = "
		      << "::getenv(\"JAVA_OUT_OF_BOUND_POLICY\");\n";
	      srcFile << "if(policy!=nullptr){\n";
	      srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
		      << "(strcmp(policy,\"WARNING\")==0)){\n";
	      srcFile << "ostringstream msg;\n";
	      srcFile << "msg << \"" << name << " : "
		      << b.varName << " is over its upper bound (\"\n << "
		      << b.varName << " << \">" << b.upperBound << ").\";\n";
	      srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      srcFile << "return throwJavaRuntimeException(msg.str());\n";
	      srcFile << "} else {\n";
	      srcFile << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	    }
	  }
	}
	srcFile << "#endif /* JAVA_NO_BOUNDS_CHECK */\n";
      }
    }
    srcFile << "jdouble " << mpd.output << ";\n"
	    << "try{\n"
	    << mpd.f.body
	    << "} catch(exception& cpp_except){\n"
	    << "  return throwJavaRuntimeException(cpp_except.what());\n"
	    << "} catch(...){\n"
      	    << "  return throwJavaRuntimeException(\"unknown C++ exception\");\n"
	    << "}\n"
	    << "return " << mpd.output << ";\n";
    srcFile << "} // end of " << name << "\n\n";
    srcFile << "#ifdef __cplusplus\n";
    srcFile << "} // end of extern \"C\"\n";
    srcFile << "#endif /* __cplusplus */\n\n";
    srcFile.close();
    // writing java class
    auto& lock = MFrontLock::getMFrontLock();
    lock.lock();
    auto methods = map<string,decltype(mpd.inputs.size())>{};
    auto jcname = getJavaClassName(mpd);
    auto jcmlst = "src/"+jcname+".lst";
    struct stat buffer; // for call to stat
    if(access(jcmlst.c_str(),F_OK)==0){
      stat(jcmlst.c_str(),&buffer);
      if(!S_ISREG(buffer.st_mode)){
    	throw(runtime_error("JavaMaterialPropertyInterface::writeOutputFiles : "
			    "'"+jcmlst+"' is not a regular file"));
      }
      ifstream f(jcmlst);
      if(!f){
    	throw(runtime_error("JavaMaterialPropertyInterface::writeOutputFiles : "
			    "unable to open '"+jcmlst+"'"));
      }
      auto line = string();
      getline(f,line);
      if(line!=this->package){
    	throw(runtime_error("JavaMaterialPropertyInterface::writeOutputFiles : "
			    "inconsistent package name for class '"+jcname+"'"));
      }
      while (getline(f, line)){
	istringstream iss{line};
	iss.exceptions(ios::badbit|ios::failbit);
	auto m = string{}; // method name
	auto n = int{};    // number of arguments
	iss >> m >> n;
	methods.insert({m,n});
      }
      f.close();
    }
    methods[mpd.law] = mpd.inputs.size();
    ofstream mf{jcmlst};
    if(!mf){
      throw(runtime_error("JavaMaterialPropertyInterface::writeOutputFiles : "
			  "unable to open '"+jcmlst+"'"));
    }
    mf.exceptions(ios::badbit|ios::failbit);
    mf << this->package << endl;
    for(const auto& m:methods){
      mf << m.first << " " << m.second << "\n";
    }
    mf.close();
    ofstream jc{"java/"+getJavaClassFileName(mpd,this->package)+".java"};
    if(!jc){
      throw(runtime_error("JavaMaterialPropertyInterface::writeOutputFiles : "
			  "unable to open '"+jcname+".java'"));
    }
    jc.exceptions(ios::badbit|ios::failbit);
    if(!this->package.empty()){
      jc << "package " << this->package << ";\n\n";
    }
    jc << "public class " << jcname <<"{\n"
       << "static {System.loadLibrary(\"" << lib  << "\");}\n";
    for(const auto& m:methods){
      jc << "public static native double " << m.first << "(";
      for(decltype(m.second) i=0;i!=m.second;++i){
	if(i!=0){
	  jc << "," << endl;
	}
	jc << "double x" << i;
      }
      jc << ");\n";
    }
    jc << "}" << endl;
    jc.close();
    lock.unlock();
  } // end of JavaMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront
