/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sfx2.hxx"

#ifndef _COM_SUN_STAR_BEANS_PROPERTYVALUE_HPP_
#include <com/sun/star/beans/StringPair.hpp>
#endif
#ifndef _COM_SUN_STAR_LANG_XMULTISERVICEFACTORY_HPP
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#endif
#ifndef _COM_SUN_STAR_IO_XACTIVEDATASOURCE_HPP
#include <com/sun/star/io/XActiveDataSource.hpp>
#endif
#ifndef _COM_SUN_STAR_XML_SAX_XPARSER_HPP
#include <com/sun/star/xml/sax/XParser.hpp>
#endif
#ifndef _COM_SUN_STAR_XML_SAX_XDOCUMENTHANDLER_HPP
#include <com/sun/star/xml/sax/XDocumentHandler.hpp>
#endif
#ifndef _COM_SUN_STAR_LANG_ILLEGALARGUMENTEXCEPTION_HPP
#include <com/sun/star/lang/IllegalArgumentException.hpp>
#endif

#include <comphelper/attributelist.hxx>

#include "doctemplateslocal.hxx"

using namespace ::com::sun::star;

// -----------------------------------
uno::Sequence< beans::StringPair > DocTemplLocaleHelper::ReadGroupLocalizationSequence( const uno::Reference< io::XInputStream >& xInStream, const uno::Reference< lang::XMultiServiceFactory > xFactory )
	throw( uno::Exception )
{
	::rtl::OUString aStringID = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "groupuinames.xml" ) );
	return ReadLocalizationSequence_Impl( xInStream, aStringID, xFactory );
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::WriteGroupLocalizationSequence( const uno::Reference< io::XOutputStream >& xOutStream, const uno::Sequence< beans::StringPair >& aSequence, const uno::Reference< lang::XMultiServiceFactory > xFactory )
	throw( uno::Exception )
{
	if ( !xOutStream.is() )
		throw uno::RuntimeException();

	uno::Reference< io::XActiveDataSource > xWriterSource(
		xFactory->createInstance(
			::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.xml.sax.Writer" ) ) ),
		uno::UNO_QUERY_THROW );
	uno::Reference< xml::sax::XDocumentHandler > xWriterHandler( xWriterSource, uno::UNO_QUERY_THROW );

	xWriterSource->setOutputStream( xOutStream );
	
	::rtl::OUString aGroupListElement( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:template-group-list" ) );
	::rtl::OUString aGroupElement( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:template-group" ) );
	::rtl::OUString aNameAttr( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:name" ) );
	::rtl::OUString aUINameAttr( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:default-ui-name" ) );
	::rtl::OUString aCDATAString( RTL_CONSTASCII_USTRINGPARAM ( "CDATA" ) );
	::rtl::OUString aWhiteSpace( RTL_CONSTASCII_USTRINGPARAM ( " " ) );

	// write the namespace
	::comphelper::AttributeList* pRootAttrList = new ::comphelper::AttributeList;
	uno::Reference< xml::sax::XAttributeList > xRootAttrList( pRootAttrList );
	pRootAttrList->AddAttribute(
		::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM ( "xmlns" ) ),
		aCDATAString,
		::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM ( "http://openoffice.org/2006/groupuinames" ) ) );

	xWriterHandler->startDocument();
	xWriterHandler->startElement( aGroupListElement, xRootAttrList );

	for ( sal_Int32 nInd = 0; nInd < aSequence.getLength(); nInd++ )
	{
		::comphelper::AttributeList *pAttrList = new ::comphelper::AttributeList;
		uno::Reference< xml::sax::XAttributeList > xAttrList( pAttrList );
		pAttrList->AddAttribute( aNameAttr, aCDATAString, aSequence[nInd].First );
		pAttrList->AddAttribute( aUINameAttr, aCDATAString, aSequence[nInd].Second );

		xWriterHandler->startElement( aGroupElement, xAttrList );
		xWriterHandler->ignorableWhitespace( aWhiteSpace );
		xWriterHandler->endElement( aGroupElement );
	}

	xWriterHandler->ignorableWhitespace( aWhiteSpace );
	xWriterHandler->endElement( aGroupListElement );
	xWriterHandler->endDocument();
}

// ==================================================================================

// -----------------------------------
uno::Sequence< beans::StringPair > SAL_CALL DocTemplLocaleHelper::ReadLocalizationSequence_Impl( const uno::Reference< io::XInputStream >& xInStream, const ::rtl::OUString& aStringID, const uno::Reference< lang::XMultiServiceFactory > xFactory )
	throw( uno::Exception )
{
	if ( !xFactory.is() || !xInStream.is() )
		throw uno::RuntimeException();

	uno::Sequence< beans::StringPair > aResult;

	uno::Reference< xml::sax::XParser > xParser( xFactory->createInstance( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.xml.sax.Parser" ) ) ), uno::UNO_QUERY_THROW );

	DocTemplLocaleHelper* pHelper = new DocTemplLocaleHelper();
	uno::Reference< xml::sax::XDocumentHandler > xHelper( static_cast< xml::sax::XDocumentHandler* >( pHelper ) );
	xml::sax::InputSource aParserInput;
	aParserInput.aInputStream = xInStream;
	aParserInput.sSystemId = aStringID;
	xParser->setDocumentHandler( xHelper );
	xParser->parseStream( aParserInput );
	xParser->setDocumentHandler( uno::Reference < xml::sax::XDocumentHandler > () );

	return pHelper->GetParsingResult();
}

// -----------------------------------
DocTemplLocaleHelper::DocTemplLocaleHelper()
: m_aGroupListElement( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:template-group-list" ) )
, m_aGroupElement( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:template-group" ) )
, m_aNameAttr( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:name" ) )
, m_aUINameAttr( RTL_CONSTASCII_USTRINGPARAM( "groupuinames:default-ui-name" ) )
{
}

// -----------------------------------
DocTemplLocaleHelper::~DocTemplLocaleHelper()
{
}

// -----------------------------------
uno::Sequence< beans::StringPair > DocTemplLocaleHelper::GetParsingResult()
{
	if ( m_aElementsSeq.getLength() )
		throw uno::RuntimeException(); // the parsing has still not finished!

	return m_aResultSeq;
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::startDocument() 	
		throw(xml::sax::SAXException, uno::RuntimeException)
{
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::endDocument() 	
		throw(xml::sax::SAXException, uno::RuntimeException)
{
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::startElement( const ::rtl::OUString& aName, const uno::Reference< xml::sax::XAttributeList >& xAttribs ) 	
		throw( xml::sax::SAXException, uno::RuntimeException )
{
	if ( aName == m_aGroupListElement )
	{
		sal_Int32 nNewLength = m_aElementsSeq.getLength() + 1;

		if ( nNewLength != 1 )
			throw xml::sax::SAXException(); // TODO: this element must be the first level element

		m_aElementsSeq.realloc( nNewLength );
		m_aElementsSeq[nNewLength-1] = aName;

		return; // nothing to do
	}
	else if ( aName == m_aGroupElement )
	{
		sal_Int32 nNewLength = m_aElementsSeq.getLength() + 1;
		if ( nNewLength != 2 )
			throw xml::sax::SAXException(); // TODO: this element must be the second level element

		m_aElementsSeq.realloc( nNewLength );
		m_aElementsSeq[nNewLength-1] = aName;

		sal_Int32 nNewEntryNum = m_aResultSeq.getLength() + 1;
		m_aResultSeq.realloc( nNewEntryNum );

		::rtl::OUString aNameValue = xAttribs->getValueByName( m_aNameAttr );
		if ( !aNameValue.getLength() )
			throw xml::sax::SAXException(); // TODO: the ID value must present

		::rtl::OUString aUINameValue = xAttribs->getValueByName( m_aUINameAttr );
		if ( !aUINameValue.getLength() )
			throw xml::sax::SAXException(); // TODO: the ID value must present

		m_aResultSeq[nNewEntryNum-1].First = aNameValue;
		m_aResultSeq[nNewEntryNum-1].Second = aUINameValue;
	}
	else
	{
		// accept future extensions
		sal_Int32 nNewLength = m_aElementsSeq.getLength() + 1;

		if ( !nNewLength )
			throw xml::sax::SAXException(); // TODO: the extension element must not be the first level element

		m_aElementsSeq.realloc( nNewLength );
		m_aElementsSeq[nNewLength-1] = aName;
	}
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::endElement( const ::rtl::OUString& aName ) 	
	throw( xml::sax::SAXException, uno::RuntimeException )
{
	sal_Int32 nLength = m_aElementsSeq.getLength();
	if ( nLength <= 0 )
		throw xml::sax::SAXException(); // TODO: no other end elements expected!

	if ( !m_aElementsSeq[nLength-1].equals( aName ) )
		throw xml::sax::SAXException(); // TODO: unexpected element ended

	m_aElementsSeq.realloc( nLength - 1 );
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::characters( const ::rtl::OUString& /*aChars*/ ) 	
		throw(xml::sax::SAXException, uno::RuntimeException)
{
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::ignorableWhitespace( const ::rtl::OUString& /*aWhitespaces*/ ) 	
		throw(xml::sax::SAXException, uno::RuntimeException)
{
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::processingInstruction( const ::rtl::OUString& /*aTarget*/, const ::rtl::OUString& /*aData*/ ) 	
		throw(xml::sax::SAXException, uno::RuntimeException)
{
}

// -----------------------------------
void SAL_CALL DocTemplLocaleHelper::setDocumentLocator( const uno::Reference< xml::sax::XLocator >& /*xLocator*/ ) 	
		throw(xml::sax::SAXException, uno::RuntimeException)
{
}

