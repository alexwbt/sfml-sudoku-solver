
#include "sudoku.h"

#include <iostream>
#include <list>
#include <set>

bool GuessNumber(Sudoku& sudoku, int x, int y, Cell& cell)
{
    return false;
}

void UniqueGrouping(int group_size, int list_size, std::function<void(std::vector<int>&)> callback, std::shared_ptr<std::vector<int>> indices = nullptr, int j = 0)
{
    if (!indices) indices = std::make_shared<std::vector<int>>();
    for (int i = j; i < list_size - (group_size - indices->size() - 1); i++)
    {
        indices->push_back(i);
        if (indices->size() == group_size) callback(*indices);
        else UniqueGrouping(group_size, list_size, callback, indices, i + 1);
        indices->pop_back();
    }
}

void Solve(Sudoku& sudoku)
{
    auto note_possible_numbers = [&](int x, int y, Cell& cell)
    {
        if (cell.value != 0)
        {
            cell.is_static = true;
            return false;
        }
        for (int i = 0; i < kNumCount; i++)
        {
            bool note = true;
            uint8_t test_number = i + 1;

            // test if number exists
            auto test = [&](int, int, Cell& test_c)
            {
                if (test_c.value == test_number)
                    note = false;
                return !note;
            };
            sudoku.MapBlock(x, y, test);
            if (note) sudoku.MapLineX(y, test);
            if (note) sudoku.MapLineY(x, test);

            cell.note[i] = note;
        }
        return false;
    };
    sudoku.Map(note_possible_numbers);

    // level 1
    bool set_value = true;
    auto set_value_and_update_note = [&](int x, int y, Cell& cell, int note)
    {
        cell.value = note + 1;
        for (int i = 0; i < kNumCount; i++)
            cell.note[i] = false;

        // remove note
        auto remove_note = [&](int, int, Cell& remove_c)
        {
            remove_c.note[note] = false;
            return false;
        };
        sudoku.MapBlock(x, y, remove_note);
        sudoku.MapLineX(y, remove_note);
        sudoku.MapLineY(x, remove_note);
    };
    auto find_only_possibility = [&](int x, int y, Cell& cell)
    {
        if (cell.value != 0) return false;

        // check only one note
        int note_count = 0;
        int note = -1;
        for (int i = 0; i < kNumCount; i++)
        {
            if (cell.note[i])
            {
                note_count++;
                note = i;
            }
        }
        if (note_count == 1)
        {
            set_value_and_update_note(x, y, cell, note);
            set_value = true;
            return false;
        }

        // test only one possibility
        for (int i = 0; i < kNumCount; i++)
        {
            bool only = true;
            if (!cell.note[i]) continue;

            auto test = [&](int tx, int ty, Cell& test_c)
            {
                if ((tx != x || ty != y) && test_c.value == 0 && test_c.note[i])
                    only = false;
                return !only;
            };
            sudoku.MapBlock(x, y, test);
            if (!only)
            {
                only = true;
                sudoku.MapLineX(y, test);
            }
            if (!only)
            {
                only = true;
                sudoku.MapLineY(x, test);
            }

            if (only)
            {
                set_value_and_update_note(x, y, cell, i);
                set_value = true;
            }
        }
        return false;
    };

    // level 2
    struct CellCoord { int x, y; Cell& cell; };
    std::vector<CellCoord> check_list;
    bool removed_note = true;
    auto add_to_list = [&](int x, int y, Cell& cell)
    {
        if (cell.value == 0)
            check_list.push_back({ x, y, cell });
        return false;
    };
    auto multi_contain_test = [&](std::function<void(std::function<bool(int, int, Cell&)>)> map_function, std::vector<int>& indices)
    {
        std::set<int> notes;
        for (int i = 0; i < indices.size(); i++)
            for (int j = 0; j < kNumCount; j++)
                if (check_list[indices[i]].cell.note[j])
                    notes.insert(j);

        if (notes.size() == indices.size())
        {
            auto mct_update = [&](int x, int y, Cell& cell)
            {
                for (int i = 0; i < indices.size(); i++)
                    if (check_list[indices[i]].x == x && check_list[indices[i]].y == y)
                        return false;
                for (int i : notes)
                {
                    if (cell.note[i])
                        removed_note = true;
                    cell.note[i] = false;
                }
                return false;
            };
            map_function(mct_update);
        }
    };

    bool running = true;
    while (running)
    {
        // level 1
        set_value = true;
        while (set_value)
        {
            set_value = false;
            sudoku.Map(find_only_possibility);

            if (!set_value && !removed_note)
                running = false;
        }

        // level 2
        removed_note = true;
        while (removed_note)
        {
            removed_note = false;

            // blocks
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    auto map = std::bind(&Sudoku::MapBlock, &sudoku, x * 3, y * 3, std::placeholders::_1);
                    map(add_to_list);
                    for (int i = 2; i < check_list.size() - 1; i++)
                        UniqueGrouping(i, check_list.size(), [&](std::vector<int>& indices) { multi_contain_test(map, indices); });
                    check_list.clear();
                }
            }

            // lines
            for (int j = 0; j < kSize; j++)
            {
                auto map_x = std::bind(&Sudoku::MapLineX, &sudoku, j, std::placeholders::_1);
                map_x(add_to_list);
                for (int i = 2; i < check_list.size() - 1; i++)
                    UniqueGrouping(i, check_list.size(), [&](std::vector<int>& indices) { multi_contain_test(map_x, indices); });
                check_list.clear();

                auto map_y = std::bind(&Sudoku::MapLineY, &sudoku, j, std::placeholders::_1);
                map_y(add_to_list);
                for (int i = 2; i < check_list.size() - 1; i++)
                    UniqueGrouping(i, check_list.size(), [&](std::vector<int>& indices) { multi_contain_test(map_y, indices); });
                check_list.clear();
            }
        }
    }
}
