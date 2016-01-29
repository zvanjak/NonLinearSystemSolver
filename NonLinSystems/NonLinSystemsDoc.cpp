// NonLinSystemsDoc.cpp : implementation of the CNLSysDoc class
// NonLinSystemsDoc.cpp : implementation of the CNLSysDoc class
//

#include "stdafx.h"
#include "NonLinSystems.h"

#include <math.h>
#include <time.h>

#include "NLSysDoc.h"
#include "NLSysView.h"
#include "SrvrItem.h"

#include "InputDiffEqDlg.h"
#include "LocateBifPointsDlg.h"
#include "BifPointsListDlg.h"
#include "CalcLyapExpDlg.h"
#include "LyapExpListDlg.h"
#include "NLSysOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLSysDoc

IMPLEMENT_DYNCREATE(CNLSysDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CNLSysDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CNLSysDoc)
	ON_COMMAND(ID_EDIT_DIFF_EQUATIONS, OnEditDiffEquations)
	ON_COMMAND(ID_ANAL_LOCATE_BIF, OnLocateBifurcation)
	ON_COMMAND(ID_VIEW_BIF_POINTS, OnViewBifurcationPoints)
	ON_COMMAND(ID_ANAL_CALC_LYAP_EXP, OnCalcLyapExp)
	ON_COMMAND(ID_VIEW_LYAP_EXP, OnViewLyapExp)
	ON_COMMAND(ID_OPTIONS_SET_OPTIONS, OnSetProgramOptions)
	ON_COMMAND(ID_ANAL_CALC_EQU_SOL, OnCalcEquSol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLSysDoc construction/destruction

CNLSysDoc::CNLSysDoc()
{
	int			i;

	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here
	memset(m_szEquations, 0, 500);
	memset(m_szStartVal, 0, 100 );
	memset(m_szParamVal, 0, 100 );
	memset(m_szIndVar, 0, MAX_VAR_NAME_LEN );

	strcpy( m_szIndVar, "t" );

	m_bSysDefined = FALSE;
	
	m_Options.m_nWindOpen = ProgramOptions::SAME_WIND;
	m_Options.m_dEps = 1e-5;
	m_Options.m_dStepMin = 1e-10;
	m_Options.m_bUseColor = TRUE;

	m_BifAnal.m_nSNBPointsNum = 0;
	m_BifAnal.m_nHopfPointsNum = 0;
	for( i=0; i<MAX_SNB_POINTS; i++ )
	{
		m_BifAnal.m_avecSNBVarValues[i] = NULL;
		m_BifAnal.m_avecSNBParValues[i] = NULL;
	}
	for( i=0; i<MAX_HOPF_POINTS; i++ )
	{
		m_BifAnal.m_avecHopfVarValues[i] = NULL;
		m_BifAnal.m_avecHopfParValues[i] = NULL;
	}

	m_LyapAnal.m_nLyapExpNum = 0;
	for( i=0; i<MAX_LYAP_EXP;i++ )
	{
		m_LyapAnal.m_avecLyapStartVal[i] = NULL;
		m_LyapAnal.m_avecLyapParVal[i] = NULL;
		m_LyapAnal.m_avecLyapExp[i] = NULL;
	}

	m_EquSolAnal.m_nEquSolNum = 0;
	for( i=0; i<MAX_EQU_SOL; i++ )
	{
		m_EquSolAnal.m_apvecParVal[i] = NULL;
		m_EquSolAnal.m_appvecSysVar[i] = NULL;
	}

	pCritIntegrate = new CCriticalSection();
} 

CNLSysDoc::~CNLSysDoc()
{
	UndefAllBifPoints();
	UndefAllLyapExp();
}

BOOL CNLSysDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	if( !IsEmbedded() )
		OnEditDiffEquations();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNLSysDoc server implementation

COleServerItem* CNLSysDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CNLSysSrvrItem* pItem = new CNLSysSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CNLSysDoc serialization

void CNLSysDoc::Serialize(CArchive& ar)
{
	CNLSysView		*pView[10];
	POSITION			pos;
	int		nRet, num, i, j, k, nRowNum,	nSize;

	Matrix		*oldMatT;
	Point3D		*oldPointLookTo;

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar.Write( m_szEquations, 500 );
		ar.Write( m_szStartVal, 100 );
		ar.Write( m_szParamVal, 100 );
		ar.Write( m_szIndVar, MAX_VAR_NAME_LEN );

		ar << m_BifAnal.m_nSNBPointsNum;
		for( i=0; i<m_BifAnal.m_nSNBPointsNum; i++ )
		{
			nSize = m_BifAnal.m_avecSNBVarValues[i]->GetDim();
			ar << nSize;
			ar.Write( &(*m_BifAnal.m_avecSNBVarValues[i])[0], sizeof(double) * nSize );

			nSize = m_BifAnal.m_avecSNBParValues[i]->GetDim();
			ar << nSize;
			ar.Write( &(*m_BifAnal.m_avecSNBParValues[i])[0], sizeof(double) * nSize );
		}

		ar << m_BifAnal.m_nHopfPointsNum;
		for( i=0; i<m_BifAnal.m_nHopfPointsNum; i++ )
		{
			nSize = m_BifAnal.m_avecHopfVarValues[i]->GetDim();
			ar << nSize;
			ar.Write( &(*m_BifAnal.m_avecHopfVarValues[i])[0], sizeof(double) * nSize );

			nSize = m_BifAnal.m_avecHopfParValues[i]->GetDim();
			ar << nSize;
			ar.Write( &(*m_BifAnal.m_avecHopfParValues[i])[0], sizeof(double) * nSize );
		}

		ar << m_LyapAnal.m_nLyapExpNum;
		for( i=0; i<m_LyapAnal.m_nLyapExpNum; i++ )
		{
			nSize = m_LyapAnal.m_avecLyapStartVal[i]->GetDim();
			ar << nSize;
			ar.Write( &(*m_LyapAnal.m_avecLyapStartVal[i])[0], sizeof(double) * nSize );

			nSize = m_LyapAnal.m_avecLyapParVal[i]->GetDim();
			ar << nSize;
			ar.Write( &(*m_LyapAnal.m_avecLyapParVal[i])[0], sizeof(double) * nSize );

			nSize = m_LyapAnal.m_avecLyapExp[i]->GetDim();
			ar << nSize;
			ar.Write( &(*m_LyapAnal.m_avecLyapExp[i])[0], sizeof(double) * nSize );
		}

		num=0;
		POSITION	 pos = GetFirstViewPosition();
		while( pos != NULL )
			pView[num++] = (CNLSysView *) GetNextView(pos);

		ar << num;			// zapiši koliko je bilo otvorenih prozora
		for( i=0; i<num; i++ )
		{
			ar.Write( &pView[i]->m_IntPar, sizeof(IntegrateParameters) );
			ar.Write( &pView[i]->m_LastSuccIntPar, sizeof(IntegrateParameters) );
			if( pView[i]->m_IntPar.m_vecInitCond != NULL )
			{
				nSize = m_DES.GetDim();
				ar << nSize;
				ar.Write( &(*(pView[i]->m_IntPar.m_vecInitCond))[0], sizeof(double) * nSize );

				nSize = m_DES.GetParNum();
				ar << nSize;
				ar.Write( &(*(pView[i]->m_IntPar.m_vecParamVal))[0], sizeof(double) * nSize );
			}
			else
			{
				nSize = 0;
				ar << nSize;
				ar << nSize;
			}
			if( pView[i]->m_IntPar.m_vecChgParList != NULL && pView[i]->m_PlotPar.m_nParNumPlot == PlotParameters::MULTI_PAR )
			{
				nSize = pView[i]->m_IntPar.m_nNumOfChgPar;
				ar << nSize;
				ar.Write( &(*(pView[i]->m_IntPar.m_vecChgParList))[0], sizeof(double) * nSize );
			}
			else
			{
				nSize = 0;
				ar << nSize;
			}
			if( pView[i]->m_LastSuccIntPar.m_vecInitCond != NULL )
			{
				nSize = m_DES.GetDim();
				ar << nSize;
				ar.Write( &(*(pView[i]->m_LastSuccIntPar.m_vecInitCond))[0], sizeof(double) * nSize );

				nSize = m_DES.GetParNum();
				ar << nSize;
				ar.Write( &(*(pView[i]->m_LastSuccIntPar.m_vecParamVal))[0], sizeof(double) * nSize );
			}
			else
			{
				nSize = 0;
				ar << nSize;
				ar << nSize;
			}
			
			ar.Write( &pView[i]->m_PlotPar, sizeof(PlotParameters) );

			for( j=0; j<MAX_PLOTTING_VARS; j++ )
			{
				if( pView[i]->m_aszLegend[j].IsEmpty() )
					ar << CString("");
				else
					ar << pView[i]->m_aszLegend[j];
			}
			ar << pView[i]->m_pntLegendPos;
			ar << pView[i]->m_pntBifLegendPos;

			ar << pView[i]->m_szTitle;

			ar << GetX(*pView[i]->m_PlotPar.pointLookTo);
			ar << GetY(*pView[i]->m_PlotPar.pointLookTo);
			ar << GetZ(*pView[i]->m_PlotPar.pointLookTo);

			ar << pView[i]->m_bCalcDone;
			ar << pView[i]->m_bCalcInProgress;
			ar << pView[i]->m_bSystemChanged;

			ar.Write( &pView[i]->m_AnalPar, sizeof(AnalyseParameters) );

			ar.Write( &pView[i]->m_nPlotType, sizeof( CNLSysView::PlotType) );

			if( pView[i]->m_nPlotType == CNLSysView::TRAJECTORY_PLOT || 
					pView[i]->m_nPlotType == CNLSysView::PHASE_PLOT_2D ||
					pView[i]->m_nPlotType == CNLSysView::PHASE_PLOT_3D )
			{
				if( pView[i]->m_PlotPar.m_nParNumPlot == PlotParameters::MULTI_PAR )
				{
					for( j=0; j<pView[i]->m_IntPar.m_nNumOfChgPar; j++ )
					{
						if( pView[i]->m_apvecX[j] != NULL )
						{
							nSize = pView[i]->m_apvecX[j]->GetDim();
							ar << nSize;
							ar.Write( &(*(pView[i]->m_apvecX[j]))[0], sizeof(double) * nSize );
						}
						else
						{
							nSize = 0;
							ar << nSize;
						}
		
						if( pView[i]->m_apmatY[j] != NULL )
						{
							nSize = ColNum(*pView[i]->m_apmatY[j]);
							nRowNum = RowNum(*pView[i]->m_apmatY[j]);
							ar << nSize;
							ar << nRowNum;
							for( k=0; k<nRowNum; k++ )
								ar.Write( (*(pView[i]->m_apmatY[j]))[k], sizeof(double) * nSize );
						}
						else
						{
							nSize = 0;
							nRowNum = 0;
							ar << nSize;
							ar << nRowNum;
						}
					}
				}
				else
				{
					if( pView[i]->m_vecX != NULL )
					{
						nSize = pView[i]->m_vecX->GetDim();
						ar << nSize;
						ar.Write( &(*(pView[i]->m_vecX))[0], sizeof(double) * nSize );
					}
					else
					{
						nSize = 0;
						ar << nSize;
					}
					
					if( pView[i]->m_matY != NULL )
					{
						nSize = ColNum(*pView[i]->m_matY);
						nRowNum = RowNum(*pView[i]->m_matY);
						ar << nSize;
						ar << nRowNum;
						for( j=0; j<nRowNum; j++ )
							ar.Write( (*(pView[i]->m_matY))[j], sizeof(double) * nSize );
					}
					else
					{
						nSize = 0;
						nRowNum = 0;
						ar << nSize;
						ar << nRowNum;
					}
				}
			}
			else if( pView[i]->m_nPlotType == CNLSysView::EQU_SURFACE_PLOT_2D )
			{
				ar.Write( &pView[i]->m_EquAnalPar, sizeof(EquSurfaceAnalyseParameter) );
				nSize = 200;
				nRowNum = 3;

				ar << nSize;
				ar << nRowNum;
				for( j=0; j<nRowNum; j++ )
					ar.Write( (*(pView[i]->m_matEquSol))[j], sizeof(double) * nSize );
			}
			else if( pView[i]->m_nPlotType == CNLSysView::FOURIER_SPECTRUM_PLOT )
			{
				ar.Write( &pView[i]->m_SpecAnal, sizeof(SpectrumAnalyseParameters) );

				if( pView[i]->m_vecFreq != NULL )
				{
					nSize = pView[i]->m_vecFreq->GetDim();
					ar << nSize;
					ar.Write( &(*(pView[i]->m_vecFreq))[0], sizeof(double) * nSize );
				}
				else
				{
					nSize = 0;
					ar << nSize;
				}
				
				if( pView[i]->m_matSpectrum != NULL )
				{
					nSize = ColNum(*pView[i]->m_matSpectrum);
					nRowNum = RowNum(*pView[i]->m_matSpectrum);
					ar << nSize;
					ar << nRowNum;
					for( j=0; j<nRowNum; j++ )
						ar.Write( (*(pView[i]->m_matSpectrum))[j], sizeof(double) * nSize );
				}
			}
			else if( pView[i]->m_nPlotType == CNLSysView::LYAP_EXP_PLOT )
			{
				ar.Write( &pView[i]->m_LyapAnal, sizeof(LyapExpAnalyseParameters) );
				if( pView[i]->m_vecParVal != NULL )
					ar.Write( &(*(pView[i]->m_vecParVal))[0], sizeof(double) * pView[i]->m_PlotPar.m_nRealCalcNum );

				if( pView[i]->m_matLyapExp != NULL )
					for( j=0; j<m_DES.GetDim(); j++ )
						ar.Write( (*(pView[i]->m_matLyapExp))[j], sizeof(double) * pView[i]->m_PlotPar.m_nRealCalcNum );
			}
		}
	}
	else
	{
		// TODO: add loading code here
		ar.Read( m_szEquations, 500 );
		ar.Read( m_szStartVal, 100 );
		ar.Read( m_szParamVal, 100 );
		ar.Read( m_szIndVar, MAX_VAR_NAME_LEN );

 		nRet = m_DES.InputDES( m_szEquations, m_szIndVar, m_szStartVal, m_szParamVal );
		if( nRet < 0 )
		{
			// pogreška u sustavu koji smo uèitali
			AfxMessageBox(m_DES.ErrorText(nRet));

			memset(m_szEquations, 0, 500);
			memset(m_szStartVal, 0, 100 );
			memset(m_szParamVal, 0, 100 );
			memset(m_szIndVar, 0, MAX_VAR_NAME_LEN );
		}
		else
		{
			ar >> m_BifAnal.m_nSNBPointsNum;
			for( i=0; i<m_BifAnal.m_nSNBPointsNum; i++ )
			{
				ar >> nSize;
				m_BifAnal.m_avecSNBVarValues[i] = new Vector(nSize);
				ar.Read( &(*m_BifAnal.m_avecSNBVarValues[i])[0], sizeof(double) * nSize );

				ar >> nSize;
				m_BifAnal.m_avecSNBParValues[i] = new Vector(nSize);
				ar.Read( &(*m_BifAnal.m_avecSNBParValues[i])[0], sizeof(double) * nSize );
			}

			// uèitavanje podataka za Hopfovu bifurkaciju
			ar >> m_BifAnal.m_nHopfPointsNum;
			for( i=0; i<m_BifAnal.m_nHopfPointsNum; i++ )
			{
				ar >> nSize;
				m_BifAnal.m_avecHopfVarValues[i] = new Vector(nSize);
				ar.Read( &(*m_BifAnal.m_avecHopfVarValues[i])[0], sizeof(double) * nSize );

				ar >> nSize;
				m_BifAnal.m_avecHopfParValues[i] = new Vector(nSize);
				ar.Read( &(*m_BifAnal.m_avecHopfParValues[i])[0], sizeof(double) * nSize );
			}

			// uèitavanje podataka za Lyapunovljeve eksponente
			ar >> m_LyapAnal.m_nLyapExpNum;
			for( i=0; i<m_LyapAnal.m_nLyapExpNum; i++ )
			{
				ar >> nSize;
				m_LyapAnal.m_avecLyapStartVal[i] = new Vector(nSize);
				ar.Read( &(*m_LyapAnal.m_avecLyapStartVal[i])[0], sizeof(double) * nSize );

				ar >> nSize;
				m_LyapAnal.m_avecLyapParVal[i] = new Vector(nSize);
				ar.Read( &(*m_LyapAnal.m_avecLyapParVal[i])[0], sizeof(double) * nSize );

				ar >> nSize;
				m_LyapAnal.m_avecLyapExp[i] = new Vector(nSize);
				ar.Read( &(*m_LyapAnal.m_avecLyapExp[i])[0], sizeof(double) * nSize );
			}

			ar >> num;
			// treba nam pozicija prvog prozora
			pos = GetFirstViewPosition();
				pView[0] = (CNLSysView *) GetNextView(pos);

			if( num > 1 )
				for( i=1; i<num; i++ )
					pView[0]->NewWindow();

			num = 0;
			pos = GetFirstViewPosition();
			while( pos != NULL )
				pView[num++] = (CNLSysView *) GetNextView(pos);

			for( i=0; i<num; i++ )
			{
				ar.Read( &pView[i]->m_IntPar, sizeof(IntegrateParameters) );
				ar.Read( &pView[i]->m_LastSuccIntPar, sizeof(IntegrateParameters) );

				ar >> nSize;
				if( nSize > 0 )
				{
					pView[i]->m_IntPar.m_vecInitCond = new Vector(nSize);
					ar.Read( &(*(pView[i]->m_IntPar.m_vecInitCond))[0], sizeof(double) * nSize );
				}
				else
					pView[i]->m_IntPar.m_vecInitCond = NULL;

				ar >> nSize;
				if( nSize > 0 )
				{
					pView[i]->m_IntPar.m_vecParamVal = new Vector(nSize);
					ar.Read( &(*(pView[i]->m_IntPar.m_vecParamVal))[0], sizeof(double) * nSize );
				}
				else
					pView[i]->m_IntPar.m_vecParamVal = NULL;

				ar >> nSize;
				if( nSize > 0 )
				{
					pView[i]->m_IntPar.m_vecChgParList = new Vector(nSize);
					ar.Read( &(*(pView[i]->m_IntPar.m_vecChgParList))[0], sizeof(double) * nSize );
				}
				else
					pView[i]->m_IntPar.m_vecChgParList = NULL;
			
				ar >> nSize;
				if( nSize > 0 )
				{
					pView[i]->m_LastSuccIntPar.m_vecInitCond = new Vector(nSize);
					ar.Read( &(*(pView[i]->m_LastSuccIntPar.m_vecInitCond))[0], sizeof(double) * nSize );
				}
				else
					pView[i]->m_LastSuccIntPar.m_vecInitCond = NULL;

				ar >> nSize;
				if( nSize > 0 )
				{
					pView[i]->m_LastSuccIntPar.m_vecParamVal = new Vector(nSize);
					ar.Read( &(*(pView[i]->m_LastSuccIntPar.m_vecParamVal))[0], sizeof(double) * nSize );
				}
				else
					pView[i]->m_LastSuccIntPar.m_vecParamVal = NULL;

				// zapamti gdje je u konstruktoru od CView adresiran prostor za matricu i toèku pogleda
				oldMatT = pView[i]->m_PlotPar.m_matT;
				oldPointLookTo = pView[i]->m_PlotPar.pointLookTo;

				ar.Read( &pView[i]->m_PlotPar, sizeof(PlotParameters));

				for( j=0; j<MAX_PLOTTING_VARS; j++ )
					ar >> pView[i]->m_aszLegend[j];
				ar >> pView[i]->m_pntLegendPos;
				ar >> pView[i]->m_pntBifLegendPos;

				ar >> pView[i]->m_szTitle;

				pView[i]->m_PlotPar.m_matT = oldMatT;
				pView[i]->m_PlotPar.pointLookTo = oldPointLookTo;

				double		val;
				ar >> val;
				SetX(*pView[i]->m_PlotPar.pointLookTo, val);
				ar >> val;
				SetY(*pView[i]->m_PlotPar.pointLookTo, val);
				ar >> val;
				SetZ(*pView[i]->m_PlotPar.pointLookTo, val);
				pView[i]->Set3DData();

				ar >> pView[i]->m_bCalcDone;
				ar >> pView[i]->m_bCalcInProgress;
				ar >> pView[i]->m_bSystemChanged;

				ar.Read( &pView[i]->m_AnalPar, sizeof(AnalyseParameters) );
				
				ar.Read( &pView[i]->m_nPlotType, sizeof( CNLSysView::PlotType) );
	
				if( pView[i]->m_nPlotType == CNLSysView::TRAJECTORY_PLOT || 
						pView[i]->m_nPlotType == CNLSysView::PHASE_PLOT_2D ||
						pView[i]->m_nPlotType == CNLSysView::PHASE_PLOT_3D )
				{
					if( pView[i]->m_PlotPar.m_nParNumPlot == PlotParameters::MULTI_PAR )
					{
						for( j=0; j<pView[i]->m_IntPar.m_nNumOfChgPar; j++ )
						{
							ar >> nSize;
							if( nSize > 0 )
							{
								pView[i]->m_apvecX[j] = new Vector( nSize );
								ar.Read( &(*(pView[i]->m_apvecX[j]))[0], sizeof(double) * nSize );
							}

							ar >> nSize;
							ar >> nRowNum;
							if( nSize > 0 )
							{
								pView[i]->m_apmatY[j] = new Matrix(nRowNum, nSize);

								for( k=0; k<nRowNum; k++ )
									ar.Read( (*(pView[i]->m_apmatY[j]))[k], sizeof(double) * nSize );
							}
						}
					}
					else
					{
						ar >> nSize;
						if( nSize > 0 )
						{
							pView[i]->m_vecX = new Vector( nSize );
							ar.Read( &(*(pView[i]->m_vecX))[0], sizeof(double) * nSize );
						}

						ar >> nSize;
						ar >> nRowNum;
						if( nSize > 0 )
						{
							pView[i]->m_matY = new Matrix(nRowNum, nSize);

							for( j=0; j<nRowNum; j++ )
								ar.Read( (*(pView[i]->m_matY))[j], sizeof(double) * nSize );
						}
					}
				}
				else if( pView[i]->m_nPlotType == CNLSysView::EQU_SURFACE_PLOT_2D )
				{
					ar.Read( &pView[i]->m_EquAnalPar, sizeof(EquSurfaceAnalyseParameter) );
					nSize = 200;
					nRowNum = 3;

					if( pView[i]->m_matEquSol == NULL )
						pView[i]->m_matEquSol = new Matrix(nRowNum,nSize);

					ar << nSize;
					ar << nRowNum;
					for( j=0; j<nRowNum; j++ )
						ar.Read( (*(pView[i]->m_matEquSol))[j], sizeof(double) * nSize );
				}
				else if( pView[i]->m_nPlotType == CNLSysView::FOURIER_SPECTRUM_PLOT )
				{
					ar.Read( &pView[i]->m_SpecAnal, sizeof(SpectrumAnalyseParameters) );
	
					ar >> nSize;
					pView[i]->m_vecFreq = new Vector( nSize );
					ar.Read( &(*(pView[i]->m_vecFreq))[0], sizeof(double) * nSize );

					ar >> nSize;
					ar >> nRowNum;
					pView[i]->m_matSpectrum = new Matrix(nRowNum, nSize);

					for( j=0; j<nRowNum; j++ )
						ar.Read( (*(pView[i]->m_matSpectrum))[j], sizeof(double) * nSize );
				}
				else if( pView[i]->m_nPlotType == CNLSysView::LYAP_EXP_PLOT )
				{
					ar.Read( &pView[i]->m_LyapAnal, sizeof(LyapExpAnalyseParameters) );
					pView[i]->m_vecParVal = new Vector(pView[i]->m_PlotPar.m_nRealCalcNum);
					ar.Read( &(*(pView[i]->m_vecParVal))[0], sizeof(double) * pView[i]->m_PlotPar.m_nRealCalcNum );

					pView[i]->m_matLyapExp = new Matrix(m_DES.GetDim(), pView[i]->m_PlotPar.m_nRealCalcNum);
					for( j=0; j<m_DES.GetDim(); j++ )
						ar.Read( (*(pView[i]->m_matLyapExp))[j], sizeof(double) * pView[i]->m_PlotPar.m_nRealCalcNum );
				}
			}
			m_bSysDefined = TRUE;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNLSysDoc diagnostics

#ifdef _DEBUG
void CNLSysDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CNLSysDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNLSysDoc commands

void CNLSysDoc::OnEditDiffEquations() 
{
	// TODO: Add your command handler code here
	CInputDiffEqDlg		dlg;
	
	char		OldParamVal[100];
	char		OldStartVal[100];
	char		OldIndVar[MAX_VAR_NAME_LEN];
	char		OldEquations[500];
	
	int			nRet=1, nRet2=0;

	// saèuvati æemo stare vrijednosti da bismo kasnije mogli provjeriti da li je korisnik
	// što promijenio
	strcpy(OldParamVal, m_szParamVal);
	strcpy(OldStartVal, m_szStartVal);
	strcpy(OldIndVar, m_szIndVar );
	strcpy(OldEquations, m_szEquations);
	
	dlg.m_szEquations = m_szEquations;
	dlg.m_szStartVal  = m_szStartVal;
	dlg.m_szParamVal	= m_szParamVal;
	dlg.m_szIndVar = m_szIndVar;

	do
	{
		nRet2 = dlg.DoModal();
 		
		if( nRet2 == IDOK )
		{
			memset(m_szEquations, 0, 500);
			memset(m_szStartVal, 0, 100 );
			memset(m_szParamVal, 0, 100 );
			memset(m_szIndVar, 0, MAX_VAR_NAME_LEN );

			strcpy(m_szEquations, dlg.m_szEquations);
			strcpy(m_szStartVal, dlg.m_szStartVal);
			strcpy(m_szParamVal, dlg.m_szParamVal);
			strcpy(m_szIndVar, dlg.m_szIndVar );

			nRet = m_DES.InputDES( m_szEquations, m_szIndVar, m_szStartVal, m_szParamVal );
			if( nRet < 0 )
				AfxMessageBox(m_DES.ErrorText(nRet));
			else
			{
				bool	bEqChanged;
				
				bEqChanged = strcmp(OldEquations, m_szEquations) == 0 ? false : true;
				if( bEqChanged || strcmp(OldParamVal, m_szParamVal) ||
						strcmp(OldIndVar, m_szIndVar) || strcmp(OldStartVal, m_szStartVal) )
				{
					// znaèi da je korisnik promijenio nešto u sustavu jednadžbi i da to treba javiti
					CNLSysView	*pView;
					POSITION		pos = GetFirstViewPosition();
					while (pos != NULL)
					{
						pView = (CNLSysView *) GetNextView(pos);
						pView->SetCalcDone(FALSE);
						pView->SystemChanged(TRUE);

						if( m_bSysDefined == FALSE )			// znaèi da je ovo prvi put da editiramo jednadžbe
						{
							// poslati æemo onda poèetne uvjete i vrijednosti parametara u view klasu
							strcpy( pView->m_IntPar.m_szParamVal, m_szParamVal );
							strcpy( pView->m_IntPar.m_szInitCond, m_szStartVal );

							pView->m_IntPar.m_vecInitCond = new Vector( m_DES.GetDim() );
							pView->m_IntPar.m_vecParamVal = new Vector( m_DES.GetParNum() );
							m_DES.FillVarValuesFromString( m_szStartVal, *pView->m_IntPar.m_vecInitCond );
							m_DES.FillParValuesFromString( m_szParamVal, *pView->m_IntPar.m_vecParamVal );
						} 
					}   			

					if( bEqChanged == true )
					{
						// ukoliko su se promijenile i jednadžbe sistema, ne vrijede nam prije izraèunate
						// bifurkacijske toèke i Lyapunovljevi eksponenti
						UndefAllBifPoints();
						UndefAllLyapExp();
					}

					SetModifiedFlag(TRUE);
					UpdateAllViews(NULL);
	//			UpdateAllItems(NULL);
					pView->GetInterval();
				}
			}
		}
	} while (nRet < 0 && nRet2 == IDOK );
	
	if( nRet2 == IDOK )				// znaèi da je sistem zadan bez greške
		m_bSysDefined = TRUE;
}

void CNLSysDoc::OnDeactivate() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateAllItems(NULL);
	
	COleServerDoc::OnDeactivate();
}

void CNLSysDoc::OnLocateBifurcation() 
{
	CLocateBifPointsDlg		dlg(this);
	Vector		*vecStart;
	double		omega;
	LPTSTR		p;
	int				retDlg=IDOK, ret, nNew=0, numSucc=0;

	if( m_DES.GetParNum() > 1 )
	{
		AfxMessageBox(IDS_BIF_POINTS_FIND_ONLY_FOR_ONE_PAR_SYS);
		goto out_locate_bif;
	}
	
	dlg.m_nHopf = -1;
	dlg.m_nSaddleNode = 0;
	dlg.m_nSearchNum = 1;
	dlg.m_nDim = m_DES.GetDim();

	p = dlg.m_szStartVal.GetBuffer(100);		strcpy(p, m_szStartVal);		dlg.m_szStartVal.ReleaseBuffer();
	p = dlg.m_szStartParVal.GetBuffer(100);		strcpy(p, m_szParamVal);		dlg.m_szStartParVal.ReleaseBuffer();
	
	while( dlg.m_nSearchNum > 0 && retDlg == IDOK )
	{
again_locate_bif:

		retDlg = dlg.DoModal();
		
		if( retDlg == IDOK )
		{
			if( dlg.m_nHopf == 0 )
				vecStart = new Vector(3*m_DES.GetDim()+m_DES.GetParNum()+1);
			else
				vecStart = new Vector(2*m_DES.GetDim()+m_DES.GetParNum());

			if( GetDialogValues(dlg,*vecStart) )
			{
				if( dlg.m_nHopf == 0 )
				{
					ret = m_DES.LocateHopfBifurcationPoints(*vecStart, &omega);
					if( ret == 0 )
					{
						nNew += PutInHopfBifArray(*vecStart,omega);
						numSucc++;
						dlg.m_nSearchNum--;
					}
				}
				else
				{
					ret = m_DES.LocateBifurcationPoints(vecStart);
					if( ret == 0 )
					{
						nNew += PutInSNBBifArray(*vecStart);
						numSucc++;
						dlg.m_nSearchNum--;
					}
				}
			}
			else
			{
				AfxMessageBox(IDS_WRONG_PAR_FOR_BIF_POINTS_LOC);
				vecStart->~Vector();
		
				goto again_locate_bif;
			}

			vecStart->~Vector();
		}
		else if( retDlg == ID_AUTOMATIC )
		{
			BifCalcParameters		*pBCP = new BifCalcParameters;
			
			pBCP->dP1 = dlg.m_dP1;
			pBCP->dP2 = dlg.m_dP2;
			pBCP->nSearchNum = dlg.m_nSearchNum;
			pBCP->pDocument  = this;
			pBCP->nType = (dlg.m_nHopf == 0) ? BifCalcParameters::HOPF_BIF : BifCalcParameters::SNB_BIF;

			AfxBeginThread(CalcBif, pBCP);
		}
	}
	if( retDlg != ID_AUTOMATIC )
	{
		char		str[200];
		sprintf(str, "Finished with calculation of bifurcation points : \n%d succesful searches, \n%d new bifurcation points found", numSucc, nNew );
		AfxMessageBox(str);
	}
out_locate_bif:
	;
}

void CNLSysDoc::OnViewBifurcationPoints() 
{
	int					i, j, len;
	char				str[500];
	CBifPointsListDlg		*pdlg;
	ComplexVector				cvec(m_DES.GetDim());
	
	pdlg = new CBifPointsListDlg;

	pdlg->m_pszSNBEigen = new CString *[m_BifAnal.m_nSNBPointsNum];
	pdlg->m_nSNBPointsNum = m_BifAnal.m_nSNBPointsNum;
	pdlg->m_pszHopfEigen = new CString *[m_BifAnal.m_nHopfPointsNum];
	pdlg->m_nHopfPointsNum = m_BifAnal.m_nHopfPointsNum;
	
	for( i=0; i<m_BifAnal.m_nSNBPointsNum; i++ )
	{
		memset(str,0,500);
		len = 0;
		for( j=0; j<m_DES.GetParNum(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", m_DES.GetParVarName(j), (*m_BifAnal.m_avecSNBParValues[i])[j] );
		
		for( j=0; j<m_DES.GetDim(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", m_DES.GetSysVarName(j), (*m_BifAnal.m_avecSNBVarValues[i])[j] );
		
		CString		add(str);
		pdlg->m_ctrlSNBList.AddString(add);	

		memset(str,0,500);
		len = 0;
		m_DES.CalcEigenvalues(0, *m_BifAnal.m_avecSNBVarValues[i], *m_BifAnal.m_avecSNBParValues[i], &cvec );
		for( j=0; j<m_DES.GetDim(); j++ )
		{
			len += sprintf(str + len, "%10.6f + i %10.6f\n\n", Re(cvec[j]), Im(cvec[j]) );
			str[len-2] = 13;
			str[len-1] = 10;
		}

		CString		addEigen(str);
		pdlg->m_pszSNBEigen[i] = new CString(str);
	}

	for( i=0; i<m_BifAnal.m_nHopfPointsNum; i++ )
	{
		memset(str,0,500);
		len = 0;
		for( j=0; j<m_DES.GetParNum(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", m_DES.GetParVarName(j), (*m_BifAnal.m_avecHopfParValues[i])[j] );
		
		for( j=0; j<m_DES.GetDim(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", m_DES.GetSysVarName(j), (*m_BifAnal.m_avecHopfVarValues[i])[j] );
		
		CString		add(str);
		pdlg->m_ctrlHopfList.AddString(add);	

		memset(str,0,500);
		len = 0;
		m_DES.CalcEigenvalues(0, *m_BifAnal.m_avecHopfVarValues[i], *m_BifAnal.m_avecHopfParValues[i], &cvec );
		for( j=0; j<m_DES.GetDim(); j++ )
		{
			len += sprintf(str + len, "%10.6f + i %10.6f\n\n", Re(cvec[j]), Im(cvec[j]) );
			str[len-2] = 13;
			str[len-1] = 10;
		}

		CString		addEigen(str);
		pdlg->m_pszHopfEigen[i] = new CString(str);
	}

	pdlg->UpdateData(FALSE);
}

void CNLSysDoc::OnCalcLyapExp() 
{
	CCalcLyapExpDlg			dlg;
	LPTSTR		p;

	dlg.m_dT = 1;
	dlg.m_dMinStepSize = 1e-20;
	dlg.m_nMaxIterNum = 10000;
	dlg.m_dEps = 1e-4;
	dlg.m_dEpsRel = 0.5e-4;
	dlg.m_dEpsAbs = 0.0;

	p = dlg.m_szStartVal.GetBuffer(100);		strcpy(p, m_szStartVal);		dlg.m_szStartVal.ReleaseBuffer();
	p = dlg.m_szStartParVal.GetBuffer(100);		strcpy(p, m_szParamVal);		dlg.m_szStartParVal.ReleaseBuffer();

again_calc_lyap:
	if( dlg.DoModal() == IDOK )
	{
		// ovi parametri nam trebaju za prijenos u nit koja æe u stvari raèunati L.exp.
		LyapCalcParameters	*pLCP = new LyapCalcParameters;
		pLCP->X = new Vector(m_DES.GetDim());
		pLCP->P = new Vector(m_DES.GetParNum());
		pLCP->pDocument = this;

		// a u ove vektore æemo pospremiti vrijednosti iz dijaloga
		Vector		X(m_DES.GetDim()), P(m_DES.GetParNum());//, vecLyap(m_DES.GetDim());

		if( GetDialogValues(dlg, X, P) == 0 )
		{
			// najprije provjeri da li smo veæ izraèunali L.exp za te rijednosti
			if( LyapExpAlreadyCalc(X,P) != TRUE )
			{
				*(pLCP->X) = X;
				*(pLCP->P) = P;
				pLCP->dT = dlg.m_dT;
				pLCP->dMinH = dlg.m_dMinStepSize;
				pLCP->dEps = dlg.m_dEps;
				pLCP->nMaxIter = dlg.m_nMaxIterNum;
				pLCP->dEpsRel = dlg.m_dEpsRel;
				pLCP->dEpsAbs = dlg.m_dEpsAbs;

				AfxBeginThread(CalcLyap, pLCP);
			}
		}
		else
		{
			AfxMessageBox(IDS_WRONG_PAR_FOR_LYAP_EXP_CALC);
	
			goto again_calc_lyap;
		}
	}
}

void CNLSysDoc::OnViewLyapExp() 
{
	CLyapExpListDlg			*pdlg;
	char			str[500];
	int				i, j, len;

	pdlg = new CLyapExpListDlg;

	pdlg->m_pszLyapExpVal = new CString *[m_LyapAnal.m_nLyapExpNum];
	pdlg->m_nLyapExpNum = m_LyapAnal.m_nLyapExpNum;
	
	for( i=0; i<m_LyapAnal.m_nLyapExpNum; i++ )
	{
		memset(str,0,500);
		len = 0;
		for( j=0; j<m_DES.GetParNum(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", m_DES.GetParVarName(j), (*m_LyapAnal.m_avecLyapParVal[i])[j] );
		
		for( j=0; j<m_DES.GetDim(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", m_DES.GetSysVarName(j), (*m_LyapAnal.m_avecLyapStartVal[i])[j] );
		
		CString		add(str);
		pdlg->m_ctrlValList.AddString(add);	

		memset(str,0,500);
		len = 0;
		for( j=0; j<m_DES.GetDim(); j++ )
		{
			len += sprintf(str + len, "%10.6f", (*m_LyapAnal.m_avecLyapExp[i])[j] );
			str[len-2] = 13;
			str[len-1] = 10;
		}

		CString		addLyap(str);
		pdlg->m_pszLyapExpVal[i] = new CString(str);
	}
}

void CNLSysDoc::OnCalcEquSol() 
{
	CCalcEquSolutionDlg		*pDlg;

	pDlg = new CCalcEquSolutionDlg(this);
	pDlg->pDoc = this;

	// kod inicijalizacije æemo za poèetne vrijednosti staviti one koje su definirane kod unosa sistem
	LPTSTR		p;
	p = pDlg->m_szSysVar.GetBuffer(100);		strcpy(p, m_szStartVal);		pDlg->m_szSysVar.ReleaseBuffer();
	p = pDlg->m_szParVal.GetBuffer(100);		strcpy(p, m_szParamVal);		pDlg->m_szParVal.ReleaseBuffer();
	
	pDlg->m_nMaxIter = 200;
	pDlg->m_dEps = 1e-5;

	pDlg->UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////
//						Pomoæne funkcije
//////////////////////////////////////////////////////////////////////////////////////////

int CNLSysDoc::GetDialogValues( CLocateBifPointsDlg &dlg, Vector &vecStart )
{
	double	dR, dI;
	char		StartVal[200], ParVal[200], sTemp[100];
	int			i, j, k, ret;
	Vector	X(m_DES.GetDim()), P(m_DES.GetParNum());
	
	strcpy(StartVal, LPCSTR(dlg.m_szStartVal) );
	strcpy(ParVal, LPCSTR(dlg.m_szStartParVal) );

	if( (ret = m_DES.FillVarValuesFromString(StartVal, X)) == 0 )
		ret = m_DES.FillParValuesFromString(ParVal, P);

	if( ret < 0 )
		AfxMessageBox(m_DES.ErrorText(ret));

	// ubacujemo poèetne vrijednosti za varijable sustava
	for( i=0; i<m_DES.GetDim(); i++ )
		vecStart[i] = X[i];

	// ubacujemo poèetne vrijednosti za parametre sustava
	if( dlg.m_nHopf == 0 )
	{
		for( i=0; i<m_DES.GetParNum(); i++ )
			vecStart[3*m_DES.GetDim()+i] = P[i];
		
		vecStart[3*m_DES.GetDim()+m_DES.GetParNum()] = 1.0;		// poèetna vrijednost za omega (iznos svojstvene vrijednosti)
	}
	else
		for( i=0; i<m_DES.GetParNum(); i++ )
			vecStart[2*m_DES.GetDim()+i] = P[i];

	// ubacujemo poèetne vrijednosti komponenata svojstvenog vektora
	i = j = k = 0;
	while( dlg.m_szEigenStart[i] != 0 )
	{
		memset(sTemp, 0, 100);

		while( dlg.m_szEigenStart[i] != 13 && dlg.m_szEigenStart[i] != 0 )
			sTemp[k++] = dlg.m_szEigenStart[i++];
		
		// sada u sTemp imamo iznos j-te poèetne vrijednost svojstvenog vektora
		if( dlg.m_nHopf == 0 )
		{
			GetComplexFromString(sTemp, &dR, &dI );
		
			vecStart[m_DES.GetDim()+j] = dR;
			vecStart[2*m_DES.GetDim()+j] = dI;
		}
		else
			vecStart[m_DES.GetDim()+j] = atof(sTemp);

		k = 0;
		j++;
		if( dlg.m_szEigenStart[i] != 0 )
			i+=2;			// da preskocimo ENTER i LINEFEED
		// ovdje treba paziti da se ne prekoraèi duljina stringa dlg.m_szEigenStart prilikom zadnje vrijednosti
		if( j == m_DES.GetDim() )
			goto out;
	}
	
out:
	if( ret == 0 )
		return TRUE;
	else
		return FALSE;
}

int CNLSysDoc::GetDialogValues( CCalcLyapExpDlg &dlg, Vector &vVar, Vector &vPar )
{
	char		StartVal[200], ParVal[200];
	int			ret;
	
	strcpy(StartVal, LPCSTR(dlg.m_szStartVal) );
	strcpy(ParVal, LPCSTR(dlg.m_szStartParVal) );

	if( (ret = m_DES.FillVarValuesFromString(StartVal, vVar)) == 0 )
		ret = m_DES.FillParValuesFromString(ParVal, vPar);

	if( ret < 0 )
		AfxMessageBox(m_DES.ErrorText(ret));

	return ret;
}

int CNLSysDoc::PutInSNBBifArray( Vector &vecBif )
{
	BOOL		bFound;
	int			i, ret=0;
	double	dx, dp, div;

	Vector	X(m_DES.GetDim()), P(m_DES.GetParNum());
	
	// najprije treba iz ovog vektora izvuæi vrijednosti varijabli vrijednosti parametara
	for( i=0; i<m_DES.GetDim(); i++ )
		X[i] = vecBif[i];
	for( i=0; i<m_DES.GetParNum(); i++ )
		P[i] = vecBif[2*m_DES.GetDim()+i];

	// a sada æemo provjeriti da li smo tu bifurkacijsku toèku veæ našli i unijeli u niz
	bFound = FALSE;
	for( i=0; i<m_BifAnal.m_nSNBPointsNum; i++ )
	{
		div = (Norm(X) + Norm(*m_BifAnal.m_avecSNBVarValues[i]) ) / 2;
		dx = Norm( X - *m_BifAnal.m_avecSNBVarValues[i] ) / div;
		 
		div = (Norm(P) + Norm(*m_BifAnal.m_avecSNBParValues[i]) ) / 2;
		dp = Norm( P - *m_BifAnal.m_avecSNBParValues[i] ) / div;
		
		// smatramo da se radi o istoj bufurkacijskoj toèci ukoliko se norme vektora razlikuju za
		// manje od 0.1 posto
		if( dx < BIF_POINT_TOL && dp < BIF_POINT_TOL )
		{
			bFound = TRUE;
			break;
		}
	}
	// ako je nismo veæ unijeli, uèiniti æemo to sada
	if( bFound == FALSE )
	{
		int		num = m_BifAnal.m_nSNBPointsNum++;
		m_BifAnal.m_avecSNBVarValues[num] = new Vector(m_DES.GetDim());
		m_BifAnal.m_avecSNBParValues[num] = new Vector(m_DES.GetParNum());

		*m_BifAnal.m_avecSNBVarValues[num] = X;
		*m_BifAnal.m_avecSNBParValues[num] = P;

		SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
		ret = 1;
	}
	return ret;
}

int CNLSysDoc::PutInSNBBifArray( Vector &X, Vector &P )
{
	BOOL		bFound;
	int			i, ret=0;
	double	dx, dp, div;

	// a sada æemo provjeriti da li smo tu bifurkacijsku toèku veæ našli i unijeli u niz
	bFound = FALSE;
	for( i=0; i<m_BifAnal.m_nSNBPointsNum; i++ )
	{
		div = (Norm(X) + Norm(*m_BifAnal.m_avecSNBVarValues[i]) ) / 2;
		dx = Norm( X - *m_BifAnal.m_avecSNBVarValues[i] ) / div;
		
		div = (Norm(P) + Norm(*m_BifAnal.m_avecSNBParValues[i]) ) / 2;
		dp = Norm( P - *m_BifAnal.m_avecSNBParValues[i] ) / div;
		
		// smatramo da se radi o istoj bufurkacijskoj toèci ukoliko se norme vektora razlikuju za
		// manje od 0.1 posto
		if( dx < BIF_POINT_TOL && dp < BIF_POINT_TOL )
		{
			bFound = TRUE;
			break;
		}
	}
	// ako je nismo veæ unijeli, uèiniti æemo to sada
	if( bFound == FALSE )
	{
		int		num = m_BifAnal.m_nSNBPointsNum++;
		m_BifAnal.m_avecSNBVarValues[num] = new Vector(m_DES.GetDim());
		m_BifAnal.m_avecSNBParValues[num] = new Vector(m_DES.GetParNum());

		*m_BifAnal.m_avecSNBVarValues[num] = X;
		*m_BifAnal.m_avecSNBParValues[num] = P;

		SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
		ret = 1;
	}
	return ret;
}

int CNLSysDoc::PutInHopfBifArray( Vector &vecBif, double omega )
{
	BOOL		bFound, bSNBBif = FALSE;
	int			i, ret=0;
	double	dx, dp, div;

	Vector	X(m_DES.GetDim()), P(m_DES.GetParNum());
	
	// najprije treba iz ovog vektora izvuæi vrijednosti varijabli vrijednosti parametara
	for( i=0; i<m_DES.GetDim(); i++ )
		X[i] = vecBif[i];
	for( i=0; i<m_DES.GetParNum(); i++ )
		P[i] = vecBif[3*m_DES.GetDim()+i];

	// s obzirom da algoritam za nalaženje Hopfove bifurkacije nalazi i toèke za SNB bif., ovdje æemo
	// takve toèke odvojiti i poslati u njihov niz
	// najprije raèunamo svojstvene vrijednosti
	ComplexVector	cvecEigen(m_DES.GetDim());
	m_DES.CalcEigenvalues(0, X, P, &cvecEigen);
	for( i=0; i<m_DES.GetDim(); i++ )
		if( fabs(Re(cvecEigen[i])) + fabs(Im(cvecEigen[i])) < 5e-5 )
		{
			// znaèi našli smo zero-svojstvenu vrijednost
			bSNBBif = TRUE;
			PutInSNBBifArray(X,P);
		}
	
	if( bSNBBif != TRUE )
	{
		// a sada æemo provjeriti da li smo tu bifurkacijsku toèku veæ našli i unijeli u niz
		bFound = FALSE;
		for( i=0; i<m_BifAnal.m_nHopfPointsNum; i++ )
		{
			div = (Norm(X) + Norm(*m_BifAnal.m_avecHopfVarValues[i]) ) / 2;
			dx = Norm( X - *m_BifAnal.m_avecHopfVarValues[i] ) / div;
			
			div = (Norm(P) + Norm(*m_BifAnal.m_avecHopfParValues[i]) ) / 2;
			dp = Norm( P - *m_BifAnal.m_avecHopfParValues[i] ) / div;
			
			// smatramo da se radi o istoj bufurkacijskoj toèci ukoliko se norme vektora razlikuju za
			// manje od BIF_POINT_TOL posto
			if( dx < BIF_POINT_TOL && dp < BIF_POINT_TOL )
			{
				bFound = TRUE;
				break;
			}
		}
		// ako je nismo veæ unijeli, uèiniti æemo to sada
		if( bFound == FALSE )
		{
			int		num = m_BifAnal.m_nHopfPointsNum++;
			m_BifAnal.m_avecHopfVarValues[num] = new Vector(m_DES.GetDim());
			m_BifAnal.m_avecHopfParValues[num] = new Vector(m_DES.GetParNum());

			*m_BifAnal.m_avecHopfVarValues[num] = X;
			*m_BifAnal.m_avecHopfParValues[num] = P;

			SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
			ret = 1;
		}
	}
	return ret;
}

void CNLSysDoc::UndefAllBifPoints( void )
{
	int		i;

	for( i=0; i<m_BifAnal.m_nSNBPointsNum; i++ )
	{
		m_BifAnal.m_avecSNBVarValues[i]->~Vector();
		m_BifAnal.m_avecSNBVarValues[i] = NULL;
		m_BifAnal.m_avecSNBParValues[i]->~Vector();
		m_BifAnal.m_avecSNBParValues[i] = NULL;
	}
	for( i=0; i<m_BifAnal.m_nHopfPointsNum; i++ )
	{
		m_BifAnal.m_avecHopfVarValues[i]->~Vector();
		m_BifAnal.m_avecHopfVarValues[i] = NULL;
		m_BifAnal.m_avecHopfParValues[i]->~Vector();
		m_BifAnal.m_avecHopfParValues[i] = NULL;
	}

	m_BifAnal.m_nSNBPointsNum = 0;
	m_BifAnal.m_nHopfPointsNum = 0;

	SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
}


BOOL	CNLSysDoc::LyapExpAlreadyCalc( Vector &X, Vector &P )
{
	int			i;

	for( i=0; i<m_LyapAnal.m_nLyapExpNum; i++ )
		if( (X == *m_LyapAnal.m_avecLyapStartVal[i]) &&
				(P == *m_LyapAnal.m_avecLyapParVal[i]) )
			return TRUE;
	
	return FALSE;
}

int CNLSysDoc::PutInCalcLyapArray( Vector &X, Vector &P, Vector &vLyap )
{
	int			i, ret=-1;
	BOOL		bFound = FALSE;

	if( m_LyapAnal.m_nLyapExpNum < MAX_LYAP_EXP )
	{
		int			num = m_LyapAnal.m_nLyapExpNum;
		double	dx, dp, div;
				
		// najprije treba provjeriti da li smo za te vrijednosti veæ ubacili u niz
		for( i=0; i<num; i++ )
		{
			div = (Norm(X) + Norm(*m_LyapAnal.m_avecLyapStartVal[i]) ) / 2;
			dx = Norm( X - *m_LyapAnal.m_avecLyapStartVal[i] ) / div;
			
			div = (Norm(P) + Norm(*m_LyapAnal.m_avecLyapParVal[i]) ) / 2;
			dp = Norm( P - *m_LyapAnal.m_avecLyapParVal[i] ) / div;
			
			// smatramo da se radi o istoj bufurkacijskoj toèci ukoliko se norme vektora razlikuju za
			// manje od EQU_SOL_TOL posto
			if( dx < LYAP_EXP_TOL && dp < LYAP_EXP_TOL )
			{
				bFound = TRUE;
				ret = i;
				break;
			}
		}
		if( bFound != TRUE )
		{
			m_LyapAnal.m_avecLyapStartVal[num] = new Vector(m_DES.GetDim());
			m_LyapAnal.m_avecLyapParVal[num] = new Vector(m_DES.GetParNum());
			m_LyapAnal.m_avecLyapExp[num] = new Vector(m_DES.GetDim());

			*m_LyapAnal.m_avecLyapStartVal[num] = X;
			*m_LyapAnal.m_avecLyapParVal[num] = P;
			*m_LyapAnal.m_avecLyapExp[num] = vLyap;
			
			ret = num;
			m_LyapAnal.m_nLyapExpNum++;
			
			SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
		}

		return ret;
	}
	else
		return ret;
}

void CNLSysDoc::UndefAllLyapExp( void )
{
	for( int i=0;i<m_LyapAnal.m_nLyapExpNum; i++ )
	{
		m_LyapAnal.m_avecLyapStartVal[i]->~Vector();
		m_LyapAnal.m_avecLyapStartVal[i] = NULL;
		m_LyapAnal.m_avecLyapParVal[i]->~Vector();
		m_LyapAnal.m_avecLyapParVal[i] = NULL;
		m_LyapAnal.m_avecLyapExp[i]->~Vector();
		m_LyapAnal.m_avecLyapExp[i] = NULL;
	}
	
	m_LyapAnal.m_nLyapExpNum = 0;
	SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
}


int CNLSysDoc::PutInEquSolArray( Vector &X, Vector &P, int *nPosInParList )
{
	int		ret = -1, i, j, numP=-1;

	if( m_EquSolAnal.m_nEquSolNum < MAX_EQU_SOL )
	{
		int			num = m_EquSolAnal.m_nEquSolNum;
		double	dx, dp, div;
		bool		bFoundPar=FALSE, bFoundVar=FALSE;;

		// najprije treba provjeriti da li smo za te vrijednosti veæ ubacili u niz
		for( i=0; i<num; i++ )
		{
			// najprije tražimo u nizu parametara iste vrijednosti
			div = (Norm(P) + Norm(*m_EquSolAnal.m_apvecParVal[i]) ) / 2;
			dp = Norm( P - *m_EquSolAnal.m_apvecParVal[i] ) / div;
			
			if( dp < EQU_SOL_TOL )
			{
				bFoundPar = TRUE;
				ret = i;
				// ako smo našli da smo za te vrijednosti parametara veæ izraèunali ravnotežne toèke
				// sada treba vidjeti da li smo dobili i ista rješenja
				j = 0;
				while( m_EquSolAnal.m_appvecSysVar[i][j] != NULL && j < MAX_DIFF_SYS_VAR_PER_EQU_SOL )
				{
					div = (Norm(X) + Norm(*m_EquSolAnal.m_appvecSysVar[i][j]) ) / 2;
					dx = Norm( X - *m_EquSolAnal.m_appvecSysVar[i][j] ) / div;

					if( dx < EQU_SOL_TOL )
					{
						bFoundVar = TRUE;
						numP = j;
						break;
					}
					j++;
				}
				break;
			}
		}
		if( bFoundPar != TRUE )
		{
			// znaèi da za te vrijednosti parametara uopæe nismo tražili ravnotežna rješenja
			m_EquSolAnal.m_apvecParVal[num] = new Vector(m_DES.GetParNum());
			*m_EquSolAnal.m_apvecParVal[num] = P;

			m_EquSolAnal.m_appvecSysVar[num] = new Vector*[MAX_DIFF_SYS_VAR_PER_EQU_SOL];
			for( j=0; j<MAX_DIFF_SYS_VAR_PER_EQU_SOL; j++ )
				m_EquSolAnal.m_appvecSysVar[num][j] = NULL;

			m_EquSolAnal.m_appvecSysVar[num][0] = new Vector(m_DES.GetDim());
			*m_EquSolAnal.m_appvecSysVar[num][0] = X;

			ret = num;
			*nPosInParList = 0;
			m_EquSolAnal.m_nEquSolNum++;
		}
		else if( bFoundVar != TRUE )
		{
			// znaèi da postoje izraèunata ravnotežna rješenja za vrijednosti parametara,
			// ali nisu ista kao ona koja smo mi dobili što se tièe vrijednosti varijabli sustava
			if( j < MAX_DIFF_SYS_VAR_PER_EQU_SOL )
			{
				m_EquSolAnal.m_appvecSysVar[ret][j] = new Vector(m_DES.GetDim());
				*m_EquSolAnal.m_appvecSysVar[ret][j] = X;

				*nPosInParList = j;
			}
			else
				ret = -1;
		}
	}

	return ret;
}

void CNLSysDoc::UndefAllEquSol( void )
{
	int		i, j;

	for( i=0; i<m_EquSolAnal.m_nEquSolNum; i++ )
	{
		m_EquSolAnal.m_apvecParVal[i]->~Vector();
		m_EquSolAnal.m_apvecParVal[i] = NULL;
		if( m_EquSolAnal.m_appvecSysVar[i] != NULL )
		{
			j = 0;
			while( m_EquSolAnal.m_appvecSysVar[i][j] != NULL )
			{
				m_EquSolAnal.m_appvecSysVar[i][j]->~Vector();
				m_EquSolAnal.m_appvecSysVar[i][j] = NULL;
			}
		}
	}

	m_EquSolAnal.m_nEquSolNum = 0;
	SetModifiedFlag(TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// CNLSysDoc help functions

UINT Integrate( LPVOID lParam )
{
	CNLSysDoc		*pDoc;
	CNLSysView	*pView = (CNLSysView *) lParam, *pViewReal;
	
	double	t1, t2, dStep, dHMin, dEps, dSave;
	int			nOK, nBAD, nKount;
	int			i, nRet=1;

	// pokazivaè na prozor koji je zatražio integraciju
	pDoc  = pView->GetDocument();

	// ovo m_matX == NULL je za sluèaj da odmah u prozoru prikažemo ravnotežnu krivulju i onda idemo crtati
	// fazni dijagram. Pošto se u prva dva donja parametra ništa nije promjenilo, program zakljuèi da
	// IMA podatke, a u stvari ih nema i kiksa
	if( pView->m_IntPar.m_bParametersChanged == TRUE || pView->m_vecX == NULL )
	{
		// ako se parametri bitni za integraciju nisu promjenili
		// nema potrebe da sve ponovo integriramo
 		t1 = pView->m_IntPar.m_dStart;
		t2 = pView->m_IntPar.m_dEnd;
		dStep = pView->m_IntPar.m_dStep;
		dSave = (t2-t1) / pView->m_IntPar.m_nSaveValNum;

		// sinkronizacijski mehanizam
		//	ukoliko se ova funkcija (Integrate) pozove paralelno više puta, ona nit koja prva
		//	uhvati ocaj CSingleLock objekt æe se izvršavati, dok æe ostale saèekati
		CSingleLock		lockIntegrate(pDoc->pCritIntegrate);
		lockIntegrate.Lock();

		// poèeli smo s integracijom
		pView->m_bCalcDone = FALSE;
		pView->m_bCalcInProgress = TRUE;

		// moguæe su dvije situacije
		// 1 - normalna itnegracija u jednom prozoru za zadane vrijednosti varijabli i parametara
		// 2 - višestruka integracija u jednom prozoru, ukoliko je zadan parametar koj iæe se mijenjati
		//     i njegove vrijednosti
		if( pDoc->m_Options.m_nWindOpen == ProgramOptions::SAME_WIND 
				&& pView->m_PlotPar.m_nParNumPlot == PlotParameters::MULTI_PAR )
		{
			if( pView->m_vecX != NULL )
			{
				delete pView->m_vecX;
				pView->m_vecX = NULL;
			}
			if( pView->m_matY != NULL )
			{
				delete pView->m_matY;
				pView->m_matY = NULL;
			}

			for( i=0; i<pView->m_IntPar.m_nNumOfChgPar; i++ )
			{
				pView->m_apvecX[i] = new Vector(pView->m_IntPar.m_nSaveValNum);
				pView->m_apmatY[i] = new Matrix(pDoc->m_DES.GetDim(), pView->m_IntPar.m_nSaveValNum );

				// za svaku novu vrijednost parametra, treba ga ubaciti u niz prije integracije
				if( pView->m_IntPar.m_enChgType == IntegrateParameters::SYS_VAR )
					(*pView->m_IntPar.m_vecInitCond)[pView->m_IntPar.m_nVarNum] = (*pView->m_IntPar.m_vecChgParList)[i];
				else
					(*pView->m_IntPar.m_vecParamVal)[pView->m_IntPar.m_nVarNum] = (*pView->m_IntPar.m_vecChgParList)[i];

				pDoc->m_DES.SetInitialCond(*pView->m_IntPar.m_vecInitCond);
				pDoc->m_DES.SetParamValues(*pView->m_IntPar.m_vecParamVal);

				dHMin = pDoc->m_Options.m_dStepMin;
				dEps = pView->m_IntPar.m_dEps;
				
				nRet = (pDoc->m_DES).Integrate(t1, t2, dEps, dStep, dHMin, &nOK, &nBAD, &nKount,
 								dSave, pView->m_apvecX[i], pView->m_apmatY[i] );

				pView->m_LastSuccIntPar.m_dStep  = pView->m_IntPar.m_dStep;
				pView->m_LastSuccIntPar.m_dStart = pView->m_IntPar.m_dStart;
				pView->m_LastSuccIntPar.m_dEnd	 = pView->m_IntPar.m_dEnd;
				pView->m_LastSuccIntPar.m_dEps   = pView->m_IntPar.m_dEps;
				pView->m_LastSuccIntPar.m_nSaveValNum = pView->m_IntPar.m_nSaveValNum;

				// u LastSuccInt æemo zabilježiti parametre s kojima smo integrirali prvi put
				if( i == 0 )
				{
					if( pView->m_LastSuccIntPar.m_vecInitCond == NULL )
					{
						pView->m_LastSuccIntPar.m_vecInitCond = new Vector(pDoc->m_DES.GetDim());
						pView->m_LastSuccIntPar.m_vecParamVal = new Vector(pDoc->m_DES.GetParNum());
					}
					*pView->m_LastSuccIntPar.m_vecInitCond = *pView->m_IntPar.m_vecInitCond;
					if( pDoc->m_DES.GetParNum() > 0 )
						*pView->m_LastSuccIntPar.m_vecParamVal = *pView->m_IntPar.m_vecParamVal;
				}

				pView->m_IntPar.m_nIntSteps[i] = nKount;

				if( nRet <= 0 )
					AfxMessageBox(pDoc->m_DES.ErrorText(nRet));
			}

			// da i druge niti koje èekaju na integraciju mogu poèeti
			lockIntegrate.Unlock();
		}
		else
		{
			if( pView->m_vecX != NULL && pView->m_vecX->GetDim() != pView->m_IntPar.m_nSaveValNum )
			{
				delete pView->m_vecX;
				pView->m_vecX = NULL;
			}
			if( pView->m_matY != NULL && 
				( RowNum(*pView->m_matY) != pDoc->m_DES.GetDim() ||
					ColNum(*pView->m_matY) != pView->m_IntPar.m_nSaveValNum ) )
			{
					delete pView->m_matY;
					pView->m_matY = NULL;
			}

			if( pView->m_vecX == NULL)
				pView->m_vecX = new Vector(pView->m_IntPar.m_nSaveValNum);
			if( pView->m_matY == NULL)
				pView->m_matY = new Matrix(pDoc->m_DES.GetDim() ,pView->m_IntPar.m_nSaveValNum);

			pDoc->m_DES.SetInitialCond(*pView->m_IntPar.m_vecInitCond);
			pDoc->m_DES.SetParamValues(*pView->m_IntPar.m_vecParamVal);

			dHMin = pDoc->m_Options.m_dStepMin;
			dEps = pView->m_IntPar.m_dEps;
			
			nRet = (pDoc->m_DES).Integrate(t1, t2, dEps, dStep, dHMin, &nOK, &nBAD, &nKount,
 								dSave, pView->m_vecX, pView->m_matY );

			// da i druge niti koje èekaju na integraciju mogu poèeti
			lockIntegrate.Unlock();

			pView->m_LastSuccIntPar.m_dStep  = pView->m_IntPar.m_dStep;
			pView->m_LastSuccIntPar.m_dStart = pView->m_IntPar.m_dStart;
			pView->m_LastSuccIntPar.m_dEnd	 = pView->m_IntPar.m_dEnd;
			pView->m_LastSuccIntPar.m_dEps   = pView->m_IntPar.m_dEps;
			pView->m_LastSuccIntPar.m_nSaveValNum = pView->m_IntPar.m_nSaveValNum;

			if( pView->m_LastSuccIntPar.m_vecInitCond == NULL )
			{
				pView->m_LastSuccIntPar.m_vecInitCond = new Vector(pDoc->m_DES.GetDim());
				pView->m_LastSuccIntPar.m_vecParamVal = new Vector(pDoc->m_DES.GetParNum());
			}
			*pView->m_LastSuccIntPar.m_vecInitCond = *pView->m_IntPar.m_vecInitCond;
			if( pDoc->m_DES.GetParNum() > 0 )
				*pView->m_LastSuccIntPar.m_vecParamVal = *pView->m_IntPar.m_vecParamVal;

			pView->m_PlotPar.m_nRealCalcNum = nKount;
			pView->m_IntPar.m_nIntSteps[0] = nKount;

			if( nRet <= 0 )
				AfxMessageBox(pDoc->m_DES.ErrorText(nRet));
		}
	}

	// možda je prilikom integracije i došlo do pogreške, ali æemo svejedno
	// iscrtati ono što smo uspjeli izintegrirati, pa da vidimo što se zbiva
	// indicira da smo gotovi s integracijom
	pView->m_bCalcDone = TRUE;
	pView->m_bCalcInProgress = FALSE;

	pView->GetInterval();
	pView->SystemChanged(FALSE);

	// sada još treba poslati poruku prozoru da mu sadržaj ne valja, 
	// odnosno da ga treba ponovo iscrtati
	pViewReal = (CNLSysView *) CNLSysView::FromHandle( pView->m_IntPar.m_hWnd );
	pViewReal->Invalidate();

	return nRet;
}


UINT	CalcLyap( LPVOID lParam )
{
	LyapCalcParameters	*pLCP = (LyapCalcParameters *) lParam;

	CNLSysDoc	*pDoc = pLCP->pDocument;
	Vector		vecLyap(pDoc->m_DES.GetDim());
	int				ret;

	ret = pDoc->m_DES.CalcLyapExp(*(pLCP->X), *(pLCP->P), vecLyap, pLCP->dT, pLCP->dMinH, 
																	pLCP->dEps, pLCP->nMaxIter, pLCP->dEpsRel, pLCP->dEpsAbs);

	if( ret == 0 )
		pDoc->PutInCalcLyapArray(*(pLCP->X), *(pLCP->P), vecLyap);

	AfxMessageBox(IDS_FINISHED_WITH_LYAP_EXP_CALC);

	delete pLCP->X;
	delete pLCP->P;
	delete pLCP;

	return ret;
}

UINT	CalcLyapMany( LPVOID lParam )
{
	LyapCalcParameters	*pLCP = (LyapCalcParameters *) lParam;

	CNLSysDoc		*pDoc = pLCP->pDocument;
	CNLSysView	*pView = pLCP->pView;
	CNLSysView	*pViewReal = (CNLSysView *) CNLSysView::FromHandle( pLCP->hWnd );

	Vector		vecLyap(pDoc->m_DES.GetDim());

	pView->m_bCalcDone = FALSE;
	pView->m_bCalcInProgress = TRUE;

	int			i, j, ret, num;
	double	x;
	
	num = 0;
	for( x=pLCP->dX1; x<=pLCP->dX2; x+=pLCP->dStep )
	{
		j = pLCP->nChangeValNum;
		
		if( pLCP->bChangePar )	// znaèi da smo za mijenjajuæi parametar odabrali parametar sustava
			(*pLCP->P)[j] = x;
		else										// a ovdje smo ako mijenjamo po varijabli sustava
			(*pLCP->X)[j] = x;

		// prije pokretanja izraèunavanja, provjeriti da li smo možda veæ izraèunal L. exp. za te vrijednosti parametara
		ret = pDoc->m_DES.CalcLyapExp(*(pLCP->X), *(pLCP->P), vecLyap, pLCP->dT, pLCP->dMinH, 
																		pLCP->dEps, pLCP->nMaxIter, pLCP->dEpsRel, pLCP->dEpsAbs);

		if( ret == 0 )
		{
			// najprije æemo ga staviti u niz izraèunatih L. exp
			i = pDoc->PutInCalcLyapArray(*(pLCP->X), *(pLCP->P), vecLyap);
			
			// a zatim i u vektor i matricu pomoæu kojih æe se iscrtavati
			(*pView->m_vecParVal)[num] = x;
			for( j=0; j<pDoc->m_DES.GetDim(); j++ )
				(*pView->m_matLyapExp)[j][num] = (*pDoc->m_LyapAnal.m_avecLyapExp[i])[j];
			num++;
		}
	}
	
	pView->m_bCalcDone = TRUE;
	pView->m_bCalcInProgress = FALSE;
	
	pView->m_PlotPar.m_nRealCalcNum = num;
	pView->SystemChanged(FALSE);

	pViewReal->Invalidate();

	delete pLCP->X;
	delete pLCP->P;
	delete pLCP;

	return 0;
}

UINT	CalcBif( LPVOID lParam )
{
	BifCalcParameters *pBCP = (BifCalcParameters *) lParam;
	CNLSysDoc					*pDoc = pBCP->pDocument;

	Vector		*vecStart, X(pDoc->m_DES.GetDim()), P(pDoc->m_DES.GetParNum());
	double		dPar, val, omega;
	int				i, j, ret, nNew=0, numSucc=0;

	for( i=0; i<pDoc->m_DES.GetDim(); i++ )
		X[i] = pDoc->m_DES.GetStartValue(i);

	for( i=0; i<pBCP->nSearchNum; i++ )
	{
		srand( (unsigned)time( NULL ) );
		// najprije æemo sluèajno odrediti vrijednost parametra, ali u zadanim granicama
		dPar = ((double)rand()) / RAND_MAX * (pBCP->dP2 - pBCP->dP1) / 2 + pBCP->dP1;

		// zatim treba odrediti poèetne vrijednosti
		// najprije æemo poèetne vrijednosti za varijable sustava postaviti na ravnotežne vrijednosti
		// za danu vrijednost parametra
		P[0] = dPar;
		ret = pDoc->m_DES.CalcEqvSolution( &X, P );

		if( pBCP->nType == BifCalcParameters::HOPF_BIF )
		{
			// kod raèunanja Hopfa neæemo kretati iz ravnotežnog rješenja jer onda program za traženje
			// upada u lokalne minimume
//			for( j=0; j<pDoc->m_DES.GetDim(); j++ )
//				X[j] = pDoc->m_DES.GetStartValue(j);

			vecStart = new Vector(3*pDoc->m_DES.GetDim()+pDoc->m_DES.GetParNum()+1);

			for( j=0; j<pDoc->m_DES.GetDim(); j++ )
				(*vecStart)[j] = X[j];
		
			// iako mi imamo samo jedan parametar, ali možda æemo kasnije nadograditi, a svejedno je
			for( j=0; j<pDoc->m_DES.GetParNum(); j++ )
				(*vecStart)[3 * pDoc->m_DES.GetDim() + j] = P[j];
			
			// a ovdje iznos imaginarne komponente svojstvene vrijednosti
			(*vecStart)[3*pDoc->m_DES.GetDim() + 1] = 1;

			for( j=0; j<pDoc->m_DES.GetDim(); j++ )
			{
				val = ((double) rand()) / RAND_MAX - 0.5;
				(*vecStart)[pDoc->m_DES.GetDim()+j] = val;

				val = ((double) rand()) / RAND_MAX - 0.5;
				(*vecStart)[2*pDoc->m_DES.GetDim()+j] = val;
			}

			ret = pDoc->m_DES.LocateHopfBifurcationPoints(*vecStart, &omega);
			if( ret == 0 )
			{
				nNew += pDoc->PutInHopfBifArray(*vecStart,omega);
				numSucc++;
			}
		}
		else
		{
			vecStart = new Vector(2*pDoc->m_DES.GetDim()+pDoc->m_DES.GetParNum());
		
			for( j=0; j<pDoc->m_DES.GetDim(); j++ )
				(*vecStart)[j] = X[j];
			
			// ubacujemo vrijednost parametra
			for( j=0; j<pDoc->m_DES.GetParNum(); j++ )
				(*vecStart)[2 * pDoc->m_DES.GetDim() + j] = P[j];

			// i još treba popuniti poèetne vrijednosti svojstvenog vektora
			for( j=0; j<pDoc->m_DES.GetDim(); j++ )
			{
				val = ((double) rand()) / RAND_MAX * 2 - 1;
				(*vecStart)[pDoc->m_DES.GetDim()+j] = val;
			}

			ret = pDoc->m_DES.LocateBifurcationPoints(vecStart);
			if( ret == 0 )
			{
				nNew += pDoc->PutInSNBBifArray(*vecStart);
				numSucc++;
			}
		}
	}
	delete pBCP;

	char		str[200];
	sprintf(str, "Finished with calculation of bifurcation points : \n%d succesful searches, \n%d new bifurcation points found", numSucc, nNew );
	AfxMessageBox(str);

	return 0;
}

// broj pokušaja za odreðivanje poèetne toèke ravnotežne krivulje
#define MAX_TRY		10
#define MIN_STEP_FORWARD	1e-6

UINT	CalcSpectrum( LPVOID lParam )
{
	CNLSysDoc		*pDoc;
	CNLSysView	*pView = (CNLSysView *) lParam, *pViewReal;

	pDoc = pView->GetDocument();

	int			i, nFreqNum, nMaxAmplitude, nVar, start_i, end_i, nPoints, nRet=0;
	double	ofac, hifac, dProb;
	double	*x, *y;

	pView->m_bCalcDone = FALSE;

	hifac = 1.0;
	ofac = 4.0;
	nFreqNum = (int) (ofac * hifac / 2 * pView->m_PlotPar.m_nRealCalcNum);

	if( pView->m_matSpectrum != NULL )
	{
		if( RowNum(*pView->m_matSpectrum) < nFreqNum )
		{
			delete pView->m_matSpectrum;
			delete pView->m_vecFreq;
			pView->m_matSpectrum = new Matrix(MAX_PLOTTING_VARS, nFreqNum+1);
			pView->m_vecFreq = new Vector(nFreqNum+1);
		}
	}
	else
	{
		pView->m_matSpectrum = new Matrix(MAX_PLOTTING_VARS, nFreqNum+1);
		pView->m_vecFreq = new Vector(nFreqNum+1);
	}

	start_i = 0;
	if( pView->m_PlotPar.m_dStartPlot > pView->m_IntPar.m_dStart )
		for( i=0; i<pView->m_PlotPar.m_nRealCalcNum; i++ )
			if( (*pView->m_vecX)[i] > pView->m_PlotPar.m_dStartPlot )
			{	
				start_i = i;
				break;
			}
	
	end_i = pView->m_PlotPar.m_nRealCalcNum-1;
	if( pView->m_PlotPar.m_dEndPlot < pView->m_IntPar.m_dEnd )
		for( i=start_i; i<pView->m_PlotPar.m_nRealCalcNum; i++ )
			if( (*pView->m_vecX)[i] > pView->m_PlotPar.m_dEndPlot )
			{
				end_i = i-1;
				break;
			}

	nPoints = end_i - start_i;
	x = &((*pView->m_vecX)[start_i]);
	for( i=0;i<MAX_PLOTTING_VARS; i++ )
	{
		if( pView->m_PlotPar.m_anPlottingVars[i] != -1 )
		{
			pView->m_bCalcInProgress = TRUE;
	
			nVar = pView->m_PlotPar.m_anSysVarsOrder[i];
			y = &( (*pView->m_matY)[nVar][start_i] );

			// najprije treba prebaciti podatke koje smo dobili integriranjem u oblik koji se može
			// predati funkciji za izraèunavanje spektra
			period( x, y, nPoints, ofac, hifac, 
							pView->m_vecFreq->GetPtr(), (*pView->m_matSpectrum)[nVar], nFreqNum, &pView->m_PlotPar.m_nRealCalcNum,
							&nMaxAmplitude, &dProb );

			(*pView->m_vecFreq)[0] = 0.0;
		}
	}
	pView->m_bCalcDone = TRUE;
	pView->m_bCalcInProgress = FALSE;

	pView->GetInterval();
	pView->SystemChanged(FALSE);

	// sada još treba poslati poruku prozoru da mu sadržaj ne valja, 
	// odnosno da ga treba ponovo iscrtati
	pViewReal = (CNLSysView *) CNLSysView::FromHandle( pView->m_IntPar.m_hWnd );
	pViewReal->Invalidate();

	return nRet;
}

UINT	CalcEqvSolution( LPVOID lParam )
{
	EqvSolCalcParameters		*pECP = (EqvSolCalcParameters *) lParam;

	double	x1, x2, dStepForward, dPrevPar, dPrevY, dStepForwardYVar, dGrad, dDir;
	int			i, nDim, nParNum, nTryNum, nRet, num, nSuccNum, dummy;

	CNLSysDoc		*pDoc = pECP->pDoc;
	CNLSysView	*pViewReal;

	x1 = pECP->x1;
	x2 = pECP->x2;

	nDim = pDoc->m_DES.GetDim();
	nParNum = pDoc->m_DES.GetParNum();

	Vector		X(nDim), Par(nParNum);

	pECP->pView->m_bCalcInProgress = TRUE;
	pECP->pView->m_bCalcDone = FALSE;

	do
	{
		// najprije pokušavamo naæi ravnotežno rješenje za poèetnu vrijednost parametra
		// koju je zadao korisnik
		nTryNum = 0;
		do
		{
			nTryNum++;
			
			// za traženje prve toèke, poèetne vrijednosti æemo odrediti sluèajno
			for( i=0; i<nDim; i++ )
				X[i] = (( double) rand()) / RAND_MAX *2 - 1;
			for( i=0; i<nParNum; i++ )
				Par[i] = (( double) rand()) / RAND_MAX *2 - 1;
			
			// osim što zadajemo vrijednost parametra u ovisnosti o kojem tražimo ravnotežna rješenja
			Par[pECP->nParVarNum] = x1;

			nRet = pDoc->m_DES.CalcEqvSolution(&X, Par);
		} while( nRet != 0 && nTryNum < MAX_TRY );
		// ukoliko nismo našli za korosnièku vrijednost paramtera, 
		//	onda æemo je mi malo poveæati pa pokušati ponovo
		if( nRet != 0 )
			x1 += (pECP->x2 - pECP->x1) / 10;
	} while( nRet != 0 && x1 < pECP->x2 );

	if( nRet != 0 )
	{
		AfxMessageBox(IDS_NO_EQV_SOL_IN_THIS_INTERVAL);
		return 1;
	}
	else
	{
		// sve ravnotežne toèke koje ovdje izraèunamo æemo ubaciti i u niz ravnotežnih toèaka
		// u document klasi
		pDoc->PutInEquSolArray(X,Par,&dummy);

		(*pECP->pView->m_matEquSol)[0][0] = Par[pECP->nParVarNum];
		(*pECP->pView->m_matEquSol)[1][0] = X[pECP->nSysVarNum];
	}
	
	if( pECP->nType == EqvSolCalcParameters::EQU_SURFACE_PLOT_2D )
	{

		num = 1;
		
		dStepForward = (pECP->x2 - pECP->x1 ) / 10;
		dStepForwardYVar = 0;
		dPrevPar = Par[pECP->nParVarNum];				// da zapamtimo prethodnu vrijednost
		dPrevY = X[pECP->nSysVarNum];
		
		nSuccNum = 1;
		do
		{
			do
			{
				Par[pECP->nParVarNum] = dPrevPar + dStepForward;						// tražimo rješenje za slijedeæu vrijednost
				X[pECP->nSysVarNum] = dPrevY + dStepForwardYVar;

				nRet = pDoc->m_DES.CalcEqvSolution(&X, Par);
				
				if( nRet != 0 )						// ako nismo konvergirali do rješenja
				{
					dStepForward /= 2;			// smanjiti æemo udaljenost na kojoj tražimo rješenje
					dStepForwardYVar /= 2;	// smanjiti æemo udaljenost na kojoj tražimo rješenje
					
					nSuccNum = 0;
				}
				else
				{
					pDoc->PutInEquSolArray(X,Par,&dummy);

					dStepForwardYVar = X[pECP->nSysVarNum] - dPrevY;
					nSuccNum++;
					
					if( nSuccNum == 3 )		// ako smo imali tri uspješna odreðivanja zaredom
					{
						dStepForward *= 2;			// onda æemo poveæati korak
						dStepForwardYVar *= 2;
						
						nSuccNum = 0;
					}
				}
			} while( nRet != 0 && dStepForward > MIN_STEP_FORWARD );
			
			if( nRet == 0 )
			{
				(*pECP->pView->m_matEquSol)[0][num] = Par[pECP->nParVarNum];
				(*pECP->pView->m_matEquSol)[1][num++] = X[pECP->nSysVarNum];

				// ovo nam mjeri kamo je usmjerena krivulja ravnotežnih toèaka
				// ukoliko vrijednost postane velika, znaèi da ide ravno prema gore, ili prema dolje,
				// što znaèi da se možda treba poveæavati parametar u suprotnom smjeru
				dGrad = (X[pECP->nSysVarNum] - dPrevY) / (Par[pECP->nParVarNum] - dPrevPar);

				dPrevPar = Par[pECP->nParVarNum];
				dPrevY = X[pECP->nSysVarNum];

//				dStepForward *= 2;
			}
			else
			{
				// nismo uspjeli naæi ravnotežno rješenje za tu vrijednost parametra
				if( fabs(dGrad) > 100 )
				{
					// onda æemo sada tražiti s poveæanjem parametra u suprotnom smjeru
					dStepForward *= -10;
					// treba i odrediti u kojem smjeru æemo krenuti sa poveæavanje Y varijable
					// to æemo odrediti tako da promotrimo razliku izmeðu zadnje uspješno odreðene r. toèke i one od 10-tog puta prije
					// i uzimamo taj smjer
					dDir = (*pECP->pView->m_matEquSol)[1][num-1] - (*pECP->pView->m_matEquSol)[1][num-10];
					if( dDir > 0 )
						dStepForwardYVar = fabs(dStepForwardYVar) * 10;
					else
						dStepForwardYVar = -fabs(dStepForwardYVar) * 10;
				}
			}
		} while( num<200 && (nRet==0 || fabs(dGrad)>100) && (Par[pECP->nParVarNum] > pECP->x1 && Par[pECP->nParVarNum] < pECP->x2) );

	}

	pECP->pView->m_EquAnalPar.m_nEquSurfaceDataNum = num;
	pECP->pView->m_EquAnalPar.m_bEquSurfaceDataAvailable = TRUE;

	pECP->pView->m_bCalcDone = TRUE;
	pECP->pView->m_bCalcInProgress = FALSE;

	pECP->pView->GetInterval();
	pECP->pView->SystemChanged(FALSE);

	// sada još treba poslati poruku prozoru da mu sadržaj ne valja, 
	// odnosno da ga treba ponovo iscrtati
	pViewReal = (CNLSysView *) CNLSysView::FromHandle( pECP->pView->m_IntPar.m_hWnd );
	pViewReal->Invalidate();

	delete pECP;

	return 0;
}

BOOL CNLSysDoc::IsDefined()
{
	return	m_bSysDefined;
}

int CNLSysDoc::GetVarNum(CString str)
{
	char		szName[MAX_VAR_NAME_LEN+1];

	strcpy( szName, LPCTSTR(str) );
	
	return m_DES.GetVarNum(szName);
}


void CNLSysDoc::OnSetProgramOptions() 
{
	CNLSysOptionsDlg		dlg;

	if( m_Options.m_nWindOpen == ProgramOptions::NEW_WIND )
	{
		dlg.m_nNewWindow = 0;
		dlg.m_nSameWindow = -1;
	}
	else
	{
		dlg.m_nNewWindow = -1;
		dlg.m_nSameWindow = 0;
	}
	dlg.m_dEps = m_Options.m_dEps;
	dlg.m_dStepMin = m_Options.m_dStepMin;
	dlg.m_bUseColor = m_Options.m_bUseColor;

	if( dlg.DoModal() == IDOK )
	{
		if( dlg.m_nNewWindow == ProgramOptions::NEW_WIND )
			m_Options.m_nWindOpen = ProgramOptions::NEW_WIND;
		else
			m_Options.m_nWindOpen = ProgramOptions::SAME_WIND;

		m_Options.m_dEps = dlg.m_dEps;
		m_Options.m_dStepMin = dlg.m_dStepMin;
		
		if( m_Options.m_bUseColor != dlg.m_bUseColor )
		{
			m_Options.m_bUseColor = dlg.m_bUseColor;
			UpdateAllViews(NULL);
		}
	}
}

