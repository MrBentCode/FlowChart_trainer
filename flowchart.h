#ifndef FLOWCHART_H
#define FLOWCHART_H

#include <string>


using namespace std;

class Block
{
public:
    string text;
    virtual void init() = 0;
};

class BlockBegin : public Block
{
public:
    string text;
    Block *next;
    virtual void init();
};

class BlockEnd : public Block
{
public:
    string text;
    Block *prev;
    virtual void init();
};

class BlockInput : public Block
{
public:
    string text;
    Block *prev;
    Block *next;
    virtual void init();
};

class BlockIf: public Block
{
public:
    string text;
    Block *prev;
    Block *yes;
    Block *no;
    virtual void init();
};

class FlowChart
{
public:
    BlockBegin *Begin;
    BlockEnd *End;
    FlowChart();
};

#endif // FLOWCHART_H
