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
      <with|color|red|(<with|math-font-family|rm|%i>73) >
    <|unfolded-io>
      a:matrix([a0,a3,a6],[a1,a4,a7],[a2,a5,a8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o73>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a0>>|<cell|<with|math-font-family|rm|a3>>|<cell|<with|math-font-family|rm|a6>>>|<row|<cell|<with|math-font-family|rm|a1>>|<cell|<with|math-font-family|rm|a4>>|<cell|<with|math-font-family|rm|a7>>>|<row|<cell|<with|math-font-family|rm|a2>>|<cell|<with|math-font-family|rm|a5>>|<cell|<with|math-font-family|rm|a8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>74) >
    <|unfolded-io>
      e:matrix([e0,e3/2,e4/2],[e3/2,e1,e5/2],[e4/2,e5/2,e2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o74>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|e0>>|<cell|<frac|<with|math-font-family|rm|e3>|2>>|<cell|<frac|<with|math-font-family|rm|e4>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|e3>|2>>|<cell|<with|math-font-family|rm|e1>>|<cell|<frac|<with|math-font-family|rm|e5>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|e4>|2>>|<cell|<frac|<with|math-font-family|rm|e5>|2>>|<cell|<with|math-font-family|rm|e2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>75) >
    <|unfolded-io>
      em:transpose(a).e.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o75>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>76) >
    <|unfolded-io>
      fullratsimp(em[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o76>)
      >><with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>77) >
    <|unfolded-io>
      fullratsimp(em[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o77>)
      >><with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>78) >
    <|unfolded-io>
      fullratsimp(em[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o78>)
      >><with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>79) >
    <|unfolded-io>
      fullratsimp(2*em[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o79>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>80) >
    <|unfolded-io>
      fullratsimp(2*em[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o80>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>81) >
    <|unfolded-io>
      fullratsimp(2*em[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o81>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>82) >
    <|unfolded-io>
      eg:a.e.transpose(a)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o82>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>83) >
    <|unfolded-io>
      fullratsimp(eg[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o83>)
      >><with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>84) >
    <|unfolded-io>
      fullratsimp(eg[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o84>)
      >><with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>85) >
    <|unfolded-io>
      fullratsimp(eg[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o85>)
      >><with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>86) >
    <|unfolded-io>
      fullratsimp(2*eg[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o86>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>87) >
    <|unfolded-io>
      fullratsimp(2*eg[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o87>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>88) >
    <|unfolded-io>
      fullratsimp(2*eg[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o88>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>89) >
    <|unfolded-io>
      s:matrix([s0,s3,s4],[s3,s1,s5],[s4,s5,s2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o89>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|s0>>|<cell|<with|math-font-family|rm|s3>>|<cell|<with|math-font-family|rm|s4>>>|<row|<cell|<with|math-font-family|rm|s3>>|<cell|<with|math-font-family|rm|s1>>|<cell|<with|math-font-family|rm|s5>>>|<row|<cell|<with|math-font-family|rm|s4>>|<cell|<with|math-font-family|rm|s5>>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>90) >
    <|unfolded-io>
      sm:transpose(a).s.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o90>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>|)>>>|<row|<cell|<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>91) >
    <|unfolded-io>
      fullratsimp(sm[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o91>)
      >>2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>92) >
    <|unfolded-io>
      fullratsimp(sm[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o92>)
      >>2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>93) >
    <|unfolded-io>
      fullratsimp(sm[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o93>)
      >>2*<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+2*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>94) >
    <|unfolded-io>
      fullratsimp(sm[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o94>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>95) >
    <|unfolded-io>
      fullratsimp(sm[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o95>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>96) >
    <|unfolded-io>
      fullratsimp(sm[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o96>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>97) >
    <|unfolded-io>
      sg:a.s.transpose(a)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o97>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>>>|<row|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>98) >
    <|unfolded-io>
      fullratsimp(sg[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o98>)
      >>2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>99) >
    <|unfolded-io>
      fullratsimp(sg[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o99>)
      >>2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>100) >
    <|unfolded-io>
      fullratsimp(sg[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o100>)
      >>2*<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>101) >
    <|unfolded-io>
      fullratsimp(sg[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o101>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>102) >
    <|unfolded-io>
      fullratsimp(sg[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o102>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>103) >
    <|unfolded-io>
      fullratsimp(sg[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o103>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>104) >
    <|unfolded-io>
      F:matrix([F0,F3,F6],[F1,F4,F7],[F2,F5,F8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o104>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>105) >
    <|unfolded-io>
      Fm:transpose(a).F.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o105>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>|)>>>|<row|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>|)>>>|<row|<cell|<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>106) >
    <|unfolded-io>
      Fm0:fullratsimp(Fm[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o106>)
      >><with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>107) >
    <|unfolded-io>
      fullratsimp(Fm[2,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o107>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>108) >
    <|unfolded-io>
      fullratsimp(Fm[3,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o108>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>109) >
    <|unfolded-io>
      fullratsimp(Fm[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o109>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>110) >
    <|unfolded-io>
      fullratsimp(Fm[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o110>)
      >><with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>111) >
    <|unfolded-io>
      fullratsimp(Fm[3,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o111>)
      >><with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>112) >
    <|unfolded-io>
      fullratsimp(Fm[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o112>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>113) >
    <|unfolded-io>
      fullratsimp(Fm[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o113>)
      >><with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>114) >
    <|unfolded-io>
      fullratsimp(Fm[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o114>)
      >><with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>115) >
    <|unfolded-io>
      Fg:fullratsimp(a.F.transpose(a))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o115>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>116) >
    <|unfolded-io>
      Fg[1,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o116>)
      >><with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>117) >
    <|unfolded-io>
      Fg[2,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o117>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>118) >
    <|unfolded-io>
      Fg[3,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o118>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>119) >
    <|unfolded-io>
      Fg[1,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o119>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>120) >
    <|unfolded-io>
      Fg[2,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o120>)
      >><with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>121) >
    <|unfolded-io>
      Fg[3,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o121>)
      >><with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>122) >
    <|unfolded-io>
      Fg[1,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o122>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>123) >
    <|unfolded-io>
      Fg[2,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o123>)
      >><with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>124) >
    <|unfolded-io>
      Fg[3,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o124>)
      >><with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>125) >
    <|unfolded-io>
      diff(Fg[1,1],F8)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o125>)
      >><with|math-font-family|rm|a6><rsup|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>127) >
    <|unfolded-io>
      s:matrix([s0,s3/sqrt(2),s4/sqrt(2)],[s3/sqrt(2),s1,s5/sqrt(2)],[s4/sqrt(2),s5/sqrt(2),s2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o127>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|s0>>|<cell|<frac|<with|math-font-family|rm|s3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|s1>>|<cell|<frac|<with|math-font-family|rm|s5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>128) >
    <|unfolded-io>
      sg:a.s.transpose(a)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o128>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>>>|<row|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>|<sqrt|2>>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>|<sqrt|2>>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>|<sqrt|2>>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>|<sqrt|2>>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>129) >
    <|unfolded-io>
      fullratsimp(sg[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o129>)
      >><sqrt|2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<sqrt|2>*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<sqrt|2>*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>130) >
    <|unfolded-io>
      fullratsimp(sg[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o130>)
      >><sqrt|2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<sqrt|2>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>+<sqrt|2>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>131) >
    <|unfolded-io>
      fullratsimp(sg[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o131>)
      >><sqrt|2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<sqrt|2>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+<sqrt|2>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>132) >
    <|unfolded-io>
      fullratsimp(sqrt(2)*sg[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o132>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<sqrt|2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>+<sqrt|2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>+<sqrt|2>*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>133) >
    <|unfolded-io>
      fullratsimp(sqrt(2)*sg[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o133>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<sqrt|2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<sqrt|2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>+<sqrt|2>*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>134) >
    <|unfolded-io>
      fullratsimp(sqrt(2)*sg[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o134>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s5>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|s3>+<sqrt|2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<sqrt|2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>+<sqrt|2>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>135) >
    <|unfolded-io>
      2*sqrt(2)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o135>)
      >>2<rsup|<frac|3|2>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>136) >
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
  </collection>
</initial>