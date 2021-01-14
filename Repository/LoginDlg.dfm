object LoginForm: TLoginForm
  Left = 378
  Top = 241
  BorderStyle = bsDialog
  Caption = 'Login'
  ClientHeight = 99
  ClientWidth = 490
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 48
    Height = 13
    Caption = 'Username'
  end
  object Label2: TLabel
    Left = 8
    Top = 32
    Width = 46
    Height = 13
    Caption = 'Password'
  end
  object EditUserName: TEdit
    Left = 80
    Top = 8
    Width = 400
    Height = 21
    TabOrder = 0
  end
  object EditPassword: TEdit
    Left = 80
    Top = 32
    Width = 400
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
  end
  object ButtonOK: TButton
    Left = 8
    Top = 64
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 64
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
