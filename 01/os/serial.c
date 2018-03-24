#include "defines.h"
#include "serial.h"

#define SERIAL_SCI_NUM 3

#define H8_3069F_SCI0 ((volatile struct h8_3069f_sci *)0xffffb0)
#define H8_3069F_SCI1 ((volatile struct h8_3069f_sci *)0xffffb8)
#define H8_3069F_SCI2 ((volatile struct h8_3069f_sci *)0xffffc0)

struct h8_3069f_sci{
    volatile uint8 smr; /* シリアル通信のモード設定 */
    volatile uint8 brr; /* ボーレートの設定 */
    volatile uint8 scr; /* 送受信の有効・無効など。シリアルコントロールレジスタ*/
    volatile uint8 tdr; /* 送信したい1文字を書き込む */
    volatile uint8 ssr; /* 送信完了・受信完了などを表す */
    volatile uint8 rdr; /* 受信した1文字を読み出す */
    volatile uint8 scmr;
};

/* SMR: シリアル通信のモード設定 */
/*
    0, 1: クロックセレクト、共にゼロならばクロックをそのまま利用する
    3: ストップビット長、0で1ビット、1で2ビット
    4: パリティの種類、0で偶数パリティ、1で奇数パリティ
    5: 0でパリティ無効、1でパリティ有効
    6: データ長、0で8ビット、1で7ビット
    7: 0で調歩同期式モード、1でクロック同期式モード
*/ 
#define H8_3069F_SCI_SMR_CKS_PER1 (0<<0) 
#define H8_3069F_SCI_SMR_CKS_PER4 (1<<0) 
#define H8_3069F_SCI_SMR_CKS_PER16 (2<<0)
#define H8_3069F_SCI_SMR_CKS_PER64 (3<<0)
#define H8_3069F_SCI_SMR_MP (1<<2) 
#define H8_3069F_SCI_SMR_STOP (1<<3)
#define H8_3069F_SCI_SMR_OE (1<<4)
#define H8_3069F_SCI_SMR_PE (1<<5)
#define H8_3069F_SCI_SMR_CHR (1<<6)
#define H8_3069F_SCI_SMR_CA (1<<7)

/* SCR: 送受信の有効/無効など シリアルコントロールレジスタ */
/*
    0, 1: クロックイネーブル、ひとまず共にゼロで良い
    4: 受信イネーブル、1で受信開始
    5: 送信イネーブル、1で送信開始
    6: 受信割り込みイネーブル、1で受信割り込み有効
    7: 送信割り込みイネーブル、1で送信割り込み有効
*/
#define H8_3069F_SCI_SCR_CKE0 (1<<0)
#define H8_3069F_SCI_SCR_CKE1 (1<<1)
#define H8_3069F_SCI_SCR_TEIE (1<<2)
#define H8_3069F_SCI_SCR_MPIE (1<<3)
#define H8_3069F_SCI_SCR_RE (1<<4) /* 受信有効*/
#define H8_3069F_SCI_SCR_TE (1<<5) /* 送信有効 */
#define H8_3069F_SCI_SCR_RIE (1<<6) /* 受信割り込み有効 */
#define H8_3069F_SCI_SCR_TIE (1<<7) /* 送信割り込み有効 */

/* SSR: 送信完了/受信完了などを表す */
#define H8_3069F_SCI_SSR_MPBT (1<<0)
#define H8_3069F_SCI_SSR_MPB (1<<1)
#define H8_3069F_SCI_SSR_TEND (1<<2)
#define H8_3069F_SCI_SSR_PER (1<<3)
#define H8_3069F_SCI_SSR_FERERS (1<<4)
#define H8_3069F_SCI_SSR_ORER (1<<5)
#define H8_3069F_SCI_SSR_RDRF (1<<6) /* 受信完了 */
#define H8_3069F_SCI_SSR_TDRE (1<<7) /* 送信完了 */

static struct{
    volatile struct h8_3069f_sci * sci;
} regs[SERIAL_SCI_NUM] = {
    { H8_3069F_SCI0 },
    { H8_3069F_SCI1 },
    { H8_3069F_SCI2 },
};

/* デバイス初期化 */
int serial_init(int index)
{
    volatile struct h8_3069f_sci *sci = regs[index].sci;

    sci->scr = 0;
    sci->smr = 0;
    sci->brr = 64;
    sci->scr = H8_3069F_SCI_SCR_RE | H8_3069F_SCI_SCR_TE; /* 送受信可能 */
    sci->ssr = 0;

    return 0;
}

/* 送受信可能か？ */
int serial_is_send_enable(int index)
{
    volatile struct h8_3069f_sci *sci = regs[index].sci;
    return (sci->ssr & H8_3069F_SCI_SSR_TDRE);
}

/* 一文字送信 */
int serial_send_byte(int index, unsigned char c)
{
    volatile struct h8_3069f_sci *sci = regs[index].sci;

    /* 送信可能になるまで待つ */
    while (!serial_is_send_enable(index))
        ;
    sci->tdr = c;
    sci->ssr &= ~H8_3069F_SCI_SSR_TDRE; /* 送信開始 */

    return 0;
}

/* 受信可能か */
int serial_is_recv_enable(int index){
    volatile struct h8_3069f_sci *sci = regs[index].sci;
    return (sci->ssr & H8_3069F_SCI_SSR_RDRF);
}

/* 一文字受信 */
unsigned char serial_recv_byte(int index){
    volatile struct h8_3069f_sci *sci = regs[index].sci;
    unsigned char c;

    /* 受信文字が来るまで */
    while(!serial_is_recv_enable(index))
        ;
    c = sci->rdr;
    sci->ssr &= ~H8_3069F_SCI_SSR_RDRF; /* 受信完了 */

    return c;
}
