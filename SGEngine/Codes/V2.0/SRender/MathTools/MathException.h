#ifndef SRENDER_MATHTOOLS_MATHEXCEPTION
#define SRENDER_MATHTOOLS_MATHEXCEPTION

#include <exception>
#include <string>

namespace SRender
{
	namespace MathTools 
	{
		// Խ���쳣
		class IndexOutOfRange :std::exception
		{
			const std::string	file;						// ����������ļ�
			const int			line;						// ����������к�
			const int			maxLength;					// �����������ó���
			const int			calledIndex;				// ʵ�ʵ��õ�����ֵ
			char *				message = new char[1024];	// �������Ϣ��Ϣ
		public:
			IndexOutOfRange(const std::string& _file,
							const int _line,
							int _maxLength,
							int _calledIndex);

			~IndexOutOfRange();

			_NODISCARD virtual char const* what() const override;
		};

		// ����ߴ粻ƥ��
		class MatSizeNotMatch :std::exception
		{
			const std::string	file;						// ����������ļ�
			const int			line;						// ����������к�
			const int			targetRow;					// ��ȷ�ľ�������
			const int			targetCol;					// ��ȷ�ľ�������
			const int			realRow;					// ʵ�ʵľ�������
			const int			realCol;					// ʵ�ʵľ�������
			char*				message = new char[1024];	// �������Ϣ��Ϣ
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

