object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 441
  ClientWidth = 912
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  TextHeight = 15
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 912
    Height = 81
    Align = alTop
    TabOrder = 0
    ExplicitLeft = -8
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
      Top = 8
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
      Top = 8
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
    object sbErase: TSpeedButton
      Left = 242
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'ERS'
    end
    object Shape1: TShape
      Left = 612
      Top = 7
      Width = 40
      Height = 50
      Brush.Color = clBlack
      OnMouseDown = Shape1MouseDown
    end
    object Shape2: TShape
      Left = 658
      Top = 7
      Width = 40
      Height = 51
      OnMouseDown = Shape2MouseDown
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
    object SpeedButton2: TSpeedButton
      Left = 242
      Top = 47
      Width = 33
      Height = 22
      Caption = 'CLS'
      OnClick = SpeedButton2Click
    end
    object SpeedButton3: TSpeedButton
      Left = 281
      Top = 8
      Width = 48
      Height = 33
      Caption = 'R'
      OnClick = SpeedButton3Click
    end
    object sbParametric: TSpeedButton
      Left = 335
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'PAR'
    end
    object SpeedButton5: TSpeedButton
      Left = 335
      Top = 47
      Width = 33
      Height = 22
      Caption = 'IMPR'
      OnClick = SpeedButton5Click
    end
    object sbCopy: TSpeedButton
      Left = 374
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'COPY'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object sbPaste: TSpeedButton
      Left = 413
      Top = 8
      Width = 33
      Height = 33
      GroupIndex = 1
      Caption = 'PASTE'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object sbFlood: TSpeedButton
      Left = 452
      Top = 8
      Width = 69
      Height = 33
      GroupIndex = 1
      Caption = 'Fllood'
    end
    object ColorBox1: TColorBox
      Left = 704
      Top = 8
      Width = 105
      Height = 22
      TabOrder = 0
      OnChange = ColorBox1Change
    end
    object ColorBox2: TColorBox
      Left = 704
      Top = 36
      Width = 105
      Height = 22
      Selected = clWhite
      TabOrder = 1
      OnChange = ColorBox2Change
    end
    object NumberBox1: TNumberBox
      Left = 557
      Top = 8
      Width = 49
      Height = 23
      MinValue = 1.000000000000000000
      MaxValue = 5.000000000000000000
      TabOrder = 2
      Value = 1.000000000000000000
      SpinButtonOptions.Placement = nbspCompact
      OnChangeValue = NumberBox1ChangeValue
    end
    object ComboBox1: TComboBox
      Left = 815
      Top = 8
      Width = 82
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
      Left = 815
      Top = 36
      Width = 82
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
    object NumberBox2: TNumberBox
      Left = 281
      Top = 47
      Width = 48
      Height = 23
      MinValue = 1.000000000000000000
      MaxValue = 359.000000000000000000
      TabOrder = 5
      SpinButtonOptions.Placement = nbspCompact
    end
    object ComboBox3: TComboBox
      Left = 452
      Top = 47
      Width = 69
      Height = 23
      ItemIndex = 0
      TabOrder = 6
      Text = 'Surface'
      Items.Strings = (
        'Surface'
        'Border'
        '')
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
  object ScrollBox1: TScrollBox
    Left = 0
    Top = 81
    Width = 912
    Height = 360
    Align = alClient
    TabOrder = 2
    object Image1: TImage
      Left = -2
      Top = -7
      Width = 912
      Height = 360
      OnMouseDown = Image1MouseDown
      OnMouseMove = Image1MouseMove
      OnMouseUp = Image1MouseUp
    end
  end
  object MainMenu1: TMainMenu
    Left = 288
    Top = 224
    object ree1: TMenuItem
      Caption = 'Log'
      object View1: TMenuItem
        Caption = 'View'
        OnClick = View1Click
      end
      object View2: TMenuItem
        Caption = 'Save'
        OnClick = View2Click
      end
      object Open1: TMenuItem
        Caption = 'Open'
        OnClick = Open1Click
      end
      object Open2: TMenuItem
        Caption = 'Undo'
        ShortCut = 16474
        OnClick = Open2Click
      end
      object Redo2: TMenuItem
        Caption = 'Redo'
        ShortCut = 16473
        OnClick = Redo2Click
      end
    end
    object Import1: TMenuItem
      Caption = 'Import'
      object oCanvas1: TMenuItem
        Caption = 'To Canvas'
        OnClick = oCanvas1Click
      end
      object oCanvas2: TMenuItem
        Caption = 'To Copy Buffer'
        OnClick = oCanvas2Click
      end
    end
    object Export1: TMenuItem
      Caption = 'Export'
      object Export2: TMenuItem
        Caption = 'As Parametric'
        OnClick = Export2Click
      end
      object AsBMPImage1: TMenuItem
        Caption = 'As BMP Image'
        OnClick = AsBMPImage1Click
      end
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
    Top = 224
  end
end
