/*! 
 * \file  FactorizedKriging1D3D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 mai 2010
 */

#include"TFEL/Math/FactorizedKriging1D3D.hxx"

namespace tfel
{

  namespace math
  {

    FactorizedKriging1D3D::FactorizedKriging1D3D(const std::vector<double>& vt,
						 const std::vector<double>& vx,
						 const std::vector<double>& vy,
						 const std::vector<double>& vz,
						 const std::vector<double>& vv)
    {
      using namespace std;
      using namespace tfel::math;
      typedef FactorizedKriging<1u,3u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<3u,double> > > FactorizedKriging;
      using std::vector;
      vector<double>::const_iterator pt;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      if((vt.size()!=vx.size())||
	 (vt.size()!=vy.size())||
	 (vt.size()!=vz.size())||
	 (vt.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n0 = KrigingUtilities::normalize(vt);
      this->a0 = n0.first;
      this->b0 = n0.second;
      pair<double,double> n1 = KrigingUtilities::normalize(vx);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vy);
      this->a2 = n2.first;
      this->b2 = n2.second;
      pair<double,double> n3 = KrigingUtilities::normalize(vz);
      this->a3 = n3.first;
      this->b3 = n3.second;
      for(pt=vt.begin(),px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  pt!=vt.end();++pt,++px,++py,++pz,++pv){
	tvector<3u> v;
	v(0) = this->a1*(*px)+b1;
	v(1) = this->a2*(*py)+b2;
	v(2) = this->a3*(*pz)+b3;
	FactorizedKriging::addValue(this->a0*(*pt)+b0,v,*pv);
      }
      FactorizedKriging::buildInterpolation();
    }

    FactorizedKriging1D3D::FactorizedKriging1D3D(const tfel::math::vector<double>& vt,
						 const tfel::math::vector<double>& vx,
						 const tfel::math::vector<double>& vy,
						 const tfel::math::vector<double>& vz,
						 const tfel::math::vector<double>& vv)
    {
      using namespace std;
      using namespace tfel::math;
      using tfel::math::vector;
      typedef FactorizedKriging<1u,3u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<3u,double> > > FactorizedKriging;
      vector<double>::const_iterator pt;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      if((vt.size()!=vx.size())||
	 (vt.size()!=vy.size())||
	 (vt.size()!=vz.size())||
	 (vt.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n0 = KrigingUtilities::normalize(vt);
      this->a0 = n0.first;
      this->b0 = n0.second;
      pair<double,double> n1 = KrigingUtilities::normalize(vx);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vy);
      this->a2 = n2.first;
      this->b2 = n2.second;
      pair<double,double> n3 = KrigingUtilities::normalize(vz);
      this->a3 = n3.first;
      this->b3 = n3.second;
      for(pt=vt.begin(),px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  pt!=vt.end();++pt,++px,++py,++pz,++pv){
	tvector<3u> v;
	v(0) = this->a1*(*px)+b1;
	v(1) = this->a2*(*py)+b2;
	v(2) = this->a3*(*pz)+b3;
	FactorizedKriging::addValue(this->a0*(*pt)+b0,v,*pv);
      }
      FactorizedKriging::buildInterpolation();
    }
  
    double
    FactorizedKriging1D3D::operator()(const double vt,
				      const double vx,
				      const double vy,
				      const double vz) const
    {
      using namespace tfel::math;
      typedef FactorizedKriging<1u,3u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<3u,double> > > FactorizedKriging;
      tvector<3u> v;
      v(0) = this->a1*(vx)+b1;
      v(1) = this->a2*(vy)+b2;
      v(2) = this->a3*(vz)+b3;
      return FactorizedKriging::operator()(this->a0*vt+this->b0,v);
    } // end of FactorizedKriging1D3D::operator()

    FactorizedKriging1D3D::~FactorizedKriging1D3D()
    {} // end of FactorizedKriging1D3D::~FactorizedKriging1D3D

  } // end of namespace math

} // end of namespace tfel

