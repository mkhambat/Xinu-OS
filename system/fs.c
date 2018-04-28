#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#ifdef FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;
int node_count = 1;



#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void
fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}


int fs_open(char *filename, int flags) {

	bs_bread(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));

	struct inode node;
	int flag = 0;
	int temp = next_open_fd;
	int i;

	for (i = 0; i < fsd.root_dir.numentries; i++)  //Iterate through all the file entries
	{
		if (strcmp(fsd.root_dir.entry[i].name, filename) == 0)  //If file found
		{	
			oft[temp].fileptr = 0;  //Set file pointer to 0
			oft[temp].state = FSTATE_OPEN;  //Set file state as open
			oft[temp].de = &fsd.root_dir.entry[i];
			fs_get_inode_by_num(0, fsd.root_dir.entry[i].inode_num, &node);
			node.type = flags;  //Update flags
			oft[temp].in = node;  //Update inode
			fs_put_inode_by_num(0, fsd.root_dir.entry[i].inode_num, &node);
			flag = 1;  //Sets flag to 1
		}
	}

	if (flag == 0)  //If file is not found
	{
		return SYSERR;
	}
	next_open_fd++;
	return temp;
}


int fs_close(int fd) {

	if(fd > 0 || fd <= next_open_fd)  //If file entry is valid
	{
		oft[fd].fileptr = 0;  //Set pointer to 0
		oft[fd].state = FSTATE_CLOSED;  //Set state to close
		next_open_fd = next_open_fd - 1; 
		return OK;
	}

	if(oft[fd].state == FSTATE_CLOSED)
	{
		printf("File already in closed state\n");
		return OK;
	}

	if(fd < 0 || fd > next_open_fd)  //If entry is invalid
	{
		return SYSERR;
	}
}


int fs_create(char *filename, int mode) {

	struct filetable file_table;
	struct directory dir = fsd.root_dir;
	struct inode in;

	int flag = 0;
	int temp;
	int block_length;
	int i = 1;
	
	temp = next_open_fd + 1;
	for (i = 0; i < dir.numentries; i++) 
	{
		if (strcmp(filename, dir.entry[i].name) == 0)  //If file already exists
		{
			flag = 1;
			return SYSERR;
		}
	}

	if (mode == O_CREAT) 
	{	
		file_table.state = FSTATE_OPEN;  //Set state to open
		
		// set initial File pointer.
		file_table.fileptr = 0;
		file_table.de = &(dir.entry[dir.numentries++]);
		
		memcpy(&(fsd.root_dir), &dir, sizeof(struct directory));  //Update root
		strcpy((file_table.de)->name, filename);  //Change file name

		in.type = INODE_TYPE_FILE;
		in.id = node_count+1;
		fs_put_inode_by_num(0, in.id, &in);
		
		block_length = in.id / INODES_PER_BLOCK;
		block_length = block_length + FIRST_INODE_BLOCK;
		fs_setmaskbit(block_length);
		
		memcpy(&(file_table.in), &in, sizeof(struct inode));
		memcpy(&(fsd.root_dir.entry[dir.numentries - 1].inode_num), &(in.id), sizeof(int));
		memcpy(oft + temp, &file_table, sizeof(struct filetable));
		memcpy(&(file_table.de->inode_num), &(in.id), sizeof(int));
	
		fsd.inodes_used = fsd.inodes_used + 1;
		return temp;
	}
	if (mode != O_CREAT)
	{	
		return SYSERR;
	}	
}


//   return SYSERR;
// }

int fs_seek(int fd, int offset) {

	offset = oft[fd].fileptr + offset;
	if (offset<0)
	{
		offset = 0;
	}

	memcpy(&((oft + fd)->fileptr), &offset, sizeof(int));
	return fd;
}



//   return SYSERR;
// }

int fs_read(int fd, void *buf, int nbytes) {

	struct filetable file_table = oft[fd];
	struct inode in = file_table.in;

	int nbytes1 = nbytes;
	int no_of_bytes = 0;
	int inode_offset = (file_table.fileptr % fsd.blocksz);
	int inode_block = (file_table.fileptr / fsd.blocksz);
	
	if (inode_block<INODEBLOCKS) 
	{
		int dst_blk = file_table.in.blocks[inode_block];
	
		while (nbytes>0) 
		{
			if (nbytes >= (fsd.blocksz - inode_offset)) 
			{
				if (inode_block == INODEBLOCKS - 1) 
				{
					printf("Byte limit exceeded");
					no_of_bytes = nbytes1 - nbytes;
					return no_of_bytes;
				}

				bs_bread(0, dst_blk, inode_offset, buf, fsd.blocksz - inode_offset);
				buf = buf + (fsd.blocksz - inode_offset);
				nbytes = nbytes - (fsd.blocksz - inode_offset);
				file_table.fileptr = file_table.fileptr + (fsd.blocksz - inode_offset);
				memcpy(oft + fd, &file_table, sizeof(struct filetable));
				dst_blk = in.blocks[++inode_block];
				inode_offset = 0;
			}

			if (nbytes < (fsd.blocksz - inode_offset)) 
			{
				bs_bread(0, dst_blk, inode_offset, buf, nbytes);
				file_table.fileptr = file_table.fileptr + nbytes;
				buf = buf + nbytes;
				nbytes = 0;
				return nbytes1;				
			}
		}
	}	
	no_of_bytes = nbytes1 - nbytes;
	return no_of_bytes;

	if (file_table.state == FSTATE_CLOSED)  //If file state is closed
	{
		return SYSERR;
	}
}

//   return SYSERR;
// }

int fs_write(int fd, void *buf, int nbytes) {

	struct filetable file_table = oft[fd];
	struct inode in = file_table.in;

	int nbytes1 = nbytes;
	int i;
	int no_of_bytes = 0, next_block = 0;
	int inode_block = (file_table.fileptr / fsd.blocksz);
	int inode_offset = (file_table.fileptr % fsd.blocksz);

	if (inode_block < INODEBLOCKS) 
	{
		while (nbytes>0) 
		{
			if (in.blocks[inode_block]>0) 
			{
				i = in.blocks[inode_block];
			}
		
			if (in.blocks[inode_block] == 0) 
			{
				for ( i = 0; i < fsd.nblocks; i++ )
				{
					if(fs_getmaskbit(i) == 0)
					{
						next_block = i;
					}
				}

				memcpy(&((oft + fd)->in), &(in), sizeof(struct inode));
				memcpy(in.blocks + inode_block, &i, sizeof(int));
				
				file_table.in = in;
				fs_put_inode_by_num(0, in.id, &in);
				fs_setmaskbit(i);
			}

			if (nbytes<(fsd.blocksz - inode_offset)) 
			{
				bs_bwrite(0, i, inode_offset, buf, nbytes);
				file_table.fileptr = file_table.fileptr + nbytes;
				nbytes = 0;
				memcpy(oft + fd, &file_table, sizeof(struct filetable));
				return nbytes1;
			}
			else 
			{
				if (inode_block == INODEBLOCKS - 1) 
				{
					no_of_bytes = nbytes1 - nbytes;
					return no_of_bytes;
				}
				
				bs_bwrite(0, i, inode_offset, buf, fsd.blocksz - inode_offset);
				buf = buf + (fsd.blocksz - inode_offset);
				nbytes = nbytes - (fsd.blocksz - inode_offset);
				file_table.fileptr = file_table.fileptr + (fsd.blocksz - inode_offset);
				memcpy(oft + fd, &file_table, sizeof(struct filetable));
				inode_offset = 0;
				inode_block = inode_block + 1;
			}
		}
	}

	if (file_table.state == FSTATE_CLOSED)  //If file state is closed
	{
		return SYSERR;
	}

	no_of_bytes = nbytes1 - nbytes;
	return no_of_bytes;
}


//   return SYSERR;
// }

#endif /* FS */