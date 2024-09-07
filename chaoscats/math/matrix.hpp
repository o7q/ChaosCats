#pragma once

namespace jmath
{
    template <typename T>
    struct Matrix2D
    {
        Matrix2D(int rowSize, int colSize)
        {
            matrix = new T[rowSize * colSize];

            this->rowSize = rowSize;
            this->colSize = colSize;
        }

        ~Matrix2D()
        {
            delete[] matrix;
        }

        T get(int row, int col)
        {
            return *(matrix + get1DIndex(row, col));
        }

        void set(int row, int col, T value)
        {
            matrix[get1DIndex(row, col)] = value;
        }

        int getRowSize()
        {
            return rowSize;
        }

        int getColSize()
        {
            return colSize;
        }

        int rowSize = 0;
        int colSize = 0;

    private:
        T *matrix = nullptr;

        int get1DIndex(int row, int col)
        {
            return row * colSize + col;
        }
    };
}