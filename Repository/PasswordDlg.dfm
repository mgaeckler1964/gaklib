object PasswordForm: TPasswordForm
  Left = 378
  Top = 241
  BorderStyle = bsDialog
  Caption = 'Password'
  ClientHeight = 90
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
  object Label2: TLabel
    Left = 8
    Top = 16
    Width = 46
    Height = 13
    Caption = 'Password'
  end
  object EditPassword: TEdit
    Left = 80
    Top = 16
    Width = 400
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
  end
  object ButtonOK: TButton
    Left = 8
    Top = 56
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 56
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
