/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	mt_cmd.h

	Abstract:

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifndef __MT_CMD_H__
#define __MT_CMD_H__

struct cmd_msg;
struct _MT_SWITCH_CHANNEL_CFG;
struct _MT_RTS_THRESHOLD_T;
struct _MT_RDG_CTRL_T;
struct _BSS_INFO_ARGUMENT_T;
struct _STA_ADMIN_CONFIG;

#ifdef BACKGROUND_SCAN_SUPPORT
struct _MT_BGND_SCAN_CFG;
struct _MT_BGND_SCAN_NOTIFY;
#endif /* BACKGROUND_SCAN_SUPPORT */

typedef VOID (*MSG_RSP_HANDLER)(struct cmd_msg *msg, char *payload, UINT16 payload_len);

#define CMD_QUERY 0
#define CMD_SET 1
#define CMD_NA 3

#define EXT_CMD_NA 0
#define P1_Q0 0x8000
#define CPU_TX_PORT 2
#define EFUSE_BLOCK_SIZE 16

#define NEW_MCU_INIT_CMD_API 1

#define MT_IGNORE_PAYLOAD_LEN_CHECK 0xffff


#define ROM_CODE_CMD_HEADER_LEN	12

#define BAND0                       0
#define BAND1                       1
#define BAND_NUM                    2
#define SKU_TABLE_SIZE             49
#define SKU_TOTAL_SIZE             53
#define SKU_TX_SPATIAL_STREAM_NUM   4

enum EEPROM_STORAGE_TYPE {
	EEPROM_PROM = 0,
	EEPROM_EFUSE = 1,
	EEPROM_FLASH = 2,
};

enum MCU_SRC_TO_DEST_INDEX_TYPE{
    HOST2N9     = 0,
    CR42N9      = 1,
    HOST2CR4    = 2,
    HOST2CR4N9  = 3
};


#define N92HOST HOST2N9
#define CR42HOST HOST2CR4

enum cmd_msg_state {
	illegal,                // 0
	tx_start,               // 1
	tx_kickout_fail,        // 2
	tx_timeout_fail,        // 3
	tx_retransmit,          // 4
	tx_done,                // 5
	wait_cmd_out,           // 6
	wait_cmd_out_and_ack,   // 7
	wait_ack,               // 8
	rx_start,               // 9
	rx_receive_fail,        // a
	rx_done,                // b
};

typedef enum _ENUM_EXT_CMD_CR4_SET_ID_T {
                CR4_SET_ID_HELP = 0,
                CR4_SET_ID_MAX_AMSDU_QUEUE_NUM = 1,
                CR4_SET_ID_READ_ONE_RX_TOKEN_WRITE_BACK_FIFO = 2,
                CR4_SET_ID_READ_ALL_RX_TOKEN_WRITE_BACK_FIFO = 3,
                CR4_SET_ID_TRIGGER_PDMA_RECIEVE = 4,
                CR4_SET_ID_TRIGGER_ASSERT = 5,
                CR4_SET_ID_TX_FRAGMENT_THRESHOLD = 6,
                CR4_SET_ID_MEM_QUERY = 7,
                CR4_SET_ID_MEM_SET = 8,
                CR4_SET_ID_STOP_RX_PDMA_RING_DEQUEUE = 9,
                CR4_SET_ID_START_RX_PDMA_RING_DEQUEUE = 0xa,
                CR4_SET_ID_CONFIG_POWER_SAVING_MODE = 0xb,
                CR4_SET_ID_CONFIG_TX_DELAY_MODE = 0xc,
                CR4_SET_ID_CONFIG_STA_AMSDU_MAX_NUM = 0xd,
                CR4_SET_ID_NUM
} ENUM_EXT_CMD_CR4_SET_ID_T, *P_ENUM_EXT_CMD_CR4_SET_ID_T;


/**
 * The CONTROL_FLAG_INIT_COMBINATION enum parameters used by Host to init
 * the flags of one msg.
 *
 * @BIT0        NEED RSP        specific the cmd need event response.
 * @BIT1        NEED RETRY      specific if the cmd fail, it can retry.
 * @BIT2        NEED WAIT       specific the cmd will sync with FW.
 * @BIT3        CMD_SET_QUERY   specific cmd type is belong to set or query.
 *
 * @BIT4        CMD_NA          specific cmd type is Neither set nor query.
 *                              Note:
 *                                      bit 3 & 4 always mutual
 *                                      exclusive to each other.
 *
 * @BIT5        LEN_VAR         specific cmd type is expected event len variable.
 *                              The RX handler will only check the minimum event
 *                              size which specific in msg.
 *
 * @BIT7        NEED FRAG       expected cmd len exceed default block size
 *                              the cmd will be sent in Frag way.
 *
 * @Others                      Reserved
 *
 */
enum CONTROL_FLAG_INIT_COMBINATION
{
    INIT_CMD_QUERY 			            = 0x00,
    INIT_CMD_QUERY_AND_RSP              = 0x01,
    INIT_CMD_QUERY_AND_WAIT_RSP 	    = 0x05,
    INIT_CMD_QUERY_AND_WAIT_RETRY_RSP 	= 0x07,

    INIT_CMD_SET 			            = 0x08,
    INIT_CMD_SET_AND_RSP 			    = 0x09,
    INIT_CMD_SET_AND_RETRY 		        = 0x0A,
    INIT_CMD_SET_AND_WAIT_RSP 		    = 0x0D,
    INIT_CMD_SET_AND_WAIT_RETRY         = 0x0E,
    INIT_CMD_SET_AND_WAIT_RETRY_RSP 	= 0x0F,

    INIT_CMD_NA 			            = 0x10,
    INIT_CMD_NA_AND_WAIT_RETRY_RSP 	    = 0x17,

    INIT_LEN_VAR_CMD_SET_AND_WAIT_RETRY_RSP = 0x27,
};

#define CMD_FLAGS_MASK                          (0x3F)

#define CMD_FLAG_CMD_LEN_VAR_MASK               (0x20)
#define CMD_FLAG_CMD_LEN_VAR_OFFSET             (5)

#define CMD_FLAG_CMD_NA_MASK                    (0x10)
#define CMD_FLAG_CMD_NA_OFFSET                  (4)

#define CMD_FLAG_SET_QUERY_MASK                 (0x08)
#define CMD_FLAG_SET_QUERY_OFFSET               (3)

#define CMD_FLAG_NEED_SYNC_WITH_FW_MASK         (0x04)
#define CMD_FLAG_NEED_SYNC_WITH_FW_OFFSET       (2)

#define CMD_FLAG_NEED_RETRY_MASK                (0x02)
#define CMD_FLAG_NEED_RETRY_OFFSET              (1)

#define CMD_FLAG_NEED_FW_RSP_MASK               (0x01)
#define CMD_FLAG_NEED_FW_RSP_OFFSET             (0)

/* CMD ATTRIBUTE initlized related Marco */
#define SET_CMD_ATTR_MCU_DEST(attr, cmd_dest) \
    (attr).mcu_dest = cmd_dest;
#define SET_CMD_ATTR_TYPE(attr, cmd_type) \
    (attr).type = cmd_type;
#define SET_CMD_ATTR_EXT_TYPE(attr, ext_cmd_type) \
    (attr).ext_type = ext_cmd_type;
#define SET_CMD_ATTR_CTRL_FLAGS(attr, cmd_flags) \
    (attr).ctrl.flags = cmd_flags;
#define SET_CMD_ATTR_RSP_EXPECT_SIZE(attr, size) \
    (attr).ctrl.expect_size = (UINT16)size;
#define SET_CMD_ATTR_RSP_WAIT_MS_TIME(attr, wait_time) \
    (attr).ctrl.wait_ms_time = (UINT16)wait_time;
#define SET_CMD_ATTR_RSP_WB_BUF_IN_CALBK(attr, write_back_buffer_in_callback) \
    (attr).rsp.wb_buf_in_calbk = (UINT32 *)write_back_buffer_in_callback;
#define SET_CMD_ATTR_RSP_HANDLER(attr, callback) \
    (attr).rsp.handler = callback;

/* CMD ATTRIBUTE ctrl flags related Macro */
#define GET_CMD_ATTR_CTRL_FLAGS(attr) \
    ((attr.ctrl.flags & CMD_FLAGS_MASK)

#define IS_CMD_ATTR_LEN_VAR_FLAG_SET(attr) \
    (((attr).ctrl.flags & CMD_FLAG_CMD_LEN_VAR_MASK) ? TRUE : FALSE)
#define IS_CMD_ATTR_NA_FLAG_SET(attr) \
    (((attr).ctrl.flags & CMD_FLAG_CMD_NA_MASK) ? TRUE : FALSE)
#define IS_CMD_ATTR_SET_QUERY_FLAG_SET(attr) \
    (((attr).ctrl.flags & CMD_FLAG_SET_QUERY_MASK) ? TRUE : FALSE)
#define IS_CMD_ATTR_NEED_SYNC_WITH_FW_FLAG_SET(attr) \
    (((attr).ctrl.flags & CMD_FLAG_NEED_SYNC_WITH_FW_MASK) ? TRUE : FALSE)
#define IS_CMD_ATTR_NEED_RETRY_FLAG_SET(attr) \
    (((attr).ctrl.flags & CMD_FLAG_NEED_RETRY_MASK) ? TRUE : FALSE)
#define IS_CMD_ATTR_NEED_FW_RSP_FLAG_SET(attr) \
    (((attr).ctrl.flags & CMD_FLAG_NEED_FW_RSP_MASK) ? TRUE : FALSE)



/* CMD MSG initlize related Marco */
#define SET_CMD_MSG_PORT_QUEUE_ID(msg, hardware_port) \
    (msg)->pq_id = hardware_port;
#define SET_CMD_MSG_SEQUENCE(msg, msg_sequence) \
    (msg)->seq = msg_sequence;
#define SET_CMD_MSG_RETRY_TIMES(msg, cmd_retry_times) \
    (msg)->retry_times = cmd_retry_times;

#define SET_CMD_MSG_MCU_DEST(msg, cmd_dest) \
    SET_CMD_ATTR_MCU_DEST((msg)->attr, cmd_dest)
//(msg)->attr.mcu_dest = msg_dest;
#define SET_CMD_MSG_TYPE(msg, cmd_type) \
    SET_CMD_ATTR_TYPE((msg)->attr, cmd_type)
//(msg)->attr.type = cmd_type;
#define SET_CMD_MSG_EXT_TYPE(msg, ext_cmd_type) \
    SET_CMD_ATTR_EXT_TYPE((msg)->attr, ext_cmd_type)
//(msg)->attr.ext_type = ext_cmd_type;
#define SET_CMD_MSG_CTRL_FLAGS(msg, cmd_flags) \
    SET_CMD_ATTR_CTRL_FLAGS((msg)->attr, cmd_flags)
//(msg)->attr.ctrl.flags = cmd_flags;
#define SET_CMD_MSG_CTRL_RSP_EXPECT_SIZE(msg, size) \
    SET_CMD_ATTR_RSP_EXPECT_SIZE((msg)->attr, size)
//(msg)->attr.ctrl.expect_size = size;
#define SET_CMD_MSG_CTRL_RSP_WAIT_MS_TIME(msg, wait_time) \
    SET_CMD_ATTR_RSP_WAIT_MS_TIME((msg)->attr, wait_time)
//(msg)->attr.ctrl.wait_ms_time = wait_time;
#define SET_CMD_MSG_RSP_WB_BUF_IN_CALBK(msg, write_back_buffer_in_callback) \
    SET_CMD_ATTR_RSP_WB_BUF_IN_CALBK((msg)->attr, write_back_buffer_in_callback)
//(msg)->attr.rsp.wb_buf_in_calbk = (UINT32 *)write_back_buffer_in_callback;
#define SET_CMD_MSG_RSP_HANDLER(msg, callback) \
    SET_CMD_ATTR_RSP_HANDLER((msg)->attr, callback)
//(msg)->attr.rsp.handler = callback;


/* CMD MSG flags related Macro, part I */
#define SET_MSG_FLAGS_VALUE(msg, val) \
    (msg)->attr.ctrl.flags = (((msg)->attr.ctrl.flags & CMD_FLAGS_MASK) \
                                            | ((val) & CMD_FLAGS_MASK))
#define GET_MSG_FLAGS_VALUE(msg) \
    (((msg)->attr.ctrl.flags & CMD_FLAGS_MASK)) ? TRUE : FALSE

#define SET_MSG_CMD_NA_FLAG(msg) \
    (msg)->attr.ctrl.flags = (((msg)->attr.ctrl.flags & CMD_FLAGS_MASK) \
                                    | (1 << CMD_FLAG_CMD_NA_OFFSET))
#define GET_MSG_CMD_NA_FLAG(msg) \
    (((msg)->attr.ctrl.flags & CMD_FLAG_CMD_NA_MASK)) ? TRUE : FALSE

#define SET_MSG_SET_QUERY_FLAG(msg) \
    (msg)->attr.ctrl.flags = (((msg)->attr.ctrl.flags & CMD_FLAGS_MASK) \
    | (1 << CMD_FLAG_SET_QUERY_OFFSET))

#define GET_MSG_SET_QUERY_FLAG(msg) \
    (((msg)->attr.ctrl.flags & CMD_FLAG_SET_QUERY_MASK)) ? TRUE : FALSE

#define SET_MSG_NEED_SYNC_WITH_FW_FLAG(msg) \
    (msg)->attr.ctrl.flags = (((msg)->attr.ctrl.flags & CMD_FLAGS_MASK) \
    | (1 << CMD_FLAG_NEED_SYNC_WITH_FW_OFFSET))

#define GET_MSG_NEED_SYNC_WITH_FW_FLAG(msg) \
    (((msg)->attr.ctrl.flags & CMD_FLAG_NEED_SYNC_WITH_FW_MASK)) ? TRUE : FALSE

#define SET_MSG_NEED_RETRY_FLAG(msg) \
    (msg)->attr.ctrl.flags = (((msg)->attr.ctrl.flags & CMD_FLAGS_MASK) \
    | (1 << CMD_FLAG_NEED_RETRY_OFFSET))

#define GET_MSG_NEED_RETRY_FLAG(msg) \
    (((msg)->attr.ctrl.flags & CMD_FLAG_NEED_RETRY_MASK)) ? TRUE : FALSE

#define SET_MSG_NEED_FW_RSP_FLAG(msg) \
    (msg)->attr.ctrl.flags = (((msg)->attr.ctrl.flags & CMD_FLAGS_MASK) \
    | (1 << CMD_FLAG_NEED_FW_RSP_OFFSET))
#define GET_MSG_NEED_FW_RSP_FLAG(msg) \
    (((msg)->attr.ctrl.flags & CMD_FLAG_NEED_FW_RSP_MASK)) ? TRUE : FALSE

/* CMD MSG flags related Macro, part II */
#define GET_CMD_MSG_CTRL_FLAGS(msg) \
    GET_CMD_ATTR_CTRL_FLAGS((msg)->attr)

#define IS_CMD_MSG_LEN_VAR_FLAG_SET(msg) \
    IS_CMD_ATTR_LEN_VAR_FLAG_SET((msg)->attr)
#define IS_CMD_MSG_NA_FLAG_SET(msg) \
    IS_CMD_ATTR_NA_FLAG_SET((msg)->attr)
#define IS_CMD_MSG_SET_QUERY_FLAG_SET(msg) \
    IS_CMD_ATTR_SET_QUERY_FLAG_SET((msg)->attr)
#define IS_CMD_MSG_NEED_SYNC_WITH_FW_FLAG_SET(msg) \
    IS_CMD_ATTR_NEED_SYNC_WITH_FW_FLAG_SET((msg)->attr)
#define IS_CMD_MSG_NEED_RETRY_FLAG_SET(msg) \
    IS_CMD_ATTR_NEED_RETRY_FLAG_SET((msg)->attr)
#define IS_CMD_MSG_NEED_FW_RSP_FLAG_SET(msg) \
    IS_CMD_ATTR_NEED_FW_RSP_FLAG_SET((msg)->attr)


#define IS_EXT_CMD_AND_SET_NEED_RSP(m) \
    (((((struct cmd_msg *)(m))->attr.type) == EXT_CID) \
    && (IS_CMD_MSG_NEED_FW_RSP_FLAG_SET(m))) ? TRUE : FALSE

typedef struct _CTRL_PARAM {
    UINT8       flags;
    UINT16      expect_size;
    UINT16      wait_ms_time;
} CTRL_PARAM, *P_CTRL_PARAM;


typedef struct _RSP_PARAM {
    UINT32         *wb_buf_in_calbk;
    MSG_RSP_HANDLER handler;
} RSP_PARAM, *P_RSP_PARAM;


typedef struct _CMD_ATTRIBUTE {
    UINT16          mcu_dest;
    UINT8           type;
    UINT8           ext_type;
    CTRL_PARAM      ctrl;
    RSP_PARAM       rsp;
} CMD_ATTRIBUTE, *P_CMD_ATTRIBUTE;

/**
 * The cmd_msg is used by Host to communicate with FW.(i.e issue request)
 * The FW may response event or not depend on cmd flags setting.
 *
 * ----------------------------- Common Part -----------------------------
 * @Field attr                  cmd attribute descriptation
 * @Filed pq_id                 specific cmd physical target
 * @Field seq                   cmd sequence
 * @Filed retransmit_times      specific cmd retransmit_times
 * @Field priv
 * @Filed net_pkt
 * @Field cmd_tx_len

 * ------------------------------  OS Part  ------------------------------
 *
 *
 */
struct cmd_msg {
    CMD_ATTRIBUTE       attr;
    UINT16              pq_id;
	UINT8               seq;
    UINT8               retry_times;
    enum cmd_msg_state  state;

	VOID                *priv;
	VOID                *net_pkt;
    UINT32              wcid;       /* Index of MacTableEntry */
    UINT32              cmd_tx_len;

    UINT8               need_sent_in_frag;
    UINT8               frag_cmd_sent_count;
    UINT16              orig_cmd_whole_len;

    UINT8               *back_orig_frag_fw_txd_and_cmd_struct_content;
   // UINT8               *back_orig_frag_cmd_struct_content;
    UINT32              back_orig_frag_cmd_struct_content_offset;
    UINT8               receive_frag_event_count;

    ULONG              sending_time_in_jiffies;        /* record the time in jiffies for send-the-command */
    ULONG              receive_time_in_jiffies;        /* record the time in jiffies for N9-firmware-response */


#if defined(MT7615) || defined(MT7622)
	UINT32              ucS2DIndex; /* Index for Src to Dst in CMD usage */
#endif /* defined(MT7615) || defined(MT7622) */
#if !defined(COMPOS_TESTMODE_WIN) && !defined(COMPOS_WIN)
	DL_LIST             list;
    RTMP_OS_COMPLETION  ack_done;
        #else /* (COMPOS_TESTMODE_WIN) || defined(COMPOS_WIN) */
    UINT32              current_pos;    //current append length
#endif

    UINT32		cmd_return_status;
};


#ifdef RT_BIG_ENDIAN
typedef	union _FW_TXD_0 {
	struct {
		UINT32 pq_id:16;
		UINT32 length:16;
	} field;
	UINT32 word;
} FW_TXD_0;
#else
typedef union _FW_TXD_0 {
	struct {
		UINT32 length:16;
		UINT32 pq_id:16;
	} field;
	UINT32 word;
} FW_TXD_0;
#endif

#define PKT_ID_CMD 0xA0
#define PKT_ID_EVENT 0xE000

#ifdef RT_BIG_ENDIAN
typedef union _FW_TXD_1 {
	struct {
		UINT32 seq_num:8;
		UINT32 set_query:8;
		UINT32 pkt_type_id:8;
		UINT32 cid:8;
	} field;
#if defined(MT7615) || defined(MT7622)
	struct {
		UINT32 seq_num:6;
		UINT32 pkt_ft:2;
		UINT32 set_query:8;
		UINT32 pkt_type_id:8;
		UINT32 cid:8;
	} field1;
#endif /* defined(MT7615) || defined(MT7622) */
	UINT32 word;
} FW_TXD_1;
#else
typedef union _FW_TXD_1 {
	struct {
		UINT32 cid:8;
		UINT32 pkt_type_id:8;
		UINT32 set_query:8;
		UINT32 seq_num:8;
	} field;
#if defined(MT7615) || defined(MT7622)
	struct {
		UINT32 cid:8;
		UINT32 pkt_type_id:8;
		UINT32 set_query:8;
		UINT32 pkt_ft:2;
		UINT32 seq_num:6;
	} field1;
#endif /* defined(MT7615) || defined(MT7622) */
	UINT32 word;
} FW_TXD_1;
#endif


#define EXT_CID_OPTION_NEED_ACK 1
#define EXT_CID_OPTION_NO_NEED_ACK 0


#ifdef RT_BIG_ENDIAN
typedef union _FW_TXD_2 {
	struct {
		UINT32 ext_cid_option:8;
		UINT32 ucS2DIndex:8;
		UINT32 ext_cid:8;
		UINT32 ucD2B0Rev:8;
	} field;
	UINT32 word;
} FW_TXD_2;
#else
typedef union _FW_TXD_2 {
	struct {
		UINT32 ucD2B0Rev:8;
		UINT32 ext_cid:8;
		UINT32 ucS2DIndex:8;
		UINT32 ext_cid_option:8;
	} field;
	UINT32 word;
} FW_TXD_2;
#endif /* RT_BIG_ENDIAN */


/*
 * FW TX descriptor
 */
typedef struct GNU_PACKED _FW_TXD_ {
	FW_TXD_0 fw_txd_0;
	FW_TXD_1 fw_txd_1;
	FW_TXD_2 fw_txd_2;
	UINT32 au4D3toD7rev[5];
} FW_TXD;

/*
 * Command type table   layer 0
 */
enum MT_CMD_TYPE {
	MT_TARGET_ADDRESS_LEN_REQ = 0x01,
	MT_FW_START_REQ = 0x02,
	INIT_CMD_ACCESS_REG = 0x3,
	MT_PATCH_START_REQ = 0x05,
	MT_PATCH_FINISH_REQ = 0x07,
	MT_PATCH_SEM_CONTROL = 0x10,
	MT_HIF_LOOPBACK = 0x20,
	CMD_CH_PRIVILEGE = 0x20,
	CMD_ACCESS_REG = 0xC2,
    INIT_CMD_ID_CR4 = 0xC4, /* for CR4test */
	EXT_CID = 0xED,
	MT_FW_SCATTER = 0xEE,
	MT_RESTART_DL_REQ = 0xEF,
};

#define CMD_START_LOAD		0x01
#define CMD_RAM_START_RUN	0x02
#define CMD_ROM_ACCESS_REG			0x03
#define CMD_PATCH_SEMAPHORE_CONTROL		0x10
#define CMD_PATCH_START			0x05
#define CMD_PATCH_FINISH			0x07
#define CMD_LOOPBACK_TEST			0x20

/*
 * Extension Command
 */
enum EXT_CMD_TYPE {
	EXT_CMD_ID_EFUSE_ACCESS  =0x01,
	EXT_CMD_RF_REG_ACCESS = 0x02,
	EXT_CMD_RF_TEST = 0x04,
	EXT_CMD_RADIO_ON_OFF_CTRL = 0x05,
	EXT_CMD_WIFI_RX_DISABLE = 0x06,
    EXT_CMD_PM_STATE_CTRL = 0x07,
	EXT_CMD_CHANNEL_SWITCH = 0x08,
	EXT_CMD_NIC_CAPABILITY = 0x09,
	EXT_CMD_PWR_SAVING = 0x0A,
	EXT_CMD_MULTIPLE_REG_ACCESS = 0x0E,
	EXT_CMD_AP_PWR_SAVING_CAPABILITY = 0xF,
	EXT_CMD_SEC_ADDREMOVE_KEY = 0x10,
	EXT_CMD_SET_TX_POWER_CTRL=0x11,
	EXT_CMD_THERMO_CAL =	0x12,
	EXT_CMD_FW_LOG_2_HOST = 0x13,
	EXT_CMD_PS_RETRIEVE_START = 0x14,
#ifdef CONFIG_MULTI_CHANNEL
       EXT_CMD_ID_MCC_OFFLOAD_START = 0x15,
       EXT_CMD_ID_MCC_OFFLOAD_STOP  = 0x16,
#endif /* CONFIG_MULTI_CHANNEL */
       EXT_CMD_ID_LED = 0x17,
    EXT_CMD_ID_PACKET_FILTER = 0x18,
	EXT_CMD_BT_COEX = 0x19,
	EXT_CMD_ID_PWR_MGT_BIT_WIFI = 0x1B,
	EXT_CMD_ID_GET_TX_POWER = 0x1C,
#if defined(MT_MAC)	&& (!defined(MT7636))
	EXT_CMD_ID_BF_ACTION=0x1E,
#endif /* MT_MAC && !MT7636 */

	EXT_CMD_EFUSE_BUFFER_MODE = 0x21,
	EXT_CMD_THERMAL_PROTECT = 0x23,
	EXT_CMD_ID_CLOCK_SWITCH_DISABLE = 0x24,
	EXT_CMD_STAREC_UPDATE = 0x25,
	EXT_CMD_ID_BSSINFO_UPDATE  = 0x26,
	EXT_CMD_ID_EDCA_SET = 0x27,
	EXT_CMD_ID_SLOT_TIME_SET = 0x28,
	EXT_CMD_GET_THEMAL_SENSOR=0x2C,
	EXT_CMD_TMR_CAL = 0x2D,
	EXT_CMD_ID_DEVINFO_UPDATE = 0x2a,
	EXT_CMD_ID_NOA_OFFLOAD_CTRL = 0x2B,
    EXT_CMD_ID_WAKEUP_OPTION = 0x2E,
	EXT_CMD_ID_GET_TX_STATISTICS = 0x30,
	EXT_CMD_ID_WTBL_UPDATE = 0x32,
    EXT_CMD_ID_TRGR_PRETBTT_INT_EVENT = 0x33,
	EXT_CMD_TDLS_CHSW = 0x34,
#ifdef VOW_SUPPORT
    EXT_CMD_ID_DRR_CTRL = 0x36,
    EXT_CMD_ID_BSSGROUP_CTRL = 0x37,
    EXT_CMD_ID_VOW_FEATURE_CTRL = 0x38,
#endif /* VOW_SUPPORT */
#ifdef MT_DFS_SUPPORT    //Jelly20141229
	EXT_CMD_ID_RDD_ON_OFF_CTRL = 0x3A,
#endif
	EXT_CMD_ID_GET_MAC_INFO = 0x3C,
#ifdef CONFIG_HW_HAL_OFFLOAD
	EXT_CMD_ID_ATE_TEST_MODE = 0x3D,
#endif
    EXT_CMD_ID_PROTECT_CTRL = 0x3e,
    EXT_CMD_ID_RDG_CTRL = 0x3f,
#ifdef CFG_SUPPORT_MU_MIMO
    EXT_CMD_ID_MU_MIMO = 0x40,
#endif
	EXT_CMD_ID_SNIFFER_MODE = 0x42,
    EXT_CMD_ID_GENERAL_TEST = 0x41,
	EXT_CMD_ID_WIFI_HIF_CTRL = 0x43,
    EXT_CMD_ID_TMR_CTRL = 0x44,
	EXT_CMD_ID_DBDC_CTRL = 0x45,
	EXT_CMD_MAC_INIT_CTRL = 0x46,
	EXT_CMD_ID_RX_HDR_TRANS  = 0x47,
    EXT_CMD_ID_CONFIG_MUAR = 0x48,
    EXT_CMD_ID_BCN_OFFLOAD = 0x49,
#ifdef VOW_SUPPORT
    EXT_CMD_ID_RX_AIRTIME_CTRL = 0x4a,
    EXT_CMD_ID_AT_PROC_MODULE = 0x4b,
#endif /* VOW_SUPPORT */
#ifdef BACKGROUND_SCAN_SUPPORT
 	EXT_CMD_ID_BGND_SCAN_NOTIFY = 0x4D,
#endif /* BACKGROUND_SCAN_SUPPORT */
    EXT_CMD_ID_SET_RX_PATH = 0x4e,
	EXT_CMD_ID_EFUSE_FREE_BLOCK = 0x4f,
	EXT_CMD_ID_AUTO_BA = 0x51,
	EXT_CMD_ID_MCAST_CLONE = 0x52,
	EXT_CMD_ID_MULTICAST_ENTRY_INSERT = 0x53,
	EXT_CMD_ID_MULTICAST_ENTRY_DELETE = 0x54,
#ifdef CFG_SUPPORT_MU_MIMO_RA
    EXT_CMD_ID_MU_MIMO_RA = 0x55,
#endif
#ifdef INTERNAL_CAPTURE_SUPPORT
	EXT_CMD_ID_WIFI_SPECTRUM = 0x56,
#endif/* INTERNAL_CAPTURE_SUPPORT */
	EXT_CMD_ID_DUMP_MEM = 0x57,
    EXT_CMD_ID_TX_POWER_FEATURE_CTRL = 0x58,
#ifdef PRE_CAL_TRX_SET1_SUPPORT
    EXT_CMD_ID_RXDCOC_CAL_RESULT = 0x59,
    EXT_CMD_ID_TXDPD_CAL_RESULT = 0x60,
    EXT_CMD_ID_RDCE_VERIFY = 0x61,
#endif /* PRE_CAL_TRX_SET1_SUPPORT */
#if defined (RLM_CAL_CACHE_SUPPORT) || defined(PRE_CAL_TRX_SET2_SUPPORT)
    EXT_CMD_ID_TXLPF_CAL_INFO = 0x62,
    EXT_CMD_ID_TXIQ_CAL_INFO = 0x63,
    EXT_CMD_ID_TXDC_CAL_INFO = 0x64,
    EXT_CMD_ID_RXFI_CAL_INFO = 0x65,
    EXT_CMD_ID_RXFD_CAL_INFO = 0x66,
    EXT_CMD_ID_POR_CAL_INFO = 0x67,
#endif /* defined(RLM_CAL_CACHE_SUPPORT) || defined(PRE_CAL_TRX_SET2_SUPPORT) */
#ifdef RED_SUPPORT
	EXT_CMD_ID_RED_ENABLE = 0x68,
	EXT_CMD_ID_RED_SHOW_STA = 0x69,
	EXT_CMD_ID_RED_TARGET_DELAY = 0x6A,
#endif /* RED_SUPPORT */
	EXT_CMD_ID_PKT_BUDGET_CTRL_CFG = 0x6C,
	EXT_CMD_ID_TOAE_ENABLE = 0x6D,
	EXT_CMD_ID_BWF_LWC_ENABLE = 0x6E,
	EXT_CMD_ID_EDCCA_CTRL = 0x70,
#if defined(CONFIG_HOTSPOT_R2)  || defined(DSCP_QOS_MAP_SUPPORT)
	EXT_CMD_ID_HOTSPOT_INFO_UPDATE = 0x71,
#endif /* CONFIG_HOTSPOT_R2 */
#ifdef GREENAP_SUPPORT
	EXT_CMD_ID_GREENAP_CTRL = 0x73,
#endif /* GREENAP_SUPPORT */
	EXT_CMD_ID_SET_MAX_PHY_RATE = 0x74,
	EXT_CMD_ID_CP_SUPPORT = 0x75,
#ifdef PRE_CAL_TRX_SET2_SUPPORT
    EXT_CMD_ID_PRE_CAL_RESULT = 0x76,
#endif /* PRE_CAL_TRX_SET2_SUPPORT */	
#ifdef PA_TRIM_SUPPORT
    EXT_CMD_ID_CAL_RESTORE_FROM_FILE = 0x77,
#endif /* PA_TRIM_SUPPORT */
#ifdef LINK_TEST_SUPPORT
    EXT_CMD_ID_CMW_FEATURE_CTRL = 0x78,
#endif /* LINK_TEST_SUPPORT */
	EXT_CMD_ID_THERMAL_RECAL_MODE = 0x79,
#ifdef MWDS
    EXT_CMD_ID_MWDS_SUPPORT   =	0x80,
#endif
};

/* CR4 test */
typedef enum _EXT_CMD_ID_CR4_T
{
    EXT_CMD_ID_CR4_QUERY = 0,
    EXT_CMD_ID_CR4_SET,
    EXT_CMD_ID_CR4_CAPABILITY,
    EXT_CMD_ID_CR4_DEBUG,
    EXT_CMD_ID_CR4_MAX_NUM,
} EXT_CMD_ID_CR4_T;

typedef enum _EXT_CMD_CR4_QUERY_OPTION_T
{
    CR4_QUERY_OPTION_HELP = 0,
    CR4_QUERY_OPTION_SYSTEM = 1,
    CR4_QUERY_OPTION_STACK_BOUNDARY = 2,    
    CR4_QUERY_OPTION_OS_TASK = 3,
    CR4_QUERY_OPTION_PDMA_INFO = 4,
    CR4_QUERY_OPTION_PDMA_DEBUG_PROBE = 5,
    CR4_QUERY_OPTION_CPU_UTILIZATION = 6,
    CR4_QUERY_OPTION_PACKET_STATISTICS = 7,
    CR4_QUERY_OPTION_WIFI_INFO = 8,
    CR4_QUERY_OPTION_RESET_WIFI_INFO = 9,
    CR4_QUERY_OPTION_BSS_TLB_STA_RECORD = 0xa,
    CR4_QUERY_OPTION_CR4_STATUS_CR = 0xb,
    CR4_QUERY_OPTION_SHOW_CR4_INTERNAL_BUFFER_STATUS = 0xc,    
    CR4_QUERY_OPTION_DUMP_RX_REORDER_QUEUE_INFO = 0xd, 
    CR4_QUERY_OPTION_SHOW_RX_REORDER_QUEUE_LEN = 0xe, 
    CR4_QUERY_OPTION_SHOW_RXCUTDISP_REG_CONTENT = 0xf, 
    CR4_QUERY_OPTION_SHOW_CR4_CAPABILITY_DEBUG_SETTING = 0x10, 
    CR4_QUERY_OPTION_SHOW_CR4_POWER_SAVING_MODE = 0x11,
    CR4_QUERY_OPTION_GET_BSS_ACQ_PKT_NUM = 0x12,
    CR4_QUERY_OPTION_MAX_NUM,    
}EXT_CMD_CR4_QUERY_OPTION_T;

enum EXT_CMD_TAG_ID {
	EXT_CMD_TAG_RDG = 0x01,
	EXT_CMD_TAG_RXMAXLEN = 0x02,
	EXT_CMD_TAG_TR_STREAM = 0x03,
	EXT_CMD_TAG_UPDATE_BA = 0x04,
	EXT_CMD_TAG_SET_MacTXRX = 0x05,
	EXT_CMD_TAG_SET_TXSCF = 0x06,
	EXT_CMD_TAG_SET_RXPATH = 0x07,
	EXT_CMD_TAG_RX_HDR_TRNS = 0x08,
	EXT_CMD_TAG_RX_HDR_TRNSBL = 0x09,
	EXT_CMD_TAG_RX_GROUP = 0x0a,
};

enum
{
	CH_SWITCH_BY_NORMAL_TX_RX = 0,
	CH_SWITCH_INTERNAL_USED_BY_FW_0 = 1,
	CH_SWITCH_INTERNAL_USED_BY_FW_1 = 1,
	CH_SWITCH_SCAN = 3,
	CH_SWITCH_INTERNAL_USED_BY_FW_3 = 4,
	CH_SWITCH_DFS = 5, //Jelly20150123
	CH_SWITCH_BACKGROUND_SCAN_START = 6,
	CH_SWITCH_BACKGROUND_SCAN_RUNNING = 7,
	CH_SWITCH_BACKGROUND_SCAN_STOP = 8,
	CH_SWITCH_SCAN_BYPASS_DPD = 9
};

#ifdef LINK_TEST_SUPPORT
typedef enum _CMW_ACTION_CATEGORY
{
    CMW_TX_CSD = 0,
    CMW_RX,
    CMW_TXPWR,
    CMW_TXPWR_UP_TABLE,
    CMW_ACR,
    CMW_RCPI,
    CMW_SEIDX,
    CMW_RCPI_MA,
    CMW_ACTION_NUM
} CMW_ACTION_CATEGORY, *P_CMW_ACTION_CATEGORY;
#endif /* LINK_TEST_SUPPORT */

#ifdef CONFIG_MULTI_CHANNEL
typedef struct GNU_PACKED _EXT_CMD_MCC_START_T
{
    // Common setting from DW0~3
    // DW0
    UINT16     u2IdleInterval;
    UINT8      ucRepeatCnt;
    UINT8      ucStartIdx;

    // DW1
    UINT32     u4StartInstant;

    // DW2,3
    UINT16     u2FreePSEPageTh;
    UINT8      ucPreSwitchInterval;
    UINT8      aucReserved0[0x5];

    // BSS0 setting from DW4~7
    // DW4
    UINT8      ucWlanIdx0;
    UINT8      ucPrimaryChannel0;
    UINT8      ucCenterChannel0Seg0;
    UINT8      ucCenterChannel0Seg1;

    // DW5
    UINT8      ucBandwidth0;
    UINT8      ucTrxStream0;
    UINT16     u2StayInterval0;

    // DW6
    UINT8     ucRole0;
    UINT8     ucOmIdx0;
    UINT8     ucBssIdx0;
    UINT8     ucWmmIdx0;

    // DW7
    UINT8     aucReserved1[0x4];

    // BSS1 setting from DW8~11
    // DW8
    UINT8     ucWlanIdx1;
    UINT8     ucPrimaryChannel1;
    UINT8     ucCenterChannel1Seg0;
    UINT8     ucCenterChannel1Seg1;

    // DW9
    UINT8     ucBandwidth1;
    UINT8     ucTrxStream1;
    UINT16    u2StayInterval1;

    // DW10
    UINT8     ucRole1;
    UINT8     ucOmIdx1;
    UINT8     ucBssIdx1;
    UINT8     ucWmmIdx1;

    // DW11
    UINT8     aucReserved2[0x4];
} EXT_CMD_MCC_START_T, *P_EXT_CMD_MCC_START_T;

typedef struct GNU_PACKED _EXT_CMD_MCC_STOP_T
{
    //DW0
    UINT8      ucParkIdx;
    UINT8      ucAutoResumeMode;
    UINT16     u2AutoResumeInterval;

    //DW1
    UINT32     u4AutoResumeInstant;

    //DW2
    UINT16     u2IdleInterval;
    UINT8      aucReserved[2];

    //DW3
    UINT16     u2StayInterval0;
    UINT16     u2StayInterval1;
} EXT_CMD_MCC_STOP_T, *P_EXT_CMD_MCC_STOP_T;


#endif /* CONFIG_MULTI_CHANNEL */





#ifdef MT_MAC_BTCOEX
/*
 * Coex Sub
 */
enum EXT_BTCOEX_SUB {
	COEX_SET_PROTECTION_FRAME = 0x1,
	COEX_WIFI_STATUS_UPDATE  = 0x2,
	COEX_UPDATE_BSS_INFO = 0x03,
};


/*
 * Coex status bit
 */
enum EXT_BTCOEX_STATUS_bit {
	COEX_STATUS_RADIO_ON 		= 	0x01,
	COEX_STATUS_SCAN_G_BAND  	= 	0x02,
	COEX_STATUS_SCAN_A_BAND 	= 	0x04,
	COEX_STATUS_LINK_UP 			= 	0x08,
	COEX_STATUS_BT_OVER_WIFI 	= 	0x10,
};

enum EXT_BTCOEX_PROTECTION_MODE {
	COEX_Legacy_CCK 		= 	0x00,
	COEX_Legacy_OFDM  	= 	0x01,
	COEX_HT_MIX 	= 	0x02,
	COEX_HT_Green 			= 	0x03,
	COEX_VHT	= 	0x04,
};

enum EXT_BTCOEX_OFDM_PROTECTION_RATE {
	PROTECTION_OFDM_6M 	= 	0x00,
	PROTECTION_OFDM_9M  	= 	0x01,
	PROTECTION_OFDM_12M 	= 	0x02,
	PROTECTION_OFDM_18M 	= 	0x03,
	PROTECTION_OFDM_24M	= 	0x04,
	PROTECTION_OFDM_36M 	= 	0x05,
	PROTECTION_OFDM_48M   = 	0x06,
	PROTECTION_OFDM_54M 	= 	0x07,
};
/*
 * Coex status bit
 */

typedef enum _WIFI_STATUS {
	STATUS_RADIO_ON = 0,
	STATUS_RADIO_OFF = 1,
	STATUS_SCAN_G_BAND = 2,
	STATUS_SCAN_G_BAND_END = 3,
	STATUS_SCAN_A_BAND = 4,
	STATUS_SCAN_A_BAND_END = 5,
	STATUS_LINK_UP = 6,
	STATUS_LINK_DOWN = 7,
	STATUS_BT_OVER_WIFI = 8,
	STATUS_BT_MAX,
} WIFI_STATUS;
#endif
/*
 * Extension Event
 */
enum EXT_EVENT_TYPE {
	EXT_EVENT_CMD_RESULT = 0x0,
	EXT_EVENT_CMD_ID_EFUSE_ACCESS  = 0x1,
	EXT_EVENT_RF_REG_ACCESS = 0x2,
	EXT_EVENT_ID_RF_TEST = 0x4,
	EXT_EVENT_MULTI_CR_ACCESS = 0x0E,
	EXT_EVENT_FW_LOG_2_HOST = 0x13,
    EXT_EVENT_BT_COEX = 0x19,
    EXT_EVENT_BEACON_LOSS = 0x1A,
    EXT_EVENT_ID_GET_TX_POWER = 0x1C,
	EXT_EVENT_THERMAL_PROTECT = 0x22,
	EXT_EVENT_ID_ASSERT_DUMP = 0x23,
	EXT_EVENT_SENSOR_RESULT = 0x2C,
	EXT_EVENT_ID_ROAMING_DETECTION_NOTIFICATION = 0x2d,
#ifdef RACTRL_FW_OFFLOAD_SUPPORT
	EXT_EVENT_RA_THROUGHPUT_BURST = 0x2F,
	EXT_EVENT_GET_TX_STATISTIC = 0x30,
#endif /* RACTRL_FW_OFFLOAD_SUPPORT */
    EXT_EVENT_PRETBTT_INT = 0x31,
	EXT_EVENT_ID_BF_STATUS_READ = 0x35,
#ifdef VOW_SUPPORT
    EXT_EVENT_ID_DRR_CTRL = 0x36,
    EXT_EVENT_ID_BSSGROUP_CTRL = 0x37,
    EXT_EVENT_ID_VOW_FEATURE_CTRL = 0x38,
#endif /* VOW_SUPPORT */
#ifdef MT_DFS_SUPPORT    //Jelly20141229
        EXT_EVENT_ID_RDD_REPORT = 0x3A,
	EXT_EVENT_ID_CAC_END = 0x3E,
#endif
    EXT_EVENT_ID_MAC_INFO = 0x3C,
    EXT_EVENT_ID_ATE_TEST_MODE = 0x3D,
#ifdef VOW_SUPPORT
    EXT_EVENT_ID_RX_AIRTIME_CTRL = 0x4a,
    EXT_EVENT_ID_AT_PROC_MODULE = 0x4b,
#endif /* VOW_SUPPORT */
    EXT_EVENT_ID_EFUSE_FREE_BLOCK = 0x4D,
#ifdef BA_TRIGGER_OFFLOAD
    EXT_EVENT_ID_BA_TRIGGER = 0x4E,
#endif /* BA_TRIGGER_OFFLOAD */
    EXT_EVENT_CSA_NOTIFY = 0x4F,
#ifdef INTERNAL_CAPTURE_SUPPORT
    EXT_EVENT_ID_WIFISPECTRUM_ICAP_RAWDATA_DUMP = 0x50,
#endif/* INTERNAL_CAPTURE_SUPPORT */
    EXT_EVENT_TMR_CALCU_INFO = 0x51,
    EXT_EVENT_ID_BSS_ACQ_PKT_NUM = 0x52,
    EXT_EVENT_ID_TX_POWER_FEATURE_CTRL = 0x58,
#if defined (RLM_CAL_CACHE_SUPPORT) || defined(PRE_CAL_TRX_SET2_SUPPORT)
    EXT_EVENT_ID_TXLPF_CAL_INFO = 0x62,
    EXT_EVENT_ID_TXIQ_CAL_INFO = 0x63,
    EXT_EVENT_ID_TXDC_CAL_INFO = 0x64,
    EXT_EVENT_ID_RXFI_CAL_INFO = 0x65,
    EXT_EVENT_ID_RXFD_CAL_INFO = 0x66,
#endif /* defined(RLM_CAL_CACHE_SUPPORT) || defined(PRE_CAL_TRX_SET2_SUPPORT) */
#ifdef RACTRL_FW_OFFLOAD_SUPPORT
	EXT_EVENT_G_BAND_256QAM_PROBE_RESULT = 0x6B,
#endif /* RACTRL_FW_OFFLOAD_SUPPORT */
#ifdef CONFIG_HOTSPOT_R2
	EXT_EVENT_ID_INFORM_HOST_REPROCESS_PKT = 0x71,
	EXT_EVENT_ID_GET_CR4_HOTSPOT_CAPABILITY = 0x72,
#endif /* CONFIG_HOTSPOT_R2 */
};

/*
 * DownLoad Type
 */
enum {
	DownLoadTypeA,
	DownLoadTypeB,
#ifdef MT7615
	DownLoadTypeC
#endif /* MT7615 */
};

#ifdef RT_BIG_ENDIAN
typedef union _FW_RXD_0 {
	struct {
		UINT32 pkt_type_id:16;
		UINT32 length:16;
	} field;
	UINT32 word;
} FW_RXD_0;
#else
typedef union _FW_RXD_0 {
	struct {
		UINT32 length:16;
		UINT32 pkt_type_id:16;
	} field;
	UINT32 word;
} FW_RXD_0;
#endif /* RT_BIG_ENDIAN */

#ifdef RT_BIG_ENDIAN
typedef union _FW_RXD_1 {
	struct {
		UINT32 rsv:16;
		UINT32 seq_num:8;
		UINT32 eid:8;
	} field;
#if defined(MT7615) || defined(MT7622)
    struct {
        UINT32 rsv:16;
        UINT32 pkt_ft:2;
        UINT32 seq_num:6;
        UINT32 eid:8;
    } field1;
#endif /* defined(MT7615) || defined(MT7622) */
	UINT32 word;
} FW_RXD_1;
#else
typedef union _FW_RXD_1 {
	struct {
		UINT32 eid:8;
		UINT32 seq_num:8;
		UINT32 rsv:16;
	} field;
#if defined(MT7615) || defined(MT7622)
	struct {
		UINT32 eid:8;
		UINT32 ptk_ft:2;
		UINT32 seq_num:6;
		UINT32 rsv:16;
	} field1;
#endif /* defined(MT7615) || defined(MT7622) */
	UINT32 word;
} FW_RXD_1;
#endif /* RT_BIG_ENDIAN */

#ifdef RT_BIG_ENDIAN
typedef union _FW_RXD_2 {
	struct {
		UINT32 s2d_index:8;
		UINT32 rsv	:16;
		UINT32 ext_eid	:8;
	} field;
	UINT32 word;
} FW_RXD_2;
#else
typedef union _FW_RXD_2 {
	struct {
		UINT32 ext_eid	:8;
		UINT32 rsv	:16;
		UINT32 s2d_index:8;
	} field;
	UINT32 word;
} FW_RXD_2;
#endif /* RT_BIG_ENDIAN */

/*
 * Event structure
 */
typedef struct GNU_PACKED _EVENT_RXD_ {
	FW_RXD_0 fw_rxd_0;
	FW_RXD_1 fw_rxd_1;
	FW_RXD_2 fw_rxd_2;
} EVENT_RXD;

/*
 * Event type table
 */
enum MT_EVENT_TYPE {
	MT_TARGET_ADDRESS_LEN_RSP = 0x01,
	MT_FW_START_RSP = 0x01,
	GENERIC_EVENT = 0x01,
	EVENT_ACCESS_REG = 0x02,
	MT_PATCH_SEM_RSP = 0x04,
	EVENT_CH_PRIVILEGE = 0x18,
	EXT_EVENT = 0xED,
	MT_RESTART_DL_RSP = 0xEF,
};

#define FW_CODE_START_ADDRESS1 0x100000
#define FW_CODE_START_ADDRESS2 0x000000
// define moved to MT763X.h file
/*#if defined(MT7637)
#define ROM_PATCH_START_ADDRESS 0xD0000
#else
#define ROM_PATCH_START_ADDRESS 0x8c000
#endif*//* MT7637_FPGA */

#define REL_PATCH_SEM	0
#define GET_PATCH_SEM	1

/*
 * CMD w/ target address/length data mode
 */
#define TARGET_ADDR_LEN_NO_RSP 0
#define ENABLE_ENCRY (1 << 0)
#define RESET_SEC_IV (1 << 3)
#define TARGET_ADDR_LEN_NEED_RSP (1 << 31)
#define ENABLE_ENCRY (1 << 0)
#define RESET_SEC_IV (1 << 3)

/*
 * FW feature set
 */
#define FW_FEATURE_SET_ENCRY (1 << 0)
#define FW_FEATURE_SET_KEY_MASK (0x3 << 1)
#define FW_FEATURE_SET_KEY(p) (((p) & 0x03) << 1)
#define FW_FEATURE_RESET_IV (1 << 3)
#ifdef MT7615
#define FW_FEATURE_CR4_FW_DOWNLOAD (1 << 4)
#endif
#define GET_FEATURE_SET_KEY(p) (((p) & FW_FEATURE_SET_KEY_MASK) >> 1)

/*
 * Erro code for target address/length response
 */
enum {
	TARGET_ADDRESS_LEN_SUCCESS,
};

/*
 * Error code for cmd(event) firmware start response
 */
enum {
	WIFI_FW_DOWNLOAD_SUCCESS,
	WIFI_FW_DOWNLOAD_INVALID_PARAM,
	WIFI_FW_DOWNLOAD_INVALID_CRC,
	WIFI_FW_DOWNLOAD_DECRYPTION_FAIL,
	WIFI_FW_DOWNLOAD_UNKNOWN_CMD,
	WIFI_FW_DOWNLOAD_TIMEOUT,
};

struct _INIT_CMD_ACCESS_REG {
	UINT8 ucSetQuery;
	UINT8 aucReserved[3];
	UINT32 u4Address;
	UINT32 u4Data;
};

#define CMD_CH_PRIV_ACTION_REQ 0
#define CMD_CH_PRIV_ACTION_ABORT 1
#define CMD_CH_PRIV_ACTION_BW_REQ 2

#define CMD_CH_PRIV_SCO_SCN 0
#define CMD_CH_PRIV_SCO_SCA 1
#define CMD_CH_PRIV_SCO_SCB 3

#define CMD_CH_PRIV_BAND_G 1
#define CMD_CH_PRIV_BAND_A 2

#define CMD_CH_PRIV_CH_WIDTH_20_40 0
#define CMD_CH_PRIV_CH_WIDTH_80	   1
#define CMD_CH_PRIV_CH_WIDTH_160   2
#define CMD_CH_PRIV_CH_WIDTH_80_80 3

#define CMD_CH_PRIV_REQ_JOIN 0
#define CMD_CH_PRIV_REQ_P2P_LISTEN 1

#ifdef MT_MAC_BTCOEX
typedef struct GNU_PACKED _CMD_COEXISTENCE_T {
    UINT8         ucSubOpCode;
    UINT8         aucReserve[3];
    UINT8          aucData[48];
} EXT_CMD_COEXISTENCE_T, *P_EXT_CMD_COEXISTENCE_T;

typedef struct GNU_PACKED _EVENT_EXT_COEXISTENCE_T {
    UINT8         ucSubOpCode;
    UINT8         aucReserve[3];
    UINT8         aucBuffer[64];
} EVENT_EXT_COEXISTENCE_T, *P_EVENT_EXT_COEXISTENCE_T;


typedef struct GNU_PACKED _COEX_WIFI_STATUS_UPDATE_T
{
    UINT32      u4WIFIStatus;
} COEX_WIFI_STATUS_UPDATE_T, *P_COEX_WIFI_STATUS_UPDATE_T;


typedef struct GNU_PACKED _COEX_SET_PROTECTION_FRAME_T
{
    UINT8      ucProFrameMode;
    UINT8      ucProFrameRate;
    UINT8      aucReserve[2];
} COEX_SET_PROTECTION_FRAME_T, *P_COEX_SET_PROTECTION_FRAME_T;

typedef struct GNU_PACKED _COEX_UPDATE_BSS_INFO_T
{
    UINT8      u4BSSPresence[4];
    UINT8      u4BSSAPMode[4];
    UINT8      u4IsQBSS[4];
} COEX_UPDATE_BSS_INFO_T, *P_COEX_UPDATE_BSS_INFO_T;

typedef struct GNU_PACKED _EVENT_COEX_CMD_RESPONSE_T {
    UINT32         u4Status;
} EVENT_COEX_CMD_RESPONSE_T, *P_EVENT_COEX_CMD_RESPONSE_T;


typedef struct GNU_PACKED _EVENT_COEX_REPORT_COEX_MODE_T {
	UINT32         u4SupportCoexMode;
    UINT32         u4CurrentCoexMode;
} EVENT_COEX_REPORT_COEX_MODE_T, *P_EVENT_COEX_REPORT_COEX_MODE_T;


typedef struct GNU_PACKED _EVENT_COEX_MASK_OFF_TX_RATE_T {
    UINT8         ucOn;
    UINT8         aucReserve[3];
} EVENT_COEX_MASK_OFF_TX_RATE_T, *P_EVENT_COEX_MASK_OFF_TX_RATE_T;



typedef struct GNU_PACKED _EVENT_COEX_CHANGE_RX_BA_SIZE_T {
    UINT8         ucOn;
    UINT8         ucRXBASize;
    UINT8         aucReserve[2];
}EVENT_COEX_CHANGE_RX_BA_SIZE_T, *P_EVENT_COEX_CHANGE_RX_BA_SIZE_T;

typedef struct GNU_PACKED _EVENT_COEX_LIMIT_BEACON_SIZE_T {
    UINT8         ucOn;
    UINT8         aucReserve[3];
}EVENT_COEX_LIMIT_BEACON_SIZE_T, *P_EVENT_COEX_LIMIT_BEACON_SIZE_T;


typedef struct GNU_PACKED _EVENT_COEX_EXTEND_BTO_ROAMING_T {
    UINT8         ucOn;
    UINT8         aucReserve[3];
} EVENT_COEX_EXTEND_BTO_ROAMING_T, *P_EVENT_COEX_EXTEND_BTO_ROAMING_T;

typedef struct GNU_PACKED _COEX_TMP_FRAME_T
{
    UINT8      ucProFrame1;
    UINT8      ucProFrame2;
	UINT8      ucProFrame3;
	UINT8      ucProFrame4;
} COEX_TMP_FRAME_T, *P_COEX_TMP_FRAME_T;
#endif /*MT_MAC_BTCOEX*/


typedef struct GNU_PACKED _CMD_SEC_ADDREMOVE_KEY_STRUC_T {
	UINT8		ucAddRemove;
	UINT8		ucTxKey;
	UINT8		ucKeyType;
	UINT8		ucIsAuthenticator;
	UINT8		aucPeerAddr[6];
	UINT8		ucBssIndex;
	UINT8		ucAlgorithmId;
	UINT8		ucKeyId;
	UINT8		ucKeyLen;
	UINT8		ucWlanIndex;
	UINT8		ucReverved;
	UINT8		aucKeyRsc[16];
	UINT8		aucKeyMaterial[32];
} CMD_SEC_ADDREMOVE_KEY_STRUC_T, *P_CMD_ADDREMOVE_KEY_STRUC_T;

typedef struct GNU_PACKED _EVENT_SEC_ADDREMOVE_STRUC_T {
	UINT32		u4WlanIdx;
	UINT32		u4Status;
	UINT32		u4Resv;
} EVENT_SEC_ADDREMOVE_STRUC_T, *P_EVENT_SEC_ADDREMOVE_STRUC_T;

typedef struct GNU_PACKED _EXT_CMD_AP_PWS_START_T {
    UINT32 u4WlanIdx;
    UINT32 u4Resv;
    UINT32 u4Resv2;
} EXT_CMD_AP_PWS_START_T, *P_EXT_CMD_AP_PWS_START_T;


typedef struct GNU_PACKED _CMD_AP_PS_CLEAR_STRUC_T {
	UINT32		u4WlanIdx;
	UINT32		u4Status;
	UINT32		u4Resv;
} CMD_AP_PS_CLEAR_STRUC_T, *P_CMD_AP_PS_CLEAR_STRUC_T;

typedef struct GNU_PACKED _CMD_CH_PRIVILEGE_T {
    UINT8      ucBssIndex;
    UINT8      ucTokenID;
    UINT8      ucAction;
    UINT8      ucPrimaryChannel;
    UINT8      ucRfSco;
    UINT8      ucRfBand;
    UINT8      ucRfChannelWidth;   /* To support 80/160MHz bandwidth */
    UINT8      ucRfCenterFreqSeg1; /* To support 80/160MHz bandwidth */
    UINT8      ucRfCenterFreqSeg2; /* To support 80/160MHz bandwidth */
    UINT8      ucReqType;
    UINT8      aucReserved[2];
    UINT32     u4MaxInterval;      /* In unit of ms */
} CMD_CH_PRIVILEGE_T, *P_CMD_CH_PRIVILEGE_T;


typedef struct GNU_PACKED _CMD_RF_REG_ACCESS_T {
	UINT32 WiFiStream;
	UINT32 Address;
	UINT32 Data;
} CMD_RF_REG_ACCESS_T;


typedef struct GNU_PACKED _CMD_ACCESS_REG_T {
	UINT32 u4Address;
	UINT32 u4Data;
} CMD_ACCESS_REG_T;

/* test CR4 */
typedef struct GNU_PACKED _EXT_CMD_CR4_QUERY_T {
    UINT32 u4Cr4QueryOptionArg0;
    UINT32 u4Cr4QueryOptionArg1;
    UINT32 u4Cr4QueryOptionArg2;
} EXT_CMD_CR4_QUERY_T, *P_EXT_CMD_CR4_QUERY_T;

typedef struct GNU_PACKED _EXT_CMD_CR4_SET_T {
    UINT32 u4Cr4SetArg0;
    UINT32 u4Cr4SetArg1;
    UINT32 u4Cr4SetArg2;
} EXT_CMD_CR4_SET_T, *P_EXT_CMD_CR4_SET_T;

typedef struct GNU_PACKED _EXT_CMD_CR4_CAPABILITY_T {
    UINT32 u4Cr4Capability;
} EXT_CMD_CR4_CAPABILITY_T, *P_EXT_CMD_CR4_CAPABILITY_T;

typedef struct GNU_PACKED _EXT_CMD_CR4_DEBUG_T {
    UINT32 u4Cr4Debug;
} EXT_CMD_CR4_DEBUG_T, *P_EXT_CMD_CR4_DEBUG_T;

typedef struct GNU_PACKED _EXT_CMD_ID_LED {
    UINT32 u4LedNo;
    UINT32 u4LedCtrl;
} EXT_CMD_ID_LED_T, *PEXT_CMD_ID_LED_T;

typedef struct GNU_PACKED _EVENT_WIFI_RDD_TEST_T {
	UINT32 u4FuncIndex;
	UINT32 u4FuncLength;
	UINT32 u4Prefix;
	UINT32 u4Count;
	UINT8 ucRddIdx;
	UINT8 aucReserve[3];
	UINT8 aucBuffer[0];
} EVENT_WIFI_RDD_TEST_T, *PEVENT_WIFI_RDD_TEST_T;

#define WIFI_RADIO_ON 1
#define WIFI_RADIO_OFF 2
typedef struct GNU_PACKED _EXT_CMD_RADIO_ON_OFF_CTRL_T {
	UINT8 ucWiFiRadioCtrl;
	UINT8 aucReserve[3];
} EXT_CMD_RADIO_ON_OFF_CTRL_T;


#if defined(MT_MAC) && (!defined(MT7636))
typedef struct GNU_PACKED _EXT_CMD_TXBf_APCLIENT_CLUSTER_T
{
    UINT8  ucPfmuProfileFormatId;
    UINT8  ucReserved;
    UINT8  ucWlanIdx;
    UINT8  ucCmmWlanId;
} EXT_CMD_TXBf_APCLIENT_CLUSTER_T, *P_EXT_CMD_TXBf_APCLIENT_CLUSTER_T;

typedef struct GNU_PACKED _EXT_CMD_REPT_CLONED_STA_BF_T
{
    UINT8  ucCmdCategoryID;
    UINT8  ucReserved;
    UINT8  ucWlanIdx;
    UINT8  ucCliIdx;
} EXT_CMD_REPT_CLONED_STA_BF_T, *P_EXT_CMD_REPT_CLONED_STA_BF_T;

typedef struct GNU_PACKED _EXT_CMD_TXBf_BFEE_CTRL_T
{
    UINT8   ucCmdCategoryID;
    BOOLEAN fgBFeeNullPktFeedbackEn;
    UINT8   ucReserved[2];
} EXT_CMD_TXBf_BFEE_CTRL_T, *P_EXT_CMD_TXBf_BFEE_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_ITXBf_PHASE_CAL_CTRL_T
{
    UINT8   ucCmdCategoryID;
    UINT8   ucGroup_L_M_H;
    UINT8   ucGroup;
    BOOLEAN fgSX2;
    UINT8   ucPhaseCalType;
    UINT8   ucPhaseVerifyLnaGainLevel;
    UINT8   ucReserved[2];
} EXT_CMD_ITXBf_PHASE_CAL_CTRL_T, *P_EXT_CMD_ITXBf_PHASE_CAL_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_ITXBf_PHASE_COMP_CTRL_T
{
    UINT8   ucCmdCategoryID;
    UINT8   ucWlanIdx;
    UINT8   ucBW;
    UINT8   ucBand;
    UINT8   ucDbdcBandIdx;
    BOOLEAN fgRdFromE2p;
    BOOLEAN fgDisComp;
    UINT8   ucReserved;
    UINT8   aucBuf[32];
} EXT_CMD_ITXBf_PHASE_COMP_CTRL_T, *P_EXT_CMD_ITXBf_PHASE_COMP_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_TXBf_TX_APPLY_CTRL_T
{
    UINT8   ucCmdCategoryID;
    UINT8   ucWlanIdx;
    BOOLEAN fgETxBf;
    BOOLEAN fgITxBf;
    BOOLEAN fgMuTxBf;
    BOOLEAN fgPhaseCali;
    UINT8   ucReserved[2];
} EXT_CMD_TXBf_TX_APPLY_CTRL_T, *P_EXT_CMD_TXBf_TX_APPLY_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_SND_PERIODIC_TRIGGER_CTRL_T
{
    UINT8   ucCmdCategoryID;
    UINT8   ucSuMuSndMode;
    UINT8   ucStaNum;
    UINT8   ucReserved;
    UINT8   ucWlanIdx[4];
    UINT32  u4SoundingInterval;     /* By ms */
} EXT_CMD_ETXBf_SND_PERIODIC_TRIGGER_CTRL_T, *P_EXT_CMD_ETXBf_SND_PERIODIC_TRIGGER_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_MU_SND_PERIODIC_TRIGGER_CTRL_T
{
    UINT8   ucCmdCategoryID;
    UINT8   ucSuMuSndMode;
    UINT8   ucStaNum;
    UINT8   ucReserved;
    UINT8   ucWlanIdx[4];
    UINT32  u4SoundingInterval;     /* By ms */
} EXT_CMD_ETXBf_MU_SND_PERIODIC_TRIGGER_CTRL_T, *P_EXT_CMD_ETXBf_MU_SND_PERIODIC_TRIGGER_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_BF_TX_PWR_BACK_OFF_T
{
    UINT8  ucCmdCategoryID;
    UINT8  ucBandIdx;
    UINT8  aucReserved1[2];
    INT8   acTxPwrFccBfOnCase[10];
    UINT8  aucReserved2[2];
    INT8   acTxPwrFccBfOffCase[10];
    UINT8  aucReserved3[2];
} EXT_CMD_BF_TX_PWR_BACK_OFF_T, *P_EXT_CMD_BF_TX_PWR_BACK_OFF_T;

typedef struct GNU_PACKED _EXT_CMD_BF_AWARE_CTRL_T
{
    UINT8    ucCmdCategoryID;
    BOOLEAN  fgBfAwareCtrl;
    UINT8    aucReserved[2];
} EXT_CMD_BF_AWARE_CTRL_T, *P_EXT_CMD_BF_AWARE_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_BFEE_HW_CTRL_T
{
    UINT8    ucCmdCategoryID;
    BOOLEAN  fgBfeeHwCtrl;
    UINT8    aucReserved[2];
} EXT_CMD_BFEE_HW_CTRL_T, *P_EXT_CMD_BFEE_HW_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_BF_HW_ENABLE_STATUS_UPDATE_T
{
    UINT8    ucCmdCategoryID;
    BOOLEAN  fgEBfHwEnStatus;
    BOOLEAN  fgIBfHwEnStatus;
    UINT8    ucReserved;
} EXT_CMD_BF_HW_ENABLE_STATUS_UPDATE_T, *P_EXT_CMD_BF_HW_ENABLE_STATUS_UPDATE_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_PFMU_PROFILE_TAG_R_T
{
    UINT8  ucPfmuProfileFormatId;
    UINT8  ucPfmuIdx;
    UINT8  fgBFer;
    UINT8  ucReserved[1];
} EXT_CMD_ETXBf_PFMU_PROFILE_TAG_R_T, *P_EXT_CMD_ETXBf_PFMU_PROFILE_TAG_R_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_PFMU_PROFILE_TAG_W_T
{
    UINT8  ucPfmuProfileFormatId;
    UINT8  ucPfmuIdx;
    UINT8  ucReserved[2];
    UINT8  ucBuf[64];
} EXT_CMD_ETXBf_PFMU_PROFILE_TAG_W_T, *P_EXT_CMD_ETXBf_PFMU_PROFILE_TAG_W_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_PFMU_PROFILE_DATA_R_T
{
    UINT8   ucPfmuProfileFormatId;
    UINT8   ucPfmuIdx;
    BOOLEAN fgBFer;
    UINT8   ucReserved[3];
    UINT16  u2SubCarrIdx;
} EXT_CMD_ETXBf_PFMU_PROFILE_DATA_R_T, *P_EXT_CMD_ETXBf_PFMU_PROFILE_DATA_R_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_PFMU_PROFILE_DATA_W_T
{
    UINT8      ucPfmuProfileFormatId;
    UINT8      ucPfmuIdx;
    UINT16     u2SubCarr;
    UINT8      ucBuf[20];
} EXT_CMD_ETXBf_PFMU_PROFILE_DATA_W_T, *P_EXT_CMD_ETXBf_PFMU_PROFILE_DATA_W_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_PFMU_PROFILE_DATA_W_20M_ALL_T
{
    UINT8      ucPfmuProfileFormatId;
    UINT8      ucPfmuIdx;
    UINT8      aucReserved[2];
    UINT8      ucBuf[512];
} EXT_CMD_ETXBf_PFMU_PROFILE_DATA_W_20M_ALL_T, *P_EXT_CMD_ETXBf_PFMU_PROFILE_DATA_W_20M_ALL_T;


typedef struct GNU_PACKED _EXT_CMD_ETXBf_PFMU_PROFILE_PN_R_T
{
    UINT8  ucPfmuProfileFormatId;
    UINT8  ucPfmuIdx;
    UINT8  ucReserved[2];
} EXT_CMD_ETXBf_PFMU_PROFILE_PN_R_T, *P_EXT_CMD_ETXBf_PFMU_PROFILE_PN_R_T;

typedef struct GNU_PACKED _EXT_CMD_ETXBf_PFMU_PROFILE_PN_W_T
{
    UINT8  ucPfmuProfileFormatId;
    UINT8  ucPfmuIdx;
    UINT8  ucBW;
    UINT8  ucReserved;
    UINT8  ucBuf[32];
} EXT_CMD_ETXBf_PFMU_PROFILE_PN_W_T, *P_EXT_CMD_ETXBf_PFMU_PROFILE_PN_W_T;

typedef struct GNU_PACKED _EXT_EVENT_BF_STATUS_T
{
    UINT8   ucBfDataFormatID;
    UINT8   ucBw;
    UINT16  u2subCarrIdx;
    BOOLEAN fgBFer;
    UINT8   aucReserved[3];
	UINT8	aucBuffer[1000]; //temparary size
} EXT_EVENT_BF_STATUS_T, *P_EXT_EVENT_BF_STATUS_T;

typedef struct GNU_PACKED _EXT_EVENT_IBF_STATUS_T
{
    UINT8   ucBfDataFormatID;
    UINT8   ucGroup_L_M_H;
    UINT8   ucGroup;
    BOOLEAN fgSX2;
    UINT8   ucStatus;
    UINT8   ucPhaseCalType;
    UINT8   aucReserved[2];
	UINT8	aucBuffer[1000]; //temparary size
} EXT_EVENT_IBF_STATUS_T, *P_EXT_EVENT_IBF_STATUS_T;

typedef enum _BF_EVENT_CATEGORY
{
    BF_PFMU_TAG = 0x10,
    BF_PFMU_DATA,
    BF_PFMU_PN,
    BF_PFMU_MEM_ALLOC_MAP,
    BF_STAREC,
    BF_CAL_PHASE
} BF_EVENT_CATEGORY;

#endif//MT_MAC


/* EXT_CMD_RF_TEST */
/* ACTION */
#define ACTION_SWITCH_TO_RFTEST 0 /* to switch firmware mode between normal mode or rf test mode */
#define ACTION_IN_RFTEST        1
/* OPMODE */
#define OPERATION_NORMAL_MODE     0
#define OPERATION_RFTEST_MODE     1
#define OPERATION_ICAP_MODE       2
#define OPERATION_ICAP_OVERLAP 	  3
#define OPERATION_WIFI_SPECTRUM   4

/* FuncIndex */
typedef enum {
	RE_CALIBRATION = 0x01,
	CALIBRATION_BYPASS = 0x02,
	TX_TONE_START = 0x03,
	TX_TONE_STOP = 0x04,
	CONTINUOUS_TX_START = 0x05,
	CONTINUOUS_TX_STOP = 0x06,
	RF_AT_EXT_FUNCID_TX_TONE_RF_GAIN = 0x07,
	RF_AT_EXT_FUNCID_TX_TONE_DIGITAL_GAIN = 0x08,
	CAL_RESULT_DUMP_FLAG = 0x09,
	RDD_TEST_MODE  = 0x0A,
	SET_ICAP_WIFI_SPECTRUM = 0x0B,
	GET_ICAP_WIFI_SPECTRUM = 0x0C,
	SET_ADC = 0x0D,
	SET_RX_GAIN = 0x0E,
	SET_TTG = 0x0F,
	TTG_ON_OFF = 0x10

}FUNC_IDX;

#define RF_TEST_DEFAULT_RESP_LEN		8	/* sizeof(struct _EVENT_EXT_CMD_RESULT_T) */
#define	RC_CAL_RESP_LEN					112
#define RX_RSSI_DCOC_CAL_RESP_LEN		304
#define	RX_DCOC_CAL_RESP_LEN			816	/* Total 4 event 808 *4 */
#define TX_DPD_RX_FI_FD_MPM_RESP_LEN	8	/* MT7615 not support */
#define TX_DPD_SCAN_HPM_RESP_LEN		8	/* MT7615 not support */
#define RX_FIIQ_CAL_RESP_LEN		208
#define RX_FDIQ_CAL_RESP_LEN		416		/* Total 408*6 */
#define TX_DPD_LINK_RESP_LEN		272		/* Total 264*6 */
#define TX_LPFG_RESP_LEN			64
#define TX_DCIQ_RESP_LEN			592
#define TX_IQM_RESP_LEN				112
#define TX_PGA_RESP_LEN				112
#define CAL_ALL_LEN					8712
#define RF_TEST_ICAP_LEN            120
/* Cal Items */
typedef enum {
	RC_CAL = 0x1,
	RX_RSSI_DCOC_CAL = 0x2,
	RX_DCOC_CAL = 0x4,
	TX_DPD_RX_FI_FD_MPM = 0x8,
	TX_DPD_SCAN_HPM = 0x10,
	RX_FIIQ_CAL = 0x20,
	RX_FDIQ_CAL = 0x40,
	TX_DPD_LINK = 0x80,
	TX_LPFG = 0x100,
	TX_DCIQC = 0x200,
	TX_IQM = 0x400,
	TX_PGA = 0x800,
	CAL_ALL = 0x80000000,
}CAL_ITEM_IDX;

/*u4CalDump*/
typedef enum {
	DISABLE_DUMP = 0x0,
	ENABLE_DUMP = 0x1
}TEST_CAL_DUMP;

typedef struct _TX_TONE_PARAM_T
{
	UINT8 ucAntIndex;
	UINT8 ucToneType;
	UINT8 ucToneFreq;
	UINT8 ucDbdcIdx;
	INT32 i4DcOffsetI;
	INT32 i4DcOffsetQ;
	UINT32 u4Band;
}TX_TONE_PARAM_T, *PTX_TONE_PARAM_T;

typedef struct GNU_PACKED _EXT_CMD_RDD_ON_OFF_CTRL_T//Jelly20150211
{
	UINT8 ucRddCtrl;
	UINT8 ucRddIdx;
	UINT8 ucRddInSel;
	UINT8 ucRddTpRatio;
} EXT_CMD_RDD_ON_OFF_CTRL_T, *P_EXT_CMD_RDD_ON_OFF_CTRL_T;

typedef struct _SET_ADC_T
{
    UINT32  u4ChannelFreq;
	UINT8 	ucAntIndex;
    UINT8	ucBW;
	UINT8   ucSX;
	UINT8 	ucDbdcIdx;
	UINT8 	ucRunType;
	UINT8	ucFType;
	UINT8	aucReserved[2];		// Reserving For future
} SET_ADC_T, *P_SET_ADC_T;

typedef struct _SET_RX_GAIN_T
{
    UINT8	ucLPFG;
    UINT8   ucLNA;
    UINT8 	ucDbdcIdx;
	UINT8	ucAntIndex;
} SET_RX_GAIN_T, *P_EXT_SET_RX_GAIN_T;

typedef struct _SET_TTG_T
{
    UINT32  u4ChannelFreq;
    UINT32  u4ToneFreq;
	UINT8	ucTTGPwrIdx;
	UINT8	ucDbdcIdx;
	UINT8	aucReserved[2];
} SET_TTG_T, *P_SET_TTG_T;

typedef struct _TTG_ON_OFF_T
{
    UINT8	ucTTGEnable;
	UINT8	ucDbdcIdx;
	UINT8 	ucAntIndex;
	UINT8	aucReserved[1];
} TTG_ON_OFF_T, *P_TTG_ON_OFF_T;

typedef struct _CONTINUOUS_TX_PARAM_T
{
	UINT8 ucCtrlCh;
	UINT8 ucCentralCh;
	UINT8 ucBW;
	UINT8 ucAntIndex;
	UINT16 u2RateCode;
	UINT8 ucBand;
	UINT8 ucTxfdMode;
}CONTINUOUS_TX_PARAM_T, *P_CONTINUOUS_TX_PARAM_T;

typedef struct _TX_TONE_POWER_GAIN_T
{
	UINT8 ucAntIndex;
	UINT8 ucTonePowerGain;
	UINT8 ucBand;
	UINT8 aucReserved[1];
}TX_TONE_POWER_GAIN_T, *P_TX_TONE_POWER_GAIN_T;

/*
u4FuncIndex	0x01: RE_CALIBRATION
			0x02: CALIBRATION_BYPASS
			0x03: TX_TONE_START
			0x04: TX_TONE_STOP
			0x05: CONTINUOUS_TX_START
			0x06: CONTINUOUS_TX_STOP
			0x07. RF_AT_EXT_FUNCID_TX_TONE_RF_GAIN
			0x08. RF_AT_EXT_FUNCID_TX_TONE_DIGITAL_GAIN
			0x09: CAL_RESULT_DUMP_FLAG
			0x0A: RDD_TEST_MODE
u4FuncData:
	works when u4FuncIndex = 1 or 2 or 0xA
u4CalDump:
  	works when u4FuncIndex = 9
TX_TONE_PARAM_T:
  	works when u4FuncIndex = 3
CONTINUOUS_TX_PARAM_T:
	works when u4FuncIndex = 5
TX_TONE_POWER_GAIN_T:
  	works when u4FuncIndex = 7 or 8

*/

#ifdef INTERNAL_CAPTURE_SUPPORT
#define WIFISPECTRUM_DEFAULT_RESP_LEN        8

typedef struct GNU_PACKED _ICAP_WIFI_SPECTRUM_SET_STRUC_T{
	UINT32  fgTrigger;
	UINT32  fgRingCapEn;
	UINT32  u4TriggerEvent;
	UINT32  u4CaptureNode;
	UINT32  u4CaptureLen;
	UINT32  u4CapStopCycle;
	UINT32  u4MACTriggerEvent;
	UINT32  u4SourceAddressLSB;
	UINT32  u4SourceAddressMSB;
	UINT32  u4Band;
	UINT8   ucBW;
	UINT8 	aucReserved[3];	
} ICAP_WIFI_SPECTRUM_SET_STRUC_T, *P_ICAP_WIFI_SPECTRUM_SET_STRUC_T;

typedef struct GNU_PACKED _EXT_CMD_WIFI_SPECTRUM_CTRL_T{
	UINT32 u4FuncIndex;
	ICAP_WIFI_SPECTRUM_SET_STRUC_T rWifiSpecInfo;
} EXT_CMD_WIFI_SPECTRUM_CTRL_T, *P_EXT_CMD_WIFI_SPECTRUM_CTRL_T;

typedef struct GNU_PACKED _EVENT_WIFI_ICAP_T{
	UINT32 u4FuncIndex;
	UINT32 u4FuncLength;
	UINT32 u4StartAddr1;
    UINT32 u4StartAddr2;
    UINT32 u4StartAddr3;
    UINT32 u4EndAddr;
    UINT32 u4StopAddr;
    UINT32 u4Wrap;
} EVENT_WIFI_ICAP_T, *P_EVENT_WIFI_ICAP_T;

typedef struct GNU_PACKED _EVENT_WIFI_ICAP_DUMP_RAW_DATA_T
{
    UINT32 u4FuncIndex;
    UINT32 u4FuncLength;
    UINT32 PktNum;
    UINT32 Data[256];
} EVENT_WIFI_ICAP_DUMP_RAW_DATA_T, *P_EVENT_WIFI_ICAP_DUMP_RAW_DATA_T;

typedef enum {
	WIFI_SPECTRUM_CTRL_FUNCID_SET_PARAMETER = 0,
	WIFI_SPECTRUM_CTRL_FUNCID_GET_CAPTURE_STOP_INFO,
	WIFI_SPECTRUM_CTRL_FUNCID_DUMP_RAW_DATA
}WIFI_SPECTRUM_CTRL_FUNCID;
#endif/*INTERNAL_CAPTURE_SUPPORT*/

typedef struct _RF_TEST_CALIBRATION_T
{
	UINT32 	u4FuncData;
	UINT8 	ucDbdcIdx;
	UINT8 	aucReserved[3];
} RF_TEST_CALIBRATION_T, *P_RF_TEST_CALIBRATION_T;

typedef struct _PARAM_MTK_WIFI_TEST_STRUC_T {
	UINT32         u4FuncIndex;
	union{
		UINT32 u4FuncData;
		UINT32 u4CalDump;
		RF_TEST_CALIBRATION_T rCalParam;
		TX_TONE_PARAM_T rTxToneParam;
		CONTINUOUS_TX_PARAM_T rConTxParam;
		TX_TONE_POWER_GAIN_T rTxToneGainParam;
#ifdef INTERNAL_CAPTURE_SUPPORT
		ICAP_WIFI_SPECTRUM_SET_STRUC_T rIcapInfo;
#endif/*INTERNAL_CAPTURE_SUPPORT*/
		EXT_CMD_RDD_ON_OFF_CTRL_T rRDDParam;
		SET_ADC_T rSetADC;
		SET_RX_GAIN_T rSetRxGain;
		SET_TTG_T	rSetTTG;
		TTG_ON_OFF_T rTTGOnOff;
	}Data;
} PARAM_MTK_WIFI_TEST_STRUC_T, *P_PARAM_MTK_WIFI_TEST_STRUC_T;

typedef struct _CMD_TEST_CTRL_T {
	UINT8 ucAction;
	UINT8 ucIcapLen;
	UINT8 aucReserved[2];
	union {
		UINT32 u4OpMode;
		UINT32 u4ChannelFreq;
		PARAM_MTK_WIFI_TEST_STRUC_T rRfATInfo;
	} u;
} CMD_TEST_CTRL_T, *P_CMD_TEST_CTRL_T;

typedef struct _EXT_EVENT_RF_TEST_RESULT_T {
	UINT32 u4FuncIndex;
	UINT32 u4PayloadLength;
	UINT8  aucEvent[0];
} EXT_EVENT_RF_TEST_RESULT_T, *PEXT_EVENT_RF_TEST_RESULT_T;

typedef struct _EXT_EVENT_RF_TEST_DATA_T {
	UINT32 u4CalIndex;
	UINT32 u4CalType;
	UINT8  aucData[0];
} EXT_EVENT_RF_TEST_DATA_T, *PEXT_EVENT_RF_TEST_DATA_T;

#define WIFI_RX_DISABLE 1
typedef struct GNU_PACKED _EXT_CMD_WIFI_RX_DISABLE_T {
	UINT8 ucWiFiRxDisableCtrl;
	UINT8 aucReserve[3];
} EXT_CMD_WIFI_RX_DISABLE_T;


//Power Management Level
#define PM2         2
#define PM4         4
#define PM5         5
#define PM6         6
#define PM7         7
#define ENTER_PM_STATE 1
#define EXIT_PM_STATE 2

#define KEEP_ALIVE_INTERVAL_IN_SEC	10		// uint: sec
// Beacon lost timing
#define BEACON_OFFLOAD_LOST_TIME	30		// unit: beacon --> 30 beacons about 3sec

typedef struct GNU_PACKED _EXT_CMD_PM_STATE_CTRL_T {
	UINT8 ucPmNumber;
	UINT8 ucPmState;
    UINT8 aucBssid[6];
    UINT8 ucDtimPeriod;
    UINT8 ucWlanIdx;
    UINT16 u2BcnInterval;
    UINT32 u4Aid;
    UINT32 u4RxFilter;
    UINT8 ucDbdcIdx;
    UINT8 aucReserve0[3];
    UINT32 u4Feature;
    UINT8 ucOwnMacIdx;
    UINT8 ucWmmIdx;
    UINT8 ucBcnLossCount;
    UINT8 ucBcnSpDuration;
} EXT_CMD_PM_STATE_CTRL_T, *P_EXT_CMD_PM_STATE_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_GREENAP_CTRL_T {
    UINT8 ucDbdcIdx;
    BOOLEAN ucGreenAPOn;
    UINT8 aucReserve[2];
} EXT_CMD_GREENAP_CTRL_T, *P_EXT_GREENAP_CTRL_T;



#define BITS2(m,n)              (BIT(m) | BIT(n) )
#define BITS3(m,n,o)            (BIT(m) | BIT (n) | BIT (o))
#define BITS4(m,n,o,p)          (BIT(m) | BIT (n) | BIT (o) | BIT(p))
#define BITS(m, n)              (~(BIT(m)-1) & ((BIT(n) - 1) | BIT(n)))

/* Station role */
#define STA_TYPE_STA		BIT(0)
#define STA_TYPE_AP		BIT(1)
#define STA_TYPE_ADHOC	BIT(2)
#define STA_TYPE_TDLS		BIT(3)
#define STA_TYPE_WDS		BIT(4)
#define STA_TYPE_BC		BIT(5)

/* Network type */
#define NETWORK_INFRA	BIT(16)
#define NETWORK_P2P		BIT(17)
#define NETWORK_IBSS		BIT(18)
#define NETWORK_MESH	BIT(19)
#define NETWORK_BOW		BIT(20)
#define NETWORK_WDS		BIT(21)


/* Connection type */
#define CONNECTION_INFRA_STA		(STA_TYPE_STA|NETWORK_INFRA)
#define CONNECTION_INFRA_AP		(STA_TYPE_AP|NETWORK_INFRA)
#define CONNECTION_P2P_GC			(STA_TYPE_STA|NETWORK_P2P)
#define CONNECTION_P2P_GO			(STA_TYPE_AP|NETWORK_P2P)
#define CONNECTION_MESH_STA		(STA_TYPE_STA|NETWORK_MESH)
#define CONNECTION_MESH_AP		(STA_TYPE_AP|NETWORK_MESH)
#define CONNECTION_IBSS_ADHOC		(STA_TYPE_ADHOC|NETWORK_IBSS)
#define CONNECTION_TDLS			(STA_TYPE_STA|NETWORK_INFRA|STA_TYPE_TDLS)
#define CONNECTION_WDS			(STA_TYPE_WDS|NETWORK_WDS)
#define CONNECTION_INFRA_BC		(STA_TYPE_BC|NETWORK_INFRA)


#define	MAX_BUF_SIZE_OF_DEVICEINFO (sizeof(CMD_DEVINFO_UPDATE_T) + sizeof(CMD_DEVINFO_ACTIVE_T))

enum
{
	DEVINFO_ACTIVE = 0,
	DEVINFO_MAX_NUM = 1,
};

typedef struct _CMD_HIF_LOOPBACK
{
	UINT32				Loopback_Enable:16;
	UINT32				DestinationQid:16;
} CMD_HIF_LOOPBACK, *PCMD_HIF_LOOPBACK;

enum
{
	DEVINFO_ACTIVE_FEATURE = (1 << DEVINFO_ACTIVE),
	DEVINFO_MAX_NUM_FEATURE = (1 << DEVINFO_MAX_NUM)
};

enum
{
	HIF_CTRL_ID_RESERVED = 0,
	HIF_CTRL_ID_HIF_USB_TX_RX_IDLE = 1,
};

typedef struct GNU_PACKED _EXT_CMD_WIFI_HIF_CTRL_T
{
	UINT8  ucHifCtrlId;
	UINT8   ucDbdcIdx;
	UINT8  aucReserved[6];
} EXT_CMD_WIFI_HIF_CTRL_T, *P_EXT_CMD_WIFI_HIF_CTRL_T;


typedef struct GNU_PACKED _CMD_DEVINFO_UPDATE_T
{
	UINT8	ucOwnMacIdx;
	UINT8	ucReserve;
	UINT16	u2TotalElementNum;
	UINT8    ucAppendCmdTLV;
	UINT8	aucReserve[3];
	UINT8	aucBuffer[];
} CMD_DEVINFO_UPDATE_T, *P_CMD_DEVINFO_UPDATE_T;

typedef struct GNU_PACKED _CMD_DEVINFO_ACTIVE_T
{
	/* Device information (Tag0) */
	UINT16	u2Tag;		// Tag = 0x00
	UINT16	u2Length;
	UINT8	ucActive;
	UINT8	ucDbdcIdx;
	UINT8	aucOwnMAC[6];
} CMD_DEVINFO_ACTIVE_T, *P_CMD_DEVINFO_ACTIVE_T;

typedef struct GNU_PACKED _CMD_DEVINFO_BSSIDX_T
{
	/* Dev information (Tag1) */
	UINT16	u2Tag;		// Tag = 0x01
	UINT16	u2Length;
	UINT8	ucBSSIndex;
	UINT8	aucReserve[3];
	UINT32	ucDevConnectionType;
} CMD_DEVINFO_BSSIDX_T, *P_CMD_BSSINFO_BSSIDX_T;


#define ORI_BA  1
#define RCV_BA  2

enum
{
	STA_REC_BASIC_STA_RECORD = 0,
	STA_REC_RA = 1,
	STA_REC_RA_COMMON_INFO = 2,         /* only for mt7636 and mt7637 */
	STA_REC_RA_UPDATE = 3,
	STA_REC_BF = 4,
	STA_REC_AMSDU = 5,
	STA_REC_BA = 6,
	STA_REC_RED = 7,
	STA_REC_TX_PROC = 8,
	STA_REC_BASIC_HT_INFO = 9,
	STA_REC_BASIC_VHT_INFO = 10,
	STA_REC_AP_PS = 11,
	STA_REC_MAX_NUM
};

enum
{
	STA_REC_BASIC_STA_RECORD_FEATURE = (1 << STA_REC_BASIC_STA_RECORD),
	STA_REC_RA_FEATURE = (1 << STA_REC_RA),
	STA_REC_RA_COMMON_INFO_FEATURE = (1 << STA_REC_RA_COMMON_INFO),     /* only for mt7636 and mt7637 */
	STA_REC_RA_UPDATE_FEATURE = (1 << STA_REC_RA_UPDATE),
	STA_REC_BF_FEATURE = (1 << STA_REC_BF),
	STA_REC_AMSDU_FEATURE = (1 << STA_REC_AMSDU),
	STA_REC_BA_FEATURE = (1 << STA_REC_BA),
	STA_REC_RED_FEATURE = (1 << STA_REC_RED),
	STA_REC_TX_PROC_FEATURE = (1 << STA_REC_TX_PROC),
	STA_REC_BASIC_HT_INFO_FEATURE  = (1 << STA_REC_BASIC_HT_INFO),
	STA_REC_BASIC_VHT_INFO_FEATURE = (1 << STA_REC_BASIC_VHT_INFO),
	STA_REC_AP_PS_FEATURE = (1 << STA_REC_AP_PS),
	STA_REC_MAX_NUM_FEATURE = (1 << STA_REC_MAX_NUM)
};


typedef struct _STAREC_HANDLE_T
{
    UINT32 StaRecTag;
	UINT32 StaRecTagLen;
    INT32 (*StaRecTagHandler)(struct _RTMP_ADAPTER *pAd,struct cmd_msg *msg,VOID *args);
}STAREC_HANDLE_T, *P_STAREC_HANDLE_T;


typedef struct GNU_PACKED _CMD_STAREC_UPDATE_T
{
    UINT8	ucBssIndex;
    UINT8	ucWlanIdx;
    UINT16	u2TotalElementNum;
    UINT8	ucAppendCmdTLV;
    UINT8   ucMuarIdx;
    UINT8	aucReserve[2];
    UINT8	aucBuffer[];
} CMD_STAREC_UPDATE_T, *P_CMD_STAREC_UPDATE_T;


typedef struct GNU_PACKED _CMD_STAREC_HT_INFO_T
{
    UINT16	u2Tag;
    UINT16	u2Length;
    UINT16	u2HtCap;
    UINT16	u2Reserved;
} CMD_STAREC_HT_INFO_T, *P_CMD_STAREC_HT_INFO_T;

typedef struct GNU_PACKED _CMD_STAREC_VHT_INFO_T
{
    UINT16	u2Tag;
    UINT16	u2Length;
    UINT32	u4VhtCap;
    UINT16	u2VhtRxMcsMap;
    UINT16	u2VhtTxMcsMap;
} CMD_STAREC_VHT_INFO_T, *P_CMD_STAREC_VHT_INFO_T;

#define STAREC_COMMON_EXTRAINFO_V2		BIT(0)
#define STAREC_COMMON_EXTRAINFO_NEWSTAREC	BIT(1)

typedef struct GNU_PACKED _STAREC_COMMON_T
{
    /* Basic STA record (Group0) */
    UINT16	u2Tag;		// Tag = 0x00
    UINT16	u2Length;
    UINT32	u4ConnectionType;
    UINT8	ucConnectionState;
    UINT8	ucIsQBSS;
    UINT16	u2AID;
    UINT8	aucPeerMacAddr[6];
	/*This is used especially for 7615 to indicate this STAREC is to create new one or simply update
	In some case host may send new STAREC without delete old STAREC in advance. (ex: lost de-auth or get assoc twice) 
	We need extra info to know if this is a brand new STAREC or not
	Consider backward compatibility, we check bit 0 in this reserve. 
	Only the bit 0 is on, N9 go new way to update STAREC if bit 1 is on too.
	If neither bit match, N9 go orinal way to update STAREC. */
    UINT16	u2ExtraInfo; 
    
} CMD_STAREC_COMMON_T, *P_CMD_STAREC_COMMON_T;


#define STATE_DISCONNECT 0
#define STATE_CONNECTED 1
#define STATE_PORT_SECURE 2

typedef struct GNU_PACKED _STAREC_AMSDU_T
{
    UINT16  u2Tag;		// Tag = 0x05
    UINT16  u2Length;
    UINT8   ucMaxAmsduNum;
    UINT8   ucMaxMpduSize;
    UINT8   acuReserve[2];
} CMD_STAREC_AMSDU_T, *P_CMD_STAREC_AMSDU_T;

typedef struct GNU_PACKED _STAREC_BA_T
{
    UINT16 u2Tag;       // Tag = 0x06
    UINT16 u2Length;
    UINT8 ucTid;
    UINT8 ucBaDirection;
    UINT8 ucAmsduCap;
    UINT8 ucBaEenable;
    UINT16 u2BaStartSeq;
    UINT16 u2BaWinSize;
} CMD_STAREC_BA_T, *P_CMD_STAREC_BA_T;

typedef struct GNU_PACKED _STAREC_RED_T
{
    UINT16	u2Tag;		// Tag = 0x07
    UINT16	u2Length;
    UINT32	u4RED[8];
} CMD_STAREC_RED_T, *P_CMD_STAREC_RED_T;

typedef struct GNU_PACKED _STAREC_TX_PROC_T
{
    UINT16	u2Tag;		// Tag = 0x08
    UINT16	u2Length;
    UINT32	u4TxProcFlag;
} CMD_STAREC_TX_PROC_T, *P_CMD_STAREC_TX_PROC_T;

typedef struct GNU_PACKED _STAREC_PS_T
{
	UINT16	u2Tag;		// Tag = 11
	UINT16	u2Length;
	UINT8	ucStaBmpDeliveryAC;
	UINT8	ucStaBmpTriggerAC;
	UINT8	ucStaMaxSPLength;
	UINT8	ucReserve1[1];
	UINT16	u2StaListenInterval;
	UINT8	ucReserve2[2];
} CMD_STAREC_PS_T, *P_CMD_STAREC_PS_T;


#define RVLAN BIT(0);
#define IPCSO BIT(1);
#define TCPUDPCSO BIT(2);
#define	TX_PROC_ACM_CFG_EN BIT(3);
#define TX_PROC_ACM_CFG_BK BIT(4);
#define TX_PROC_ACM_CFG_BE BIT(5);
#define TX_PROC_ACM_CFG_VI BIT(6);
#define TX_PROC_ACM_CFG_VO BIT(7);

#define MAX_BUF_SIZE_OF_BSS_INFO \
    (sizeof(CMD_BSSINFO_UPDATE_T) + \
    sizeof(CMD_BSSINFO_CONNECT_OWN_DEV_T) + \
    sizeof(CMD_BSSINFO_BASIC_T) + \
    sizeof(CMD_BSSINFO_RF_CH_T) + \
    sizeof(CMD_BSSINFO_PM_T) + \
    sizeof(CMD_BSSINFO_UAPSD_T) + \
    sizeof(CMD_BSSINFO_RSSI_RM_DET_T) + \
    sizeof(CMD_BSSINFO_EXT_BSS_INFO_T) + \
	sizeof(CMD_BSSINFO_AUTO_RATE_CFG_T))
    /* Carter, not finish yet. + \
    sizeof(CMD_BSSINFO_SYNC_MODE_CTRL_T))*/

enum
{
    BSS_INFO_OWN_MAC = 0,
    BSS_INFO_BASIC = 1,
    BSS_INFO_RF_CH = 2,
    BSS_INFO_PM = 3,
    BSS_INFO_UAPSD = 4,
    BSS_INFO_ROAM_DETECTION = 5,
    BSS_INFO_LQ_RM = 6,
    BSS_INFO_EXT_BSS = 7,
    BSS_INFO_BROADCAST_INFO = 8,
    BSS_INFO_SYNC_MODE = 9,
    BSS_INFO_RA = 10,
    BSS_INFO_MAX_NUM
};

enum
{
    BSS_INFO_OWN_MAC_FEATURE = (1 << BSS_INFO_OWN_MAC),
    BSS_INFO_BASIC_FEATURE = (1 << BSS_INFO_BASIC),
    BSS_INFO_RF_CH_FEATURE = (1 << BSS_INFO_RF_CH),
    BSS_INFO_PM_FEATURE = (1 << BSS_INFO_PM),
    BSS_INFO_UAPSD_FEATURE = (1 << BSS_INFO_UAPSD),
    BSS_INFO_ROAM_DETECTION_FEATURE = (1 << BSS_INFO_ROAM_DETECTION),
    BSS_INFO_LQ_RM_FEATURE = (1 << BSS_INFO_LQ_RM),
    BSS_INFO_EXT_BSS_FEATURE = (1 << BSS_INFO_EXT_BSS),
    BSS_INFO_BROADCAST_INFO_FEATURE = (1 << BSS_INFO_BROADCAST_INFO),
    BSS_INFO_SYNC_MODE_FEATURE = (1 << BSS_INFO_SYNC_MODE),
    BSS_INFO_RA_FEATURE = (1 << BSS_INFO_RA),
    BSS_INFO_MAX_NUM_FEATURE = (1 << BSS_INFO_MAX_NUM)
};

#define	UNDEFINED_VALUE_TBD	0

typedef struct GNU_PACKED _CMD_BSSINFO_UPDATE_T
{
	UINT8	ucBssIndex;
	UINT8	ucReserve;
	UINT16	u2TotalElementNum;
	UINT8	ucAppendCmdTLV;
	UINT8	aucReserve[3];
	UINT8	aucBuffer[];
} CMD_BSSINFO_UPDATE_T, *P_CMD_BSSINFO_UPDATE_T;

typedef struct GNU_PACKED _BSSINFO_CONNECT_OWN_DEV_T
{
    /* BSS connect to own dev (Tag0) */
    UINT16	u2Tag;		// Tag = 0x00
    UINT16	u2Length;
    UINT8	ucHwBSSIndex;
    UINT8	ucOwnMacIdx;
    UINT8	aucReserve[2];
    UINT32	u4ConnectionType;
    UINT32	u4Reserved;
} CMD_BSSINFO_CONNECT_OWN_DEV_T, *P_CMD_BSSINFO_CONNECT_OWN_DEV_T;

typedef struct GNU_PACKED _BSSINFO_BASIC_T
{
	/* Basic BSS information (Tag1) */
	UINT16	u2Tag;		// Tag = 0x01
	UINT16	u2Length;
	UINT32	u4NetworkType;
	UINT8	ucActive;
	UINT8	ucReserve0;
	UINT16	u2BcnInterval;
	UINT8	aucBSSID[6];
	UINT8	ucWmmIdx;
	UINT8	ucDtimPeriod;
	UINT8	ucBcMcWlanidx;  // indicate which wlan-idx used for MC/BC transmission.
	UINT8	ucCipherSuit;
	UINT8	acuReserve[6];
} CMD_BSSINFO_BASIC_T, *P_CMD_BSSINFO_BASIC_T;

typedef struct GNU_PACKED _BSSINFO_RF_CH_T
{
	/* RF channel (Tag2) */
	UINT16	u2Tag;		// Tag = 0x02
	UINT16	u2Length;
	UINT8	ucPrimaryChannel;
	UINT8	ucCenterChannelSeg0;
	UINT8	ucCenterChannelSeg1;
	UINT8	ucBandwidth;
} CMD_BSSINFO_RF_CH_T, *P_CMD_BSSINFO_RF_CH_T;

typedef struct GNU_PACKED _BSSINFO_PM_T
{
	/* Power management (Tag3) */
	UINT16	u2Tag;		// Tag = 0x03
	UINT16	u2Length;
	UINT8	ucKeepAliveEn;
	UINT8	ucKeepAlivePeriod;		//unit is second
	UINT8	ucBeaconLossReportEn;
	UINT8	ucBeaconLossCount;
	UINT8	ucBcnSpState0Min;
	UINT8	ucBcnSpState0Max;
	UINT8	ucBcnSpState1Min;
	UINT8	ucBcnSpState1Max;
	UINT8	ucBcnSpState2Min;
	UINT8	ucBcnSpState2Max;
	UINT8	ucBcnSpState3Min;
	UINT8	ucBcnSpState3Max;
	UINT8	ucBcnSpState4Min;
	UINT8	ucBcnSpState4Max;
	UINT16	u2Reserve;
} CMD_BSSINFO_PM_T, *P_CMD_BSSINFO_PM_T;

typedef struct GNU_PACKED _BSSINFO_UAPSD_T
{
	/* UAPSD offload (Tag4) */
	UINT16	u2Tag;		// Tag = 0x04
	UINT16	u2Length;
	UINT8	ucIsUapsdSupported;
	UINT8	ucUapsdTriggerAC;
	UINT8	ucUapsdDeliveryAC;
	UINT8	ucReportSpToEvent;
	UINT16	u2UapsdServicePeriodTO;   //unit is second
	UINT16	u2Reserve;
} CMD_BSSINFO_UAPSD_T, *P_CMD_BSSINFO_UAPSD_T;

typedef struct GNU_PACKED _BSSINFO_RSSI_RM_DET_T
{
	/* RSSI Roaming Detection (Tag5) */
	UINT16	u2Tag;				// Tag = 0x05
	UINT16	u2Length;
	UINT8	fgEnable;				// Enable the Packet RSSI Detection( and moving average)
	UINT8	ucPktSource;			// Packet Seletction
	UINT8	ucPktMAPara; 			// Moving  Average Parameter for Received Packets
	INT8		cRssiCCKLowThr;		// input in RSSI, required by driver
	INT8		cRssiCCKHighThr;		// input in RSSI, required by driver
	INT8		cRssiOFDMLowThr;		// input in RSSI, required by driver
	INT8		cRssiOFDMHighThr;		// input in RSSI, required by driver
	UINT8	ucReserved0;
} CMD_BSSINFO_RSSI_RM_DET_T, *P_CMD_BSSINFO_RSSI_RM_DET_T;

/* Extension BSS information (Tag7) */
typedef struct GNU_PACKED _BSSINFO_EXT_BSS_INFO_T
{
    UINT16 u2Tag;              // Tag = 0x07
    UINT16 u2Length;
    UINT32 ucMbssTsfOffset;
    UINT8  aucReserved[8];
} CMD_BSSINFO_EXT_BSS_INFO_T, *P_BSSINFO_EXT_BSS_INFO_T;


/* Extension BSS information (Tag8) */
typedef struct GNU_PACKED _BSSINFO_BMC_RATE_T
{
    /* Broad Mcast Frame Rate (Tag8) */
    UINT16 u2Tag;              // Tag = 0x08
    UINT16 u2Length;
    UINT16 u2BcTransmit;
    UINT16 u2McTransmit;
    UINT8 ucPreambleMode;
    UINT8 aucReserved[7];
} CMD_BSSINFO_BMC_RATE_T, *P_BSSINFO_BMC_RATE_T;


/* Sync Mode control (Tag9) */
typedef struct GNU_PACKED _BSSINFO_SYNC_MODE_CTRL_T
{
    UINT16 u2Tag;              // Tag = 0x09
    UINT16 u2Length;
    UINT16 u2BcnInterval;
    UINT8  fgIsEnableSync;
    UINT8  ucDtimPeriod;
    UINT8  aucReserved[8];
} CMD_BSSINFO_SYNC_MODE_CTRL_T, *P_BSSINFO_SYNC_MODE_CTRL_T;


typedef struct _BSS_INFO_HANDLE_T
{
    UINT32 BssInfoTag;
    VOID (*BssInfoTagHandler)(
        struct _RTMP_ADAPTER *pAd,
        struct _BSS_INFO_ARGUMENT_T bss_info_argument,
        struct cmd_msg *msg);
}BSS_INFO_HANDLE_T, *P_BSS_INFO_HANDLE_T;

/* WTBL */
#define	MAX_BUF_SIZE_OF_WTBL_INFO	(sizeof(CMD_WTBL_UPDATE_T) + \
									sizeof(CMD_WTBL_GENERIC_T) + \
									sizeof(CMD_WTBL_RX_T) + \
									sizeof(CMD_WTBL_HT_T) + \
									sizeof(CMD_WTBL_VHT_T) + \
									sizeof(CMD_WTBL_PEER_PS_T) + \
									sizeof(CMD_WTBL_TX_PS_T) + \
									sizeof(CMD_WTBL_HDR_TRANS_T) + \
									sizeof(CMD_WTBL_SECURITY_KEY_T) + \
									sizeof(CMD_WTBL_BA_T) + \
									sizeof(CMD_WTBL_RDG_T) + \
									sizeof(CMD_WTBL_PROTECTION_T) + \
									sizeof(CMD_WTBL_CLEAR_T) + \
									sizeof(CMD_WTBL_BF_T) + \
									sizeof(CMD_WTBL_SMPS_T) + \
									sizeof(CMD_WTBL_RAW_DATA_RW_T))


/**
 * @addtogroup wtbl
 * @{
 * @name wtbl TLV
 * @{
 */
enum WTBL_TLV
{
	WTBL_GENERIC = 0,
	WTBL_RX = 1,
	WTBL_HT = 2,
	WTBL_VHT = 3,
	WTBL_PEER_PS = 4,
	WTBL_TX_PS = 5,
	WTBL_HDR_TRANS = 6,
	WTBL_SECURITY_KEY = 7,
	WTBL_BA = 8,
	WTBL_RDG = 9,
	WTBL_PROTECTION = 10,
	WTBL_CLEAR = 11,
	WTBL_BF = 12,
	WTBL_SMPS = 13,
	WTBL_RAW_DATA_RW = 14,
	WTBL_DUMP = 15,
	WTBL_SPE = 16,
	WTBL_MAX_NUM = 17,
};
/** @} */
/** @} */

typedef struct _CMD_WTBL_UPDATE_T
{
	UINT8	ucWlanIdx;
	UINT8	ucOperation;
	UINT16	u2TotalElementNum;
	UINT32	u4Reserve;
	UINT8	aucBuffer[];
} CMD_WTBL_UPDATE_T, *P_CMD_WTBL_UPDATE_T;

enum
{
	RESET_WTBL_AND_SET 	= 1,
	SET_WTBL 				= 2,
	QUERY_WTBL 			= 3,
	RESET_ALL_WTBL		= 4,
};

typedef struct GNU_PACKED _WTBL_GENERIC_TLV_T
{
	UINT16	u2Tag;
	UINT16	u2Length;
	UINT8	aucBuffer[];
} CMD_WTBL_GENERIC_TLV_T, *P_CMD_WTBL_GENERIC_TLV_T;

typedef struct GNU_PACKED _WTBL_GENERIC_T
{
	UINT16	u2Tag;		// Tag = 0x00
	UINT16	u2Length;
	UINT8	aucPeerAddress[6];
	UINT8	ucMUARIndex;
	UINT8	ucSkipTx;
	UINT8	ucCfAck;
	UINT8	ucQos;
	UINT8	ucMesh;
	UINT8	ucAdm;
	UINT16	u2PartialAID;
	UINT8	ucBafEn;
	UINT8   ucAadOm;
} CMD_WTBL_GENERIC_T, *P_CMD_WTBL_GENERIC_T;

typedef struct GNU_PACKED _WTBL_RX_T
{
	UINT16	u2Tag;		// Tag = 0x01
	UINT16	u2Length;
	UINT8	ucRcid;
	UINT8	ucRca1;
	UINT8	ucRca2;
	UINT8	ucRv;
	UINT8	aucReserved[4];
} CMD_WTBL_RX_T, *P_CMD_WTBL_RX_T;

typedef struct GNU_PACKED _WTBL_HT_T
{
	UINT16	u2Tag;		// Tag = 0x02
	UINT16	u2Length;
	UINT8	ucHt;
	UINT8	ucLdpc;
	UINT8	ucAf;
	UINT8	ucMm;
} CMD_WTBL_HT_T, *P_CMD_WTBL_HT_T;

typedef struct GNU_PACKED _WTBL_VHT_T
{
	UINT16	u2Tag;		// Tag = 0x03
	UINT16	u2Length;
	UINT8	ucLdpcVht;
	UINT8	ucDynBw;
	UINT8	ucVht;
	UINT8	ucTxopPsCap;
} CMD_WTBL_VHT_T, *P_CMD_WTBL_VHT_T;

typedef struct GNU_PACKED _WTBL_PEER_PS_T
{
	UINT16	u2Tag;		// Tag = 0x04
	UINT16	u2Length;
	UINT8	ucDuIPsm;
	UINT8	ucIPsm;
	UINT8	ucRsvd0;
	UINT8	ucRsvd1;
} CMD_WTBL_PEER_PS_T, *P_CMD_WTBL_PEER_PS_T;

typedef struct GNU_PACKED _WTBL_TX_PS_T
{
	UINT16	u2Tag;		// Tag = 0x05
	UINT16	u2Length;
	UINT8	ucTxPs;
	UINT8	ucRsvd0;
	UINT8	ucRsvd1;
	UINT8	ucRsvd2;
} CMD_WTBL_TX_PS_T, *P_CMD_WTBL_TX_PS_T;

typedef struct GNU_PACKED _WTBL_HDR_TRANS_T
{
	UINT16	u2Tag;		// Tag = 0x06
	UINT16	u2Length;
	UINT8	ucTd;
	UINT8	ucFd;
	UINT8	ucDisRhtr;
	UINT8	ucRsvd0;
} CMD_WTBL_HDR_TRANS_T, *P_CMD_WTBL_HDR_TRANS_T;

typedef struct GNU_PACKED _WTBL_SECURITY_KEY_T
{
	UINT16	u2Tag;		// Tag = 0x07
	UINT16	u2Length;
	UINT8	ucAddRemove; /* 0: add, 1: remove */
	//UINT8	ucKeyType; /* 0: SHAREDKEYTABLE, 1: PAIRWISEKEYTABLE */
	UINT8	ucRkv;
	UINT8	ucIkv;
	UINT8	ucAlgorithmId; /* refer to ENUM_CIPHER_SUIT_T256 */
	UINT8	ucKeyId;
	UINT8	ucKeyLen;
	UINT8	ucrRsvd0;
	UINT8	ucrRsvd1;
	UINT8	aucKeyMaterial[32];
} CMD_WTBL_SECURITY_KEY_T, *P_CMD_WTBL_SECURITY_KEY_T;

typedef struct GNU_PACKED _WTBL_BA_T
{
	UINT16	u2Tag;		// Tag = 0x08
	UINT16	u2Length;
	/* Recipient + Originator */
	UINT8	ucTid;
	UINT8	ucBaSessionType;
	UINT8	aucReserved[2];
	/* Originator */
	UINT16	u2Sn;
	UINT8	ucBaEn;
	UINT8	ucBaSize;
	UINT8	aucReserved1[2];
	/* Recipient */
	UINT8	aucPeerAddress[MAC_ADDR_LEN];
	UINT8	ucRstBaTid;
	UINT8	ucRstBaSel;
	UINT8	ucStartRstBaSb;
	UINT8	ucBandIdx;
	UINT8	aucReserved2[4];
} CMD_WTBL_BA_T, *P_CMD_WTBL_BA_T;

typedef struct GNU_PACKED _WTBL_RDG_T
{
	UINT16	u2Tag;		// Tag = 0x09
	UINT16	u2Length;
	UINT8	ucRdgBa;
	UINT8	ucR;
	UINT8	ucrRsvd0;
	UINT8	ucrRsvd1;
} CMD_WTBL_RDG_T, *P_CMD_WTBL_RDG_T;

typedef struct GNU_PACKED _WTBL_PROTECTION_T
{
	UINT16	u2Tag;		// Tag = 0x0a
	UINT16	u2Length;
	UINT8	ucRts;
	UINT8	ucrRsvd0;
	UINT8	ucrRsvd1;
	UINT8	ucrRsvd2;
} CMD_WTBL_PROTECTION_T, *P_CMD_WTBL_PROTECTION_T;

/* bit 0: Clear PSM (WF_WTBLON: 0x60322300, Bit 31 set 1 then set 0) */
/* bit 1: Clear BA (WTBL2.DW15) */
/* bit 2: Clear Rx Counter (6019_00002, bit 14) */
/* bit 3: Clear Tx Counter (6019_0000, bit 15) */
/* bit 4: Clear ADM Counter (6019_0000, bit 12) */
/* bit 5: Clear Cipher key (WTBL3)*/

typedef struct GNU_PACKED _WTBL_CLEAR_T
{
	UINT16	u2Tag;		// Tag = 0x0b
	UINT16	u2Length;
	UINT8	ucClear;
	UINT8	ucrRsvd0;
	UINT8	ucrRsvd1;
	UINT8	ucrRsvd2;
} CMD_WTBL_CLEAR_T, *P_CMD_WTBL_CLEAR_T;

typedef struct GNU_PACKED _WTBL_BF_T
{
	UINT16	u2Tag;		// Tag = 0x0c
	UINT16	u2Length;
	UINT8	ucTiBf;
	UINT8	ucTeBf;
	UINT8	ucTibfVht;
	UINT8	ucTebfVht;
	UINT8	ucGid;
	UINT8	ucPFMUIdx;
	UINT8	ucrRsvd1;
	UINT8	ucrRsvd2;
} CMD_WTBL_BF_T, *P_CMD_WTBL_BF_T;

typedef struct GNU_PACKED _WTBL_SMPS_T
{
	UINT16	u2Tag;		// Tag = 0x0d
	UINT16	u2Length;
	UINT8	ucSmPs;
	UINT8	ucrRsvd0;
	UINT8	ucrRsvd1;
	UINT8	ucrRsvd2;
} CMD_WTBL_SMPS_T, *P_CMD_WTBL_SMPS_T;

typedef struct GNU_PACKED _WTBL_RAW_DATA_RW_T
{
	UINT16	u2Tag;		// Tag = 0x0e
	UINT16	u2Length;
	UINT8	ucWtblIdx;           /* WTBL 1/2/3/4 in MT7636, the field don't care in MT7615 */
	UINT8	ucWhichDW;
	UINT8	aucReserve[2];
	UINT32	u4DwMask;		/* Show these bits don't be writen */
	UINT32	u4DwValue;
} CMD_WTBL_RAW_DATA_RW_T, *P_CMD_WTBL_RAW_DATA_RW_T;

typedef struct GNU_PACKED _WTBL_SPE_T
{
	UINT16	u2Tag;		// Tag = 0x10
	UINT16	u2Length;
	UINT8	ucSpeIdx;
	UINT8	ucrRsvd0;
	UINT8	ucrRsvd1;
	UINT8	ucrRsvd2;
} CMD_WTBL_SPE_T, *P_CMD_WTBL_SPE_T;

#ifdef MT7615
#define	WTBL_BUFFER_SIZE		0x100
#endif /* MT7615 */

#ifdef MT7622
#define	WTBL_BUFFER_SIZE		0x100
#endif /* MT7622 */


typedef struct GNU_PACKED _WTBL_DUMP_T
{
	UINT16	u2Tag;		// Tag = 0x0f
	UINT16	u2Length;
	UINT8	aucWtblBuffer[WTBL_BUFFER_SIZE]	;    // need 4 byte alignment
} CMD_WTBL_DUMP_T, *P_CMD_WTBL_DUMP_T;

#define SKU_SIZE 49

enum {
	SKU_CCK_1_2=0,
	SKU_CCK_55_11,
	SKU_OFDM_6_9,
	SKU_OFDM_12_18,
	SKU_OFDM_24_36,
	SKU_OFDM_48,
	SKU_OFDM_54,
	SKU_HT20_0_8,
	SKU_HT20_32,
	SKU_HT20_1_2_9_10,
	SKU_HT20_3_4_11_12,
	SKU_HT20_5_13,
	SKU_HT20_6_14,
	SKU_HT20_7_15,
	SKU_HT40_0_8,
	SKU_HT40_32,
	SKU_HT40_1_2_9_10,
	SKU_HT40_3_4_11_12,
	SKU_HT40_5_13,
	SKU_HT40_6_14,
	SKU_HT40_7_15,
	SKU_VHT20_0,
	SKU_VHT20_1_2,
	SKU_VHT20_3_4,
	SKU_VHT20_5_6,
	SKU_VHT20_7,
	SKU_VHT20_8,
	SKU_VHT20_9,
	SKU_VHT40_0,
	SKU_VHT40_1_2,
	SKU_VHT40_3_4,
	SKU_VHT40_5_6,
	SKU_VHT40_7,
	SKU_VHT40_8,
	SKU_VHT40_9,
	SKU_VHT80_0,
	SKU_VHT80_1_2,
	SKU_VHT80_3_4,
	SKU_VHT80_5_6,
	SKU_VHT80_7,
	SKU_VHT80_8,
	SKU_VHT80_9,
	SKU_VHT160_0,
	SKU_VHT160_1_2,
	SKU_VHT160_3_4,
	SKU_VHT160_5_6,
	SKU_VHT160_7,
	SKU_VHT160_8,
	SKU_VHT160_9,
	SKU_1SS_Delta,
	SKU_2SS_Delta,
	SKU_3SS_Delta,
	SKU_4SS_Delta,
};

#define BF_GAIN_FINAL_SIZE    10
#define BF_GAIN_STREAM_SIZE    4

enum {
	ENTRY_1 = 0,  // 1T 1Nss
    ENTRY_2,      // 2T 1Nss
    ENTRY_3,      // 2T 2Nss
    ENTRY_4,      // 3T 1Nss
    ENTRY_5,      // 3T 2Nss
    ENTRY_6,      // 3T 3Nss
    ENTRY_7,      // 4T 1Nss
    ENTRY_8,      // 4T 2Nss
    ENTRY_9,      // 4T 3Nss
    ENTRY_10,     // 4T 4Nss
};

enum {
    BF_GAIN_1T = 0,
    BF_GAIN_2T,
    BF_GAIN_3T,
    BF_GAIN_4T,
};

enum {
	CMD_BW_20,
	CMD_BW_40,
	CMD_BW_80,
	CMD_BW_160,
	CMD_BW_10,
	CMD_BW_5,
	CMD_BW_8080
};

typedef struct GNU_PACKED _EXT_CMD_CHAN_SWITCH_T {
    UINT8	ucPrimCh;
    UINT8	ucCentralCh;
    UINT8	ucBW;
    UINT8	ucTxStreamNum;
    
    UINT8	ucRxStreamNum;
    UINT8	ucSwitchReason;
    UINT8	ucDbdcIdx;
    UINT8	ucCentralCh2;
    
    UINT16	u2CacCase;
    UINT8	ucBand;
    UINT8	aucReserve0[1];
    
	UINT32  u4OutBandFreq;
	
	INT8    cTxPowerDrop;
    UINT8	aucReserve1[3];
    
    UINT8	aucTxPowerSKU[SKU_TOTAL_SIZE];
    UINT8	aucReserve2[3];
} EXT_CMD_CHAN_SWITCH_T, *P_EXT_CMD_CHAN_SWITCH_T;

#if defined(MT7615) || defined(MT7622)
#define EFUSE_CONTENT_SIZE 908 /* EFUSE_CONTENT_START to EFUSE_CONTENT_END */
#define EFUSE_CONTENT_START 0x34
#define EFUSE_CONTENT_END 0x3BF
#elif defined (MT7637)
// todo: efuse structure need unify
#define EFUSE_CONTENT_SIZE 368 /* EFUSE_CONTENT_START to EFUSE_CONTENT_END */
#define EFUSE_CONTENT_START 0x2E
#define EFUSE_CONTENT_END 0x19D
#else
#define EFUSE_CONTENT_SIZE 224
#define EFUSE_CONTENT_START 0x20
#define EFUSE_CONTENT_END 0xFF
#endif
typedef struct GNU_PACKED _EXT_EVENT_ID_GET_TX_POWER_T {
	UINT8 ucTxPwrType;
	UINT8 ucEfuseAddr;
	UINT8 ucEfuseContent;
	UINT8 ucBand;
} EXT_EVENT_ID_GET_TX_POWER_T, *P_EXT_EVENT_ID_GET_TX_POWER_T;

typedef struct GNU_PACKED _EXT_CMD_GET_TX_POWER_T{
	UINT8 ucTxPwrType;
	UINT8 ucCenterChannel;
	UINT8 ucDbdcIdx;
	UINT8 ucBand;
	UINT8 aucReserved[4];
} EXT_CMD_GET_TX_POWER_T, *P_EXT_CMD_GET_TX_POWER_T;

typedef struct GNU_PACKED _EXT_CMD_TX_POWER_CTRL_T {
	UINT8 ucCenterChannel;
	UINT8 ucDbdcIdx;
	UINT8 ucBand;
	UINT8 ucReserved[1];
	UINT8 aucBinContent[EFUSE_CONTENT_SIZE];
} EXT_CMD_TX_POWER_CTRL_T, *P_EXT_CMD_TX_POWER_CTRL_T;

#ifdef ABSOLUTE_POWER_TEST
typedef struct _CMD_POWER_RATE_TXPOWER_CTRL_T
{
    UINT8    ucPowerCtrlFormatId;
    UINT8    ucPhyMode;
	UINT8    ucTxRate;
	UINT8    ucBW;
    UINT8    ucBandIdx;
	INT8     cTxPower;
    UINT8    ucReserved[2];
} CMD_POWER_RATE_TXPOWER_CTRL_T, *P_CMD_POWER_RATE_TXPOWER_CTRL_T;
#endif /* ABSOLUTE_POWER_TEST */

#ifdef BACKGROUND_SCAN_SUPPORT
typedef struct GNU_PACKED _EXT_CMD_BGND_SCAN_NOTIFY_T {
	UINT8 ucNotifyFunc;
	UINT8 ucBgndScanStatus;
	UINT8 resv[2];
} EXT_CMD_BGND_SCAN_NOTIFY_T, *P_EXT_CMD_BGND_SCAN_NOTIFY_T;
#endif /* BACKGROUND_SCAN_SUPPORT */

typedef struct _INIT_EVENT_ACCESS_REG {
	UINT32 u4Address;
	UINT32 u4Data;
} INIT_EVENT_ACCESS_REG, *P_INIT_EVENT_ACCESS_REG;

#define CMD_RESULT_SUCCESS 0
#define CMD_RESULT_NONSUPPORT 254


typedef struct _INIT_EVENT_CMD_RESULT {
        UINT8 ucStatus;
	UINT8 ucCID;
        UINT8 aucReserved[2];
} INIT_EVENT_CMD_RESULT;

typedef struct GNU_PACKED _EVENT_EXT_CMD_RESULT_T {
	UINT8 ucExTenCID;
	UINT8 aucReserve[3];
	UINT32 u4Status;
} EVENT_EXT_CMD_RESULT_T, *PEVENT_EXT_CMD_RESULT_T;


typedef struct GNU_PACKED _EVENT_STAREC_UPDATE_T
{
    UINT8   ucExtenCID;		// Fix at 0x25
    UINT8   aucReserve[3];
    UINT32  u4Status;
    UINT8	ucBssInfoIdx;
    UINT8	ucWlanIdx;
    UINT16	u2TotalElementNum;
    UINT8   ucMuarIdx;
    UINT8	aucReserved[3];
    UINT8	aucBuffer[];
} EVENT_STAREC_UPDATE_T, *P_EVENT_STAREC_UPDATE_T;


typedef struct GNU_PACKED _EVENT_BSSINFO_UPDATE_T
{
    UINT8   ucExtenCID;		// Fix at 0x26
    UINT8   aucReserve[3];
    UINT32  u4Status;
    UINT8	ucBssInfoIdx;
    UINT8	ucReserve;
    UINT16	u2TotalElementNum;
    UINT8	aucReserved[4];
    UINT8	aucBuffer[];
} EVENT_BSSINFO_UPDATE_T, *P_EVENT_BSSINFO_UPDATE_T;

typedef struct GNU_PACKED _EVENT_DEVINFO_UPDATE_T
{
    UINT8   ucExtenCID;		// Fix at 0x2A
    UINT8   aucReserve[3];
    UINT32  u4Status;
    UINT8	ucOwnMacIdx;
    UINT8	ucReserve;
    UINT16	u2TotalElementNum;
    UINT8	aucReserved[4];
    UINT8	aucBuffer[];
} EVENT_DEVINFO_UPDATE_T, *P_EVENT_DEVINFO_UPDATE_T;

typedef struct GNU_PACKED _EXT_EVENT_NIC_CAPABILITY_T {
	UINT8 aucDateCode[16];
	UINT8 aucVersionCode[12];
} EXT_EVENT_NIC_CAPABILITY;

#ifdef MT_MAC
// TODO: Star, fix me, the "RF_CR" is dupicated with andes_rlt.h
enum {
	MAC_CR,
	RF_CR,
};
#endif /* MT_MAC */

typedef struct GNU_PACKED _CMD_MULTI_CR_ACCESS_T {
	UINT32 u4Type;
	UINT32 u4Addr;
	UINT32 u4Data;
} CMD_MULTI_CR_ACCESS_T;

typedef struct GNU_PACKED _EXT_EVENT_MULTI_CR_ACCESS_WR_T {
	UINT32 u4Status;
	UINT32 u4Resv;
	UINT32 u4Resv2;
} EXT_EVENT_MULTI_CR_ACCESS_WR_T;

typedef struct GNU_PACKED _EXT_EVENT_MULTI_CR_ACCESS_RD_T {
	UINT32 u4Type;
	UINT32 u4Addr;
	UINT32 u4Data;
} EXT_EVENT_MULTI_CR_ACCESS_RD_T;

enum {
	ANDES_LOG_DISABLE,
	ANDES_LOG_TO_UART,
	ANDES_LOG_TO_EVENT,
};

typedef struct GNU_PACKED _EXT_CMD_FW_LOG_2_HOST_CTRL_T {
	UINT8 ucFwLog2HostCtrl;
	UINT8 ucReserve[3];
} EXT_CMD_FW_LOG_2_HOST_CTRL_T;


typedef struct GNU_PACKED _CMD_AP_PS_RETRIEVE_T {
    UINT32 u4Option; /* 0: AP_PWS enable, 1: redirect disable */
    UINT32 u4Param1; /* for 0: enable/disable. for 1: wlan idx */
    UINT32 u4Resv;
} CMD_AP_PS_RETRIEVE_STRUC_T, *P_CMD_AP_PS_RETRIEVE_STRUC_T;

typedef struct GNU_PACKED _EXT_EVENT_AP_PS_RETRIEVE_T {
    UINT32 u4Param1; /* for 0: enable/disable. for 1: wlan idx */
    UINT32 u4Resv;
    UINT32 u4Resv2;
} EXT_EVENT_AP_PS_RETRIEVE_T, *P_EXT_EVENT_AP_PS_RETRIEVE_T;


typedef struct GNU_PACKED _BIN_CONTENT_T {
	UINT16 u2Addr;
	UINT8 ucValue;
	UINT8 ucReserved;
} BIN_CONTENT_T, *P_BIN_CONTENT_T;

typedef struct GNU_PACKED _EXT_CMD_GET_SENSOR_RESULT_T {
    UINT8 ucActionIdx;
    UINT8 aucReserved[3];
} EXT_CMD_GET_SENSOR_RESULT_T, *P_EXT_CMD_GET_SENSOR_RESULT_T;

typedef struct GNU_PACKED _EXT_EVENT_GET_SENSOR_RESULT_T
{
    UINT32 u4SensorResult;
    UINT32 u4Reserved;
} EXT_EVENT_GET_SENSOR_RESULT_T, *P_EXT_EVENT_GET_SENSOR_RESULT_T;
typedef struct GNU_PACKED _EXT_EVENT_TDLS_SETUP_T
{
    UINT8  ucResultId;
    UINT8  aucReserved[3];

    UINT32 u4StartTime;
    UINT32 u4EndTime;
    UINT32 u4TbttTime;
    UINT32 u4StayTime;
    UINT32 u4RestTime;
} EXT_EVENT_TDLS_SETUP_T, *P_EXT_EVENT_TDLS_SETUP_T;

typedef struct GNU_PACKED _EXT_EVENT_TDLS_STATUS_T
{
    UINT8  ucResultId;
    UINT8  aucReserved[3];
} EXT_EVENT_TDLS_STATUS_T, *P_EXT_EVENT_TDLS_STATUS_T;
#if defined(MT7615) || defined(MT7622) //buffer mode workaround
typedef struct GNU_PACKED _EXT_CMD_EFUSE_BUFFER_MODE_T {
	UINT8 ucSourceMode;
	UINT8 ucReserved; //Keep next UINT16 16-bit aligned, this needs correct N9 FW.
	UINT16 ucCount;	
	UINT8 ucBinContent[EFUSE_CONTENT_SIZE];
} EXT_CMD_EFUSE_BUFFER_MODE_T, *P_EXT_CMD_EFUSE_BUFFER_MODE_T;
#else
typedef struct _EXT_CMD_EFUSE_BUFFER_MODE_T {
	UINT8 ucSourceMode;
	UINT16 ucCount;
	UINT8 ucReserved;
	BIN_CONTENT_T aBinContent[EFUSE_CONTENT_SIZE];
} EXT_CMD_EFUSE_BUFFER_MODE_T, *P_EXT_CMD_EFUSE_BUFFER_MODE_T;
#endif /* defined(MT7615) || defined(MT7622) */

typedef struct GNU_PACKED _EXT_CMD_EVENT_DUMP_MEM_T {
	UINT32	u4MemAddr;
	UINT8  	ucData[64];
} EXT_CMD_EVENT_DUMP_MEM_T, *P_EXT_CMD_EVENT_DUMP_MEM_T;

typedef struct _MEM_DUMP_DATA_T {
	PUINT8 pValue;
} MEM_DUMP_DATA_T;

typedef enum _EXT_ENUM_PM_FEATURE_T
{
    PM_CMD_FEATURE_PSPOLL_OFFLOAD       = 0x00000001,
    PM_CMD_FEATURE_PS_TX_REDIRECT        = 0x00000002,
    PM_CMD_FEATURE_SMART_BCN_SP          = 0x00000004,
	PM_CMD_FEATURE_SEND_NULL_FRAME 		 = 0x00000008,
} EXT_ENUM_PM_FEATURE_T;


enum _ENUM_BCN_LOSS_REASON_T
{
	/* For STA/ApCli mode (Beacon stop receiving) */
	ENUM_BCN_LOSS_STA = 			0x00,

	/* For AP mode (Beacon stop sending) */
	ENUM_BCN_LOSS_AP_DISABLE = 		0x10,
	ENUM_BCN_LOSS_AP_SER_TRIGGER = 	0x11,
	ENUM_BCN_LOSS_AP_ERROR =	 	0x12
};

typedef struct GNU_PACKED _EXT_EVENT_BEACON_LOSS_T
{
	UINT8		aucBssid[6];
	UINT8		ucReason;
	UINT8		ucReserve;
} EXT_EVENT_BEACON_LOSS_T, *P_EXT_EVENT_BEACON_LOSS_T;

#ifdef MT_DFS_SUPPORT//Jelly20150123
typedef struct GNU_PACKED _EXT_EVENT_RDD_REPORT_T
{
	UINT8       ucRddIdx;
	UINT8       aucReserve[3];
} EXT_EVENT_RDD_REPORT_T, *P_EXT_EVENT_RDD_REPORT_T;

typedef struct GNU_PACKED _EXT_EVENT_CAC_END_T
{
	UINT8       ucRddIdx;
	UINT8       aucReserve[3];
} EXT_EVENT_CAC_END_T, *P_EXT_EVENT_CAC_END_T;
#endif

typedef struct GNU_PACKED _EXT_EVENT_ROAMING_DETECT_RESULT_T
{
	UINT8		ucBssidIdx;
	UINT8		aucReserved[3];
	UINT32		u4RoamReason;
}EXT_EVENT_ROAMING_DETECT_RESULT_T, *P_EXT_EVENT_ROAMING_DETECT_RESULT_T;

enum
{
	ROAMING_STATUS_NOT_DETERMINED = 0x00,
	ROAMING_RCPI_CCK_EXCEED_MAX = 0x01,
	ROAMING_RCPI_CCK_LOWER_MIN = 0x02,
	ROAMING_RCPI_OFDM_EXCEED_MAX = 0x04,
	ROAMING_RCPI_OFDM_LOWER_MIN  = 0x08,
	ROAMING_LQ_CCK_EXCEED_MAX = 0x10,
	ROAMING_LQ_CCK_LOWER_MIN  = 0x20,
	ROAMING_LQ_OFDM_EXCEED_MAX = 0x40,
	ROAMING_LQ_OFDM_LOWER_MIN = 0x80,
};

typedef struct GNU_PACKED _EXT_EVENT_ASSERT_DUMP_T
{
	UINT8	aucBuffer[1000]; //temparary size
} EXT_EVENT_ASSERT_DUMP_T, *P_EXT_EVENT_ASSERT_DUMP_T;

typedef struct GNU_PACKED _EXT_CMD_PWR_MGT_BIT_T
{
	UINT8		ucWlanIdx;
	UINT8		ucPwrMgtBit;
	UINT8		aucReserve[2];
} EXT_CMD_PWR_MGT_BIT_T, *P_EXT_CMD_PWR_MGT_BIT_T;

enum {
	 EEPROM_MODE_EFUSE = 0,
	 EEPROM_MODE_BUFFER = 1,
};

typedef struct GNU_PACKED _CMD_ACCESS_EFUSE_T
{
    UINT32         u4Address;
    UINT32         u4Valid;
    UINT8          aucData[16];
} CMD_ACCESS_EFUSE_T, *P_CMD_ACCESS_EFUSE_T, _EXT_EVENT_ACCESS_EFUSE_T;


typedef struct _EFUSE_ACCESS_DATA_T{
	PUINT pIsValid;
	PUSHORT pValue;
}EFUSE_ACCESS_DATA_T;


typedef struct GNU_PACKED _EXT_CMD_EFUSE_FREE_BLOCK_T
{
    UINT8 ucGetFreeBlock;
    UINT8 aucReserve[3];
} EXT_CMD_EFUSE_FREE_BLOCK_T, *P_EXT__CMD_EFUSE_FREE_BLOCK_T;


typedef struct GNU_PACKED _EXT_EVENT_EFUSE_FREE_BLOCK_T
{
    UINT32 ucFreeBlockNum;
    UINT8 aucReserve[4];
} EXT_EVENT_EFUSE_FREE_BLOCK_T, *P_EXT_EVENT_EFUSE_FREE_BLOCK_T;


enum {
	HIGH_TEMP_THD = 0,
	LOW_TEMP_THD = 1,
};

enum _ENUM_THERMAL_PROTECTION_EXTRA_TAG {
    THERAML_PROTECTION_TAG_SET_ADMIT_DUTY = 1
};

typedef enum _ENUM_THERMAL_PROTECTION_REASON_T {
    THERAML_PROTECTION_REASON_NTX,
    THERAML_PROTECTION_REASON_ADM,
    THERAML_PROTECTION_REASON_RADIO,
    THERAML_PROTECTION_REASON_TEMPERATURE
} ENUM_THERMAL_PROTECTION_REASON_T, *P_ENUM_THERMAL_PROTECTION_REASON_T; 

typedef struct GNU_PACKED _EXT_CMD_THERMAL_PROTECT_T {
	UINT8 ucHighEnable;
	CHAR cHighTempThreshold;
	UINT8 ucLowEnable;
	CHAR cLowTempThreshold;
	UINT32 RecheckTimer;
    UINT8 ucRFOffEnable;
    CHAR cRFOffThreshold;
	UINT8 ucType;
    UINT8 ucExtraTag;
    UINT8 ucLv0Duty;
    UINT8 ucLv1Duty;
    UINT8 ucLv2Duty;
    UINT8 ucLv3Duty;
} EXT_CMD_THERMAL_PROTECT_T, *P_EXT_CMD_THERMAL_PROTECT_T;

typedef struct GNU_PACKED _EXT_EVENT_THERMAL_PROTECT_T {
	UINT8 ucHLType;
	CHAR cCurrentTemp;
    UINT8 ucReason;
	UINT8 aucReserve;
} EXT_EVENT_THERMAL_PROTECT_T, *P_EXT_EVENT_THERMAL_PROTECT_T;

typedef struct GNU_PACKED _EXT_CMD_TMR_CAL_T {
	UINT8 ucEnable;
	UINT8 ucBand;//0: 2G, 1: 5G
	UINT8 ucBW;//0: 20MHz, 1: 40MHz, 2: 80MHz, 3: 160MHz, 4: 10MHz, 5: 5MHz
	UINT8 ucAnt;//0: Atn0, 1: Ant1

	UINT8 ucRole;//0: initiator, 1: responder
	UINT8 aucReserve[3];
} EXT_CMD_TMR_CAL_T, *P_EXT_CMD_TMR_CAL_T;


typedef struct GNU_PACKED _UPDATE_RTS_THRESHOLD_T {
    UINT32 u4RtsPktLenThreshold;
    UINT32 u4RtsPktNumThreshold;
} UPDATE_RTS_THRESHOLD_T, *P_UPDATE_RTS_THRESHOLD_T;


typedef struct GNU_PACKED _UPDATE_PROTECTION_T {
    UINT8 ucLongNav;
    UINT8 ucMMProtect;
    UINT8 ucGFProtect;
    UINT8 ucBW40Protect;
    UINT8 ucRifsProtect;
    UINT8 ucBW80Protect;
    UINT8 ucBW160Protect;
    UINT8 ucERProtectMask;
} UPDATE_PROTECTION_T, *P_UPDATE_PROTECTION_T;


#define UPDATE_RTS_THRESHOLD    0x1
#define UPDATE_PROTECTION_CTRL  0x2
typedef struct GNU_PACKED _EXT_CMD_UPDATE_PROTECT_T {
    UINT8 ucProtectIdx; // 0: Rsv. 1: Rts Threshold 2: Protect
    UINT8 ucDbdcIdx;
    UINT8 aucRsv[2];

    union {
        UPDATE_RTS_THRESHOLD_T  rUpdateRtsThld;
        UPDATE_PROTECTION_T     rUpdateProtect;
    } Data;
} EXT_CMD_UPDATE_PROTECT_T, *P_EXT_CMD_UPDATE_PROTECT_T;


typedef struct GNU_PACKED _EXT_CMD_RDG_CTRL_T {
	UINT32 u4TxOP;
    UINT8 ucLongNav;
	UINT8 ucInit;
	UINT8 ucResp;
	UINT8 ucWlanIdx;
    UINT8 ucBand;
    UINT8 aucReserved[3];
} EXT_CMD_RDG_T, *P_EXT_CMD_RDG_T;

#ifdef VOW_SUPPORT
/*************************************************/
/* EXT_CMD_ID_DRR_CTRL = 0x36 */
/*************************************************/
typedef struct GNU_PACKED _AIRTIME_QUA_ALL_FIELD_T {
    UINT8     ucAirTimeQuantum[8];
} AIRTIME_QUA_ALL_FIELD_T, *P_AIRTIME_QUA_ALL_FIELD_T;

typedef union GNU_PACKED _VOW_DRR_CTRL_VALUE_T {
    UINT32                     u4ComValue;
    AIRTIME_QUA_ALL_FIELD_T     rAirTimeQuantumAllField;    /* used for ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_ALL ctrl only */
} VOW_DRR_CTRL_VALUE_T, *P_VOW_DRR_CTRL_VALUE_T;

typedef struct GNU_PACKED _EXT_CMD_VOW_DRR_CTRL_T
{
    UINT32                 u4CtrlFieldID;
    UINT8                  ucStaID;
    UINT8                  ucCtrlStatus;
    UINT8                  ucReserve[2];
    UINT32                 u4ReserveDW;
    VOW_DRR_CTRL_VALUE_T    rAirTimeCtrlValue;
} EXT_CMD_VOW_DRR_CTRL_T, *P_EXT_CMD_VOW_DRR_CTRL_T;

typedef enum _ENUM_EXT_CMD_VOW_DRR_CTRL_ID_T
{
    /* Type 1 */
    ENUM_VOW_DRR_CTRL_FIELD_STA_ALL             = 0x00000000,
    ENUM_VOW_DRR_CTRL_FIELD_STA_BSS_GROUP       = 0x00000001,
    ENUM_VOW_DRR_CTRL_FIELD_STA_PRIORITY        = 0x00000002,
    ENUM_VOW_DRR_CTRL_FIELD_STA_AC0_QUA_ID      = 0x00000003,
    ENUM_VOW_DRR_CTRL_FIELD_STA_AC1_QUA_ID      = 0x00000004,
    ENUM_VOW_DRR_CTRL_FIELD_STA_AC2_QUA_ID      = 0x00000005,
    ENUM_VOW_DRR_CTRL_FIELD_STA_AC3_QUA_ID      = 0x00000006,
    ENUM_VOW_DRR_CTRL_FIELD_STA_WMM_ID              = 0x00000007,

    ENUM_VOW_DRR_CTRL_FIELD_TYPE_1_BOUNDARY     = 0x0000000f,

    /* Type 2 */
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_DEFICIT_BOUND   = 0x00000010,
    ENUM_VOW_DRR_CTRL_FIELD_BW_DEFICIT_BOUND        = 0x00000011,

    ENUM_VOW_DRR_CTRL_FIELD_TYPE_2_BOUNDARY     = 0x0000001f,

    /* Type 3 */
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L0  = 0x00000020,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L1  = 0x00000021,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L2  = 0x00000022,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L3  = 0x00000023,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L4  = 0x00000024,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L5  = 0x00000025,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L6  = 0x00000026,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_L7  = 0x00000027,
    ENUM_VOW_DRR_CTRL_FIELD_AIRTIME_QUANTUM_ALL = 0x00000028,

    ENUM_VOW_DRR_CTRL_FIELD_TYPE_3_BOUNDARY     = 0x0000002f,

    /* Type 4 */
    ENUM_VOW_DRR_CTRL_FIELD_STA_PAUSE_SETTING       = 0x00000030,
    ENUM_VOW_DRR_CTRL_FIELD_TYPE_4_BOUNDARY         = 0x0000003f,

} ENUM_EXT_CMD_VOW_DRR_CTRL_ID_T, P_ENUM__EXT_CMD_VOW_DRR_CTRL_ID_T;
/* end of EXT_CMD_ID_DRR_CTRL = 0x36 */

/*************************************************/
/* EXT_CMD_ID_BSSGROUP_CTRL = 0x37 */
/*************************************************/
typedef struct GNU_PACKED _BW_BSS_TOKEN_SETTING_T
{
    /* DW#0 */
    UINT16     u2MinRateToken;                 /* unit: 1 bit */
    UINT16     u2MaxRateToken;                 /* unit: 1 bit */

		/* DW#1 */
#ifdef RT_BIG_ENDIAN
	UINT32	   u4MinTokenBucketLengSize:12;    /* unit: 1024 bit */
	UINT32	   u4D1B19Rev:1;				   /* reserve */
	UINT32	   u4MinAirTimeToken:11;		   /* unit: 1/8 us */
	UINT32	   u4MinTokenBucketTimeSize:8;	   /* unit: 1024 us */
#else
	UINT32	   u4MinTokenBucketTimeSize:8;	   /* unit: 1024 us */
	UINT32	   u4MinAirTimeToken:11;		   /* unit: 1/8 us */
	UINT32	   u4D1B19Rev:1;				   /* reserve */
	UINT32	   u4MinTokenBucketLengSize:12;    /* unit: 1024 bit */
#endif
	/* DW#2 */
#ifdef RT_BIG_ENDIAN
	UINT32	   u4MaxTokenBucketLengSize:12;    /* unit: 1024 bit */
	UINT32	   u4D2B19Rev:1;				   /* reserve */	
	UINT32	   u4MaxAirTimeToken:11;		   /* unit: 1/8 us */
	UINT32	   u4MaxTokenBucketTimeSize:8;	   /* unit: 1024 us */
#else
	UINT32	   u4MaxTokenBucketTimeSize:8;	   /* unit: 1024 us */
	UINT32	   u4MaxAirTimeToken:11;		   /* unit: 1/8 us */
	UINT32	   u4D2B19Rev:1;				   /* reserve */
	UINT32	   u4MaxTokenBucketLengSize:12;    /* unit: 1024 bit */
#endif
	/* DW#3 */
#ifdef RT_BIG_ENDIAN
	UINT32	   u4D3B28toB31Rev:4;			   /* reserve */	
	UINT32	   u4MaxBacklogSize:12; 		   /* unit: 1024 bit */
	UINT32	   u4D3B8toB15Rev:8;			   /* reserve */
	UINT32	   u4MaxWaitTime:8; 			   /* unit: 1024 us */
#else
	UINT32	   u4MaxWaitTime:8; 			   /* unit: 1024 us */
	UINT32	   u4D3B8toB15Rev:8;			   /* reserve */
	UINT32	   u4MaxBacklogSize:12; 		   /* unit: 1024 bit */
	UINT32	   u4D3B28toB31Rev:4;			   /* reserve */
#endif

} BW_BSS_TOKEN_SETTING_T, *P_BW_BSS_TOKEN_SETTING_T;

typedef struct GNU_PACKED _EXT_CMD_BSS_CTRL_T
{
    UINT32                     u4CtrlFieldID;
    UINT8                      ucBssGroupID;
    UINT8                      ucCtrlStatus;
    UINT8                      ucReserve[2];
    UINT32                     u4ReserveDW;
    UINT32                     u4SingleFieldIDValue;
    BW_BSS_TOKEN_SETTING_T   arAllBssGroupMultiField[16];
    UINT8                      ucBssGroupQuantumTime[16];
} EXT_CMD_BSS_CTRL_T, *P_EXT_CMD_BSS_CTRL_T;

typedef enum _ENUM_EXT_CMD_BSSGROUP_CTRL_ID_T
{
    ENUM_BSSGROUP_CTRL_ALL_ITEM_FOR_1_GROUP                     = 0x00,
    ENUM_BSSGROUP_CTRL_MIN_RATE_TOKEN_CFG_ITEM                  = 0x01,
    ENUM_BSSGROUP_CTRL_MAX_RATE_TOKEN_CFG_ITEM                  = 0x02,
    ENUM_BSSGROUP_CTRL_MIN_TOKEN_BUCKET_TIME_SIZE_CFG_ITEM      = 0x03,
    ENUM_BSSGROUP_CTRL_MIN_AIRTIME_TOKEN_CFG_ITEM               = 0x04,
    ENUM_BSSGROUP_CTRL_MIN_TOKEN_BUCKET_LENG_SIZE_CFG_ITEM = 0x05,
    ENUM_BSSGROUP_CTRL_MAX_TOKEN_BUCKET_TIME_SIZE_CFG_ITEM = 0x06,
    ENUM_BSSGROUP_CTRL_MAX_AIRTIME_TOKEN_CFG_ITEM               = 0x07,
    ENUM_BSSGROUP_CTRL_MAX_TOKEN_BUCKET_LENG_SIZE_CFG_ITEM = 0x08,
    ENUM_BSSGROUP_CTRL_MAX_WAIT_TIME_CFG_ITEM                   = 0x09,
    ENUM_BSSGROUP_CTRL_MAX_BACKLOG_SIZE_CFG_ITEM                = 0x0a,
    ENUM_BSSGROUP_CTRL_ALL_ITEM_FOR_ALL_GROUP                   = 0x10,

    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_BASE                    = 0x20,

    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_00                    = 0x20,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_01                    = 0x21,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_02                    = 0x22,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_03                    = 0x23,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_04                    = 0x24,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_05                    = 0x25,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_06                    = 0x26,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_07                    = 0x27,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_08                    = 0x28,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_09                    = 0x29,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_0A                    = 0x2A,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_0B                    = 0x2B,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_0C                    = 0x2C,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_0D                    = 0x2D,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_0E                    = 0x2E,
    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_L_0F                    = 0x2F,

    ENUM_BSSGROUP_CTRL_BW_GROUP_QUANTUM_ALL                     = 0x30,

}ENUM_EXT_CMD_BSSGROUP_CTRL_ID_T, P_ENUM_EXT_CMD_BSSGROUP_CTRL_ID_T;
/* end of EXT_CMD_ID_BSSGROUP_CTRL = 0x37 */

/*************************************************/
/* EXT_CMD_ID_VOW_FEATURE_CTRL = 0x38 */
/*************************************************/
typedef struct GNU_PACKED _EXT_CMD_VOW_FEATURE_CTRL_T
{
    /* DW#0 */
    UINT16     u2IfApplyBss_0_to_16_CtrlFlag;                  /* BITWISE */
#ifdef RT_BIG_ENDIAN 
	UINT16	   u2IfApplyEnbwCtrlFlag:1;
	UINT16	   u2IfApplyEnbwrefillFlag:1;
	UINT16	   u2IfApplyAirTimeFairnessFlag:1;
	UINT16	   u2IfApplyEnTxopNoChangeBssFlag:1;
	UINT16     u2Reserve_b26_to_b27Flag:2;
	UINT16     u2IfApplyWeightedAirTimeFairnessFlag:1;
	UINT16     u2Reserve_b22_to_b24Flag:3;
	UINT16	   u2IfApplyDbdc0SearchRuleFlag:1;
	UINT16	   u2IfApplyDbdc1SearchRuleFlag:1;
	UINT16	   u2Reserve_b19Flag:1;
	UINT16	   u2Reserve_b17_to_b18Flag:2;
	UINT16	   u2IfApplyRefillPerildFlag:1;
#else
    UINT16     u2IfApplyRefillPerildFlag:1;
    UINT16     u2Reserve_b17_to_b18Flag:2;
    UINT16     u2Reserve_b19Flag:1;
    UINT16     u2IfApplyDbdc1SearchRuleFlag:1;
    UINT16     u2IfApplyDbdc0SearchRuleFlag:1;
	UINT16     u2Reserve_b22_to_b24Flag:3;
	UINT16     u2IfApplyWeightedAirTimeFairnessFlag:1;
    UINT16     u2Reserve_b26_to_b27Flag:2;
    UINT16     u2IfApplyEnTxopNoChangeBssFlag:1;
    UINT16     u2IfApplyAirTimeFairnessFlag:1;
    UINT16     u2IfApplyEnbwrefillFlag:1;
    UINT16     u2IfApplyEnbwCtrlFlag:1;
#endif
    /* DW#1 */
    UINT16     u2IfApplyBssCheckTimeToken_0_to_16_CtrlFlag;    /* BITWISE */
    UINT16     u2Resreve1Flag;

    /* DW#2 */
    UINT16     u2IfApplyBssCheckLengthToken_0_to_16_CtrlFlag;  /* BITWISE */
    UINT16     u2Resreve2Flag;

    /* DW#3, 4 */
    UINT32     u2ResreveBackupFlag[2];

    /* DW#5 */
    UINT16     u2Bss_0_to_16_CtrlValue;                        /* BITWISE */
#ifdef RT_BIG_ENDIAN
		UINT16	   u2EnbwCtrlValue:1;
		UINT16	   u2EnbwrefillValue:1;
		UINT16	   u2AirTimeFairnessValue:1;
		UINT16	   u2EnTxopNoChangeBssValue:1;
		UINT16	   u2Reserve_b26_to_b27Value:2;
		UINT16	   u2WeightedAirTimeFairnessValue:1;
		UINT16	   u2Reserve_b22_to_b24Value:3;
		UINT16	   u2Dbdc0SearchRuleValue:1;
		UINT16	   u2Dbdc1SearchRuleValue:1;
		UINT16	   u2Reserve_b19Value:1;
		UINT16	   u2RefillPerildValue:3;
#else

    UINT16     u2RefillPerildValue:3;
    UINT16     u2Reserve_b19Value:1;
    UINT16     u2Dbdc1SearchRuleValue:1;
    UINT16     u2Dbdc0SearchRuleValue:1;
	UINT16     u2Reserve_b22_to_b24Value:3;
	UINT16     u2WeightedAirTimeFairnessValue:1;
    UINT16     u2Reserve_b26_to_b27Value:2;
    UINT16     u2EnTxopNoChangeBssValue:1;
    UINT16     u2AirTimeFairnessValue:1;
    UINT16     u2EnbwrefillValue:1;
    UINT16     u2EnbwCtrlValue:1;
#endif
    /* DW#6 */
    UINT16     u2BssCheckTimeToken_0_to_16_CtrlValue;    /* BITWISE */
    UINT16     u2Resreve1Value;

    /* DW#7 */
    UINT16     u2BssCheckLengthToken_0_to_16_CtrlValue;  /* BITWISE */
    UINT16     u2Resreve2Value;

    /* DW#8,9 */
    UINT32     u2ResreveBackupValue[2];

} EXT_CMD_VOW_FEATURE_CTRL_T, *P_EXT_CMD_VOW_FEATURE_CTRL_T;

typedef enum _ENUM_UMAC_VOW_REFILL_PERIOD_T
{
    ENUM_UMAC_VOW_REFILL_IDX_0_1_US = 0,
    ENUM_UMAC_VOW_REFILL_IDX_1_2_US = 1,
    ENUM_UMAC_VOW_REFILL_IDX_2_4_US = 2,
    ENUM_UMAC_VOW_REFILL_IDX_3_8_US = 3,
    ENUM_UMAC_VOW_REFILL_IDX_4_16_US = 4,
    ENUM_UMAC_VOW_REFILL_IDX_5_32_US = 5,
    ENUM_UMAC_VOW_REFILL_IDX_6_64_US = 6,
    ENUM_UMAC_VOW_REFILL_IDX_7_128_US = 7,
    ENUM_UMAC_VOW_REFILL_IDX_TOTAL_NUM = 8,
} ENUM_UMAC_VOW_REFILL_PERIOD_T, *P_ENUM_UMAC_VOW_REFILL_PERIOD_T;
/* end of EXT_CMD_ID_VOW_FEATURE_CTRL = 0x38 */

/*************************************************/
/*       EXT_CMD_ID_RX_AIRTIME_CTRL = 0x4a       */
/*************************************************/
/* MIB IFS related data type */
typedef struct GNU_PACKED _RX_AT_IFS_CFG_T {
    UINT8 ucAC0Ifs;
    UINT8 ucAC1Ifs;
    UINT8 ucAC2Ifs;
    UINT8 ucAC3Ifs;
}  RX_AT_IFS_CFG_T, *P_RX_AT_IFS_CFG_T;


typedef enum _ENUM_RX_AT_AC_Q_MASK_T {
    ENUM_RX_AT_AC_Q0_MASK_T = BIT(0),
    ENUM_RX_AT_AC_Q1_MASK_T = BIT(1),
    ENUM_RX_AT_AC_Q2_MASK_T = BIT(2),
    ENUM_RX_AT_AC_Q3_MASK_T = BIT(3),
    ENUM_RX_AT_AC_ALL_MASK_T = (BIT(3)|BIT(2)|BIT(1)|BIT(0)),
} ENUM_RX_AT_AC_Q_MASK_T, *P_ENUM_RX_AT_AC_Q_MASK_T;


typedef enum _ENUM_RX_AT_WMM_GROUP_IDX_T {

    ENUM_RX_AT_WMM_GROUP_0 = 0,             /* MIBTIME1 */
    ENUM_RX_AT_WMM_GROUP_1 = 1,             /* MIBTIME2 */
    ENUM_RX_AT_WMM_GROUP_2 = 2,             /* MIBTIME3 */
    ENUM_RX_AT_WMM_GROUP_3 = 3,             /* MIBTIME4 */
    ENUM_RX_AT_WMM_GROUP_PEPEATER = 4,      /* MIBTIME7 */
    ENUM_RX_AT_WMM_GROUP_STA = 5,           /* MIBTIME8 */
    ENUM_RX_AT_NON_QOS = 6,                 /* MIBTIME0 */
    ENUM_RX_AT_OBSS = 7,                    /* MIBTIME0 */

} ENUM_RX_AT_WMM_GROUP_IDX_T, *P_ENUM_RX_AT_WMM_GROUP_IDX_T;

/* RX AT Backoff related data type */

typedef struct GNU_PACKED _RX_AT_BACKOFF_CFG_T {
    UINT16 u2AC0Backoff;
    UINT16 u2AC1Backoff;
    UINT16 u2AC2Backoff;
    UINT16 u2AC3Backoff;
}  RX_AT_BACKOFF_CFG_T, *P_RX_AT_BACKOFF_CFG_T;


/* Compensate mode enum definition */
typedef enum _ENUM_RX_AT_SW_COMPENSATE_MODE_T {
    ENUM_RX_AT_SW_COMPENSATE_OBSS = 1,
    ENUM_RX_AT_SW_COMPENSATE_NON_WIFI = 2,
} ENUM_RX_AT_SW_COMPENSATE_MODE_T, *P_ENUM_RX_AT_SW_COMPENSATE_MODE_T;



/* ENUM list for u4CtrlFieldID */
typedef enum _ENUM_RX_AT_CTRL_FIELD_T {
    ENUM_RX_AT_FEATURE_CTRL = 1,
    ENUM_RX_AT_BITWISE_CTRL = 2,
    ENUM_RX_AT_TIMER_VALUE_CTRL = 3,
    EMUM_RX_AT_REPORT_CTRL = 4,
} ENUM_RX_AT_CTRL_FIELD_T, *P_ENUM_RX_AT_CTRL_FIELD_T;

/* ENUM list for (u4CtrlSubFieldID == ENUM_RX_AT_FEATURE_CTRL) */
typedef enum _ENUM_RX_AT_FEATURE_SUB_FIELD_T {
    ENUM_RX_AT_FEATURE_SUB_TYPE_AIRTIME_EN = 1,
    ENUM_RX_AT_FEATURE_SUB_TYPE_MIBTIME_EN = 2,
} ENUM_RX_AT_FEATURE_SUB_FIELD_T, *P_ENUM_RX_AT_FEATURE_SUB_FIELD_T;


/* ENUM list for (u4CtrlSubFieldID == ENUM_RX_AT_FEATURE_CTRL) */
typedef enum _ENUM_RX_AT_BITWISE_SUB_FIELD_T {
    ENUM_RX_AT_BITWISE_SUB_TYPE_AIRTIME_CLR = 1,
    ENUM_RX_AT_BITWISE_SUB_TYPE_MIBTIME_CLR = 2,
    ENUM_RX_AT_BITWISE_SUB_TYPE_STA_WMM_CTRL = 3,
    ENUM_RX_AT_BITWISE_SUB_TYPE_MBSS_WMM_CTRL = 4,
} ENUM_RX_AT_BITWISE_SUB_FIELD_T, *_ENUM_RX_AT_BITWISE_SUB_FIELD_T;

/* ENUM list for (u4CtrlSubFieldID == ENUM_RX_AT_TIMER_VALUE_CTRL) */
typedef enum _ENUM_RX_AT_TIME_VALUE_SUB_FIELD_T {
    ENUM_RX_AT_TIME_VALUE_SUB_TYPE_ED_OFFSET_CTRL = 1,
    ENUM_RX_AT_TIME_VALUE_SUB_TYPE_SW_TIMER = 2,
    ENUM_RX_AT_TIME_VALUE_SUB_TYPE_BACKOFF_TIMER = 3,
    ENUM_RX_AT_TIME_VALUE_SUB_TYPE_IFS_TIMER = 4,
} ENUM_RX_AT_TIME_VALUE_SUB_FIELD_T, *P_ENUM_RX_AT_TIME_VALUE_SUB_FIELD_T;


/* ENUM list for (u4CtrlSubFieldID == EMUM_RX_AT_REPORT_CTRL) */
typedef enum _ENUM_RX_AT_TIME_REPORT_SUB_FIELD_T {
    ENUM_RX_AT_REPORT_SUB_TYPE_RX_NONWIFI_TIME = 1,
    ENUM_RX_AT_REPORT_SUB_TYPE_RX_OBSS_TIME = 2,
    ENUM_RX_AT_REPORT_SUB_TYPE_MIB_OBSS_TIME = 3,
    ENUM_RX_AT_REPORT_SUB_TYPE_PER_STA_RX_TIME = 4,
} ENUM_RX_AT_TIME_REPORT_SUB_FIELD_T, *P_ENUM_RX_AT_TIME_REPORT_SUB_FIELD_T;

typedef struct GNU_PACKED _RX_AT_FEATURE_SUB_FIELD_CTRL_T
{
    BOOLEAN                 fgRxAirTimeEn;
    BOOLEAN                 fgRxMibTimeEn;
    UINT8                  ucReserve[2];
    UINT32                 u4ReserveDW[2];
} RX_AT_FEATURE_SUB_FIELD_CTRL_T, *P_RX_AT_FEATURE_SUB_FIELD_CTRL_T;

typedef struct GNU_PACKED _RX_AT_BITWISE_SUB_FIELD_CTRL_T
{
    /* DW#0 */
    BOOLEAN                 fgRxAirTimeClrEn;
    BOOLEAN                 fgRxMibTimeClrEn;
    UINT8                  ucReserve[2];

    /* DW#1 */
    UINT8                  ucOwnMacID;
    BOOLEAN                 fgtoApplyWm00to03MibCfg;
    UINT8                  ucReserve1[2];

    /* DW#2 */
    UINT8                  ucMbssGroup;
    UINT8                  ucWmmGroup;
    UINT8                  ucReserve2[2];

    /* DW#3,4 */
    UINT32                 u4ReserveDW[2];
} RX_AT_BITWISE_SUB_FIELD_CTRL_T, *P_RX_AT_BITWISE_SUB_FIELD_CTRL_T;

typedef enum _ENUM_DBDC_BN_T {
    ENUM_BAND_0,
    ENUM_BAND_1,
    ENUM_BAND_NUM,
    ENUM_BAND_ALL
} ENUM_DBDC_BN_T, *P_ENUM_DBDC_BN_T;

typedef struct GNU_PACKED _RX_AT_TIMER_VALUE_SUB_FIELD_CTRL_T
{
    /* DW#0 */
    UINT8                          	ucEdOffsetValue;
    UINT8                          	ucReserve0[3];

    /* DW#1 */
    UINT8 						   	rCompensateMode;
    UINT8                  		   	rRxBand;
    UINT8                          	ucSwCompensateTimeValue;
    UINT8                          	ucReserve1;

    /* D2#2/3/4 */
    RX_AT_BACKOFF_CFG_T             rRxATBackOffCfg;
    UINT8      						rRxATBackoffWmmGroupIdx;
    UINT8          					rRxAtBackoffAcQMask;
    UINT8                          	ucReserve2[2];

    /* DW#5/6 */
    RX_AT_IFS_CFG_T                 rRxATIfsCfg;
    UINT8      						rRxATIfsWmmGroupIdx;
    UINT8							rRxAtIfsAcQMask;
    UINT8                          	ucReserve3[2];

    UINT32                         	u4ReserveDW[2];
} RX_AT_TIMER_VALUE_SUB_FIELD_CTRL_T, *P_RX_AT_TIMER_VALUE_SUB_FIELD_CTRL_T;

typedef struct GNU_PACKED _RX_AT_REPORT_SUB_FIELD_CTRL_T
{
    /* DW# 0/1 */
    UINT32         	u4RxNonWiFiBandTimer;
    UINT8  			ucRxNonWiFiBandIdx;
    UINT8          	ucReserve0[3];

    /* DW# 2/3 */
    UINT32         	u4RxObssBandTimer;
    UINT8  			ucRxObssBandIdx;
    UINT8          	ucReserve1[3];

    /* DW# 4/5 */
    UINT32         	u4RxMibObssBandTimer;
    UINT8  			ucRxMibObssBandIdx;
    UINT8          	ucReserve2[3];

    /* DW# 6/7/8/9/10 */
    UINT32         u4StaAcRxTimer[4];
    UINT8          ucStaID;
    UINT8          ucReserve3[3];

} RX_AT_REPORT_SUB_FIELD_CTRL_T, *P_RX_AT_REPORT_SUB_FIELD_CTRL_T;


typedef union GNU_PACKED _RX_AT_GENERAL_CTRL_FIELD_T {
    RX_AT_FEATURE_SUB_FIELD_CTRL_T          rRxAtFeatureSubCtrl;
    RX_AT_BITWISE_SUB_FIELD_CTRL_T          rRxAtBitWiseSubCtrl;
    RX_AT_TIMER_VALUE_SUB_FIELD_CTRL_T      rRxAtTimeValueSubCtrl;
    RX_AT_REPORT_SUB_FIELD_CTRL_T           rRxAtReportSubCtrl;
} RX_AT_GENERAL_CTRL_FIELD_T, *P_RX_AT_GENERAL_CTRL_FIELD_T;

typedef struct GNU_PACKED _EXT_CMD_RX_AT_CTRL_T
{
    UINT16                     u4CtrlFieldID;
    UINT16                     u4CtrlSubFieldID;
    UINT32                     u4CtrlSetStatus;
    UINT32                     u4CtrlGetStatus;
    UINT8                      ucReserve[4];
    UINT32                     u4ReserveDW[2];

    RX_AT_GENERAL_CTRL_FIELD_T  rRxAtGeneralCtrl;

} EXT_CMD_RX_AT_CTRL_T, *P_EXT_CMD_RX_AT_CTRL_T;

/* end of EXT_CMD_ID_RX_AIRTIME_CTRL = 0x4a */

/*************************************************/
/*      EXT_CMD_ID_AT_PROC_MODULE = 0x4b         */
/*************************************************/
typedef struct GNU_PACKED _AT_ESTIMATE_SUB_FIELD_CTRL_T
{
    /* DW#0 */
    BOOLEAN         fgAtEstimateOnOff;
    UINT8          ucReserve;
    UINT16         u2AtEstMonitorPeriod;

    /* DW#1, 2~9 */
    UINT32         u4GroupRatioBitMask;
    UINT16         u2GroupMaxRatioValue[16];
    UINT16         u2GroupMinRatioValue[16];

    /* DW#10 */
    UINT8          ucGrouptoSelectBand;
    UINT8          ucBandSelectedfromGroup;
    UINT8          ucReserve1[2];

} AT_ESTIMATE_SUB_FIELD_CTRL_T, *P_AT_ESTIMATE_SUB_FIELD_CTRL_T;

typedef struct GNU_PACKED _AT_BAD_NODE_SUB_FIELD_CTRL_T
{
    /* DW#0 */
    BOOLEAN         fgAtBadNodeOnOff;
    UINT8          ucReserve;
    UINT16         u2AtBadNodeMonitorPeriod;

    /* DW#1 */
    UINT8          ucFallbackThreshold;
    UINT8         ucTxPERThreshold;
    UINT8          ucReserve1[2];

} AT_BAD_NODE_SUB_FIELD_CTRL_T, *P_AT_BAD_NODE_SUB_FIELD_CTRL_T;


typedef union _AT_PROC_GENERAL_CTRL_FIELD_T {
    AT_ESTIMATE_SUB_FIELD_CTRL_T          rAtEstimateSubCtrl;
    AT_BAD_NODE_SUB_FIELD_CTRL_T          rAtBadNodeSubCtrl;
} AT_PROC_GENERAL_CTRL_FIELD_T, *P_AT_PROC_GENERAL_CTRL_FIELD_T;

typedef struct GNU_PACKED _EXT_CMD_AT_PROC_MODULE_CTRL_T
{
    UINT16                      u4CtrlFieldID;
    UINT16                      u4CtrlSubFieldID;
    UINT32                      u4CtrlSetStatus;
    UINT32                      u4CtrlGetStatus;
    UINT8                       ucReserve[4];
    UINT32                      u4ReserveDW[2];

    AT_PROC_GENERAL_CTRL_FIELD_T rAtProcGeneralCtrl;
} EXT_CMD_AT_PROC_MODULE_CTRL_T, *P_EXT_CMD_AT_PROC_MODULE_CTRL_T;

typedef enum _ENUM_AT_ESTIMATE_SUB_FIELD_T {
    ENUM_AT_PROC_EST_FEATURE_CTRL = 1,
    ENUM_AT_PROC_EST_MONITOR_PERIOD_CTRL = 2,
    ENUM_AT_PROC_EST_GROUP_RATIO_CTRL = 3,
    ENUM_AT_PROC_EST_GROUP_TO_BAND_MAPPING = 4,
} ENUM_AT_ESTIMATE_SUB_FIELD_T, *P_ENUM_AT_ESTIMATE_SUB_FIELD_T;


typedef enum _ENUM_AT_BAD_NODE_SUB_FIELD_T {
    ENUM_AT_PROC_BAD_NODE_FEATURE_CTRL = 1,
    ENUM_AT_PROC_BAD_NODE_MONITOR_PERIOD_CTRL = 2,
    ENUM_AT_PROC_BAD_NODE_FALLBACK_THRESHOLD = 3,
    ENUM_AT_PROC_BAD_NODE_PER_THRESHOLD = 4,
} ENUM_AT_BAD_NODE_SUB_FIELD_T, *P_ENUM_AT_BAD_NODE_SUB_FIELD_T;


/* ENUM list for u4CtrlFieldID in AT PROCESS control field */
typedef enum _ENUM_AT_RPOCESS_FIELD_T {
    ENUM_AT_RPOCESS_ESTIMATE_MODULE_CTRL = 1,
    ENUM_AT_RPOCESS_BAD_NODE_MODULE_CTRL = 2,
} ENUM_AT_RPOCESS_FIELD_T, *P_ENUM_AT_RPOCESS_FIELD_T;


/* end of EXT_CMD_ID_AT_PROC_MODULE = 0x4b */

#endif /* VOW_SUPPORT */


#define SET_RX_MAX_PKT_LEN(x)	((x) << 2)
typedef struct _EXT_CMD_RX_MAX_PKT_LEN_T {
	UINT16 u2Tag; // EXT_CMD_TAG_RXMAXLEN
	UINT16 u2RxMaxPktLength; // CR unit is DWORD(4B)
} EXT_CMD_RX_MAX_PKT_LEN_T, *P_EXT_CMD_RX_MAX_PKT_LEN_T;


typedef struct GNU_PACKED _CMD_SLOT_TIME_SET_T
{
    UINT8   ucSlotTime;
    UINT8   ucSifs;
    UINT8   ucRifs;
    UINT8 ucOldEifs;		//occupied for backward compatible
    UINT16  u2Eifs;
    UINT8 ucBandNum;
    UINT8 aucReserved[5];
}CMD_SLOT_TIME_SET_T,*P_CMD_SLOT_TIME_SET_T;

typedef struct GNU_PACKED _CMD_POWER_PWERCENTAGE_LEVEL_SET_T
{
    INT8   cPowerDropLevel;
    UINT8  ucBand;
    UINT8  aucReserved[10];
}CMD_POWER_PWERCENTAGE_LEVEL_SET_T,*P_CMD_POWER_PWERCENTAGE_LEVEL_SET_T;

#define TX_STREAM	0x0
#define RX_STREAM	0x1
#define SET_TR_STREAM_NUM(x, y)	(((x)<<16)|(y))

enum EXT_CMD_ATE_CFG_ONOFF_TYPE {
    EXT_CFG_ONOFF_TSSI = 0x0,
    EXT_CFG_ONOFF_DPD = 0x1,
    EXT_CFG_ONOFF_RATE_POWER_OFFSET = 0x2,
    EXT_CFG_ONOFF_TEMP_COMP = 0x3,
    EXT_CFG_ONOFF_THERMAL_SENSOR = 0x4,
    EXT_CFG_ONOFF_TXPOWER_CTRL = 0x5,
    EXT_CFG_ONOFF_SINGLE_SKU = 0x6,
    EXT_CFG_ONOFF_POWER_PERCENTAGE = 0x7,
    EXT_CFG_ONOFF_BF_BACKOFF = 0x8,
};

#ifdef CONFIG_HW_HAL_OFFLOAD
enum EXT_CMD_ATE_TRX_SET_IDX {
	EXT_ATE_SET_RESERV = 0x0,
	EXT_ATE_SET_TRX = 0x1,
	EXT_ATE_SET_RX_PATH = 0x2,
	EXT_ATE_SET_RX_FILTER = 0x3,
	EXT_ATE_SET_TX_STREAM = 0x4,
	EXT_ATE_SET_TSSI = 0x5,
	EXT_ATE_SET_DPD = 0x6,
	EXT_ATE_SET_RATE_POWER_OFFSET = 0X7,
	EXT_ATE_SET_THERNAL_COMPENSATION = 0X8,
	EXT_ATE_SET_RX_FILTER_PKT_LEN = 0x09,
	EXT_ATE_SET_FREQ_OFFSET = 0x0A,
	EXT_ATE_GET_FREQ_OFFSET = 0x0B,
	EXT_ATE_GET_TSSI = 0xC,
	EXT_ATE_GET_DPD = 0xD,
	EXT_ATE_GET_THERNAL_COMPENSATION = 0XE,
	EXT_ATE_SET_RXV_INDEX = 0x0F,
	EXT_ATE_SET_FAGC_PATH = 0x10,
	EXT_ATE_SET_PHY_COUNT = 0x11,
	EXT_ATE_SET_ANTENNA_PORT = 0x12,
	EXT_ATE_SET_SLOT_TIME = 0x13,
	EXT_ATE_CFG_THERMAL_ONOFF = 0x14,
	EXT_ATE_SET_TX_POWER_CONTROL_ALL_RF = 0x15,
	EXT_ATE_GET_RATE_POWER_OFFSET = 0x16,
	EXT_ATE_SET_SINGLE_SKU = 0x18,
	EXT_ATE_SET_POWER_PERCENTAGE = 0x19,
	EXT_ATE_SET_BF_BACKOFF = 0x1a,
	EXT_ATE_SET_POWER_PERCENTAGE_LEVEL = 0x1b,
};

typedef struct GNU_PACKED _EXT_EVENT_ATE_TEST_MODE_T {
	UINT8 ucAteIdx;
	UINT8 aucReserved[3];
	UINT8 aucAteResult[0];
} EXT_EVENT_ATE_TEST_MODE_T, *P_EXT_EVENT_ATE_TEST_MODE_T;

typedef struct _GET_FREQ_OFFSET_T {
	UINT32 u4FreqOffset;
} GET_FREQ_OFFSET_T, *P_GET_FREQ_OFFSET_T;

typedef struct _GET_TSSI_STATUS_T {
	UINT8 ucEnable;
	UINT8 ucBand;
	UINT8 aucReserved[2];
} GET_TSSI_STATUS_T, *P_GET_TSSI_STATUS_T;

typedef struct _GET_DPD_STATUS_T {
	UINT8 ucEnable;
	UINT8 ucBand;
	UINT8 aucReserved[2];
} GET_DPD_STATUS_T, *P_GET_DPD_STATUS_T;

typedef struct _GET_THERMO_COMP_STATUS_T {
	UINT8 ucEnable;
	UINT8 aucReserved[3];
} GET_THERMO_COMP_STATUS_T, *P_GET_THERMO_COMP_STATUS_T;

typedef struct GNU_PACKED _EVENT_EXT_GET_FREQOFFSET_T {
	UINT8  ucAteTestModeEn;
	UINT8  ucAteIdx;
	UINT8  aucReserved[2];
	UINT32 u4FreqOffset;
} EVENT_EXT_GET_FREQOFFSET_T;

typedef struct _ATE_TEST_SET_TX_STREAM_T
{
	UINT8  ucStreamNum;
	UINT8  ucBand;
	UINT8  aucReserved[2];
} ATE_TEST_SET_TX_STREAM_T, *P_ATE_TEST_SET_TX_STREAM_T;

typedef struct _ATE_TEST_SET_RX_FILTER_T
{
	UINT8  ucPromiscuousMode;
	UINT8  ucReportEn;
	UINT8  ucBand;
	UINT8  ucReserved;
	UINT32 u4FilterMask;
	UINT8  aucReserved[4];
} ATE_TEST_SET_RX_FILTER_T, *P_ATE_TEST_SET_RX_FILTER_T;

typedef struct _ATE_TEST_SET_RX_PATH_T
{
	UINT8  ucType;
	UINT8  ucBand;
	UINT8  aucReserved[2];
} ATE_TEST_SET_RX_PATH_T, *P_ATE_TEST_SET_RX_PATH_T;

typedef struct _ATE_TEST_SET_TRX_T
{
	UINT8  ucType;
	UINT8  ucEnable;
	UINT8  ucBand;
	UINT8  ucReserved;
} ATE_TEST_SET_TRX_T, *P_ATE_TEST_SET_TRX_T;

typedef struct _RF_TEST_ON_OFF_SETTING_T{
	UINT8 ucEnable;
	UINT8 ucBand;
	UINT8 aucReserved[2];
}RF_TEST_ON_OFF_SETTING_T, *P_RF_TEST_ON_OFF_SETTING_T;

typedef struct _CFG_RX_FILTER_PKT_LEN_T
{
	UINT8 ucEnable;
	UINT8 ucBand;
	UINT8 aucReserved[2];
	UINT32 u4RxPktLen;
}RX_FILTER_PKT_LEN_T, *P_RX_FILTER_PKT_LEN_T;

typedef struct _CFG_PHY_SETTING_RXV_IDX_T
{
	UINT8 ucValue1;
	UINT8 ucValue2;
	UINT8 ucDbdcIdx;
	UINT8 ucReserved;
}CFG_PHY_SETTING_RXV_IDX_T, *P_CFG_PHY_SETTING_RXV_IDX_T;

typedef struct _CFG_PHY_SETTING_RSSI_PATH_T
{
	UINT8 ucValue;
	UINT8 ucDbdcIdx;
	UINT8 aucReserved[2];
} CFG_PHY_SETTING_RSSI_PATH_T, *P_CFG_PHY_SETTING_RSSI_PATH_T;

typedef struct _CFG_RF_ANT_PORT_SETTING_T
{
    UINT8  ucRfModeMask;
    UINT8  ucRfPortMask;
    UINT8  ucAntPortMask;
    UINT8  aucReserved[1];
} CFG_RF_ANT_PORT_SETTING_T, *P_CFG_RF_ANT_PORT_SETTING_T;


typedef struct GNU_PACKED _EXT_CMD_ATE_TEST_MODE_T
{
	UINT8  ucAteTestModeEn;
	UINT8  ucAteIdx;
	UINT8  aucReserved[2];
	union{
		UINT32 u4Data;
		ATE_TEST_SET_TRX_T rAteSetTrx;
		ATE_TEST_SET_RX_PATH_T rAteSetRxPath;
		ATE_TEST_SET_RX_FILTER_T rAteSetRxFilter;
		ATE_TEST_SET_TX_STREAM_T rAteSetTxStream;
		RF_TEST_ON_OFF_SETTING_T rCfgOnOff;
		RX_FILTER_PKT_LEN_T rRxFilterPktLen;
		CFG_PHY_SETTING_RXV_IDX_T rSetRxvIdx;
		CFG_PHY_SETTING_RSSI_PATH_T rSetFagcRssiPath;
		RF_TEST_ON_OFF_SETTING_T rPhyStatusCnt;
		CFG_RF_ANT_PORT_SETTING_T rCfgRfAntPortSetting;
		CMD_SLOT_TIME_SET_T rSlotTimeSet;
        CMD_POWER_PWERCENTAGE_LEVEL_SET_T rPowerLevelSet;
	}Data;
} EXT_CMD_ATE_TEST_MODE_T, *P_EXT_CMD_ATE_TEST_MODE_T;
#endif /* CONFIG_HW_HAL_OFFLOAD */

typedef struct GNU_PACKED _CMD_MCU_CLK_SWITCH_DISABLE_T
{
    UINT8 disable;
    UINT8 aucReserved[3];
} CMD_MCU_CLK_SWITCH_DISABLE_T, *P_CMD_MCU_CLK_SWITCH_DISABLE_T;


typedef struct GNU_PACKED _EXT_CMD_SNIFFER_MODE_T
{
    UINT8  ucSnifferEn;
    UINT8  ucDbdcIdx;
    UINT8  aucReserved[6];
} EXT_CMD_SNIFFER_MODE_T, *P_EXT_CMD_SNIFFER_MODE_T;



typedef struct GNU_PACKED _EXT_CMD_TR_STREAM_T {
	UINT16 u2Tag; // EXT_CMD_TAG_TR_STREAM
	UINT16 u2TRStreamNum; // [31..16] Tx:1/Rx:0, [15..0] s1:0/s2:1/s3:2
} EXT_CMD_TR_STREAM_T, *P_EXT_CMD_TR_STREAM_T;

typedef struct GNU_PACKED _EXT_CMD_BA_CONTROL_T {
	UINT16 u2Tag; // EXT_CMD_TAG_UPDATE_BA
	BOOLEAN bIsAdd; // BOOLEAN in host is 1Byte
	UINT8 ucWcid;
	UINT8 ucTid;
	UINT8 ucBaWinSize;
	UINT8 ucBaSessionType;
	UINT8 aucPeerAddr[MAC_ADDR_LEN];
	UINT16 u8Sn;
	UINT8 aucRsv[1];
} EXT_CMD_CONTROL_BA_T, *P_EXT_CMD_CONTROL_BA_T;

#define MT_UPLOAD_FW_UNIT (1024 * 4)

#define CMD_EDCA_AIFS_BIT 		1 << 0
#define CMD_EDCA_WIN_MIN_BIT 	1<<1
#define CMD_EDCA_WIN_MAX_BIT 	1 <<2
#define CMD_EDCA_TXOP_BIT 		1<<3
#define CMD_EDCA_ALL_BITS 		CMD_EDCA_AIFS_BIT | CMD_EDCA_WIN_MIN_BIT | CMD_EDCA_WIN_MAX_BIT |CMD_EDCA_TXOP_BIT

#define CMD_EDCA_AC_MAX 4

#if defined(MT7615) || defined(MT7622)
typedef enum _ENUM_HW_TX_QUE_T {
    TX_Q_AC0 = 0,
    TX_Q_AC1,
    TX_Q_AC2,
    TX_Q_AC3,
    TX_Q_AC10,
    TX_Q_AC11,
    TX_Q_AC12,
    TX_Q_AC13,
    TX_Q_AC20,
    TX_Q_AC21,
    TX_Q_AC22,
    TX_Q_AC23,
    TX_Q_AC30,
    TX_Q_AC31,
    TX_Q_AC32,
    TX_Q_AC33,
    TX_Q_ALTX0,
    TX_Q_BMC,
    TX_Q_BCN,
    TX_Q_PSMP,
    TX_Q_ALTX1,
    TX_Q_BMC1,
    TX_Q_BCN1,
    TX_Q_PSMP1,
    HW_TX_QUE_NUM
} ENUM_HW_TX_QUE_T;
#else
typedef enum _ENUM_HW_TX_QUE_T {
    TX_Q_AC0 = 0,
    TX_Q_AC1,
    TX_Q_AC2,
    TX_Q_AC3,
    TX_Q_AC4,
    TX_Q_AC5,
    TX_Q_AC6,
    TX_Q_BCN,
    TX_Q_BMC,
    TX_Q_AC10,
    TX_Q_AC11,
    TX_Q_AC12,
    TX_Q_AC13,
    TX_Q_AC14,
    HW_TX_QUE_NUM
} ENUM_HW_TX_QUE_T;
#endif /* defined(MT7615) || defined(MT7622) */


typedef struct _TX_AC_PARAM_T
{
	UINT8    ucAcNum;
	UINT8    ucVaildBit;
	UINT8	ucAifs;
	UINT8  	ucWinMin;
	UINT16  u2WinMax;
	UINT16  u2Txop;

}TX_AC_PARAM_T,*P_TX_AC_PARAM_T;


enum {
	EDCA_ACT_SET,
	EDCA_ACT_GET,
	EDCA_ACT_MAX
};

typedef struct GNU_PACKED _CMD_EDCA_SET_T {

	UINT8		 	 ucTotalNum;
	UINT8			 ucAction;
	UINT8			 aucReserve[2];
	TX_AC_PARAM_T rAcParam[HW_TX_QUE_NUM];

} CMD_EDCA_SET_T,CMD_EDCA_CTRL_T, *P_CMD_EDCA_CTRL_T, MT_EDCA_CTRL_T;

typedef struct GNU_PACKED _WIFI_EVENT_MUAR_T
{
    UINT8      ucEntryCnt;
    UINT8      ucAccessMode;
    UINT8      aucReserved[2];
    UINT32     u4Status;
} WIFI_EVENT_MUAR_T, *P_WIFI_EVENT_MUAR_T;

typedef struct GNU_PACKED _WIFI_EVENT_MUAR_MULTI_RW_T
{
    UINT8      ucEntryIdx;
    UINT8      ucReserved;
    UINT8      aucMacAddr[6];

} WIFI_EVENT_MUAR_MULTI_RW_T, *P_WIFI_EVENT_MUAR_MULTI_RW_T;

typedef struct GNU_PACKED _EXT_CMD_MUAR_MULTI_ENTRY_T
{
    UINT8 ucMuarIdx;
    UINT8 ucBssid;
    UINT8 aucMacAddr[6];
} EXT_CMD_MUAR_MULTI_ENTRY_T, *P_EXT_CMD_MUAR_MULTI_ENTRY_T;

typedef enum _MUAR_MODE
{
    MUAR_NORMAL = 0,
    MUAR_REPEATER,
    MUAR_HASH,
    MUAR_MAX
} MUAR_MODE;

typedef enum _MUAR_ACCESS_MODE
{
    MUAR_READ = 0,
    MUAR_WRITE,
} _MUAR_ACCESS_MODE;

typedef struct GNU_PACKED _EXT_CMD_MUAR_T
{
    UINT8 ucMuarModeSel;
    UINT8 ucForceClear;
    UINT8 ausClearBitmap[8];
    UINT8 ucEntryCnt;
    UINT8 ucAccessMode; /* 0:read, 1:write */
} EXT_CMD_MUAR_T, *P_EXT_CMD_MUAR_T;

typedef struct GNU_PACKED _EXT_CMD_TRGR_PRETBTT_INT_EVENT_T {
    UINT8 ucHwBssidIdx;
    UINT8 ucExtBssidIdx;
    UINT8 ucEnable;
    UINT8 aucReserved1[1];

    UINT16 u2BcnPeriod;
    UINT8 aucReserved2[2];

} CMD_TRGR_PRETBTT_INT_EVENT_T, *P_CMD_TRGR_PRETBTT_INT_EVENT_T;

typedef struct GNU_PACKED _EXT_CMD_BCN_OFFLOAD_T {
    UINT8 ucOwnMacIdx;
    UINT8 ucEnable;
    UINT8 ucWlanIdx;
    UINT8 ucBandIdx;//0: band 0, 1: band 1

    UINT8 ucPktType;//0: Bcn, 1: Tim Frame.
    BOOLEAN fgBcnOffloadSupport;
    UINT16  u2CsaIePos; // CSA IE position

    UINT16 u2PktLength;
    UINT16 u2TimIePos;//Tim IE position in pkt.

    UINT8 acPktContent[512];//whole pkt template length which include TXD, max shall not exceed 512 bytes.
    UINT8 ucCsaCount; // count down value of CSA IE
    UINT8 aucReserved[3];
} CMD_BCN_OFFLOAD_T, *P_CMD_BCN_OFFLOAD_T;

enum _TMR_CTRL_TYPE_T
{
    SET_TMR_ENABLE  = 0x00,
    TMR_CALIBRATION = 0x01
};

typedef struct GNU_PACKED _CMD_TMR_CTRL_T
{
    UINT8 ucTmrCtrlType;
    UINT8 ucTmrVer;
    UINT8 aucReserved[2];
    UINT8 pTmrCtrlPayload[];
} CMD_TMR_CTRL_T, *P_CMD_TMR_CTRL_T;

typedef struct GNU_PACKED _TMR_CTRL_SET_TMR_EN_T
{
    UINT8 ucEnable;
    UINT8 ucRole;
    UINT8 ucReserved;
    UINT8 ucDbdcIdx;
    UINT8 aucType_Subtype[4];
} TMR_CTRL_SET_TMR_EN_T, *P_TMR_CTRL_SET_TMR_EN_T;

typedef struct GNU_PACKED  _CMD_SET_THERMO_CAL_T{
	UINT8	ucEnable;
	UINT8	ucSourceMode;
	UINT8     ucRFDiffTemp;
	UINT8     ucHiBBPHT;
	UINT8     ucHiBBPNT;
	INT8       cLoBBPLT;
	INT8       cLoBBPNT;
	UINT8     ucReserve;
	BIN_CONTENT_T ucThermoSetting[3];
}CMD_SET_THERMO_CAL_T, *P_CMD_SET_THERMO_CAL_T;

#ifdef MT_WOW_SUPPORT
enum ENUM_PACKETFILTER_TYPE
{
	_ENUM_TYPE_MAGIC 			= 0,
	_ENUM_TYPE_BITMAP 			= 1,
	_ENUM_TYPE_ARPNS 			= 2,
	_ENUM_TYPE_GTK_REKEY 		= 3,
	_ENUM_TYPE_CF 				= 4,
	_ENUM_TYPE_GLOBAL_EN 		= 5,
	_ENUM_TYPE_TCP_SYN 			= 6,
	_ENUM_TYPE_DETECTION_MASK 	= 7,
};

enum ENUM_FUNCTION_SELECT
{
	_ENUM_PF					= 0,
	_ENUM_GLOBAL_MAGIC			= 1,
	_ENUM_GLOBAL_BITMAP			= 2,
	_ENUM_GLOBAL_EAPOL			= 3,
	_ENUM_GLOBAL_TDLS			= 4,
	_ENUM_GLOBAL_ARPNS			= 5,
	_ENUM_GLOBAL_CF				= 6,
	_ENUM_GLOBAL_MODE			= 7,
	_ENUM_GLOBAL_BSSID			= 8,
	_ENUM_GLOBAL_MGMT			= 9,
	_ENUM_GLOBAL_BMC_DROP		= 10,
	_ENUM_GLOBAL_UC_DROP		= 11,
	_ENUM_GLOBAL_ALL_TOMCU		= 12,
    _ENUM_GLOBAL_WOW_EN         = 16,
};

enum ENUM_PF_MODE_T
{
	PF_MODE_WHITE_LIST		= 0,
	PF_MODE_BLACK_LIST		= 1,
	PF_MODE_NUM
};

enum ENUM_PF_BSSID_IDX_T
{
	PF_BSSID_DISABLE    		= 0,
	PF_BSSID_0					= (1 << 0),
	PF_BSSID_1					= (1 << 1),
	PF_BSSID_2					= (1 << 2),
	PF_BSSID_3					= (1 << 3),
};

enum ENUM_PF_BAND_IDX_T
{
	PF_BAND_0					= 0,
	PF_BAND_1					= 1,
};

enum ENUM_PF_WAP_VER_T
{
	PF_WPA						= 0,
	PF_WPA2					= 1,
};

enum ENUM_PF_WMM_IDX_T
{
	PF_WMM_0				= 0,
	PF_WMM_1				= 1,
	PF_WMM_2				= 2,
	PF_WMM_3				= 3,
};

enum ENUM_PF_ARPNS_SET_T
{
	PF_ARP_NS_SET_0			= 0,
	PF_ARP_NS_SET_1			= 1,
};

enum ENUM_PF_ARPNS_ENSABLE_T
{
	PF_ARP_NS_DISABLE			= 0,
	PF_ARP_NS_ENABLE			= 1,
};

enum ENUM_PF_ARPNS_OFFLOAD_TYPE_T
{
	PF_ARP_OFFLOAD			= 0,
	PF_NS_OFFLOAD				= 1,
};

enum ENUM_PF_ARPNS_PKT_TYPE_T
{
	PF_ARP_NS_UC_PKT			= (1 << 0),
	PF_ARP_NS_BC_PKT			= (1 << 1),
	PF_ARP_NS_MC_PKT			= (1 << 2),
	PF_ARP_NS_ALL_PKT			= ((1 << 0) | (1 << 1) | (1 << 2)),
};


typedef struct GNU_PACKED _CMD_PACKET_FILTER_WAKEUP_OPTION_T {
    UINT32	WakeupInterface;
	UINT32	GPIONumber;
	UINT32	GPIOTimer;
	UINT32	GpioParameter;
} CMD_PACKET_FILTER_WAKEUP_OPTION_T, *P_CMD_PACKET_FILTER_WAKEUP_OPTION_T;

typedef struct GNU_PACKED _CMD_PACKET_FILTER_GLOBAL_T {
    UINT32	PFType;
	UINT32	FunctionSelect;
	UINT32	Enable;
	UINT32	Band;	
} CMD_PACKET_FILTER_GLOBAL_T, *P_CMD_PACKET_FILTER_GLOBAL_T;

typedef struct GNU_PACKED _CMD_PACKET_FILTER_MAGIC_PACKET_T {
    UINT32	PFType;
	UINT32	BssidEnable;
} CMD_PACKET_FILTER_MAGIC_PACKET_T, *P_CMD_PACKET_FILTER_MAGIC_PACKET_T;

typedef struct GNU_PACKED _CMD_PACKET_FILTER_BITMAP_PATTERN_T {
    UINT32	PFType;
	UINT32	Index;
	UINT32	Enable;
	UINT32	BssidEnable;
	UINT32	Offset;
	UINT32	FeatureBits;
	UINT32	Resv;
	UINT32	PatternLength;
	UINT32	Mask[4];
	UINT32	Pattern[32];
} CMD_PACKET_FILTER_BITMAP_PATTERN_T, *P_CMD_PACKET_FILTER_BITMAP_PATTERN_T;

typedef struct GNU_PACKED _CMD_PACKET_FILTER_ARPNS_T {
    UINT32	PFType;
	UINT32	IPIndex;
	UINT32	Enable;
	UINT32	BssidEnable;
	UINT32	Offload;
	UINT32	Type;
	UINT32	FeatureBits;
	UINT32	Resv;
	UINT8	IPAddress[16];
} CMD_PACKET_FILTER_ARPNS_T, *P_CMD_PACKET_FILTER_ARPNS_T;

typedef struct GNU_PACKED _CMD_PACKET_FILTER_GTK_T {
    UINT32	PFType;
	UINT32	WPAVersion;
	UINT32	PTK[16];
	UINT32	ReplayCounter[2];
	UINT32	PairKeyIndex;
	UINT32	GroupKeyIndex;
	UINT32	BssidIndex;
	UINT32	OwnMacIndex;
	UINT32 	WmmIndex;
	UINT32	Resv1;
} CMD_PACKET_FILTER_GTK_T, *P_CMD_PACKET_FILTER_GTK_T;

typedef struct GNU_PACKED _CMD_PACKET_FILTER_COALESCE_T {
    UINT32	PFType;
	UINT32	FilterID;
	UINT32	Enable;
	UINT32	BssidEnable;
	UINT32	PacketType;
	UINT32	CoalesceOP;
	UINT32	FeatureBits;
	UINT8	Resv;
	UINT8	FieldLength;
	UINT8	CompareOP;
	UINT8	FieldID;
	UINT32	Mask[2];
	UINT32 	Pattern[4];
} CMD_PACKET_FILTER_COALESCE_T, *P_CMD_PACKET_FILTER_COALESCE_T;

typedef struct GNU_PACKED _CMD_PACKET_TCPSYN_T {
    UINT32	PFType;
	UINT32	AddressType;
	UINT32	Enable;
	UINT32	BssidEnable;
	UINT32	PacketType;
	UINT32	FeatureBits;
	UINT32	TCPSrcPort;
	UINT32 	TCPDstPort;
	UINT32	SourceIP[4];
	UINT32	DstIP[4];
} CMD_PACKET_FILTER_TCPSYN_T, *P_CMD_PACKET_FILTER_TCPSYN_T;


typedef struct GNU_PACKED _EXT_EVENT_PF_GENERAL_T {
 	UINT32   u4PfCmdType;
 	UINT32   u4Status;
 	UINT32   u4Resv;
} EXT_EVENT_PF_GENERAL_T, *P_EXT_EVENT_PF_GENERAL_T;

typedef struct GNU_PACKED _EXT_EVENT_WAKEUP_OPTION_T {
 	UINT32   u4PfCmdType;
 	UINT32   u4Status;
} EXT_EVENT_WAKEUP_OPTION_T, *P_EXT_EVENT_WAKEUP_OPTION_T;
#endif

#if defined(RT_CFG80211_P2P_CONCURRENT_DEVICE) || defined(CFG80211_MULTI_STA)
typedef struct GNU_PACKED _EXT_CMD_NOA_CTRL_T {
	UINT8 ucMode0;
	UINT8 acuReserved0[3];

	UINT8 ucMode1;
	UINT8 acuReserved1[3];

	UINT8 ucMode2;
	UINT8 acuReserved2[3];

	UINT8 ucMode3;
	UINT8 acuReserved3[3];
} EXT_CMD_NOA_CTRL_T, *P_EXT_CMD_NOA_CTRL_T;

#endif /* defined(RT_CFG80211_P2P_CONCURRENT_DEVICE) || defined(CFG80211_MULTI_STA) */
#ifdef CFG_TDLS_SUPPORT
typedef struct GNU_PACKED _EXT_CMD_CFG_TDLS_CHSW_T {
	//UINT8 ucCmd;
	UINT8	ucOffPrimaryChannel;
	UINT8	ucOffCenterChannelSeg0;
	UINT8	ucOffCenterChannelSeg1;
	UINT8	ucOffBandwidth;
	UINT32 	u4StartTimeTsf;
	UINT32	u4SwitchTime;	 /* us */
	UINT32 	u4SwitchTimeout;	/* us */
	UINT8	ucRole;
	UINT8	ucBssIndex;
	UINT8	Reserved[4];

} EXT_CMD_CFG_TDLS_CHSW_T, *P_EXT_CMD_CFG_TDLS_CHSW_T;
#endif /*CFG_TDLS_SUPPORT*/

/*
    CMD prototype structure
*/
#define MAX_FRAG_SIZE               (512)
#define CONTROL_INFO_SIZE           (8)
#define FRAG_CMD_PAYLOAD_SIZE       (MAX_FRAG_SIZE - CONTROL_INFO_SIZE)     /* 504 */

typedef enum _ENUM_FRAG_SEND_STATUS_T
{
    ENUM_FRAG_STATUS_FRAG_ID_SUCCESS            = 0,
    ENUM_FRAG_STATUS_FRAG_ID_WRONG              = 1,
	ENUM_FRAG_STATUS_FRAG_SEQ_WRONG             = 2,
	ENUM_FRAG_STATUS_FRAG_LENGTH_WRONG          = 3,
	ENUM_FRAG_STATUS_FRAG_TOTAL_LENGTH_WRONG    = 4,
	ENUM_FRAG_STATUS_FRAG_RCVD_OUT_OF_ORDER     = 5,
	ENUM_FRAG_STATUS_FRAG_TOTAL_LENGTH_MISMATCH = 6
} ENUM_FRAG_SEND_STATUS_T, *P_ENUM_FRAG_SEND_STATUS_T;

typedef struct GNU_PACKED _EXT_CMD_FRAG_CTRL_T
{
    UINT8  FragmentID;  	                        /* to check fragments in the same group */
    UINT8  FragmentSeq; 	                        /* fragment sequence */
    UINT8  IsLastFragment; 	                        /* indicated the fragment is end */
    UINT8  Reserved;

    UINT16 FragLength;                              /* Indicate the length of current Frag Payload*/
    UINT16 TotalCmdLength;                          /* indicted the total len of original CMD */

    UINT8  FragmentPayload[FRAG_CMD_PAYLOAD_SIZE];  /* to pointer the fragment payload */
    //VOID *FragmentPayload; 	                    /* to pointer the fragment payload */
} EXT_CMD_FRAG_CTRL_T, *P_EXT_CMD_FRAG_CTRL_T;

typedef struct GNU_PACKED _EXT_EVENT_FRAG_SEND_T
{
    UINT8   ucFragID;                       /* FragID is same as Frag Cmd */
    UINT8   ucFragSeq;                      /* Frag Seq is same as Frag Cmd*/
    UINT8   ucErrorStatus;                  /* Error Status */
    UINT8   ucReserved;
} EXT_EVENT_FRAG_SEND_T, *P_EXT_EVENT_FRAG_SEND_T;


typedef struct _MT_RF_REG_PAIR {
    UINT8 WiFiStream;
    UINT32 Register;
    UINT32 Value;
} MT_RF_REG_PAIR;

typedef struct _MT_PWR_MGT_BIT_WIFI_T
{
	UINT8 ucWlanIdx;
	UINT8 ucPwrMgtBit;
 } MT_PWR_MGT_BIT_WIFI_T, *PMT_PWR_MGT_BIT_WIFI_T;

typedef struct _MT_STA_CFG_PTR_T
{
	struct _STA_ADMIN_CONFIG *pStaCfg;
}MT_STA_CFG_PTR_T, *PMT_STA_CFG_PTR_T;

typedef struct _PSM_BIT_CTRL_T
{
	struct _STA_ADMIN_CONFIG *pStaCfg;
	USHORT psm_val;
}PSM_BIT_CTRL_T, *PPSM_BIT_CTRL_T;

typedef struct _RADIO_ON_OFF_T
{
	UINT8 ucDbdcIdx;
	UINT8 ucRadio;
}RADIO_ON_OFF_T, *PRADIO_ON_OFF_T;

typedef struct _GREENAP_ON_OFF_T
{
	UINT8 ucDbdcIdx;
	BOOLEAN ucGreenAPOn;
}GREENAP_ON_OFF_T, *PGREENAP_ON_OFF_T;


typedef struct  {
	UINT8 PmNumber;
	UINT8 PmState;
	UINT8 Bssid[6];
	UINT8 DtimPeriod;
	UINT8 WlanIdx;
	UINT16 BcnInterval;
	UINT32 Aid;
	UINT8 OwnMacIdx;
	UINT8 BcnLossCount;
    UINT8 DbdcIdx;
	UINT8 WmmIdx;
 } MT_PMSTAT_CTRL_T;

 typedef struct  {
	UINT8 ucDbdcIdx;
	BOOLEAN ucGreenAPOn;
 } MT_GREENAP_CTRL_T;



typedef struct {
	UCHAR Channel;
	UCHAR Bw;
	UCHAR CentralSeg0;
	UCHAR CentralSeg1;
	UCHAR Role;
	USHORT StayTime;
	UCHAR OwnMACAddressIdx;
	UCHAR WlanIdx;
	UCHAR BssIdx;
	UCHAR WmmIdx;
}MT_MCC_ENTRT_T;

//#ifdef ERR_RECOVERY
typedef struct _GENERAL_TEST_SIM_ERR_SWITCH_ON_OFF_T
{
    BOOLEAN ucSwitchMode;
    UINT8 ucReserved[3];
}GENERAL_TEST_SIM_ERR_SWITCH_ON_OFF_T,*P_GENERAL_TEST_SIM_ERR_SWITCH_ON_OFF_T;

typedef struct _GENERAL_TEST_SIM_ERR_DET_RECOVERY_T
{
    UINT8 ucModule;
    UINT8 ucSubModule;
    UINT8 ucReserved[2];
} GENERAL_TEST_SIM_ERR_DET_RECOVERY_T, *P_GENERAL_TEST_SIM_ERR_DET_RECOVERY_T;

typedef struct GNU_PACKED _EXT_CMD_GENERAL_TEST_T
{
    UINT8 ucCategory;
    UINT8 ucAction;
    UINT8 ucDiaglogToken;
    UINT8 ucReserved;

    union
    {
        /* for category = GENERAL_TEST_CATEGORY_SIM_ERROR_DETECTION
         * and ucAction = GENERAL_TEST_ACTION_SWITCH_ON_OFF
         */
        GENERAL_TEST_SIM_ERR_SWITCH_ON_OFF_T rGeneralTestSimErrorSwitchOnOff;

        /* for category = GENERAL_TEST_CATEGORY_SIM_ERROR_DETECTION
         * and ucAction = GENERAL_TEST_ACTION_RECOVERY
         */
        GENERAL_TEST_SIM_ERR_DET_RECOVERY_T rGeneralTestSimErrDetRecovery;
    } Data;
} EXT_CMD_GENERAL_TEST_T, *P_EXT_CMD_GENERAL_TEST_T;

#define GENERAL_TEST_CATEGORY_NON (0x0)
#define GENERAL_TEST_CATEGORY_SIM_ERROR_DETECTION (0x1)
#define GENERAL_TEST_CATEGORY_APPWS               (0x2)

#define GENERAL_TEST_ACTION_NON (0x0)
#define GENERAL_TEST_ACTION_SWITCH_ON_OFF (0x1)
#define GENERAL_TEST_ACTION_RECOVERY (0x2)

#define GENERAL_TEST_MODULE_NON (0x0)
#define GENERAL_TEST_MODULE_LMAC (0x1)
#define GENERAL_TEST_MODULE_UMAC (0x2)
#define GENERAL_TEST_MODULE_HIF (0x3)
#define GENERAL_TEST_MODULE_MCU (0x4)

#define GENERAL_TEST_SUBMOD_LMAC_NON (0x0)
#define GENERAL_TEST_SUBMOD_LMAC_TXRXR (0x1)
#define GENERAL_TEST_SUBMOD_LMAC_TX (0x2)
#define GENERAL_TEST_SUBMOD_LMAC_RX (0x3)
#define GENERAL_TEST_SUBMOD_UMAC_NON (0x0)
#define GENERAL_TEST_SUBMOD_UMAC_RESET (0x1)
#define GENERAL_TEST_SUBMOD_HIF_NON (0x0)
#define GENERAL_TEST_SUBMOD_HIF_PDMA0 (0x1)
#define GENERAL_TEST_SUBMOD_HIF_PDMA1 (0x2)
#define GENERAL_TEST_SUBMOD_HIF_PDMA2 (0x3)
//#endif /* ERR_RECOVERY */

/* Action ID of Category GENERAL_TEST_CATEGORY_APPWS */
#define APPWS_ACTION_DUMP_INFO                     0

#ifdef DBDC_MODE

typedef enum {
	DBDC_TYPE_WMM=0,
	DBDC_TYPE_MGMT,
	DBDC_TYPE_BSS,
	DBDC_TYPE_MBSS,
	DBDC_TYPE_REPEATER,
	DBDC_TYPE_MU,
	DBDC_TYPE_BF,
	DBDC_TYPE_PTA,
}DBDC_TYPE;

typedef struct _BAND_CTRL_ENTRY_T
{
	UINT8  ucType;
	UINT8  ucIndex;
	UINT8  ucBandIdx;
	UINT8  ucReserve;
} BAND_CTRL_ENTRY_T, *P_BAND_CTRL_ENTRY_T;

typedef struct GNU_PACKED _EXT_CMD_DBDC_CTRL_T
{
	UINT8  ucDbdcEnable;
	UINT8  ucTotalNum;
	UINT8  aucReserved[2];
	BAND_CTRL_ENTRY_T  aBCtrlEntry[64];
} EXT_CMD_DBDC_CTRL_T, *P_EXT_CMD_DBDC_CTRL_T, EXT_EVENT_DBDC_CTRL_T, *P_EXT_EVENT_DBDC_CTRL_T;

#endif /*DBDC_MODE*/


enum {
	MAC_INFO_TYPE_RESERVE=0,
	MAC_INFO_TYPE_CHANNEL_BUSY_CNT=0x1,
	MAC_INFO_TYPE_TSF=0x2,
	MAC_INFO_TYPE_MIB=0x3,
	MAC_INFO_TYPE_EDCA=0x4,
	MAC_INFO_TYPE_WIFI_INT_CNT=0x5,
};

/*MAC INFO ID:  Get Channel Busy Cnt (0x01)*/
typedef struct _EXTRA_ARG_CH_BUSY_CNT_T
{
    UINT8  ucBand;
    UINT8  aucReserved[3];
} EXTRA_ARG_CH_BUSY_CNT_T, *P_EXTRA_ARG_CH_BUSY_CNT_T;

/*MAC INFO ID:  Get TSF (0x02)*/
typedef struct _EXTRA_ARG_TSF_T
{
    UINT8  ucHwBssidIndex;
    UINT8  aucReserved[3];
} EXTRA_ARG_TSF_T, *P_EXTRA_ARG_TSF_T;


/*MAC INFO ID:  EDCA (0x04)*/
typedef struct _EXTRA_ARG_EDCA_T
{
    UINT8  ucTotalAcNum;
    UINT8  aucReserved[3];
    UINT32 au4AcIndex[HW_TX_QUE_NUM];
} EXTRA_ARG_EDCA_T, *P_EXTRA_ARG_EDCA_T;

/* MAC INFO ID: Get wifi interrupt counter (0x05)*/
typedef struct _EXTRA_ARG_WF_INTERRUPT_CNT_T
{
    UINT8  ucBand;
    UINT8  ucWifiInterruptNum;
    UINT8  aucReserved[2];
    UINT32 u4WifiInterruptMask;
} EXTRA_ARG_WF_INTERRUPT_CNT_T, *P_EXTRA_ARG_WF_INTERRUPT_CNT_T;

typedef union {
	EXTRA_ARG_CH_BUSY_CNT_T	ChBusyCntArg;
	EXTRA_ARG_TSF_T			TsfArg;
	EXTRA_ARG_EDCA_T			EdcaArg;
    EXTRA_ARG_WF_INTERRUPT_CNT_T WifiInterruptCntArg;
} EXTRA_ARG_MAC_INFO_T;



typedef struct GNU_PACKED _EXT_CMD_GET_MAC_INFO_T
{
	UINT16 u2MacInfoId;
	UINT8  aucReserved[2];
	EXTRA_ARG_MAC_INFO_T aucExtraArgument;
} EXT_CMD_GET_MAC_INFO_T, *P_EXT_CMD_GET_MAC_INFO_T;


/*MacInfo ID: Get Channel Busy Cnt(0x01) */
typedef struct _GET_CH_BUSY_CNT_T
{
    UINT32   u4ChBusyCnt;
} GET_CH_BUSY_CNT_T, *P_GET_CH_BUSY_CNT_T;


/*MacInfo ID: 0x02 TSF*/
typedef struct _TSF_RESULT_T
{
    UINT32   u4TsfBit0_31;
    UINT32   u4TsfBit63_32;
} TSF_RESULT_T, *P_TSFRESULT_T;

/* MacInfo ID: 0x05 Get wifi interrupt counter */
typedef struct _GET_WF_INTERRUPT_CNT_T
{
    UINT8   ucWifiInterruptNum;
    UINT8   aucReserved[3];
    UINT32  u4WifiInterruptCounter[0];
} GET_WF_INTERRUPT_CNT_T, *P_WF_INTERRUPT_CNT_T;

/*MacInfo ID: 0x04 EDCA*/
typedef union {
	GET_CH_BUSY_CNT_T	ChBusyCntResult;
	TSF_RESULT_T			TsfResult;
	MT_EDCA_CTRL_T		EdcaResult;
    GET_WF_INTERRUPT_CNT_T WifiIntCntResult;
}MAC_INFO_RESULT_T;


typedef struct GNU_PACKED _EXT_EVENT_MAC_INFO_T
{
    UINT16  u2MacInfoId;
    UINT8  aucReserved[2];
    MAC_INFO_RESULT_T  aucMacInfoResult;
} EXT_EVENT_MAC_INFO_T, *P_EXT_EVENT_MAC_INFO_T;


typedef struct GNU_PACKED _EXT_CMD_MAC_INIT_CTRL_T
{
    UINT8         ucMacInitCtrl;
    UINT8         ucBand;
    UINT8         aucReserve[2];

} EXT_CMD_MAC_INIT_CTRL_T, *P_EXT_CMD_MAC_INIT_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_ID_BWF_LWC_ENABLE
{
    UINT8 		ucBwfLwcEnable; // 0: Disable, 1: Enable
    UINT8 		aucReserve[3];
} EXT_CMD_ID_BWF_LWC_ENABLE_T, *P_EXT_CMD_ID_BWF_LWC_ENABLE_T;

#if defined(CONFIG_HOTSPOT_R2) || defined(DSCP_QOS_MAP_SUPPORT)
typedef struct GNU_PACKED _EXT_CMD_ID_HOTSPOT_INFO_UPDATE {	
	/* hs bss flag */
    UINT8			ucUpdateType;
    UINT8			ucHotspotBssFlags;
    UINT8			ucHotspotBssId;
    /* sta DSCP */
    UINT8			ucStaWcid;
    UINT8			ucStaQosMapFlagAndIdx;
    /* DSCP pool */
    UINT8			ucPoolID;
    UINT8			ucTableValid;
	UINT8			ucPoolDscpExceptionCount;
	UINT32			u4Ac;
	UINT16			au2PoolDscpRange[8];
	UINT16			au2PoolDscpException[21];
} EXT_CMD_ID_HOTSPOT_INFO_UPDATE_T, *P_EXT_CMD_ID_HOTSPOT_INFO_UPDATE_T;

#endif /* CONFIG_HOTSPOT_R2 */

#if defined(PRE_CAL_TRX_SET1_SUPPORT) || defined(RLM_CAL_CACHE_SUPPORT) || defined(PRE_CAL_TRX_SET2_SUPPORT)
#define RXDCOC_SIZE 256
#define TXDPD_SIZE 216
#endif

#ifdef PRE_CAL_TRX_SET1_SUPPORT
typedef struct _RXDCOC_RESULT_T
{                       
    UINT16         u2ChFreq;/* Primary Channel Number */
    UINT8          ucBW;    /* 0: BW20, 1: BW40, 2: BW80, 3:BW160 */
    UINT8          ucBand;   /* 0: 2.4G, 1: 5G */
    BOOLEAN        bSecBW80; /* 0: primary BW80, 1: secondary BW80  - used only in BW160*/
    BOOLEAN        ResultSuccess;
    UINT8		   DBDCEnable;  /* 1: DBDC enable, 0: not in DBDC mode  */
    UINT8          ucReserved;
    
    /* WF0 SX0 */
    UINT32         ucDCOCTBL_I_WF0_SX0_LNA[4];
    UINT32         ucDCOCTBL_Q_WF0_SX0_LNA[4];
    /* WF0 SX2 */
    UINT32         ucDCOCTBL_I_WF0_SX2_LNA[4];
    UINT32         ucDCOCTBL_Q_WF0_SX2_LNA[4];
    /* WF1 SX0 */
    UINT32         ucDCOCTBL_I_WF1_SX0_LNA[4];
    UINT32         ucDCOCTBL_Q_WF1_SX0_LNA[4];
    /* WF1 SX2 */
    UINT32         ucDCOCTBL_I_WF1_SX2_LNA[4];
    UINT32         ucDCOCTBL_Q_WF1_SX2_LNA[4];
    /* WF2 SX0 */
    UINT32         ucDCOCTBL_I_WF2_SX0_LNA[4];
    UINT32         ucDCOCTBL_Q_WF2_SX0_LNA[4];
    /* WF2 SX2 */
    UINT32         ucDCOCTBL_I_WF2_SX2_LNA[4];
    UINT32         ucDCOCTBL_Q_WF2_SX2_LNA[4];
    /* WF3 SX0 */
    UINT32         ucDCOCTBL_I_WF3_SX0_LNA[4];
    UINT32         ucDCOCTBL_Q_WF3_SX0_LNA[4];
    /* WF3 SX2 */
    UINT32         ucDCOCTBL_I_WF3_SX2_LNA[4];
    UINT32         ucDCOCTBL_Q_WF3_SX2_LNA[4];
} RXDCOC_RESULT_T, *P_RXDCOC_RESULT_T;


typedef struct GNU_PACKED _EXT_CMD_GET_RXDCOC_RESULT_T
{
    BOOLEAN             DirectionToCR; 
    UINT8				ucDoRuntimeCalibration;
	UINT8				aucReserved[2];
    RXDCOC_RESULT_T     RxDCOCResult;
} EXT_CMD_GET_RXDCOC_RESULT_T, *P_EXT_CMD_GET_RXDCOC_RESULT_T;

typedef struct _TXDPD_RESULT_T
{	
	UINT16         u2ChFreq;/* Primary Channel Number */
    UINT8          ucBW;    /* 0: BW20, 1: BW40, 2: BW80, 3:BW160 */
    UINT8          ucBand;   /* 0: 2.4G, 1: 5G */
    BOOLEAN        bSecBW80; /* 0: primary BW80, 1: secondary BW80  - used only in BW160*/
    BOOLEAN        ResultSuccess;
    UINT8		   DBDCEnable;  /* 1: DBDC enable, 0: not in DBDC mode  */
    UINT8          ucReserved;    
    /* WF0 */
    UINT32  	   u4DPDG0_WF0_Prim;
    UINT8		   ucDPDLUTEntry_WF0_B0_6[16];	 /* WF0 entry prim part I */
    UINT8		   ucDPDLUTEntry_WF0_B16_23[16]; /* WF0 entry prim part II */
    /* WF1 */
    UINT32  	   u4DPDG0_WF1_Prim;
    UINT8		   ucDPDLUTEntry_WF1_B0_6[16];	 /* WF1 entry prim part I */
    UINT8		   ucDPDLUTEntry_WF1_B16_23[16]; /* WF1 entry prim part II */
    /* WF2 */
    UINT32  	   u4DPDG0_WF2_Prim;
    UINT32  	   u4DPDG0_WF2_Sec;
    UINT8		   ucDPDLUTEntry_WF2_B0_6[16];	 	/* WF2 entry prim part I */
    UINT8		   ucDPDLUTEntry_WF2_B16_23[16];	/* WF2 entry prim part II */
    UINT8		   ucDPDLUTEntry_WF2_B8_14[16];	 	/* WF2 entry secondary part I */
    UINT8		   ucDPDLUTEntry_WF2_B24_31[16];	/* WF2 entry secondary part II */
    /* WF3 */
    UINT32  	   u4DPDG0_WF3_Prim;
    UINT32  	   u4DPDG0_WF3_Sec;
    UINT8		   ucDPDLUTEntry_WF3_B0_6[16];	 	/* WF3 entry prim part I */
    UINT8		   ucDPDLUTEntry_WF3_B16_23[16];	/* WF3 entry prim part II */
    UINT8		   ucDPDLUTEntry_WF3_B8_14[16];	 	/* WF3 entry secondary part I */
    UINT8		   ucDPDLUTEntry_WF3_B24_31[16];	/* WF3 entry secondary part II */
} TXDPD_RESULT_T, *P_TXDPD_RESULT_T;

typedef struct GNU_PACKED _EXT_CMD_GET_TXDPD_RESULT_T
{
	BOOLEAN			DirectionToCR;
	UINT8			ucDoRuntimeCalibration;
	UINT8           aucReserved[2];
	TXDPD_RESULT_T	TxDpdResult;
    	
} EXT_CMD_GET_TXDPD_RESULT_T, *P_EXT_CMD_GET_TXDPD_RESULT_T;

typedef struct GNU_PACKED _EXT_CMD_RDCE_VERIFY_T
{
	BOOLEAN			Result; /* 1 -success ,0 - fail */
	UINT8			ucType; /* 0 - RDCE without compensation , 1 - RDCE with compensation */
	UINT8          	ucBW;    /* 0: BW20, 1: BW40, 2: BW80, 3:BW160 */
   	UINT8          	ucBand;   /* 0: 2.4G, 1: 5G */    	
} EXT_CMD_RDCE_VERIFY_T, *P_EXT_CMD_RDCE_VERIFY_T;

#endif /* PRE_CAL_TRX_SET1_SUPPORT */

#if defined(RLM_CAL_CACHE_SUPPORT) || defined(PRE_CAL_TRX_SET2_SUPPORT)
typedef struct GNU_PACKED _EXT_CMD_GET_PRECAL_RESULT_T
{
	UINT16		u2PreCalBitMap;
	UINT8		ucCalId;
	UINT8		aucReserved;   	
} EXT_CMD_GET_PRECAL_RESULT_T, *P_EXT_CMD_GET_PRECAL_RESULT_T;

typedef enum _PRE_CAL_TYPE{ 
	PRECAL_TXLPF,
	PRECAL_TXIQ,
	PRECAL_TXDC, 
	PRECAL_RXFI,
	PRECAL_RXFD
}PRE_CAL_TYPE;
#endif /* defined(RLM_CAL_CACHE_SUPPORT) || defined(PRE_CAL_TRX_SET2_SUPPORT) */

#ifdef PA_TRIM_SUPPORT
typedef struct GNU_PACKED _EXT_CMD_CAL_CTRL_T
{     
    UINT8  ucFuncIndex;
    UINT8  aucReserved[3];
    UINT32 u4DataLen;
} EXT_CMD_CAL_CTRL_T, *P_EXT_CMD_CAL_CTRL_T;

typedef struct GNU_PACKED _EXT_CMD_PA_TRIM_T
{ 
    EXT_CMD_CAL_CTRL_T Header;
    UINT32 u4Data[4];       
} EXT_CMD_PA_TRIM_T, *P_EXT_CMD_PA_TRIM_T;

typedef enum 
{
	CAL_RESTORE_PA_TRIM = 0x00
}CAL_RESTORE_FUNC_IDX;
#endif /* PA_TRIM_SUPPORT */

typedef struct GNU_PACKED _EXT_CMD_THERMAL_RECAL_MODE_CTRL_T
{     
    UINT8  ucMode;
    UINT8  aucReserved[3];
} EXT_CMD_THERMAL_RECAL_MODE_CTRL_T, *P_EXT_CMD_THERMAL_RECAL_MODE_CTRL_T;

enum
{
	RXHDR_TRANS = 0,
	RXHDR_BL = 1,
};

typedef struct _EXT_RX_HEADER_TRANSLATE_T
{
	UINT8	ucOperation;
	UINT8	ucEnable;
	UINT8	ucCheckBssid;
	UINT8	ucInsertVlan;
	UINT8	ucRemoveVlan;
	UINT8	ucUserQosTid;
	UINT8	ucTranslationMode;
	UINT8	ucReserve;
} EXT_RX_HEADER_TRANSLATE_T, *P_EXT_RX_HEADER_TRANSLATE_T;

typedef struct _EXT_RX_HEADER_TRANSLATE_BL_T
{
	UINT8	ucOperation;
	UINT8	ucCount;
	UINT8	ucReserv[2];
	UINT8	ucBlackListIndex;
	UINT8	ucEnable;
	UINT16	usEtherType;
} EXT_RX_HEADER_TRANSLATE_BL_T, *P_EXT_RX_HEADER_TRANSLATE_BL_T;



#define MAX_BCTRL_ENTRY 64

#ifdef DBDC_MODE

typedef struct _BCTRL_ENTRY {
	UINT8 Type;
	UINT8 Index;
	UINT8 BandIdx;
} BCTRL_ENTRY_T;


typedef struct  _BCTRL_INFO_T {
	BOOLEAN DBDCEnable;
	UINT32 TotalNum;
	BCTRL_ENTRY_T BctrlEntries[MAX_BCTRL_ENTRY];
}BCTRL_INFO_T;

#endif /*DBDC_MODE*/


typedef struct _STA_REC_BA_CFG {
	UCHAR MuarIdx;
	UCHAR BssIdx;
	UCHAR WlanIdx;
	UCHAR tid;
	UCHAR baDirection;
	BOOLEAN BaEnable;
	VOID *BaEntry;
}STA_REC_BA_CFG_T;


typedef struct _STA_REC_CFG {
	UINT8 ucBssIndex;
	UINT8 ucWlanIdx;
	UINT8 ConnectionState;
	UINT8 MuarIdx;
	UINT32 ConnectionType;
	UINT32 u4EnableFeature;
	UINT8 IsNewSTARec;
	struct wifi_dev *wdev;
	struct _MAC_TABLE_ENTRY *pEntry;
	struct _STAREC_AUTO_RATE_UPDATE_T *pRaParam;
}STA_REC_CFG_T;

#define MAX_STA_REC_HISTORY_RECORD  200
typedef struct _STA_REC_TRACKING {
    ULONG StaRecNum;
    ULONG RecordLoop;
    STA_REC_CFG_T sta_rec_cfg_record[MAX_STA_REC_HISTORY_RECORD];
} STA_REC_TRACKING;

UINT16 GetRealPortQueueID(struct cmd_msg *msg, UINT8 cmd_type);

#ifdef CONFIG_ATE
typedef struct _ATE_TXPOWER {
	UINT32 Ant_idx;
	UINT32 Power;
	UINT32 Channel;
	UINT32 Dbdc_idx;
	UINT32 Band_idx;
} ATE_TXPOWER;
#endif /* CONFIG_ATE */

#ifdef BA_TRIGGER_OFFLOAD
typedef struct _EXT_CMD_ID_AUTO_BA {
    UINT8 ucAutoBaEnable; // 0: No reload, 1: do reload
    UINT8 ucTarget;
    UINT8 aucReserve[2];
    UINT32 u4Timeout; /* timeout value, unit ms. */
} EXT_CMD_ID_AUTO_BA_T, *P_EXT_CMD_ID_AUTO_BA_T;

typedef struct GNU_PACKED _CMD_BA_TRIGGER_EVENT_T
{
    UINT8 ucWlanIdx;
    UINT8 ucTid;
    UINT8 aucReserved[2];
} CMD_BA_TRIGGER_EVENT_T, *P_CMD_BA_TRIGGER_EVENT_T;
#endif /* BA_TRIGGER_OFFLOAD */

#define CR4_GET_BSS_ACQ_PKT_NUM_READ_CLEAR_EN		BIT(31)
#define CR4_GET_BSS_ACQ_PKT_NUM_BSS_GROUP_DEFAULT	0x00FFFFFF
#define CR4_GET_BSS_ACQ_PKT_NUM_CMD_DEFAULT     	(CR4_GET_BSS_ACQ_PKT_NUM_READ_CLEAR_EN | CR4_GET_BSS_ACQ_PKT_NUM_BSS_GROUP_DEFAULT)

#define CR4_CFG_BSS_NUM		24
#define CR4_NUM_OF_WMM_AC 	4
typedef struct _EVENT_PER_BSS_ACQ_PKT_NUM_T
{
	UINT32	au4AcqPktCnt[CR4_NUM_OF_WMM_AC];
} EVENT_PER_BSS_ACQ_PKT_NUM_T, *P_EVENT_PER_BSS_ACQ_PKT_NUM_T;

typedef struct _EVENT_BSS_ACQ_PKT_NUM_T
{
	UINT32				u4BssMap;
	EVENT_PER_BSS_ACQ_PKT_NUM_T	bssPktInfo[CR4_CFG_BSS_NUM];
} EVENT_BSS_ACQ_PKT_NUM_T, *P_EVENT_BSS_ACQ_PKT_NUM_T;

typedef struct _CMD_PKT_REPROCESS_EVENT_T {    
	/* MSDU token ID */
    UINT16         u2MsduToken;   
} CMD_PKT_REPROCESS_EVENT_T, *P_CMD_PKT_REPROCESS_EVENT_T;

typedef struct _CMD_GET_CR4_HOTSPOT_CAPABILITY_T {    
    UINT8         ucHotspotBssFlags[CR4_CFG_BSS_NUM];
} CMD_GET_CR4_HOTSPOT_CAPABILITY_T, *P_CMD_GET_CR4_HOTSPOT_CAPABILITY_T;


typedef struct _EXT_EVENT_THERMAL_NOTIFY_T {    
    INT8 cLoThresh; 
    INT8 cHiThresh;   
    INT8 cCurrTemp;   
    INT8 cResv[1];
} EXT_EVENT_THERMAL_NOTIFY_T, *P_EXT_EVENT_THERMAL_NOTIFY_T;

typedef struct GNU_PACKED _EXT_EVENT_CSA_NOTIFY_T
{
    UINT8 ucOwnMacIdx;
    UINT8 ucChannelSwitchCount;
    UINT8 aucReserved[2];
} EXT_EVENT_CSA_NOTIFY_T, *P_EXT_EVENT_CSA_NOTIFY_T;

typedef struct GNU_PACKED _EXT_EVENT_TMR_CALCU_INFO_T
{
    UINT8 aucTmrFrm[36];
    INT16 i2LtfStartAddr;
    UINT16 u2OfdmCoarseTimeMSB;
    UINT32 u4OfdmCoarseTimeLSB;
    INT16 i2MinTFineTime;
    UINT8 ucChBw;
    UINT8 aucResv[1];
    UINT32 u4TOAECalibrationResult;
} EXT_EVENT_TMR_CALCU_INFO_T, *P_EXT_EVENT_TMR_CALCU_INFO_T;



typedef struct GNU_PACKED _EXT_CMD_ID_MCAST_CLONE
{
	UINT8 ucMcastCloneEnable; // 0: Disable, 1: Enable
	UINT8 uc_omac_idx;
	UINT8 aucReserve[2];
} EXT_CMD_ID_MCAST_CLONE_T, *P_EXT_CMD_ID_MCAST_CLONE_T;


typedef struct GNU_PACKED _EXT_CMD_ID_MULTICAST_ENTRY_INSERT
{
	UINT8 aucGroupId[6];
	UINT8 ucBssInfoIdx;
	UINT8 ucMcastEntryType; // 0: STATIC, 1: DYNAMIC
	UINT8 ucMemberNum; // 0: no member. 1: one member, 2:
	UINT8 aucReserve[3];
	UINT8 aucMemberAddr[6];
	UINT8 ucIndex;
	UINT8 ucReserve;
} EXT_CMD_ID_MULTICAST_ENTRY_INSERT_T, *P_EXT_CMD_ID_MULTICAST_ENTRY_INSERT_T;


typedef struct GNU_PACKED _EXT_CMD_ID_MULTICAST_ENTRY_DELETE
{
	UINT8 aucGroupId[6];
	UINT8 ucBssInfoIdx;
	UINT8 ucMemberNum; // 0: no member. 1: one member, 2:
	UINT8 aucMemberAddr[6];
	UINT8 ucIndex;
	UINT8 ucReserve;
} EXT_CMD_ID_MULTICAST_ENTRY_DELETE_T, *P_EXT_CMD_ID_MULTICAST_ENTRY_DELETE_T;

typedef struct _CMD_POWER_SKU_CTRL_T
{
    UINT8  ucPowerCtrlFormatId;
    UCHAR  ucSKUEnable;
    UCHAR  ucBandIdx;
    UINT8  ucReserved;
} CMD_POWER_SKU_CTRL_T, *P_CMD_POWER_SKU_CTRL_T;

typedef struct _CMD_POWER_PERCENTAGE_CTRL_T
{
    UINT8  ucPowerCtrlFormatId;
    UCHAR  ucPercentageEnable;
    UCHAR  ucBandIdx;
    UINT8  ucReserved;
} CMD_POWER_PERCENTAGE_CTRL_T, *P_CMD_POWER_PERCENTAGE_CTRL_T;

typedef struct _CMD_POWER_PERCENTAGE_DROP_CTRL_T
{
    UINT8  ucPowerCtrlFormatId;
    INT8   cPowerDropLevel;
    UINT8  ucBandIdx;
    UINT8  ucReserved;
} CMD_POWER_PERCENTAGE_DROP_CTRL_T, *P_CMD_POWER_PERCENTAGE_DROP_CTRL_T;

typedef struct _CMD_POWER_BF_BACKOFF_CTRL_T
{
    UINT8  ucPowerCtrlFormatId;
    UCHAR  ucBFBackoffEnable;
    UCHAR  ucBandIdx;
    UINT8  ucReserved;
} CMD_POWER_BF_BACKOFF_CTRL_T, *P_CMD_POWER_BF_BACKOFF_CTRL_T;

typedef struct _CMD_POWER_THERMAL_COMP_CTRL_T
{
    UINT8    ucPowerCtrlFormatId;
    BOOLEAN  fgThermalCompEn;
    UINT8    ucBandIdx;
    UINT8    ucReserved;
} CMD_POWER_THERMAL_COMP_CTRL_T, *P_CMD_POWER_THERMAL_COMP_CTRL_T;

typedef struct _CMD_POWER_UPPER_BOUND_CTRL_T
{
    UINT8  ucPowerCtrlFormatId;
    UCHAR  ucUpperBoundCtrl;    // 0: recover to default (0x820F4080=0x3F403F40)  1: reset by EEPROM low rate Power
    UINT8  ucReserved[2];
} CMD_POWER_UPPER_BOUND_CTRL_T, *P_CMD_POWER_UPPER_BOUND_CTRL_T;

typedef struct _CMD_POWER_RF_TXANT_CTRL_T
{
    UINT8  ucPowerCtrlFormatId;
    UINT8  ucTxAntIdx;          // bitwise representation. 0x5 means only TX0, TX2 enabled
    UINT8  ucReserved[2];
} CMD_POWER_RF_TXANT_CTRL_T, *P_CMD_POWER_RF_TXANT_CTRL_T;

typedef struct _CMD_TX_POWER_SHOW_INFO_T
{
    UINT8    ucPowerCtrlFormatId;
    BOOLEAN  fgTxPowerInfoEn;    // 1: Enable TxPower Show Info  0: Disable TxPower Show Info
    UINT8    ucBandIdx;
    UINT8    ucReserved;
} CMD_TX_POWER_SHOW_INFO_T, *P_CMD_TX_POWER_SHOW_INFO_T;

typedef struct _CMD_TOAE_ON_OFF_CTRL {
    BOOLEAN fgTOAEEnable;
    UINT8   aucReserve[3];
} CMD_TOAE_ON_OFF_CTRL, *P_CMD_TOAE_ON_OFF_CTRL;

typedef struct _CMD_EDCCA_ON_OFF_CTRL {
    BOOLEAN fgEDCCAEnable;
    UINT8   ucDbdcBandIdx;
    UINT8   aucReserve[2];
} CMD_EDCCA_ON_OFF_CTRL, *P_CMD_EDCCA_ON_OFF_CTRL;

typedef struct _CMD_POWER_MU_CTRL_T
{
    UINT8   ucPowerCtrlFormatId;
    BOOLEAN fgMUPowerForceMode;
    INT8    cMUPower;
    UCHAR   ucBandIdx;
} CMD_POWER_MU_CTRL_T, *P_CMD_POWER_MU_CTRL_T;

typedef struct _CMD_BF_NDPA_TXD_CTRL_T
{
    UINT8    ucPowerCtrlFormatId;
    BOOLEAN  fgNDPA_ManualMode;
    UINT8    ucNDPA_TxMode;
    UINT8    ucNDPA_Rate;
    UINT8    ucNDPA_BW;
    UINT8    ucNDPA_PowerOffset;
    UINT8    ucReserved[2];
} CMD_BF_NDPA_TXD_CTRL_T, *P_CMD_BF_NDPA_TXD_CTRL_T;

typedef struct _CMD_SET_TSSI_TRAINING_T
{
    UINT8    ucPowerCtrlFormatId;
    UINT8    ucSubFuncId;
    BOOLEAN  fgEnable;
    UINT8    ucReserved;
} CMD_SET_TSSI_TRAINING_T, *P_CMD_SET_TSSI_TRAINING_T;

typedef struct _CMD_POWER_TEMPERATURE_CTRL_T
{
    UINT8    ucPowerCtrlFormatId;
    BOOLEAN  fgManualMode;    // 1: Enable Temperature Manual Ctrl,  0: Disable Temperature Manual Ctrl
    CHAR     cTemperature;    // Temperature (Celsius Degree)
    UINT8    ucReserved;
} CMD_POWER_TEMPERATURE_CTRL_T, *P_CMD_POWER_TEMPERATURE_CTRL_T;

typedef struct _CMD_POWER_BOOST_TABLE_CTRL_T
{
    UINT8    ucPowerCtrlFormatId;
	UINT8    ucBandIdx;
	INT8     cPwrUpCat;
	INT8     cPwrUpValue[7];
    UINT8    ucReserved[2];
} CMD_POWER_BOOST_TABLE_CTRL_T, *P_CMD_POWER_BOOST_TABLE_CTRL_T;

#define SKU_TABLE_SIZE_ALL      53
#define BF_BACKOFF_ON_MODE       0
#define BF_BACKOFF_OFF_MODE      1
#define BF_BACKOFF_MODE          2
#define BF_BACKOFF_CASE         10

typedef struct _EXT_EVENT_TXPOWER_INFO_T {
    UINT8    ucEventCategoryID;
    UINT8    ucBandIdx;
    BOOLEAN  fg2GEPA;
    BOOLEAN  fg5GEPA;

    BOOLEAN  fgSKUEnable;
    BOOLEAN  fgPERCENTAGEEnable;
    BOOLEAN  fgBFBACKOFFEnable;
    BOOLEAN  fgThermalCompEnable;
    
    INT8     cSKUTable[SKU_TABLE_SIZE_ALL];
    INT8     cThermalCompValue;
    INT8     cPowerDrop;
    INT8     aucReserve;

    UINT32   u4RatePowerCRValue[8];     // (TMAC) Band0: 0x820F4020~0x820F403C, Band1: 0x820F4040~0x820F405C

    INT8     cTxPwrBFBackoffValue[BF_BACKOFF_MODE][BF_BACKOFF_CASE];
    
    UINT32   u4BackoffCRValue[6];       // (BBP) Band0: 0x8207067C~82070690, Band1: 0x8207087C~82070890

    UINT32   u4PowerBoundCRValue;       // (TMAC) 0x820F4080
} EXT_EVENT_TXPOWER_INFO_T, *P_EXT_EVENT_TXPOWER_INFO_T;

typedef struct _EXT_EVENT_TXPOWER_BACKUP_T {
    UINT8   ucEventCategoryID;
    UINT8   aucReserve1[3];
    INT8    cTxPowerCompBackup[BAND_NUM][SKU_TABLE_SIZE][SKU_TX_SPATIAL_STREAM_NUM];
} EXT_EVENT_TXPOWER_BACKUP_T, *P_EXT_EVENT_TXPOWER_BACKUP_T;

typedef struct _EXT_EVENT_EPA_STATUS_T {
    UINT8   ucEventCategoryID;
    BOOLEAN fgEPA;
    UINT8   aucReserve1[2];
} EXT_EVENT_EPA_STATUS_T, *P_EXT_EVENT_EPA_STATUS_T;

typedef struct _CMD_ATE_MODE_CTRL_T
{
    UINT8    ucPowerCtrlFormatId;
    BOOLEAN  fgATEModeEn;         // 1: Enable ATE mode  0: disable ATE mode
    UINT8    ucReserved[2];
} CMD_ATE_MODE_CTRL_T, *P_CMD_ATE_MODE_CTRL_T;

typedef struct _CMD_POWER_TPC_CTRL_T
{
    UINT8  ucPowerCtrlFormatId;
    INT8  cTPCPowerValue;
    UINT8  ucBand;
    UINT8  ucChannelBand;
    UINT8  ucCentralChannel;
    UINT8  ucReserved[3];
} CMD_POWER_TPC_CTRL_T, *P_CMD_POWER_TPC_CTRL_T;

#ifdef LINK_TEST_SUPPORT
typedef struct _CMD_CMW_TX_CSD_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    BOOLEAN  fgTxCsdConfigEn;
	UINT8    ucDbdcBandIdx;
    UINT8    ucBandIdx;
    UINT8    ucReserved;
} CMD_CMW_TX_CSD_CTRL_T, *P_CMD_CMW_TX_CSD_CTRL_T;

typedef struct _CMD_CMW_RX_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    UINT8    ucRxAntIdx;
    UINT8    ucReserved[2];
} CMD_CMW_RX_CTRL_T, *P_CMD_CMW_RX_CTRL_T;

typedef struct _CMD_CMW_TXPWR_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    BOOLEAN  fgTxPwrConfigEn;
    UINT8    ucDbdcBandIdx;
    UINT8    ucBandIdx;
} CMD_CMW_TXPWR_CTRL_T, *P_CMD_CMW_TXPWR_CTRL_T;

typedef struct _CMD_CMW_TXPWR_UP_TABLE_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    UINT8    ucTxPwrUpCat;
    UINT8    ucTxPwrUpValue[13];
    UINT8    ucReserved;
} CMD_CMW_TXPWR_UP_TABLE_CTRL_T, *P_CMD_CMW_TXPWR_UP_TABLE_CTRL_T;

typedef struct _CMD_CMW_ACR_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    BOOLEAN  fgACRConfigEn;
    UINT8    ucDbdcBandIdx;
    UINT8    ucReserved;
} CMD_CMW_ACR_CTRL_T, *P_CMD_CMW_ACR_CTRL_T;

typedef struct _CMD_CMW_RCPI_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    BOOLEAN  fgRCPIConfigEn;
    UINT8    ucReserved[2];
} CMD_CMW_RCPI_CTRL_T, *P_CMD_CMW_RCPI_CTRL_T;

typedef struct _CMD_CMW270_SEIDX_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    BOOLEAN  fgSeIdxConfigEn;
    UINT8    ucReserved[2];
} CMD_CMW_SEIDX_CTRL_T, *P_CMD_CMW_SEIDX_CTRL_T;

typedef struct _CMD_CMW_RCPI_MA_CTRL_T
{
    UINT8    ucCMWCtrlFormatId;
    UINT8    ucMAParameter;
    UINT8    ucReserved[2];
} CMD_CMW_RCPI_MA_CTRL_T, *P_CMD_CMW_RCPI_MA_CTRL_T;
#endif /* LINK_TEST_SUPPORT */

#ifdef BCN_OFFLOAD_SUPPORT
VOID RT28xx_UpdateBcnAndTimToMcu(
    IN struct _RTMP_ADAPTER *pAd,
    VOID *wdev_void,
    IN ULONG FrameLen,
    IN ULONG UpdatePos,
    IN UCHAR UpdatePktType);

INT32 MtCmdBcnOffloadSet(struct _RTMP_ADAPTER *pAd, CMD_BCN_OFFLOAD_T bcn_offload);
#endif

INT32 MtCmdMuarConfigSet(struct _RTMP_ADAPTER *pAd, UCHAR *pdata);

INT32 MtCmdExtPwrMgtBitWifi(struct _RTMP_ADAPTER *pAd, MT_PWR_MGT_BIT_WIFI_T rPwrMgtBitWifi);
#ifdef GREENAP_SUPPORT
INT32 MtCmdExtGreenAPOnOffCtrl(struct _RTMP_ADAPTER *pAd, MT_GREENAP_CTRL_T GreenAPCtrl);
#endif /* GREENAP_SUPPORT */
INT32 MtCmdExtPmStateCtrl(struct _RTMP_ADAPTER *pAd, MT_PMSTAT_CTRL_T PmStatCtrl);

UCHAR GetCfgBw2RawBw(UCHAR CfgBw);

INT32 CmdExtPwrMgtBitWifi(struct _RTMP_ADAPTER *pAd, UINT8 ucWlanIdx, UINT8 ucPwrMgtBit);

INT32 MtCmdRestartDLReq(struct _RTMP_ADAPTER *pAd);

INT32 MtCmdHIFLoopBackTest(struct _RTMP_ADAPTER *pAdapter, BOOLEAN IsEnable, UINT8 RxQ);

INT32 MtCmdPatchSemGet(struct _RTMP_ADAPTER *pAd, UINT32 Semaphore);

 INT32 CmdHIFLoopbackReq(struct _RTMP_ADAPTER *pAd, UINT32 enable, UINT32 qidx);

INT32 MtCmdAddressLenReq(struct _RTMP_ADAPTER *pAd, UINT32 address, UINT32 len, UINT32 data_mode);

INT32 MtCmdFwScatter(struct _RTMP_ADAPTER *pAd, UINT8 *dl_payload, UINT32 dl_len, UINT32 count);

 INT32 MtCmdPatchFinishReq(struct _RTMP_ADAPTER *pAd);

 INT32 MtCmdFwStartReq(struct _RTMP_ADAPTER *pAd, UINT32 override, UINT32 address);

INT32 MtCmdWifiHifCtrl(struct _RTMP_ADAPTER *pAd, UINT8 ucDbdcIdx, UINT8 ucHifCtrlId, VOID *pRsult);

INT32 CmdInitAccessRegWrite(struct _RTMP_ADAPTER *pAd, UINT32 address, UINT32 data);

INT32 CmdInitAccessRegRead(struct _RTMP_ADAPTER *pAd, UINT32 address, UINT32 *data);

INT32 CmdChPrivilege(struct _RTMP_ADAPTER *pAd, UINT8 Action, UINT8 control_chl, UINT8 central_chl,
							UINT8 BW, UINT8 TXStream, UINT8 RXStream);

INT32 CmdAccessRegWrite(struct _RTMP_ADAPTER *pAd, UINT32 address, UINT32 data);

INT32 CmdAccessRegRead(struct _RTMP_ADAPTER *pAd, UINT32 address, UINT32 *data);

INT32 MtCmdRFRegAccessWrite(struct _RTMP_ADAPTER *pAd, UINT32 RFIdx, UINT32 Offset, UINT32 Value);

INT32 MtCmdRFRegAccessRead(struct _RTMP_ADAPTER *pAd, UINT32 RFIdx, UINT32 Offset, UINT32 *Value);
INT32 MtCmdRadioOnOffCtrl(struct _RTMP_ADAPTER *pAd, UINT8 On);

INT32 MtCmdWiFiRxDisable(struct _RTMP_ADAPTER *pAd);

INT32 MtCmdChannelSwitch(struct _RTMP_ADAPTER *pAd, struct _MT_SWITCH_CHANNEL_CFG SwChCfg);

#ifdef NEW_SET_RX_STREAM
INT MtCmdSetRxPath(struct _RTMP_ADAPTER *pAd, UINT32 Path, UCHAR BandIdx);
#endif

INT32 CmdNicCapability(struct _RTMP_ADAPTER *pAd);

#ifdef MT_PS

INT32 CmdPsRetrieveStartReq(struct _RTMP_ADAPTER *pAd, UINT32 WlanIdx);

INT32 CmdPsClearReq(struct _RTMP_ADAPTER *pAd, UINT32 wlanidx, BOOLEAN p_wait);

#endif /* MT_PS */
INT32 MtCmdSecKeyReq(struct _RTMP_ADAPTER *pAd, UINT8 AddRemove, UINT8 Keytype, UINT8 *pAddr, UINT8 Alg,
							UINT8 KeyID, UINT8 KeyLen, UINT8 WlanIdx, UINT8 *KeyMaterial);

INT32 MtCmdRfTestSwitchMode(struct _RTMP_ADAPTER *pAd,  UINT32 OpMode, UINT8 IcapLen, UINT16 rsp_len);

#ifdef INTERNAL_CAPTURE_SUPPORT
INT32 MtCmdWifiSpectrum(
    IN struct _RTMP_ADAPTER *pAd, 
    IN EXT_CMD_WIFI_SPECTRUM_CTRL_T WifiSpectrumCtrl, 
    IN CHAR*rsp_payload, 
    IN UINT16 rsp_len);

INT32 MtCmdWifiSpectrumParamSet(
    IN struct _RTMP_ADAPTER *pAd, 
    IN UINT32 Trigger, 
    IN UINT32 RingCapEn,
    IN UINT32 TriggerEvent, 
    IN UINT32 CaptureNode, 
    IN UINT32 CaptureLen,
    IN UINT32 CapStopCycle, 
    IN UINT8  BW,
    IN UINT32 MACTriggerEvent, 
    IN UINT32 SourceAddressLSB, 
    IN UINT32 SourceAddressMSB, 
    IN UINT32 Band);

INT32 MtCmdWifiSpectrumResultGet(
    IN struct _RTMP_ADAPTER *pAd, 
    IN EVENT_WIFI_ICAP_T *WifiSpecInfo);

INT32 MtCmdWifiSpectrumRawDataProc(
    IN struct _RTMP_ADAPTER *pAd);

INT32 MtCmdRfTestIcapParamSet(
    IN struct _RTMP_ADAPTER *pAd, 
    IN UINT32 Trigger, 
    IN UINT32 RingCapEn,
    IN UINT32 TriggerEvent, 
    IN UINT32 CaptureNode, 
    IN UINT32 CaptureLen,
    IN UINT32 CapStopCycle, 
    IN UINT8  BW,
    IN UINT32 MACTriggerEvent, 
    IN UINT32 SourceAddressLSB, 
    IN UINT32 SourceAddressMSB, 
    IN UINT32 Band);

INT32 MtCmdRfTestIcapResultGet(
    IN struct _RTMP_ADAPTER *pAd, 
    IN EVENT_WIFI_ICAP_T *IcapInfo);
#endif /*INTERNAL_CAPTURE_SUPPORT*/

INT32 MtCmdRfTestSetADC(struct _RTMP_ADAPTER *pAd, UINT32 ChannelFreq, UINT8 AntIndex, UINT8 BW, UINT8 SX, UINT8 DbdcIdx, UINT8	RunType, UINT8 FType);

INT32 MtCmdRfTestSetRxGain(struct _RTMP_ADAPTER *pAd, UINT8 LPFG, UINT8	LNA, UINT8 DbdcIdx, UINT8 AntIndex);

INT32 MtCmdRfTestSetTTG(struct _RTMP_ADAPTER *pAd, UINT32 ChannelFreq, UINT32 ToneFreq, UINT8 TTGPwrIdx, UINT8 DbdcIdx);

INT32 MtCmdRfTestSetTTGOnOff(struct _RTMP_ADAPTER *pAd, UINT8 TTGEnable, UINT8 DbdcIdx, UINT8 AntIndex);

INT32 MtCmdDoCalibration(struct _RTMP_ADAPTER *pAd, UINT32 func_idx, UINT32 CalItem, UINT32 band_idx);

INT32 MtCmdTxContinous(struct _RTMP_ADAPTER *pAd, UINT32 PhyMode, UINT32 BW, UINT32 PriCh, UINT32 Central_Ch, UINT32 Mcs, UINT32 WFSel, UINT32 Txfd, UINT8 Band, UINT8 onoff);

INT32 MtCmdTxTone(struct _RTMP_ADAPTER *pAd, UINT8 BandIdx, UINT8 Control, UINT8 AntIndex, UINT8 ToneType,
				  UINT8 ToneFreq, INT32 DcOffset_I, INT32 DcOffset_Q, UINT32 Band);

INT32 MtCmdTxTonePower(struct _RTMP_ADAPTER *pAd, INT32 type, INT32 dec, UINT8 TxAntennaSel, UINT8 Band);

INT32 MtCmdSetRDDTestExt(struct _RTMP_ADAPTER *pAd, UINT32 rdd_idx, UINT32 rdd_in_sel, UINT32 IsStart);

INT32 MtCmdSetRDDTest(struct _RTMP_ADAPTER *pAd, UINT32 IsStart);

INT32 MtCmdSetCalDump(struct _RTMP_ADAPTER *pAd, UINT32 IsEnable);

INT32 MtCmdMultipleMacRegAccessWrite(struct _RTMP_ADAPTER *pAd, struct _RTMP_REG_PAIR *RegPair, UINT32 Num);

INT32 MtCmdMultipleMacRegAccessRead(struct _RTMP_ADAPTER *pAd, struct _RTMP_REG_PAIR *RegPair, UINT32 Num);

INT32 MtCmdMultipleRfRegAccessWrite(struct _RTMP_ADAPTER *pAd,struct _MT_RF_REG_PAIR *RegPair, UINT32 Num);

INT32 MtCmdMultipleRfRegAccessRead(struct _RTMP_ADAPTER *pAd,struct _MT_RF_REG_PAIR *RegPair, UINT32 Num);

INT32 MtCmdThermoCal(struct _RTMP_ADAPTER *pAd, UINT8 IsEnable, UINT8 SourceMode, UINT8 RFDiffTemp, UINT8 HiBBPHT, UINT8 HiBBPNT, INT8 LoBBPLT, INT8 LoBBPNT);

INT32 MtCmdFwLog2Host(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT8 FWLog2HostCtrl);

VOID CmdIOWrite32(struct _RTMP_ADAPTER *pAd, UINT32 Offset, UINT32 Value);

VOID CmdIORead32(struct _RTMP_ADAPTER *pAd, UINT32 Offset, UINT32 *Value);

VOID MtCmdEfusBufferModeSet(struct _RTMP_ADAPTER *pAd, UINT8 EepromType);

#ifdef FWDL_IN_PROBE
VOID MtCmdEfusBufferModeSetNoRsp(struct _RTMP_ADAPTER *pAd, UINT8 EepromType);
#endif

NTSTATUS MtCmdPowerOnWiFiSys(struct _RTMP_ADAPTER *pAd);

VOID CmdExtEventRsp(struct cmd_msg *msg, char *Data, UINT16 Len);

INT32 MtCmdSendRaw(struct _RTMP_ADAPTER *pAd, UCHAR ExtendID, UCHAR *Input, INT len, UCHAR SetQuery);

#ifdef CONFIG_ATE
INT32 MtCmdGetTxPower(struct _RTMP_ADAPTER *pAd, UINT8 pwrType, UINT8 centerCh, UINT8 dbdc_idx, UINT8 Ch_Band, P_EXT_EVENT_ID_GET_TX_POWER_T prTxPwrResult);

INT32 MtCmdSetTxPowerCtrl(struct _RTMP_ADAPTER *pAd, struct _ATE_TXPOWER TxPower);
#ifdef ABSOLUTE_POWER_TEST
INT32 MtCmdSetForceTxPowerCtrl(struct _RTMP_ADAPTER *pAd, UINT8 ucBandIdx, INT8 cTxPower, UINT8 ucPhyMode, UINT8 ucTxRate, UINT8 ucBW);
#endif /* ABSOLUTE_POWER_TEST */
#endif /* CONFIG_ATE */

#ifdef MT_MAC
INT32 CmdETxBfAidSetting(
          struct _RTMP_ADAPTER *pAd,
          UINT16  Aid);

INT32 CmdTxBfApClientCluster(
          struct _RTMP_ADAPTER *pAd,
          UCHAR   ucWlanIdx,
          UCHAR   ucCmmWlanId);

INT32 CmdTxBfReptClonedStaToNormalSta(
          struct _RTMP_ADAPTER *pAd,
          UCHAR   ucWlanIdx,
          UCHAR   ucCliIdx);

INT32 CmdTxBfTxApplyCtrl(
          struct _RTMP_ADAPTER *pAd,
          UCHAR   ucWlanId,
          BOOLEAN fgETxBf,
	      BOOLEAN fgITxBf,
	      BOOLEAN fgMuTxBf,
	      BOOLEAN fgPhaseCali);

INT32 CmdITxBfPhaseCal(
          struct _RTMP_ADAPTER *pAd,
          UCHAR   ucGroup,
          UCHAR   ucGroupL_M_H,
          BOOLEAN fgSX2,
          BOOLEAN ucPhaseCal,
          UCHAR   ucPhaseVerifyLnaGainLevel);

INT32 CmdITxBfPhaseComp(
          struct _RTMP_ADAPTER *pAd,
          UCHAR   ucBW,
          UCHAR   ucBand,
          UCHAR   ucDbdcBandIdx,
          UCHAR	  ucGroup,
          BOOLEAN fgRdFromE2p,
          BOOLEAN fgDisComp);

INT32 CmdTxBfLnaGain(
          struct _RTMP_ADAPTER *pAd,
          UCHAR   ucLnaGain);

INT32 CmdETxBfSoundingPeriodicTriggerCtrl(
          struct _RTMP_ADAPTER *pAd,
          UCHAR   SndgEn,
          UINT32  u4SNDPeriod,
          UCHAR   ucSu_Mu,
          UCHAR   ucMuNum,
          PUCHAR  pwlanidx);

INT32 CmdPfmuMemAlloc(
          struct _RTMP_ADAPTER *pAd,
          UCHAR ucSu_Mu,
          UCHAR ucWlanIdx);

INT32 CmdPfmuMemRelease(
          struct _RTMP_ADAPTER *pAd,
          UCHAR ucWlanIdx);

INT32 CmdPfmuMemAllocMapRead(
          struct _RTMP_ADAPTER *pAd);

INT32 CmdETxBfPfmuProfileTagRead(
          struct _RTMP_ADAPTER *pAd,
          UCHAR                PfmuIdx,
          BOOLEAN              fgBFer);

INT32 CmdETxBfPfmuProfileTagWrite(
          struct _RTMP_ADAPTER *pAd,
          PUCHAR               prPfmuTag1,
          PUCHAR               prPfmuTag2,
          UCHAR                PfmuIdx);

INT32 CmdETxBfPfmuProfileDataRead(
          struct _RTMP_ADAPTER *pAd,
          UCHAR                PfmuIdx,
          BOOLEAN              fgBFer,
          USHORT               SubCarrIdx);

INT32 CmdETxBfPfmuProfileDataWrite(
          struct _RTMP_ADAPTER *pAd,
          UCHAR                PfmuIdx,
          USHORT               SubCarrIdx,
          PUCHAR               pProfileData);

INT32 CmdETxBfPfmuProfileDataWrite20MAll(
          struct _RTMP_ADAPTER *pAd,
          UCHAR                PfmuIdx,
          PUCHAR               pProfileData);

INT32 CmdETxBfPfmuProfilePnRead(
          struct _RTMP_ADAPTER *pAd,
          UCHAR                PfmuIdx);

INT32 CmdETxBfPfmuProfilePnWrite(
          struct _RTMP_ADAPTER *pAd,
          UCHAR                PfmuIdx,
          UCHAR                ucBw,
          PUCHAR               pProfileData);
#endif /* MT_MAC */

#ifdef MT_MAC_BTCOEX
INT AndesCoexOP(struct _RTMP_ADAPTER *pAd,  UCHAR Status);

INT AndesCoexProtectionFrameOP( struct _RTMP_ADAPTER *pAd, UCHAR Mode, UCHAR Rate);

INT AndesCoexBSSInfo( struct _RTMP_ADAPTER *pAd, BOOLEAN Enable, UCHAR bQoS);
#endif

#ifdef RTMP_EFUSE_SUPPORT

VOID MtCmdEfuseAccessRead(struct _RTMP_ADAPTER *pAd, USHORT offset,PUCHAR pData,PUINT isVaild);

VOID MtCmdEfuseAccessWrite(struct _RTMP_ADAPTER *pAd, USHORT offset,PUCHAR pData);

INT32 MtCmdEfuseFreeBlockCount(struct _RTMP_ADAPTER *pAd, UINT32 GetFreeBlock, UINT32 *Value);

#endif /* RTMP_EFUSE_SUPPORT */

INT32 MtCmdThermalProtect(
    struct _RTMP_ADAPTER *pAd,
    UINT8 HighEn,
    CHAR HighTempTh,
    UINT8 LowEn,
    CHAR LowTempTh,
    UINT32 RechkTimer,
    UINT8 RFOffEn,
    CHAR RFOffTh, 
    UINT8 ucType
    );

INT32
MtCmdThermalProtectAdmitDuty(
    struct _RTMP_ADAPTER *pAd,
	UINT32 u4Lv0Duty,
	UINT32 u4Lv1Duty,
	UINT32 u4Lv2Duty,
	UINT32 u4Lv3Duty
	);

VOID MtUpdateBcnAndTimToMcu(
    struct _RTMP_ADAPTER *pAd,
    VOID *wdev_void,
    ULONG FrameLen,
    ULONG UpdatePos,
    UCHAR UpdatePktType);

#ifdef PRETBTT_INT_EVENT_SUPPORT
VOID MtSetTriggerPretbttIntEvent(struct _RTMP_ADAPTER *ad, INT apidx, UCHAR HWBssidIdx, BOOLEAN Enable, UINT16 BeaconPeriod);
INT32 MtCmdTrgrPretbttIntEventSet(struct _RTMP_ADAPTER *ad, CMD_TRGR_PRETBTT_INT_EVENT_T trgr_pretbtt_int_event);
#endif /*PRETBTT_INT_EVENT_SUPPORT*/

INT32 CmdCrUpdate(struct _RTMP_ADAPTER *pAd, VOID *Context, UINT16 Length);

//PNDIS_PACKET WtblTlvBufferAlloc(struct _RTMP_ADAPTER *pAd,  UINT32 u4AllocateSize);
//VOID* WtblNextTlvBuffer(PNDIS_PACKET pWtblTlvBuffer, UINT16 u2Length);
//VOID WtblTlvBufferAppend(PNDIS_PACKET pWtblTlvBuffer,  UINT16 u2Type, UINT16 u2Length, PUCHAR pNextWtblTlvBuffer);
//VOID WtblTlvBufferFree(struct _RTMP_ADAPTER *pAd, PNDIS_PACKET pWtblTlvBuffer);
VOID *pTlvAppend(VOID *pTlvBuffer, UINT16 u2Type, UINT16 u2Length, VOID *pNextTlvBuffer, UINT32 *pu4TotalTlvLen, UCHAR *pucTotalTlvNumber);
INT32 CmdExtTlvBufferSend(
	struct _RTMP_ADAPTER *pAd,
	UINT8 ExtCmdType,
	VOID* pTlvBuffer,
	UINT32 u4TlvLength);
INT32 CmdExtWtblUpdate(struct _RTMP_ADAPTER *pAd, UINT8 ucWlanIdx, UINT8 ucOperation, VOID* pBuffer, UINT32 u4BufferLen);
UINT32 WtblDwQuery(struct _RTMP_ADAPTER *pAd, UINT8 ucWlanIdx, UINT8 ucWtbl1234, UINT8 ucWhichDW);
INT32 WtblDwSet(struct _RTMP_ADAPTER *pAd, UINT8 ucWlanIdx, UINT8 ucWtbl1234, UINT8 ucWhichDW, UINT32 u4DwMask, UINT32 u4DwValue);


struct cmd_wtbl_dw_mask_set{
    UINT8 ucWhichDW;
    UINT32 u4DwMask;
    UINT32 u4DwValue;
};
INT32 WtblResetAndDWsSet(struct _RTMP_ADAPTER *pAd, UINT8 ucWlanIdx, UINT8 ucWtbl1234, INT dw_cnt, struct cmd_wtbl_dw_mask_set *dw_set);
            

INT32 CmdExtDevInfoUpdate(
	struct _RTMP_ADAPTER *pAd,
	UINT8 OwnMacIdx,
	UINT8 *OwnMacAddr,
	UINT8 BandIdx,
	UINT8 Active,
							UINT32 EnableFeature);

INT32 CmdExtSetTmrCR(
            struct _RTMP_ADAPTER *pAd,
            UCHAR enable,
            UCHAR BandIdx);

INT32 CmdExtStaRecUpdate(
	struct _RTMP_ADAPTER *pAd,
	STA_REC_CFG_T StaRecCfg);

INT32 CmdETxBfStaRecRead(
    struct _RTMP_ADAPTER *pAd,
    UCHAR ucWlanID);

INT32 CmdTxBfTxPwrBackOff(
    struct _RTMP_ADAPTER *pAd,
    UCHAR  ucBandIdx,
    PCHAR  pacTxPwrFccBfOnCase,
    PCHAR  pacTxPwrFccBfOffCase);

INT32 CmdTxBfAwareCtrl(
    struct _RTMP_ADAPTER *pAd,
    BOOLEAN fgBfAwareCtrl);

INT32 CmdTxBfeeHwCtrl(
    struct _RTMP_ADAPTER *pAd,
    BOOLEAN fgBfeeHwEn);

INT32 CmdTxBfHwEnableStatusUpdate(
    struct _RTMP_ADAPTER *pAd,
    BOOLEAN fgEBf,
    BOOLEAN fgIBf);

INT32 CmdExtStaRecBaUpdate(
	struct _RTMP_ADAPTER *pAd,
	STA_REC_BA_CFG_T StaRecBaCfg);


INT32 CmdSetSyncModeByBssInfoUpdate(
    struct _RTMP_ADAPTER *pAd,
    struct _BSS_INFO_ARGUMENT_T bss_info_argument);

INT32 CmdExtBssInfoUpdate(
    struct _RTMP_ADAPTER *pAd,
    struct _BSS_INFO_ARGUMENT_T bss_info_argument);

#ifdef CONFIG_HW_HAL_OFFLOAD
INT32 MtCmdATETest(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_ATE_TEST_MODE_T *param);
INT32 MtCmdCfgOnOff(struct _RTMP_ADAPTER *pAd, UINT8 Type, UINT8 Enable, UINT8 Band);
INT32 MtCmdSetAntennaPort(struct _RTMP_ADAPTER *pAd, UINT8 RfModeMask, UINT8 RfPortMask, UINT8 AntPortMask);
INT32 MtCmdATESetSlotTime(struct _RTMP_ADAPTER *pAd, UINT8 SlotTime, UINT8 SifsTime,UINT8 RifsTime,UINT16 EifsTime, UCHAR BandIdx);
INT32 MtCmdATESetPowerDropLevel(struct _RTMP_ADAPTER *pAd, UINT8 PowerDropLevel, UCHAR BandIdx);
INT32 MtCmdRxFilterPktLen(struct _RTMP_ADAPTER *pAd, UINT8 Enable, UINT8 Band, UINT32 RxPktLen);
INT32 MtCmdSetFreqOffset(struct _RTMP_ADAPTER *pAd, UINT32 FreqOffset);
INT32 MtCmdGetFreqOffset(struct _RTMP_ADAPTER *pAd, UINT32 *FreqOffsetResult);
INT32 MtCmdGetCfgOnOff(struct _RTMP_ADAPTER *pAd, UINT32 Type, UINT8 Band, UINT32 * Status);
INT32 MtCmdSetPhyCounter(struct _RTMP_ADAPTER *pAd, UINT32 Control, UINT8 band_idx);
INT32 MtCmdSetRxvIndex(struct _RTMP_ADAPTER *pAd, UINT8 Group_1, UINT8 Group_2, UINT8 band_idx);
INT32 MtCmdSetFAGCPath(struct _RTMP_ADAPTER *pAd, UINT8 Path, UINT8 band_idx);
#endif
INT32 MtCmdClockSwitchDisable(struct _RTMP_ADAPTER *pAd, UINT8 isDisable);
INT32 MtCmdUpdateProtect(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_UPDATE_PROTECT_T *param);
INT32 MtCmdSetRdg(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_RDG_CTRL_T *param);
INT32 MtCmdSetSnifferMode(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_SNIFFER_MODE_T *param);

VOID MtCmdMemDump(struct _RTMP_ADAPTER *pAd, UINT32 Addr, PUINT8 pData);


#ifdef CONFIG_ATE
INT32 CmdTxContinous(struct _RTMP_ADAPTER *pAd, UINT32 PhyMode, UINT32 BW, UINT32 PriCh, UINT32 Mcs, UINT32 WFSel, UCHAR onoff);
INT32 CmdTxTonePower(struct _RTMP_ADAPTER *pAd, INT32 type, INT32 dec);
#endif
INT32 MtCmdGetThermalSensorResult(struct _RTMP_ADAPTER *pAd, UINT8 ActionIdx, UINT32 *SensorResult);

#ifdef RACTRL_FW_OFFLOAD_SUPPORT
struct _EXT_EVENT_TX_STATISTIC_RESULT_T;
INT32 MtCmdGetTxStatistic(struct _RTMP_ADAPTER *pAd, UINT32 u4Field, UINT8 ucBand, UINT8 ucWcid, struct _EXT_EVENT_TX_STATISTIC_RESULT_T *prTxStatResult);
#ifdef RACTRL_LIMIT_MAX_PHY_RATE
INT32 MtCmdSetMaxPhyRate(struct _RTMP_ADAPTER *pAd, UINT16 u2MaxPhyRate);
#endif /* RACTRL_LIMIT_MAX_PHY_RATE */
#endif /* RACTRL_FW_OFFLOAD_SUPPORT */

INT32 MtCmdTmrCal(struct _RTMP_ADAPTER *pAd, UINT8 enable, UINT8 band, UINT8 bw, UINT8 ant, UINT8 role);

INT32 MtCmdEdcaParameterSet(struct _RTMP_ADAPTER *pAd, MT_EDCA_CTRL_T EdcaParam);

INT32 MtCmdSlotTimeSet(struct _RTMP_ADAPTER *pAd, UINT8 SlotTime,UINT8 SifsTime,UINT8 RifsTime,UINT16 EifsTime,UCHAR BandIdx);

#ifdef CONFIG_MULTI_CHANNEL
INT MtCmdMccStart(struct _RTMP_ADAPTER *pAd, UINT32 Num,MT_MCC_ENTRT_T *MccEntries,USHORT IdleTime,USHORT NullRepeatCnt,ULONG StartTsf);

INT32 MtCmdMccStop(struct _RTMP_ADAPTER *pAd,UCHAR ParkingIndex,UCHAR   AutoResumeMode,UINT16 AutoResumeInterval,ULONG  AutoResumeTsf);
#endif /* CONFIG_MULTI_CHANNEL */

#if defined(RT_CFG80211_P2P_CONCURRENT_DEVICE) || defined(CFG80211_MULTI_STA)
INT32 CmdP2pNoaOffloadCtrl(struct _RTMP_ADAPTER *ad, UINT8 enable);
#endif /* defined(RT_CFG80211_P2P_CONCURRENT_DEVICE) || defined(CFG80211_MULTI_STA) */

INT32 MtCmdSendMsg(struct _RTMP_ADAPTER *pAd, struct cmd_msg *msg);
INT32 MtCmdLEDCtrl(struct _RTMP_ADAPTER *pAd, UINT32 LEDNumber, UINT32 LEDBehavior);

#ifdef ERR_RECOVERY
INT32 CmdExtGeneralTestOn(struct _RTMP_ADAPTER *pAd, BOOLEAN enable);
INT32 CmdExtGeneralTestMode(struct _RTMP_ADAPTER *pAd, UINT8 mode, UINT8 submode);
#endif /* ERR_RECOVERY */

#ifdef DBDC_MODE
INT32 MtCmdGetDbdcCtrl(struct _RTMP_ADAPTER *pAd, struct _BCTRL_INFO_T *pBandInfo);
INT32 MtCmdSetDbdcCtrl(struct _RTMP_ADAPTER *pAd, struct _BCTRL_INFO_T *pBandInfo);
#endif

#ifdef MT_DFS_SUPPORT//Jelly20150123
INT32 MtCmdRddCtrl(
    IN struct _RTMP_ADAPTER *pAd,
    IN UCHAR ucRddCtrl,
    IN UCHAR ucRddIdex,
    IN UCHAR ucRddInSel,
    IN UCHAR ucRddTpRatio);
#endif /*MT_DFS_SUPPORT*/

INT32 MtCmdGetEdca(struct _RTMP_ADAPTER *pAd,MT_EDCA_CTRL_T *pEdcaCtrl);
INT32 MtCmdGetTsfTime(
            struct _RTMP_ADAPTER *pAd,
            UCHAR HwBssidIdx,
            TSF_RESULT_T *pTsfResult);
INT32 MtCmdGetChBusyCnt(struct _RTMP_ADAPTER *pAd,UCHAR ChIdx,UINT32 *pChBusyCnt);
INT32 MtCmdGetWifiInterruptCnt(struct _RTMP_ADAPTER *pAd,UCHAR ChIdx,UCHAR WifiIntNum,UINT32 WifiIntMask,UINT32 *pWifiInterruptCnt);
INT32 MtCmdSetMacTxRx(struct _RTMP_ADAPTER *pAd, UCHAR BandIdx, BOOLEAN bEnable);
INT32 MtCmdSetDfsTxStart(struct _RTMP_ADAPTER *pAd, UCHAR BandIdx);
#ifdef PRE_CAL_TRX_SET1_SUPPORT
INT32 MtCmdGetRXDCOCCalResult(struct _RTMP_ADAPTER *pAd, BOOLEAN DirectionToCR
	,UINT16 CentralFreq,UINT8 BW,UINT8 Band,BOOLEAN IsSecondary80, BOOLEAN DoRuntimeCalibration, RXDCOC_RESULT_T *pRxDcocResult);
INT32 MtCmdGetTXDPDCalResult(struct _RTMP_ADAPTER *pAd, BOOLEAN DirectionToCR
	,UINT16 CentralFreq,UINT8 BW,UINT8 Band,BOOLEAN IsSecondary80, BOOLEAN DoRuntimeCalibration, TXDPD_RESULT_T *pTxDPDResult);
INT32 MtCmdRDCE(struct _RTMP_ADAPTER *pAd, UINT8 type, UINT8 BW, UINT8 Band);
#endif /* PRE_CAL_TRX_SET1_SUPPORT */

#ifdef RLM_CAL_CACHE_SUPPORT
VOID rlmCalCacheApply(struct _RTMP_ADAPTER *pAd, VOID* rlmCache);
#endif /* RLM_CAL_CACHE_SUPPORT */

#ifdef PRE_CAL_TRX_SET2_SUPPORT
INT32 MtCmdGetPreCalResult(struct _RTMP_ADAPTER *pAd, UINT8 CalId, UINT16 PreCalBitMap);
INT32 MtCmdPreCalReStoreProc(struct _RTMP_ADAPTER *pAd, INT32 *pPreCalBuffer);
#endif/* PRE_CAL_TRX_SET2_SUPPORT */ 

#ifdef PA_TRIM_SUPPORT
INT32 MtCmdCalReStoreFromFileProc(struct _RTMP_ADAPTER *pAd, CAL_RESTORE_FUNC_IDX FuncIdx);
INT32 MtCmdPATrimReStoreProc(struct _RTMP_ADAPTER *pAd);
#endif /* PA_TRIM_SUPPORT */
INT32 MtCmdThermalReCalMode(struct _RTMP_ADAPTER *pAd, UINT8 Mode);
INT32 CmdRxHdrTransUpdate(struct _RTMP_ADAPTER *pAd, BOOLEAN En, BOOLEAN ChkBssid, BOOLEAN InSVlan, BOOLEAN RmVlan, BOOLEAN SwPcP);
INT32 CmdRxHdrTransBLUpdate(struct _RTMP_ADAPTER *pAd, UINT8 Index, UINT8 En, UINT16 EthType);
#ifdef VOW_SUPPORT
INT32 MtCmdSetVoWDRRCtrl(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_VOW_DRR_CTRL_T *param);
INT32 MtCmdSetVoWGroupCtrl(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_BSS_CTRL_T *param);
INT32 MtCmdSetVoWFeatureCtrl(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_VOW_FEATURE_CTRL_T *param);
INT32 MtCmdSetVoWRxAirtimeCtrl(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_RX_AT_CTRL_T *param);
INT32 MtCmdGetVoWRxAirtimeCtrl(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_RX_AT_CTRL_T *param);
INT32 MtCmdSetVoWModuleCtrl(struct _RTMP_ADAPTER *pAd, struct _EXT_CMD_AT_PROC_MODULE_CTRL_T *param);
INT32 MtCmdSetVoWCounterCtrl(struct _RTMP_ADAPTER *pAd, UCHAR cmd, UCHAR val);
#if defined(MT7615_FPGA) || defined(MT7622_FPGA)
INT32 MtCmdSetStaQLen(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 qLen);
INT32 MtCmdSetSta2QLen(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 qLen);
INT32 MtCmdSetEmptyThreshold(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 threshold);
INT32 MtCmdSetStaCnt(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 cnt);
#endif /* defined(MT7615_FPGA) || defined(MT7622_FPGA) */
#endif /* VOW_SUPPORT */

#ifdef RED_SUPPORT
INT32 MtCmdSetRedShowSta(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 Num);
INT32 MtCmdSetRedEnable(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 en);
INT32 MtCmdSetRedTargetDelay(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 Num);
#endif /* RED_SUPPORT */
INT32 MtCmdSetCPSEnable(struct _RTMP_ADAPTER *pAd, UINT8 McuDest, UINT32 en);
INT32 CmdAutoBATrigger(struct _RTMP_ADAPTER *pAd, BOOLEAN Enable, UINT32 Timeout);

INT32 MtCmdCr4Query(struct _RTMP_ADAPTER *pAd, UINT32 arg0, UINT32 arg1, UINT32 arg2);
INT32 MtCmdCr4Set(struct _RTMP_ADAPTER *pAd, UINT32 arg0, UINT32 arg1, UINT32 arg2);
INT32 MtCmdCr4Capability(struct _RTMP_ADAPTER *pAd, UINT32 option);
INT32 MtCmdCr4Debug(struct _RTMP_ADAPTER *pAd, UINT32 option);
INT MtCmdSetTxRxPath(struct _RTMP_ADAPTER *pAd,struct _MT_SWITCH_CHANNEL_CFG SwChCfg);
INT32 MtCmdCr4QueryBssAcQPktNum(
    struct _RTMP_ADAPTER *pAd, 
    UINT32 u4bssbitmap);

#ifdef BACKGROUND_SCAN_SUPPORT
INT32 MtCmdBgndScan(struct _RTMP_ADAPTER *pAd, struct _MT_BGND_SCAN_CFG BgScCfg);
INT32 MtCmdBgndScanNotify(struct _RTMP_ADAPTER *pAd, struct _MT_BGND_SCAN_NOTIFY BgScNotify);
#endif /* BACKGROUND_SCAN_SUPPORT */

INT32 CmdExtGeneralTestAPPWS(struct _RTMP_ADAPTER *pAd, UINT action);
INT32 CmdMcastCloneEnable(struct _RTMP_ADAPTER *pAd, BOOLEAN Enable, UINT8 omac_idx);
BOOLEAN CmdMcastEntryInsert(struct _RTMP_ADAPTER *pAd, PUCHAR GrpAddr, UINT8 BssIdx, UINT8 Type, PUCHAR MemberAddr, PNET_DEV dev, UINT8 WlanIndex);
BOOLEAN CmdMcastEntryDelete(struct _RTMP_ADAPTER *pAd, PUCHAR GrpAddr, UINT8 BssIdx, PUCHAR MemberAddr, PNET_DEV dev, UINT8 WlanIndex);

#ifdef SINGLE_SKU_V2
NDIS_STATUS	RTMPSetSingleSKUParameters(struct _RTMP_ADAPTER *pAd);
NDIS_STATUS	RTMPSetBfBackOffParameters(struct _RTMP_ADAPTER *pAd);
INT	MtSingleSkuLoadParam(struct _RTMP_ADAPTER *pAd);
VOID MtSingleSkuUnloadParam(struct _RTMP_ADAPTER *pAd);
INT	MtBfBackOffLoadTable(struct _RTMP_ADAPTER *pAd);
VOID MtBfBackOffUnloadTable(struct _RTMP_ADAPTER *pAd);
#endif /* SINGLE_SKU_V2 */

INT32 MtCmdTxPowerSKUCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgTxPowerSKUEn, UCHAR BandIdx);
INT32 MtCmdTxPowerPercentCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgTxPowerPercentEn, UCHAR BandIdx);
INT32 MtCmdTxPowerDropCtrl(struct _RTMP_ADAPTER *pAd, UINT8 ucPowerDrop, UCHAR BandIdx);
INT32 MtCmdTxBfBackoffCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgTxBFBackoffEn, UCHAR BandIdx);
INT32 MtCmdThermoCompCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgThermoCompEn, UCHAR BandIdx);
INT32 MtCmdTxPwrRfTxAntCtrl(struct _RTMP_ADAPTER *pAd, UINT8 ucTxAntIdx);
INT32 MtCmdTxPwrShowInfo(struct _RTMP_ADAPTER *pAd, UCHAR TxPowerInfoEn, UINT8 BandIdx);
INT32 MtCmdTOAECalCtrl(struct _RTMP_ADAPTER *pAd, UCHAR TOAECtrl);
INT32 MtCmdEDCCACtrl(struct _RTMP_ADAPTER *pAd, UCHAR BandIdx, UCHAR EDCCACtrl);
INT32 MtCmdMUPowerCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN MUPowerForce, UCHAR MUPowerCtrl, UCHAR BandIdx);
INT32 MtCmdBFNDPATxDCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgNDPA_ManualMode, UINT8 ucNDPA_TxMode, UINT8 ucNDPA_Rate, UINT8 ucNDPA_BW, UINT8 ucNDPA_PowerOffset);
INT32 MtEPAcheck(struct _RTMP_ADAPTER *pAd);
INT32 MtATETSSITracking(struct _RTMP_ADAPTER *pAd, BOOLEAN fgEnable);
INT32 MtATEFCBWCfg(struct _RTMP_ADAPTER *pAd, BOOLEAN fgEnable);
INT32 MtTSSICompBackup(struct _RTMP_ADAPTER *pAd, BOOLEAN fgEnable);
INT32 MtTSSICompCfg(struct _RTMP_ADAPTER *pAd);
INT32 MtCmdTemperatureCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgManualMode, CHAR cTemperature);

#ifdef TX_POWER_CONTROL_SUPPORT
INT32 MtCmdTxPwrUpCtrl(struct _RTMP_ADAPTER *pAd, INT8 ucBandIdx, CHAR cPwrUpCat, CHAR cPwrUpValue[POWER_UP_CATEGORY_RATE_NUM]);
#endif /* TX_POWER_CONTROL_SUPPORT */

#ifdef LINK_TEST_SUPPORT
INT32 MtCmdLinkTestTxCsdCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgTxCsdConfigEn, UINT8 ucDbdcBandIdx, UINT8 ucBandIdx);
INT32 MtCmdLinkTestRxCtrl(struct _RTMP_ADAPTER *pAd, UINT8 ucRxAntIdx);
INT32 MtCmdLinkTestTxPwrCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgTxPwrConfigEn, UINT8 ucDbdcBandIdx, UINT8 ucBandIdx);
INT32 MtCmdLinkTestTxPwrUpTblCtrl(struct _RTMP_ADAPTER *pAd, UINT8 ucTxPwrUpCat, PUINT8 pucTxPwrUpValue);
INT32 MtCmdLinkTestACRCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgACRConfigEn, UINT8 ucDbdcBandIdx);
INT32 MtCmdLinkTestRcpiCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgRCPIConfigEn);
INT32 MtCmdLinkTestSeIdxCtrl(struct _RTMP_ADAPTER *pAd, BOOLEAN fgSeIdxConfigEn);
INT32 MtCmdLinkTestRcpiMACtrl(struct _RTMP_ADAPTER *pAd, UINT8 ucMAParameter);
#endif /* LINK_TEST_SUPPORT */

#ifdef TPC_SUPPORT
INT32 MtCmdTpcFeatureCtrl(struct _RTMP_ADAPTER *pAd, INT8 TpcPowerValue, UINT8 BandIdx, UINT8 CentralChannel);
#endif /* TPC_SUPPORT */
INT32 MtCmdATEModeCtrl(struct _RTMP_ADAPTER *pAd, UCHAR ATEMode);


/*CR4 commond for budget control*/

#ifdef PKT_BUDGET_CTRL_SUPPORT
#define PBC_NUM_OF_PKT_BUDGET_CTRL_QUE  (5)
#define PBC_BSS_IDX_FOR_ALL             (0xFF)
#define PBC_WLAN_IDX_FOR_ALL            (0xFFFF)
#define PBC_BOUNDARY_RESET_TO_DEFAULT 	(0xFFFF)

#define PBC_WMM_UP_DEFAULT_BK (900)
#define PBC_WMM_UP_DEFAULT_BE (1500)
#define PBC_WMM_UP_DEFAULT_VI (1900)
#define PBC_WMM_UP_DEFAULT_VO (1900)
#define PBC_WMM_UP_DEFAULT_MGMT (32)


typedef struct GNU_PACKED _CMD_PKT_BUDGET_CTRL_ENTRY_T
{
                UINT16 lower_bound;
                UINT16 upper_bound;
} CMD_PKT_BUDGET_CTRL_ENTRY_T, *P_CMD_PKT_BUDGET_CTRL_ENTRY_T;


typedef struct GNU_PACKED _CMD_PKT_BUDGET_CTRL_T
{
                UINT8 bss_id;
                UINT8 queue_num;
                UINT16 wlan_idx;
                UINT8 aucReserved[4];
                CMD_PKT_BUDGET_CTRL_ENTRY_T aacQue[PBC_NUM_OF_PKT_BUDGET_CTRL_QUE];
} CMD_PKT_BUDGET_CTRL_T, *P_CMD_PKT_BUDGET_CTRL_T;


enum {
	PBC_TYPE_FIRST=0,
	PBC_TYPE_NORMAL=PBC_TYPE_FIRST,
	PBC_TYPE_WMM,
	PBC_TYPE_END
};

enum {
	PBC_AC_BK=0,
	PBC_AC_BE=1,
	PBC_AC_VI=2,
	PBC_AC_VO=3,
	PBC_AC_MGMT=4,
	PBC_AC_NUM=PBC_NUM_OF_PKT_BUDGET_CTRL_QUE,
};

INT32 MtCmdPktBudgetCtrl(struct _RTMP_ADAPTER *pAd,UINT8 bss_idx,UINT16 wcid,UCHAR type);

#endif /*PKT_BUDGET_CTRL_SUPPORT*/

INT32 MtCmdSetBWFEnable(struct _RTMP_ADAPTER *pAd, UINT8 Enable);
#if  defined(CONFIG_HOTSPOT_R2) || defined(DSCP_QOS_MAP_SUPPORT)
INT32 MtCmdHotspotInfoUpdate(struct _RTMP_ADAPTER *pAd, EXT_CMD_ID_HOTSPOT_INFO_UPDATE_T InfoUpdateT);
#endif /* CONFIG_HOTSPOT_R2 */

#endif /* __MT_CMD_H__ */

