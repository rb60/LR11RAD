//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.NumberBox.hpp>
#include <jpeg.hpp>
#include <pngimage.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TPanel *Panel1;
	TSpeedButton *sbPencil;
	TSpeedButton *sbLine;
	TSpeedButton *sbRect;
	TSpeedButton *sbText;
	TSpeedButton *sbElipse;
	TSpeedButton *sbPip;
	TSpeedButton *sbErase;
	TMainMenu *MainMenu1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TShape *Shape1;
	TShape *Shape2;
	TColorBox *ColorBox1;
	TColorBox *ColorBox2;
	TNumberBox *NumberBox1;
	TMenuItem *ree1;
	TComboBox *ComboBox1;
	TComboBox *ComboBox2;
	TEdit *Edit1;
	TFontDialog *FontDialog1;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TNumberBox *NumberBox2;
	TMenuItem *View1;
	TMenuItem *View2;
	TMenuItem *Open1;
	TMenuItem *Open2;
	TMenuItem *Redo2;
	TMenuItem *Export1;
	TMenuItem *Export2;
	TSpeedButton *sbParametric;
	TSpeedButton *SpeedButton5;
	TSpeedButton *sbCopy;
	TSpeedButton *sbPaste;
	TSpeedButton *sbFlood;
	TComboBox *ComboBox3;
	TMenuItem *Import1;
	TMenuItem *oCanvas1;
	TMenuItem *oCanvas2;
	TMenuItem *AsBMPImage1;
	TScrollBox *ScrollBox1;
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall View1Click(TObject *Sender);
	void __fastcall ColorBox1Change(TObject *Sender);
	void __fastcall ColorBox2Change(TObject *Sender);
	void __fastcall NumberBox1ChangeValue(TObject *Sender);
	void __fastcall Update1Click(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall ComboBox2Change(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Edit1Exit(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall Shape1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Shape2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall View2Click(TObject *Sender);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Open2Click(TObject *Sender);
	void __fastcall Redo2Click(TObject *Sender);
	void __fastcall Export2Click(TObject *Sender);
	void __fastcall SpeedButton5Click(TObject *Sender);
	void __fastcall oCanvas1Click(TObject *Sender);
	void __fastcall oCanvas2Click(TObject *Sender);
	void __fastcall AsBMPImage1Click(TObject *Sender);




private:	// User declarations
	void findMinMax(int& minx, int& maxx, int& miny, int& maxy, TStringList* imglog);
	TStringList* toParametric(TStringList* imgLog);
	void clearImg();
	void readLog(TStringList* imglog);
	void rotate(int angle);
    void resizeImg();
	__property TFont Font = {read = getFont, write = setFont};
	__property TBrushStyle BrushStyle = {read = getBrushStyle, write = setBrushStyle};
	__property TPenStyle PenStyle = {read = getPenStyle, write = setPenStyle};
	__property TColor PenColor = {read = getPenColor, write = setPenColor};
	__property TColor BrushColor = {read = getBrushColor, write = setBrushColor};
	__property int PenWidth = {read = getPenWidth, write = setPenWidth};
	TFont* getFont();
	TBrushStyle getBrushStyle();
	TPenStyle getPenStyle();
	int getPenWidth();
	TColor getPenColor();
	TColor getBrushColor();
	void setFont(TFont* value);
	void setBrushStyle(TBrushStyle value);
	void setPenStyle(TPenStyle value);
	void setPenWidth(int value);
	void setPenColor(TColor value);
	void setBrushColor(TColor value);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
    void update();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
