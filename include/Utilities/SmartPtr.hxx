/*
 * \file   SmartPtr.hxx
 * \brief  this file describes the implementation of SmartPtr.
 * \author Thomas Helfer
 * \date   december 2005
 */

#ifndef _LIB_TFEL_SMARTPTR_HPP_
#define _LIB_TFEL_SMARTPTR_HPP_

#include<string>
#include<cassert>
#include"Utilities/Name.hxx"

#define TFEL_INLINE inline

namespace tfel{
  
  namespace utilities{

    /*! \class SmartPtr
     *  \brief La classe \c SmartPtr se substitue aux pointeurs
     *  traditionnels auxquels elle ajoute la désallocation
     *  automatique de la mémoire.
     *  \author  Helfer Thomas
     *  \date    20 Février 2006
     *  \warning Cette classe ne doit pas être utilisée sur un tableau
     *
     *  La classe \c SmartPtr propose une implantation d'un pointeur
     *  intelligent basé sur un compteur de référence. Cette classe
     *  est adaptée à son utilisation avec les conteneurs de la STL.
     */  
    template<typename T>
    struct SmartPtr{

      typedef T type;

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static
      std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("tfel::utilities::SmartPtr<")+Name<T>::getName()+string(">");
      }

      /*! \fn SmartPtr::SmartPtr() 
       *  \brief Constructeur par défaut
       *  
       *  Par défaut, les membres \c p et \c i sont initialisés au
       *  pointeur \c 0
       *
       */
      TFEL_INLINE SmartPtr() throw()
	: p(0), i(0) {}
  
      /*! \fn SmartPtr::SmartPtr(T *src) 
       *  \brief Constructeur à partir d'un pointeur
       *  \warning Cette classe ne doit pas être utilisée sur un tableau
       *
       *  La classe \c SmartPtr se charge de désallouer la mémoire sur
       *  laquelle pointe le pointeur en argument.
       *
       *  \latexonly
       *  Une utilisation typique de cette méthode est~:
       *  \begin{center}
       *  SmartPtr<double> p(new double);
       *  \end{center}
       *  \endlatexonly
       *
       *  \param src : pointeur servant à initialiser la classe 
       */
      TFEL_INLINE explicit SmartPtr(T *src)
	: p(src), i(0) {
	if(p!=0){
	  this->i = new unsigned int(1);
	}
      }

      /*! \fn SmartPtr::SmartPtr(const T& src) 
       *  \brief Constructeur à partir d'un objet
       *  \warning Cette classe ne doit pas être utilisée sur un tableau
       *  \warning La classe T doit posséder un constructeur par copie
       *  La classe \c SmartPtr se charge de désallouer la mémoire sur
       *  laquelle pointe le pointeur en argument.
       *
       *  \latexonly
       *  Une utilisation typique de cette méthode est~:
       *  \begin{center}
       *  SmartPtr<double> p(12.);
       *  \end{center}
       *  \endlatexonly
       *
       *  \param src : pointeur servant à initialiser la classe 
       */
      TFEL_INLINE explicit SmartPtr(const T& src)
	: i(0) {
	this->p = new T(src);
	this->i = new unsigned int(1);
      }

      /*! \fn SmartPtr::SmartPtr(const SmartPtr<T>& src) 
       *  \brief  Constructeur par copie
       *  
       *  \param[in] src \c SmartPtr source utilisé pour initialiser la
       *  classe. 
       */
      TFEL_INLINE SmartPtr(const SmartPtr<T>& src) throw()
	: p(src.p), i(src.i) 
      {
	if(i!=0){
	  ++(*(this->i));
	}
      }

      /*! \fn    SmartPtr::~SmartPtr() 
       *  \brief Destructeur
       *  
       */
      TFEL_INLINE
      ~SmartPtr()  throw()
      {
	this->decreaseCounter();
      }

      /*! \fn      const SmartPtr<T>& operator=(const SmartPtr<T>& src)
       *  \brief  Opérateur d'affectation
       *  \param[in] src \c SmartPtr source auquel la classe
       *  est affecté.        
       */
      TFEL_INLINE
      SmartPtr<T>&
      operator=(const SmartPtr<T>& src)
	throw()
      {
	// Traitement d'une affectation à soi-même
	if(this==&src){
	  return *this;
	}
	if(this->i!=0){
	  this->decreaseCounter();
	}
	this->i=src.i;
	this->p=src.p;
	if(i!=0){
	  *(this->i) += 1;
	}
	return *this;
      }

      /*! \fn     const T& operator*(void) 
       *  \brief  Opérateur de déférencement (version \c const).
       *  \warning Il est a priori nécessaire de vérifier que l'objet référencé par la classe à bien été initialisé.
       *  \return Retourne une référence constante vers l'objet
       *  contenu par la classe.
       */
      TFEL_INLINE const T& operator*(void) const 
	throw()
      {
	assert(p!=0);
	return *(this->p);
      }

      /*! \fn     const T& operator*(void) 
       *  \brief  Opérateur de déférencement.
       *  \warning Il est a priori nécessaire de vérifier que l'objet référencé par la classe à bien été initialisé.
       *  \return Retourne une référence vers l'objet contenu par la
       *  classe.
       */
      TFEL_INLINE T& operator*(void) throw()
      {
	assert(p!=0);
	return *(this->p);
      }

      /*! \fn const T* operator->(void) 
       *  \brief  Opérateur de d'indirection (version constante).
       *  \warning Il est a priori nécessaire de vérifier que l'objet référencé par la classe à bien été initialisé.
       *  \return Retourne un pointeur vers l'objet contenu par la
       *  classe. L'objet pointé ne peut être modifié.
       */
      TFEL_INLINE
      const T*
      operator->(void) const throw()
      {
	assert(p!=0);
	return this->p;
      }
      
      /*! 
       *  \brief  Opérateur de d'indirection.
       *  \warning Il est a priori nécessaire de vérifier que l'objet référencé par la classe à bien été initialisé.
       *  \return Retourne un pointeur vers l'objet contenu par la
       *  classe. L'objet pointé ne peut être modifié.
       */
      TFEL_INLINE
      T* operator->(void) throw()
      {
	assert(p!=0);
	return this->p;
      }
      
      /*! 
       *  \brief   Permet de savoir si la classe pointe sur un objet valide
       *  \return \c true si la classe pointe sur un objet valide, \c false sinon.
       */
      TFEL_INLINE
      bool
      isValid(void) const throw()
      {
	return !(this->p==0);
      }

      /*! \fn      unsigned int count(void)
       *  \brief   Permet de connaître le nombre de référénces sur
       *           l'objet référencé par la classe.
       *  \warning Cette méthode n'est fournie que pour le débogage~:
       *           elle est désactivée si le symbole NDEBUG est défini.  
       *  \return  Le nombre de références
       */
      TFEL_INLINE
      unsigned int
      count(void) const throw()
      {
	if(i!=0){
	  return *(this->i);
	}
	return 0;
      }

      /*! \fn    void makeUniq(void)
       *  \brief Réalise un copie de l'objet référencé et pointe dessus 
       */
      TFEL_INLINE
      void makeUniq(void){
	if(this->i!=0){
	  if(*(this->i)!=1){
	    --(*(this->i));
	    this->p = new T(*(this->p));
	    this->i = new unsigned int(1);
	  }
	}
      }

      /*! \fn T* getPtr(void)
       */
      TFEL_INLINE T * getPtr(void){
	return this->p;
      }
      
      /*! \fn T* getPtr(void) const
       */
      TFEL_INLINE
      const T *
      getPtr(void) const{
	return this->p;
      }

    protected:

      T *p;
      
    private:

      mutable unsigned int *i; 
     
      /*! \fn SmartPtr::decreaseCounter() 
       *  \brief   méthode chargée de décrémenter le compteur de référence
       *  
       *  Cette méthode est chargée de décrémenter le compteur de
       *  référence \c i et de désallouer la mémoire si celui-ci est nul.
       */
      TFEL_INLINE void decreaseCounter(void) throw() {
	if(this->i!=0){
	  if((--*(this->i))==0){
	    delete i;
	    delete p;
	    i=0;
	    p=0;
	  }
	}
      }

    };

    template<typename T>
    struct PtrType
    {
      typedef SmartPtr<T> type;
    }; // end of struct PtrType

  } // end of namespace utilities

} // end of namespace tfel

#endif	    /* !SMARTPTR_HPP_ */
