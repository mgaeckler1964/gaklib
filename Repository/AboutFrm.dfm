object AboutProgramForm: TAboutProgramForm
  Left = 505
  Top = 284
  BorderStyle = bsDialog
  Caption = 'Product'
  ClientHeight = 197
  ClientWidth = 439
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 420
    Height = 153
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 0
    object ProgramIcon: TImage
      Left = 8
      Top = 8
      Width = 33
      Height = 33
      Stretch = True
      IsControl = True
    end
    object ProductName: TLabel
      Left = 56
      Top = 16
      Width = 37
      Height = 13
      Caption = 'Product'
      IsControl = True
    end
    object Version: TLabel
      Left = 56
      Top = 40
      Width = 35
      Height = 13
      Caption = 'Version'
      IsControl = True
    end
    object Copyright: TLabel
      Left = 8
      Top = 64
      Width = 401
      Height = 33
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = 'Copyright'
      WordWrap = True
      IsControl = True
    end
    object Comments: TLabel
      Left = 8
      Top = 104
      Width = 401
      Height = 39
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = 'Comments'
      WordWrap = True
      IsControl = True
    end
    object Image1: TImage
      Left = 379
      Top = 8
      Width = 33
      Height = 33
      Anchors = [akTop, akRight]
      Picture.Data = {
        055449636F6E0000010001002020100000000000E80200001600000028000000
        2000000040000000010004000000000080020000000000000000000000000000
        0000000000000000000080000080000000808000800000008000800080800000
        80808000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000
        FFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFF9FFFFFFFFFFFFFFFCCCCCFFFFFFFFFFF9FFFFFFFFFFFFFCCFF
        FFFCCFFFFFFFFF9FFFFFFFFFFFFCFFFFFFFFFCFFFFFFFF9FFFFFFFFFFFCFFFFF
        FFFFFFCFFFFFFF9FFFFFFFFFFFCFFFFFFFFFFFCFFFFFFF9FFFFFFFFFFCFFFFFF
        FFFFFFFCFFFFFF9FFFFFFFFFFCFFFFFFFFFFFFFCFFFFFF9FFFFFFFFFFCFFFFFF
        FFFFFFFC9FFFFF9FFFFF9FFFFCFFFFFFFFFFFFFCF9FFFF9FFFF9FFFFFCFFFFFF
        FFFFFFFCFF9FFF9FFF9FFFFFFFCFFFFFFFFFFFCFFFF9FF9FF9FFFFFFFFCFFFFF
        FFFFFFCFFFFF9F9F9FFFFFFFFFFCFFFFFFFFFCFFFFFFF999FFFFFFFFFFFFCCFF
        FFFCCFFFFFFFFF9FFFFFFFFFFFFFFFCCCCCFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFCFFFFFFFFFFFFFFF9FFFFFFFFFFFFFFFCFFFFFFFFFFFFFF9
        99FFFFFFFFFFFFFFCFFFFFFFFFFFFF9F9F9FFFFFFFFFFFFFCFFFFFFFFFFFF9FF
        9FF9FFFFFFFFFFFFCFFFFFFFFFFF9FFF9FFF9FFFFFFFFFFFCFFFFFFFFFF9FFFF
        9FFFF9FFFCFFFFFFCFFFFFFFFF9FFFFF9FFFFF9FFFCFFFFFCFFFFFFFF9FFFFFF
        9FFFFFF9FFFCFFFFCFFFFFFFFFFFFFFF9FFFFFFFFFFFCFFFCFFFFFFFFFFFFFFF
        9FFFFFFFFFFFFCFFCFFFFFFFFFFFFFFF9FFFFFFFFFFFFFCFCFFFFFFFFFFFFFFF
        9FFFFFFFFFFFFFFCCFFFFFFFFFFFFFFF9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFF00000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000}
      Stretch = True
      IsControl = True
    end
  end
  object OKButton: TButton
    Left = 182
    Top = 164
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
end
