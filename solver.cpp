
#include "sudoku.h"

#include <iostream>
#include <list>
#include <set>

bool GuessNumber(Sudoku& sudoku, int x, int y, Cell& cell)
{
    return false;
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

    // level 1
    while (true)
    {
        bool set_value = false;
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
        sudoku.Map(find_only_possibility);

        if (!set_value) break;
    }

    // level 2
    struct CellCoord { int x, y; Cell* cell; };
    std::list<CellCoord> cells;
    std::set<int> notes;
    bool found = false;
    auto multi_contain_test = [&](std::function<void(const CellCoord&)> update_note, int x, int y, Cell& cell)
    {
        if (cell.value > 0) return false;

        if (cells.size() < 3) // doesn't work
            cells.push_back({ x, y, &cell });
        else cells.pop_front();

        if (cells.size() < 2) return false;

        notes.clear();
        for (auto c : cells)
            for (int i = 0; i < kNumCount; i++)
                if (c.cell->note[i])
                    notes.insert(i);

        if (notes.size() == cells.size())
        {
            for (const auto& c : cells)
                update_note(c);
            cells.clear();
            found = true;
        }
        return false;
    };
    auto mct_update = [&](int x, int y, Cell& cell)
    {
        for (const auto& nc : cells)
            if (nc.x == x && nc.y == y)
                return false;
        for (int i : notes)
            cell.note[i] = false;
        return false;
    };
    auto mct_map_block = std::bind<bool>(multi_contain_test, [&](const CellCoord& c)
        { sudoku.MapBlock(c.x, c.y, mct_update); }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    auto mct_map_x = std::bind<bool>(multi_contain_test, [&](const CellCoord& c)
        { sudoku.MapLineX(c.y, mct_update); }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    auto mct_map_y = std::bind<bool>(multi_contain_test, [&](const CellCoord& c)
        { sudoku.MapLineY(c.x, mct_update); }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    while (true)
    {
        found = false;
        auto map = [&](int x, int y, Cell& cell)
        {
            cells.clear();
            notes.clear();
            sudoku.MapBlock(x, y, mct_map_block);
            cells.clear();
            notes.clear();
            sudoku.MapLineX(y, mct_map_x);
            cells.clear();
            notes.clear();
            sudoku.MapLineY(x, mct_map_y);
            return false;
        };
        sudoku.Map(map);

        if (!found) break;
    }
}
