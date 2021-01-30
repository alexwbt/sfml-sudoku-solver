
#include "sudoku.h"

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

    int loop_count = 0;
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

        if (!set_value || loop_count > 0) loop_count++;
        if (loop_count > 10) break;
    }
}
