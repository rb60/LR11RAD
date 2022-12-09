object Form3: TForm3
  Left = 0
  Top = 0
  Caption = 'Form3'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 624
    Height = 97
    Align = alTop
    TabOrder = 0
    object ComboBox1: TComboBox
      Left = 16
      Top = 16
      Width = 145
      Height = 23
      ItemIndex = 0
      TabOrder = 0
      Text = 'Merge'
      Items.Strings = (
        'Merge')
    end
    object ComboBox2: TComboBox
      Left = 233
      Top = 16
      Width = 145
      Height = 23
      TabOrder = 1
    end
    object Button1: TButton
      Left = 16
      Top = 56
      Width = 75
      Height = 25
      Caption = 'Update'
      TabOrder = 2
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 104
      Top = 56
      Width = 75
      Height = 25
      Caption = 'Execute'
      TabOrder = 3
      OnClick = Button2Click
    end
  end
  object Memo1: TMemo
    Left = 0
    Top = 97
    Width = 624
    Height = 344
    Align = alClient
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object ComboBox3: TComboBox
    Left = 384
    Top = 16
    Width = 145
    Height = 23
    TabOrder = 2
  end
end
