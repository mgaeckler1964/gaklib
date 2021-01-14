object ChangePasswordForm: TChangePasswordForm
  Left = 379
  Top = 375
  BorderStyle = bsDialog
  Caption = 'Change Password'
  ClientHeight = 123
  ClientWidth = 490
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 8
    Top = 32
    Width = 71
    Height = 13
    Caption = 'New Password'
  end
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 65
    Height = 13
    Caption = 'Old Password'
  end
  object Label3: TLabel
    Left = 8
    Top = 56
    Width = 41
    Height = 13
    Caption = 'Re-Type'
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 88
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object ButtonOK: TButton
    Left = 8
    Top = 88
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 3
    OnClick = ButtonOKClick
  end
  object EditNewPassword: TEdit
    Left = 80
    Top = 32
    Width = 400
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
  end
  object EditOldPassword: TEdit
    Left = 80
    Top = 8
    Width = 400
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
  end
  object EditNewPassword2: TEdit
    Left = 80
    Top = 56
    Width = 400
    Height = 21
    PasswordChar = '*'
    TabOrder = 2
  end
end
