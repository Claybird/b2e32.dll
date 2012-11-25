#include "stdafx.h"
#include "main.h"
#include "OpenArchiveData.h"

COpenArchiveData::COpenArchiveData(CNoahArchiverManager &nam):
	m_itemIndex(0),
	m_ArchiveManager(nam)
{
}

COpenArchiveData::~COpenArchiveData()
{
}

bool COpenArchiveData::ScanArchive(const LPCTSTR lpszArcFileName)
{
	m_arcFile=lpszArcFileName;
	m_ItemArray.empty();
	return m_ArchiveManager.do_filelist(lpszArcFileName,m_ItemArray);
}

int COpenArchiveData::GetItemCount()
{
	return m_ItemArray.len();
}

LPCTSTR COpenArchiveData::GetArchiveName()
{
	return m_arcFile;
}

bool COpenArchiveData::GetItemData(arcfile &af)
{
	if(m_ItemArray.len()<=m_itemIndex)return false;

	af=m_ItemArray[m_itemIndex];
	return true;
}

bool COpenArchiveData::FindFirstFile()
{
	m_itemIndex=0;
	return m_ItemArray.len()>m_itemIndex;
}

bool COpenArchiveData::FindNextFile()
{
	m_itemIndex++;
	return m_ItemArray.len()>m_itemIndex;
}

