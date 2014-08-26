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
      <with|color|red|(<with|math-font-family|rm|%i>60) >
    <|unfolded-io>
      F:matrix([F0,F3,F5],[F4,F1,F7],[F6,F8,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o60>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>61) >
    <|unfolded-io>
      G:matrix([G0,G3,G5],[G4,G1,G7],[G6,G8,G2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o61>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|G3>>|<cell|<with|math-font-family|rm|G5>>>|<row|<cell|<with|math-font-family|rm|G4>>|<cell|<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|G7>>>|<row|<cell|<with|math-font-family|rm|G6>>|<cell|<with|math-font-family|rm|G8>>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>62) >
    <|unfolded-io>
      prod:F.G
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o62>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G6>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G8>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F5>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F7>*<with|math-font-family|rm|G6>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|F7>*<with|math-font-family|rm|G8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F7>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G6>+<with|math-font-family|rm|F8>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F6>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G8>+<with|math-font-family|rm|F6>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F8>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|F8>*<with|math-font-family|rm|G7>+<with|math-font-family|rm|F6>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>63) >
    <|unfolded-io>
      vF:transpose(matrix([F0,F1,F2,F3,F4,F5,F6,F7,F8]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o63>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F3>>>|<row|<cell|<with|math-font-family|rm|F4>>>|<row|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>64) >
    <|unfolded-io>
      vG:transpose(matrix([G0,G1,G2,G3,G4,G5,G6,G7,G8]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o64>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|G3>>>|<row|<cell|<with|math-font-family|rm|G4>>>|<row|<cell|<with|math-font-family|rm|G5>>>|<row|<cell|<with|math-font-family|rm|G6>>>|<row|<cell|<with|math-font-family|rm|G7>>>|<row|<cell|<with|math-font-family|rm|G8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>65) >
    <|unfolded-io>
      pv:transpose(matrix([prod[1,1],prod[2,2],prod[3,3],prod[1,2],prod[2,1],prod[1,3],prod[3,1],prod[2,3],prod[3,2]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o65>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G6>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|F7>*<with|math-font-family|rm|G8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F8>*<with|math-font-family|rm|G7>+<with|math-font-family|rm|F6>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G8>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F7>*<with|math-font-family|rm|G6>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F5>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G6>+<with|math-font-family|rm|F8>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F6>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F7>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G8>+<with|math-font-family|rm|F6>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F8>*<with|math-font-family|rm|G1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>66) >
    <|unfolded-io>
      J1:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3),diff(pv,F4),diff(pv,F5),diff(pv,F6),diff(pv,F7),diff(pv,F8))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o66>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G4>>|<cell|0>|<cell|<with|math-font-family|rm|G6>>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G3>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G8>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G5>>|<cell|0>|<cell|<with|math-font-family|rm|G7>>>|<row|<cell|<with|math-font-family|rm|G3>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>|<cell|<with|math-font-family|rm|G8>>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G4>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G6>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|G5>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G7>>|<cell|0>|<cell|<with|math-font-family|rm|G2>>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G6>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|<with|math-font-family|rm|G4>>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G7>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G5>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G8>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G3>>|<cell|0>|<cell|<with|math-font-family|rm|G1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>67) >
    <|unfolded-io>
      J2:addcol(diff(pv,G0),diff(pv,G1),diff(pv,G2),diff(pv,G3),diff(pv,G4),diff(pv,G5),diff(pv,G6),diff(pv,G7),diff(pv,G8))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o67>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>|<cell|<with|math-font-family|rm|F5>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|<with|math-font-family|rm|F4>>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F6>>|<cell|0>|<cell|<with|math-font-family|rm|F8>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|<with|math-font-family|rm|F7>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F5>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F6>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F8>>|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F7>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F4>>|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F8>>|<cell|0>|<cell|<with|math-font-family|rm|F6>>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>68) >
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