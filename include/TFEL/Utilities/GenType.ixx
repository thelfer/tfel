/*!
 * \file   GenType.ixx
 * \see    GenerateGenType.cxx to see how this file was generated.
 * \brief  This file implements the GenType template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_GENTYPE_I_
#define _LIB_TFEL_GENTYPE_I_ 

#include"TFEL/Metaprogramming/EnableIf.hxx"

#include"TFEL/Utilities/Name.hxx"

namespace tfel{

  namespace utilities{

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9,typename T10,typename T11,
	     typename T12,typename T13,typename T14,typename T15,
	     typename T16,typename T17,typename T18,typename T19,
	     typename T20,typename T21,typename T22,typename T23,
	     typename T24,typename T25,typename T26,typename T27,
	     typename T28,typename T29,typename T30,typename T31,
	     typename T32,typename T33,typename T34,typename T35,
	     typename T36,typename T37,typename T38,typename T39,
	     typename T40,typename T41,typename T42,typename T43,
	     typename T44,typename T45,typename T46,typename T47,
	     typename T48,typename T49>
    struct GenType
      : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
								 T5,T6,T7,T8,
								 T9,T10,T11,T12,
								 T13,T14,T15,T16,
								 T17,T18,T19,T20,
								 T21,T22,T23,T24,
								 T25,T26,T27,T28,
								 T29,T30,T31,T32,
								 T33,T34,T35,T36,
								 T37,T38,T39,T40,
								 T41,T42,T43,T44,
								 T45,T46,T47,T48,
								 T49>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief  Constructor from T43.
       * \param  const T43&, initial value
       */
      GenType(const T43& v)
      {
	this->index = 43;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T43(v);
      }
      /*!
       * \brief  Constructor from T44.
       * \param  const T44&, initial value
       */
      GenType(const T44& v)
      {
	this->index = 44;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T44(v);
      }
      /*!
       * \brief  Constructor from T45.
       * \param  const T45&, initial value
       */
      GenType(const T45& v)
      {
	this->index = 45;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T45(v);
      }
      /*!
       * \brief  Constructor from T46.
       * \param  const T46&, initial value
       */
      GenType(const T46& v)
      {
	this->index = 46;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T46(v);
      }
      /*!
       * \brief  Constructor from T47.
       * \param  const T47&, initial value
       */
      GenType(const T47& v)
      {
	this->index = 47;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T47(v);
      }
      /*!
       * \brief  Constructor from T48.
       * \param  const T48&, initial value
       */
      GenType(const T48& v)
      {
	this->index = 48;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T48(v);
      }
      /*!
       * \brief  Constructor from T49.
       * \param  const T49&, initial value
       */
      GenType(const T49& v)
      {
	this->index = 49;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T49(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+",";
	name+=Name<T43>::getName()+","+Name<T44>::getName()+","+Name<T45>::getName()+",";
	name+=Name<T46>::getName()+","+Name<T47>::getName()+","+Name<T48>::getName()+",";
	name+=Name<T49>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42,
						    T43,T44,T45,T46,T47,T48,
						    T49>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };


    template<typename T0>
    struct GenType<T0,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1>
    struct GenType<T0,T1,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>
					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2>
    struct GenType<T0,T1,T2,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3>
    struct GenType<T0,T1,T2,T3,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4>
    struct GenType<T0,T1,T2,T3,T4,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5>
    struct GenType<T0,T1,T2,T3,T4,T5,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
		   T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
		   GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
		   GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
		   GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

		     : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
										T5,T6,T7,T8,
										T9,T10,T11,T12,
										T13,T14,T15,T16,
										T17,T18,T19,T20>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,GTEnd,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,GTEnd,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,GTEnd,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,GTEnd,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41,typename T42>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,T42,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41,T42>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41,typename T42,typename T43>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,T42,T43,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41,T42,T43>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief  Constructor from T43.
       * \param  const T43&, initial value
       */
      GenType(const T43& v)
      {
	this->index = 43;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T43(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+",";
	name+=Name<T43>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42,
						    T43>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41,typename T42,typename T43,typename T44>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,T42,T43,T44,GTEnd,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41,T42,T43,T44>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief  Constructor from T43.
       * \param  const T43&, initial value
       */
      GenType(const T43& v)
      {
	this->index = 43;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T43(v);
      }
      /*!
       * \brief  Constructor from T44.
       * \param  const T44&, initial value
       */
      GenType(const T44& v)
      {
	this->index = 44;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T44(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+",";
	name+=Name<T43>::getName()+","+Name<T44>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42,
						    T43,T44>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41,typename T42,typename T43,typename T44,
	     typename T45>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,T42,T43,T44,T45,GTEnd,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41,T42,T43,T44,
												      T45>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief  Constructor from T43.
       * \param  const T43&, initial value
       */
      GenType(const T43& v)
      {
	this->index = 43;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T43(v);
      }
      /*!
       * \brief  Constructor from T44.
       * \param  const T44&, initial value
       */
      GenType(const T44& v)
      {
	this->index = 44;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T44(v);
      }
      /*!
       * \brief  Constructor from T45.
       * \param  const T45&, initial value
       */
      GenType(const T45& v)
      {
	this->index = 45;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T45(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+",";
	name+=Name<T43>::getName()+","+Name<T44>::getName()+","+Name<T45>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42,
						    T43,T44,T45>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41,typename T42,typename T43,typename T44,
	     typename T45,typename T46>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,T42,T43,T44,T45,T46,GTEnd,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41,T42,T43,T44,
												      T45,T46>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief  Constructor from T43.
       * \param  const T43&, initial value
       */
      GenType(const T43& v)
      {
	this->index = 43;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T43(v);
      }
      /*!
       * \brief  Constructor from T44.
       * \param  const T44&, initial value
       */
      GenType(const T44& v)
      {
	this->index = 44;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T44(v);
      }
      /*!
       * \brief  Constructor from T45.
       * \param  const T45&, initial value
       */
      GenType(const T45& v)
      {
	this->index = 45;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T45(v);
      }
      /*!
       * \brief  Constructor from T46.
       * \param  const T46&, initial value
       */
      GenType(const T46& v)
      {
	this->index = 46;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T46(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+",";
	name+=Name<T43>::getName()+","+Name<T44>::getName()+","+Name<T45>::getName()+",";
	name+=Name<T46>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42,
						    T43,T44,T45,T46>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41,typename T42,typename T43,typename T44,
	     typename T45,typename T46,typename T47>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,T42,T43,T44,T45,T46,T47,GTEnd,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41,T42,T43,T44,
												      T45,T46,T47>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief  Constructor from T43.
       * \param  const T43&, initial value
       */
      GenType(const T43& v)
      {
	this->index = 43;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T43(v);
      }
      /*!
       * \brief  Constructor from T44.
       * \param  const T44&, initial value
       */
      GenType(const T44& v)
      {
	this->index = 44;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T44(v);
      }
      /*!
       * \brief  Constructor from T45.
       * \param  const T45&, initial value
       */
      GenType(const T45& v)
      {
	this->index = 45;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T45(v);
      }
      /*!
       * \brief  Constructor from T46.
       * \param  const T46&, initial value
       */
      GenType(const T46& v)
      {
	this->index = 46;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T46(v);
      }
      /*!
       * \brief  Constructor from T47.
       * \param  const T47&, initial value
       */
      GenType(const T47& v)
      {
	this->index = 47;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T47(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+",";
	name+=Name<T43>::getName()+","+Name<T44>::getName()+","+Name<T45>::getName()+",";
	name+=Name<T46>::getName()+","+Name<T47>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42,
						    T43,T44,T45,T46,T47>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,
	     typename T5,typename T6,typename T7,typename T8,
	     typename T9,typename T10,typename T11,typename T12,
	     typename T13,typename T14,typename T15,typename T16,
	     typename T17,typename T18,typename T19,typename T20,
	     typename T21,typename T22,typename T23,typename T24,
	     typename T25,typename T26,typename T27,typename T28,
	     typename T29,typename T30,typename T31,typename T32,
	     typename T33,typename T34,typename T35,typename T36,
	     typename T37,typename T38,typename T39,typename T40,
	     typename T41,typename T42,typename T43,typename T44,
	     typename T45,typename T46,typename T47,typename T48>
    struct GenType<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,
					 T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,
					 T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,
					 T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,
					 T41,T42,T43,T44,T45,T46,T47,T48,GTEnd>

					   : public GenTypeBase<typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,
												      T5,T6,T7,T8,
												      T9,T10,T11,T12,
												      T13,T14,T15,T16,
												      T17,T18,T19,T20,
												      T21,T22,T23,T24,
												      T25,T26,T27,T28,
												      T29,T30,T31,T32,
												      T33,T34,T35,T36,
												      T37,T38,T39,T40,
												      T41,T42,T43,T44,
												      T45,T46,T47,T48>::type>
    {
      /*!
       * \brief  Default Constructor.
       */
      GenType()
      {}

      /*!
       * \brief Copy Constructor.
       * \param const GenType&, initial value.
       */
      GenType(const GenType& src)
	: Base(src)
      {}

      /*!
       * \brief  Constructor from T0.
       * \param  const T0&, initial value
       */
      GenType(const T0& v)
      {
	this->index = 0;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T0(v);
      }
      /*!
       * \brief  Constructor from T1.
       * \param  const T1&, initial value
       */
      GenType(const T1& v)
      {
	this->index = 1;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T1(v);
      }
      /*!
       * \brief  Constructor from T2.
       * \param  const T2&, initial value
       */
      GenType(const T2& v)
      {
	this->index = 2;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T2(v);
      }
      /*!
       * \brief  Constructor from T3.
       * \param  const T3&, initial value
       */
      GenType(const T3& v)
      {
	this->index = 3;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T3(v);
      }
      /*!
       * \brief  Constructor from T4.
       * \param  const T4&, initial value
       */
      GenType(const T4& v)
      {
	this->index = 4;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T4(v);
      }
      /*!
       * \brief  Constructor from T5.
       * \param  const T5&, initial value
       */
      GenType(const T5& v)
      {
	this->index = 5;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T5(v);
      }
      /*!
       * \brief  Constructor from T6.
       * \param  const T6&, initial value
       */
      GenType(const T6& v)
      {
	this->index = 6;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T6(v);
      }
      /*!
       * \brief  Constructor from T7.
       * \param  const T7&, initial value
       */
      GenType(const T7& v)
      {
	this->index = 7;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T7(v);
      }
      /*!
       * \brief  Constructor from T8.
       * \param  const T8&, initial value
       */
      GenType(const T8& v)
      {
	this->index = 8;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T8(v);
      }
      /*!
       * \brief  Constructor from T9.
       * \param  const T9&, initial value
       */
      GenType(const T9& v)
      {
	this->index = 9;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T9(v);
      }
      /*!
       * \brief  Constructor from T10.
       * \param  const T10&, initial value
       */
      GenType(const T10& v)
      {
	this->index = 10;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T10(v);
      }
      /*!
       * \brief  Constructor from T11.
       * \param  const T11&, initial value
       */
      GenType(const T11& v)
      {
	this->index = 11;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T11(v);
      }
      /*!
       * \brief  Constructor from T12.
       * \param  const T12&, initial value
       */
      GenType(const T12& v)
      {
	this->index = 12;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T12(v);
      }
      /*!
       * \brief  Constructor from T13.
       * \param  const T13&, initial value
       */
      GenType(const T13& v)
      {
	this->index = 13;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T13(v);
      }
      /*!
       * \brief  Constructor from T14.
       * \param  const T14&, initial value
       */
      GenType(const T14& v)
      {
	this->index = 14;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T14(v);
      }
      /*!
       * \brief  Constructor from T15.
       * \param  const T15&, initial value
       */
      GenType(const T15& v)
      {
	this->index = 15;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T15(v);
      }
      /*!
       * \brief  Constructor from T16.
       * \param  const T16&, initial value
       */
      GenType(const T16& v)
      {
	this->index = 16;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T16(v);
      }
      /*!
       * \brief  Constructor from T17.
       * \param  const T17&, initial value
       */
      GenType(const T17& v)
      {
	this->index = 17;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T17(v);
      }
      /*!
       * \brief  Constructor from T18.
       * \param  const T18&, initial value
       */
      GenType(const T18& v)
      {
	this->index = 18;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T18(v);
      }
      /*!
       * \brief  Constructor from T19.
       * \param  const T19&, initial value
       */
      GenType(const T19& v)
      {
	this->index = 19;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T19(v);
      }
      /*!
       * \brief  Constructor from T20.
       * \param  const T20&, initial value
       */
      GenType(const T20& v)
      {
	this->index = 20;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T20(v);
      }
      /*!
       * \brief  Constructor from T21.
       * \param  const T21&, initial value
       */
      GenType(const T21& v)
      {
	this->index = 21;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T21(v);
      }
      /*!
       * \brief  Constructor from T22.
       * \param  const T22&, initial value
       */
      GenType(const T22& v)
      {
	this->index = 22;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T22(v);
      }
      /*!
       * \brief  Constructor from T23.
       * \param  const T23&, initial value
       */
      GenType(const T23& v)
      {
	this->index = 23;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T23(v);
      }
      /*!
       * \brief  Constructor from T24.
       * \param  const T24&, initial value
       */
      GenType(const T24& v)
      {
	this->index = 24;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T24(v);
      }
      /*!
       * \brief  Constructor from T25.
       * \param  const T25&, initial value
       */
      GenType(const T25& v)
      {
	this->index = 25;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T25(v);
      }
      /*!
       * \brief  Constructor from T26.
       * \param  const T26&, initial value
       */
      GenType(const T26& v)
      {
	this->index = 26;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T26(v);
      }
      /*!
       * \brief  Constructor from T27.
       * \param  const T27&, initial value
       */
      GenType(const T27& v)
      {
	this->index = 27;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T27(v);
      }
      /*!
       * \brief  Constructor from T28.
       * \param  const T28&, initial value
       */
      GenType(const T28& v)
      {
	this->index = 28;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T28(v);
      }
      /*!
       * \brief  Constructor from T29.
       * \param  const T29&, initial value
       */
      GenType(const T29& v)
      {
	this->index = 29;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T29(v);
      }
      /*!
       * \brief  Constructor from T30.
       * \param  const T30&, initial value
       */
      GenType(const T30& v)
      {
	this->index = 30;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T30(v);
      }
      /*!
       * \brief  Constructor from T31.
       * \param  const T31&, initial value
       */
      GenType(const T31& v)
      {
	this->index = 31;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T31(v);
      }
      /*!
       * \brief  Constructor from T32.
       * \param  const T32&, initial value
       */
      GenType(const T32& v)
      {
	this->index = 32;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T32(v);
      }
      /*!
       * \brief  Constructor from T33.
       * \param  const T33&, initial value
       */
      GenType(const T33& v)
      {
	this->index = 33;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T33(v);
      }
      /*!
       * \brief  Constructor from T34.
       * \param  const T34&, initial value
       */
      GenType(const T34& v)
      {
	this->index = 34;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T34(v);
      }
      /*!
       * \brief  Constructor from T35.
       * \param  const T35&, initial value
       */
      GenType(const T35& v)
      {
	this->index = 35;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T35(v);
      }
      /*!
       * \brief  Constructor from T36.
       * \param  const T36&, initial value
       */
      GenType(const T36& v)
      {
	this->index = 36;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T36(v);
      }
      /*!
       * \brief  Constructor from T37.
       * \param  const T37&, initial value
       */
      GenType(const T37& v)
      {
	this->index = 37;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T37(v);
      }
      /*!
       * \brief  Constructor from T38.
       * \param  const T38&, initial value
       */
      GenType(const T38& v)
      {
	this->index = 38;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T38(v);
      }
      /*!
       * \brief  Constructor from T39.
       * \param  const T39&, initial value
       */
      GenType(const T39& v)
      {
	this->index = 39;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T39(v);
      }
      /*!
       * \brief  Constructor from T40.
       * \param  const T40&, initial value
       */
      GenType(const T40& v)
      {
	this->index = 40;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T40(v);
      }
      /*!
       * \brief  Constructor from T41.
       * \param  const T41&, initial value
       */
      GenType(const T41& v)
      {
	this->index = 41;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T41(v);
      }
      /*!
       * \brief  Constructor from T42.
       * \param  const T42&, initial value
       */
      GenType(const T42& v)
      {
	this->index = 42;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T42(v);
      }
      /*!
       * \brief  Constructor from T43.
       * \param  const T43&, initial value
       */
      GenType(const T43& v)
      {
	this->index = 43;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T43(v);
      }
      /*!
       * \brief  Constructor from T44.
       * \param  const T44&, initial value
       */
      GenType(const T44& v)
      {
	this->index = 44;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T44(v);
      }
      /*!
       * \brief  Constructor from T45.
       * \param  const T45&, initial value
       */
      GenType(const T45& v)
      {
	this->index = 45;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T45(v);
      }
      /*!
       * \brief  Constructor from T46.
       * \param  const T46&, initial value
       */
      GenType(const T46& v)
      {
	this->index = 46;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T46(v);
      }
      /*!
       * \brief  Constructor from T47.
       * \param  const T47&, initial value
       */
      GenType(const T47& v)
      {
	this->index = 47;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T47(v);
      }
      /*!
       * \brief  Constructor from T48.
       * \param  const T48&, initial value
       */
      GenType(const T48& v)
      {
	this->index = 48;
	void * p = reinterpret_cast<void*>(&(this->container.buffer[0]));
	// the magic of placement new...
	new (p) T48(v);
      }
      /*!
       * \brief Assignement operator.
       * \param  const GenType&, right hand side.
       * \return GenType&, a reference to this.
       */
      GenType&
      operator=(const GenType& src){
	Base::operator=(src);
	return *this;
      }

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("GenType<");
	name+=Name<T0>::getName()+",";
	name+=Name<T1>::getName()+","+Name<T2>::getName()+","+Name<T3>::getName()+",";
	name+=Name<T4>::getName()+","+Name<T5>::getName()+","+Name<T6>::getName()+",";
	name+=Name<T7>::getName()+","+Name<T8>::getName()+","+Name<T9>::getName()+",";
	name+=Name<T10>::getName()+","+Name<T11>::getName()+","+Name<T12>::getName()+",";
	name+=Name<T13>::getName()+","+Name<T14>::getName()+","+Name<T15>::getName()+",";
	name+=Name<T16>::getName()+","+Name<T17>::getName()+","+Name<T18>::getName()+",";
	name+=Name<T19>::getName()+","+Name<T20>::getName()+","+Name<T21>::getName()+",";
	name+=Name<T22>::getName()+","+Name<T23>::getName()+","+Name<T24>::getName()+",";
	name+=Name<T25>::getName()+","+Name<T26>::getName()+","+Name<T27>::getName()+",";
	name+=Name<T28>::getName()+","+Name<T29>::getName()+","+Name<T30>::getName()+",";
	name+=Name<T31>::getName()+","+Name<T32>::getName()+","+Name<T33>::getName()+",";
	name+=Name<T34>::getName()+","+Name<T35>::getName()+","+Name<T36>::getName()+",";
	name+=Name<T37>::getName()+","+Name<T38>::getName()+","+Name<T39>::getName()+",";
	name+=Name<T40>::getName()+","+Name<T41>::getName()+","+Name<T42>::getName()+",";
	name+=Name<T43>::getName()+","+Name<T44>::getName()+","+Name<T45>::getName()+",";
	name+=Name<T46>::getName()+","+Name<T47>::getName()+","+Name<T48>::getName()+">";
	return name;
      }

    private:
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,
						    T7,T8,T9,T10,T11,T12,
						    T13,T14,T15,T16,T17,T18,
						    T19,T20,T21,T22,T23,T24,
						    T25,T26,T27,T28,T29,T30,
						    T31,T32,T33,T34,T35,T36,
						    T37,T38,T39,T40,T41,T42,
						    T43,T44,T45,T46,T47,T48>::type ListOfTypes;
      typedef GenTypeBase<ListOfTypes> Base;

    };

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_GENTYPE_I_ */
