/*
 *  SQUID.C - Just like SQINFO only crunchy.
 *  Written 1996 by Andrew Clarke and released to the public domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

typedef struct {
    unsigned short sz_sqbase;   /*   0 */
    unsigned short rsvd1;       /*   2 */
    unsigned long num_msg;      /*   4 */
    unsigned long high_msg;     /*   8 */
    unsigned long skip_msg;     /*  12 */
    unsigned long high_water;   /*  16 */
    unsigned long uid;          /*  20 */
    unsigned char base[80];     /*  24 */
    unsigned long first_frame;  /* 104 */
    unsigned long last_frame;   /* 108 */
    unsigned long first_free_frame;  /* 112 */
    unsigned long last_free_frame;  /* 116 */
    unsigned long end_frame;    /* 120 */
    unsigned long max_msg;      /* 124 */
    unsigned short keep_days;   /* 128 */
    unsigned short sz_sqhdr;    /* 130 */
    unsigned char rsvd[124];    /* 132 */
} SQBASE;                       /* 256 */

typedef struct {
    unsigned long frame_id;     /*   0 */
    unsigned long next_frame;   /*   4 */
    unsigned long prev_frame;   /*   8 */
    unsigned long frame_len;    /*  12 */
    unsigned long msg_len;      /*  16 */
    unsigned long ctrl_len;     /*  20 */
    unsigned short frame_type;  /*  24 */
    unsigned short rsvd1;       /*  26 */
} SQFRAME;                      /*  28 */

typedef struct {
    unsigned long attr;         /*   0 */
    unsigned char orig_user[36];  /*   4 */
    unsigned char dest_user[36];  /*  40 */
    unsigned char subject[72];  /*  76 */
    unsigned short orig_zone;   /* 148 */
    unsigned short orig_net;    /* 150 */
    unsigned short orig_node;   /* 152 */
    unsigned short orig_point;  /* 154 */
    unsigned short dest_zone;   /* 156 */
    unsigned short dest_net;    /* 158 */
    unsigned short dest_node;   /* 160 */
    unsigned short dest_point;  /* 162 */
    unsigned short date_written;  /* 164 */
    unsigned short time_written;  /* 166 */
    unsigned short date_arrived;  /* 168 */
    unsigned short time_arrived;  /* 170 */
    signed short utc_ofs;       /* 172 */
    unsigned long replyto;      /* 174 */
    unsigned long see[9];       /* 178 */
    unsigned long umsgid;       /* 214 */
    unsigned char ftsc_date[20];  /* 218 */
} SQXMSG;                       /* 238 */

void get_sqbase(FILE * fp, SQBASE * x)
{
    unsigned long ofs;
    ofs = (unsigned long) ftell(fp);
    fread(&x->sz_sqbase, sizeof(x->sz_sqbase), 1, fp);
    fseek(fp, ofs + 2, SEEK_SET);
    fread(&x->rsvd1, sizeof(x->rsvd1), 1, fp);
    fseek(fp, ofs + 4, SEEK_SET);
    fread(&x->num_msg, sizeof(x->num_msg), 1, fp);
    fseek(fp, ofs + 8, SEEK_SET);
    fread(&x->high_msg, sizeof(x->high_msg), 1, fp);
    fseek(fp, ofs + 12, SEEK_SET);
    fread(&x->skip_msg, sizeof(x->skip_msg), 1, fp);
    fseek(fp, ofs + 16, SEEK_SET);
    fread(&x->high_water, sizeof(x->high_water), 1, fp);
    fseek(fp, ofs + 20, SEEK_SET);
    fread(&x->uid, sizeof(x->uid), 1, fp);
    fseek(fp, ofs + 24, SEEK_SET);
    fread(&x->base, sizeof(x->base), 1, fp);
    fseek(fp, ofs + 104, SEEK_SET);
    fread(&x->first_frame, sizeof(x->first_frame), 1, fp);
    fseek(fp, ofs + 108, SEEK_SET);
    fread(&x->last_frame, sizeof(x->last_frame), 1, fp);
    fseek(fp, ofs + 112, SEEK_SET);
    fread(&x->first_free_frame, sizeof(x->first_free_frame), 1, fp);
    fseek(fp, ofs + 116, SEEK_SET);
    fread(&x->last_free_frame, sizeof(x->last_free_frame), 1, fp);
    fseek(fp, ofs + 120, SEEK_SET);
    fread(&x->end_frame, sizeof(x->end_frame), 1, fp);
    fseek(fp, ofs + 124, SEEK_SET);
    fread(&x->max_msg, sizeof(x->max_msg), 1, fp);
    fseek(fp, ofs + 128, SEEK_SET);
    fread(&x->keep_days, sizeof(x->keep_days), 1, fp);
    fseek(fp, ofs + 130, SEEK_SET);
    fread(&x->sz_sqhdr, sizeof(x->sz_sqhdr), 1, fp);
    fseek(fp, ofs + 132, SEEK_SET);
    fread(&x->rsvd, sizeof(x->rsvd), 1, fp);
    fseek(fp, ofs + 256, SEEK_SET);
}

void get_sqframe(FILE * fp, SQFRAME * x)
{
    unsigned long ofs;
    ofs = (unsigned long) ftell(fp);
    fread(&x->frame_id, sizeof(x->frame_id), 1, fp);
    fseek(fp, ofs + 4, SEEK_SET);
    fread(&x->next_frame, sizeof(x->next_frame), 1, fp);
    fseek(fp, ofs + 8, SEEK_SET);
    fread(&x->prev_frame, sizeof(x->prev_frame), 1, fp);
    fseek(fp, ofs + 12, SEEK_SET);
    fread(&x->frame_len, sizeof(x->frame_len), 1, fp);
    fseek(fp, ofs + 16, SEEK_SET);
    fread(&x->msg_len, sizeof(x->msg_len), 1, fp);
    fseek(fp, ofs + 20, SEEK_SET);
    fread(&x->ctrl_len, sizeof(x->ctrl_len), 1, fp);
    fseek(fp, ofs + 24, SEEK_SET);
    fread(&x->frame_type, sizeof(x->frame_type), 1, fp);
    fseek(fp, ofs + 26, SEEK_SET);
    fread(&x->rsvd1, sizeof(x->rsvd1), 1, fp);
    fseek(fp, ofs + 28, SEEK_SET);
}

void get_sqxmsg(FILE * fp, SQXMSG * x)
{
    unsigned long ofs;
    ofs = (unsigned long) ftell(fp);
    fread(&x->attr, sizeof(x->attr), 1, fp);
    fseek(fp, ofs + 4, SEEK_SET);
    fread(&x->orig_user, sizeof(x->orig_user), 1, fp);
    fseek(fp, ofs + 40, SEEK_SET);
    fread(&x->dest_user, sizeof(x->dest_user), 1, fp);
    fseek(fp, ofs + 76, SEEK_SET);
    fread(&x->subject, sizeof(x->subject), 1, fp);
    fseek(fp, ofs + 148, SEEK_SET);
    fread(&x->orig_zone, sizeof(x->orig_zone), 1, fp);
    fseek(fp, ofs + 150, SEEK_SET);
    fread(&x->orig_net, sizeof(x->orig_net), 1, fp);
    fseek(fp, ofs + 152, SEEK_SET);
    fread(&x->orig_node, sizeof(x->orig_node), 1, fp);
    fseek(fp, ofs + 154, SEEK_SET);
    fread(&x->orig_point, sizeof(x->orig_point), 1, fp);
    fseek(fp, ofs + 156, SEEK_SET);
    fread(&x->dest_zone, sizeof(x->dest_zone), 1, fp);
    fseek(fp, ofs + 158, SEEK_SET);
    fread(&x->dest_net, sizeof(x->dest_net), 1, fp);
    fseek(fp, ofs + 160, SEEK_SET);
    fread(&x->dest_node, sizeof(x->dest_node), 1, fp);
    fseek(fp, ofs + 162, SEEK_SET);
    fread(&x->dest_point, sizeof(x->dest_point), 1, fp);
    fseek(fp, ofs + 164, SEEK_SET);
    fread(&x->date_written, sizeof(x->date_written), 1, fp);
    fseek(fp, ofs + 166, SEEK_SET);
    fread(&x->time_written, sizeof(x->time_written), 1, fp);
    fseek(fp, ofs + 168, SEEK_SET);
    fread(&x->date_arrived, sizeof(x->date_arrived), 1, fp);
    fseek(fp, ofs + 170, SEEK_SET);
    fread(&x->time_arrived, sizeof(x->time_arrived), 1, fp);
    fseek(fp, ofs + 172, SEEK_SET);
    fread(&x->utc_ofs, sizeof(x->utc_ofs), 1, fp);
    fseek(fp, ofs + 174, SEEK_SET);
    fread(&x->replyto, sizeof(x->replyto), 1, fp);
    fseek(fp, ofs + 178, SEEK_SET);
    fread(&x->see, sizeof(x->see), 1, fp);
    fseek(fp, ofs + 214, SEEK_SET);
    fread(&x->umsgid, sizeof(x->umsgid), 1, fp);
    fseek(fp, ofs + 218, SEEK_SET);
    fread(&x->ftsc_date, sizeof(x->ftsc_date), 1, fp);
    fseek(fp, ofs + 238, SEEK_SET);
}

void dump_sqbase(SQBASE * x)
{
    printf("Dump of SQBASE structure\n");
    printf("------------------------------------------------------------------------------\n\n");
    printf("sz_sqbase      : %hd\n", x->sz_sqbase);
    printf("num_msg        : %ld\n", x->num_msg);
    printf("high_msg       : %ld\n", x->high_msg);
    printf("uid            : %ld\n", x->uid);
    printf("first_frame    : %08lXh (%ld)\n", x->first_frame, x->first_frame);
    printf("last_frame     : %08lXh (%ld)\n", x->last_frame, x->last_frame);
    printf("first_free_frm : %08lXh (%ld)\n", x->first_free_frame, x->first_free_frame);
    printf("last_free_frm  : %08lXh (%ld)\n", x->last_free_frame, x->last_free_frame);
    printf("end_frame      : %08lXh (%ld)\n", x->end_frame, x->end_frame);
    printf("sz_sqhdr       : %hd\n", x->sz_sqhdr);
    printf("max_msg        : %ld\n", x->max_msg);
    printf("skip_msg       : %ld\n", x->skip_msg);
    printf("keep_days      : %hd\n", x->keep_days);
    printf("high_water     : %ld\n", x->high_water);
}

void dump_sqframe(SQFRAME * x)
{
    printf("\n\nDump of SQFRAME structure\n");
    printf("------------------------------------------------------------------------------\n\n");
    printf("frame_id       : %08lXh (%s)\n", x->frame_id,
      x->frame_id == 0xAFAE4453L ? "OK" : "Should be AFAE4453h!");
    printf("next_frame     : %08lXh (%ld)\n", x->next_frame, x->next_frame);
    printf("prev_frame     : %08lXh (%ld)\n", x->prev_frame, x->prev_frame);
    printf("frame_len      : %ld\n", x->frame_len);
    printf("msg_len        : %ld\n", x->msg_len);
    printf("ctrl_len       : %ld\n", x->ctrl_len);
    switch (x->frame_type) {
    case 0:
        printf("frame_type     : %s\n", "Normal frame");
        break;
    case 1:
        printf("frame_type     : %s\n", "Free frame");
        break;
    case 2:
        printf("frame_type     : %s\n", "LZSS frame");
        break;
    case 3:
        printf("frame_type     : %s\n", "Frame update");
        break;
    default:
        printf("frame_type     : %s\n", "Unknown!");
        break;
    }
}

void dump_sqxmsg(SQXMSG * x)
{
    printf("\n\nDump of SQXMSG structure\n");
    printf("------------------------------------------------------------------------------\n\n");
    printf("attr           : %08lXh (%ld)\n", x->attr, x->attr);
    printf("orig_user      : \"%s\"\n", x->orig_user);
    printf("orig_addr      : %hd:%hd/%hd.%hd\n", x->orig_zone, x->orig_net, x->orig_node, x->orig_point);
    printf("dest_user      : \"%s\"\n", x->dest_user);
    printf("dest_addr      : %hd:%hd/%hd.%hd\n", x->dest_zone, x->dest_net, x->dest_node, x->dest_point);
    printf("subject        : \"%s\"\n", x->subject);
    printf("date/time writ : %04hXh %04hXh\n", x->date_written, x->time_written);
    printf("date/time ariv : %04hXh %04hXh\n", x->date_arrived, x->time_arrived);
    printf("utc_ofs        : %hd\n", x->utc_ofs);
    printf("replyto        : %ld\n", x->replyto);
    printf("see            : %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
      x->see[0], x->see[1], x->see[2], x->see[3], x->see[4],
      x->see[5], x->see[6], x->see[7], x->see[8]);
    printf("umsgid         : %ld\n", x->umsgid);
    printf("ftsc_date      : \"%s\"\n", x->ftsc_date);
}

int main(int argc, char *argv[])
{
    SQBASE sqb;
    SQFRAME sqf;
    SQXMSG sqx;
    unsigned long frame_ofs;

    if (argc != 2) {
        fprintf(stderr, "Usage: SQUID <area name>\n");
        return EXIT_FAILURE;
    }
    fp = fopen(*(argv + 1), "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    get_sqbase(fp, &sqb);
    dump_sqbase(&sqb);
    frame_ofs = sqb.first_frame;
    while (frame_ofs != 0L) {
        fseek(fp, frame_ofs, SEEK_SET);
        get_sqframe(fp, &sqf);
        dump_sqframe(&sqf);
        frame_ofs = sqf.next_frame;
        get_sqxmsg(fp, &sqx);
        dump_sqxmsg(&sqx);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

