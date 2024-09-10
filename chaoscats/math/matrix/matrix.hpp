#pragma once

template <typename T>
struct Mat2D
{
    Mat2D(int rowSize, int colSize)
    {
        matrix = new T[rowSize * colSize];

        this->rowSize = rowSize;
        this->colSize = colSize;
    }

    ~Mat2D()
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

    bool inBounds(int row, int col)
    {
        return row >= 0 && row < rowSize && col >= 0 && col < colSize;
    }

private:
    T *matrix = nullptr;

    int rowSize = 0;
    int colSize = 0;

    int get1DIndex(int row, int col)
    {
        return row * colSize + col;
    }
};