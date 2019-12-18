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
      <with|color|red|(<with|math-font-family|rm|%i>2) >
    <|unfolded-io>
      F:matrix([F0,F3,F5],[F4,F1,F7],[F6,F8,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>3) >
    <|unfolded-io>
      b:F.transpose(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o3>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0><rsup|2>>|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F1><rsup|2>>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>4) >
    <|unfolded-io>
      pv:transpose(matrix([b[1,1],b[2,2],b[3,3],sqrt(2)*b[1,2],sqrt(2)*b[1,3],sqrt(2)*b[2,3]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0><rsup|2>>>|<row|<cell|<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F1><rsup|2>>>|<row|<cell|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F2><rsup|2>>>|<row|<cell|<sqrt|2>*<around*|(|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>>>|<row|<cell|<sqrt|2>*<around*|(|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>|)>>>|<row|<cell|<sqrt|2>*<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>5) >
    <|unfolded-io>
      dC:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3),diff(pv,F4),diff(pv,F5),diff(pv,F6),diff(pv,F7),diff(pv,F8))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o5>)
      >><matrix|<tformat|<table|<row|<cell|2*<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|F3>>|<cell|0>|<cell|2*<with|math-font-family|rm|F5>>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|2*<with|math-font-family|rm|F1>>|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|F4>>|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|F7>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|F2>>|<cell|0>|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|F6>>|<cell|0>|<cell|2*<with|math-font-family|rm|F8>>>|<row|<cell|<sqrt|2>*<with|math-font-family|rm|F4>>|<cell|<sqrt|2>*<with|math-font-family|rm|F3>>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F1>>|<cell|<sqrt|2>*<with|math-font-family|rm|F0>>|<cell|<sqrt|2>*<with|math-font-family|rm|F7>>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F5>>|<cell|0>>|<row|<cell|<sqrt|2>*<with|math-font-family|rm|F6>>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F5>>|<cell|<sqrt|2>*<with|math-font-family|rm|F8>>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F2>>|<cell|<sqrt|2>*<with|math-font-family|rm|F0>>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F3>>>|<row|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F8>>|<cell|<sqrt|2>*<with|math-font-family|rm|F7>>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F6>>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|F4>>|<cell|<sqrt|2>*<with|math-font-family|rm|F2>>|<cell|<sqrt|2>*<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) >
    <|unfolded-io>
      dC[1,5]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      >>0>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
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