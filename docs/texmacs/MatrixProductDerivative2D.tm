<TeXmacs|1.0.7.15>

<style|<tuple|generic|maxima>>

<\body>
  <\session|maxima|default>
    <\output>
      \;

      Maxima 5.27.0 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>76) >
    <|unfolded-io>
      F:matrix([F0,F3,0],[F4,F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o76>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>77) >
    <|unfolded-io>
      G:matrix([G0,G3,0],[G4,G1,0],[0,0,G2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o77>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|G3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|G4>>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>78) >
    <|unfolded-io>
      prod:F.G
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o78>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>79) >
    <|unfolded-io>
      vF:transpose(matrix([F0,F1,F2]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o79>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>80) >
    <|unfolded-io>
      vG:transpose(matrix([G0,G1,G2]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o80>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>82) >
    <|unfolded-io>
      pv:transpose(matrix([prod[1,1],prod[2,2],prod[3,3],prod[1,2],prod[2,1]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o82>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>83) >
    <|unfolded-io>
      J1:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3),diff(pv,F4))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o83>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G4>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G3>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>|<cell|0>|<cell|0>>|<row|<cell|<with|math-font-family|rm|G3>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G4>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>84) >
    <|unfolded-io>
      J2:addcol(diff(pv,G0),diff(pv,G1),diff(pv,G2),diff(pv,G3),diff(pv,G4))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o84>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F3>>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|<with|math-font-family|rm|F4>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>85) >
    <|input>
      \;
    </input>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>49) >
    <|input>
      \;
    </input>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>50) >
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
    <associate|page-type|a2>
  </collection>
</initial>