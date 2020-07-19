// SetInterfaceLayer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

struct Table
{//-2147483648 is used to represent NULL
        //Later could replace this integer value with a bitwise operation, just because it would look a lot better
        //Need to identify what the bit/byte representation of this number is
    std::vector<std::vector<int>> columns;
};

struct SelectList
{
    int table;
    int column;
};

struct SelectList2
{
    int column;
    int staticValue;
};

struct ConditionList
{
    //int table;    //Currently only utilized within 'Select(..)' where there is only one Table
    int column;
    //int condition;    //For now everything is '=='
    int staticValue;
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
        int value = column.at(i);
        if (value != -2147483648)
            std::cout << value << std::endl;
        else
            std::cout << "NULL" << std::endl;
    }
}

void Print(Table table)
{
    int columnCount = table.columns.size();
    if (columnCount > 0)
    {
        int rowCount = table.columns.at(0).size();
        for (int i = 0; i < rowCount; i++)
        {
            for (int j = 0; j < columnCount; j++)
            {
                int value = table.columns.at(j).at(i);
                if (value != -2147483648)
                    std::cout << value << "\t";
                else
                    std::cout << "NULL" << "\t";
            }
            std::cout << std::endl;
        }

    }
}

//1 Table in 1 Table out
//Can be used to reorder or omit columns
//Can utilize a static value condition
//Condition limited to 1 entry for now
    //In the future when there are multiple conditions, instead of checking all of the conditions for each row one at a time:
                                                    //  check 1 condition at a time and maintain a subset of indexes that either passed or failed the condition
                                                    //  each subsequent condition check would be against that subset of indexes continuing the pattern
                                                    //  can start to optimize my code in a 'Set' fashion
Table Select(Table tableA, std::vector<SelectList2> selectColumn, std::vector<ConditionList> condition)
{
    Table newTable;//select.size() determines the number of columns in the new table
    newTable.columns.resize(selectColumn.size());

    //Both tables have columns (Because of the below size check against column[0])
    if (tableA.columns.size() > 0)
    {
        //In the future there will be a different way to manage the record counts in a table
        int tableARowCount = tableA.columns.at(0).size();
        for (int ARow = 0; ARow < tableARowCount; ARow++)
        {                                                       //Hardcoded condition at 0 index
            std::vector<int>& tableAConditionColumn = tableA.columns.at(condition.at(0).column);
            int tableAConditionValue = tableAConditionColumn.at(ARow);
            int staticValue = condition.at(0).staticValue;

            if (tableAConditionValue == staticValue)
            {
                for (int selectID = 0; selectID < selectColumn.size(); selectID++)
                {
                    int value;
                    int columnID = selectColumn.at(selectID).column;
                    if (columnID > -1)
                    {
                        int rowID = ARow;
                        value = tableA.columns.at(columnID).at(rowID);
                    }
                    else
                    {
                        value = selectColumn.at(selectID).staticValue;
                    }
                    newTable.columns.at(selectID).push_back(value);
                }
            }
        }
    }

    return newTable;
}

//Join limited to 1 entry for now
Table InnerJoin(Table tableA, Table tableB, std::vector<SelectList> select, std::vector<JoinList> join)
{
    Table newTable;//select.size() determines the number of columns in the new table
    newTable.columns.resize(select.size());

    //Both tables have columns
    if (tableA.columns.size() > 0 && tableB.columns.size() > 0)
    {
        //Both tables have rows     //In the future there will be a different way to manage the record counts in a table
        int tableARowCount = tableA.columns.at(0).size();
        int tableBRowCount = tableB.columns.at(0).size();
        if (tableARowCount > 0 && tableBRowCount > 0)
        {
            for (int ARow = 0; ARow < tableARowCount; ARow++)
            {
                for (int BRow = 0; BRow < tableBRowCount; BRow++)
                {                                                       //Hardcoded join at 0 index
                    std::vector<int>& tableAJoinColumn = tableA.columns.at(join.at(0).tableAColumn);
                    std::vector<int>& tableBJoinColumn = tableB.columns.at(join.at(0).tableBColumn);
                    int tableAJoinValue = tableAJoinColumn.at(ARow);
                    int tableBJoinValue = tableBJoinColumn.at(BRow);

                    if (tableAJoinValue == tableBJoinValue)
                    {
                        for (int selectID = 0; selectID < select.size(); selectID++)
                        {
                            int tableID = select.at(selectID).table;
                            int columnID = select.at(selectID).column;
                            int rowID = (tableID == 0 ? ARow : BRow);

                            int value = (tableID == 0 ? tableA : tableB).columns.at(columnID).at(rowID);

                            newTable.columns.at(selectID).push_back(value);
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
    std::vector<int> tableAcolumnA({ 1,2,3,4 });
    std::vector<int> tableAcolumnB({ 3,4,11,12 });
    Table tableA({ { tableAcolumnA ,tableAcolumnB } });

    std::vector<int> tableBcolumnA({ 2,1,1,4 });
    std::vector<int> tableBcolumnB({ 5,6,21,22 });
    Table tableB({ { tableBcolumnA ,tableBcolumnB } });

    Table tableC = InnerJoin(tableA, tableB, { {0,0},{0,1},{1,1} }, { {0,0} });
    Table tableD = Select(tableC, { {0,0},{2,0},{-1,9},{1,0},{-1,7} }, { {1,3} });

    std::cout << "TableA:" << std::endl;
    Print(tableA);
    std::cout << std::endl << "TableB:" << std::endl;
    Print(tableB);
    std::cout << std::endl << "TableC:" << std::endl;
    Print(tableC);
    std::cout << std::endl << "TableD:" << std::endl;
    Print(tableD);
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
