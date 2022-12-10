object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 441
  ClientWidth = 748
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
    Top = 81
    Width = 748
    Height = 360
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
    Width = 748
    Height = 81
    Align = alTop
    TabOrder = 0
    ExplicitLeft = -16
    ExplicitTop = -16
    object sbPencil: TSpeedButton
      Left = 8
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'S'
    end
    object sbLine: TSpeedButton
      Left = 47
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = '/'
    end
    object sbRect: TSpeedButton
      Left = 86
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = '[]'
    end
    object sbText: TSpeedButton
      Left = 164
      Top = 9
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'T'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
    end
    object sbElipse: TSpeedButton
      Left = 125
      Top = 9
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'O'
    end
    object sbPip: TSpeedButton
      Left = 203
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'P'
    end
    object SpeedButton7: TSpeedButton
      Left = 242
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
    end
    object Shape1: TShape
      Left = 340
      Top = 9
      Width = 40
      Height = 50
      Brush.Color = clBlack
    end
    object Shape2: TShape
      Left = 386
      Top = 9
      Width = 40
      Height = 51
    end
    object SpeedButton1: TSpeedButton
      Left = 164
      Top = 48
      Width = 33
      Height = 22
      Caption = 'FONT'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object ColorBox1: TColorBox
      Left = 432
      Top = 9
      Width = 105
      Height = 22
      TabOrder = 0
      OnChange = ColorBox1Change
    end
    object ColorBox2: TColorBox
      Left = 432
      Top = 37
      Width = 105
      Height = 22
      Selected = clWhite
      TabOrder = 1
      OnChange = ColorBox2Change
    end
    object NumberBox1: TNumberBox
      Left = 281
      Top = 9
      Width = 49
      Height = 22
      MinValue = 1.000000000000000000
      MaxValue = 5.000000000000000000
      TabOrder = 2
      Value = 1.000000000000000000
      SpinButtonOptions.Placement = nbspCompact
      OnChangeValue = NumberBox1ChangeValue
    end
    object ComboBox1: TComboBox
      Left = 543
      Top = 9
      Width = 145
      Height = 23
      ItemIndex = 0
      TabOrder = 3
      Text = 'Solid'
      OnChange = ComboBox1Change
      Items.Strings = (
        'Solid'
        'Dash'
        'Dot'
        'DashDot'
        'DashDotDot'
        'Clear')
    end
    object ComboBox2: TComboBox
      Left = 543
      Top = 38
      Width = 145
      Height = 23
      ItemIndex = 0
      TabOrder = 4
      Text = 'Solid'
      OnChange = ComboBox2Change
      Items.Strings = (
        'Solid'
        'Clear'
        'Horizontal'
        'Vertical'
        'FDiagonal'
        'BDiagonal'
        'Cross'
        'DiagCross')
    end
  end
  object Edit1: TEdit
    Left = 362
    Top = 232
    Width = 121
    Height = 23
    Enabled = False
    TabOrder = 1
    Visible = False
    OnExit = Edit1Exit
    OnKeyPress = Edit1KeyPress
  end
  object MainMenu1: TMainMenu
    Left = 288
    Top = 224
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
    object UndoRedo: TMenuItem
      Caption = 'UndoRedo'
      object Undo1: TMenuItem
        Caption = 'Undo'
        ShortCut = 16474
        OnClick = Undo1Click
      end
      object Redo1: TMenuItem
        Caption = 'Redo'
        ShortCut = 16473
        OnClick = Redo1Click
      end
    end
    object ree1: TMenuItem
      Caption = 'Tree'
      OnClick = ree1Click
    end
    object Update1: TMenuItem
      Caption = 'Update'
      OnClick = Update1Click
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 200
    Top = 224
  end
  object SaveDialog1: TSaveDialog
    Left = 104
    Top = 224
  end
  object FontDialog1: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    Left = 496
    Top = 232
  end
end
