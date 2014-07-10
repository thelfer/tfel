Declare Function SIC_YOUNGMODULUS_SNEAD Lib "libCastemMaterialLaw" (temp As Double) As Double

Public Function LoiMat(ParamArray VarTableau())
    Dim v(2) As Double
    v(0) = VarTableau(0)
    v(1) = VarTableau(1)
    LoiMat = SIC_YOUNGMODULUS_SNEAD(v(0))
End Function