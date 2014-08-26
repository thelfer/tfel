/*!
 *\file   Glossary.cxx
 *\author Helfer Thomas
 *\date   09/06/14
*/

#include<stdexcept>

#include"TFEL/Glossary/Glossary.hxx"

namespace tfel
{
namespace glossary
{

const GlossaryEntry Glossary::AxialStrain("AxialStrain","AxialStrain",
" ","scalaire",
"la déformation axiale (cette grandeur n'a de sens que pour les calculs plan généralisé)",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ConvectiveHeatTransferCoefficient("ConvectiveHeatTransferCoefficient","ConvectiveHeatTransferCoefficient",
"W.m^{-2}.K^{-1}","scalaire",
"le coefficient d'échange par convection",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::CylindricalStress("CylindricalStress","CylindricalStress",
"Pa","tensorielle",
"les contraintes exprimées dans le repère cylindrique",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::Displacement("Displacement","Displacement",
"m","vectorielle",
"le déplacement",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ExternalDisplacement("ExternalDisplacement","ExternalDisplacement",
"m","scalaire",
"un déplacement imposé.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"L'utilisation de ce nom pour imposer un déplacement n'est pas ""@^separator^@"
"obligatoire. ");

const GlossaryEntry Glossary::ExternalPressure("ExternalPressure","ExternalPressure",
"Pa","scalaire",
"la pression imposée.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"L'utilisation de ce nom pour imposer une pression n'est pas ""@^separator^@"
"obligatoire, l'utilisateur pouvant utiliser un nom différent lors de ""@^separator^@"
"la définition de la condition aux limites.. ");

const GlossaryEntry Glossary::ExternalTemperature("ExternalTemperature","ExternalTemperature",
"K","scalaire",
"la température imposée.",
"" /* no 'description' defined */,
"L'utilisation de ce nom pour imposer une température n'est pas ""@^separator^@"
"obligatoire. ",
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ExternalTraction("ExternalTraction","ExternalTraction",
"Pa","scalaire",
"une contrainte de traction imposée.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"L'utilisation de ce nom pour imposer une traction n'est pas ""@^separator^@"
"obligatoire, l'utilisateur pouvant utiliser un nom différent lors de ""@^separator^@"
"la définition de la condition aux limites. ");

const GlossaryEntry Glossary::FirstMaterialRadius("FirstMaterialRadius","FirstMaterialRadius",
"m","scalaire",
"le rayon du premier matériau dans le cas d'une condition d'échange entre deux matériaux (le choix du \\og~second~\\fg matériau est fait par l'utilisateur)",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::FirstMaterialTemperature("FirstMaterialTemperature","FirstMaterialTemperature",
"K","scalaire",
"la température sur le \\og~premier~\\fg matériau d'une condition d'échange (le choix de ce \\og~premier~\\fg matériau est fait par l'utilisateur)",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::GaseousSwelling("GaseousSwelling","GaseousSwelling",
" ","scalaire",
"un gonflement imposé dû à des produits de fission gazeux.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"L'utilisation de ce nom pour imposer un ""@^separator^@"
"gonflement n'est pas obligatoire ");

const GlossaryEntry Glossary::HeatFlux("HeatFlux","HeatFlux",
"J.m^{-2}.s^{-1}","scalaire",
"la projection du flux de chaleur sur la normale à une surface (uniquement disponible pour les matériaux isotropes)",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::HydrostaticPressure("HydrostaticPressure","HydrostaticPressure",
"Pa","tensorielle",
"la pression hydrostatique (égale au tiers de la trace du tenseur des contraintes)",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::IrradiationInducedSwelling("IrradiationInducedSwelling","IrradiationInducedSwelling",
" ","scalaire",
"un gonflement imposé  dû à des dégâts d'irradiation.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"L'utilisation de ce nom pour imposer ""@^separator^@"
"un gonflement n'est pas obligatoire ");

const GlossaryEntry Glossary::IrradiationTemperature("IrradiationTemperature","IrradiationTemperature",
"K","scalaire",
"la température moyenne (dans le temps) au cours de l'irradiation.",
"" /* no 'description' defined */,
"Cette température est ""@^separator^@"
"définie ainsi~: ""@^separator^@"
"\\[ ""@^separator^@"
"\\average{T}\\paren{t,\\vec{r}}  = \\Frac{1}{t-t_{0}}\\int_{t_{0}}^{t}T\\paren{u,\\vec{r}}\\,\\dtot\\, u ""@^separator^@"
"\\] ""@^separator^@"
"où ""@^separator^@"
"\\begin{minipage}[t]{0.8\\linewidth} ""@^separator^@"
"\\begin{itemize} ""@^separator^@"
"\\item \\(T\\paren{t,\\vec{r}}\\) est la valeur à un instant \\(t\\) de ""@^separator^@"
"la température au point \\(\\vec{r}\\)~; ""@^separator^@"
"\\item \\(t_{0}\\) est la date du début de calcul~; ""@^separator^@"
"\\item \\(t\\) est la date courante~; ""@^separator^@"
"\\end{itemize} ""@^separator^@"
"\\end{minipage}\\\\  ""@^separator^@"
"En pratique, l'intégrale temporelle est évaluée de manière incrémentale ainsi~: ""@^separator^@"
"\\[ ""@^separator^@"
"\\begin{aligned} ""@^separator^@"
"\\average{T}\\paren{t+dt,\\vec{r}}  &= \\Frac{1}{t+dt-t_{0}}\\int_{t_{0}}^{t+dt}T\\paren{u,\\vec{r}}\\,\\dtot\\, u \\\\  ""@^separator^@"
"&= \\Frac{1}{t+dt-t_{0}}\\left[\\int_{t_{0}}^{t}T\\paren{u,\\vec{r}}\\,\\dtot\\, u+\\int_{t}^{t+dt}T\\paren{u,\\vec{r}}\\,\\dtot\\, u\\right] \\\\  ""@^separator^@"
"&= \\Frac{1}{t+dt-t_{0}}\\left[\\paren{t-t_{0}}\\,\\average{T}\\paren{t,\\vec{r}}+\\int_{t}^{t+dt}T\\paren{u,\\vec{r}}\\,\\dtot\\, u\\right] \\\\  ""@^separator^@"
"&\\approx \\Frac{1}{t+dt-t_{0}}\\left[\\paren{t-t_{0}}\\,\\average{T}\\paren{t,\\vec{r}}+\\Frac{dt}{2}\\left[T\\paren{t,\\vec{r}}+T\\paren{t+dt,\\vec{r}}\\right]\\right] \\\\  ""@^separator^@"
"\\end{aligned} ""@^separator^@"
"\\] ",
"Ce mode de calcul peut conduire à de légères erreurs numériques ");

const GlossaryEntry Glossary::KelvinTemperature("KelvinTemperature","KelvinTemperature",
"K","scalaire",
"la température.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"Cette entrée a été rajoutée par compatibilité avec les choix d'implantation de lois de comportements mécaniques utilisés par le code Germinal ");

const GlossaryEntry Glossary::MassDensity("MassDensity","MassDensity",
"kg/m^{3}","scalaire",
"la densité massique",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::MeanIrradiationTemperature("MeanIrradiationTemperature","MeanIrradiationTemperature",
"K","scalaire",
"la température moyenne dans le temps et dans un domaine \\(\\Omega\\) donné.",
"" /* no 'description' defined */,
"Cette ""@^separator^@"
"température est définie ainsi~: ""@^separator^@"
"\\[ ""@^separator^@"
"\\average{T}\\paren{t} = ""@^separator^@"
"\\Frac{1}{t-t_{0}}\\Frac{1}{\\int_{\\Omega}\\dtot\\,V}\\int_{t_{0}}^{t}\\paren{\\int_{\\Omega}T\\paren{u,\\vec{r}}\\,\\dtot\\, ""@^separator^@"
"V} ""@^separator^@"
"\\] ""@^separator^@"
"où \\(T\\paren{t,\\vec{r}}\\) est la valeur à un instant \\(t\\) de la ""@^separator^@"
"température au point \\(\\vec{r}\\). ",
"En pratique, l'intégrale spatiale est calculée à l'aide de la ""@^separator^@"
"discrétisation par éléments finis et l'intégrale temporelle est ""@^separator^@"
"calculée de manière incrémentale ");

const GlossaryEntry Glossary::MeanTemperature("MeanTemperature","MeanTemperature",
"K","scalaire",
"la température moyenne dans un domaine \\(\\Omega\\) donné. ",
"" /* no 'description' defined */,
"Cette température est définie ainsi~: ""@^separator^@"
"\\[ ""@^separator^@"
"\\average{T}\\paren{t}  = \\Frac{1}{\\int_{\\Omega}\\dtot\\,V}\\int_{\\Omega}T\\paren{t,\\vec{r}}\\,\\dtot\\, V ""@^separator^@"
"\\] ""@^separator^@"
"où \\(T\\paren{t,\\vec{r}}\\) est la valeur à un instant \\(t\\) de la ""@^separator^@"
"température au point \\(\\vec{r}\\). ",
"En pratique, l'intégrale spatiale ""@^separator^@"
"est calculée à l'aide de la discrétisation par éléments finis ");

const GlossaryEntry Glossary::OrthotropicAxisX1("OrthotropicAxisX1","OrthotropicAxisX1",
" ","scalaire",
"la première coordonnée du premier axe d'orthotropie.",
"" /* no 'description' defined */,
"Cette quantité est automatiquement ""@^separator^@"
"calculée par Licos à partir de la définition du matériau ",
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::OrthotropicAxisX2("OrthotropicAxisX2","OrthotropicAxisX2",
" ","scalaire",
"la première coordonnée du second axe d'orthotropie.",
"" /* no 'description' defined */,
"Cette quantité est automatiquement ""@^separator^@"
"calculée par Licos à partir de la définition du matériau ",
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::OrthotropicAxisY1("OrthotropicAxisY1","OrthotropicAxisY1",
" ","scalaire",
"la deuxième coordonnée du premier axe d'orthotropie.",
"" /* no 'description' defined */,
"Cette quantité est automatiquement ""@^separator^@"
"calculée par Licos à partir de la définition du matériau ",
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::OrthotropicAxisY2("OrthotropicAxisY2","OrthotropicAxisY2",
" ","scalaire",
"la deuxième coordonnée du second axe d'orthotropie.",
"" /* no 'description' defined */,
"Cette quantité est automatiquement ""@^separator^@"
" calculée par Licos à partir de la définition du matériau ",
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::OrthotropicAxisZ1("OrthotropicAxisZ1","OrthotropicAxisZ1",
" ","scalaire",
"la troisième coordonnée du premier axe d'orthotropie.",
"" /* no 'description' defined */,
"Cette quantité est automatiquement ""@^separator^@"
"calculée par Licos à partir de la définition du matériau ",
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::OrthotropicAxisZ2("OrthotropicAxisZ2","OrthotropicAxisZ2",
" ","scalaire",
"la troisième coordonnée du second axe d'orthotropie.",
"" /* no 'description' defined */,
"Cette quantité est automatiquement ""@^separator^@"
"calculée par Licos à partir de la définition du matériau ",
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::PoissonRatio("PoissonRatio","PoissonRatio",
" ","scalaire",
"le coefficient de Poisson d'un matériau isotrope",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::PoissonRatio12("PoissonRatio12","PoissonRatio12",
" ","scalaire",
"le coefficient de Poisson d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::PoissonRatio13("PoissonRatio13","PoissonRatio13",
" ","scalaire",
"le coefficient de Poisson d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::PoissonRatio23("PoissonRatio23","PoissonRatio23",
" ","scalaire",
"le coefficient de Poisson d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::PowerDensity("PowerDensity","PowerDensity",
"W.m^{-3}","scalaire",
"la densité de puissance",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::Pressure("Pressure","Pressure",
"Pa","scalaire",
"la pression d'un gaz",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::PrincipalStress1("PrincipalStress1","PrincipalStress1",
"Pa","scalaire",
"la première contrainte principale.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"Son sens est précisé dans la notice ""@^separator^@"
"de l'opérateur prin de castem ""@^separator^@"
"(\\href{http://www-cast3m.cea.fr/cast3m/printnotice.do?name=prin}{http://www-cast3m.cea.fr}) ");

const GlossaryEntry Glossary::PrincipalStress2("PrincipalStress2","PrincipalStress2",
"Pa","scalaire",
"la deuxième contrainte principale.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"Son sens est précisé dans la notice de l'opérateur prin ""@^separator^@"
"de castem ""@^separator^@"
"(\\href{http://www-cast3m.cea.fr/cast3m/printnotice.do?name=prin}{http://www-cast3m.cea.fr/}) ");

const GlossaryEntry Glossary::PrincipalStress3("PrincipalStress3","PrincipalStress3",
"Pa","scalaire",
"la troisième contrainte principale.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"Son sens est précisé dans la notice de l'opérateur prin ""@^separator^@"
" de castem ""@^separator^@"
"(\\href{http://www-cast3m.cea.fr/cast3m/printnotice.do?name=prin}{http://www-cast3m.cea.fr/cast3m/}) ");

const GlossaryEntry Glossary::SecondMaterialRadius("SecondMaterialRadius","SecondMaterialRadius",
"m","scalaire",
"le rayon du second matériau dans le cas d'une condition d'échange entre deux matériaux (le choix du \\og~second~\\fg matériau est fait par l'utilisateur)",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::SecondMaterialTemperature("SecondMaterialTemperature","SecondMaterialTemperature",
"K","scalaire",
"la température sur le \\og~second~\\fg matériau d'une condition d'échange (le choix de ce \\og~second~\\fg matériau est fait par l'utilisateur)",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ShearModulus12("ShearModulus12","ShearModulus12",
"Pa","scalaire",
"le module de cisaillement d'un matériau orthotrope relatif aux première et deuxième directions d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ShearModulus13("ShearModulus13","ShearModulus13",
"Pa","scalaire",
"le module de cisaillement d'un matériau orthotrope relatif aux première et troisième directions d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ShearModulus23("ShearModulus23","ShearModulus23",
"Pa","scalaire",
"le module de cisaillement d'un matériau orthotrope relatif aux deuxième et troisième directions d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::SolidSwelling("SolidSwelling","SolidSwelling",
" ","scalaire",
"un gonflement imposé dû à des produits de fission solides.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"L'utilisation de ce nom ""@^separator^@"
"pour imposer un gonflement n'est pas obligatoire ");

const GlossaryEntry Glossary::SpecificHeat("SpecificHeat","SpecificHeat",
"J.kg^{-1}.K^{-1}","scalaire",
"la chaleur spécifique",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::SphericalStress("SphericalStress","SphericalStress",
"Pa","tensorielle",
"les contraintes exprimées dans le repère sphérique",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::Strain("Strain","Strain",
" ","tensorielle",
"la déformation",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::Stress("Stress","Stress",
"Pa","tensorielle",
"les contraintes",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::Swelling("Swelling","Swelling",
" ","scalaire",
"un gonflement imposé.",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"l'utilisation de ce nom pour imposer un gonflement n'est pas ""@^separator^@"
"obligatoire. ");

const GlossaryEntry Glossary::Temperature("Temperature","Temperature",
"K","scalaire",
"la température",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::TemperatureGradient("TemperatureGradient","TemperatureGradient",
"T.m^{-1}","vectorielle",
"le gradient de température",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalConductivity("ThermalConductivity","ThermalConductivity",
"W.m^{-1}","scalaire",
"la conductivité thermique d'un matériau isotrope",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalConductivity1("ThermalConductivity1","ThermalConductivity1",
"W.m^{-1}","scalaire",
"la conductivité thermique d'un matériau orthotrope suivant la première direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalConductivity2("ThermalConductivity2","ThermalConductivity2",
"W.m^{-1}","scalaire",
"la conductivité thermique d'un matériau orthotrope suivant la deuxième direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalConductivity3("ThermalConductivity3","ThermalConductivity3",
"W.m^{-1}","scalaire",
"la conductivité thermique d'un matériau orthotrope suivant la troisième direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalExpansion("ThermalExpansion","ThermalExpansion",
"K^{-1}","scalaire",
"le coefficient de dilatation linéique d'un matériau isotrope",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalExpansion1("ThermalExpansion1","ThermalExpansion1",
"K^{-1}","scalaire",
"le coefficient de dilatation linéique d'un matériau orthotrope suivant la première direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalExpansion2("ThermalExpansion2","ThermalExpansion2",
"K^{-1}","scalaire",
"le coefficient de dilatation linéique d'un matériau orthotrope suivant la deuxième direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::ThermalExpansion3("ThermalExpansion3","ThermalExpansion3",
"K^{-1}","scalaire",
"le coefficient de dilatation linéique d'un matériau orthotrope suivant la troisième direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::Trescastress("Trescastress","Trescastress",
"Pa","scalaire",
"la contrainte équivalente au sens de \\nom{Tresca}",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::UpdatedFirstCoordinate("UpdatedFirstCoordinate","UpdatedFirstCoordinate",
"m","scalaire",
"la première coordonnée actualisée",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::UpdatedSecondCoordinate("UpdatedSecondCoordinate","UpdatedSecondCoordinate",
"m","scalaire",
"la deuxième coordonnée actualisée",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::UpdatedThirdCoordinate("UpdatedThirdCoordinate","UpdatedThirdCoordinate",
"m","scalaire",
"la troisième coordonnée actualisée",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::VolumetricStrain("VolumetricStrain","VolumetricStrain",
" ","scalaire",
"la dilatation volumique",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::VonMisesStress("VonMisesStress","VonMisesStress",
"Pa","scalaire",
"la contrainte équivalente au sens de \\nom{von mises}",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::YoungModulus("YoungModulus","YoungModulus",
"Pa","scalaire",
"le module d'Young d'un matériau isotrope",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::YoungModulus1("YoungModulus1","YoungModulus1",
"Pa","scalaire",
"le module d'Young d'un matériau orthotrope suivant la première direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::YoungModulus2("YoungModulus2","YoungModulus2",
"Pa","scalaire",
"le module d'Young d'un matériau orthotrope suivant la deuxième direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::YoungModulus3("YoungModulus3","YoungModulus3",
"Pa","scalaire",
"le module d'Young d'un matériau orthotrope suivant la troisième direction d'orthotropie",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

Glossary&
Glossary::getGlossary(void)
{
static Glossary glossary;
return glossary;
} // end of Glossary::getGlossary

Glossary::Glossary(){
this->insert(Glossary::AxialStrain);
this->insert(Glossary::ConvectiveHeatTransferCoefficient);
this->insert(Glossary::CylindricalStress);
this->insert(Glossary::Displacement);
this->insert(Glossary::ExternalDisplacement);
this->insert(Glossary::ExternalPressure);
this->insert(Glossary::ExternalTemperature);
this->insert(Glossary::ExternalTraction);
this->insert(Glossary::FirstMaterialRadius);
this->insert(Glossary::FirstMaterialTemperature);
this->insert(Glossary::GaseousSwelling);
this->insert(Glossary::HeatFlux);
this->insert(Glossary::HydrostaticPressure);
this->insert(Glossary::IrradiationInducedSwelling);
this->insert(Glossary::IrradiationTemperature);
this->insert(Glossary::KelvinTemperature);
this->insert(Glossary::MassDensity);
this->insert(Glossary::MeanIrradiationTemperature);
this->insert(Glossary::MeanTemperature);
this->insert(Glossary::OrthotropicAxisX1);
this->insert(Glossary::OrthotropicAxisX2);
this->insert(Glossary::OrthotropicAxisY1);
this->insert(Glossary::OrthotropicAxisY2);
this->insert(Glossary::OrthotropicAxisZ1);
this->insert(Glossary::OrthotropicAxisZ2);
this->insert(Glossary::PoissonRatio);
this->insert(Glossary::PoissonRatio12);
this->insert(Glossary::PoissonRatio13);
this->insert(Glossary::PoissonRatio23);
this->insert(Glossary::PowerDensity);
this->insert(Glossary::Pressure);
this->insert(Glossary::PrincipalStress1);
this->insert(Glossary::PrincipalStress2);
this->insert(Glossary::PrincipalStress3);
this->insert(Glossary::SecondMaterialRadius);
this->insert(Glossary::SecondMaterialTemperature);
this->insert(Glossary::ShearModulus12);
this->insert(Glossary::ShearModulus13);
this->insert(Glossary::ShearModulus23);
this->insert(Glossary::SolidSwelling);
this->insert(Glossary::SpecificHeat);
this->insert(Glossary::SphericalStress);
this->insert(Glossary::Strain);
this->insert(Glossary::Stress);
this->insert(Glossary::Swelling);
this->insert(Glossary::Temperature);
this->insert(Glossary::TemperatureGradient);
this->insert(Glossary::ThermalConductivity);
this->insert(Glossary::ThermalConductivity1);
this->insert(Glossary::ThermalConductivity2);
this->insert(Glossary::ThermalConductivity3);
this->insert(Glossary::ThermalExpansion);
this->insert(Glossary::ThermalExpansion1);
this->insert(Glossary::ThermalExpansion2);
this->insert(Glossary::ThermalExpansion3);
this->insert(Glossary::Trescastress);
this->insert(Glossary::UpdatedFirstCoordinate);
this->insert(Glossary::UpdatedSecondCoordinate);
this->insert(Glossary::UpdatedThirdCoordinate);
this->insert(Glossary::VolumetricStrain);
this->insert(Glossary::VonMisesStress);
this->insert(Glossary::YoungModulus);
this->insert(Glossary::YoungModulus1);
this->insert(Glossary::YoungModulus2);
this->insert(Glossary::YoungModulus3);
} // end of Glossary::Glossary

void
Glossary::insert(const GlossaryEntry& e)
{
using namespace std;
if(!this->entries.insert(e).second){
string msg("Glossary::insert : ");
msg += "'"+e.getName()+"' already declared";
throw(runtime_error(msg));
}
} // end of Glossary::insert

bool
Glossary::contains(const std::string& w) const
{
using namespace std;
set<GlossaryEntry>::const_iterator p;
for(p=this->entries.begin();p!=this->entries.end();++p){
if(p->getName()==w){
return true;
}
}
return false;
} // end of Glossary::contains

} // end of namespace glossary

} // end of namespace tfel
