#include "flowchart.h"
#include <QDebug>

FlowChart::FlowChart()
{
    Begin = new BlockBegin();
    End = new BlockEnd();
    Begin->next = End;
    End->prev = Begin;
    Begin->text = "Начало";
    End->text = "Конец";
}


void BlockBegin::init()
{

}


void BlockEnd::init()
{

}


void BlockInput::init()
{

}


void BlockIf::init()
{

}
