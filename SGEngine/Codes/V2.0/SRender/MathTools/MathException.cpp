#include "MathException.h"

SRender::MathTools::IndexOutOfRange::IndexOutOfRange(
	const std::string& _file,
	const int _line,
	int _maxLength,
	int _calledIndex) :
	file(_file), line(_line), maxLength(_maxLength), calledIndex(_calledIndex)
{
	;
}

inline SRender::MathTools::IndexOutOfRange::~IndexOutOfRange()
{
	delete[] message;
}

inline char const* SRender::MathTools::IndexOutOfRange::what() const
{
	snprintf(message, 1024, "Error:IndexOutOfRange at File:%s Line:%d MaxLength:%d CalledIndex:%d", file.c_str(), line, maxLength, calledIndex);
	return message;
}

SRender::MathTools::MatSizeNotMatch::MatSizeNotMatch(
	const std::string& _file, 
	const int _line, 
	const int targetRow, 
	const int targetCol, 
	const int realRow, 
	const int realCol) :
	file(_file), line(_line), targetRow(targetRow), targetCol(targetCol), realRow(realRow), realCol(realCol)
{
	;
}

inline SRender::MathTools::MatSizeNotMatch::~MatSizeNotMatch()
{
	delete[] message;
}

char const* SRender::MathTools::MatSizeNotMatch::what() const
{
	snprintf(message, 1024, "Error:MatSizeNotMatch at File:%s Line:%d TargetSize:(%d,%d) RealSize:(%d,%d)", 
		file.c_str(), line, targetRow, targetCol, realRow, realCol);
	return message;
}
