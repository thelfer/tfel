/*!
 * \file   StensorSizeToDime.hxx
 * \brief  This file implements the StensorDimeToSize and StensorSizeToDime classes.
 * \author Helfer Thomas
 * \date   07 Aug 2006
 */

#ifndef _LIB_TFEL_STENSORSIZETODIME_H_
#define _LIB_TFEL_STENSORSIZETODIME_H_ 

namespace tfel{
  
  namespace math {

    /*!
     * \class StensorDimeToSize
     * \brief Metafunction which returns the number of components of
     * an stensor given the spatial dimension used.
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
     *     2         & 4 \\
     *     \hline
     *     3         & 6 \\    
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
     * <TD> 4 </TD>
     * </TR>
     * <TR>
     * <TD> 3 </TD>
     * <TD> 6 </TD>
     * </TR>
     * </TBODY>
     * </TABLE>
     * \endhtmlonly
     * \param N, the spatial dimension.
     * \param value, the number of components of an stensor
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   StensorSizeToDime and StensorSizeToDime.hxx for implementation.
     */
    template<unsigned short N>
    struct StensorDimeToSize;

    /*!
     * \class StensorSizeToDime
     * \brief Metafunction which returns the spatial dimension given 
     * the number of components of a stensor.
     * \param N, the number of components of an stensor
     * \param value, the spatial dimension.
     * \pre   This class is only defined for N=3u,4u and 6u.
     * \see   StensorDimeToSize and StensorSizeToDime.hxx for implementation.
     */
    template<unsigned short N>
    struct StensorSizeToDime;

    /*!
     * \brief Partial specialisation for 1D.
     */
    template<>
    struct StensorDimeToSize<1u>
    {
      static const unsigned short value = 3u;
    };

    /*!
     * \brief Partial specialisation for 2D.
     */
    template<>
    struct StensorDimeToSize<2u>
    {
      static const unsigned short value = 4u;
    };

    /*!
     * \brief Partial specialisation for 3D.
     */
    template<>
    struct StensorDimeToSize<3u>
    {
      static const unsigned short value = 6u;
    };

    /*!
     * \brief Partial specialisation for 1D.
     */
    template<>
    struct StensorSizeToDime<3u>
    {
      static const unsigned short value = 1u;
    };

    /*!
     * \brief Partial specialisation for 2D.
     */
    template<>
    struct StensorSizeToDime<4u>
    {
      static const unsigned short value = 2u;
    };

    /*!
     * \brief Partial specialisation for 3D.
     */
    template<>
    struct StensorSizeToDime<6u>
    {
      static const unsigned short value = 3u;
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSORSIZETODIME_H */

