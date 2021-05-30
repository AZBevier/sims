/*
 * volmcopy.c
 *
 * This program scans a metatape file and prints file count and sizes.
 * It is only for volmgr save images.  Save images have the following format:
 * One or more 6144 byte records containing a list of files saved 16 char file
 * name followed my 16 char directory name followed by 16 char volume name.
 * A 1536 byte file definition entry will be followed by 1 to 8 768 byte
 * file data records followed by an EOF for each file.  If the file size is
 * greater than 6144 bytes, 1 or more 6144 byte records are output followed
 * by last record modulo 768 bytes.  Two EOFs in a row define the EOT.
 * input - stdin or specified filename  
 * output - stdout
 */

#include <stdio.h>
#include <stdlib.h>
//#include <sys/file.h>
//#include <unistd.h>
//#include <string.h>
#include <ctype.h>
//#include <errno.h>
#include <time.h>

int filen = 1;
int EOFcnt = 0;
int count=0, lcount=0;
int size=0, tsize=0;
int size_512K = 512 * 1024;
int ln;
//int inp, outp;
FILE *infp, *outfp;
#define PRINTABLE(x) ((x < 32) || (x > 126)) ? '.' : x

/* get a line of input. */
int getloi(char *s, int lim)
{
//  int c, i;
    int n1, n2, hc, tc, n;

//  errno = 0;
    /* read the byte count in 32 bit word as header */
//  n1 = read(inp, (char *)(&hc), (size_t)4);
    n1 = fread((char *)(&hc), (size_t)1, (size_t)4, infp);
    if (n1 <= 0)
        hc = -1;        /* at EOM on disk file */

    /* check for EOF & EOM on tape data */
    if (hc & 0xffff0000)    /* check for garbage, assume EOM */
        hc = -1;        /* at EOM on disk file */

    /* check for EOF & EOM on tape data */
    if (hc == 0) {
        /* we are at tape EOF */
        if (++EOFcnt < 2) {     /* if 1st EOF, print file info */
            filen++;    /* set next file number */
        }
        count = 0;      /* file record count back to zero */
        lcount = 0;     /* last record count back to zero */
        tsize += size;  /* add to total tape size */
        size = 0;       /* file size back to zero */
        ln = -1;        /* set ln to -1 showing we are at EOF */

        /* we have EOF */
        return 0;       /* return EOF on tape data */
    }
    if (hc == -1)
        return -1;      /* at EOM on disk file */

    /* read the data */
//  n = read(inp, s, (size_t)hc);
    n = fread(s, (size_t)1, (size_t)hc, infp);

    /* if odd byte record, read extra byte and throw it away */
    if (n & 0x1) {
//      n2 = read(inp, (char *)(&tc), (size_t)1);
        n2 = fread((char *)(&tc), (size_t)1, (size_t)1, infp);
        if (n2 <= 0)
            return -1;          /* at EOM on disk file */
    }

    /* read the byte count in 32 bit word as trailer */
//  n2 = read(inp, (char *)(&tc), (size_t)4);
    n2 = fread((char *)(&tc), (size_t)1, (size_t)4, infp);
    count++;        /* bump record count */
    size += n;      /* update bytes read */
    EOFcnt = 0;     /* not an EOF */
    if (n != ln) {
        ln = n;
        lcount = count;
    }
    /* return bytes in buffer */
    return n;
}

int main (int argc, char *argv[])
{
    unsigned char *buf;
    size_t size_512K = 512 * 1024;
    size_t buf_size = 512 * 1024;
//  char *cp, *np;
    int ll;
    int lfilen = filen;
    unsigned int mpxtime[2];
    char path[64], command[128], dtime[64];
    time_t utime=0;
    struct tm *dt;

    if (argc != 2) {
        fprintf(stderr, "usage: %s infile\n", argv[0]);
        exit(1);
    } /* end of if */

//  if ((inp = open(argv[1], O_RDONLY, 0666)) < 0) {
    if ((infp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr,"%s: fopen: unable to open input file %s\n", argv[0], argv[1]);
        return (1);
    }
//  outp = -1;
    outfp = NULL;

    /* get a 512k buffer */
    if ((buf = malloc(buf_size)) == NULL) {
        fprintf(stderr, "Can't allocate memory for tscan\n");
        return (4);
    }
    /* init counts */
    ln = -2;    /* look for 2 eof */
    count = 0;
    size = 0;
    tsize = 0;
    lcount = 0;
    filen = 1;
    lfilen = filen;
    printf("\nfile %d:\n", filen);
    path[0] = '\0';
    command[0] = '\0';

    /* get lines until eof */
    while ((ll=getloi(buf, buf_size)) != EOF) {
        if (ll == 0) {
            /* eof found, process new file */
//          printf("\nfile %d:\n", filen);
            printf("file %d:\n", filen);
        } else {
            int cc = 0;
//          unsigned int curchar;

            /* dump first 2 words */
            int w1, w2, i, j;
            w1 = (buf[0] & 0xff) << 24 | buf[1] << 16 | buf[2] << 8 | (buf[3] & 0xff);
            w2 = (buf[4] & 0xff) << 24 | buf[5] << 16 | buf[6] << 8 | (buf[7] & 0xff);
// printf("w1 = %x, w2 = %x count = %d\n", w1, w2, count);
            if (count == 1 && w1 == 1) {
                char file[20], dir[20], vol[20];
                int off = 8;
                int l = 0;
printf("Directory with %d entries\n", w2);
                /* we have directory entries */
                for (j=0; j<w2; j++) {
                    int k = l++ * 48;
                    if (k > (6144-48-off)) {
                        ll=getloi(buf, buf_size);
                        off = 0;
                        l = 0;
                        k = 0;
// printf("reread: got ll= %d\n", ll);
                    }
                    for (i=0; i<16; i++) {
                        file[i] = tolower(buf[k+off+i]);
                        if (file[i] == ' ')
                            file[i] = '\0';
                    }
                    file[16] = '\0';
                    for (i=0; i<16; i++) {
                        dir[i] = tolower(buf[k+off+16+i]);
                        if (dir[i] == ' ')
                            dir[i] = '\0';
                    }
                    dir[16] = '\0';
                    for (i=0; i<16; i++) {
                        vol[i] = tolower(buf[k+off+32+i]);
                        if (vol[i] == ' ')
                            vol[i] = '\0';
                    }
                    vol[16] = '\0';
                    sprintf(path, "./%s/%s", vol, dir);
#if 0
                    /* create the directory/file */
                    sprintf(command, "mkdir -p %s", path);
                    system(command);
#endif
                    sprintf(path, "./%s/%s/%s", vol, dir, file);
#if 0
                    sprintf(command, "touch %s", path);
                    system(command);
#endif
// printf("path0 = %s\n", path);
                }
            } else
            if (count == 1 && w1 == 2 && w2 == 0) {
                char file[20], dir[20], vol[20];
//              if (outp >= 0)
                if (outfp != NULL) {
//                  close(outp);
                    fclose(outfp);
                    sprintf(command, "touch -a -m -t %s %s", dtime, path);
//                  sprintf(command, "touch -t %s %s", dtime, path);
// printf("1cmd [%s] path[%s]\n", command, path);
// fflush(stdout);
                    system(command);
                }
//              outp = -1;
                outfp = NULL;
                /* process file definition */
                /* we have a 1536 byte file definition entry */
                for (i=0; i<16; i++) {
                    file[i] = tolower(buf[8+i]);
                    if (file[i] == ' ')
                        file[i] = '\0';
                }
                file[16] = '\0';
                for (i=0; i<16; i++) {
                    dir[i] = tolower(buf[24+i]);
                    if (dir[i] == ' ')
                        dir[i] = '\0';
                }
                dir[16] = '\0';
                for (i=0; i<16; i++) {
                    vol[i] = tolower(buf[40+i]);
                    if (vol[i] == ' ')
                        vol[i] = '\0';
                }
                vol[16] = '\0';

                i = 0x300 + 0x38;       /* last modified date */
//              i = 0x300 + 0x20;       /* creation date */
                /* get last modification time for file */
                /* day number since 1/1/1960 */
// printf("D %02x%02x%02x%02x M %02x%02x%02x%02x\n",
//     buf[i+0],buf[i+1],buf[i+2],buf[i+3],buf[i+4],buf[i+5],buf[i+6],buf[i+7]);
                mpxtime[0] = buf[i+0] << 24 | buf[i+1] << 16 | buf[i+2] << 8 | (buf[i+3] & 0xff);
                /* .1ms count in day 0-864,000,000 */
                mpxtime[1] = (buf[i+4] << 24) | (buf[i+5] << 16) | (buf[i+6] << 8) | (buf[i+7] & 0xff);
                /* get UNIX time, secs sense 1970, not 1960 */
                utime =(time_t)((mpxtime[0]-3653)*86400)+(time_t)(mpxtime[1]/10000);
                dt = gmtime(&utime);
                j = dt->tm_year+1900;
                if (j > 2037)
                    j -= 100;
                sprintf(dtime, "%04d%02d%02d%02d%02d", j, dt->tm_mon+1,
                    dt->tm_mday, dt->tm_hour, dt->tm_min);
// printf("mpx day %x ms %x utime %ld dtime %s\n", mpxtime[0], mpxtime[1], utime, dtime);
// fflush(stdout);

                sprintf(path, "./%s/%s", vol, dir);
                /* create the directory/file */
// printf("dir path = %s\n", path);
                sprintf(command, "mkdir -p %s", path);
                system(command);
                sprintf(path, "./%s/%s/%s", vol, dir, file);
   printf("size bytes %d[0x%x] path = %s\n", ll, ll, path);
// fflush(stdout);

                /* open output file, create it if necessary */
//              if ((outp = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0) {
                if ((outfp = fopen(path, "w")) == NULL) {
                    (void)fprintf(stderr, "Can't open %s\n", path);
//                  close(inp);
                    fclose(infp);
                    free(buf);
                    return (3);
                }
// printf("opened path = %s ll = %d\n", path, ll);
// fflush(stdout);
                /* process file data for file */
                if (ll > 1536) {
//                  int no = write(outp, buf+1536, ll-1536);
                    int no = fwrite(buf+1536, (size_t)1, (size_t)(ll-1536), outfp);
                    if (no != ll)
                        fprintf(stderr, "write (%d) != read (%d) on file %s\n", no, ll, path);
                }
            } else
            if (count > 1) {
                /* process file data for file */
//              int no = write(outp, buf, ll);
                int no = fwrite(buf, (size_t)1, (size_t)ll, outfp);
                if (no != ll)
                    fprintf(stderr, "write (%d) != read (%d) on file %s\n", no, ll, path);
            }
        }
    }
//  close(inp);
    fclose(infp);
    free(buf);
//  if (outp >= 0)
    if (outfp != NULL) {
//      close(outp);
        fclose(outfp);
        sprintf(command, "touch -a -m -t %s %s", dtime, path);
//      sprintf(command, "touch -t %s %s", dtime, path);
// printf("2cmd [%s]\n", command);
// fflush(stdout);
        system(command);
    }
    exit(0);
}
