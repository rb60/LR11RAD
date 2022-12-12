//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include "Unit1.h"
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Unit4.h"
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TMemo *Memo1;
	TComboBox *ComboBox1;
	TComboBox *ComboBox2;
	TComboBox *ComboBox3;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
private:	// User declarations
    TForm1* owner;
public:		// User declarations
	void update();
	ImgGraph* g;
	ImgNode** cur;
	__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
