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
      F:matrix([F0,F3,F6],[F1,F4,F7],[F2,F5,F8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o1>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>5) >
    <|unfolded-io>
      0.5*(transpose(F).F-ident(3))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o5>)
      >><matrix|<tformat|<table|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>|)>>>|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F3><rsup|2>-1|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>|)>>>|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F6><rsup|2>-1|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
    <|unfolded-io>
      determinant(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o7>)
      >><with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>|)>-<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>|)>+<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>|)>*<with|math-font-family|rm|F6>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>10) >
    <|unfolded-io>
      pk:matrix([p0,p3,p4],[p3,p1,p5],[p4,p5,p2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|p0>>|<cell|<with|math-font-family|rm|p3>>|<cell|<with|math-font-family|rm|p4>>>|<row|<cell|<with|math-font-family|rm|p3>>|<cell|<with|math-font-family|rm|p1>>|<cell|<with|math-font-family|rm|p5>>>|<row|<cell|<with|math-font-family|rm|p4>>|<cell|<with|math-font-family|rm|p5>>|<cell|<with|math-font-family|rm|p2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      detF_times_s:F.pk.transpose(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>|<row|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>|<row|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>>|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F8>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>12) >
    <|unfolded-io>
      detF_times_s[1,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      detF_times_s[2,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>14) >
    <|unfolded-io>
      detF_times_s[3,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o14>)
      >><with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F8>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      detF_times_s[1,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      detF_times_s[1,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>17) >
    <|unfolded-io>
      detF_times_s[2,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      >><with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>18) >
    <|input>
      \;
    </input>
  </session>
</body>