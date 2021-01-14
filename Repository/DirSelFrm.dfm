object DirectorySelector: TDirectorySelector
  Left = 147
  Top = 160
  Width = 555
  Height = 310
  Caption = 'Directory Selector'
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
  object LabelDirectory: TLabel
    Left = 8
    Top = 229
    Width = 137
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'C:\CRESD\Source\DocMan'
  end
  object DirectoryListBox: TDirectoryListBox
    Left = 8
    Top = 32
    Width = 528
    Height = 193
    Anchors = [akLeft, akTop, akRight, akBottom]
    DirLabel = LabelDirectory
    ItemHeight = 16
    TabOrder = 1
  end
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 8
    Width = 528
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    DirList = DirectoryListBox
    TabOrder = 0
  end
  object ButtonOK: TButton
    Left = 8
    Top = 248
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = ButtonOKClick
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 248
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object EditMkdir: TEdit
    Left = 256
    Top = 248
    Width = 273
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 4
    Visible = False
  end
  object ButtonMkdir: TButton
    Left = 168
    Top = 248
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'New'
    TabOrder = 5
    OnClick = ButtonMkdirClick
  end
end
