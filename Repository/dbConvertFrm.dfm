object DbConvertForm: TDbConvertForm
  Left = 203
  Top = 192
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Database Administration'
  ClientHeight = 265
  ClientWidth = 682
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
  object UpgradeButton: TButton
    Left = 168
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Upgrade'
    TabOrder = 3
    OnClick = UpgradeButtonClick
  end
  object Memo: TMemo
    Left = 8
    Top = 8
    Width = 665
    Height = 217
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object EmptyButton: TButton
    Left = 248
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Empty'
    TabOrder = 4
    OnClick = EmptyButtonClick
  end
  object SessionButton: TButton
    Left = 600
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Unlock'
    TabOrder = 7
    OnClick = SessionButtonClick
  end
  object BackupButton: TButton
    Left = 336
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Backup'
    TabOrder = 5
    OnClick = BackupButtonClick
  end
  object RestoreButton: TButton
    Left = 416
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Restore'
    TabOrder = 6
    OnClick = RestoreButtonClick
  end
  object ButtonCheck: TButton
    Left = 88
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Check'
    TabOrder = 2
    OnClick = ButtonCheckClick
  end
  object ButtonCreate: TButton
    Left = 8
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Create'
    TabOrder = 1
    OnClick = ButtonCreateClick
  end
end
