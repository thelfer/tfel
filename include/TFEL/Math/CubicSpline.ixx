/*!
 * \file   CubicSpline.ixx
 * \author Castelier Etienne
 * \date   07/08/2007
 */

#ifndef _LIB_TFEL_MATH_CUBICSPLINE_IXX
#define _LIB_TFEL_MATH_CUBICSPLINE_IXX 1

#include<limits>
#include<algorithm>

namespace tfel
{

  namespace math
  {

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
      if(px==pxe){
	throw(CubicSplineInvalidAbscissaVectorSize());
      }
      this->values.clear();
      Point p;
      p.x = *px;
      p.y = *py;
      this->values.push_back(p);
      px2 = px;
      ++px;
      ++py;
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
    void
    CubicSpline<real,value>::setCollocationPoints(const tfel::math::vector<real>& x,
						  const tfel::math::vector<value>& y)
    {
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
      using std::vector;
      if(this->values.size()==1){
	this->values[0].d = value(real(0));
      }{
	typename vector<real>::size_type i;
	typename vector<Point>::iterator p;
	typename vector<value>::const_iterator pd;
	typename vector<real>::size_type s = this->values.size()-1u;
	vector<real>  md(s+1u); // main  diagonal
	vector<real>  mu(s);    // upper diagonal
	vector<value> d(s+1u);
	real  ho = real(0);
	value uo = value(real(0));
	for(i=0;i!=s;++i){
	  const real  hn = 1./(this->values[i+1].x-this->values[i].x);
	  const value un = 3.*hn*hn*(this->values[i+1].y-this->values[i].y);
	  mu[i] = hn; 
	  md[i] = 2.*(hn+ho); 
	  d[i]  = un+uo;
	  uo=un;
	  ho=hn;
	}
	md[s] = 2.*ho;
	d[s]  = uo;
	solveMatrix(d,&mu[0],&md[0]);
	for(p=this->values.begin(),pd=d.begin();
	    p!=this->values.end();++p,++pd){
	  p->d = *pd;
	}
      }
    }
          
    template<typename real,
	     typename value>
    void CubicSpline<real,value>::solveMatrix(std::vector<value>& vx,
					      const real * const c,
					      real * const b)
    {
      using namespace std;
      using std::vector;
      typename std::vector<typename CubicSpline<real,value>::Point>::size_type i;
      typename std::vector<typename CubicSpline<real,value>::Point>::size_type n;
      n = vx.size();
      vector<value> v(n);
      v.swap(vx);
      value * const x = &vx[0];
      for (i = 1; i < n; i++) {
	real m = c[i-1]/b[i-1];
	b[i] = b[i] - m*c[i-1];
	v[i] = v[i] - m*v[i-1];
      }
      x[n-1] = v[n-1]/b[n-1];
      i  = n;
      i -= 2u;
      for (; i != 0; i--){
	if(abs(b[i])<100.*numeric_limits<real>::min()){
	  throw(CubicSplineNullPivot());
	}
	x[i]=(v[i]-c[i]*x[i+1])/b[i];
      }
      if(abs(b[0])<100.*numeric_limits<real>::min()){
	throw(CubicSplineNullPivot());
      }
      x[0]=(v[0]-c[0]*x[0+1])/b[0];
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
