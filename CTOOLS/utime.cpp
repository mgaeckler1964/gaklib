/*
		Project:		GAKLIB
		Module:			utime.cpp
		Description:	replacement for buggy utime
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <errno.h>
#include <time.h>

#if defined( _MSC_VER )
#include <sys/utime.h>
#else
#include <utime.h>
#endif

#ifdef _Windows
#	ifndef STRICT
#		define STRICT	1
#	endif
#	include <windows.h>
#endif

#include <gak/gaklib.h>
#include <gak/datetime.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

#if defined( __BORLANDC__ )
extern "C" int __NTerror(void);
#elif defined(_MSC_VER )

#define Exxx    EINVAL  /* NT errors that are unused or don't map to Unix */

static char  _dosErrorToSV [] =
{
    0,
    EINVAL,     /* ERROR_INVALID_FUNCTION            1 */
    ENOENT,     /* ERROR_FILE_NOT_FOUND              2 */
    ENOENT,     /* ERROR_PATH_NOT_FOUND              3 */
    EMFILE,     /* ERROR_TOO_MANY_OPEN_FILES         4 */
    EACCES,     /* ERROR_ACCESS_DENIED               5 */
    EBADF,      /* ERROR_INVALID_HANDLE              6 */
    ENOMEM,     /* ERROR_ARENA_TRASHED               7 */
    ENOMEM,     /* ERROR_NOT_ENOUGH_MEMORY           8 */
    ENOMEM,     /* ERROR_INVALID_BLOCK               9 */
    E2BIG,      /* ERROR_BAD_ENVIRONMENT            10 */
    ENOEXEC,    /* ERROR_BAD_FORMAT                 11 */
    EINVAL,     /* ERROR_INVALID_ACCESS             12 */
    EINVAL,     /* ERROR_INVALID_DATA               13 */
    EFAULT,     /*                                  14 */
    ENOENT,     /* ERROR_INVALID_DRIVE              15 */
    EACCES,     /* ERROR_CURRENT_DIRECTORY          16 */
    EXDEV,      /* ERROR_NOT_SAME_DEVICE            17 */
    ENOENT,     /* ERROR_NO_MORE_FILES              18 */
    EROFS,      /* ERROR_WRITE_PROTECT              19 */
    ENXIO,      /* ERROR_BAD_UNIT                   20 */
    EBUSY,      /* ERROR_NOT_READY                  21 */
    EIO,        /* ERROR_BAD_COMMAND                22 */
    EIO,        /* ERROR_CRC                        23 */
    EIO,        /* ERROR_BAD_LENGTH                 24 */
    EIO,        /* ERROR_SEEK                       25 */
    EIO,        /* ERROR_NOT_DOS_DISK               26 */
    ENXIO,      /* ERROR_SECTOR_NOT_FOUND           27 */
    EBUSY,      /* ERROR_OUT_OF_PAPER               28 */
    EIO,        /* ERROR_WRITE_FAULT                29 */
    EIO,        /* ERROR_READ_FAULT                 30 */
    EIO,        /* ERROR_GEN_FAILURE                31 */
    EACCES,     /* ERROR_SHARING_VIOLATION          32 */
    EACCES,     /* ERROR_LOCK_VIOLATION             33 */
    ENXIO,      /* ERROR_WRONG_DISK                 34 */
    ENFILE,     /* ERROR_FCB_UNAVAILABLE            35 */
    ENFILE,     /* ERROR_SHARING_BUFFER_EXCEEDED    36 */
    EFAULT,     /* ERROR_CODE_PAGE_MISMATCHED       37 */
    EFAULT,     /* ERROR_HANDLE_EOF                 38 */
    EFAULT,     /* ERROR_HANDLE_DISK_FULL           39 */
    EFAULT,     /*%msg%ERROR_BAD_COMMAND            40 */
    EFAULT,     /*%msg%ERROR_CRC                    41 */
    EFAULT,     /*%msg%ERROR_BAD_LENGTH             42 */
    EFAULT,     /*%msg%ERROR_SEEK                   43 */
    EFAULT,     /*%msg%ERROR_NOT_DOS_DISK           44 */
    EFAULT,     /*%msg%ERROR_SECTOR_NOT_FOUND       45 */
    EFAULT,     /*%msg%ERROR_OUT_OF_PAPER           46 */
    EFAULT,     /*%msg%ERROR_WRITE_FAULT            47 */
    EFAULT,     /*%msg%ERROR_READ_FAULT             48 */
    EFAULT,     /*%msg%ERROR_GEN_FAILURE            49 */
    ENODEV,     /* ERROR_NOT_SUPPORTED              50 */
    EBUSY,      /* ERROR_REM_NOT_LIST               51 */
    EEXIST,     /* ERROR_DUP_NAME                   52 */
    ENOENT,     /* ERROR_BAD_NETPATH                53 */
    EBUSY,      /* ERROR_NETWORK_BUSY               54 */
    ENODEV,     /* ERROR_DEV_NOT_EXIST              55 */
    EAGAIN,     /* ERROR_TOO_MANY_CMDS              56 */
    EIO,        /* ERROR_ADAP_HDW_ERR               57 */
    EIO,        /* ERROR_BAD_NET_RESP               58 */
    EIO,        /* ERROR_UNEXP_NET_ERR              59 */
    EINVAL,     /* ERROR_BAD_REM_ADAP               60 */
    EFBIG,      /* ERROR_PRINTQ_FULL                61 */
    ENOSPC,     /* ERROR_NO_SPOOL_SPACE             62 */
    ENOENT,     /* ERROR_PRINT_CANCELLED            63 */
    ENOENT,     /* ERROR_NETNAME_DELETED            64 */
    EACCES,     /* ERROR_NETWORK_ACCESS_DENIED      65 */
    ENODEV,     /* ERROR_BAD_DEV_TYPE               66 */
    ENOENT,     /* ERROR_BAD_NET_NAME               67 */
    ENFILE,     /* ERROR_TOO_MANY_NAMES             68 */
    EIO,        /* ERROR_TOO_MANY_SESS              69 */
    EAGAIN,     /* ERROR_SHARING_PAUSED             70 */
    EINVAL,     /* ERROR_REQ_NOT_ACCEP              71 */
    EAGAIN,     /* ERROR_REDIR_PAUSED               72 */
    EFAULT,     /* ERROR_SBCS_ATT_WRITE_PROT        73 */
    EFAULT,     /* ERROR_SBCS_GENERAL_FAILURE       74 */
    EFAULT,     /* ERROR_XGA_OUT_MEMORY             75 */
    EFAULT,     /*                                  76 */
    EFAULT,     /*                                  77 */
    EFAULT,     /*                                  78 */
    EFAULT,     /*                                  79 */
    EEXIST,     /* ERROR_FILE_EXISTS                80 */
    EFAULT,     /* ERROR_DUP_FCB                    81 */
    EACCES,     /* ERROR_CANNOT_MAKE                82 */
    EACCES,     /* ERROR_FAIL_I24                   83 */
    ENFILE,     /* ERROR_OUT_OF_STRUCTURES          84 */
    EEXIST,     /* ERROR_ALREADY_ASSIGNED           85 */
    EPERM,      /* ERROR_INVALID_PASSWORD           86 */
    EINVAL,     /* ERROR_INVALID_PARAMETER          87 */
    EIO,        /* ERROR_NET_WRITE_FAULT            88 */
    EAGAIN,     /* ERROR_NO_PROC_SLOTS              89 */
    Exxx,       /* ERROR_NOT_FROZEN                 90 */
    Exxx,       /* ERR_TSTOVFL                      91 */
    Exxx,       /* ERR_TSTDUP                       92 */
    Exxx,       /* ERROR_NO_ITEMS                   93 */
    Exxx,       /*                                  94 */
    Exxx,       /* ERROR_INTERRUPT                  95 */
    Exxx,       /*                                  96 */
    Exxx,       /*                                  97 */
    Exxx,       /*                                  98 */
    EBUSY,      /* ERROR_DEVICE_IN_USE              99 */
    EAGAIN,     /* ERROR_TOO_MANY_SEMAPHORES       100 */
    EAGAIN,     /* ERROR_EXCL_SEM_ALREADY_OWNED    101 */
    Exxx,       /* ERROR_SEM_IS_SET                102 */
    Exxx,       /* ERROR_TOO_MANY_SEM_REQUESTS     103 */
    Exxx,       /* ERROR_INVALID_AT_INTERRUPT_TIME 104 */
    Exxx,       /* ERROR_SEM_OWNER_DIED            105 */
    Exxx,       /* ERROR_SEM_USER_LIMIT            106 */
    EXDEV,      /* ERROR_DISK_CHANGE               107 */
    EACCES,     /* ERROR_DRIVE_LOCKED              108 */
    EPIPE,      /* ERROR_BROKEN_PIPE               109 */
    ENOENT,     /* ERROR_OPEN_FAILED               110 */
    Exxx,       /* ERROR_BUFFER_OVERFLOW           111 */
    ENOSPC,     /* ERROR_DISK_FULL                 112 */
    EMFILE,     /* ERROR_NO_MORE_SEARCH_HANDLES    113 */
    EBADF,      /* ERROR_INVALID_TARGET_HANDLE     114 */
    EFAULT,     /* ERROR_PROTECTION_VIOLATION      115 */
    Exxx,       /* ERROR_VIOKBD_REQUEST            116 */
    Exxx,       /* ERROR_INVALID_CATEGORY          117 */
    Exxx,       /* ERROR_INVALID_VERIFY_SWITCH     118 */
    Exxx,       /* ERROR_BAD_DRIVER_LEVEL          119 */
    Exxx,       /* ERROR_CALL_NOT_IMPLEMENTED      120 */
    Exxx,       /* ERROR_SEM_TIMEOUT               121 */
    Exxx,       /* ERROR_INSUFFICIENT_BUFFER       122 */
    ENOENT,     /* ERROR_INVALID_NAME              123 */
    EINVAL,     /* ERROR_INVALID_LEVEL             124 */
    Exxx,       /* ERROR_NO_VOLUME_LABEL           125 */
    Exxx,       /* ERROR_MOD_NOT_FOUND             126 */
    ESRCH,      /* ERROR_PROC_NOT_FOUND            127 */
    ECHILD,     /* ERROR_WAIT_NO_CHILDREN          128 */
    ECHILD,     /* ERROR_CHILD_NOT_COMPLETE        129 */
    EBADF,      /* ERROR_DIRECT_ACCESS_HANDLE      130 */
    EINVAL,     /* ERROR_NEGATIVE_SEEK             131 */
    EACCES,     /* ERROR_SEEK_ON_DEVICE            132 */
    Exxx,       /* ERROR_IS_JOIN_TARGET            133 */
    Exxx,       /* ERROR_IS_JOINED                 134 */
    Exxx,       /* ERROR_IS_SUBSTED                135 */
    Exxx,       /* ERROR_NOT_JOINED                136 */
    Exxx,       /* ERROR_NOT_SUBSTED               137 */
    Exxx,       /* ERROR_JOIN_TO_JOIN              138 */
    Exxx,       /* ERROR_SUBST_TO_SUBST            139 */
    Exxx,       /* ERROR_JOIN_TO_SUBST             140 */
    Exxx,       /* ERROR_SUBST_TO_JOIN             141 */
    EAGAIN,     /* ERROR_BUSY_DRIVE                142 */
    Exxx,       /* ERROR_SAME_DRIVE                143 */
    Exxx,       /* ERROR_DIR_NOT_ROOT              144 */
    ENOTEMPTY,  /* ERROR_DIR_NOT_EMPTY             145 */
    Exxx,       /* ERROR_IS_SUBST_PATH             146 */
    Exxx,       /* ERROR_IS_JOIN_PATH              147 */
    Exxx,       /* ERROR_PATH_BUSY                 148 */
    Exxx,       /* ERROR_IS_SUBST_TARGET           149 */
    Exxx,       /* ERROR_SYSTEM_TRACE              150 */
    Exxx,       /* ERROR_INVALID_EVENT_COUNT       151 */
    Exxx,       /* ERROR_TOO_MANY_MUXWAITERS       152 */
    Exxx,       /* ERROR_INVALID_LIST_FORMAT       153 */
    Exxx,       /* ERROR_LABEL_TOO_LONG            154 */
    Exxx,       /* ERROR_TOO_MANY_TCBS             155 */
    Exxx,       /* ERROR_SIGNAL_REFUSED            156 */
    Exxx,       /* ERROR_DISCARDED                 157 */
    EACCES,     /* ERROR_NOT_LOCKED                158 */
    Exxx,       /* ERROR_BAD_THREADID_ADDR         159 */
    Exxx,       /* ERROR_BAD_ARGUMENTS             160 */
    ENOENT,     /* ERROR_BAD_PATHNAME              161 */
    Exxx,       /* ERROR_SIGNAL_PENDING            162 */
    Exxx,       /* ERROR_UNCERTAIN_MEDIA           163 */
    EAGAIN,     /* ERROR_MAX_THRDS_REACHED         164 */
    Exxx,       /* ERROR_MONITORS_NOT_SUPPORTED    165 */
    Exxx,       /* ERROR_UNC_DRIVER_NOT_INSTALLED  166 */
    EACCES,     /* ERROR_LOCK_FAILED               167 */
    Exxx,       /* ERROR_SWAPIO_FAILED             168 */
    Exxx,       /* ERROR_SWAPIN_FAILED             169 */
    Exxx,       /* ERROR_BUSY                      170 */
    Exxx,       /*                                 171 */
    Exxx,       /*                                 172 */
    Exxx,       /* ERROR_CANCEL_VIOLATION          173 */
    Exxx,       /* ERROR_ATOMIC_LOCK_NOT_SUPPORTED 174 */
    Exxx,       /* ERROR_READ_LOCKS_NOT_SUPPORTED  175 */
    Exxx,       /*                                 176 */
    Exxx,       /*                                 177 */
    Exxx,       /*                                 178 */
    Exxx,       /*                                 179 */
    Exxx,       /* ERROR_INVALID_SEGMENT_NUMBER    180 */
    Exxx,       /* ERROR_INVALID_CALLGATE          181 */
    Exxx,       /* ERROR_INVALID_ORDINAL           182 */
    EEXIST,     /* ERROR_ALREADY_EXISTS            183 */
    ECHILD,     /* ERROR_NO_CHILD_PROCESS          184 */
    Exxx,       /* ERROR_CHILD_ALIVE_NOWAIT        185 */
    Exxx,       /* ERROR_INVALID_FLAG_NUMBER       186 */
    Exxx,       /* ERROR_SEM_NOT_FOUND             187 */
    Exxx,       /* ERROR_INVALID_STARTING_CODESEG  188 */
    Exxx,       /* ERROR_INVALID_STACKSEG          189 */
    Exxx,       /* ERROR_INVALID_MODULETYPE        190 */
    Exxx,       /* ERROR_INVALID_EXE_SIGNATURE     191 */
    Exxx,       /* ERROR_EXE_MARKED_INVALID        192 */
    Exxx,       /* ERROR_BAD_EXE_FORMAT            193 */
    Exxx,       /* ERROR_ITERATED_DATA_EXCEEDS_64k 194 */
    Exxx,       /* ERROR_INVALID_MINALLOCSIZE      195 */
    Exxx,       /* ERROR_DYNLINK_FROM_INVALID_RING 196 */
    Exxx,       /* ERROR_IOPL_NOT_ENABLED          197 */
    Exxx,       /* ERROR_INVALID_SEGDPL            198 */
    Exxx,       /* ERROR_AUTODATASEG_EXCEEDS_64k   199 */
    Exxx,       /* ERROR_RING2SEG_MUST_BE_MOVABLE  200 */
    Exxx,       /* ERROR_RELOC_CHAIN_XEEDS_SEGLIM  201 */
    Exxx,       /* ERROR_INFLOOP_IN_RELOC_CHAIN    202 */
    Exxx,       /* ERROR_ENVVAR_NOT_FOUND          203 */
    Exxx,       /* ERROR_NOT_CURRENT_CTRY          204 */
    Exxx,       /* ERROR_NO_SIGNAL_SENT            205 */
    ENOENT,     /* ERROR_FILENAME_EXCED_RANGE      206 */
    Exxx,       /* ERROR_RING2_STACK_IN_USE        207 */
    Exxx,       /* ERROR_META_EXPANSION_TOO_LONG   208 */
    Exxx,       /* ERROR_INVALID_SIGNAL_NUMBER     209 */
    Exxx,       /* ERROR_THREAD_1_INACTIVE         210 */
    Exxx,       /* ERROR_INFO_NOT_AVAIL            211 */
    Exxx,       /* ERROR_LOCKED                    212 */
    Exxx,       /* ERROR_BAD_DYNALINK              213 */
    Exxx,       /* ERROR_TOO_MANY_MODULES          214 */
    EAGAIN,     /* ERROR_NESTING_NOT_ALLOWED       215 */
    Exxx,       /* ERROR_CANNOT_SHRINK             216 */
    Exxx,       /* ERROR_ZOMBIE_PROCESS            217 */
    Exxx,       /* ERROR_STACK_IN_HIGH_MEMORY      218 */
    Exxx,       /* ERROR_INVALID_EXITROUTINE_RING  219 */
    Exxx,       /* ERROR_GETBUF_FAILED             220 */
    Exxx,       /* ERROR_FLUSHBUF_FAILED           221 */
    Exxx,       /* ERROR_TRANSFER_TOO_LONG         222 */
    Exxx,       /* ERROR_FORCENOSWAP_FAILED        223 */
    Exxx,       /* ERROR_SMG_NO_TARGET_WINDOW      224 */
    Exxx,       /*                                 225 */
    Exxx,       /*                                 226 */
    Exxx,       /*                                 227 */
    Exxx,       /* ERROR_NO_CHILDREN               228 */
    Exxx,       /* ERROR_INVALID_SCREEN_GROUP      229 */
    EPIPE,      /* ERROR_BAD_PIPE                  230 */
    EAGAIN,     /* ERROR_PIPE_BUSY                 231 */
    Exxx,       /* ERROR_NO_DATA                   232 */
    EPIPE,      /* ERROR_PIPE_NOT_CONNECTED        233 */
    Exxx,       /* ERROR_MORE_DATA                 234 */
    Exxx,       /*                                 235 */
    Exxx,       /*                                 236 */
    Exxx,       /*                                 237 */
    Exxx,       /*                                 238 */
    Exxx,       /*                                 239 */
    Exxx,       /* ERROR_VC_DISCONNECTED           240 */
    Exxx,       /*                                 241 */
    Exxx,       /*                                 242 */
    Exxx,       /*                                 243 */
    Exxx,       /*                                 244 */
    Exxx,       /*                                 245 */
    Exxx,       /*                                 246 */
    Exxx,       /*                                 247 */
    Exxx,       /*                                 248 */
    Exxx,       /*                                 249 */
    Exxx,       /* ERROR_CIRCULARITY_REQUESTED     250 */
    Exxx,       /* ERROR_DIRECTORY_IN_CDS          251 */
    Exxx,       /* ERROR_INVALID_FSD_NAME          252 */
    Exxx,       /* ERROR_INVALID_PATH              253 */
    Exxx,       /* ERROR_INVALID_EA_NAME           254 */
    Exxx,       /* ERROR_EA_LIST_INCONSISTENT      255 */
    Exxx,       /* ERROR_EA_LIST_TOO_LONG          256 */
    Exxx,       /* ERROR_NO_META_MATCH             257 */
    Exxx,       /* ERROR_FINDNOTIFY_TIMEOUT        258 */
    Exxx,       /* ERROR_NO_MORE_ITEMS             259 */
    Exxx,       /* ERROR_SEARCH_STRUC_REUSED       260 */
    Exxx,       /* ERROR_CHAR_NOT_FOUND            261 */
    Exxx,       /* ERROR_TOO_MUCH_STACK            262 */
    Exxx,       /* ERROR_INVALID_ATTR              263 */
    Exxx,       /* ERROR_INVALID_STARTING_RING     264 */
    Exxx,       /* ERROR_INVALID_DLL_INIT_RING     265 */
    Exxx,       /* ERROR_CANNOT_COPY               266 */
    Exxx,       /* ERROR_DIRECTORY                 267 */
    Exxx,       /* ERROR_OPLOCKED_FILE             268 */
    Exxx,       /* ERROR_OPLOCK_THREAD_EXISTS      269 */
    Exxx,       /* ERROR_VOLUME_CHANGED            270 */
    Exxx,       /* ERROR_FINDNOTIFY_HANDLE_IN_USE  271 */
    Exxx,       /* ERROR_FINDNOTIFY_HANDLE_CLOSED  272 */
    Exxx,       /* ERROR_NOTIFY_OBJECT_REMOVED     273 */
    Exxx,       /* ERROR_ALREADY_SHUTDOWN          274 */
    Exxx,       /* ERROR_EAS_DIDNT_FIT             275 */
    Exxx,       /* ERROR_EA_FILE_CORRUPT           276 */
    Exxx,       /* ERROR_EA_TABLE_FULL             277 */
    Exxx,       /* ERROR_INVALID_EA_HANDLE         278 */
    Exxx,       /* ERROR_NO_CLUSTER                279 */
    Exxx,       /* ERROR_CREATE_EA_FILE            280 */
    Exxx,       /* ERROR_CANNOT_OPEN_EA_FILE       281 */
    Exxx,       /* ERROR_EAS_NOT_SUPPORTED         282 */
    Exxx,       /* ERROR_NEED_EAS_FOUND            283 */
    Exxx,       /* ERROR_DUPLICATE_HANDLE          284 */
    Exxx,       /* ERROR_DUPLICATE_NAME            285 */
    Exxx,       /* ERROR_EMPTY_MUXWAIT             286 */
    Exxx,       /* ERROR_MUTEX_OWNED               287 */
    Exxx,       /* ERROR_NOT_OWNER                 288 */
    Exxx,       /* ERROR_PARAM_TOO_SMALL           289 */
    Exxx,       /* ERROR_TOO_MANY_HANDLES          290 */
    Exxx,       /* ERROR_TOO_MANY_OPENS            291 */
    Exxx,       /* ERROR_WRONG_TYPE                292 */
    Exxx,       /* ERROR_UNUSED_CODE               293 */
    Exxx,       /* ERROR_THREAD_NOT_TERMINATED     294 */
    Exxx,       /* ERROR_INIT_ROUTINE_FAILED       295 */
    Exxx,       /* ERROR_MODULE_IN_USE             296 */
    Exxx,       /* ERROR_NOT_ENOUGH_WATCHPOINTS    297 */
    Exxx,       /* ERROR_TOO_MANY_POSTS            298 */
};

#define LAST_ERROR (sizeof(_dosErrorToSV)-1)

static int __IOerror( unsigned dosErr )
{
    int     val = dosErr;

    _doserrno = dosErr;
    if (val <= LAST_ERROR )
	{
	    errno = _dosErrorToSV[dosErr];
	}
	else
	{
	    _doserrno = ERROR_INVALID_FUNCTION;
		errno = Exxx;
	}

    return -1;
}

static inline int __NTerror(void)
{
    return __IOerror( unsigned( GetLastError() ) & 0xffff );
}
#endif

static inline void unixtofile( time_t tim, FILETIME *ftp )
{
	DateTime	theTime( tim );

	theTime.getFileTime( ftp );
}

static int doUtime( HANDLE handle, struct utimbuf *times )
{
	bool		err;

	SYSTEMTIME	st;
	FILETIME	fta;       /* access time */
	FILETIME	ftm;       /* modification time */

	if( handle != INVALID_HANDLE_VALUE )
	{
		/* If times not supplied, use current time.
		 */
		if (times == NULL)
		{
			GetLocalTime(&st);
			SystemTimeToFileTime(&st,&fta);
			ftm = fta;
		}
		else
		{
			/* Convert supplied modification and access times to NT file times.
			 */
			unixtofile(times->actime, &fta);
			unixtofile(times->modtime, &ftm);
		}

		/* Set the file time and date, close the file.
		 */
		err = (SetFileTime(handle,&ftm,&fta,&ftm) == FALSE);

		CloseHandle(handle);
	}
	else
		err = true;

	return (err ? __NTerror() : 0);
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

extern "C"
{

int utime( const char *filename, struct utimbuf *times )
{
	HANDLE handle = CreateFileA(
		filename, GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL
	);
	return doUtime( handle, times );
}

extern "C" int _wutime( const wchar_t *filename, struct utimbuf *times )
{
	HANDLE handle = CreateFileW(
		filename, GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL
	);
	return doUtime( handle, times );
}

}	// extern "C"
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

