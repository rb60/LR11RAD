//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TForm3::update()
{
	ComboBox2->Items->Clear();
	ComboBox3->Items->Clear();
	for (int i = 0; i < g->nodes.size(); i++)
	{
		ComboBox2->Items->Add(IntToStr(g->nodes[i]->data.number));
		ComboBox3->Items->Add(IntToStr(g->nodes[i]->data.number));
	}
	ComboBox2->ItemIndex = 0;
    ComboBox3->ItemIndex = 0;
    Memo1->Lines = GraphString(g, *cur);
}
void __fastcall TForm3::Button1Click(TObject *Sender)
{
    update();
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button2Click(TObject *Sender)
{
	switch (ComboBox1->ItemIndex)
	{
	case 0:
		if (g->nodes[ComboBox2->ItemIndex] != g->nodes[ComboBox3->ItemIndex])
		{
			merge(g,g->nodes[ComboBox2->ItemIndex],g->nodes[ComboBox3->ItemIndex]);
			*cur = g->nodes[ComboBox3->ItemIndex];
		}

	break;

    default:
        ;
	}

}
//---------------------------------------------------------------------------


