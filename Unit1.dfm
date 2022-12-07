object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  TextHeight = 15
  object Image1: TImage
    Left = 0
    Top = 65
    Width = 624
    Height = 376
    Align = alClient
    OnMouseDown = Image1MouseDown
    OnMouseMove = Image1MouseMove
    OnMouseUp = Image1MouseUp
    ExplicitLeft = 32
    ExplicitTop = 136
    ExplicitWidth = 105
    ExplicitHeight = 105
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 624
    Height = 65
    Align = alTop
    TabOrder = 0
    object sbPencil: TSpeedButton
      Left = 8
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'S'
    end
    object SpeedButton2: TSpeedButton
      Left = 47
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
    end
    object SpeedButton3: TSpeedButton
      Left = 86
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
    end
    object SpeedButton4: TSpeedButton
      Left = 125
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
    end
    object SpeedButton5: TSpeedButton
      Left = 164
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
    end
    object SpeedButton6: TSpeedButton
      Left = 203
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
    end
    object SpeedButton7: TSpeedButton
      Left = 242
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
    end
  end
  object MainMenu1: TMainMenu
    Left = 416
    Top = 144
    object Log1: TMenuItem
      Caption = 'Log'
      object Save1: TMenuItem
        Caption = 'Save'
        OnClick = Save1Click
      end
      object Save2: TMenuItem
        Caption = 'Open'
        OnClick = Save2Click
      end
      object View1: TMenuItem
        Caption = 'View'
        OnClick = View1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 200
    Top = 136
  end
  object SaveDialog1: TSaveDialog
    Left = 96
    Top = 168
  end
end
