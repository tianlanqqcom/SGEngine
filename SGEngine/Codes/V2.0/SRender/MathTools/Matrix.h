// Template - Matrix Class
// Can be used for classes that has correct operator.
// If needed, input zero and one func to set element. 

#ifndef SRENDER_MATHTOOLS_MATRIX
#define SRENDER_MATHTOOLS_MATRIX

#include "MathException.h"
#include "../EngineTools/StringTools.h"

namespace SRender
{
    namespace MathTools
    {
        template<typename T>
        class Matrix
        {
        public:
            //Constructors
            Matrix();

            // @ para int row
            // @ para int col
            Matrix(int row, int col);

            // @ para int row
            // @ para int col
            // @ para T   val 矩阵元素的默认值
            Matrix(int row, int col, T val);

            // @ para int row
            // @ para int col
            // @ para T(*zero)()    指定一个得到零元素的方法
            // @ para T(*one)()     指定一个得到一元素的方法
            Matrix(int row, int col, T(*zero)(), T(*one)());

            // 复制构造函数
            Matrix(Matrix const& other);

            ~Matrix();

            // 更改矩阵大小并重新分配空间
            // @ para int row
            // @ para int col
            void ReSize(int row, int col);

            void ReSize(int row, int col, T val);

            void ReSize(int row, int col, int val);

            //Basic Get/Set methods
            int GetRow() const;

            int GetCol() const;

            T GetValue(int x, int y) const;

            T* const GetValuePtr() const;

            void SetValuePtr(T* _value);

            void SetValue(int x, int y, T val);

            void SetZeroFunc(T(*zero)());

            void SetOneFunc(T(*one)());

            //Math operation
            Matrix Add(Matrix const& other) const;

            Matrix Subtract(Matrix const& other) const;

            Matrix Multiply(Matrix const& other) const;

            Matrix Transfer() const;

            Matrix Inverse() const;

            //Operator overloading
            Matrix& operator=(Matrix const& other);

            Matrix operator+(Matrix const& other) const;

            Matrix operator-(Matrix const& other) const;

            Matrix operator*(Matrix const& other) const;

            Matrix operator+=(Matrix const& other);

            Matrix operator-=(Matrix const& other);

            Matrix operator*=(Matrix const& other);

            T* operator[](int x) const;

            std::string ToString() const;

            // static Methods, used to get special matrix
            
            // Get Zero Matrix
            // @ para int row
            // @ para int col
            static Matrix GetZeros(int row, int col);

            // Get Zero Matrix using zero function
            // @ para int row
            // @ para int col
            // @ para T(*zero)()    zero funtion
            static Matrix GetZeros(int row, int col, T(*zero)());

            // Get One Matrix
            // @ para int row
            // @ para int col
            static Matrix GetOnes(int row, int col);

            // Get One Matrix using one function
            // @ para int row
            // @ para int col
            // @ para T(*one)()    one funtion
            static Matrix GetOnes(int row, int col, T(*one)());

            // Get Unit Matrix
            // @ para int size
            static Matrix GetUnit(int size);

            // Get Unit Matrix using one function
            // @ para int size
            // @ para T(*one)()    one funtion
            static Matrix GetUnit(int size, T(*one)());

        private:
            int row         = 0;
            int col         = 0;
            T*  value       = nullptr;
            T   (*zero)()   = nullptr;
            T   (*one)()    = nullptr;

            // 重新分配地址空间
            void Resize();
        };

        template<typename T>
        inline Matrix<T>::Matrix()
        {
            this->row = 1;
            this->col = 1;
            Resize();
        }

        template<typename T>
        inline Matrix<T>::Matrix(int row, int col)
        {
            this->row = row;
            this->col = col;
            Resize();
            /*for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    value[i * col + j] = 0;
                }
            }*/
        }

        template<typename T>
        inline Matrix<T>::Matrix(int row, int col, T val)
        {
            this->row = row;
            this->col = col;
            Resize();
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    value[i * col + j] = val;
                }
            }
        }

        template<typename T>
        inline Matrix<T>::Matrix(int row, int col, T(*zero)(), T(*one)())
        {
            this->row = row;
            this->col = col;
            this->zero = zero;
            this->one = one;
            Resize();
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    if (zero)
                    {
                        value[i * col + j] = zero();
                    }
                }
            }
        }

        template<typename T>
        inline Matrix<T>::Matrix(const Matrix& other)
        {
            this->row = other.GetRow();
            this->col = other.GetCol();
            Resize();
            if (value)
            {
                memcpy(value, other.value, sizeof(T) * row * col);
            }
        }

        template<typename T>
        inline Matrix<T>::~Matrix()
        {
            free(value);
        }

        template<typename T>
        inline void Matrix<T>::ReSize(int row, int col)
        {
            this->row = row;
            this->col = col;
            Resize();
        }

        template<typename T>
        inline void Matrix<T>::ReSize(int row, int col, T val)
        {
            this->row = row;
            this->col = col;
            Resize();
            for (int i = 0; i < row; i++)
            {
                int disp = i * col;
                for (int j = 0; j < col; j++)
                {
                    value[disp + j] = val;
                }
            }
        }

        template<typename T>
        inline void Matrix<T>::ReSize(int row, int col, int val)
        {
            this->row = row;
            this->col = col;
            Resize();
            memset(value, val, sizeof(T) * row * col);
        }

        template<typename T>
        inline void Matrix<T>::Resize()
        {
            if (value)
                free(value);
            value = (T*)malloc(row * col * sizeof(T));
        }

        template<typename T>
        inline int Matrix<T>::GetRow() const
        {
            return this->row;
        }

        template<typename T>
        inline int Matrix<T>::GetCol() const
        {
            return this->col;
        }

        template<typename T>
        inline T Matrix<T>::GetValue(int x, int y) const
        {
            if (x >= row)
            {
                throw IndexOutOfRange(__FILE__, __LINE__, row, x);
            }

            if (y >= col)
            {
                throw IndexOutOfRange(__FILE__, __LINE__, col, y);
            }

            return value[x * col + y];
        }

        template<typename T>
        inline T* const Matrix<T>::GetValuePtr() const
        {
            return value;
        }

        template<typename T>
        inline void Matrix<T>::SetValuePtr(T* _value)
        {
            free(value);
            value = _value;
        }

        template<typename T>
        inline void Matrix<T>::SetValue(int x, int y, T val)
        {
            if (x >= row)
            {
                throw IndexOutOfRange(__FILE__, __LINE__, row, x);
            }

            if (y >= col)
            {
                throw IndexOutOfRange(__FILE__, __LINE__, col, y);
            }

            value[x * col + y] = val;
        }

        template<typename T>
        inline void Matrix<T>::SetZeroFunc(T(*zero)())
        {
            this->zero = zero;
        }

        template<typename T>
        inline void Matrix<T>::SetOneFunc(T(*one)())
        {
            this->one = one;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::Add(Matrix const& other) const
        {
            if (row != other.row || col != other.col)
            {
                throw MatSizeNotMatch(__FILE__, __LINE__, row, col, other.row, other.col);
            }

            Matrix result(row, col);

            for (int i = 0; i < GetRow(); i++)
            {
                for (int j = 0; j < GetCol(); j++)
                {
                    result.SetValue(i, j, value[i * col + j] + other.GetValue(i, j));
                }
            }
            return result;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::Subtract(Matrix const& other) const
        {
            if (row != other.row || col != other.col)
            {
                throw MatSizeNotMatch(__FILE__, __LINE__, row, col, other.row, other.col);
            }

            Matrix result(row, col);

            for (int i = 0; i < GetRow(); i++)
            {
                for (int j = 0; j < GetCol(); j++)
                {
                    result.SetValue(i, j, value[i * col + j] - other.GetValue(i, j));
                }
            }
            return result;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::Multiply(Matrix const& other) const
        {
            if (col != other.row)
            {
                throw MatSizeNotMatch(__FILE__, __LINE__, col, 0x7fffffff, other.row, other.col);
            }

            Matrix result(row, other.col, 0);

            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < other.GetCol(); j++)
                {
                    T sum = zero ? zero() : 0;
                    for (int k = 0; k < row; k++)
                    {
                        sum += value[i * col + k] * other.GetValue(k, j);
                    }
                    result.SetValue(i, j, sum);
                }
            }
            return result;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::Transfer() const
        {
            Matrix<T> result(col, row);
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    result.SetValue(j, i, value[i * col + j]);
                }
            }
            return result;
        }

        template<typename T>
        inline Matrix<T>& Matrix<T>::operator=(Matrix const& other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (other.GetRow() != GetRow()
                || other.GetCol() != GetCol())
            {
                free(value);
                value = nullptr;

                row = other.GetRow();
                col = other.GetCol();
                Resize();
            }

            if (value)
            {
                memcpy(value, other.value, sizeof(T) * row * col);
            }
            
            return *this;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::operator+(Matrix const& other) const
        {
            return Add(other);
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::operator-(Matrix const& other) const
        {
            return Subtract(other);
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::operator*(Matrix const& other) const
        {
            return Multiply(other);
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::operator+=(Matrix const& other)
        {
            *this = Add(other);
            return *this;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::operator-=(Matrix const& other)
        {
            *this = Subtract(other);
            return *this;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::operator*=(Matrix const& other)
        {
            *this = Multiply(other);
            return *this;
        }

        template<typename T>
        inline T* Matrix<T>::operator[](int x) const
        {
// #ifdef DEBUG
            if (x >= row)
            {
                throw IndexOutOfRange(__FILE__, __LINE__, row, x);
            }
// #endif // DEBUG

            return &value[x * col];
        }

        template<typename T>
        inline std::string Matrix<T>::ToString() const
        {
            std::string res = "";
            res += ("Row = " + std::to_string(row));
            res += " Col = " + std::to_string(col) + '\n';
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    res += std::to_string(value[i * col + j]);
                    res += ' ';
                }
                res += '\n';
            };
            return res;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::GetZeros(int row, int col)
        {
            return Matrix(row, col, 0);
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::GetZeros(int row, int col, T(*zero)())
        {
            Matrix<T> result(row, col, zero());
            return result;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::GetOnes(int row, int col)
        {
            return Matrix(row, col, 1);
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::GetOnes(int row, int col, T(*one)())
        {
            return Matrix(row, col, one());
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::GetUnit(int size)
        {
            Matrix<T> result(size, size);
            for (int i = 0; i < size; i++)
            {
                result.SetValue(i, i, 1);
            }
            return result;
        }

        template<typename T>
        inline Matrix<T> Matrix<T>::GetUnit(int size, T(*one)())
        {
            Matrix<T> result(size, size);
            T temp = one();
            for (int i = 0; i < size; i++)
            {
                result.SetValue(i, i, temp);
            }
            return result;
        }

        template<typename T>
        Matrix<T> Matrix<T>::Inverse() const
        {
            if (row != col)
            {
                throw std::invalid_argument("Matrix must be square to calculate inverse.");
            }

            int n = row;
            Matrix<T> result(n, n, 0);
            Matrix<T> copy(*this); // Copy of the original matrix

            // Initialize result as an identity matrix
            for (int i = 0; i < n; i++)
            {
                result.SetValue(i, i, 1);
            }

            // Gaussian elimination
            for (int i = 0; i < n; i++)
            {
                // Find maximum in this column
                T maxEl = abs(copy.GetValue(i, i));
                int maxRow = i;
                for (int k = i + 1; k < n; k++)
                {
                    if (abs(copy.GetValue(k, i)) > maxEl)
                    {
                        maxEl = abs(copy.GetValue(k, i));
                        maxRow = k;
                    }
                }

                // Swap maximum row with current row
                for (int k = i; k < n; k++)
                {
                    T tmp = copy.GetValue(maxRow, k);
                    copy.SetValue(maxRow, k, copy.GetValue(i, k));
                    copy.SetValue(i, k, tmp);
                }

                for (int k = 0; k < n; k++)
                {
                    T tmp = result.GetValue(maxRow, k);
                    result.SetValue(maxRow, k, result.GetValue(i, k));
                    result.SetValue(i, k, tmp);
                }

                // Make all rows below this one 0 in current column
                for (int k = i + 1; k < n; k++)
                {
                    T c = -copy.GetValue(k, i) / copy.GetValue(i, i);
                    for (int j = 0; j < n; j++)
                    {
                        if (i == j)
                        {
                            copy.SetValue(k, j, 0);
                        }
                        else
                        {
                            copy.SetValue(k, j, copy.GetValue(k, j) + c * copy.GetValue(i, j));
                        }
                        result.SetValue(k, j, result.GetValue(k, j) + c * result.GetValue(i, j));
                    }
                }
            }

            // Solve equation Ax=b for an upper triangular matrix A
            for (int i = n - 1; i >= 0; i--)
            {
                for (int k = i + 1; k < n; k++)
                {
                    result.SetValue(i, k, result.GetValue(i, k) - copy.GetValue(i, k) * result.GetValue(k, k));
                }
                result.SetValue(i, i, result.GetValue(i, i) / copy.GetValue(i, i));
            }

            return result;
        }
    }
}
#endif // SRENDER_MATHTOOLS_MATRIX
