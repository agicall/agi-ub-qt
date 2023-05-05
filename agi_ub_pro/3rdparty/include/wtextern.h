#ifndef __WTEXTERN_H__
#define __WTEXTERN_H__

/*
#ifdef WT_EXPORTS
#define WT_DECL __declspec(dllexport)
#else
#define WT_DECL __declspec(dllimport)
#endif

*/


#if defined(_MSC_VER)

	#define AGI_API				__stdcall//WINAPI
	#define AGI_CALLBACK		__stdcall

#elif __APPLE__  

	#define AGI_API  
	#define AGI_CALLBACK

#elif __ANDROID__ 

	#define AGI_API
	#define AGI_CALLBACK

#elif __GNUC__ >= 4

	#define AGI_API			__attribute__((visibility("default")))
	#define AGI_CALLBACK	__attribute__((visibility("default")))

#else

	#define AGI_API 
	#define AGI_CALLBACK

#endif


#define WT_API		AGI_API


#ifndef __has_declspec_attribute
#define __has_declspec_attribute(x) 0
#endif

#ifdef AGI_STATICLIB
#  define CURL_EXTERN
#elif defined(WIN32) || defined(__SYMBIAN32__) || \
     (__has_declspec_attribute(dllexport) && \
      __has_declspec_attribute(dllimport))
#  if defined(BUILDING_LIBAGI)
#    define AGI_EXTERN  __declspec(dllexport)
#  else
#    define AGI_EXTERN  __declspec(dllimport)
#  endif
#elif defined(BUILDING_LIBAGI) && defined(AGI_HIDDEN_SYMBOLS)
#  define AGI_EXTERN AGI_EXTERN_SYMBOL
#else
#  define AGI_EXTERN
#endif

//================================================================
//成功
#define		ERR_FAILED			-1
#define		ERR_NO				0
#define		ERR_OK				ERR_NO
#define		ERR_PENDING			70000001
#define		ERR_END				70000002

//啥都没干
#define		ERR_IGNORE			80000000
#define		ERR_NOMEMORY		80000001
//没有初始化指针内容
#define		ERR_NOINIT			80000002
//参数内容不合法
#define		ERR_INVALID_PARAM	80000003
#define		ERR_INVALID_DEV		80000004
#define		ERR_INVALID_ID		80000005
#define		ERR_INVALID_CMD		80000008
#define		ERR_INVALID_HANDLE	80000009
#define		ERR_INVALID_TOKEN	80000010
#define		ERR_INVALID_FILE	80000011
#define		ERR_INVALID_CODE	80000012
#define		ERR_INVALID_TIME	80000013
#define		ERR_INVALID_DATA	80000014
#define		ERR_INVALID_AUTH	80000015
#define		ERR_INVALID_PWD		80000016
#define		ERR_INVALID_ACC		80000017
#define		ERR_INVALID_KEY		80000018
#define		ERR_INVALID_CRC		80000019

#define		ERR_OVERFLOW		80000100
#define		ERR_NORES			80000101
#define		ERR_EMPTY			80000102
#define		ERR_TIMETOUT		80000103
#define		ERR_EXEC			80000104
#define		ERR_BUSY			80000105
//======================================================


#ifdef __cplusplus
#define WT_BEGIN_C extern "C" {
#define WT_END_C }
#else
#define WT_BEGIN_C
#define WT_END_C
#endif

//========================================================
//#define		int64_t			int64
#if defined(_MSC_VER)

typedef wchar_t             wchar;
typedef unsigned char		uchar;
typedef char*				pchar;
typedef unsigned short		ushort;
typedef short				int16;
typedef unsigned short		uint16;
typedef int					int32;
typedef unsigned int		uint32;
typedef __int64				int64;
typedef unsigned __int64	uint64;

typedef short				SHORT;
typedef ushort				USHORT;
typedef short				INT16;
typedef ushort				UINT16;

typedef uint32				UINT32;
typedef int64				INT64;
typedef uint64				UINT64;


#ifndef YES
#define T_YES    (BOOL)1
#endif

#ifndef NO
#define T_NO    (BOOL)0
#endif

//不能修改,shared_ptr返回用时有问题
//
//#ifndef nullptr
//#define nullptr NULL
//#endif

#else //======================================

#ifndef NULL
#define NULL	0
#endif

//#ifndef true
//#define true 1
//#endif
//
//#ifndef false
//#define false 0
//#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif


#ifndef YES
#define YES    (BOOL)true
#endif

#ifndef NO
#define NO    (BOOL)false
#endif


#ifndef  MAX_PATH
#define  MAX_PATH 256
#endif // ! MAX_PATH



typedef wchar_t             wchar;
typedef unsigned char		uchar;
typedef char*				pchar;
typedef  unsigned char      BYTE;
typedef unsigned short		ushort;
typedef short				int16;
typedef unsigned short		uint16;
typedef int					int32;
typedef unsigned int		uint32;
typedef long long			int64;
typedef unsigned long long  uint64;

typedef unsigned char		UCHAR;
#define		BOOL			uint32

typedef void*				HANDLE;
typedef void*				LPVOID;
typedef ushort              WORD;
typedef unsigned int        DWORD;
typedef short				SHORT;
typedef ushort				USHORT;
typedef short				INT16;
typedef ushort				UINT16;

typedef uint32              UINT;
typedef uint32				UINT32;
typedef int64				INT64;
typedef unsigned long long	UINT64;


typedef int                 SOCKET;

typedef  const char *       LPCTSTR;
typedef  const wchar_t *    LPWSTR;
typedef  const char *       LPCSTR;
typedef  char*              LPTSTR;

#endif



#endif

