#ifndef		__LIBAGI_UB_H__
#define		__LIBAGI_UB_H__

#include "wtextern.h"

//====================================
#ifndef		ERR_NO
#define		ERR_NO   0
#endif

#define		ERR_AGI_UB_BUSY						900001
#define		ERR_AGI_UB_PENDING					900002
//=====================================

#define		AGI_UB_OUT_JSON_SIZE				4096

#define		AGI_UB_MAJOR_VER					"23"
#define		AGI_UB_MINOR_VER					"4.24.101"

//====================================================
#define		AGI_UB_JSON_EP_ACC					"ep_acc"
#define		AGI_UB_JSON_ACTION					"action"
#define		AGI_UB_JSON_PARAM					"param"
#define		AGI_UB_JSON_KEY						"key"
#define		AGI_UB_JSON_CMD						"cmd"
#define		AGI_UB_JSON_ACCOUNT					"account"
#define		AGI_UB_JSON_UPDATE					"update"
#define		AGI_UB_JSON_REPEAT					"repeat"

//action类型
#define		AGI_UB_JSON_ACTION_VALUE_DO_CTRL				"do_ctrl"
#define		AGI_UB_JSON_ACTION_VALUE_QUERY					"query"
#define		AGI_UB_JSON_ACTION_VALUE_PLAY_FILE				"play_file"
#define		AGI_UB_JSON_ACTION_VALUE_REC_FILE				"rec_file"
#define		AGI_UB_JSON_ACTION_VALUE_CALL_OUT				"call_out"
#define		AGI_UB_JSON_ACTION_VALUE_DIAL_DTMF				"dial_dtmf"
#define		AGI_UB_JSON_ACTION_VALUE_HANGUP					"hangup"
#define		AGI_UB_JSON_ACTION_VALUE_ANSWER					"answer"//可以摘机后继续处理某些默认控制,比如打开麦克风等
#define		AGI_UB_JSON_ACTION_VALUE_SET_ACC_PARAM			"set_acc_param"//设备账号相关参数,可以是永久也可以是临时,永久的就是保持到acc_param.json里
#define		AGI_UB_JSON_ACTION_VALUE_SET_CALL_PARAM			"set_call_param"
#define		AGI_UB_JSON_ACTION_VALUE_UPDATE_CALL_PARAM		"update_call_param"
#define		AGI_UB_JSON_ACTION_VALUE_WRITE_AGICALL_AUTH		"write_agicall_auth"//写入设备授权信息



#define		AGI_UB_JSON_QUERY_VALUE_ACC_INFO				"acc_info"

#define		AGI_UB_ACC_INFO									"acc_info"//包含data和ctrl
#define		AGI_UB_ACC_DATA									"acc_data"

//====================================================
#define		AGI_UB_JSON_KEY_VER							"version"
#define		AGI_UB_JSON_KEY_MAJOR						"major"
#define		AGI_UB_JSON_KEY_MINOR						"minjor"
#define		AGI_UB_JSON_KEY_HANDLE						"handle"
#define		AGI_UB_JSON_KEY_INDEX						"index"
#define		AGI_UB_JSON_KEY_SERIAL						"serial"
#define		AGI_UB_JSON_KEY_TYPE						"type"
#define		AGI_UB_JSON_KEY_GUID						"guid"
#define		AGI_UB_JSON_KEY_AUTH						"auth"
#define		AGI_UB_JSON_KEY_FUNC						"func"
#define		AGI_UB_JSON_KEY_DATE						"date"
//====================================================

#define		AGI_UB_JSON_KEY_CTRL_NAME					"ctrl_name"
#define		AGI_UB_JSON_KEY_VALUE						"value"
#define		AGI_UB_JSON_KEY_FILE						"file"
#define		AGI_UB_JSON_KEY_CODE						"code"
#define		AGI_UB_JSON_KEY_DST_TO						"dst_to"
#define		AGI_UB_JSON_PHONE_HOOK						"phone_hook"

#define		AGI_UB_JSON_VALUE_LED						"led"
#define		AGI_UB_JSON_VALUE_API_HOOK					"api_hook"
#define		AGI_UB_JSON_VALUE_LOUD_SPK					"loud_spk"//喇叭.0->关闭 1->play_to  2->line_to
#define		AGI_UB_JSON_VALUE_HP_SPK					"hp_spk"//耳机
#define		AGI_UB_JSON_VALUE_PLAY_TO_HP_SPK			"play_to_hp_spk"
#define		AGI_UB_JSON_VALUE_PLAY_TO_LINE				"play_to_line"
#define		AGI_UB_JSON_VALUE_LINE_TO_HP_SPK			"line_to_hp_spk"
#define		AGI_UB_JSON_VALUE_MIC_TO_LINE				"mic_to_line"//设备上自带的mic
#define		AGI_UB_JSON_VALUE_SNDCARD_MIC_TO_LINE		"sndcard_mic_to_line"//计算机声卡的mic
#define		AGI_UB_JSON_VALUE_LINE_TO_SNDCARD_SPK		"line_to_sndcard_spk"//计算机声卡的mic
#define		AGI_UB_JSON_VALUE_SNDCARD_MIC_AEC			"sndcard_mic_aec"//启用mic的回音抵消算法,如果关闭就只有声卡自带回音抵消,如果是外放模式并且声卡没有回音抵消才启用
//====================================

#define		AGI_UB_JSON_EVT_ID							"evt_id"
#define		AGI_UB_JSON_EVT_CLASS						"evt_class"

#define		AGI_UB_JSON_VALUE_START						"start"
#define		AGI_UB_JSON_VALUE_STOP						"stop"



#define		AGI_UB_ACCOUNT					"acc"
#define		AGI_UB_DATA						"data"
#define		AGI_UB_AUTH						"auth"
#define		AGI_UB_AUTH_ACC					"acc_auth"
#define		AGI_UB_AUTH_IP					"ip_auth"
#define		AGI_UB_DEV_USB					"usb"
#define		AGI_UB_DEV_SIP					"sip"


#define		AGI_UB_EVENT_CLASS_ACC			"acc"
#define		AGI_UB_EVENT_CLASS_DIALOG		"dialog"
#define		AGI_UB_EVENT_CLASS_CONF			"conf"
#define		AGI_UB_EVENT_CLASS_IVR			"ivr"



#define		AGI_UB_CALL_EVENT_CLASS_REGISTER		"register"
#define		AGI_UB_CALL_EVENT_CLASS_DIALOG			"dialog"
#define		AGI_UB_CALL_EVENT_CLASS_CONF			"conf"
#define		AGI_UB_CALL_EVENT_CLASS_HTTP_ACTION		"http"
#define		AGI_UB_CALL_EVENT_CLASS_WS_ACTION		"ws"
#define		AGI_UB_CALL_EVENT_CLASS_IVR				"ivr"


#define		AGI_UB_LOUD_SPK_DISABLE			0
#define		AGI_UB_LOUD_SPK_LINE			1
#define		AGI_UB_LOUD_SPK_PLAY			2


#define		AUDIO_DEV_TYPE_NULL				-1//不使用,一般用在ivr
#define		AUDIO_DEV_TYPE_DEFAULT			0//使用后台默认的

#define		AGI_UB_JSON_AUDIO_DEVICE		"au_dev"
#define		AUDIO_DEVICE_SNDCARD			"sndcard"
#define		AUDIO_DEVICE_UB_DEV				"ub_dev"


#define		DEV_CFG_KEY_LINE_PARAM			"line_param"
#define		DEV_CFG_KEY_DEV_PARAM			"dev_param"
#define		DEV_CFG_KEY_DEV_ATTR			"dev_attr"//外部自定义,不进行内部操作
#define		DEV_CFG_KEY_AUTO_API_HANGUP		"auto_api_hangup"//自动软挂机
#define		DEV_CFG_KEY_AUTO_RECFILE		"auto_recfile"//自动录音模式,目前默认摘机offhook自动录音,后续可以拨号dialend后/ringing回铃音/connected接通自动录音
#define		DEV_CFG_KEY_UB_GAIN				"ub_gain"
#define		DEV_CFG_KEY_AUDIO_DEVICE		"audio_dev"

#define		DEV_CFG_KEY_LOGOUT				"logout"
#define		DEV_CFG_GAIN_LINE_IN			"line_in"
#define		DEV_CFG_GAIN_LINE_OUT			"line_out"
#define		DEV_CFG_GAIN_LOUD_SPK			"loud_spk"
#define		DEV_CFG_GAIN_HP_MIC				"hp_mic"
#define		DEV_CFG_GAIN_HP_SPK				"hp_spk"


#define		DEV_CFG_KEY_DEVICE				"dev"
#define		DEV_CFG_KEY_VOLUME				"volume"

WT_BEGIN_C

//返回sdk库版本的json信息
int32	 AGI_API		agi_ub_version(char *out_json_buf, int32 buf_size);

//输入输出格式统一使用json格式
//初始化SDK资源
//in_json_param:保留
int32	 AGI_API		agi_ub_init(const char *in_json_param);

//释放SDK资源
int32	 AGI_API		agi_ub_uninit();

//检查是否已经初始化资源
int32	 AGI_API		agi_ub_isinit();

//返回已经连接打开的设备数量
int32	 AGI_API		agi_ub_count();

//创建一个usb设备接口
//in_json_param:保留
int32	 AGI_API		agi_ub_create(const char *in_json_param, char *out_json_buf, int32 buf_size);

//删除一个usb设备接口
//in_json_param:保留
int32	 AGI_API		agi_ub_destroy(const char *in_json_param);

//对设备执行一个操作,具体格式看文档
int32	 AGI_API		agi_ub_action(const char *in_json, char *out_json_buf, int32 buf_size);
//对设备执行一个操作,具体格式看文档
int32	 AGI_API		agi_ub_action_param(const char *ep_acc, const char *act_name, const char *json_param, char *out_json_buf, int32 buf_size);

//简化api操作,不使用json输入
//ep_acc: 1 -> 128设备序号 或者 设备序列号,空就表示第一个设备
//cmd_name 设备控制名称
//value 控制值
int32	 AGI_API		agi_ub_do_ctrl(const char *ep_acc, const char *ctrl_name, int32 value);

//code:线路拨号号码
//dst_to:设备真实输出的号码,一般是code前面加了某些前缀,为空就是等同code.比如:138104xxxxx是客户号码, 9是分机出局号码,一个逗号延迟500ms,可以多个,0表示外地号码需要加拨0
//agi_ub_callout("", "138104xxxxx", "9,0138104xxxxx");
//比如: 客户号码是88888888, 9是分机出局号码,一个逗号延迟500ms,可以多个,010表示外地号码需要加拨0
//agi_ub_callout("", "88888888", "9,01088888888");
int32	 AGI_API		agi_ub_callout(const char *ep_acc, const char *code, const char *dst_to, int32 au_dev);
int32	 AGI_API		agi_ub_dial_dtmf(const char *ep_acc, const char *code);
//来电应答
//au_dev:0->使用后台设置的默认设备 -1就是不用设备,一般用在ivr
int32	 AGI_API		agi_ub_answer(const char *ep_acc, int32 au_dev);
//设备挂机
int32	 AGI_API		agi_ub_hangup(const char *ep_acc);
//开始放音操作
//cmd_name
int32	 AGI_API		agi_ub_playfile_start(const char *ep_acc, const char *file, int32 repeat_cnt);
//停止放音操作
//handle->保留
int32	 AGI_API		agi_ub_playfile_stop(const char *ep_acc, int32 handle);
//录音操作
//file 默认可以为空,也可以手工指定,音频格式太复杂不用设置,使用后台设置方式
int32	 AGI_API		agi_ub_recfile_start(const char *ep_acc, const char *file);
//停止录音操作
//handle->保留
int32	 AGI_API		agi_ub_recfile_stop(const char *ep_acc, int32 handle);

//设备账号状态信息
int32	 AGI_API		agi_ub_acc_info(const char *ep_acc, char *out_json_buf, int32 buf_size);



WT_END_C

#endif

