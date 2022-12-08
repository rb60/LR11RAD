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
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TPanel *Panel1;
	TSpeedButton *sbPencil;
	TSpeedButton *sbLine;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton6;
	TSpeedButton *SpeedButton7;
	TMainMenu *MainMenu1;
	TMenuItem *Log1;
	TMenuItem *Save1;
	TMenuItem *Save2;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TMenuItem *View1;
	TShape *Shape1;
	TShape *Shape2;
	TColorBox *ColorBox1;
	TColorBox *ColorBox2;
	TNumberBox *NumberBox1;
	TMenuItem *UndoRedo;
	TMenuItem *ree1;
	TMenuItem *Undo1;
	TMenuItem *Redo1;
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Save1Click(TObject *Sender);
	void __fastcall Save2Click(TObject *Sender);
	void __fastcall View1Click(TObject *Sender);
	void __fastcall ColorBox1Change(TObject *Sender);
	void __fastcall ColorBox2Change(TObject *Sender);
	void __fastcall NumberBox1ChangeValue(TObject *Sender);
	void __fastcall ree1Click(TObject *Sender);
	void __fastcall Undo1Click(TObject *Sender);
	void __fastcall Redo1Click(TObject *Sender);

private:	// User declarations
    void clearImg();
	void readLog(TStringList* imglog);
	__property TColor PenColor = {read = getPenColor, write = setPenColor};
	__property TColor BrushColor = {read = getBrushColor, write = setBrushColor};
	__property int PenWidth = {read = getPenWidth, write = setPenWidth};
	int getPenWidth();
	TColor getPenColor();
	TColor getBrushColor();
	void setPenWidth(int value);
	void setPenColor(TColor value);
	void setBrushColor(TColor value);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
