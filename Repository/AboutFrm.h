/*
		Project:		GAKLIB
		Module:			AboutFrm.h
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

//----------------------------------------------------------------------------
#ifndef AboutFrmH
#define AboutFrmH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>

#include <gak/string.h>
#include <winlib/version.h>

//----------------------------------------------------------------------------
class TAboutProgramForm : public TForm
{
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *ProductName;
	TLabel *Version;
	TLabel *Copyright;
	TLabel *Comments;
	TButton *OKButton;
	TImage *Image1;
	void __fastcall FormCreate(TObject *Sender);
private:
	winlib::ProgramVersionInfo	theVersionInfo;

public:
	virtual __fastcall TAboutProgramForm(TComponent* AOwner);
	const gak::STRING &getVersionString( void ) const
	{
		return theVersionInfo.getVersionString();
	}
	const gak::STRING &getLegalCopyRight( void ) const
	{
		return theVersionInfo.getLegalCopyRight();
	}
	const gak::STRING &getFileDescription( void ) const
	{
		return theVersionInfo.getFileDescription();
	}
	const gak::STRING &getProductName( void ) const
	{
		return theVersionInfo.getProductName();
	}
	const gak::STRING &getCompanyName( void ) const
	{
		return theVersionInfo.getCompanyName();
	}
};
//----------------------------------------------------------------------------
extern PACKAGE TAboutProgramForm *AboutProgramForm;
//----------------------------------------------------------------------------

#endif	// AboutFrmH
