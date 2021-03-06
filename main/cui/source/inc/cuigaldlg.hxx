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



#ifndef _CUI_GALDLG_HXX_
#define _CUI_GALDLG_HXX_

#include <vos/thread.hxx>
#include <vcl/dialog.hxx>
#include <vcl/graph.hxx>
#include <vcl/fixed.hxx>
#include <vcl/button.hxx>
#include <vcl/lstbox.hxx>
#include <vcl/menu.hxx>
#include <vcl/edit.hxx>
#include <vcl/combobox.hxx>
#include <svl/slstitm.hxx>
#include <svtools/transfer.hxx>
#include <svtools/grfmgr.hxx>
#include <sfx2/tabdlg.hxx>
#include <svx/galctrl.hxx>
#include <svx/galmisc.hxx>
#include <com/sun/star/media/XPlayer.hpp>
#include <com/sun/star/ui/dialogs/XFolderPicker.hpp>
#include <svtools/dialogclosedlistener.hxx>

DECLARE_LIST( StringList, String* )

// ------------
// - Forwards -
// ------------

class Gallery;
class GalleryTheme;
class SearchProgress;
class TakeProgress;
class TPGalleryThemeProperties;

// ---------------
// - FilterEntry -
// ---------------

struct FilterEntry
{
	String	aFilterName;
};

// ----------------
// - SearchThread -
// ----------------

class SearchThread : public ::vos::OThread
{
private:

	SearchProgress*				mpProgress;
	TPGalleryThemeProperties*	mpBrowser;
	INetURLObject				maStartURL;

	void						ImplSearch( const INetURLObject& rStartURL,
                                            const ::std::vector< String >& rFormats,
                                            sal_Bool bRecursive );

	virtual void SAL_CALL		run();
    virtual void SAL_CALL       onTerminated();

public:

								SearchThread( SearchProgress* pProgess,
											  TPGalleryThemeProperties* pBrowser,
											  const INetURLObject& rStartURL );
    virtual						~SearchThread();
};

// ------------------
// - SearchProgress -
// ------------------

class SearchProgress : public ModalDialog
{
private:

	FixedText			aFtSearchDir;
    FixedLine           aFLSearchDir;
	FixedText			aFtSearchType;
    FixedLine           aFLSearchType;
	CancelButton		aBtnCancel;
	SearchThread		maSearchThread;

						DECL_LINK( ClickCancelBtn, void* );
	void				Terminate();

public:
						SearchProgress( Window* pParent, const INetURLObject& rStartURL );
						~SearchProgress() {};

						DECL_LINK( CleanUpHdl, void* );

	virtual short		Execute();
    virtual void        StartExecuteModal( const Link& rEndDialogHdl );
	void 				SetFileType( const String& rType ) { aFtSearchType.SetText( rType ); }
	void 				SetDirectory( const INetURLObject& rURL ) { aFtSearchDir.SetText( GetReducedString( rURL, 30 ) ); }
};

// --------------
// - TakeThread -
// --------------

class TakeThread : public ::vos::OThread
{
private:

	TakeProgress*				mpProgress;
	TPGalleryThemeProperties*	mpBrowser;
	List&						mrTakenList;

	virtual void SAL_CALL		run();
    virtual void SAL_CALL       onTerminated();

public:

								TakeThread( TakeProgress* pProgess, TPGalleryThemeProperties* pBrowser, List& rTakenList );
	virtual                     ~TakeThread();
};

// ----------------
// - TakeProgress -
// ----------------

class TakeProgress : public ModalDialog
{
private:

	FixedText			aFtTakeFile;
    FixedLine           aFLTakeProgress;
	CancelButton		aBtnCancel;
	TakeThread			maTakeThread;
	List				maTakenList;

						DECL_LINK( ClickCancelBtn, void* );
	void				Terminate();

public:

						TakeProgress( Window* pWindow );
						~TakeProgress() {};

						DECL_LINK( CleanUpHdl, void* );

	void 				SetFile( const INetURLObject& rURL ) { aFtTakeFile.SetText( GetReducedString( rURL, 30 ) ); }
	virtual short		Execute();
    virtual void        StartExecuteModal( const Link& rEndDialogHdl );
};

// ---------------------
// - ActualizeProgress -
// ---------------------

class ActualizeProgress : public ModalDialog
{
private:

	FixedText			aFtActualizeFile;
    FixedLine           aFLActualizeProgress;
	CancelButton		aBtnCancel;
	Timer*				pTimer;
	GalleryTheme*		pTheme;
	GalleryProgress		aStatusProgress;

						DECL_LINK( ClickCancelBtn, void* );
						DECL_LINK( TimeoutHdl, Timer* );
						DECL_LINK( ActualizeHdl, INetURLObject* pURL );

public:
						ActualizeProgress( Window* pWindow, GalleryTheme* pThm );
						~ActualizeProgress() {};

	virtual short		Execute();
};

// ---------------
// - TitleDialog -
// ---------------

class TitleDialog : public ModalDialog
{
private:

	OKButton			maOk;
	CancelButton		maCancel;
	HelpButton			maHelp;
    FixedLine           maFL;
	Edit				maEdit;

public:

						TitleDialog( Window* pParent, const String& rOldText );
	String				GetTitle() const { return maEdit.GetText(); }
};

// -------------------
// - GalleryIdDialog -
// -------------------

class GalleryIdDialog : public ModalDialog
{
private:

	OKButton		aBtnOk;
	CancelButton	aBtnCancel;
    FixedLine       aFLId;
	ListBox			aLbResName;
	GalleryTheme*	pThm;

					DECL_LINK( ClickOkHdl, void* );
					DECL_LINK( ClickResNameHdl, void* );

public:

					GalleryIdDialog( Window* pParent, GalleryTheme* pThm );
					~GalleryIdDialog() {}

	sal_uLong			GetId() const { return aLbResName.GetSelectEntryPos(); }
};

// --------------------------
// - GalleryThemeProperties -
// --------------------------

class GalleryThemeProperties : public SfxTabDialog
{
	ExchangeData*	pData;

	virtual void	PageCreated( sal_uInt16 nId, SfxTabPage &rPage );

public:

					GalleryThemeProperties( Window* pParent, ExchangeData* pData, SfxItemSet* pItemSet  );
					~GalleryThemeProperties() {}
};

// -------------------------
// - TPGalleryThemeGeneral -
// -------------------------

class TPGalleryThemeGeneral : public SfxTabPage
{
private:

	FixedImage			aFiMSImage;
	Edit				aEdtMSName;
	FixedLine			aFlMSGeneralFirst;
	FixedText			aFtMSType;
	FixedText			aFtMSShowType;
	FixedText			aFtMSPath;
	FixedText			aFtMSShowPath;
	FixedText			aFtMSContent;
	FixedText			aFtMSShowContent;
	FixedLine			aFlMSGeneralSecond;
	FixedText			aFtMSChangeDate;
	FixedText			aFtMSShowChangeDate;
	ExchangeData*		pData;

	virtual void		Reset( const SfxItemSet& ) {}
	virtual sal_Bool        FillItemSet( SfxItemSet& rSet );


public:

						TPGalleryThemeGeneral( Window* pParent, const SfxItemSet& rSet );
						~TPGalleryThemeGeneral() {}

	void				SetXChgData( ExchangeData* pData );
	const ExchangeData*	GetXChgData() const { return pData; }

	static SfxTabPage*	Create( Window* pParent, const SfxItemSet& rSet );
};

// ----------------------------
// - TPGalleryThemeProperties -
// ----------------------------

class TPGalleryThemeProperties : public SfxTabPage
{
	friend class SearchThread;
	friend class TakeProgress;
	friend class TakeThread;

	FixedText			aFtFileType;
	ComboBox			aCbbFileType;
    MultiListBox		aLbxFound;
	PushButton			aBtnSearch;
	PushButton			aBtnTake;
	PushButton			aBtnTakeAll;
	CheckBox			aCbxPreview;
	GalleryPreview		aWndPreview;

	ExchangeData*		pData;
	StringList			aFoundList;
	List				aFilterEntryList;
	Timer				aPreviewTimer;
	String				aLastFilterName;
	String				aPreviewString;
	INetURLObject		aURL;
	sal_uInt16				nCurFilterPos;
	sal_uInt16				nFirstExtFilterPos;
	sal_Bool				bEntriesFound;
	sal_Bool				bInputAllowed;
	sal_Bool				bTakeAll;
	sal_Bool				bSearchRecursive;

    ::com::sun::star::uno::Reference< ::svt::DialogClosedListener >                  xDialogListener;
    ::com::sun::star::uno::Reference< ::com::sun::star::media::XPlayer >             xMediaPlayer;
    ::com::sun::star::uno::Reference< ::com::sun::star::ui::dialogs::XFolderPicker > xFolderPicker;

    virtual void        Reset( const SfxItemSet& /*rSet*/ ) {}
    virtual sal_Bool        FillItemSet( SfxItemSet& /*rSet*/ ) { return sal_True; }
	::rtl::OUString 	addExtension( const ::rtl::OUString&, const ::rtl::OUString& );
	void				FillFilterList();

	void				SearchFiles();
	void 				TakeFiles();
	void				DoPreview();

						DECL_LINK( ClickPreviewHdl, void* );
						DECL_LINK( ClickCloseBrowserHdl, void* );
						DECL_LINK( ClickSearchHdl, void* );
						DECL_LINK( ClickTakeHdl, void* );
						DECL_LINK( ClickTakeAllHdl, void* );
						DECL_LINK( SelectFoundHdl, void* );
						DECL_LINK( SelectThemeHdl, void* );
						DECL_LINK( SelectFileTypeHdl, void* );
						DECL_LINK( DClickFoundHdl, void* );
						DECL_LINK( PreviewTimerHdl, void* );
                        DECL_LINK( EndSearchProgressHdl, SearchProgress* );
                        DECL_LINK( DialogClosedHdl, ::com::sun::star::ui::dialogs::DialogClosedEvent* );

public:
						TPGalleryThemeProperties( Window* pWindow, const SfxItemSet& rSet );
						~TPGalleryThemeProperties();

	void				SetXChgData( ExchangeData* pData );
	const ExchangeData*	GetXChgData() const { return pData; }

    void                StartSearchFiles( const String& _rFolderURL, short _nDlgResult );

	static SfxTabPage*	Create( Window* pParent, const SfxItemSet& rSet );
};

#endif // _CUI_GALDLG_HXX_
