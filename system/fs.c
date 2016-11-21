#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#if FS
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
int inode_num = 1;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int inode_id = 1;

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */





int fs_create(char *filename, int mode)
{
    if(mode==O_CREAT){
        //get root dir:
        struct directory dir=fsd.root_dir;
        //check if file name is valid. i.e., no file with same name exists
        int i;
        
        for(i=0;i<fsd.root_dir.numentries;i++){
            if(strcmp(filename, fsd.root_dir.entry[i].name) == 0)
            {
                kprintf("File already exists.\n\r");
                return SYSERR;
            }
        }
        //request entry in filetable:
        
        int fd = next_open_fd;
        struct filetable ft;
        
        oft[next_open_fd].state=FSTATE_OPEN;
        oft[next_open_fd].fileptr=0;
        oft[next_open_fd].de=&(fsd.root_dir.entry[fsd.root_dir.numentries++]);       
        strcpy((oft[next_open_fd].de)->name, filename); 
        //memcpy(&(fsd.root_dir),&dir,sizeof(struct directory));
        //struct inode in;
        //in.id=inode_num++;
        oft[next_open_fd].in.id = inode_num;
        fs_put_inode_by_num(0, oft[next_open_fd].in.id, &(oft[next_open_fd].in));
        int bl = oft[next_open_fd].in.id / INODES_PER_BLOCK;
        bl += FIRST_INODE_BLOCK;
        fs_setmaskbit(bl);
        //memcpy(&(ft.in),&in,sizeof(struct inode));
        //memcpy(&(ft.de->inode_num),&(in.id),sizeof(int));
        //memcpy(&(fsd.root_dir.entry[dir.numentries-1].inode_num),&(in.id),sizeof(int));
        //memcpy(oft+fd,&ft,sizeof(struct filetable));
        fsd.inodes_used++;
	inode_num++;
	next_open_fd++;
        return fd;
    }
    else return SYSERR;
}


int fs_open(char *filename, int flags){
 int i;
 for(i=0;i<next_open_fd;i++){
 	if(strcmp(oft[i].de->name,filename)==0){
		oft[next_open_fd].state=FSTATE_OPEN;
 		
		return i;
	}
 }
 printf("\n File not found");
 return SYSERR;
}

int fs_seek(int fd, int offset){
 if(oft[fd].fileptr+offset+1 <0){
	printf("\nInvalid offset");
	return SYSERR;
 }
 else{
	 oft[fd].fileptr+=offset+1;
 	printf("\n Seek successful. File PTR IS NOW %d",oft[fd].fileptr);
 	return OK;
 }
}

int fs_close(int fd){
 if(fd>next_open_fd||oft[fd].state==FSTATE_CLOSED)
	return SYSERR;
 else{
	oft[fd].state=FSTATE_CLOSED;
	return OK;
 }
}

int find_free_block()
{
  int i;
  for(i = 0; i < fsd.nblocks; i++)
  {
    if(fs_getmaskbit(i) == 0)
    {
      return i;
    }
  }
  printf("Cannot find free block. \n");
  return -1;
}


int fs_read(int fd, void *buf, int nbytes)
{
  int originalbytes = nbytes;
  
  
  if(oft[fd].state == 0)
  {
    printf("File descriptor is closed.\n");
    return 0;
  }
  
  int start_inode_blk = ((oft[fd].fileptr) / fsd.blocksz);
  int block_offset = ((oft[fd].fileptr) % fsd.blocksz);
  
  if(start_inode_blk < INODEBLOCKS)
  {
    int dest_blk = oft[fd].in.blocks[start_inode_blk];
    
    while(nbytes > 0)
    {
      //the data to be read is all in one block
      if(nbytes < (fsd.blocksz - block_offset))
      {
        bs_bread(0, dest_blk, block_offset, buf, nbytes);
        
        oft[fd].fileptr += nbytes;
        buf += nbytes;
        nbytes = 0;
        return originalbytes;
      }
      else
      {
        if(start_inode_blk == INODEBLOCKS - 1)
        {
          printf("Reading only the maximum limit of number of bytes to read.\n");
          return originalbytes - nbytes;
        }
        
        bs_bread(0, dest_blk, block_offset, buf, fsd.blocksz - block_offset);
        buf += (fsd.blocksz - block_offset);
        nbytes -= (fsd.blocksz - block_offset);
        oft[fd].fileptr += (fsd.blocksz - block_offset);
        
        start_inode_blk++;
        dest_blk = oft[fd].in.blocks[start_inode_blk];
        block_offset = 0;
      }
    }
  }
  
  return (originalbytes - nbytes);
}

int fs_write(int fd, void *buf, int nbytes)
{
  int originalbytes = nbytes;
  
  if(oft[fd].state == 0)
  {
    printf("File descriptor is closed.\n");
    return 0;
  }
  
  int start_inode_blk = ((oft[fd].fileptr) / fsd.blocksz);
  int block_offset = ((oft[fd].fileptr) % fsd.blocksz);
  
  if(start_inode_blk < INODEBLOCKS)
  {
    int dest_blk;
    
    while(nbytes > 0)
    {
      if(oft[fd].in.blocks[start_inode_blk] == 0)
      {
        dest_blk = find_free_block();
        memcpy(oft[fd].in.blocks+start_inode_blk, &dest_blk, sizeof(int));
        
        fs_put_inode_by_num(0, oft[fd].in.id, &(oft[fd].in));
        fs_setmaskbit(dest_blk);
      }
      else
      {
        dest_blk = oft[fd].in.blocks[start_inode_blk];
      }
      
      //if all the bytes can be written to the same block
      if(nbytes < (fsd.blocksz - block_offset))
      {
        bs_bwrite(0, dest_blk, block_offset, buf, nbytes);
        oft[fd].fileptr += nbytes;
        nbytes = 0;
        return originalbytes;
      }
      else
      {
        if(start_inode_blk == INODEBLOCKS - 1)
        {
          printf("Writing only the maximum limit of number of bytes allowed.\n");
          return originalbytes - nbytes;
        }
        
        bs_bwrite(0, dest_blk, block_offset, buf, fsd.blocksz - block_offset);
        
        buf += (fsd.blocksz - block_offset);
        nbytes -= (fsd.blocksz - block_offset);
        oft[fd].fileptr += (fsd.blocksz - block_offset);
        
        start_inode_blk++;
        block_offset = 0;
      }
    }
  }
  
  return originalbytes - nbytes;
}
  

/*
int fs_read(int fd, void *buf, int nbytes)
{
   
    int orig_nbytes=nbytes;
    
    struct filetable ft=oft[fd];
    if(ft.state==FSTATE_CLOSED){
        kprintf("Error Invalid descriptor.\n\r");
        return 0;
    }
    struct inode in = ft.in;

    int inodeblk= (ft.fileptr / fsd.blocksz);
    int inodeoffset=(ft.fileptr % fsd.blocksz);
    if (inodeblk<INODEBLOCKS){
        int dst_blk=ft.in.blocks[inodeblk];
        while(nbytes>0){
            
            if(nbytes<(fsd.blocksz-inodeoffset)){
                bs_bread(0,dst_blk,inodeoffset,buf,nbytes);
                ft.fileptr+=nbytes;
                buf+=nbytes;
                nbytes=0;
                return orig_nbytes;
            }
            else{
                if(inodeblk==INODEBLOCKS-1){
                    kprintf("Requested bytes exceeds limit.\n\r");
                    return orig_nbytes-nbytes;
                }
                bs_bread(0,dst_blk,inodeoffset,buf,fsd.blocksz-inodeoffset);
                buf+=(fsd.blocksz-inodeoffset);
                nbytes-=(fsd.blocksz-inodeoffset);
                ft.fileptr+=(fsd.blocksz-inodeoffset);
          
                memcpy(oft+fd,&ft,sizeof(struct filetable));
              
                dst_blk=in.blocks[++inodeblk];
                inodeoffset=0;
            }
        }
    }
    return orig_nbytes-nbytes;
}

int fs_write(int fd, void *buf, int nbytes)
{
    int orig_nbytes=nbytes;
    
    struct filetable ft=oft[fd];

    if(ft.state==FSTATE_CLOSED){
        kprintf("Error %d. Invalid descriptor.\n\r",ft.state);
        return 0;
    }
    struct inode in = ft.in;
    
    int inodeblk= (ft.fileptr / fsd.blocksz);
    int inodeoffset=(ft.fileptr % fsd.blocksz);
    if (inodeblk<INODEBLOCKS){
        int dst_blk;
        while(nbytes>0){

            if(in.blocks[inodeblk]==0){
                //alloc new block
                dst_blk=find_free_block();
                memcpy(in.blocks+inodeblk,&dst_blk,sizeof(int));

                //copy in back to ft.in:
                memcpy(&((oft+fd)->in),&(in),sizeof(struct inode));
                ft.in=in;
                
                fs_put_inode_by_num(0,in.id,&in);

                //mark that block 
                fs_setmaskbit(dst_blk);
            }
            else if(in.blocks[inodeblk]>0){
                dst_blk=in.blocks[inodeblk]; 
            }
            //Check if all data we want to write can be put in same block
            if(nbytes<(fsd.blocksz-inodeoffset)){
                bs_bwrite(0,dst_blk,inodeoffset,buf,nbytes);
                //incr fileptr:
                ft.fileptr+=nbytes;
                nbytes=0;
                //write to oft:
                memcpy(oft+fd,&ft,sizeof(struct filetable));
                return orig_nbytes;
            }
            else{
                if(inodeblk==INODEBLOCKS-1){
                    kprintf("\nRequested bytes exceeds limit.\n\r");
                    return orig_nbytes-nbytes;
                }
                bs_bwrite(0,dst_blk,inodeoffset,buf,fsd.blocksz-inodeoffset);
                buf+=(fsd.blocksz-inodeoffset);
                nbytes-=(fsd.blocksz-inodeoffset);
                ft.fileptr+=(fsd.blocksz-inodeoffset);
                //write to oft:
                memcpy(oft+fd,&ft,sizeof(struct filetable));
    
                inodeblk++;
                inodeoffset=0;
            }
        }
    }
    return orig_nbytes-nbytes;
}  

*/



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

#endif /* FS */
