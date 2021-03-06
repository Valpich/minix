
#include "syslib.h"

#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <minix/sysutil.h>

void *alloc_contig(size_t len, int flags, phys_bytes *phys)
{
	vir_bytes buf;
	int mmapflags = MAP_PREALLOC|MAP_CONTIG|MAP_ANON;

	if(flags & AC_LOWER16M)
		mmapflags |= MAP_LOWER16M;
	if(flags & AC_LOWER1M)
		mmapflags |= MAP_LOWER1M;
	if(flags & AC_ALIGN64K)
		mmapflags |= MAP_ALIGNMENT_64KB;

	/* First try to get memory with mmap. This is guaranteed
	 * to be page-aligned, and we can tell VM it has to be
	 * pre-allocated and contiguous.
	 */
	errno = 0;
	buf = (vir_bytes) mmap(0, len, PROT_READ|PROT_WRITE, mmapflags, -1, 0);

	if(buf == (vir_bytes) MAP_FAILED) {
		return NULL;
	}

	/* Get physical address, if requested. */
        if(phys != NULL && sys_umap(SELF, VM_D, buf, len, phys) != OK)
		panic("sys_umap_data_fb failed");

	return (void *) buf;
}

int free_contig(void *addr, size_t len)
{
	return munmap(addr, len);
}

