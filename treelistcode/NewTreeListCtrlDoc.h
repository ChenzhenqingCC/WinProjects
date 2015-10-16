// NewTreeListCtrlDoc.h : interface of the CNewTreeListCtrlDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWTREELISTCTRLDOC_H__73F685C3_D58D_45F3_ACCC_8154B1919BF1__INCLUDED_)
#define AFX_NEWTREELISTCTRLDOC_H__73F685C3_D58D_45F3_ACCC_8154B1919BF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNewTreeListCtrlDoc : public CDocument
{
protected: // create from serialization only
	CNewTreeListCtrlDoc();
	DECLARE_DYNCREATE(CNewTreeListCtrlDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewTreeListCtrlDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewTreeListCtrlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNewTreeListCtrlDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWTREELISTCTRLDOC_H__73F685C3_D58D_45F3_ACCC_8154B1919BF1__INCLUDED_)
