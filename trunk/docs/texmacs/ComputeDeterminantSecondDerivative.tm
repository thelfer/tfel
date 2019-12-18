<TeXmacs|1.99.2>

<style|<tuple|generic|french|maxima>>

<\body>
  <\session|maxima|default>
    <\output>
      \;

      Maxima 5.34.1 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.12 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>1) >
    <|unfolded-io>
      F:matrix([F0,F3/sqrt(2),0],[F3/sqrt(2),F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o1>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>4) >
    <|unfolded-io>
      iFm:determinant(F)*invert(F)
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>|<cell|-<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|0>>|<row|<cell|-<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<frac|<with|math-font-family|rm|F3><rsup|2>|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>10) >
    <|unfolded-io>
      iF:transpose(matrix([iFm[1,1],iFm[2,2],iFm[3,3],sqrt(2)*iFm[1,2]]))
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<frac|<with|math-font-family|rm|F3><rsup|2>|2>>>|<row|<cell|-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      diF:addcol(diff(iF,F0),diff(iF,F1),diff(iF,F2),diff(iF,F3))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|-<with|math-font-family|rm|F3>>>|<row|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F3>>|<cell|-<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      F:matrix([F0,0,0],[0,F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>17) >
    <|unfolded-io>
      iFm:determinant(F)*invert(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>19) >
    <|unfolded-io>
      iF:transpose(matrix([iFm[1,1],iFm[2,2],iFm[3,3]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o19>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>20) >
    <|unfolded-io>
      diF:addcol(diff(iF,F0),diff(iF,F1),diff(iF,F2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o20>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F0>>|<cell|0>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>40) >
    <|unfolded-io>
      F:matrix([F0,G3,G4],[G3,F1,G5],[G4,G5,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o40>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|G3>>|<cell|<with|math-font-family|rm|G4>>>|<row|<cell|<with|math-font-family|rm|G3>>|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|G5>>>|<row|<cell|<with|math-font-family|rm|G4>>|<cell|<with|math-font-family|rm|G5>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>47) >
    <|unfolded-io>
      iFm2:fullratsimp(determinant(F)*invert(F))
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o47>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>-<with|math-font-family|rm|G5><rsup|2>>|<cell|<with|math-font-family|rm|G4>*<with|math-font-family|rm|G5>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|G3>>|<cell|<with|math-font-family|rm|G3>*<with|math-font-family|rm|G5>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>>>|<row|<cell|<with|math-font-family|rm|G4>*<with|math-font-family|rm|G5>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|G3>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>-<with|math-font-family|rm|G4><rsup|2>>|<cell|<with|math-font-family|rm|G3>*<with|math-font-family|rm|G4>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|G5>>>|<row|<cell|<with|math-font-family|rm|G3>*<with|math-font-family|rm|G5>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>>|<cell|<with|math-font-family|rm|G3>*<with|math-font-family|rm|G4>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|G5>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<with|math-font-family|rm|G3><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>48) >
    <|unfolded-io>
      iFm:iFm2,G3=F3/sqrt(2),G4=F4/sqrt(2),G5=F5/sqrt(2)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o48>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>-<frac|<with|math-font-family|rm|F5><rsup|2>|2>>|<cell|<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>|2>-<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|2>-<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>|2>-<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>-<frac|<with|math-font-family|rm|F4><rsup|2>|2>>|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>|2>-<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|2>-<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>|2>-<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<frac|<with|math-font-family|rm|F3><rsup|2>|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>49) >
    <|unfolded-io>
      iF:transpose(matrix([iFm[1,1],iFm[2,2],iFm[3,3],sqrt(2)*iFm[1,2],sqrt(2)*iFm[1,3],sqrt(2)*iFm[2,3]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o49>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>-<frac|<with|math-font-family|rm|F5><rsup|2>|2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>-<frac|<with|math-font-family|rm|F4><rsup|2>|2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<frac|<with|math-font-family|rm|F3><rsup|2>|2>>>|<row|<cell|<sqrt|2>*<around*|(|<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>|2>-<frac|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|<sqrt|2>>|)>>>|<row|<cell|<sqrt|2>*<around*|(|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|2>-<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>|<sqrt|2>>|)>>>|<row|<cell|<sqrt|2>*<around*|(|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>|2>-<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>|<sqrt|2>>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>50) >
    <|unfolded-io>
      diF:addcol(diff(iF,F0),diff(iF,F1),diff(iF,F2),diff(iF,F3),diff(iF,F4),diff(iF,F5))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o50>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|-<with|math-font-family|rm|F4>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|-<with|math-font-family|rm|F3>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F3>>|<cell|-<with|math-font-family|rm|F2>>|<cell|<frac|<with|math-font-family|rm|F5>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F4>|<sqrt|2>>>>|<row|<cell|0>|<cell|-<with|math-font-family|rm|F4>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F5>|<sqrt|2>>>|<cell|-<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>>|<row|<cell|-<with|math-font-family|rm|F5>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|-<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>51) >
    <|input>
      \;
    </input>
  </session>
</body>