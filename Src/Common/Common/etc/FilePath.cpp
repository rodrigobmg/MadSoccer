
#include "stdafx.h"
#include "FilePath.h"
#include <shlwapi.h> // �� ��� ���Ͽ� FilePath�� ���õ� �Լ����� ����. �� �̿��غ���.
#pragma comment(lib, "shlwapi")

using namespace common;

//	_splitpath(findFilePath.c_str(), drive, dir, NULL,NULL); �Լ��� ���ϸ��� �и��� �� �ִ�.


//------------------------------------------------------------------------
// fileName�� ��ο� Ȯ���ڸ� ������ �����̸����� �����Ѵ�.
//------------------------------------------------------------------------
std::string common::GetFileNameExceptExt(const std::string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	char *name = PathFindFileNameA(srcFileName);
	PathRemoveExtensionA(name);
	return name;
}


//------------------------------------------------------------------------
// fileName���� �����̸��� Ȯ���ڸ� ������ ������ ��θ� �����Ѵ�.
//------------------------------------------------------------------------
std::string common::GetFilePathExceptFileName(const std::string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	PathRemoveFileSpecA( srcFileName );
	return srcFileName;
}