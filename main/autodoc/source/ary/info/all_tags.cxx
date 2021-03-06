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

#include <precomp.h>
#include <ary/info/all_tags.hxx>


// NOT FULLY DEFINED SERVICES
#include <limits>
#include <ary/info/infodisp.hxx>
#include <adc_cl.hxx>


namespace ary
{
namespace info
{



//*****************************		StdTag		***********************//


StdTag::StdTag( E_AtTagId i_eId )
	:	eId(i_eId),
		// aText,
		pNext(0)
{
}

bool
StdTag::Add_SpecialMeaningToken( const char *	,
								 intt		    )
{
	// Does nothing

	// KORR_FUTURE
	//   Should be a logical exception:
	// csv_assert(false);
    return false;
}

UINT8
StdTag::NrOfSpecialMeaningTokens() const
{
	return 0;
}

AtTag *
StdTag::GetFollower()
{
	if (pNext != 0)
		return pNext->GetFollower();
	pNext = new StdTag(eId);
	return pNext;
}

void
StdTag::do_StoreAt( DocuDisplay &  o_rDisplay ) const
{
    o_rDisplay.Display_StdTag( *this );
}

DocuText *
StdTag::Text()
{
	return &aText;
}



//*****************************		BaseTag		***********************//

BaseTag::BaseTag()
	:	// sBase
		// aText
		pNext(0)
{
}

bool
BaseTag::Add_SpecialMeaningToken( const char *		i_sText,
								  intt				i_nNr )
{
	if ( i_nNr == 1 )
	{
		sBase.AssignText(i_sText,"::");
        return true;
	}
    return false;
}

const char *
BaseTag::Title() const
{
	return "Base Classes";
}

UINT8
BaseTag::NrOfSpecialMeaningTokens() const
{
	return 1;
}

AtTag *
BaseTag::GetFollower()
{
	if (pNext != 0)
		return pNext->GetFollower();
	pNext = new BaseTag;
	return pNext;
}

DocuText *
BaseTag::Text()
{
	return &aText;
}



//*****************************		ExceptionTag	***********************//

ExceptionTag::ExceptionTag()
	:	// sException,
		// aText
		pNext(0)
{
}

bool
ExceptionTag::Add_SpecialMeaningToken( const char *		i_sText,
									   intt				i_nNr )
{
	if ( i_nNr == 1 )
	{
		sException.AssignText(i_sText,"::");
        return true;
	}
    return false;
}

const char *
ExceptionTag::Title() const
{
	return "Thrown Exceptions";
}

UINT8
ExceptionTag::NrOfSpecialMeaningTokens() const
{
	return 1;
}

AtTag *
ExceptionTag::GetFollower()
{
	if (pNext != 0)
		return pNext->GetFollower();
	pNext = new ExceptionTag;
	return pNext;
}

DocuText *
ExceptionTag::Text()
{
	return &aText;
}


//*****************************		ImplementsTag	***********************//

ImplementsTag::ImplementsTag()
	:	// sBase
		// aText
		pNext(0)
{
}

bool
ImplementsTag::Add_SpecialMeaningToken( const char *		i_sText,
										intt				i_nNr )
{
	if ( i_nNr == 1 )
	{
		sName.AssignText(i_sText,"::");
	}
	else
    {
		GetFollower()->Add_SpecialMeaningToken(i_sText,1);
	}
    return true;
}

const char *
ImplementsTag::Title() const
{
	return "Implements";
}

UINT8
ImplementsTag::NrOfSpecialMeaningTokens() const
{
	return std::numeric_limits<UINT8>::max();
}

AtTag *
ImplementsTag::GetFollower()
{
	if (pNext != 0)
		return pNext->GetFollower();
	pNext = new ImplementsTag;
	return pNext;
}

DocuText *
ImplementsTag::Text()
{
	return 0;
}


//*****************************		KeywordTag		***********************//


KeywordTag::KeywordTag()
//	:	sKeys
{
}

bool
KeywordTag::Add_SpecialMeaningToken( const char *		i_sText,
									 intt				)
{
	sKeys.push_back(i_sText);
    return true;
}

const char *
KeywordTag::Title() const
{
	return "Keywords";
}

UINT8
KeywordTag::NrOfSpecialMeaningTokens() const
{
	return std::numeric_limits<UINT8>::max();
}

AtTag *
KeywordTag::GetFollower()
{
	return this;
}

DocuText *
KeywordTag::Text()
{
	return 0;
}



//*****************************		ParameterTag	***********************//


ParameterTag::ParameterTag()
	:	// sName
		// aText
		pNext(0)
{
}

bool
ParameterTag::Add_SpecialMeaningToken( const char *		i_sText,
									   intt				i_nNr )
{
	if ( i_nNr == 1 )
	{
		sName = i_sText;
        return true;
	}
	else if (i_nNr == 2)
	{
		uintt nLen = strlen(i_sText);
		if (*i_sText == '[' AND i_sText[nLen-1] == ']')
		{
			sValidRange = String(i_sText+1, nLen-2);
            return true;
		}
	}
    return false;
}

UINT8
ParameterTag::NrOfSpecialMeaningTokens() const
{
	return 2;
}

AtTag *
ParameterTag::GetFollower()
{
	if (pNext != 0)
		return pNext->GetFollower();
	return  pNext = new ParameterTag;
}

DocuText *
ParameterTag::Text()
{
	return &aText;
}

void
ParameterTag::do_StoreAt( DocuDisplay &  o_rDisplay ) const
{
    o_rDisplay.Display_ParameterTag( *this );
}



//*****************************		SeeTag		    ***********************//



SeeTag::SeeTag()
//	:	sReferences
{
}

bool
SeeTag::Add_SpecialMeaningToken( const char *		i_sText,
								 intt				)
{
	static QualifiedName aNull_;
	sReferences.push_back(aNull_);
	sReferences.back().AssignText(i_sText,"::");

    return true;
}

const char *
SeeTag::Title() const
{
	return "See Also";
}

UINT8
SeeTag::NrOfSpecialMeaningTokens() const
{
	return std::numeric_limits<UINT8>::max();
}

AtTag *
SeeTag::GetFollower()
{
	return this;
}

void
SeeTag::do_StoreAt( DocuDisplay &  o_rDisplay ) const
{
    o_rDisplay.Display_SeeTag( *this );
}

DocuText *
SeeTag::Text()
{
	return 0;
}



//*****************************		TemplateTag		***********************//


TemplateTag::TemplateTag()
	:	// sName
		// aText
		pNext(0)
{
}

bool
TemplateTag::Add_SpecialMeaningToken( const char *		i_sText,
									   intt				i_nNr )
{
	if ( i_nNr == 1 )
	{
		sName = i_sText;
        return true;
	}
    return false;
}

const char *
TemplateTag::Title() const
{
	return "Template Parameters";
}

UINT8
TemplateTag::NrOfSpecialMeaningTokens() const
{
	return 1;
}

AtTag *
TemplateTag::GetFollower()
{
	if (pNext != 0)
		return pNext->GetFollower();
	return  pNext = new TemplateTag;
}

void
TemplateTag::do_StoreAt( DocuDisplay &  o_rDisplay ) const
{
    o_rDisplay.Display_TemplateTag( *this );
}


DocuText *
TemplateTag::Text()
{
	return &aText;
}


//*****************************		LabelTag		***********************//



LabelTag::LabelTag()
	:	sLabel()
{
}

bool
LabelTag::Add_SpecialMeaningToken( const char *		i_sText,
								   intt				i_nNr )
{
	if ( i_nNr == 1 AND sLabel.length() == 0 )
	{
		sLabel = i_sText;
        return true;
	}
    // KORR_FUTURE
//	else	// Throw exception because of double label.
    return false;
}

const char *
LabelTag::Title() const
{
	return "Label";
}

UINT8
LabelTag::NrOfSpecialMeaningTokens() const
{
	return 1;
}

AtTag *
LabelTag::GetFollower()
{
	return this;
}

DocuText *
LabelTag::Text()
{
	return 0;
}


//*****************************		SinceTag		***********************//

SinceTag::SinceTag()
	:	sVersion()
{
}

bool
SinceTag::Add_SpecialMeaningToken( const char *		i_sText,
								   intt				)
{
    const char cCiphersend = '9' + 1;
	if ( sVersion.empty()
	     AND NOT csv::in_range('0', *i_sText, cCiphersend)
	     AND autodoc::CommandLine::Get_().DoesTransform_SinceTag() )
	{
	    return true;
	}

    if (sVersion.empty())
    {
        sVersion = i_sText;
    }
    else
    {
        StreamLock sHelp(100);
        sVersion = sHelp() << sVersion << " " << i_sText << c_str;
    }

    return true;
}

const char *
SinceTag::Title() const
{
	return "Label";
}

UINT8
SinceTag::NrOfSpecialMeaningTokens() const
{
	return UINT8(-1);
}

AtTag *
SinceTag::GetFollower()
{
	return this;
}

void
SinceTag::do_StoreAt( DocuDisplay & o_rDisplay ) const
{
    o_rDisplay.Display_SinceTag( *this );
}

DocuText *
SinceTag::Text()
{
	return 0;
}


}   // namespace info
}   // namespace ary

