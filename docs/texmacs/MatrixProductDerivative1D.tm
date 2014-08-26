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
      <with|color|red|(<with|math-font-family|rm|%i>27) >
    <|unfolded-io>
      F:matrix([F0,0,0],[0,F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o27>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>28) >
    <|unfolded-io>
      G:matrix([G0,0,0],[0,G1,0],[0,0,G2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o28>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      prod:F.G
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|unfolded-io>
      vF:transpose(matrix([F0,F1,F2]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o30>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>31) >
    <|unfolded-io>
      vG:transpose(matrix([G0,G1,G2]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o31>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>32) >
    <|unfolded-io>
      pv:transpose(matrix([prod[1,1],prod[2,2],prod[3,3]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o32>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>33) >
    <|unfolded-io>
      J1:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o33>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>34) >
    <|unfolded-io>
      J2:addcol(diff(pv,G0),diff(pv,G1),diff(pv,G2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o34>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>35) >
    <|unfolded-io>
      C:matrix([C01,C01,C02],[C10,C11,C12],[C20,C21,C22])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o35>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C02>>>|<row|<cell|<with|math-font-family|rm|C10>>|<cell|<with|math-font-family|rm|C11>>|<cell|<with|math-font-family|rm|C12>>>|<row|<cell|<with|math-font-family|rm|C20>>|<cell|<with|math-font-family|rm|C21>>|<cell|<with|math-font-family|rm|C22>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>36) >
    <|unfolded-io>
      J1.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o36>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>37) >
    <|unfolded-io>
      J2.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o37>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>38) >
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