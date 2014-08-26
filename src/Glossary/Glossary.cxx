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

const GlossaryEntry Glossary::IrradiationTemperature("IrradiationTemperature","IrradiationTemperature",
"Kelvin","scalar",
"la température moyenne (temporelle) au cours de l'irradiation au point considéré",
"" /* no 'description' defined */,
"Cette température est définie ainsi~: ""@^separator^@"
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
"\\end{minipage} ""@^separator^@"
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

const GlossaryEntry Glossary::Temperature("Temperature","Temperature",
"Kelvin","scalar",
"la température",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"" /* no 'notes' defined */);

const GlossaryEntry Glossary::KelvinTemperature("KelvinTemperature","Temperature (K)",
"Kelvin","scalar",
"la température",
"" /* no 'description' defined */,
"" /* no 'latex description' defined */,
"Cette entréea été rajoutée par  ""@^separator^@"
"compatibilité avec les choix d'implantation  ""@^separator^@"
"de lois de comportements mécaniques ""@^separator^@"
"utilisés par le code Germinal ");

Glossary&
Glossary::getGlossary(void)
{
static Glossary glossary;
return glossary;
} // end of Glossary::getGlossary

Glossary::Glossary(){
this->insert(Glossary::IrradiationTemperature);
this->insert(Glossary::Temperature);
this->insert(Glossary::KelvinTemperature);
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
