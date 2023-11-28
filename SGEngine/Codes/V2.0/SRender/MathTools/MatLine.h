// 矩阵行，实际是一个数组
// Warning: The file is not used.  

#ifndef SRENDER_MATHTOOLS_MATLINE
#define SRENDER_MATHTOOLS_MATLINE

#include "MathException.h"

namespace SRender 
{
	namespace MathTools 
	{
		template<typename T>
		class MatLine
		{
		public:
			unsigned int length = 0;
			T* data = nullptr;

			MatLine() = default;
			MatLine(unsigned int len);
			MatLine(const MatLine& other);

			T& operator[](unsigned int index);

			void Resize(unsigned int len);
		};

		template<typename T>
		inline MatLine<T>::MatLine(unsigned int len)
		{
			length = len;
			data = new T[len];
		}

		template<typename T>
		inline MatLine<T>::MatLine(const MatLine& other)
		{
			length = other.length;
			data = new T[length];

			for (int i = 0; i < length; i++)
			{
				data[i] = other[i];
			}
		}

		template<typename T>
		inline T& MatLine<T>::operator[](unsigned int index)
		{
			if (index < length)
			{
				return data[index];
			}
			throw IndexOutOfRange(__FILE__, __LINE__, length, index);
		}

		template<typename T>
		inline void MatLine<T>::Resize(unsigned int len)
		{
			delete data;
			length = len;
			data = new T[len];
		}
	}
}



#endif // SRENDER_MATHTOOLS_MATLINE


