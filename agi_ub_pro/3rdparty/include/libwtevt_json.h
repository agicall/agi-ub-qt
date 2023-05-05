#ifndef		__LIBWTEVT_JSON_H__
#define		__LIBWTEVT_JSON_H__
//api接口
#include "wtextern.h"
#include <functional>
#include <memory>

//重新定义log ui输出,这样不用包含真个libwtlog.h了
typedef int32 (AGI_API *ptr_log_ui_callback)(void *pUserData, int32 nModelID, uint32 uLevelID, const char *pLog, int32 nLen, int32 nLog_Encode);

typedef int32 (AGI_API *ptr_evt_json_callback)(void *ptr_user_data, const char *ptr_evt_class_name, const char *ptr_evt_account_name, const char *ptr_evt_json);

//只标准c++支持
typedef std::function<int32(const char *, const char *, const char *)>  func_wt_evt_cb_bind;

#define	  EVT_CB_BIND(func, pThis)			std::bind(func, pThis, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define	  REQ_DATA_CB_BIND(func, pThis)		std::bind(func, pThis, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
//==

#ifndef _REQ_DATA_CALLBACK_
typedef int32 (AGI_API *ptr_req_data_callback)(void *ptr_user_data, const char *ptr_req_param, char *ptr_res_buf, int32 buf_size);
typedef std::function<int32(const char *, char *, int32)>  func_req_data_bind;
#endif


#define		NONE_CLASS_NAME			"_none_class_"
#define		ALL_CLASS_NAME			"_all_class_"
#define		ALL_ACCOUNT_NAME		"_all_account_"

#define		MD5_ACCOUNT_PWD_KEY		"_agicall.com"


#define		REQ_CB_MODE_BIND_PWD	"bind_pwd"//绑定时检测用户密码
#define		REQ_CB_MODE_POST_MSG	"post_msg"//http/ws投递消息,可能是控制设备或者其它信息
#define		REQ_CB_MODE_POST_ACT	"post_act"//操作请求

#define		WTEVT_CLASS_CALLLOG		"calllog"
#define		WTEVT_CLASS_DIALOG		"dialog"
#define		WTEVT_CLASS_ACCOUNT		"account"

//把事件投递到哪些回调
//回调函数/管道/http post/http svr/ws svr
#define		PUSH_EVT_POST_MASK_CB		0x1
#define		PUSH_EVT_POST_MASK_PIPE		0x2
#define		PUSH_EVT_POST_MASK_POST		0x4
#define		PUSH_EVT_POST_MASK_HTTP		0x8
#define		PUSH_EVT_POST_MASK_WS		0x10

#define		PUSH_EVT_POST_MASK_ALL		0xFF

WT_BEGIN_C

	int32	AGI_API	evt_init();

	int32	AGI_API	evt_uninit();

	int32	AGI_API	evt_enable_logout(bool enable);

//dll模式时需要捕获log输出信息,格式跟原始一样就行
	int32	AGI_API	evt_set_logout_callback(void *pUserData, ptr_log_ui_callback  pUI_Callback);

//追加事件
	int32	AGI_API	evt_push_event(const char* p_class_name, const char* p_account_name, const char* p_json_evt_data);
//mask指定该事件只给哪些方式回调
	int32	AGI_API	evt_push_event_mask(int32 mask, const char* p_class_name, const char* p_account_name, const char* p_json_evt_data);
//给用户层可以指定投递给某个auth_key,保留,后期实现调试
	int32	AGI_API	evt_push_event_ep_auth(const char* p_ep_type, int32 ep_id, const char* p_auth_key, const char* p_json_evt_data);
//给用户层可以指定投递给某个agent,保留,后期实现调试
	int32	AGI_API	evt_push_event_ep_agent(const char* p_ep_type, int32 ep_id, const char* p_agent_id, const char* p_json_evt_data);
//输出事件

//callback mode
//适用用c++
	int32	AGI_API	evt_create_json_cb_bind(func_wt_evt_cb_bind cb, const char *p_class_name, const char *p_account_name, int32 &cb_id);
//继承类 CWtEvt_cb_c_base_class
	int32   AGI_API	evt_create_json_cb(ptr_evt_json_callback ptr_func_cb, void *ptr_user_data, const char *p_class_name, const char *p_account_name);

	int32   AGI_API	evt_destroy_json_cb(ptr_evt_json_callback ptr_func_cb, void *ptr_user_data);
	int32   AGI_API	evt_destroy_json_cb_id(int32 cb_id);
//=====

//pipe mode
//一个管道只能一个地方读
	int32   AGI_API	evt_create_json_pipe(const char *p_class_name, const char *p_account_name);

	int32   AGI_API	evt_destroy_json_pipe(int32 pipe_id);
//当前最老一个获取的字符串长度
	int32   AGI_API	evt_get_json_buf_size(int32 pipe_id, int32 timeout_ms);
//当前最老一个获取的事件,读取后自动丢弃
	int32   AGI_API	evt_pop_json_buf_data(int32 pipe_id, char* p_json_buf, int32 buf_size, int32 timeout_ms);

	int32   AGI_API	evt_clear_json_data(int32 pipe_id);
//激活pipe,然后等待的立即返回
	int32   AGI_API	evt_keepalive_json_data(int32 pipe_id);
//设置事件溢出条件, <0就是忽略，保留原来的
	int32   AGI_API	evt_set_pipe_overlow(int32 pipe_id, int32 timeout_ms, int32 max_num);
//===

//远程模式需要校验密码,获取事件需要绑定对应账号,不能为空来表示所有账号
//收到http/ws等用户命令时回调处理，回调时加个命令类型

//特定设置获取密码校验的接口模式,以最后一种设置的实现
//回调模式一般用在c/c++里
	int32	AGI_API	evt_set_req_null();
	int32	AGI_API	evt_set_req_url(const char *url, int32 connect_timeout, int32 total_timeout);
	int32	AGI_API	evt_set_req_cb(ptr_req_data_callback cb, void* p_user_data);
	int32	AGI_API	evt_set_req_bind(func_req_data_bind bind);

//ws/wss mode,
//===
	int32	AGI_API	evt_create_json_ws(const char *p_json_ep_skt);
//返回某个ep_id启动结果
	int32	AGI_API	evt_get_ws_ep_id_err_id(int32 ep_id);
//返回整个json 数组的err_id信息
	int32	AGI_API	evt_get_ws_json_err_id(char* p_json_buf, int32 buf_size);
//效果等同create("")
	int32	AGI_API	evt_destroy_json_ws();

//获取ep_id的服务参数和结果
	int32	AGI_API	evt_get_ws_json_skt(char* p_json_buf, int32 buf_size);
//===


//http/https service mode, bs存在跨域问题,可以先不考虑, 以后有特定需要再考虑,可以让web服务器转发过来就支持

	int32	AGI_API	evt_create_json_http(const char *p_json_ep_skt);
//返回某个ep_id启动结果
	int32	AGI_API	evt_get_http_ep_id_err_id(int32 ep_id);
//返回整个json 数组的err_id信息
	int32	AGI_API	evt_get_http_json_err_id(char* p_json_buf, int32 buf_size);
	int32	AGI_API	evt_destroy_json_http();
//获取ep_id的服务参数和结果
	int32	AGI_API	evt_get_http_json_skt(char* p_json_buf, int32 buf_size);
//==

//http/https post主动投递到对方服务器的模式,可以设置多个url,每个url设置需要的对应事件的class和account
//数组形式，每个数组指定post url,account,class
	int32   AGI_API	evt_create_json_url_post(const char *ptr_json_url);
	int32   AGI_API	evt_destroy_json_url_post();
//
/*
{
	"http_service": "config/evt_post/http_service.json",
	"ws_service": "config/evt_post/ws_service.json",
	"url_post":"config/evt_post/url_post.json"
}
*/
//http/ws/post的参数文件目录,好处是后期对接只需要修改配置文件信息,不需要再调用接口
	int32	AGI_API	evt_set_post_json_cfg(const char *p_json_evt_cfg);

//文件路径保存cfg
	int32	AGI_API	evt_set_post_file_cfg(const char *p_json_cfg_file);


WT_END_C


#endif

