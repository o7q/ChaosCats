#pragma once

template <typename T>
struct Matrix
{
    Matrix(int rowSize, int colSize)
    {
        matrix = new T[rowSize * colSize];

        this->rowSize = rowSize;
        this->colSize = colSize;
    }

    ~Matrix()
    {
        delete[] matrix;
    }

    void init(T value)
    {
        for (int row = 0; row < rowSize; ++row)
        {
            for (int col = 0; col < colSize; ++col)
            {
                set(row, col, value);
            }
        }
    }

    T get(int row, int col) const
    {
        return matrix[get1DIndex(row, col)];
    }

    void set(int row, int col, T value)
    {
        matrix[get1DIndex(row, col)] = value;
    }

    int getRowSize() const
    {
        return rowSize;
    }

    int getColSize() const
    {
        return colSize;
    }

    bool inBounds(int row, int col) const
    {
        return row >= 0 && row < rowSize && col >= 0 && col < colSize;
    }

private:
    T *matrix = nullptr;

    int rowSize = 0;
    int colSize = 0;

    int get1DIndex(int row, int col) const
    {
        return row * colSize + col;
    }
};