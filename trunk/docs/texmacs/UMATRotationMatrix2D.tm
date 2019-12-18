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
      a:matrix([a0,a2,0],[a1,a3,0],[0,0,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o1>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a0>>|<cell|<with|math-font-family|rm|a2>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|a1>>|<cell|<with|math-font-family|rm|a3>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|1>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>2) >
    <|unfolded-io>
      e:matrix([e0,e3/2,0],[e3/2,e1,0],[0,0,e2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|e0>>|<cell|<frac|<with|math-font-family|rm|e3>|2>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|e3>|2>>|<cell|<with|math-font-family|rm|e1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|e2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>3) >
    <|unfolded-io>
      em:transpose(a).e.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o3>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>>|<cell|<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e1>|)>>|<cell|<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|e2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>4) >
    <|unfolded-io>
      fullratsimp(em[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      >><with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>5) >
    <|unfolded-io>
      fullratsimp(em[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o5>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) >
    <|unfolded-io>
      fullratsimp(em[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      >><with|math-font-family|rm|e2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
    <|unfolded-io>
      fullratsimp(2*em[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o7>)
      >><around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|unfolded-io>
      eg:a.e.transpose(a)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e1>|)>>|<cell|<with|math-font-family|rm|a0>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a2>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a0>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|e1>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<frac|<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>|)>+<with|math-font-family|rm|a3>*<around*|(|<frac|<with|math-font-family|rm|a1>*<with|math-font-family|rm|e3>|2>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>|)>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|e2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>9) >
    <|unfolded-io>
      fullratsimp(eg[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o9>)
      >><with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>10) >
    <|unfolded-io>
      fullratsimp(eg[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e3>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|e1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      fullratsimp(eg[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><with|math-font-family|rm|e2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>12) >
    <|unfolded-io>
      fullratsimp(2*eg[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
      >><around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>|)>*<with|math-font-family|rm|e3>+2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|e1>+2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|e0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      s:matrix([s0,s3,0],[s3,s1,0],[0,0,s2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|s0>>|<cell|<with|math-font-family|rm|s3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|s3>>|<cell|<with|math-font-family|rm|s1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>14) >
    <|unfolded-io>
      sm:transpose(a).s.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o14>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>>|<cell|<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s1>|)>>|<cell|<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      fullratsimp(sm[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >>2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      fullratsimp(sm[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >>2*<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>17) >
    <|unfolded-io>
      fullratsimp(sm[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      >><with|math-font-family|rm|s2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>18) >
    <|unfolded-io>
      fullratsimp(sm[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o18>)
      >><around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>19) >
    <|unfolded-io>
      sg:a.s.transpose(a)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o19>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s1>|)>>|<cell|<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|s1>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>|)>+<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>|)>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>20) >
    <|unfolded-io>
      fullratsimp(sg[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o20>)
      >>2*<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) >
    <|unfolded-io>
      fullratsimp(sg[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      >>2*<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>22) >
    <|unfolded-io>
      fullratsimp(sg[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o22>)
      >><with|math-font-family|rm|s2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>23) >
    <|unfolded-io>
      fullratsimp(sg[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o23>)
      >><around*|(|<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>|)>*<with|math-font-family|rm|s3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|s1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|s0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      F:matrix([F0,F3,0],[F1,F4,0],[0,0,F8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F4>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>25) >
    <|unfolded-io>
      Fm:transpose(a).F.a
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o25>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a1>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a0>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>|)>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|a3>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|a2>*<around*|(|<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>|)>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>26) >
    <|unfolded-io>
      Fm0:fullratsimp(Fm[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o26>)
      >><with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>27) >
    <|unfolded-io>
      fullratsimp(Fm[2,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o27>)
      >><with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>28) >
    <|unfolded-io>
      fullratsimp(Fm[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o28>)
      >><with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      fullratsimp(Fm[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|unfolded-io>
      fullratsimp(Fm[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o30>)
      >><with|math-font-family|rm|F8>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>31) >
    <|unfolded-io>
      Fg:fullratsimp(a.F.transpose(a))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o31>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F0>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>32) >
    <|unfolded-io>
      Fg[1,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o32>)
      >><with|math-font-family|rm|a2><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>33) >
    <|unfolded-io>
      Fg[2,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o33>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>36) >
    <|unfolded-io>
      Fg[1,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o36>)
      >><with|math-font-family|rm|a2>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a2>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a0>*<with|math-font-family|rm|a1>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>37) >
    <|unfolded-io>
      Fg[2,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o37>)
      >><with|math-font-family|rm|a3><rsup|2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|a1>*<with|math-font-family|rm|a3>*<with|math-font-family|rm|F1>+<with|math-font-family|rm|a1><rsup|2>*<with|math-font-family|rm|F0>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>41) >
    <|unfolded-io>
      Fg[3,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o41>)
      >><with|math-font-family|rm|F8>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>42) >
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