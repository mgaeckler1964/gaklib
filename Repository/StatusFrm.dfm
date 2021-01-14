object StatusForm: TStatusForm
  Left = 382
  Top = 338
  Width = 543
  Height = 106
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Status Form'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = CheckThread
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StatusLabel: TLabel
    Left = 0
    Top = 0
    Width = 535
    Height = 38
    Align = alClient
    AutoSize = False
    WordWrap = True
  end
  object Panel1: TPanel
    Left = 0
    Top = 38
    Width = 535
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object IdleLabel: TLabel
      Left = 104
      Top = 16
      Width = 3
      Height = 13
    end
    object BitBtnStop: TBitBtn
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Stop'
      TabOrder = 0
      OnClick = BitBtnStopClick
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000130B0000130B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333FFFFF3333333333999993333333333F77777FFF333333999999999
        3333333777333777FF3333993333339993333377FF3333377FF3399993333339
        993337777FF3333377F3393999333333993337F777FF333337FF993399933333
        399377F3777FF333377F993339993333399377F33777FF33377F993333999333
        399377F333777FF3377F993333399933399377F3333777FF377F993333339993
        399377FF3333777FF7733993333339993933373FF3333777F7F3399933333399
        99333773FF3333777733339993333339933333773FFFFFF77333333999999999
        3333333777333777333333333999993333333333377777333333}
      NumGlyphs = 2
    end
  end
end
