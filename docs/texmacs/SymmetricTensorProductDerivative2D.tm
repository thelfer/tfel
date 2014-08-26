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
      <with|color|red|(<with|math-font-family|rm|%i>17) >
    <|unfolded-io>
      F:matrix([F0,F3/sqrt(2),F4/sqrt(2)],[F3/sqrt(2),F1,F5/sqrt(2)],[F4/sqrt(2),F5/sqrt(2),F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|F5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>20) >
    <|unfolded-io>
      G:matrix([G0,G3/sqrt(2),G4/sqrt(2)],[G3/sqrt(2),G1,G5/sqrt(2)],[G4/sqrt(2),G5/sqrt(2),G2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o20>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|G4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|G5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|G4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|G5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) >
    <|unfolded-io>
      prod:F.G
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>|2>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G4>|<sqrt|2>>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G2>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G4>|2>+<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G0>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G5>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>|2>+<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G2>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G4>|<sqrt|2>>+<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G3>|2>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G0>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G5>|<sqrt|2>>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>|2>+<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G1>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>|2>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      pv:transpose(matrix([prod[1,1],prod[2,2],prod[3,3],sqrt(2)*prod[1,2],sqrt(2)*prod[1,3],sqrt(2)*prod[3,2]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>|2>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>>|<row|<cell|<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>>|<row|<cell|<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>|2>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>|<row|<cell|<sqrt|2>*<around*|(|<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>|<sqrt|2>>|)>>>|<row|<cell|<sqrt|2>*<around*|(|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G5>|2>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G4>|<sqrt|2>>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G2>|<sqrt|2>>|)>>>|<row|<cell|<sqrt|2>*<around*|(|<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G5>|<sqrt|2>>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>|2>+<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G1>|<sqrt|2>>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      J1:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3),diff(pv,F4),diff(pv,F5))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G3>|2>>|<cell|<frac|<with|math-font-family|rm|G4>|2>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G3>|2>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G5>|2>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G4>|2>>|<cell|<frac|<with|math-font-family|rm|G5>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G1>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|G5>|2>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|G4>|<sqrt|2>>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G5>|2>>|<cell|<frac|<with|math-font-family|rm|G2>|<sqrt|2>>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G5>|<sqrt|2>>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G3>|2>>|<cell|<frac|<with|math-font-family|rm|G1>|<sqrt|2>>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|unfolded-io>
      J2:addcol(diff(pv,G0),diff(pv,G1),diff(pv,G2),diff(pv,G3),diff(pv,G4),diff(pv,G5))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o30>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F3>|2>>|<cell|<frac|<with|math-font-family|rm|F4>|2>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F3>|2>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F5>|2>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F4>|2>>|<cell|<frac|<with|math-font-family|rm|F5>|2>>>|<row|<cell|0>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F0>|<sqrt|2>>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F4>|2>>>|<row|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F4>|<sqrt|2>>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F0>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F3>|2>>>|<row|<cell|0>|<cell|<frac|<with|math-font-family|rm|F5>|<sqrt|2>>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F4>|2>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F2>|<sqrt|2>>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>31) >
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