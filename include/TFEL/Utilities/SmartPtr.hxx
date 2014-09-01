/*!
 * \file   include/TFEL/Utilities/SmartPtr.hxx
 * \brief  this file describes the implementation of shared_ptr.
 * \author Thomas Helfer
 * \date   december 2005
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_UTILITIES_SMARTPTR_HPP_
#define _LIB_TFEL_UTILITIES_SMARTPTR_HPP_

#include<string>
#include<cassert>

#ifdef TFEL_HAVE_BOOST
#include<boost/shared_ptr.hpp>
#endif /* TFEL_HAVE_BOOST */

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{
  
  namespace utilities{

#ifdef TFEL_HAVE_BOOST

    using boost::shared_ptr;

#else /* TFEL_HAVE_BOOST */

    /*    ! \class shared_ptr
     *  \brief La classe \c shared_ptr se substitue aux pointeurs
     *  traditionnels auxquels elle ajoute la désallocation
     *  automatique de la mémoire.
     *  \author  Helfer Thomas
     *  \date    20 Février 2006
     *  \warning Cette classe ne doit pas être utilisée sur un tableau
     *
     *  La classe \c shared_ptr propose une implantation d'un pointeur
     *  intelligent basé sur un compteur de référence. Cette classe
     *  est adaptée à son utilisation avec les conteneurs de la STL.
     */
    template<typename T>
    struct shared_ptr{

      typedef T type;

      /*! \fn shared_ptr::shared_ptr() 
       *  \brief Constructeur par défaut
       *  
       *  Par défaut, les membres \c p et \c i sont initialisés au
       *  pointeur \c 0
       *
       */
      TFEL_INLINE shared_ptr() throw()
	: p(0), i(0) {}
  
      /*! \fn shared_ptr::shared_ptr(T *src) 
       *  \brief Constructeur à partir d'un pointeur
       *  \warning Cette classe ne doit pas être utilisée sur un tableau
       *
       *  La classe \c shared_ptr se charge de désallouer la mémoire sur
       *  laquelle pointe le pointeur en argument.
       *
       *  \latexonly
       *  Une utilisation typique de cette méthode est~:
       *  \begin{center}
       *  shared_ptr<double> p(new double);
       *  \end{center}
       *  \endlatexonly
       *
       *  \param src : pointeur servant à initialiser la classe 
       */
      TFEL_INLINE explicit shared_ptr(T *src)
	: p(src), i(0) {
	if(p!=0){
	  this->i = new unsigned int(1);
	}
      }

      /*! \fn shared_ptr::shared_ptr(const shared_ptr<T>& src) 
       *  \brief  Constructeur par copie
       *  
       *  \param[in] src \c shared_ptr source utilisé pour initialiser la
       *  classe. 
       */
      TFEL_INLINE shared_ptr(const shared_ptr<T>& src) throw()
	: p(src.p), i(src.i) 
      {
	if(i!=0){
	  ++(*(this->i));
	}
      }

      /*! \fn    shared_ptr::~shared_ptr() 
       *  \brief Destructeur
       *  
       */
      TFEL_INLINE
      ~shared_ptr()  throw()
      {
	this->decreaseCounter();
      }

      /*! \fn      const shared_ptr<T>& operator=(const shared_ptr<T>& src)
       *  \brief  Opérateur d'affectation
       *  \param[in] src \c shared_ptr source auquel la classe
       *  est affecté.        
       */
      TFEL_INLINE
      shared_ptr<T>&
      operator=(const shared_ptr<T>& src)
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

      /*! \fn      bool unique(void)
       *  \brief   permet de savoir si un seul objet est référencé
       *  \return  true ou false suivant le cas
       */
      TFEL_INLINE
      bool
      unique(void) const throw()
      {
	if(i!=0){
	  return (*(this->i)==1);
	}
	return false;
      }

      /*! \fn T* getPtr(void)
       */
      TFEL_INLINE T * get(void){
	return this->p;
      }
      
      /*! \fn T* getPtr(void) const
       */
      TFEL_INLINE
      const T *
      get(void) const{
	return this->p;
      }

    protected:

      T *p;
      
    private:

      mutable unsigned int *i; 
     
      /*! \fn shared_ptr::decreaseCounter() 
       *  \brief   méthode chargée de décrémenter le compteur de référence
       *  
       *  Cette méthode est chargée de décrémenter le compteur de
       *  référence i et de désallouer la mémoire si celui-ci est nul.
       */
      TFEL_INLINE void decreaseCounter(void) throw() {
	if(this->i!=0){
	  if((--*(this->i))==0){
	    delete this->i;
	    delete this->p;
	    this->i=0;
	    this->p=0;
	  }
	}
      }

    };

#endif /* TFEL_HAVE_BOOST */

    /*!
     * A small metafunction which returns the shared_ptr class
     * associated with the given type
     * \param T : type to be transformed
     */
    template<typename T>
    struct PtrType
    {
      //! the result
      typedef shared_ptr<T> type;
    }; // end of struct PtrType

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_SMARTPTR_HPP_ */
