<TeXmacs|1.0.7.15>

<style|<tuple|generic|maxima>>

<\body>
  Formulae used to compute Green-Lagrange deformation tensor and the Cauchy
  stress tensor when the deformation gradient is given in fortran notations.

  These formulae are used in the UMATFiniteStrain.cxx file\ 

  <\session|maxima|default>
    <\output>
      Maxima 5.27.0 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      F:matrix([F0,F3,F6],[F1,F4,F7],[F2,F5,F8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>14) >
    <|unfolded-io>
      tF:transpose(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o14>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F7>>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      id:matrix([1,0,0],[0,1,0],[0,0,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >><matrix|<tformat|<table|<row|<cell|1>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|1>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|1>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      tF.F-id
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F3><rsup|2>-1>|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F6><rsup|2>-1>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>18) >
    <|unfolded-io>
      determinant(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o18>)
      >><with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>|)>-<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>|)>+<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>|)>*<with|math-font-family|rm|F6>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>19) >
    <|unfolded-io>
      p:matrix([p0,p3,p4],[p3,p1,p5],[p4,p5,p2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o19>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|p0>>|<cell|<with|math-font-family|rm|p3>>|<cell|<with|math-font-family|rm|p4>>>|<row|<cell|<with|math-font-family|rm|p3>>|<cell|<with|math-font-family|rm|p1>>|<cell|<with|math-font-family|rm|p5>>>|<row|<cell|<with|math-font-family|rm|p4>>|<cell|<with|math-font-family|rm|p5>>|<cell|<with|math-font-family|rm|p2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>25) >
    <|unfolded-io>
      s:F.p.tF
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o25>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>|<row|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>|<row|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>>|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F8>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>27) >
    <|unfolded-io>
      s[1,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o27>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>28) >
    <|unfolded-io>
      s[2,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o28>)
      >><with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      s[3,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F8>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|unfolded-io>
      s[1,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o30>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>31) >
    <|unfolded-io>
      s[1,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o31>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>32) >
    <|unfolded-io>
      s[2,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o32>)
      >><with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>33) >
    <|input>
      \;
    </input>
  </session>
</body>