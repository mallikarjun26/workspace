// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__038FDE5F_BFAD_4C92_AB3E_2657A4DEB591__INCLUDED_)
#define AFX_STDAFX_H__038FDE5F_BFAD_4C92_AB3E_2657A4DEB591__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>

#define  uchar     unsigned char
#define  FALSE     0


template <class T>
T**	New2DPointer(int n1D, int n2D)
{
	T**		pp;
	typedef		T*	T_P;
	int		i, j;
	if(n1D <= 0 || n2D <= 0)
		return FALSE;
	
	pp = new T_P[n1D];
	if(!pp)
		return NULL;
	for(i=0; i<n1D; i++)
	{
		pp[i] = new T[n2D];
		if(!pp[i])
		{
			for(j=0; j<i; j++)
			{
				delete	pp[j];
			}
			delete	pp;
			return NULL;
		}
	}
	return pp;
}//New2DPointer

template <class T>
void	Delete2DPointer(T **pp, int n1D)
{
	int		i;
	if(pp == NULL)
		return;
	for(i=0; i<n1D; i++)
	{
		if(pp[i])
			delete[] pp[i];// the original is "delete pp[i];", add [] after delete to avoid the memory leak
	}
	delete[]	pp; // the original is "delete pp; ", add [] after delete to avoid the memory leak
	pp=NULL;
}//Delete2DPointer


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__038FDE5F_BFAD_4C92_AB3E_2657A4DEB591__INCLUDED_)
