/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  genpmsg - generate packet message.                               */
/*                                                                   */
/*********************************************************************/

#include <string.h>
#include <stdio.h>

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
} dt;

typedef struct {
    unsigned char origNode[2];
    unsigned char destNode[2];
    unsigned char origNet[2];
    unsigned char destNet[2];
    unsigned char Attribute[2];
    unsigned char cost[2];
} raw_fido_msg_hdr;

typedef struct {
    int origNode;
    int destNode;
    int origNet;
    int destNet;
    int origZone;
    int destZone;
    unsigned int Attribute;
    int cost;
} fido_msg_hdr;

typedef struct {
    unsigned char origNode[2];
    unsigned char destNode[2];
    unsigned char year[2];
    unsigned char month[2];
    unsigned char day[2];
    unsigned char hour[2];
    unsigned char minute[2];
    unsigned char second[2];
    unsigned char baud[2];
    unsigned char PacketType[2];
    unsigned char origNet[2];
    unsigned char destNet[2];
    unsigned char prodCode;
    unsigned char serialNo;
    unsigned char password[8];
    unsigned char origZone[2];
    unsigned char destZone[2];
    unsigned char auxNet[2];
    unsigned char CWvalidationCopy[2];
    unsigned char prodCodeH;
    unsigned char Revision;
    unsigned char CapabilWord[2];
    unsigned char origZone2[2];
    unsigned char destZone2[2];
    unsigned char origPoint[2];
    unsigned char destPoint[2];
    unsigned char prodData[4];
    /* zero or more packed messages, followed by "\x00\x00\x00\x00"
       follow.*/
} raw_fido_packet_hdr;

typedef struct {
    unsigned int origNode;
    unsigned int destNode;
    dt pdate;
    unsigned int baud;
    unsigned int PacketType;
    unsigned int origNet;
    unsigned int destNet;
    unsigned int prodCode;
    unsigned int serialNo;
    unsigned char password[8];
    unsigned int origZone;
    unsigned int destZone;
    unsigned int capability;
    unsigned int origPoint;
    unsigned int destPoint;
    unsigned int Revision;
    unsigned char prodData[4];
} fido_packet_hdr;

int cvt_to_raw_packet_hdr(const fido_packet_hdr *fp_hdr,
    raw_fido_packet_hdr *raw_p);

int cvt_to_raw_message_hdr(const fido_msg_hdr *fm_hdr,
    raw_fido_msg_hdr *raw_m);

int main(void)
{
    fido_packet_hdr ph;
    raw_fido_packet_hdr rph;
    fido_msg_hdr mh;
    raw_fido_msg_hdr rmh;
    FILE *fq;

    ph.origNode = 305;
    ph.destNode = 934;
    ph.pdate.year = 1994;
    ph.pdate.month = 1;
    ph.pdate.day = 1;
    ph.pdate.hour = 1;
    ph.pdate.min = 1;
    ph.pdate.sec = 1;
    ph.baud = 0;
    ph.PacketType = 2;
    ph.origNet = 640;
    ph.destNet = 711;
    ph.prodCode = 0;
    ph.serialNo = 0;
    memset(ph.password, 0x00, 8);
    ph.origZone = 3;
    ph.destZone = 3;
    ph.capability = 1;
    ph.origPoint = 0;
    ph.destPoint = 0;
    ph.Revision = 0;
    memset(ph.prodData, 0x00, 4);
    cvt_to_raw_packet_hdr(&ph, &rph);

    mh.origNode = 305;
    mh.destNode = 934;
    mh.origNet = 640;
    mh.destNet = 711;
    mh.origZone = 3;
    mh.destZone = 3;
    mh.Attribute = 0;
    mh.cost = 0;

    cvt_to_raw_message_hdr(&mh, &rmh);
    
    fq = fopen("temp.pkt", "wb");
    if (fq != NULL)
    {
        fwrite(&rph, 1, sizeof rph, fq);
        fputc(0x02, fq);
        fputc(0x00, fq);
        fwrite(&rmh, 1, sizeof rmh, fq);
        fprintf(fq, "01 Jan 94  01:01:01");
        fputc('\0', fq);
        fprintf(fq, "Paul Edwards");
        fputc('\0', fq);
        fprintf(fq, "Neil Armstrong");
        fputc('\0', fq);
        fprintf(fq, "fascist pigs");
        fputc('\0', fq);
        fprintf(fq, "AREA:AUST_AVTECH\r");
        fprintf(fq, "Hello, world!\r");
        fprintf(fq, "\r");
        fprintf(fq, "---\r");
        fprintf(fq, " * Origin: X (3:640/305)\r");
        fprintf(fq, "SEEN-BY: 640/305 711/934\r");
        fputc('\0', fq);
        fputc('\0', fq);
        fputc('\0', fq);
        fclose(fq);
    }
    return (0);
}

int cvt_to_raw_message_hdr(const fido_msg_hdr *fm_hdr,
                           raw_fido_msg_hdr *raw_m)
{
    raw_m->origNode[0] = (unsigned char)(fm_hdr->origNode & 0xff);
    raw_m->origNode[1] = (unsigned char)((fm_hdr->origNode >> 8) & 0xff);
    raw_m->destNode[0] = (unsigned char)(fm_hdr->destNode & 0xff);
    raw_m->destNode[1] = (unsigned char)((fm_hdr->destNode >> 8) & 0xff);
    raw_m->origNet[0] = (unsigned char)(fm_hdr->origNet & 0xff);
    raw_m->origNet[1] = (unsigned char)((fm_hdr->origNet >> 8) & 0xff);
    raw_m->destNet[0] = (unsigned char)(fm_hdr->destNet & 0xff);
    raw_m->destNet[1] = (unsigned char)((fm_hdr->destNet >> 8) & 0xff);
    raw_m->Attribute[0] = (unsigned char)(fm_hdr->Attribute & 0xff);
    raw_m->Attribute[1] = (unsigned char)((fm_hdr->Attribute >> 8) & 0xff);
    raw_m->cost[0] = (unsigned char)(fm_hdr->cost & 0xff);
    raw_m->cost[1] = (unsigned char)((fm_hdr->cost >> 8) & 0xff);
    return (0);
}

int cvt_to_raw_packet_hdr(const fido_packet_hdr *fp_hdr,
    raw_fido_packet_hdr *raw_p)
{
    memset(raw_p, '\0', sizeof *raw_p);
    raw_p->origNode[0] = (unsigned char)(fp_hdr->origNode & 0xff);
    raw_p->origNode[1] = (unsigned char)((fp_hdr->origNode >> 8) & 0xff);
    raw_p->destNode[0] = (unsigned char)(fp_hdr->destNode & 0xff);
    raw_p->destNode[1] = (unsigned char)((fp_hdr->destNode >> 8) & 0xff);
    raw_p->year[0] = (unsigned char)(fp_hdr->pdate.year & 0xff);
    raw_p->year[1] = (unsigned char)((fp_hdr->pdate.year >> 8) & 0xff);
    raw_p->month[0] = (unsigned char)(fp_hdr->pdate.month & 0xff);
    raw_p->month[1] = (unsigned char)((fp_hdr->pdate.month >> 8) & 0xff);
    raw_p->day[0] = (unsigned char)(fp_hdr->pdate.day & 0xff);
    raw_p->day[1] = (unsigned char)((fp_hdr->pdate.day >> 8) & 0xff);
    raw_p->hour[0] = (unsigned char)(fp_hdr->pdate.hour & 0xff);
    raw_p->hour[1] = (unsigned char)((fp_hdr->pdate.hour >> 8) & 0xff);
    raw_p->minute[0] = (unsigned char)(fp_hdr->pdate.min & 0xff);
    raw_p->minute[1] = (unsigned char)((fp_hdr->pdate.min >> 8) & 0xff);
    raw_p->second[0] = (unsigned char)(fp_hdr->pdate.sec & 0xff);
    raw_p->second[1] = (unsigned char)((fp_hdr->pdate.sec >> 8) & 0xff);
    raw_p->baud[0] = (unsigned char)(fp_hdr->baud & 0xff);
    raw_p->baud[1] = (unsigned char)((fp_hdr->baud >> 8) & 0xff);
    raw_p->PacketType[0] = (unsigned char)(fp_hdr->PacketType & 0xff);
    raw_p->PacketType[1] = (unsigned char)((fp_hdr->PacketType >> 8) & 0xff);
    raw_p->origNet[0] = (unsigned char)(fp_hdr->origNet & 0xff);
    raw_p->origNet[1] = (unsigned char)((fp_hdr->origNet >> 8) & 0xff);
    raw_p->destNet[0] = (unsigned char)(fp_hdr->destNet & 0xff);
    raw_p->destNet[1] = (unsigned char)((fp_hdr->destNet >> 8) & 0xff);
    raw_p->prodCode = (unsigned char)(fp_hdr->prodCode & 0xff);
    raw_p->serialNo = (unsigned char)(fp_hdr->serialNo & 0xff);
    memcpy(raw_p->password, fp_hdr->password, sizeof raw_p->password);
    raw_p->origZone[0] = (unsigned char)(fp_hdr->origZone & 0xff);
    raw_p->origZone[1] = (unsigned char)((fp_hdr->origZone >> 8) & 0xff);
    raw_p->destZone[0] = (unsigned char)(fp_hdr->destZone & 0xff);
    raw_p->destZone[1] = (unsigned char)((fp_hdr->destZone >> 8) & 0xff);
    raw_p->auxNet[0] = (unsigned char)raw_p->destNet[0];
    raw_p->auxNet[1] = (unsigned char)raw_p->destNet[1];
    raw_p->CWvalidationCopy[0] = (unsigned char)
         ((fp_hdr->capability >> 8) & 0xff);
    raw_p->CWvalidationCopy[1] = (unsigned char)(fp_hdr->capability & 0xff);
    raw_p->CapabilWord[0] = raw_p->CWvalidationCopy[1];
    raw_p->CapabilWord[1] = raw_p->CWvalidationCopy[0];
    raw_p->prodCodeH = (unsigned char)((fp_hdr->prodCode >> 8) & 0xff);
    raw_p->origZone2[0] = raw_p->origZone[0];
    raw_p->origZone2[1] = raw_p->origZone[1];
    raw_p->destZone2[0] = raw_p->destZone[0];
    raw_p->destZone2[1] = raw_p->destZone[1];
    raw_p->Revision = (unsigned char)fp_hdr->Revision;
    raw_p->origPoint[0] = (unsigned char)(fp_hdr->origPoint & 0xff);
    raw_p->origPoint[1] = (unsigned char)((fp_hdr->origPoint >> 8) & 0xff);
    raw_p->destPoint[0] = (unsigned char)(fp_hdr->destPoint & 0xff);
    raw_p->destPoint[1] = (unsigned char)((fp_hdr->destPoint >> 8) & 0xff);
    memcpy(raw_p->prodData, fp_hdr->prodData, sizeof raw_p->prodData);
    return (0);
}    
