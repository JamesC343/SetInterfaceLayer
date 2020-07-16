// SetInterfaceLayer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

struct Table
{
    std::vector<std::vector<int>*> columns;
};

struct SelectList
{
    int table;
    int column;
};

struct JoinList
{
    int tableAColumn;
    int tableBColumn;
};

void Print(std::vector<int> column)
{
    for (int i = 0; i < column.size(); i++)
    {
        std::cout << column.at(i) << std::endl;
    }
}

void Print(Table table)
{
    int columnCount = table.columns.size();
    if (columnCount > 0)
    {
        int rowCount = table.columns.at(0)->size();
        for (int i = 0; i < rowCount; i++)
        {
            for (int j = 0; j < columnCount; j++)
            {
                int value = table.columns.at(j)->at(i);
                std::cout << value << "\t";
            }
            std::cout << std::endl;
        }

    }
}

//Join limited to 1 entry for now
Table InnerJoin(Table tableA, Table tableB, std::vector<SelectList> select, std::vector<JoinList> join)
{
    Table newTable;
    for (int i = 0; i < select.size(); i++)
    {
        newTable.columns.push_back(new std::vector<int>);
    }

    //Both tables have columns
    if (tableA.columns.size() > 0 && tableB.columns.size() > 0)
    {
        //Both tables have rows
        int tableARowCount = tableA.columns.at(0)->size();
        int tableBRowCount = tableB.columns.at(0)->size();
        if (tableARowCount > 0 && tableBRowCount > 0)
        {
            for (int i = 0; i < tableARowCount; i++)
            {
                for (int j = 0; j < tableBRowCount; j++)
                {
                    int tableAValue = tableA.columns.at(join.at(0).tableAColumn)->at(i);
                    int tableBValue = tableB.columns.at(join.at(0).tableBColumn)->at(j);
                    if (tableAValue == tableBValue)
                    {
                        for (int k = 0; k < select.size(); k++)
                        {
                            int value = 0;
                            if (select.at(k).table == 0)
                                value = tableA.columns.at(select.at(k).column)->at(i);
                            else if (select.at(k).table == 1)
                                value = tableB.columns.at(select.at(k).column)->at(j);
                            newTable.columns.at(k)->push_back(value);
                        }
                    }
                }

            }
        }
    }

    return newTable;
}

int main()
{
    std::vector<int> tableAcolumnA({ 1,2 });
    std::vector<int> tableAcolumnB({ 3,4 });
    Table tableA({ { &tableAcolumnA ,&tableAcolumnB } });

    std::vector<int> tableBcolumnA({ 2,1 });
    std::vector<int> tableBcolumnB({ 5,6 });
    Table tableB({ { &tableBcolumnA ,&tableBcolumnB } });

    Table tableC = InnerJoin(tableA, tableB, { {0,0},{0,1},{1,1} }, { {0,0} });

    std::cout << "TableA:" << std::endl;
    Print(tableA);
    std::cout << std::endl << "TableB:" << std::endl;
    Print(tableB);
    std::cout << std::endl << "TableC:" << std::endl;
    Print(tableC);
    std::cout << std::endl;

    std::cin.get();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
