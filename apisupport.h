#ifndef __APISUPPORT_H__
#define __APISUPPORT_H__

//APIがサポートされているかどうか
const static BOOL s_bAPISupportTable[ISARC_FUNCTION_END+1/*=94*/]={
TRUE,	//0	B2E
TRUE,	//1	B2EGetVersion
TRUE,	//2	B2EGetCursorInterval
TRUE,	//3	B2ESetCursorInterval
TRUE,	//4	B2EGetBackGroundMode
TRUE,	//5	B2ESetBackGroundMode
TRUE,	//6	B2EGetCursorMode
TRUE,	//7	B2ESetCursorMode
TRUE,	//8	B2EGetRunning
FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,	//9-15:欠番
TRUE,	//16	B2ECheckArchive
TRUE,	//17	B2EConfigDialog
TRUE,	//18	B2EGetFileCount
TRUE,	//19	B2EQueryFunctionList
FALSE,	//20	(B2EHOut)
FALSE,	//21	(B2EStructOut)
FALSE,	//22	B2EGetArcFileInfo

TRUE,	//23	B2EOpenArchive
TRUE,	//24	B2ECloseArchive
TRUE,	//25	B2EFindFirst
TRUE,	//26	B2EFindNext
FALSE,	//27	(B2EExtract)
FALSE,	//28	(B2EAdd)
FALSE,	//29	(B2EMove)
FALSE,	//30	(B2EDelete)
FALSE,	//31	B2ESetOwnerWindow
FALSE,	//32	B2EClearOwnerWindow
FALSE,	//33	B2ESetOwnerWindowEx
FALSE,	//34	B2EKillOwnerWindowEx
FALSE,FALSE,FALSE,FALSE,FALSE,	//35-39:欠番
TRUE,	//40	B2EGetArcFileName
FALSE,	//41	B2EGetArcFileSize
FALSE,	//42	B2EArcOriginalSize
FALSE,	//43	B2EGetArcCompressedSize
FALSE,	//44	B2EGetArcRatio
FALSE,	//45	B2EGetArcDate
FALSE,	//46	B2EGetArcTime
FALSE,	//47	B2EGetArcOSType
FALSE,	//48	B2EGetArcIsSFXFile
FALSE,	//49	B2EGetArcWriteTimeEx
FALSE,	//50	B2EGetArcCreateTimeEx
FALSE,	//51	B2EGetArcAccessTimeEx
FALSE,	//52	B2EGetArcCreateTimeEx2
FALSE,	//53	B2EGetArcWriteTimeEx2
TRUE,	//57	B2EGetFileName
TRUE,	//58	B2EGetOriginalSize
TRUE,	//59	B2EGetCompressedSize
TRUE,	//60	B2EGetRatio
TRUE,	//61	B2EGetDate
TRUE,	//62	B2EGetTime
TRUE,	//63	B2EGetCRC
FALSE,	//64	B2EGetAttribute
FALSE,	//65	B2EGetOSType
TRUE,	//66	B2EGetMethod
FALSE,	//67	B2EGetWriteTime
FALSE,	//68	B2EGetCreateTime
FALSE,	//69	B2EGetAccessTime
FALSE,	//70	B2EGetWriteTimeEx
FALSE,	//71	B2EGetCreateTimeEx
FALSE,	//72	B2EGetAccessTimeEx
FALSE,	//80	B2ESetEnumMembersProc
FALSE,	//81	B2EClearEnumMembersProc
FALSE,	//82	B2EGetArcFileSizeEx
FALSE,	//83	B2EArcOriginalSizeEx
FALSE,	//84	B2EGetArcCompressedSizeEx
FALSE,	//85	B2EGetOriginalSizeEx
FALSE,	//86	B2EGetCompressedSizeEx
FALSE,	//87	B2ESetOwnerWindowEx64
FALSE,	//88	B2EKillOwnerWindowEx64
FALSE,	//89	B2ESetEnumMembersProc64
FALSE,	//90	B2EClearEnumMembersProc64
FALSE,	//91	B2EOpenArchive2
FALSE,	//92	B2EGetArcReadSize
FALSE,	//93	B2EGetArcReadSizeEx
};

#endif //__APISUPPORT_H__
