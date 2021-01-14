/*
		Project:		GAKLIB
		Module:			AboutFrm.cpp
		Description:	
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

//---------------------------------------------------------------------
#include <stdio.h>

#include <vcl.h>

#include <gak/fmtNumber.h>

#pragma hdrstop

#include "AboutFrm.h"
//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutProgramForm *AboutProgramForm;
//---------------------------------------------------------------------
ProgramVersionInfo::ProgramVersionInfo( const STRING &appFileName )
{
	DWORD	dummy1, versionInfoSize;
	UINT	dummy2;
	char	tmpBuffer[128];

	versionInfoSize = GetFileVersionInfoSize(
		appFileName, &dummy1
	);
	if( versionInfoSize )
	{
		void *data = malloc( versionInfoSize );
		if( data )
		{
			VS_FIXEDFILEINFO	*fixedFileInfo;

			GetFileVersionInfo( Application->ExeName.c_str(), 0L, versionInfoSize, data );
			if( VerQueryValue( data, "\\", (void **)&fixedFileInfo, &dummy2 ) )
			{
				major = (unsigned short)(fixedFileInfo->dwFileVersionMS >> 16);
				minor = (unsigned short)(fixedFileInfo->dwFileVersionMS & 0xFFFF);
				patch = (unsigned short)(fixedFileInfo->dwFileVersionLS >> 16);
				build = (unsigned short)(fixedFileInfo->dwFileVersionLS & 0xFFFF);
				versionString =
					formatNumber( major ) + '.' +
					formatNumber( minor ) + '.' +
					formatNumber( patch ) + '.' +
					formatNumber( build )
				;
			}

			struct LANGANDCODEPAGE
			{
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate;

			const char *cpData;


			if( VerQueryValue(
				data,
				"\\VarFileInfo\\Translation", (void**)&lpTranslate,
				&dummy2
			) )
			{
				sprintf(
					tmpBuffer,
					"\\StringFileInfo\\%04x%04x\\LegalCopyright",
					lpTranslate->wLanguage,
					lpTranslate->wCodePage
				);
				if( VerQueryValue(
					data,
					tmpBuffer, (void**)&cpData,
					&dummy2
				) )
				{
					legalCopyRight = cpData;
				}

				sprintf(
					tmpBuffer,
					"\\StringFileInfo\\%04x%04x\\FileDescription",
					lpTranslate->wLanguage,
					lpTranslate->wCodePage
				);
				if( VerQueryValue(
					data,
					tmpBuffer, (void**)&cpData,
					&dummy2
				) )
				{
					fileDescription = cpData;
				}
				sprintf(
					tmpBuffer,
					"\\StringFileInfo\\%04x%04x\\CompanyName",
					lpTranslate->wLanguage,
					lpTranslate->wCodePage
				);
				if( VerQueryValue(
					data,
					tmpBuffer, (void**)&cpData,
					&dummy2
				) )
				{
					companyName = cpData;
				}
				sprintf(
					tmpBuffer,
					"\\StringFileInfo\\%04x%04x\\ProductName",
					lpTranslate->wLanguage,
					lpTranslate->wCodePage
				);
				if( VerQueryValue(
					data,
					tmpBuffer, (void**)&cpData,
					&dummy2
				) )
				{
					productName = cpData;
				}
			}
			free( data );
		}
	}
}

//---------------------------------------------------------------------
__fastcall TAboutProgramForm::TAboutProgramForm(TComponent* AOwner)
	: TForm(AOwner), theVersionInfo( Application->ExeName.c_str() ) 
{
}
//---------------------------------------------------------------------



void __fastcall TAboutProgramForm::FormCreate(TObject *)
{
	Caption  = (const char *)(
		getProductName() + " by " + getCompanyName()
	);
	ProductName->Caption = Application->Title;
	ProgramIcon->Picture-> Icon = Application->Icon;

	Version->Caption = (const char *)getVersionString();
	Copyright->Caption = (const char *)getLegalCopyRight();
	Comments->Caption = (const char *)getFileDescription();

}
//---------------------------------------------------------------------------

