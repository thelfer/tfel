<TeXmacs|1.99.2>

<style|<tuple|old-generic|maxima|french>>

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
      <with|color|red|(<with|math-font-family|rm|%i>59) >
    <|unfolded-io>
      iF:matrix([iF0,iF3,0],[iF4,iF1,0],[0,0,iF2])
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o59>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|iF0>>|<cell|<with|math-font-family|rm|iF3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|iF4>>|<cell|<with|math-font-family|rm|iF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>60) >
    <|unfolded-io>
      t:matrix([t0,t3/sqrt(2),0],[t3/sqrt(2),t1,0],[0,0,t2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o60>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|t0>>|<cell|<frac|<with|math-font-family|rm|t3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|t3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|t1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|t2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>61) >
    <|unfolded-io>
      dF:matrix([dF0,dF3,0],[dF4,dF1,0],[0,0,dF2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o61>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|dF0>>|<cell|<with|math-font-family|rm|dF3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|dF4>>|<cell|<with|math-font-family|rm|dF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|dF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>62) >
    <|unfolded-io>
      L:dF.iF
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o62>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF4>+<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF0>>|<cell|<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>>|<cell|<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF3>+<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|dF2>*<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>63) >
    <|unfolded-io>
      W:(L-transpose(L))/2
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o63>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|<frac|-<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>-<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>-<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>-<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>64) >
    <|unfolded-io>
      sWt:fullratsimp(W.t-t.W)
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o64>)
      >><matrix|<tformat|<table|<row|<cell|-<frac|<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|-<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<around*|(|<with|math-font-family|rm|t0>-<with|math-font-family|rm|t1>|)>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<around*|(|<with|math-font-family|rm|t0>-<with|math-font-family|rm|t1>|)>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|0>>|<row|<cell|-<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<around*|(|<with|math-font-family|rm|t0>-<with|math-font-family|rm|t1>|)>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<around*|(|<with|math-font-family|rm|t0>-<with|math-font-family|rm|t1>|)>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|<frac|<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>65) >
    <|unfolded-io>
      fullratsimp(sWt[2,1]-sWt[1,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o65>)
      >>0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>66) >
    <|unfolded-io>
      pv:transpose(matrix([sWt[1,1],sWt[2,2],sWt[3,3],sqrt(2)*sWt[1,2]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o66>)
      >><matrix|<tformat|<table|<row|<cell|-<frac|<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>>|<row|<cell|<frac|<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>-<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<sqrt|2>*<with|math-font-family|rm|t3>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>>|<row|<cell|0>>|<row|<cell|-<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<around*|(|<with|math-font-family|rm|t0>-<with|math-font-family|rm|t1>|)>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<around*|(|<with|math-font-family|rm|t0>-<with|math-font-family|rm|t1>|)>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>67) >
    <|unfolded-io>
      J1:fullratsimp(addcol(diff(pv,dF0),diff(pv,dF1),diff(pv,dF2),diff(pv,dF3),diff(pv,dF4)))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o67>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF3>|<sqrt|2>>>|<cell|-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF4>|<sqrt|2>>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>|<row|<cell|-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF4>|<sqrt|2>>>|<cell|0>|<cell|-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF3>|<sqrt|2>>>|<cell|-<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF4>|<sqrt|2>>>|<cell|0>|<cell|<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|-<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>71) >
    <|unfolded-io>
      Dm:(L+transpose(L))/2
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o71>)
      >><matrix|<tformat|<table|<row|<cell|<frac|2*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF4>+2*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF0>|2>>|<cell|<frac|<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>|<cell|<frac|2*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF3>+2*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF1>|2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|dF2>*<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>72) >
    <|unfolded-io>
      D:transpose(matrix([Dm[1,1],Dm[2,2],Dm[3,3],sqrt(2)*Dm[1,2]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o72>)
      >><matrix|<tformat|<table|<row|<cell|<frac|2*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF4>+2*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF0>|2>>>|<row|<cell|<frac|2*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF3>+2*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF1>|2>>>|<row|<cell|<with|math-font-family|rm|dF2>*<with|math-font-family|rm|iF2>>>|<row|<cell|<frac|<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>73) >
    <|unfolded-io>
      dD:fullratsimp(addcol(diff(D,dF0),diff(D,dF1),diff(D,dF2),diff(D,dF3),diff(D,dF4)))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o73>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|iF0>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|iF4>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|iF1>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|iF3>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|iF2>>|<cell|0>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|iF3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|iF4>|<sqrt|2>>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|iF0>|<sqrt|2>>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>74) >
    <|unfolded-io>
      C:matrix([C00,C01,C02,C03],[C10,C11,C12,C13],[C20,C21,C22,C23],[C30,C31,C32,C33])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o74>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C00>>|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C02>>|<cell|<with|math-font-family|rm|C03>>>|<row|<cell|<with|math-font-family|rm|C10>>|<cell|<with|math-font-family|rm|C11>>|<cell|<with|math-font-family|rm|C12>>|<cell|<with|math-font-family|rm|C13>>>|<row|<cell|<with|math-font-family|rm|C20>>|<cell|<with|math-font-family|rm|C21>>|<cell|<with|math-font-family|rm|C22>>|<cell|<with|math-font-family|rm|C23>>>|<row|<cell|<with|math-font-family|rm|C30>>|<cell|<with|math-font-family|rm|C31>>|<cell|<with|math-font-family|rm|C32>>|<cell|<with|math-font-family|rm|C33>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>76) >
    <|unfolded-io>
      C.dD+J1
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o76>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|C03>*<with|math-font-family|rm|iF3>|<sqrt|2>>+<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF3>|<sqrt|2>>+<with|math-font-family|rm|C00>*<with|math-font-family|rm|iF0>>|<cell|<frac|<with|math-font-family|rm|C03>*<with|math-font-family|rm|iF4>|<sqrt|2>>-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF4>|<sqrt|2>>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|iF1>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|iF2>>|<cell|<with|math-font-family|rm|C00>*<with|math-font-family|rm|iF4>+<frac|<with|math-font-family|rm|C03>*<with|math-font-family|rm|iF1>|<sqrt|2>>+<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|iF3>+<frac|<with|math-font-family|rm|C03>*<with|math-font-family|rm|iF0>|<sqrt|2>>-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|C13>*<with|math-font-family|rm|iF3>|<sqrt|2>>-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF3>|<sqrt|2>>+<with|math-font-family|rm|C10>*<with|math-font-family|rm|iF0>>|<cell|<frac|<with|math-font-family|rm|C13>*<with|math-font-family|rm|iF4>|<sqrt|2>>+<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF4>|<sqrt|2>>+<with|math-font-family|rm|C11>*<with|math-font-family|rm|iF1>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|iF2>>|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|iF4>+<frac|<with|math-font-family|rm|C13>*<with|math-font-family|rm|iF1>|<sqrt|2>>-<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|iF3>+<frac|<with|math-font-family|rm|C13>*<with|math-font-family|rm|iF0>|<sqrt|2>>+<frac|<with|math-font-family|rm|t3>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|C23>*<with|math-font-family|rm|iF3>|<sqrt|2>>+<with|math-font-family|rm|C20>*<with|math-font-family|rm|iF0>>|<cell|<frac|<with|math-font-family|rm|C23>*<with|math-font-family|rm|iF4>|<sqrt|2>>+<with|math-font-family|rm|C21>*<with|math-font-family|rm|iF1>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|iF2>>|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|iF4>+<frac|<with|math-font-family|rm|C23>*<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|iF3>+<frac|<with|math-font-family|rm|C23>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|iF3>|<sqrt|2>>+<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF3>|<sqrt|2>>+<with|math-font-family|rm|C30>*<with|math-font-family|rm|iF0>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|iF4>|<sqrt|2>>-<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF4>|<sqrt|2>>+<with|math-font-family|rm|C31>*<with|math-font-family|rm|iF1>>|<cell|<with|math-font-family|rm|C32>*<with|math-font-family|rm|iF2>>|<cell|<with|math-font-family|rm|C30>*<with|math-font-family|rm|iF4>+<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|iF1>|<sqrt|2>>+<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF1>|<sqrt|2>>>|<cell|<with|math-font-family|rm|C31>*<with|math-font-family|rm|iF3>+<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|iF0>|<sqrt|2>>-<frac|<around*|(|<with|math-font-family|rm|t1>-<with|math-font-family|rm|t0>|)>*<with|math-font-family|rm|iF0>|<sqrt|2>>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>77) >
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|page-type|a2>
  </collection>
</initial>