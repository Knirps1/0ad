#include "precompiled.h"

#include <unistd.h>
#include <stdio.h>
#include <wchar.h>

#include "lib.h"
#include "sysdep/sysdep.h"
#include "udbg.h"

#define GNU_SOURCE
#include <dlfcn.h>

// these are basic POSIX-compatible backends for the sysdep.h functions.
// Win32 has better versions which override these.

void sys_display_msg(const char* caption, const char* msg)
{
	fprintf(stderr, "%s: %s\n", caption, msg);
}

void sys_display_msgw(const wchar_t* caption, const wchar_t* msg)
{
	fwprintf(stderr, L"%ls: %ls\n", caption, msg);
}


int get_executable_name(char* n_path, size_t buf_size)
{
	Dl_info dl_info;

	memset(&dl_info, 0, sizeof(dl_info));
	if (!dladdr((void *)get_executable_name, &dl_info) ||
		!dl_info.dli_fname )
	{
		return -ENOSYS;
	}

	strncpy(n_path, dl_info.dli_fname, buf_size);
	return 0;
}

extern int cpus;
int unix_get_cpu_info()
{
	long res = sysconf(_SC_NPROCESSORS_CONF);
	if (res == -1)
		cpus = 1;
	else
		cpus = (int)res;
	return 0;
}

// apparently not possible on non-Windows OSes because they seem to lack
// a CPU affinity API. see sysdep.h comment.
int sys_on_each_cpu(void(*cb)())
{
	return ERR_NO_SYS;
}

ErrorReaction sys_display_error(const wchar_t* text, int flags)
{
	printf("%ls\n\n", text);

	const bool manual_break   = flags & DE_MANUAL_BREAK;
	const bool allow_suppress = flags & DE_ALLOW_SUPPRESS;
	const bool no_continue    = flags & DE_NO_CONTINUE;

	// until valid input given:
	for(;;)
	{
		if(!no_continue)
			printf("(C)ontinue, ");
		if(allow_suppress)
			printf("(S)uppress, ");
		printf("(B)reak, Launch (D)ebugger, or (E)xit?\n");
		// TODO Should have some kind of timeout here.. in case you're unable to
		// access the controlling terminal (As might be the case if launched
		// from an xterm and in full-screen mode)
		int c = getchar();
		// note: don't use tolower because it'll choke on EOF
		switch(c)
		{
		case EOF:
		case 'd': case 'D':
			udbg_launch_debugger();
			//-fallthrough

		case 'b': case 'B':
			if(manual_break)
				return ER_BREAK;
			debug_break();
			return ER_CONTINUE;

		case 'c': case 'C':
			if(!no_continue)
                return ER_CONTINUE;
			// continue isn't allowed, so this was invalid input. loop again.
			break;
		case 's': case 'S':
			if(allow_suppress)
				return ER_SUPPRESS;
			// suppress isn't allowed, so this was invalid input. loop again.
			break;

		case 'e': case 'E':
			abort();
			return ER_EXIT;	// placebo; never reached
		}
	}
}


// mouse cursor stubs (required by lib/res/cursor.cpp)
// note: do not return ERR_NOT_IMPLEMENTED or similar because that
// would result in WARN_ERRs.
//
// TODO: implementing these would be nice because then the game can
// take advantage of hardware mouse cursors instead of the (jerky when
// loading) OpenGL cursor.

int sys_cursor_create(uint w, uint h, void* bgra_img,
	uint hx, uint hy, void** cursor)
{
	*cursor = 0;
	return 0;
}

int sys_cursor_set(void* cursor)
{
	return 0;
}

int sys_cursor_free(void* cursor)
{
	return 0;
}

int sys_error_description_r(int err, char* buf, size_t max_chars)
{
	// don't need to do anything: lib/errors.cpp already queries
	// libc's strerror(). if we ever end up needing translation of
	// e.g. Qt or X errors, that'd go here.
	return -1;
}