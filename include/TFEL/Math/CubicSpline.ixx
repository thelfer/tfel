/*!
 * \file   CubicSpline.ixx
 * \author Castelier Etienne
 * \date   07/08/2007
 */

#ifndef _LIB_TFEL_MATH_CUBICSPLINE_IXX
#define _LIB_TFEL_MATH_CUBICSPLINE_IXX 1

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel
{

  namespace math
  {

    /*!
     * an helper function used to multiply two objects
     *
     * \return the result of the operation
     * \param a : left operand
     * \param b : right operand
     *
     * \note partial specialisation are provided for tvector and
     * stensor
     */
    template<typename real>
    real
    CubicSplineMultiply(const real a,
		      const real b)
    {
      return a*b;
    } // end of CubicSplineMultiply

    /*!
     * an helper function used to multiply two tvector
     *
     * \return the result of the operation
     * \param a : left operand
     * \param b : right operand
     */
    template<unsigned short N,
	     typename real>
    tfel::math::tvector<N,real>
    CubicSplineMultiply(const tfel::math::tvector<N,real>& a,
		      const tfel::math::tvector<N,real>& b)
    {
      tfel::math::tvector<N,real> r;
      typename tfel::math::tvector<N,real>::size_type i;
      for(i=0;i!=N;++i){
	r[i] = a[i]*b[i];
      }
      return r;
    } // end of CubicSplineMultiply

    /*!
     * an helper function used to multiply two stensor
     *
     * \return the result of the operation
     * \param a : left operand
     * \param b : right operand
     */
    template<typename real>
    tfel::math::stensor<1u,real>
    CubicSplineMultiply(const tfel::math::stensor<1u,real>& a,
		      const tfel::math::stensor<1u,real>& b)
    {
      tfel::math::stensor<1u,real> r;
      r[0] = a[0]*b[0];
      r[1] = a[1]*b[1];
      r[2] = a[2]*b[2];
      return r;
    } // end of CubicSplineMultiply

    template<typename real>
    tfel::math::stensor<2u,real>
    CubicSplineMultiply(const tfel::math::stensor<2u,real>& a,
		      const tfel::math::stensor<2u,real>& b)
    {
      tfel::math::stensor<2u,real> r;
      r[0] = a[0]*b[0];
      r[1] = a[1]*b[1];
      r[2] = a[2]*b[2];
      r[3] = a[3]*b[3];
      return r;
    } // end of CubicSplineMultiply

    template<typename real>
    tfel::math::stensor<3u,real>
    CubicSplineMultiply(const tfel::math::stensor<3u,real>& a,
		      const tfel::math::stensor<3u,real>& b)
    {
      tfel::math::stensor<3u,real> r;
      r[0] = a[0]*b[0];
      r[1] = a[1]*b[1];
      r[2] = a[2]*b[2];
      r[3] = a[3]*b[3];
      r[4] = a[4]*b[4];
      r[5] = a[5]*b[5];
      return r;
    } // end of CubicSplineMultiply

    /*!
     * an helper function used to divide two objects
     *
     * \return the result of the operation
     * \param a : left operand
     * \param b : right operand
     *
     * \note partial specialisation are provided for tvector and
     * stensor
     */
    template<typename real>
    real
    CubicSplineDivide(const real a,
		      const real b)
    {
      return a/b;
    } // end of CubicSplineDivide

    /*!
     * an helper function used to divide two tvector
     *
     * \return the result of the operation
     * \param a : left operand
     * \param b : right operand
     */
    template<unsigned short N,
	     typename real>
    tfel::math::tvector<N,real>
    CubicSplineDivide(const tfel::math::tvector<N,real>& a,
		      const tfel::math::tvector<N,real>& b)
    {
      tfel::math::tvector<N,real> r;
      typename tfel::math::tvector<N,real>::size_type i;
      for(i=0;i!=N;++i){
	r[i] = a[i]/b[i];
      }
      return r;
    } // end of CubicSplineDivide

    /*!
     * an helper function used to divide two stensor
     *
     * \return the result of the operation
     * \param a : left operand
     * \param b : right operand
     */
    template<typename real>
    tfel::math::stensor<1u,real>
    CubicSplineDivide(const tfel::math::stensor<1u,real>& a,
		      const tfel::math::stensor<1u,real>& b)
    {
      tfel::math::stensor<1u,real> r;
      r[0] = a[0]/b[0];
      r[1] = a[1]/b[1];
      r[2] = a[2]/b[2];
      return r;
    } // end of CubicSplineDivide

    template<typename real>
    tfel::math::stensor<2u,real>
    CubicSplineDivide(const tfel::math::stensor<2u,real>& a,
		      const tfel::math::stensor<2u,real>& b)
    {
      tfel::math::stensor<2u,real> r;
      r[0] = a[0]/b[0];
      r[1] = a[1]/b[1];
      r[2] = a[2]/b[2];
      r[3] = a[3]/b[3];
      return r;
    } // end of CubicSplineDivide

    template<typename real>
    tfel::math::stensor<3u,real>
    CubicSplineDivide(const tfel::math::stensor<3u,real>& a,
		      const tfel::math::stensor<3u,real>& b)
    {
      tfel::math::stensor<3u,real> r;
      r[0] = a[0]/b[0];
      r[1] = a[1]/b[1];
      r[2] = a[2]/b[2];
      r[3] = a[3]/b[3];
      r[4] = a[4]/b[4];
      r[5] = a[5]/b[5];
      return r;
    } // end of CubicSplineDivide

    template<typename real,
	     typename value>
    bool
    CubicSpline<real,value>::PointComparator::operator()(const CubicSpline<real,value>::Point& p,
							 const real& x) const
    {
      return p.x < x ;
    } // end of CubicSpline<real,value>::Point

    template<typename real,
	     typename value>
    template<typename AIterator,
	     typename OIterator>
    void
    CubicSpline<real,value>::setCollocationPoints(AIterator px,
						  AIterator pxe,
						  OIterator py)
    {
      using namespace std;
      AIterator px2;
      size_t i;
      this->values.clear();
      Point p;
      p.x = *px;
      p.y = *py;
      this->values.push_back(p);
      px2 = px;
      ++px;
      ++py;
      i=1;
      while(px!=pxe){
	px2 = px-1u;
	if(*px2>=*px){
	  throw(CubicSplineUnorderedAbscissaVector());
	}
	p.x = *px;
	p.y = *py;
	this->values.push_back(p);
	px2 = px;
	++px;
	++py;
	++i;
      }
      if(i<1){
	throw(CubicSplineInvalidAbscissaVectorSize());
      }
      this->buildInterpolation();
    } // CubicSpline<real,value>::CubicSpline

    template<typename real,
	     typename value>
    void
    CubicSpline<real,value>::setCollocationPoints(const std::vector<real>& x,
						  const std::vector<value>& y)
    {
      using namespace std;
      this->values.clear();
      typename vector<real>::const_iterator px;
      typename vector<real>::const_iterator px2;
      typename vector<value>::const_iterator py;
      Point p;
      if(x.size()<1){
	throw(CubicSplineInvalidAbscissaVectorSize());
      }
      if(y.size()<1){
	throw(CubicSplineInvalidOrdinateVectorSize());
      }
      if(x.size()!=y.size()){
	throw(CubicSplineInvalidInputs());
      }
      px = x.begin();
      py = y.begin();
      p.x = *px;
      p.y = *py;
      this->values.push_back(p);
      ++px;
      ++py;
      while(px!=x.end()){
	px2 = px-1u;
	if(*px2>=*px){
	  throw(CubicSplineUnorderedAbscissaVector());
	}
	p.x = *px;
	p.y = *py;
	this->values.push_back(p);
	++px;
	++py;
      }
      this->buildInterpolation();
    } // CubicSpline<real,value>::CubicSpline

    template<typename real,
	     typename value>
    void CubicSpline<real,value>::buildInterpolation(void) {
      using namespace std;
      // Autres cas
      real go,gn,Gn;
      value uo,un,vn;
      value d_o,d_n,de_o,de_n,X;
      vector<value> del(this->values.size());
      typename vector<value>::iterator p1=del.begin();
      typename vector<value>::const_iterator p2;
      typename vector<Point>::iterator in= this->values.begin();
      typename vector<Point>::iterator ip;
      value d1;

      // Étape n = 1
      d_o=value(real(0));
      in->d=d_n=value(real(0));
      de_o=value(real(0));
      de_n=value(real(1));
      *(p1++)=de_n;

      gn=0;
      un=value(real(0));

      ip=in++;

      // Étapes Médianes 1 < n < N
      while (in != this->values.end()) {
	// Calcul g_{n-1}, u_{n-1}, G_{n-1}, v_{n-1}
	go=gn;
	gn=1/(in->x-ip->x);
	Gn=2*(go+gn);

	uo=un;
	un=3*(in->y-ip->y)*gn*gn;
	vn=uo+un;
    
	// Estimation de la dérivée d_n
	X=(vn-go*d_o-Gn*d_n)/gn;
	d_o=d_n;
	d_n=X;

	X=-(go*de_o+Gn*de_n)/gn;
	de_o=de_n;
	de_n=X;
	*(p1++)=de_n;

	// Estimation de d1
	d1=CubicSplineDivide(value(un-gn*(d_o+2*d_n)),value(gn*(de_o+2*de_n)));
	// Correction des dérivées
	for (ip=this->values.begin(), p2=del.begin(); ip != in; ++ip, ++p2) {
	  ip->d+=CubicSplineMultiply(*p2,d1);
	}

	// Réactualisation des dérivées d_{n-1}, d_{n}
	in->d=d_n=d_n+CubicSplineMultiply(de_n,d1);
	d_o=(in-1)->d;

	ip=in++;
      }
    }

    template<typename real,
	     typename value>
    void CubicSpline<real,value>::computeLocalCoefficients(value& a2,
							   value& a3,
							   const typename std::vector<typename CubicSpline<real,value>::Point>::const_iterator pa){
      using namespace std;
      const typename vector<Point>::const_iterator pb = pa+1;
      const real usL = 1/(pb->x-pa->x);
      const value Dy = (pb->y-pa->y)*usL;
      a2 = (3*Dy-pb->d-2*pa->d)*usL;
      a3 = (-2*Dy+pb->d+pa->d)*usL*usL;
    } // end of CubicSpline<real,value>::computeLocalCoefficient

    template<typename real,
	     typename value>
    value CubicSpline<real,value>::computeLocalIntegral(const real xa,
							const real xb,
							const typename std::vector<typename CubicSpline<real,value>::Point>::const_iterator pa){
      using namespace std;
      const value py = pa->y;
      const value d  = pa->d;
      value a2;
      value a3;
      CubicSpline<real,value>::computeLocalCoefficients(a2,a3,pa);
      const real x0 = xa - pa->x;
      const real x1 = xb - pa->x;
      return (3*a3*(x1*x1*x1*x1-x0*x0*x0*x0)
	      + 4*a2*(x1*x1*x1-x0*x0*x0)
	      + 6*d*(x1*x1-x0*x0)
	      + 12*py*(x1-x0))/12;
    } // end of 

    template<typename real,
	     typename value>
    value CubicSpline<real,value>::computeMeanValue(const real xa,
						    const real xb) const
    {
      return this->computeIntegral(xa,xb)/(xb-xa);
    } // end of value CubicSpline<real,value>::computeMeanValue

    template<typename real,
	     typename value>
    value CubicSpline<real,value>::computeIntegral(const real xa,
						   const real xb) const
    {
      using namespace std;
      typename vector<Point>::const_iterator pa;
      typename vector<Point>::const_iterator pb;
      if(this->values.empty()){
	throw(CubicSplineUninitialised());
      }
      if (this->values.size() == 1) {
	const value& f = values.back().y;
	return f*(xb-xa);
      }
      if(xb<xa){
	return -this->computeIntegral(xb,xa);
      }
      pa = lower_bound(this->values.begin(),this->values.end(),xa,PointComparator());
      pb = lower_bound(this->values.begin(),this->values.end(),xb,PointComparator());
      if(pa==pb){
	if(pb==this->values.begin()){
	  const real   xe = this->values.front().x;
	  const value& ye = this->values.front().y;
	  const value& df = this->values.front().d;
	  // l'équation de l'extrapoltion est :
	  // y = ye-df*xe + df*x
	  // l'intégrale est alors:
	  // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
	  return ye*(xb-xa)+0.5*df*((xb-xe)*(xb-xe)-(xa-xe)*(xa-xe));
	} else if(pb==this->values.end()){
	  const real   xe = this->values.back().x;
	  const value& ye = this->values.back().y;
	  const value& df = this->values.back().d;
	  // l'équation de l'extrapoltion est :
	  // y = ye-df*xe + df*x
	  // l'intégrale est alors:
	  // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
	  return ye*(xb-xa)+0.5*df*((xb-xe)*(xb-xe)-(xa-xe)*(xa-xe));
	} else {
	  pa = pb - 1;
	  // spline=pa->y+(x-pa->x)*(pa->d+(x-pa->x)*(a2+(x-pa->x)*a3));
	  //       =pa->y+(x-pa->x)*(pa->d+(x-pa->x)*(a2+(x-pa->x)*a3));
	  return CubicSpline<real,value>::computeLocalIntegral(xa,xb,pa);
	}
      }
      value s(real(0));
      if(pa==this->values.begin()){
	const real   xe = this->values.front().x;
	const value& ye = this->values.front().y;
	const value& df = this->values.front().d;
	// l'équation de l'extrapoltion est :
	// y = ye-df*xe + df*x
	// l'intégrale est alors:
	// (ye-df*xe)*(pa->a-xa)+0.5*df*(pa->a-xa)*(pa->a-xa)
	s += ye*(xe-xa)-0.5*df*((xa-xe)*(xa-xe));
      } else {
	s += CubicSpline<real,value>::computeLocalIntegral(xa,pa->x,pa-1);
      }
      if(pb==this->values.end()){
	const real   xe = this->values.back().x;
	const value& ye = this->values.back().y;
	const value& df = this->values.back().d;
	// l'équation de l'extrapoltion est :
	// y = ye-df*xe + df*x
	// l'intégrale est alors:
	// (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
	s += ye*(xb-xe)+0.5*df*((xb-xe)*(xb-xe));
	//	s += (ye-df*xe)*(xb-pb->x)+0.5*df*(xb-pb->x)*(xb-pb->x);
      } else {
	s += CubicSpline<real,value>::computeLocalIntegral((pb-1)->x,xb,pb-1);
      }
      --pb;
      while(pa!=pb){
	s += CubicSpline<real,value>::computeLocalIntegral(pa->x,(pa+1)->x,pa);
	++pa;
      }
      return s;
    } // end of CubicSpline<real,value>::computeIntegral


    // Valeur de la spline et de sa dérivée
    // Interpolation cubique
    // x : Abscisse où calculer la valeur de la spline
    // f : Valeur de la fonction
    // df : Valeur de la dérivée
    template<typename real,
	     typename value>
    void CubicSpline<real,value>::getValues(value& f,value& df,real x) const {
      using namespace std;
      typename vector<Point>::const_iterator in;
      if(this->values.empty()){
	throw(CubicSplineUninitialised());
      }
      // Cas d'un seul couple
      if (this->values.size() == 1) {
	f  = this->values[0].y;
	df = value(real(0));
	return;
      }
      in = lower_bound(this->values.begin(),this->values.end(),x,PointComparator());
      // Extrapolation
      if (in == this->values.begin()) {
	x-=in->x;
	df=in->d;
	f=in->y+x*df;
	return;
      }
      if (in == this->values.end()) {
	--in;
	x-=in->x;
	df=in->d;
	f=in->y+x*df;
	return;
      }
      typename vector<Point>::const_iterator ip=in-1;
      value a2;
      value a3;
      CubicSpline<real,value>::computeLocalCoefficients(a2,a3,ip);
      x-=ip->x;
      f=ip->y+x*(ip->d+x*(a2+x*a3));
      df=ip->d+x*(2*a2+x*3*a3);
    }

    template<typename real,
	     typename value>
    value
    CubicSpline<real,value>::operator()(const real x) const {
      return this->getValue(x);
    }

    // Valeur de la spline
    // Interpolation cubique
    // x : Abscisse où calculer la valeur de la spline
    template<typename real,
	     typename value>
    value
    CubicSpline<real,value>::getValue(real x) const {
      using namespace std;
      typename vector<Point>::const_iterator in;
      if(this->values.empty()){
	throw(CubicSplineUninitialised());
      }
      // Cas d'un seul couple
      if (this->values.size() == 1) {
	return this->values[0].y;
      }
      in = lower_bound(this->values.begin(),this->values.end(),x,PointComparator());
      // Extrapolation
      if (in == this->values.begin()) {
	x-=in->x;
	return in->y+x*in->d;
      }
      if (in == this->values.end()) {
	--in;
	x-=in->x;
	return in->y+x*in->d;
      }
      typename vector<Point>::const_iterator ip=in-1;
      value a2;
      value a3;
      CubicSpline<real,value>::computeLocalCoefficients(a2,a3,ip);
      x-=ip->x;
      return ip->y+x*(ip->d+x*(a2+x*a3));
    }

    // Valeur de la spline et de sa dérivée
    // Interpolation cubique
    // x : Abscisse où calculer la valeur de la spline
    // f : Valeur de la fonction
    // df : Valeur de la dérivée
    // d2f : Valeur de la dérivée seconde
    template<typename real,
	     typename value>
    void CubicSpline<real,value>::getValues(value& f,value& df,value& d2f,real x) const {
      using namespace std;
      if(this->values.empty()){
	throw(CubicSplineUninitialised());
      }
      typename vector<Point>::const_iterator in;
      in = this->values.begin();
      // Cas d'un seul couple
      if (this->values.size() == 1) {
	f   = this->values[0].y;
	df  = value(real(0));
	d2f = value(real(0));
	return;
      }
      in = lower_bound(this->values.begin(),this->values.end(),x,PointComparator());
      // Extrapolation
      if (in == this->values.begin()) {
	x-=in->x;
	df=in->d;
	f=in->y+x*df;
	d2f=value(real(0));
	return;
      }
      if (in == this->values.end()) {
	--in;
	x-=in->x;
	df=in->d;
	f=in->y+x*df;
	d2f=value(real(0));
	return;
      }
      typename vector<Point>::const_iterator ip=in-1;
      value a2;
      value a3;
      CubicSpline<real,value>::computeLocalCoefficients(a2,a3,ip);
      x-=ip->x;
      f=ip->y+x*(ip->d+x*(a2+x*a3));
      df=ip->d+x*(2*a2+x*3*a3);
      d2f=2*a2+x*6*a3;
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_CUBICSPLINE_IXX */
