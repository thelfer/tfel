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
      for(pt=vt.begin(),px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  pt!=vt.end();++pt,++px,++py,++pz,++pv){
	tvector<3u> v;
	v(0) = *px;
	v(1) = *py;
	v(2) = *pz;
	FactorizedKriging::addValue(*pt,v,*pv);
      }
      FactorizedKriging::buildInterpolation();
    }

    FactorizedKriging1D3D::FactorizedKriging1D3D(const tfel::math::vector<double>& vt,
						 const tfel::math::vector<double>& vx,
						 const tfel::math::vector<double>& vy,
						 const tfel::math::vector<double>& vz,
						 const tfel::math::vector<double>& vv)
    {
      using namespace tfel::math;
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
      for(pt=vt.begin(),px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  pt!=vt.end();++pt,++px,++py,++pz,++pv){
	tvector<3u> v;
	v(0) = *px;
	v(1) = *py;
	v(2) = *pz;
	FactorizedKriging::addValue(*pt,v,*pv);
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
      v(0) = vx;
      v(1) = vy;
      v(2) = vz;
      return FactorizedKriging::operator()(vt,v);
    } // end of FactorizedKriging1D3D::operator()

    FactorizedKriging1D3D::~FactorizedKriging1D3D()
    {} // end of FactorizedKriging1D3D::~FactorizedKriging1D3D

  } // end of namespace math

} // end of namespace tfel

