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
      <with|color|red|(<with|math-font-family|rm|%i>34) >
    <|unfolded-io>
      iF:matrix([iF0,0,0],[0,iF1,0],[0,0,iF2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o34>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|iF0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|iF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>35) >
    <|unfolded-io>
      t:matrix([t0,0,0],[0,t1,0],[0,0,t2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o35>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|t0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|t1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|t2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>36) >
    <|unfolded-io>
      dF:matrix([dF0,0,0],[0,dF1,0],[0,0,dF2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o36>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|dF0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|dF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|dF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>37) >
    <|unfolded-io>
      L:dF.iF
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o37>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|dF2>*<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>38) >
    <|unfolded-io>
      W:(L-transpose(L))/2
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o38>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>39) >
    <|unfolded-io>
      sWt:fullratsimp(W.t-t.W)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o39>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>>>>>>>
    </unfolded-io>

    \;

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>40) >
    <|unfolded-io>
      pv:transpose(matrix([sWt[1,1],sWt[2,2],sWt[3,3]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o40>)
      >><matrix|<tformat|<table|<row|<cell|0>>|<row|<cell|0>>|<row|<cell|0>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>41) >
    <|unfolded-io>
      J1:fullratsimp(addcol(diff(pv,dF0),diff(pv,dF1),diff(pv,dF2),diff(pv,dF3),diff(pv,dF4)))
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o41>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>98) >
    <|unfolded-io>
      C:matrix([C00,C01,C02],[C10,C11,C12],[C20,C21,C22])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o98>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C00>>|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C02>>>|<row|<cell|<with|math-font-family|rm|C10>>|<cell|<with|math-font-family|rm|C11>>|<cell|<with|math-font-family|rm|C12>>>|<row|<cell|<with|math-font-family|rm|C20>>|<cell|<with|math-font-family|rm|C21>>|<cell|<with|math-font-family|rm|C22>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>45) >
    <|unfolded-io>
      C.iF
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o45>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C00>*<with|math-font-family|rm|iF0>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|iF1>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|iF2>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|iF0>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|iF1>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|iF2>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|iF0>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|iF1>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>46) >
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