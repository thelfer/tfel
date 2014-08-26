/*!
 *\file   PythonGlossary.cxx
 *\author Helfer Thomas
 *\date   09/06/14
*/

#include<boost/python.hpp>

#include"TFEL/Glossary/Glossary.hxx"

void
declareGlossary(void)
{
using namespace boost;
using namespace boost::python;
using namespace tfel::glossary;
class_<Glossary,noncopyable>("Glossary",no_init)
.def("getGlossary",Glossary::getGlossary,
return_value_policy<reference_existing_object>())
.staticmethod("getGlossary")
.def("contains",&Glossary::contains)
.def_readonly("AxialStrain",&Glossary::AxialStrain,
"la déformation axiale (cette grandeur n'a de sens que pour les calculs plan généralisé)")
.def_readonly("ConvectiveHeatTransferCoefficient",&Glossary::ConvectiveHeatTransferCoefficient,
"le coefficient d'échange par convection")
.def_readonly("CylindricalStress",&Glossary::CylindricalStress,
"les contraintes exprimées dans le repère cylindrique")
.def_readonly("Displacement",&Glossary::Displacement,
"le déplacement")
.def_readonly("ExternalDisplacement",&Glossary::ExternalDisplacement,
"un déplacement imposé.")
.def_readonly("ExternalPressure",&Glossary::ExternalPressure,
"la pression imposée.")
.def_readonly("ExternalTemperature",&Glossary::ExternalTemperature,
"la température imposée.")
.def_readonly("ExternalTraction",&Glossary::ExternalTraction,
"une contrainte de traction imposée.")
.def_readonly("FirstMaterialRadius",&Glossary::FirstMaterialRadius,
"le rayon du premier matériau dans le cas d'une condition d'échange entre deux matériaux (le choix du \\og~second~\\fg matériau est fait par l'utilisateur)")
.def_readonly("FirstMaterialTemperature",&Glossary::FirstMaterialTemperature,
"la température sur le \\og~premier~\\fg matériau d'une condition d'échange (le choix de ce \\og~premier~\\fg matériau est fait par l'utilisateur)")
.def_readonly("GaseousSwelling",&Glossary::GaseousSwelling,
"un gonflement imposé dû à des produits de fission gazeux.")
.def_readonly("HeatFlux",&Glossary::HeatFlux,
"la projection du flux de chaleur sur la normale à une surface (uniquement disponible pour les matériaux isotropes)")
.def_readonly("HydrostaticPressure",&Glossary::HydrostaticPressure,
"la pression hydrostatique (égale au tiers de la trace du tenseur des contraintes)")
.def_readonly("IrradiationInducedSwelling",&Glossary::IrradiationInducedSwelling,
"un gonflement imposé  dû à des dégâts d'irradiation.")
.def_readonly("IrradiationTemperature",&Glossary::IrradiationTemperature,
"la température moyenne (dans le temps) au cours de l'irradiation.")
.def_readonly("KelvinTemperature",&Glossary::KelvinTemperature,
"la température.")
.def_readonly("MassDensity",&Glossary::MassDensity,
"la densité massique")
.def_readonly("MeanIrradiationTemperature",&Glossary::MeanIrradiationTemperature,
"la température moyenne dans le temps et dans un domaine \\(\\Omega\\) donné.")
.def_readonly("MeanTemperature",&Glossary::MeanTemperature,
"la température moyenne dans un domaine \\(\\Omega\\) donné. ")
.def_readonly("OrthotropicAxisX1",&Glossary::OrthotropicAxisX1,
"la première coordonnée du premier axe d'orthotropie.")
.def_readonly("OrthotropicAxisX2",&Glossary::OrthotropicAxisX2,
"la première coordonnée du second axe d'orthotropie.")
.def_readonly("OrthotropicAxisY1",&Glossary::OrthotropicAxisY1,
"la deuxième coordonnée du premier axe d'orthotropie.")
.def_readonly("OrthotropicAxisY2",&Glossary::OrthotropicAxisY2,
"la deuxième coordonnée du second axe d'orthotropie.")
.def_readonly("OrthotropicAxisZ1",&Glossary::OrthotropicAxisZ1,
"la troisième coordonnée du premier axe d'orthotropie.")
.def_readonly("OrthotropicAxisZ2",&Glossary::OrthotropicAxisZ2,
"la troisième coordonnée du second axe d'orthotropie.")
.def_readonly("PoissonRatio",&Glossary::PoissonRatio,
"le coefficient de Poisson d'un matériau isotrope")
.def_readonly("PoissonRatio12",&Glossary::PoissonRatio12,
"le coefficient de Poisson d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie")
.def_readonly("PoissonRatio13",&Glossary::PoissonRatio13,
"le coefficient de Poisson d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie")
.def_readonly("PoissonRatio23",&Glossary::PoissonRatio23,
"le coefficient de Poisson d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie")
.def_readonly("PowerDensity",&Glossary::PowerDensity,
"la densité de puissance")
.def_readonly("Pressure",&Glossary::Pressure,
"la pression d'un gaz")
.def_readonly("PrincipalStress1",&Glossary::PrincipalStress1,
"la première contrainte principale.")
.def_readonly("PrincipalStress2",&Glossary::PrincipalStress2,
"la deuxième contrainte principale.")
.def_readonly("PrincipalStress3",&Glossary::PrincipalStress3,
"la troisième contrainte principale.")
.def_readonly("SecondMaterialRadius",&Glossary::SecondMaterialRadius,
"le rayon du second matériau dans le cas d'une condition d'échange entre deux matériaux (le choix du \\og~second~\\fg matériau est fait par l'utilisateur)")
.def_readonly("SecondMaterialTemperature",&Glossary::SecondMaterialTemperature,
"la température sur le \\og~second~\\fg matériau d'une condition d'échange (le choix de ce \\og~second~\\fg matériau est fait par l'utilisateur)")
.def_readonly("ShearModulus12",&Glossary::ShearModulus12,
"le module de cisaillement d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie")
.def_readonly("ShearModulus13",&Glossary::ShearModulus13,
"le module de cisaillement d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie")
.def_readonly("ShearModulus23",&Glossary::ShearModulus23,
"le module de cisaillement d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie")
.def_readonly("SolidSwelling",&Glossary::SolidSwelling,
"un gonflement imposé dû à des produits de fission solides.")
.def_readonly("SpecificHeat",&Glossary::SpecificHeat,
"la chaleur spécifique")
.def_readonly("SphericalStress",&Glossary::SphericalStress,
"les contraintes exprimées dans le repère sphérique")
.def_readonly("Strain",&Glossary::Strain,
"la déformation")
.def_readonly("Stress",&Glossary::Stress,
"les contraintes")
.def_readonly("Swelling",&Glossary::Swelling,
"un gonflement imposé.")
.def_readonly("Temperature",&Glossary::Temperature,
"la température")
.def_readonly("TemperatureGradient",&Glossary::TemperatureGradient,
"le gradient de température")
.def_readonly("ThermalConductivity",&Glossary::ThermalConductivity,
"la conductivité thermique d'un matériau isotrope")
.def_readonly("ThermalConductivity1",&Glossary::ThermalConductivity1,
"la conductivité thermique d'un matériau orthotrope suivant la première direction d'orthotropie")
.def_readonly("ThermalConductivity2",&Glossary::ThermalConductivity2,
"la conductivité thermique d'un matériau orthotrope suivant la deuxième direction d'orthotropie")
.def_readonly("ThermalConductivity3",&Glossary::ThermalConductivity3,
"la conductivité thermique d'un matériau orthotrope suivant la troisième direction d'orthotropie")
.def_readonly("ThermalExpansion",&Glossary::ThermalExpansion,
"le coefficient de dilatation linéique d'un matériau isotrope")
.def_readonly("ThermalExpansion1",&Glossary::ThermalExpansion1,
"le coefficient de dilatation linéique d'un matériau orthotrope suivant la première direction d'orthotropie")
.def_readonly("ThermalExpansion2",&Glossary::ThermalExpansion2,
"le coefficient de dilatation linéique d'un matériau orthotrope suivant la deuxième direction d'orthotropie")
.def_readonly("ThermalExpansion3",&Glossary::ThermalExpansion3,
"le coefficient de dilatation linéique d'un matériau orthotrope suivant la troisième direction d'orthotropie")
.def_readonly("Trescastress",&Glossary::Trescastress,
"la contrainte équivalente au sens de \\nom{Tresca}")
.def_readonly("UpdatedFirstCoordinate",&Glossary::UpdatedFirstCoordinate,
"la première coordonnée actualisée")
.def_readonly("UpdatedSecondCoordinate",&Glossary::UpdatedSecondCoordinate,
"la deuxième coordonnée actualisée")
.def_readonly("UpdatedThirdCoordinate",&Glossary::UpdatedThirdCoordinate,
"la troisième coordonnée actualisée")
.def_readonly("VolumetricStrain",&Glossary::VolumetricStrain,
"la dilatation volumique")
.def_readonly("VonMisesStress",&Glossary::VonMisesStress,
"la contrainte équivalente au sens de \\nom{von mises}")
.def_readonly("YoungModulus",&Glossary::YoungModulus,
"le module d'Young d'un matériau isotrope")
.def_readonly("YoungModulus1",&Glossary::YoungModulus1,
"le module d'Young d'un matériau orthotrope suivant la première direction d'orthotropie")
.def_readonly("YoungModulus2",&Glossary::YoungModulus2,
"le module d'Young d'un matériau orthotrope suivant la deuxième direction d'orthotropie")
.def_readonly("YoungModulus3",&Glossary::YoungModulus3,
"le module d'Young d'un matériau orthotrope suivant la troisième direction d'orthotropie")
;

}
