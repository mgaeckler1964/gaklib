inherited AboutOpenSSLForm: TAboutOpenSSLForm
  Caption = 'AboutOpenSSLForm'
  ClientHeight = 246
  PixelsPerInch = 96
  TextHeight = 13
  inherited Panel1: TPanel
    Height = 202
    object Label1: TLabel
      Left = 8
      Top = 160
      Width = 401
      Height = 33
      AutoSize = False
      Caption = 
        'This product includes software developed by the OpenSSL Project ' +
        'for use in the OpenSSL Toolkit. (http://www.openssl.org/)'
      WordWrap = True
    end
  end
  inherited OKButton: TButton
    Top = 213
  end
end
