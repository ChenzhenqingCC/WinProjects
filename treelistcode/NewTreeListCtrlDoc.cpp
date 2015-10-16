// NewTreeListCtrlDoc.cpp : implementation of the CNewTreeListCtrlDoc class
//

#include "stdafx.h"
#include "NewTreeListCtrl.h"

#include "NewTreeListCtrlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlDoc

IMPLEMENT_DYNCREATE(CNewTreeListCtrlDoc, CDocument)

BEGIN_MESSAGE_MAP(CNewTreeListCtrlDoc, CDocument)
	//{{AFX_MSG_MAP(CNewTreeListCtrlDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlDoc construction/destruction

CNewTreeListCtrlDoc::CNewTreeListCtrlDoc()
{
	// TODO: add one-time construction code here

}

CNewTreeListCtrlDoc::~CNewTreeListCtrlDoc()
{
}

BOOL CNewTreeListCtrlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlDoc serialization

void CNewTreeListCtrlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlDoc diagnostics

#ifdef _DEBUG
void CNewTreeListCtrlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNewTreeListCtrlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlDoc commands
