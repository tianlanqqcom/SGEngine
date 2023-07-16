#ifndef SRENDER_MATHTOOLS_MATHEXCEPTION
#define SRENDER_MATHTOOLS_MATHEXCEPTION

#include <exception>
#include <string>

namespace SRender
{
	namespace MathTools 
	{
		// 越界异常
		class IndexOutOfRange :std::exception
		{
			const std::string	file;						// 发生问题的文件
			const int			line;						// 发生问题的行号
			const int			maxLength;					// 该数组最大可用长度
			const int			calledIndex;				// 实际调用的索引值
			char *				message = new char[1024];	// 输出的消息信息
		public:
			IndexOutOfRange(const std::string& _file,
							const int _line,
							int _maxLength,
							int _calledIndex);

			~IndexOutOfRange();

			_NODISCARD virtual char const* what() const override;
		};

		// 矩阵尺寸不匹配
		class MatSizeNotMatch :std::exception
		{
			const std::string	file;						// 发生问题的文件
			const int			line;						// 发生问题的行号
			const int			targetRow;					// 正确的矩阵行数
			const int			targetCol;					// 正确的矩阵列数
			const int			realRow;					// 实际的矩阵行数
			const int			realCol;					// 实际的矩阵列数
			char*				message = new char[1024];	// 输出的消息信息
		public:
			MatSizeNotMatch(const std::string& _file,
				const int _line,
				const int targetRow,
				const int targetCol,
				const int realRow,
				const int realCol);

			~MatSizeNotMatch();

			_NODISCARD virtual char const* what() const override;
		};
	}
}
#endif // SRENDER_MATHTOOLS_MATHEXCEPTION

