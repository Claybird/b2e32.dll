#ifndef __B2E32_H__
#define __B2E32_H__

#define B2E32_VERSION			7
#define B2E32_SUB_VERSION		0

#ifndef FNAME_MAX32
 #define FNAME_MAX32		512
#endif /*FNAME_MAX32*/

#ifndef ARC_DECSTRACT
#define ARC_DECSTRACT

#if defined(__BORLANDC__)
#pragma option -a-
#else
#pragma pack(1)
#endif/*__BORLANDC__*/

#if !defined(__BORLANDC__) || __BORLANDC__ >= 0x550
typedef LONGLONG	ULHA_INT64;
#else
typedef struct {
	DWORD	LowPart;
	LONG	HighPart;
} ULHA_INT64, *LPULHA_INT64;
#endif/* !defined(__BORLANDC__) || __BORLANDC__ >= 0x550*/

typedef	HGLOBAL	HARC;

typedef struct {
	DWORD 			dwOriginalSize;		/* ファイルのサイズ */
 	DWORD 			dwCompressedSize;	/* 圧縮後のサイズ */
	DWORD			dwCRC;				/* 格納ファイルのチェックサム */
	UINT			uFlag;				/* 処理結果 */
										/* Status flag */
	UINT			uOSType;			/* 書庫作成に使われた OS */
	WORD			wRatio;				/* 圧縮率 */
	WORD			wDate;				/* 格納ファイルの日付(DOS 形式) */
	WORD 			wTime;				/* 格納ファイルの時刻(〃) */
	char			szFileName[FNAME_MAX32 + 1];	/* 書庫名 */
	char			dummy1[3];
	char			szAttribute[8];		/* 格納ファイルの属性(書庫固有) */
	char			szMode[8];			/* 格納ファイルの格納モード(〃) */
}	INDIVIDUALINFO, *LPINDIVIDUALINFO;

typedef struct {
	DWORD 			dwFileSize;		/* 格納ファイルのサイズ */
	DWORD			dwWriteSize;	/* 書き込みサイズ */
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
									/* 解凍先または圧縮元パス名 */
	char			dummy[3];
}	EXTRACTINGINFO, *LPEXTRACTINGINFO;

typedef struct {
	EXTRACTINGINFO exinfo;
	DWORD dwCompressedSize;
	DWORD dwCRC;
	UINT  uOSType;
	WORD  wRatio;
	WORD  wDate;
	WORD  wTime;
	char  szAttribute[8];
	char  szMode[8];
} EXTRACTINGINFOEX, *LPEXTRACTINGINFOEX;

typedef struct {
	DWORD			dwStructSize;
	EXTRACTINGINFO	exinfo;
	DWORD			dwFileSize;			/* 格納ファイルのサイズ */
	DWORD			dwCompressedSize;
	DWORD			dwWriteSize;		/* 書き込みサイズ */
	DWORD			dwAttributes;
	DWORD 			dwCRC;
	UINT  			uOSType;
	WORD  			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char  			szMode[8];
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
										/* 解凍先または圧縮元パス名 */
	char			dummy2[3];
} EXTRACTINGINFOEX32, *LPEXTRACTINGINFOEX32;

typedef struct {
	DWORD			dwStructSize;
	EXTRACTINGINFO	exinfo;
	ULHA_INT64		llFileSize;			/* 格納ファイルのサイズ */
	ULHA_INT64		llCompressedSize;
	ULHA_INT64		llWriteSize;		/* 書き込みサイズ */
	DWORD			dwAttributes;
	DWORD 			dwCRC;
	UINT  			uOSType;
	WORD  			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char  			szMode[8];
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
										/* 解凍先または圧縮元パス名 */
	char			dummy2[3];
} EXTRACTINGINFOEX64, *LPEXTRACTINGINFOEX64;

typedef BOOL CALLBACK ARCHIVERPROC(HWND _hwnd, UINT _uMsg,
						UINT _nState, LPVOID _lpEis);
typedef ARCHIVERPROC *LPARCHIVERPROC;

typedef struct {
	DWORD			dwStructSize;
	UINT			uCommand;
	DWORD			dwOriginalSize;
	DWORD			dwCompressedSize;
	DWORD			dwAttributes;
	DWORD			dwCRC;
	UINT			uOSType;
	WORD			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char			szFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szAddFileName[FNAME_MAX32 + 1];
	char			dummy2[3];
}	B2E_ENUM_MEMBER_INFO, *LPB2E_ENUM_MEMBER_INFO;

typedef struct {
	DWORD			dwStructSize;
	UINT			uCommand;
	ULHA_INT64		llOriginalSize;
	ULHA_INT64		llCompressedSize;
	DWORD			dwAttributes;
	DWORD			dwCRC;
	UINT			uOSType;
	WORD			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char			szFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szAddFileName[FNAME_MAX32 + 1];
	char			dummy2[3];
}	B2E_ENUM_MEMBER_INFO64, *LPB2E_ENUM_MEMBER_INFO64;

typedef BOOL (CALLBACK *B2E_WND_ENUMMEMBPROC)(LPVOID);

#if !defined(__BORLANDC__)
#pragma pack()
#else
#pragma option -a.
#endif/* !defined(__BORLANDC__)*/

#endif /* ARC_DECSTRACT */

#if !defined(__BORLANDC__)
#define	_export
#endif/* !defined(__BORLANDC__)*/

#ifdef __cplusplus
extern "C" {
#endif/* __cplusplus*/

/* ### LHA.DLL Ver 1.1 と互換性のある API 群です。### */
/* LHA.DLL compatible API */
WORD WINAPI _export B2EGetVersion(VOID);

BOOL WINAPI _export B2EGetRunning(VOID);

BOOL WINAPI _export B2EGetBackGroundMode(VOID);
BOOL WINAPI _export B2ESetBackGroundMode(const BOOL _BackGroundMode);
BOOL WINAPI _export B2EGetCursorMode(VOID);
BOOL WINAPI _export B2ESetCursorMode(const BOOL _CursorMode);
WORD WINAPI _export B2EGetCursorInterval(VOID);
BOOL WINAPI _export B2ESetCursorInterval(const WORD _Interval);

int WINAPI _export B2E(const HWND _hwnd, LPCSTR _szCmdLine,
						LPSTR _szOutput, const DWORD _dwSize);

/* ###『統合アーカイバ API』共通の API です。### */
/* API of Common Archiver Project */
#if !defined(CHECKARCHIVE_RAPID)
#define	CHECKARCHIVE_RAPID		0	/* 簡易(最初の３ヘッダまで) */
#define	CHECKARCHIVE_BASIC		1	/* 標準(全てのヘッダ) */
#define	CHECKARCHIVE_FULLCRC	2	/* 完全(格納ファイルの CRC チェック) */

/* 以下のフラグは上記と組み合わせて使用。*/
#define CHECKARCHIVE_RECOVERY	4   /* 破損ヘッダを読み飛ばして処理 */
#define CHECKARCHIVE_SFX		8	/* SFX かどうかを返す */
#define CHECKARCHIVE_ALL		16	/* ファイルの最後まで検索する */
#define CHECKARCHIVE_ENDDATA	32	/* 書庫より後ろの余剰データを許可 */
#endif /* !defined(CHECKARCHIVE_RAPID)*/

BOOL WINAPI _export B2ECheckArchive(LPCSTR _szFileName, const int _iMode);

int WINAPI _export B2EGetFileCount(LPCSTR _szArcFile);

#if !defined(UNPACK_CONFIG_MODE)
#define	UNPACK_CONFIG_MODE		1	/* 解凍系の設定 */
#define PACK_CONFIG_MODE		2	/* 圧縮系の設定 */
#endif

BOOL WINAPI _export B2EConfigDialog(const HWND _hwnd, LPSTR _lpszComBuffer,
						const int _iMode);

#if !defined(ISARC_FUNCTION_START)
#define ISARC_FUNCTION_START			0
#define ISARC							0	/* B2E */
#define ISARC_GET_VERSION				1	/* B2EGetVersion */
#define ISARC_GET_CURSOR_INTERVAL		2	/* B2EGetCursorInterval */
#define ISARC_SET_CURSOR_INTERVAL		3	/* B2ESetCursorInterval */
#define ISARC_GET_BACK_GROUND_MODE		4	/* B2EGetBackGroundMode */
#define ISARC_SET_BACK_GROUND_MODE		5	/* B2ESetBackGroundMode */
#define ISARC_GET_CURSOR_MODE			6	/* B2EGetCursorMode */
#define ISARC_SET_CURSOR_MODE			7	/* B2ESetCursorMode */
#define ISARC_GET_RUNNING				8	/* B2EGetRunning */

#define ISARC_CHECK_ARCHIVE				16	/* B2ECheckArchive */
#define ISARC_CONFIG_DIALOG				17	/* B2EConfigDialog */
#define ISARC_GET_FILE_COUNT			18	/* B2EGetFileCount */
#define ISARC_QUERY_FUNCTION_LIST		19	/* B2EQueryFunctionList */
#define ISARC_HOUT						20	/* (B2EHOut) */
#define ISARC_STRUCTOUT					21	/* (B2EStructOut) */
#define ISARC_GET_ARC_FILE_INFO			22	/* B2EGetArcFileInfo */

#define ISARC_OPEN_ARCHIVE				23	/* B2EOpenArchive */
#define ISARC_CLOSE_ARCHIVE				24	/* B2ECloseArchive */
#define ISARC_FIND_FIRST				25	/* B2EFindFirst */
#define ISARC_FIND_NEXT					26	/* B2EFindNext */
#define ISARC_EXTRACT					27	/* (B2EExtract) */
#define ISARC_ADD						28	/* (B2EAdd) */
#define ISARC_MOVE						29	/* (B2EMove) */
#define ISARC_DELETE					30	/* (B2EDelete) */
#define ISARC_SETOWNERWINDOW			31	/* B2ESetOwnerWindow */
#define ISARC_CLEAROWNERWINDOW			32	/* B2EClearOwnerWindow */
#define ISARC_SETOWNERWINDOWEX			33	/* B2ESetOwnerWindowEx */
#define ISARC_KILLOWNERWINDOWEX			34	/* B2EKillOwnerWindowEx */

#define ISARC_GET_ARC_FILE_NAME			40	/* B2EGetArcFileName */
#define ISARC_GET_ARC_FILE_SIZE			41	/* B2EGetArcFileSize */
#define ISARC_GET_ARC_ORIGINAL_SIZE		42	/* B2EArcOriginalSize */
#define ISARC_GET_ARC_COMPRESSED_SIZE	43	/* B2EGetArcCompressedSize */
#define ISARC_GET_ARC_RATIO				44	/* B2EGetArcRatio */
#define ISARC_GET_ARC_DATE				45	/* B2EGetArcDate */
#define ISARC_GET_ARC_TIME				46	/* B2EGetArcTime */
#define ISARC_GET_ARC_OS_TYPE			47	/* B2EGetArcOSType */
#define ISARC_GET_ARC_IS_SFX_FILE		48	/* B2EGetArcIsSFXFile */
#define ISARC_GET_ARC_WRITE_TIME_EX		49	/* B2EGetArcWriteTimeEx */
#define ISARC_GET_ARC_CREATE_TIME_EX	50	/* B2EGetArcCreateTimeEx */
#define	ISARC_GET_ARC_ACCESS_TIME_EX	51	/* B2EGetArcAccessTimeEx */
#define	ISARC_GET_ARC_CREATE_TIME_EX2	52	/* B2EGetArcCreateTimeEx2 */
#define ISARC_GET_ARC_WRITE_TIME_EX2	53	/* B2EGetArcWriteTimeEx2 */
#define ISARC_GET_FILE_NAME				57	/* B2EGetFileName */
#define ISARC_GET_ORIGINAL_SIZE			58	/* B2EGetOriginalSize */
#define ISARC_GET_COMPRESSED_SIZE		59	/* B2EGetCompressedSize */
#define ISARC_GET_RATIO					60	/* B2EGetRatio */
#define ISARC_GET_DATE					61	/* B2EGetDate */
#define ISARC_GET_TIME					62	/* B2EGetTime */
#define ISARC_GET_CRC					63	/* B2EGetCRC */
#define ISARC_GET_ATTRIBUTE				64	/* B2EGetAttribute */
#define ISARC_GET_OS_TYPE				65	/* B2EGetOSType */
#define ISARC_GET_METHOD				66	/* B2EGetMethod */
#define ISARC_GET_WRITE_TIME			67	/* B2EGetWriteTime */
#define ISARC_GET_CREATE_TIME			68	/* B2EGetCreateTime */
#define ISARC_GET_ACCESS_TIME			69	/* B2EGetAccessTime */
#define ISARC_GET_WRITE_TIME_EX			70	/* B2EGetWriteTimeEx */
#define ISARC_GET_CREATE_TIME_EX		71	/* B2EGetCreateTimeEx */
#define ISARC_GET_ACCESS_TIME_EX		72	/* B2EGetAccessTimeEx */
#define ISARC_SET_ENUM_MEMBERS_PROC		80  /* B2ESetEnumMembersProc */
#define ISARC_CLEAR_ENUM_MEMBERS_PROC	81	/* B2EClearEnumMembersProc */
#define ISARC_GET_ARC_FILE_SIZE_EX		82	/* B2EGetArcFileSizeEx */
#define ISARC_GET_ARC_ORIGINAL_SIZE_EX	83	/* B2EArcOriginalSizeEx */
#define ISARC_GET_ARC_COMPRESSED_SIZE_EX	84	/* B2EGetArcCompressedSizeEx */
#define ISARC_GET_ORIGINAL_SIZE_EX		85	/* B2EGetOriginalSizeEx */
#define ISARC_GET_COMPRESSED_SIZE_EX	86	/* B2EGetCompressedSizeEx */
#define ISARC_SETOWNERWINDOWEX64		87	/* B2ESetOwnerWindowEx64 */
#define ISARC_KILLOWNERWINDOWEX64		88	/* B2EKillOwnerWindowEx64 */
#define ISARC_SET_ENUM_MEMBERS_PROC64	89  /* B2ESetEnumMembersProc64 */
#define ISARC_CLEAR_ENUM_MEMBERS_PROC64	90	/* B2EClearEnumMembersProc64 */
#define ISARC_OPEN_ARCHIVE2				91	/* B2EOpenArchive2 */
#define ISARC_GET_ARC_READ_SIZE			92	/* B2EGetArcReadSize */
#define ISARC_GET_ARC_READ_SIZE_EX		93	/* B2EGetArcReadSizeEx */
#define ISARC_FUNCTION_END				ISARC_GET_ARC_READ_SIZE_EX

#endif	/* ISARC_FUNCTION_START */

BOOL WINAPI _export B2EQueryFunctionList(const int _iFunction);

#ifndef WM_ARCEXTRACT
#define	WM_ARCEXTRACT	"wm_arcextract"

#define	ARCEXTRACT_BEGIN		0	/* 該当ファイルの処理の開始 */
#define	ARCEXTRACT_INPROCESS	1	/* 該当ファイルの展開中 */
#define	ARCEXTRACT_END			2	/* 処理終了、関連メモリを開放 */
#define ARCEXTRACT_OPEN			3	/* 該当書庫の処理の開始 */
#define ARCEXTRACT_COPY			4	/* ワークファイルの書き戻し */

BOOL WINAPI _export B2ESetOwnerWindow(const HWND _hwnd);
BOOL WINAPI _export B2EClearOwnerWindow(VOID);

BOOL WINAPI _export B2ESetOwnerWindowEx(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc);
BOOL WINAPI _export B2EKillOwnerWindowEx(const HWND _hwnd);
BOOL WINAPI _export B2ESetOwnerWindowEx64(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize);
BOOL WINAPI _export B2EKillOwnerWindowEx64(const HWND _hwnd);
#endif

/* OpenArchive 系 API です。 */

#if !defined(EXTRACT_FOUND_FILE)
/* MODE (for UnarjOpenArchive) */
#define M_INIT_FILE_USE			0x00000001L	/* レジストリの設定を使用 */
#define M_REGARDLESS_INIT_FILE	0x00000002L	/* 〃 を使用しない */
#define M_NO_BACKGROUND_MODE	0x00000004L	/* バックグラウンドを禁止 */
#define M_NOT_USE_TIME_STAMP	0x00000008L
#define M_EXTRACT_REPLACE_FILE	0x00000010L
#define M_EXTRACT_NEW_FILE		0x00000020L
#define M_EXTRACT_UPDATE_FILE	0x00000040L
#define M_CHECK_ALL_PATH		0x00000100L	/* 厳格なファイル名サーチ */
#define M_CHECK_FILENAME_ONLY	0x00000200L	/* 〃を行わない */
#define M_CHECK_DISK_SIZE		0x00000400L
#define M_REGARDLESS_DISK_SIZE	0x00000800L
#define M_USE_DRIVE_LETTER		0x00001000L	/* ドライブ名から格納 */
#define M_NOT_USE_DRIVE_LETTER	0x00002000L	/* 〃 を格納しない */
#define M_INQUIRE_DIRECTORY		0x00004000L
#define M_NOT_INQUIRE_DIRECTORY 0x00008000L
#define M_INQUIRE_WRITE			0x00010000L
#define M_NOT_INQUIRE_WRITE		0x00020000L
#define M_CHECK_READONLY		0x00040000L
#define M_REGARDLESS_READONLY	0x00080000L
#define M_REGARD_E_COMMAND		0x00100000L
#define M_REGARD_X_COMMAND		0x00200000L
#define M_ERROR_MESSAGE_ON		0x00400000L	/* エラーメッセージを表示 */
#define M_ERROR_MESSAGE_OFF		0x00800000L	/* 〃を表示しない */
#define M_BAR_WINDOW_ON			0x01000000L
#define M_BAR_WINDOW_OFF		0x02000000L
#define M_CHECK_PATH			0x04000000L
#define M_RECOVERY_ON			0x08000000L /* 破損ヘッダの読み飛ばし */

#define M_MAKE_INDEX_FILE		0x10000000L
#define M_NOT_MAKE_INDEX_FILE	0x20000000L
#define EXTRACT_FOUND_FILE		0x40000000L	/* 検索されたファイルを解凍 */
#define EXTRACT_NAMED_FILE		0x80000000L	/* 指定したファイルを解凍 */
#endif /* EXTRACT_FOUND_FILE */

#if !defined(SFX_NOT)
#define SFX_NOT					0	/* 通常の書庫 */

#define SFX_DOS_S				1		/* LHA's SFX 系 (small) */
#define SFX_DOS_204S			1		/* LHA's SFX 2.04S 以降 */
#define SFX_DOS_213S			1		/* LHA's SFX 2.04S 以降 */
#define SFX_DOS_250S			2		/* LHA's SFX 2.50S 以降 */
#define SFX_DOS_260S			3		/* LHA's SFX 2.60S 以降 */
#define SFX_DOS_265S			3		/* LHA's SFX 2.60S 以降 */

#define SFX_DOS_L				51		/* LHA's SFX 系 (large) */
#define SFX_DOS_204L			51		/* LHA's SFX 2.04L 以降 */
#define SFX_DOS_213L			51		/* LHA's SFX 2.04L 以降 */
#define SFX_DOS_250L			52		/* LHA's SFX 2.50L 以降 */
#define SFX_DOS_260L			53		/* LHA's SFX 2.60L 以降 */
#define SFX_DOS_265L			53		/* LHA's SFX 2.60L 以降 */

#define SFX_DOS_LARC			201		/* SFX by LARC 系 */
#define SFX_DOS_LARC_S			201		/* SFX by LARC (small) */
#define SFX_DOS_LARC_L			202		/* SFX by LARC (large) */

#define SFX_DOS_LHARC			301		/* LHarc's SFX 系 */
#define SFX_DOS_LHARC_S			301		/* LHarc's SFX (small) */
#define SFX_DOS_LHARC_L			351		/* LHarc's SFX (large) */

#define SFX_WIN16_213			1001	/* LHA's SFX 2.13.w16 系 */
#define SFX_WIN16_213_1			1001	/* WinSFX 2.13.w16.1 */
#define SFX_WIN16_213_2			1002	/* WinSFX 2.13.w16.2 */
#define SFX_WIN16_213_3			1003	/* WinSFX 2.13.w16.3 以降 */

#define SFX_WIN16_250			1011	/* LHA's SFX 2.50.w16 系 */
#define SFX_WIN16_250_1			1011	/* WinSFXM 2.50.w16.0001 以降 */
#define SFX_WIN16_255_1			1021	/* WinSFXM 2.55.w16.0001 以降 */

#define SFX_WIN32_213			2001	/* LHA's SFX 2.13.w32 系 */
#define SFX_WIN32_213_1			2001	/* WinSFX32 2.13.w32.1 以降 */
#define SFX_WIN32_213_3			2002	/* WinSFX32 2.13.w32.3 以降 */

#define SFX_WIN32_214_1			2003	/* WinSFX32 2.14.w32.0001 以降 */

#define SFX_WIN32_250			2011	/* LHA's SFX 2.50.w32 系 */
#define SFX_WIN32_250_1			2011	/* WinSFX32M 2.50.w32.0001 以降 */
#define SFX_WIN32_250_6			2012	/* WinSFX32M 2.50.w32.0006 以降 */

#define SFX_LZHSFX				2051	/* LZHSFX 系 */
#define SFX_LZHSFX_1002			2051	/* LZHSFX 1.0.0.2 以降 */
#define SFX_LZHSFX_1100			2052	/* LZHSFX 1.1.0.0 以降 */

#define SFX_LZHAUTO				2101	/* LZHAUTO 系 */
#define SFX_LZHAUTO_0002		2101	/* LZHAUTO 0.0.0.2 以降 */
#define SFX_LZHAUTO_1000		2102	/* LZHAUTO 1.0.0.0 以降 */
#define SFX_LZHAUTO_1002		2102	/* LZHAUTO 1.0.0.0 以降 */
#define SFX_LZHAUTO_1100		2103	/* LZHAUTO 1.1.0.0 以降 */

#define SFX_WIN32_LHASA			3001	/* Lhasa インストーラ */

#define SFX_DOS_UNKNOWN			9901	/* 認識できない DOS SFX */
#define SFX_WIN16_UNKNOWN		9911	/* 認識できない Win16 SFX */
#define SFX_WIN32_UNKNOWN		9921	/* 認識できない Win32 SFX */
#endif

HARC WINAPI _export B2EOpenArchive(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode);
HARC WINAPI _export B2EOpenArchive2(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode, LPCSTR _szOption);
int WINAPI _export B2ECloseArchive(HARC _harc);
int WINAPI _export B2EFindFirst(HARC _harc, LPCSTR _szWildName,
							INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export B2EFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export B2EGetArcFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export B2EGetArcFileSize(HARC _harc);
DWORD WINAPI _export B2EGetArcOriginalSize(HARC _harc);
DWORD WINAPI _export B2EGetArcCompressedSize(HARC _harc);
BOOL WINAPI _export B2EGetArcFileSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export B2EGetArcOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export B2EGetArcCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
DWORD WINAPI _export B2EGetArcReadSize(HARC _harc);
BOOL WINAPI _export B2EGetArcReadSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export B2EGetArcRatio(HARC _harc);
WORD WINAPI _export B2EGetArcDate(HARC _harc);
WORD WINAPI _export B2EGetArcTime(HARC _harc);
BOOL WINAPI _export B2EGetArcWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export B2EGetArcCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export B2EGetArcAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
UINT WINAPI _export B2EGetArcOSType(HARC _harc);
int WINAPI _export B2EIsSFXFile(HARC _harc);
int WINAPI _export B2EGetFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
int WINAPI _export B2EGetMethod(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export B2EGetOriginalSize(HARC _harc);
DWORD WINAPI _export B2EGetCompressedSize(HARC _harc);
BOOL WINAPI _export B2EGetOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export B2EGetCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export B2EGetRatio(HARC _harc);
WORD WINAPI _export B2EGetDate(HARC _harc);
WORD WINAPI _export B2EGetTime(HARC _harc);
DWORD WINAPI _export B2EGetWriteTime(HARC _harc);
DWORD WINAPI _export B2EGetAccessTime(HARC _harc);
DWORD WINAPI _export B2EGetCreateTime(HARC _harc);
BOOL WINAPI _export B2EGetWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export B2EGetCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export B2EGetAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
DWORD WINAPI _export B2EGetCRC(HARC _harc);
int WINAPI _export B2EGetAttribute(HARC _harc);
int WINAPI _export B2EGetAttributes(HARC _harc);
UINT WINAPI _export B2EGetOSType(HARC _harc);

#ifndef FA_RDONLY
/* Attribute */
#define FA_RDONLY       0x01            /* 書き込み保護属性 */
#define FA_HIDDEN       0x02            /* 隠し属性 */
#define FA_SYSTEM       0x04            /* システム属性 */
#define FA_LABEL        0x08            /* ボリューム・ラベル */
#define FA_DIREC        0x10            /* ディレクトリ */
#define FA_ARCH         0x20            /* アーカイブ属性 */
#define FA_COMPRESSED	0x0800			/* 圧縮属性 */
#define FA_ENCRYPTED	0x4000			/* 暗号化属性 */
#endif

#if !defined(SFXFLAG_AUTO)
#define SFXFLAG_AUTO			0x0001
#define SFXFLAG_OVERWRITE		0x0002
#define SFXFLAG_CHECKCRC		0x0004
#define SFXFLAG_CHECKTIME		0x0008
#define SFXFLAG_ATTRIBUTE		0x0010
#define SFXFLAG_NO_AUTO			0x0100
#define SFXFLAG_NO_OVERWRITE	0x0200
#define SFXFLAG_NO_CHECKCRC		0x0400
#define SFXFLAG_NO_CHECKTIME	0x0800
#define SFXFLAG_NO_ATTRIBUTE	0x1000
#define SFXFLAG_NO_WAIT			0x2000
#endif

#if !defined(BPL_NORMAL)
#define BPL_NORMAL					0
#define BPL_DENY_TOO_MANY_PARENTS	1
#define BPL_DENY_ABS_PATH			2
#define BPL_DENY_PARENTS			4
#define BPL_DENY_EXECUTABLE			8
#define BPL_DENY_LEVEL_0			0
#define BPL_DENY_LEVEL_1			1
#define BPL_DENY_LEVEL_2			3
#define BPL_DENY_LEVEL_3			7
#endif

#if !defined(ERROR_START)
#define ERROR_START				0x8000
/* WARNING */
#define ERROR_DISK_SPACE		0x8005
#define ERROR_READ_ONLY			0x8006
#define ERROR_USER_SKIP			0x8007
#define ERROR_UNKNOWN_TYPE		0x8008
#define ERROR_METHOD			0x8009
#define ERROR_PASSWORD_FILE		0x800A
#define ERROR_VERSION			0x800B
#define ERROR_FILE_CRC			0x800C
#define ERROR_FILE_OPEN			0x800D
#define ERROR_MORE_FRESH		0x800E
#define ERROR_NOT_EXIST			0x800F
#define ERROR_ALREADY_EXIST		0x8010

#define ERROR_TOO_MANY_FILES	0x8011

/* ERROR */
#define ERROR_MAKEDIRECTORY		0x8012
#define ERROR_CANNOT_WRITE		0x8013
#define ERROR_HUFFMAN_CODE		0x8014
#define ERROR_COMMENT_HEADER	0x8015
#define ERROR_HEADER_CRC		0x8016
#define ERROR_HEADER_BROKEN		0x8017
#define ERROR_ARC_FILE_OPEN		0x8018
#define ERROR_NOT_ARC_FILE		0x8019
#define ERROR_CANNOT_READ		0x801A
#define ERROR_FILE_STYLE		0x801B
#define ERROR_COMMAND_NAME		0x801C
#define ERROR_MORE_HEAP_MEMORY	0x801D
#define ERROR_ENOUGH_MEMORY		0x801E
#if !defined(ERROR_ALREADY_RUNNING)
#define ERROR_ALREADY_RUNNING	0x801F
#endif
#define ERROR_USER_CANCEL		0x8020
#define ERROR_HARC_ISNOT_OPENED	0x8021
#define ERROR_NOT_SEARCH_MODE	0x8022
#define ERROR_NOT_SUPPORT		0x8023
#define ERROR_TIME_STAMP		0x8024
#define ERROR_TMP_OPEN			0x8025
#define ERROR_LONG_FILE_NAME	0x8026
#define ERROR_ARC_READ_ONLY		0x8027
#define ERROR_SAME_NAME_FILE	0x8028
#define ERROR_NOT_FIND_ARC_FILE 0x8029
#define ERROR_RESPONSE_READ		0x802A
#define ERROR_NOT_FILENAME		0x802B
#define ERROR_TMP_COPY			0x802C
#define ERROR_EOF				0x802D
#define ERROR_ADD_TO_LARC		0x802E
#define ERROR_TMP_BACK_SPACE	0x802F
#define ERROR_SHARING			0x8030
#define ERROR_NOT_FIND_FILE		0x8031
#define ERROR_LOG_FILE			0x8032
#define	ERROR_NO_DEVICE			0x8033
#define ERROR_GET_ATTRIBUTES	0x8034
#define ERROR_SET_ATTRIBUTES	0x8035
#define ERROR_GET_INFORMATION	0x8036
#define ERROR_GET_POINT			0x8037
#define ERROR_SET_POINT			0x8038
#define ERROR_CONVERT_TIME		0x8039
#define ERROR_GET_TIME			0x803a
#define ERROR_SET_TIME			0x803b
#define ERROR_CLOSE_FILE		0x803c
#define ERROR_HEAP_MEMORY		0x803d
#define ERROR_HANDLE			0x803e
#define ERROR_TIME_STAMP_RANGE	0x803f
#define ERROR_MAKE_ARCHIVE		0x8040
#define ERROR_NOT_CONFIRM_NAME	0x8041
#define ERROR_UNEXPECTED_EOF	0x8042
#define ERROR_INVALID_END_MARK	0x8043
#define ERROR_INVOLVED_LZH		0x8044
#define ERROR_NO_END_MARK		0x8045
#define ERROR_HDR_INVALID_SIZE	0x8046
#define ERROR_UNKNOWN_LEVEL		0x8047
#define ERROR_BROKEN_DATA		0x8048
#define ERROR_INVALID_PATH		0x8049
#define ERROR_TOO_BIG			0x804A
#define ERROR_EXECUTABLE_FILE	0x804B

#define ERROR_END	ERROR_EXECUTABLE_FILE
#endif /* ERROR_START */

/* ### B2E32.DLL 独自の API です。### */
#define B2E_LIST_COMMAND		1
#define B2E_ADD_COMMAND		2
#define B2E_FRESH_COMMAND		3
#define B2E_DELETE_COMMAND	4
#define B2E_EXTRACT_COMMAND	5
#define B2E_PRINT_COMMAND		6
#define B2E_TEST_COMMAND		7
#define B2E_MAKESFX_COMMAND	8
#define B2E_JOINT_COMMAND		9
#define B2E_CONVERT_COMMAND	10
#define B2E_RENAME_COMMAND	11

WORD WINAPI _export B2EGetSubVersion(VOID);

int WINAPI _export B2EExtractMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize,
		time_t *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize);
int WINAPI _export B2ECompressMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize,
		const time_t *_lpTime, const LPWORD _lpwAttr,
		LPDWORD _lpdwWriteSize);

BOOL WINAPI _export B2ESetEnumMembersProc(B2E_WND_ENUMMEMBPROC
		_lpEnumProc);
BOOL WINAPI _export B2EClearEnumMembersProc(VOID);
BOOL WINAPI _export B2ESetEnumMembersProc64(B2E_WND_ENUMMEMBPROC
		_lpEnumProc, DWORD _dwStructSize);
BOOL WINAPI _export B2EClearEnumMembersProc64(VOID);

#define STID_FONTGOTICK			101
#define STID_FONTPGOTICK		102
#define STID_REG_STANDARD		103

#define STID_B2E32DLG1		1001
#define STID_B2E32DLG2		1002
#define STID_B2E32DLG3		1003
#define STID_B2E32DLG4		1004

#define STID_GETFNAMEDLG		1011
#define STID_GETCMTDLG			1012
#define STID_GETWINSFXDLG		1013
#define STID_B2E32OWDLG		1014
#define STID_B2E32MKDIRDLG	1015

#define STID_DLG_T_PROC			1101
#define STID_DLG_P_ARCNAME		1102
#define STID_DLG_P_FILENAME		1103
#define STID_DLG_P_EXTRACTDIR	1104
#define STID_DLG_P_WRITESIZE	1105
#define STID_DLG_P_CANCEL		1106

#define STID_DLG_T_LOCALSETTING	1111
#define STID_DLG_P_HIGH_MATCH	1112
#define STID_DLG_P_MAKE_DIR		1113
#define STID_DLG_P_CHK_SPACE	1114
#define STID_DLG_P_TOTAL		1115
#define STID_DLG_P_MINI_DLG		1116
#define STID_DLG_P_FLUSH		1117
#define STID_DLG_P_OLD_LOG		1118
#define STID_DLG_P_OLD_GF		1119
#define STID_DLG_P_OK			1120
#define STID_DLG_P_MAPPEDFILE	1121

#define STID_DLG_T_OWDIALOG		1131
#define STID_DLG_P_OWGROUP		1132
#define STID_DLG_P_OWOVWRITE	1133
#define STID_DLG_P_OWNOTOW		1134
#define STID_DLG_P_OWALLOW		1135
#define STID_DLG_P_OWALLNOT		1136
#define STID_DLG_P_BREAK		1137

#define STID_DLG_T_CHGFNAME		1141
#define STID_DLG_P_FNAMETITLE	1142

#define STID_DLG_T_CDDIALOG		1151
#define STID_DLG_P_CDCREATE		1152
#define STID_DLG_P_CDNOTCREATE	1153
#define STID_DLG_P_CDALLCREATE	1154
#define STID_DLG_P_CDALLNOT		1155

#define STID_DLG_T_CMTDIALOG	1161

#define STID_DLG_T_WINSFXSET	1171
#define STID_DLG_P_WSWINTITLE	1172
#define STID_DLG_P_WSDIRTITLE	1173
#define STID_DLG_P_WSINITDIR	1174
#define STID_DLG_P_WSCOMMAND	1175
#define STID_DLG_P_WSSETATTR	1176
#define STID_DLG_P_WSSETPATH	1177
#define STID_DLG_P_WSIGNORETIME	1178
#define STID_DLG_P_WSAUTOOW		1179
#define STID_DLG_P_WSAUTORUN	1180
#define STID_DLG_P_WSTAMPERCHK	1181

#define STID_DLG_T_SETTING		1191
#define STID_DLG_P_SUBASEDIR	1192
#define STID_DLG_P_SUFORCE_E	1193
#define STID_DLG_P_SUSEL_DIR	1194
#define STID_DLG_P_SUABS_DIR	1195
#define STID_DLG_P_SUREL_DIR	1196
#define STID_DLG_P_SUSEL_OW		1197
#define STID_DLG_P_SUALW_OW		1198
#define STID_DLG_P_SUQUE_OW		1199
#define STID_DLG_P_SUNOT_OW		1200
#define STID_DLG_P_SUATR_MODE	1201
#define STID_DLG_P_SULOCAL_SET	1202
#define STID_DLG_P_SUSAVE_ENV	1203
#define STID_DLG_P_SUSEL_PATH	1204
#define STID_DLG_P_SUDNY_NO		1205
#define STID_DLG_P_SUDNY_PAR	1206
#define STID_DLG_P_SUDNY_ABS	1207

#define STID_DLG_T_BPDIALOG		1211
#define STID_DLG_P_BPCHGNAME	1212
#define STID_DLG_P_BPNOT		1213
#define STID_DLG_P_BPALLCHG		1214

#define STID_DLG_T_APDIALOG		1221
#define STID_DLG_P_APRELATIVE	1222
#define STID_DLG_P_APABS		1223
#define STID_DLG_P_APINVALID	1224
#define STID_DLG_T_TLDIALOG		1225
#define STID_DLG_T_EEDIALOG		1226
#define STID_DLG_P_EEEXTRACT	1227
#define STID_DLG_P_EEALLEXT		1228

#define STID_MES_T_B2E32_WAR	2001
#define STID_MES_T_B2E32_ERR	2002

#define STID_MES_T_PROC			2011
#define STID_MES_T_COMPRESS		2012
#define STID_MES_P_PROCFILE		2013
#define STID_MES_P_ADDFILE		2014
#define STID_MES_P_WORKFILE		2015

#define STID_MES_P_FILECOPY		2021

#define STID_MES_T_DELETE		2101
#define STID_MES_P_DELETE		2102
#define STID_MES_P_OVERWRITE	2104
#define STID_MES_T_ROWRITE		2105
#define STID_MES_P_ROWRITE		2106
#define STID_MES_P_CRCCHECK		2107
#define STID_MES_T_CRCERR		2108
#define STID_MES_P_CRCERR		2109
#define STID_MES_T_CRCERR2		2110
#define STID_MES_P_CRCERR2		2111
#define STID_MES_T_BADHUF		2112
#define STID_MES_P_BADHUF		2113
#define STID_MES_T_NAMECHANGE	2114
#define STID_MES_P_NAMECHANGE	2115
#define STID_MES_P_SHARE		2116
#define STID_MES_T_MAKEDIR		2117
#define STID_MES_P_MAKEDIR		2118
#define STID_MES_T_CONTINUE		2119
#define STID_MES_P_CONTINUE		2120
#define STID_MES_T_BADARC		2121
#define STID_MES_P_BADARC		2122
#define STID_MES_T_CONFIRM		2123
#define STID_MES_P_DICSIZ_WAR	2124
#define STID_MES_P_DISKSPACE	2125
#define STID_MES_T_SFXSET32		2126
#define STID_LOG_WAR_EXTDATA	2127
#define STID_LOG_WAR_SFX		2128
#define STID_MES_P_BADPATH		2129
#define STID_MES_P_ABSPATH		2130
#define STID_MES_P_TOOBIG		2131
#define STID_MES_P_EXECUTABLE	2132

#define STID_M_CANTOPEN			3001
#define STID_M_MAKEDIR			3002
#define STID_M_DIFFHOST			3003
#define STID_M_FEXISTS			3004
#define STID_M_FNEW				3005
#define STID_M_FOLD				3006
#define STID_M_FSAME			3007
#define STID_M_FNOEXIST			3008
#define STID_M_NOTSPCE			3009
#define STID_M_RDONLY			3010
#define STID_M_SPATTR			3011
#define STID_M_USKIP			3012
#define STID_M_WRITEER			3013
#define STID_M_DANNAME			3014
#define STID_M_SHARE			3015

#define STID_M_TOOLONG			3101
#define STID_M_TOOBIG			3102
#define STID_M_TOOLARGE			3102
#define STID_M_UNKNMETH			3103
#define STID_M_BADPATH			3104
#define STID_M_TOO_BIG2			3105
#define STID_M_EXEFILE			3106

#define STID_M_ERRORCNT			3201
#define STID_M_NOSPCL			3202

#define STID_ERR_8005			9001
#define STID_ERR_8006			9002
#define STID_ERR_8007			9003
#define STID_ERR_8008			9004
#define STID_ERR_8009			9005
#define STID_ERR_800A			9006
#define STID_ERR_800B			9007
#define STID_ERR_800C			9008
#define STID_ERR_800D			9009
#define STID_ERR_800E			9010
#define STID_ERR_800F			9011
#define STID_ERR_8010			9012
#define STID_ERR_8011			9013

#define STID_ERR_8012			9014
#define STID_ERR_8013			9015
#define STID_ERR_8014			9016
#define STID_ERR_8015			9017
#define STID_ERR_8016			9018
#define STID_ERR_8017			9019
#define STID_ERR_8018			9020
#define STID_ERR_8019			9021
#define STID_ERR_801A			9022
#define STID_ERR_801B			9023
#define STID_ERR_801C			9024
#define STID_ERR_801D			9025
#define STID_ERR_801E			9026
#define STID_ERR_801F			9027
#define STID_ERR_8020			9028
#define STID_ERR_8021			9029
#define STID_ERR_8022			9030
#define STID_ERR_8023			9031
#define STID_ERR_8024			9032
#define STID_ERR_8025			9033
#define STID_ERR_8026			9034
#define STID_ERR_8027			9035
#define STID_ERR_8028			9036
#define STID_ERR_8029			9037
#define STID_ERR_802A			9038
#define STID_ERR_802B			9039
#define STID_ERR_802C			9040
#define STID_ERR_802D			9041
#define STID_ERR_802E			9042
#define STID_ERR_802F			9043
#define STID_ERR_8030			9044
#define STID_ERR_8031			9045
#define STID_ERR_8032			9046
#define STID_ERR_8033			9047
#define STID_ERR_8034			9048
#define STID_ERR_8035			9049
#define STID_ERR_8036			9050
#define STID_ERR_8037			9051
#define STID_ERR_8038			9052
#define STID_ERR_8039			9053
#define STID_ERR_803A			9054
#define STID_ERR_803B			9055
#define STID_ERR_803C			9056
#define STID_ERR_803D			9057
#define STID_ERR_803E			9058
#define STID_ERR_803F			9059
#define STID_ERR_8040			9060
#define STID_ERR_8041			9061
#define STID_ERR_8042			9062
#define STID_ERR_8043			9063
#define STID_ERR_8044			9064
#define STID_ERR_8045			9065
#define STID_ERR_8046			9066
#define STID_ERR_8047			9067
#define STID_ERR_8048			9068
#define STID_ERR_8049			9069
#define STID_ERR_804A			9070
#define STID_ERR_804B			9071

/**************************/
/* 互換性のための別名関数 */
/**************************/

/* ### LHA.DLL Ver 1.1 と互換性のある API 群です。### */
/* LHA.DLL compatible API */

WORD WINAPI _export BtoEGetVersion(VOID);
BOOL WINAPI _export BtoEGetRunning(VOID);
BOOL WINAPI _export BtoEGetBackGroundMode(VOID);
BOOL WINAPI _export BtoESetBackGroundMode(const BOOL _BackGroundMode);
BOOL WINAPI _export BtoEGetCursorMode(VOID);
BOOL WINAPI _export BtoESetCursorMode(const BOOL _CursorMode);
WORD WINAPI _export BtoEGetCursorInterval(VOID);
BOOL WINAPI _export BtoESetCursorInterval(const WORD _Interval);
int WINAPI _export BtoE(const HWND _hwnd, LPCSTR _szCmdLine,
						LPSTR _szOutput, const DWORD _dwSize);
BOOL WINAPI _export BtoECheckArchive(LPCSTR _szFileName, const int _iMode);
int WINAPI _export BtoEGetFileCount(LPCSTR _szArcFile);
BOOL WINAPI _export BtoEConfigDialog(const HWND _hwnd, LPSTR _lpszComBuffer,
						const int _iMode);
BOOL WINAPI _export BtoEQueryFunctionList(const int _iFunction);
BOOL WINAPI _export BtoESetOwnerWindow(const HWND _hwnd);
BOOL WINAPI _export BtoEClearOwnerWindow(VOID);
BOOL WINAPI _export BtoESetOwnerWindowEx(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc);
BOOL WINAPI _export BtoEKillOwnerWindowEx(const HWND _hwnd);
BOOL WINAPI _export BtoESetOwnerWindowEx64(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize);
BOOL WINAPI _export BtoEKillOwnerWindowEx64(const HWND _hwnd);

/* OpenArchive 系 API です。 */
HARC WINAPI _export BtoEOpenArchive(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode);
HARC WINAPI _export BtoEOpenArchive2(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode, LPCSTR _szOption);
int WINAPI _export BtoECloseArchive(HARC _harc);
int WINAPI _export BtoEFindFirst(HARC _harc, LPCSTR _szWildName,
							INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export BtoEFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export BtoEGetArcFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export BtoEGetArcFileSize(HARC _harc);
DWORD WINAPI _export BtoEGetArcOriginalSize(HARC _harc);
DWORD WINAPI _export BtoEGetArcCompressedSize(HARC _harc);
BOOL WINAPI _export BtoEGetArcFileSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export BtoEGetArcOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export BtoEGetArcCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
DWORD WINAPI _export BtoEGetArcReadSize(HARC _harc);
BOOL WINAPI _export BtoEGetArcReadSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export BtoEGetArcRatio(HARC _harc);
WORD WINAPI _export BtoEGetArcDate(HARC _harc);
WORD WINAPI _export BtoEGetArcTime(HARC _harc);
BOOL WINAPI _export BtoEGetArcWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export BtoEGetArcCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export BtoEGetArcAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
UINT WINAPI _export BtoEGetArcOSType(HARC _harc);
int WINAPI _export BtoEIsSFXFile(HARC _harc);
int WINAPI _export BtoEGetFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
int WINAPI _export BtoEGetMethod(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export BtoEGetOriginalSize(HARC _harc);
DWORD WINAPI _export BtoEGetCompressedSize(HARC _harc);
BOOL WINAPI _export BtoEGetOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export BtoEGetCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export BtoEGetRatio(HARC _harc);
WORD WINAPI _export BtoEGetDate(HARC _harc);
WORD WINAPI _export BtoEGetTime(HARC _harc);
DWORD WINAPI _export BtoEGetWriteTime(HARC _harc);
DWORD WINAPI _export BtoEGetAccessTime(HARC _harc);
DWORD WINAPI _export BtoEGetCreateTime(HARC _harc);
BOOL WINAPI _export BtoEGetWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export BtoEGetCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export BtoEGetAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
DWORD WINAPI _export BtoEGetCRC(HARC _harc);
int WINAPI _export BtoEGetAttribute(HARC _harc);
int WINAPI _export BtoEGetAttributes(HARC _harc);
UINT WINAPI _export BtoEGetOSType(HARC _harc);
WORD WINAPI _export BtoEGetSubVersion(VOID);
int WINAPI _export BtoEExtractMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize,
		time_t *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize);
int WINAPI _export BtoECompressMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize,
		const time_t *_lpTime, const LPWORD _lpwAttr,
		LPDWORD _lpdwWriteSize);
BOOL WINAPI _export BtoESetEnumMembersProc(B2E_WND_ENUMMEMBPROC
		_lpEnumProc);
BOOL WINAPI _export BtoEClearEnumMembersProc(VOID);
BOOL WINAPI _export BtoESetEnumMembersProc64(B2E_WND_ENUMMEMBPROC
		_lpEnumProc, DWORD _dwStructSize);
BOOL WINAPI _export BtoEClearEnumMembersProc64(VOID);

/**********************************************
B2E32.DLL独自API
**********************************************/

/*B2Eスクリプトのあるディレクトリを指定*/
BOOL WINAPI _export B2ESetScriptDirectory(LPCSTR _szDirectory);

/*B2Eスクリプトの数を取得*/
int WINAPI _export B2EScriptGetCount();

/*B2Eスクリプト名を取得:
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
*/
BOOL WINAPI _export B2EScriptGetName(const UINT _nIndex,LPSTR _szName,const DWORD _dwSize);

/*
B2Eスクリプトの能力を列挙:
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _lpAbility:能力
*/
#define ABILITY_CHECK		1		/*CheckArchive()相当の処理が可能*/
#define ABILITY_MELT		2		/*解凍処理が可能*/
#define ABILITY_LIST		4		/*書庫内ファイルの列挙が可能*/
#define ABILITY_MELT_EACH	8		/*指定したファイルのみの解凍が可能*/
#define ABILITY_COMPRESS	16		/*圧縮が可能*/
#define ABILITY_ARCHIVE		32		/*複数ファイルをまとめることが可能(cf.GZip)*/
#define ABILITY_SFX			64		/*自己解凍ファイルを作成可能*/
#define ABILITY_ADD			128		/*ファイルの追加が可能*/
#define ABILITY_DELETE		256		/*ファイルの削除が可能*/


BOOL WINAPI _export B2EScriptGetAbility(const UINT _nIndex,LPWORD _lpAbility);

/*
解凍できる拡張子('.'付き)の_nExtIndex番目のものを取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _nExtIndex:対応拡張子のインデックス(0オリジン)
*/
BOOL WINAPI _export B2EScriptGetExtractExtensions(const UINT _nIndex,const UINT _nExtIndex,LPSTR _szExt,const DWORD _dwSize);

/*
B2Eの圧縮対応形式を取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _szType:形式名
*/
BOOL WINAPI _export B2EScriptGetCompressType(const UINT _nIndex,LPSTR _szType,const DWORD _dwSize);

/*
圧縮用B2Eの対応メソッドを取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _nMthdIndex:メソッドの番号
  _szMethod:メソッド名
*/
BOOL WINAPI _export B2EScriptGetCompressMethod(const UINT _nIndex,const UINT _nMthdIndex,LPSTR _szMethod,const DWORD _dwSize);

/*
圧縮用B2Eのデフォルトメソッド番号を取得
  _nIndex:B2Eスクリプトを指定するインデックス(0オリジン)
  _szMethod:メソッド名
*/
int WINAPI _export B2EScriptGetDefaultCompressMethod(const UINT _nIndex);

/*
  指定したアーカイブファイルの解凍に使うB2Eのインデックスを取得
    _szFileName  チェックすべきアーカイブファイルのファイル名
戻り値
    非負                 B2Eスクリプトのインデックス
    0xFFFFFFFF           エラー、もしくは現在利用可能なB2Eスクリプトで扱えないファイル
*/
UINT WINAPI _export B2EScriptGetExtractorIndex(LPCSTR _szFileName);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif	/* __B2E32_H__ */
