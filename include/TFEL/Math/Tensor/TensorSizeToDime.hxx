/*!
 * \file   TensorSizeToDime.hxx
 * \brief  This file implements the TensorDimeToSize and TensorSizeToDime classes.
 * \author Helfer Thomas
 * \date   07 Aug 2006
 */

#ifndef _LIB_TFEL_TENSORSIZETODIME_H_
#define _LIB_TFEL_TENSORSIZETODIME_H_ 

namespace tfel{
  
  namespace math {

    /*!
     * \class TensorDimeToSize
     * \brief Metafunction which returns the number of components of
     * an tensor given the spatial dimension used.
     * The correspondance between this two numbers is given by the following table:
     * \latexonly
     * \begin{center}
     *   \begin{tabular}[htbp]{|c|c|}
     *     \hline
     *     Spatial Dimension &  Number of components\\
     *     \hline
     *     \hline
     *     1         & 3 \\
     *     \hline
     *     2         & 5 \\
     *     \hline
     *     3         & 9 \\    
     *     \hline
     *   \end{tabular}
     * \end{center}
     * \endlatexonly
     * \htmlonly
     * <TABLE border="1">
     * <TBODY>
     * <TR>
     * <TD> Spatial Dimension </TD>
     * <TD> Number of components </TD>
     * </TR>
     * <TR>
     * <TD> 1 </TD>
     * <TD> 3 </TD>
     * </TBODY>
     * <TBODY>
     * <TD> 2 </TD>
     * <TD> 5 </TD>
     * </TR>
     * <TR>
     * <TD> 3 </TD>
     * <TD> 9 </TD>
     * </TR>
     * </TBODY>
     * </TABLE>
     * \endhtmlonly
     * \param N, the spatial dimension.
     * \param value, the number of components of an tensor
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   TensorSizeToDime and TensorSizeToDime.hxx for implementation.
     */
    template<unsigned short N>
    struct TensorDimeToSize;

    /*!
     * \class TensorSizeToDime
     * \brief Metafunction which returns the spatial dimension given 
     * the number of components of a tensor.
     * \param N, the number of components of an tensor
     * \param value, the spatial dimension.
     * \pre   This class is only defined for N=3u,5u and 9u.
     * \see   TensorDimeToSize and TensorSizeToDime.hxx for implementation.
     */
    template<unsigned short N>
    struct TensorSizeToDime;

    /*!
     * \brief Partial specialisation for 1D.
     */
    template<>
    struct TensorDimeToSize<1u>
    {
      static const unsigned short value = 3u;
    };

    /*!
     * \brief Partial specialisation for 2D.
     */
    template<>
    struct TensorDimeToSize<2u>
    {
      static const unsigned short value = 5u;
    };

    /*!
     * \brief Partial specialisation for 3D.
     */
    template<>
    struct TensorDimeToSize<3u>
    {
      static const unsigned short value = 9u;
    };

    /*!
     * \brief Partial specialisation for 1D.
     */
    template<>
    struct TensorSizeToDime<3u>
    {
      static const unsigned short value = 1u;
    };

    /*!
     * \brief Partial specialisation for 2D.
     */
    template<>
    struct TensorSizeToDime<5u>
    {
      static const unsigned short value = 2u;
    };

    /*!
     * \brief Partial specialisation for 3D.
     */
    template<>
    struct TensorSizeToDime<9u>
    {
      static const unsigned short value = 3u;
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TENSORSIZETODIME_H */

