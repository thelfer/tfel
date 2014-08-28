/*! 
 * \file  mfront/include/MFront/MFrontSearchFile.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mai 2013
 */

#ifndef _LIB_MFRONT_MFRONTSEARCHFILE_H_
#define _LIB_MFRONT_MFRONTSEARCHFILE_H_ 

#include<set>
#include<string>

namespace mfront
{

  /*!
   * structure in charge of:
   * - storing search paths
   * - searching imported mfront file
   */
  struct MFrontSearchFile
  {
    /*!
     * search a file in the specified paths.
     * The file is searched using
     * the following conventions :
     * - search in the current directory.
     * - search in the search paths from the most recently declared
         search path to the older ones.
     *
     * Search paths declared through the MFRONT_INCLUDE_PATH are
     * stored at the end of the list.
     *
     * \param[in] f : file name
     * \return the full path of the file
     */
    static std::string
    search(const std::string&);
    /*!
     * \param[in] p : new search paths
     */
    static void
    addSearchPaths(const std::string&);
    /*!
     * \return the list of the search paths
     */
    static const std::vector<std::string>&
    getSearchPaths(void);
  private:
    /*!
     * return the uniq instance of the class
     */
    static MFrontSearchFile&
    getMFrontSearchFile(void);
    /*!
     * Default constructor
     *
     * The MFRONT_INCLUDE_PATH environnement variable
     * is used to fill the search paths.
     */
    MFrontSearchFile();
    //! copy constructor (disabled)
    MFrontSearchFile(const MFrontSearchFile&);
    //! assignement operator constructor (disabled)
    MFrontSearchFile&
    operator=(const MFrontSearchFile&);
    //! list of search paths
    std::vector<std::string> paths;
  };

} // end of MFrontSearchFile

#endif /* _LIB_MFRONT_MFRONTSEARCHFILE_H */

