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
      <with|color|red|(<with|math-font-family|rm|%i>1) >
    <|unfolded-io>
      a:matrix([a0,a3,a6],[a1,a4,a7],[a2,a5,a8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o1>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a0>>|<cell|<with|math-font-family|rm|a3>>|<cell|<with|math-font-family|rm|a6>>>|<row|<cell|<with|math-font-family|rm|a1>>|<cell|<with|math-font-family|rm|a4>>|<cell|<with|math-font-family|rm|a7>>>|<row|<cell|<with|math-font-family|rm|a2>>|<cell|<with|math-font-family|rm|a5>>|<cell|<with|math-font-family|rm|a8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>2) >
    <|unfolded-io>
      e:matrix([e0,e3/2,e4/2],[e3/2,e1,e5/2],[e4/2,e5/2,e2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|e0>>|<cell|<frac|<with|math-font-family|rm|e3>|2>>|<cell|<frac|<with|math-font-family|rm|e4>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|e3>|2>>|<cell|<with|math-font-family|rm|e1>>|<cell|<frac|<with|math-font-family|rm|e5>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|e4>|2>>|<cell|<frac|<with|math-font-family|rm|e5>|2>>|<cell|<with|math-font-family|rm|e2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>3) >
    <|unfolded-io>
      em:transpose(a).e.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o3>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>4) >
    <|unfolded-io>
      fullratsimp(em[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      >><with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>5) >
    <|unfolded-io>
      fullratsimp(em[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o5>)
      >><with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) >
    <|unfolded-io>
      fullratsimp(em[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      >><with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
    <|unfolded-io>
      fullratsimp(2*em[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o7>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|unfolded-io>
      fullratsimp(2*em[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>9) >
    <|unfolded-io>
      fullratsimp(2*em[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o9>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>10) >
    <|unfolded-io>
      eg:a.e.transpose(a)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a6>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a7>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>>>|<row|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>|)>+<with|math-font-family|rm|a8>*<around*|(|<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e5>|2>+<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|2>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|2>+<frac|<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      fullratsimp(eg[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >><with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      fullratsimp(eg[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >><with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>17) >
    <|unfolded-io>
      fullratsimp(eg[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      >><with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|e2>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>18) >
    <|unfolded-io>
      fullratsimp(2*eg[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o18>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>19) >
    <|unfolded-io>
      fullratsimp(2*eg[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o19>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>20) >
    <|unfolded-io>
      fullratsimp(2*eg[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o20>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e5>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|e4>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e2>+2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) >
    <|unfolded-io>
      s:matrix([s0,s3,e4],[s3,s1,s5],[s4,s5,s2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|s0>>|<cell|<with|math-font-family|rm|s3>>|<cell|<with|math-font-family|rm|e4>>>|<row|<cell|<with|math-font-family|rm|s3>>|<cell|<with|math-font-family|rm|s1>>|<cell|<with|math-font-family|rm|s5>>>|<row|<cell|<with|math-font-family|rm|s4>>|<cell|<with|math-font-family|rm|s5>>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>22) >
    <|unfolded-io>
      sm:transpose(a).s.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o22>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|)>>>|<row|<cell|<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>23) >
    <|unfolded-io>
      fullratsimp(sm[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o23>)
      >>2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      fullratsimp(sm[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >>2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>25) >
    <|unfolded-io>
      fullratsimp(sm[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o25>)
      >>2*<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>26) >
    <|unfolded-io>
      fullratsimp(sm[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o26>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>27) >
    <|unfolded-io>
      fullratsimp(sm[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o27>)
      >><around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>28) >
    <|unfolded-io>
      fullratsimp(sm[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o28>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      sg:a.s.transpose(a)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|)>>>|<row|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|)>>>|<row|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>|)>+<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s4>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|unfolded-io>
      fullratsimp(sg[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o30>)
      >>2*<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>31) >
    <|unfolded-io>
      fullratsimp(sg[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o31>)
      >>2*<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>32) >
    <|unfolded-io>
      fullratsimp(sg[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o32>)
      >>2*<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s4>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>33) >
    <|unfolded-io>
      fullratsimp(sg[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o33>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>34) >
    <|unfolded-io>
      fullratsimp(sg[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o34>)
      >><around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>|)>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>35) >
    <|unfolded-io>
      fullratsimp(sg[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o35>)
      >><around*|(|<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>|)>*<with|math-font-family|rm|s5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|s4>+<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|s2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|e4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>36) >
    <|unfolded-io>
      F:matrix([F0,F3,F6],[F1,F4,F7],[F2,F5,F8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o36>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>37) >
    <|unfolded-io>
      Fm:transpose(a).F.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o37>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>|)>>>|<row|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a5>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a4>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>|)>>>|<row|<cell|<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a8>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|a7>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a6>*<around*|(|<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>47) >
    <|unfolded-io>
      Fm0:fullratsimp(Fm[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o47>)
      >><with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>39) >
    <|unfolded-io>
      fullratsimp(Fm[2,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o39>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>40) >
    <|unfolded-io>
      fullratsimp(Fm[3,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o40>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>41) >
    <|unfolded-io>
      fullratsimp(Fm[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o41>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>42) >
    <|unfolded-io>
      fullratsimp(Fm[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o42>)
      >><with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>43) >
    <|unfolded-io>
      fullratsimp(Fm[3,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o43>)
      >><with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>44) >
    <|unfolded-io>
      fullratsimp(Fm[1,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o44>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>45) >
    <|unfolded-io>
      fullratsimp(Fm[2,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o45>)
      >><with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>46) >
    <|unfolded-io>
      fullratsimp(Fm[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o46>)
      >><with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>52) >
    <|unfolded-io>
      Fg:fullratsimp(a.F.transpose(a))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o52>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>53) >
    <|unfolded-io>
      Fg[1,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o53>)
      >><with|math-font-family|rm|a6><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>56) >
    <|unfolded-io>
      Fg[2,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o56>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>57) >
    <|unfolded-io>
      Fg[3,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o57>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>58) >
    <|unfolded-io>
      Fg[1,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o58>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>59) >
    <|unfolded-io>
      Fg[2,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o59>)
      >><with|math-font-family|rm|a7><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>60) >
    <|unfolded-io>
      Fg[3,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o60>)
      >><with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>61) >
    <|unfolded-io>
      Fg[1,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o61>)
      >><with|math-font-family|rm|a6>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a6>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>62) >
    <|unfolded-io>
      Fg[2,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o62>)
      >><with|math-font-family|rm|a7>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a4>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a7>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a4>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>63) >
    <|unfolded-io>
      Fg[3,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o63>)
      >><with|math-font-family|rm|a8><rsup|2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|a5>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|a5><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a8>*<with|math-font-family|rm|F2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a5>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>64) >
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