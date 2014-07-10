/*! 
 * \file  FactorizedKriging1D2D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 mai 2010
 */

#include"TFEL/Math/FactorizedKriging1D2D.hxx"

namespace tfel
{

  namespace math
  {

    FactorizedKriging1D2D::FactorizedKriging1D2D(const std::vector<double>& vx,
						 const std::vector<double>& vy,
						 const std::vector<double>& vz,
						 const std::vector<double>& vv)
    {
      using namespace std;
      using namespace tfel::math;
      typedef FactorizedKriging<1u,2u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<2u,double> > > MyFactorizedKriging;
      using std::vector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n0 = KrigingUtilities::normalize(vx);
      this->a0 = n0.first;
      this->b0 = n0.second;
      pair<double,double> n1 = KrigingUtilities::normalize(vy);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vz);
      this->a2 = n2.first;
      this->b2 = n2.second;
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	tvector<2u> v;
	v(0) = this->a1*(*py)+b1;
	v(1) = this->a2*(*pz)+b2;
	MyFactorizedKriging::addValue(this->a0*(*px)+this->b0,v,*pv);
      }
      MyFactorizedKriging::buildInterpolation();
    }

    FactorizedKriging1D2D::FactorizedKriging1D2D(const tfel::math::vector<double>& vx,
						 const tfel::math::vector<double>& vy,
						 const tfel::math::vector<double>& vz,
						 const tfel::math::vector<double>& vv)
    {
      using namespace std;
      using namespace tfel::math;
      using tfel::math::vector;
      typedef FactorizedKriging<1u,2u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<2u,double> > > MyFactorizedKriging;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n0 = KrigingUtilities::normalize(vx);
      this->a0 = n0.first;
      this->b0 = n0.second;
      pair<double,double> n1 = KrigingUtilities::normalize(vy);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vz);
      this->a2 = n2.first;
      this->b2 = n2.second;
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	tvector<2u> v;
	v(0) = this->a1*(*py)+b1;
	v(1) = this->a2*(*pz)+b2;
	MyFactorizedKriging::addValue(this->a0*(*px)+this->b0,v,*pv);
      }
      MyFactorizedKriging::buildInterpolation();
    }
  
    double
    FactorizedKriging1D2D::operator()(const double vx,
				      const double vy,
				      const double vz) const
    {
      using namespace tfel::math;
      typedef FactorizedKriging<1u,2u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<2u,double> > > MyFactorizedKriging;
      tvector<2u> v;
      v(0) = this->a1*(vy)+b1;
      v(1) = this->a2*(vz)+b2;
      return MyFactorizedKriging::operator()(this->a0*vx+this->b0,v);
    } // end of FactorizedKriging1D2D::operator()

    FactorizedKriging1D2D::~FactorizedKriging1D2D()
    {} // end of FactorizedKriging1D2D::~FactorizedKriging1D2D

  } // end of namespace math

} // end of namespace tfel

