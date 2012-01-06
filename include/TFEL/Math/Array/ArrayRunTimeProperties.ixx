/*!
 * \file   ArrayRunTimeProperties.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 jui 2006
 */

#ifndef _LIB_TFEL_ARRAYRUNTIMEPROPERTIES_IXX_
#define _LIB_TFEL_ARRAYRUNTIMEPROPERTIES_IXX_ 

#include <string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Name.hxx"

namespace tfel{
  
  namespace math {

    template<>
    struct ArrayRunTimeProperties<1u>
    {
    protected:
      unsigned int nb_cols;

    public:

      static std::string getName(void){
	return std::string("ArrayRunTimeProperties<1u>");
      }

      TFEL_MATH_INLINE2 ArrayRunTimeProperties(const unsigned int nb_cols_src)
	: nb_cols(nb_cols_src)
      {}

      TFEL_MATH_INLINE2 ArrayRunTimeProperties()
	: nb_cols(0u)
      {}

      TFEL_MATH_INLINE2 ArrayRunTimeProperties(const ArrayRunTimeProperties<1u>& src)
	: nb_cols(src.nb_cols)
      {}
      
      TFEL_MATH_INLINE2 unsigned int getNbCols(void) const
      {
	return this->nb_cols;
      }
      
      TFEL_MATH_INLINE2 ArrayRunTimeProperties& operator=(const ArrayRunTimeProperties& src)
      {
	this->nb_cols=src.nb_cols;
	return *this;
      }

      TFEL_MATH_INLINE2 bool operator==(const ArrayRunTimeProperties<1u>& b) const
      {
	return this->nb_cols==b.nb_cols;
      }

      TFEL_MATH_INLINE2 bool operator!=(const ArrayRunTimeProperties<1u>& b) const
      {
	return this->nb_cols!=b.nb_cols;
      }
	
    };

    template<>
    struct ArrayRunTimeProperties<2u>
    {
    protected:
      unsigned int nb_cols;
      unsigned int nb_rows;

    public:

      static std::string getName(void){
	return std::string("ArrayRunTimeProperties<2u>");
      }

      TFEL_MATH_INLINE2 ArrayRunTimeProperties(const unsigned int nb_rows_src,
					       const unsigned int nb_cols_src)
	: nb_cols(nb_cols_src),nb_rows(nb_rows_src)
      {}

      TFEL_MATH_INLINE2 ArrayRunTimeProperties()
	: nb_cols(0u),nb_rows(0u)
      {}

      TFEL_MATH_INLINE2 ArrayRunTimeProperties(const ArrayRunTimeProperties<2u>& src)
	: nb_cols(src.nb_cols),nb_rows(src.nb_rows)
      {}

      TFEL_MATH_INLINE2 unsigned int getNbCols(void) const
      {
	return this->nb_cols;
      }

      TFEL_MATH_INLINE2 unsigned int getNbRows(void) const
      {
	return this->nb_rows;
      }

      TFEL_MATH_INLINE2 ArrayRunTimeProperties& operator=(const ArrayRunTimeProperties& src)
      {
	this->nb_cols=src.nb_cols;
	this->nb_rows=src.nb_rows;
	return *this;
      }

      TFEL_MATH_INLINE2 bool operator==(const ArrayRunTimeProperties<2u>& b) const
      {
	return (this->nb_cols==b.nb_cols)&&(this->nb_rows==b.nb_rows);
      }

      TFEL_MATH_INLINE2 bool operator!=(const ArrayRunTimeProperties<2u>& b) const
      {
	return (this->nb_cols!=b.nb_cols)&&(this->nb_rows!=b.nb_rows);
      }

    };

    template<>
    struct ArrayRunTimeProperties<3u>
    {

    protected:
      unsigned int nb_cols;
      unsigned int nb_rows;
      unsigned int nb_slices;

    public:

      static std::string getName(void){
	return std::string("ArrayRunTimeProperties<3u>");
      }

      TFEL_MATH_INLINE2 ArrayRunTimeProperties(const unsigned int nb_rows_src, 
					       const unsigned int nb_cols_src, 
					       const unsigned int nb_slices_src)
	: nb_cols(nb_cols_src),nb_rows(nb_rows_src),nb_slices(nb_slices_src)
      {}

      TFEL_MATH_INLINE2 ArrayRunTimeProperties()
	: nb_cols(0u),nb_rows(0u),nb_slices(0u)
      {}

      TFEL_MATH_INLINE2 ArrayRunTimeProperties(const ArrayRunTimeProperties<3u>& src)
	: nb_cols(src.nb_cols),nb_rows(src.nb_rows),nb_slices(src.nb_slices)
      {}

      TFEL_MATH_INLINE2 unsigned int getNbCols(void) const
      {
	return this->nb_cols;
      }

      TFEL_MATH_INLINE2 unsigned int getNbRows(void) const
      {
	return this->nb_rows;
      }

      TFEL_MATH_INLINE2 unsigned int getNbSlices(void) const
      {
	return this->nb_slices;
      }
      
      TFEL_MATH_INLINE2 ArrayRunTimeProperties& operator=(const ArrayRunTimeProperties& src)
      {
	this->nb_cols   = src.nb_cols;
	this->nb_rows   = src.nb_rows;
	this->nb_slices = src.nb_slices;
	return *this;
      }

      TFEL_MATH_INLINE2 bool operator==(const ArrayRunTimeProperties<3u>& b) const
      {
	return (this->nb_cols==b.nb_cols)&&
	  (this->nb_rows==b.nb_rows)&&
	  (this->nb_slices==b.nb_slices);
      }

      TFEL_MATH_INLINE2 bool operator!=(const ArrayRunTimeProperties<3u>& b) const
      {
	return (this->nb_cols!=b.nb_cols)&&
	  (this->nb_rows!=b.nb_rows)&&
	  (this->nb_slices!=b.nb_slices);
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ARRAYRUNTIMEPROPERTIES_IXX */

