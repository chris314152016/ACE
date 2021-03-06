//============================================================
/**
 * @file XML_Helper.h
 *
 * @brief Some helper functions for XML
 *
 * @author Bala Natarajan <bala@dre.vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//============================================================
#ifndef ACE_XML_HELPER_H
#define ACE_XML_HELPER_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML_Error_Handler.h"
#include "XML_Schema_Resolver.h"

#include "xercesc/util/XercesDefs.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"

#include <memory>

namespace XERCES_CPP_NAMESPACE
{
  class DOMDocument;
  class DOMWriter;
  class DOMImplementation;
  class DOMDocumentType;
}

namespace XML
{
  /**
    * @class XML_Helper
    *
    * @brief Helper class for some routine XML stuff.
    */
  template <typename Resolver = XML_Schema_Resolver <>,
            typename Error_Handler = XML_Error_Handler>
  class XML_Helper
  {
  public:
    XML_Helper (Resolver *resolver = nullptr, Error_Handler *eh = nullptr);

    ~XML_Helper ();

    /// Create a DOM tree
    XERCES_CPP_NAMESPACE::DOMDocument *
    create_dom (const ACE_TCHAR *uri) const;

    XERCES_CPP_NAMESPACE::DOMDocument *
    create_dom (const ACE_TCHAR *root,
                const ACE_TCHAR *ns,
                XERCES_CPP_NAMESPACE::DOMDocumentType * doctype = 0) const;

    XERCES_CPP_NAMESPACE::DOMDocumentType *
    create_doctype (const ACE_TCHAR *qn,
                    const ACE_TCHAR *pid,
                    const ACE_TCHAR *sid) const;

    /// Writes out a DOMDocument to an XML file
    bool write_DOM (XERCES_CPP_NAMESPACE::DOMDocument *doc,
                    const ACE_TCHAR *file) const;

    bool is_initialized () const;

    /// Terminate the parser
    void terminate_parser ();

    Resolver &get_resolver ();

    Error_Handler &get_error_handler ();

  protected:
    /// Initialize the parser
    void init_parser ();

  private:
    bool initialized_ { false };
    XERCES_CPP_NAMESPACE::DOMImplementation *impl_ {};
    mutable std::unique_ptr<XERCES_CPP_NAMESPACE::XercesDOMParser> parser_;

    Resolver *resolver_;
    bool release_resolver_ { false };
    Error_Handler *e_handler_;
    bool release_e_handler_ { false };
  };
}

#include "XML_Helper.tpp"

#include /**/ "ace/post.h"
#endif/*ACE_XML_HELPER_H*/
