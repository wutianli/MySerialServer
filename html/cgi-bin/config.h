#ifndef _CONFIG_H_
#define _CONFIG_H_


#define  MSGBUFSIZE 		3072
#define  SEND_PORT  		7388 

/*转码通道数*/
#define CHANTOTALNUM  		4
/*本地网络设置命令*/
#define	 SET_IP				"Fuction-ID:Set-IP;"
/*目标网络设置命令*/
#define	 SET_UDP			"Fuction-ID:Set-UDP;"
/*数据源网络设置命令*/
#define	 SET_MULTI			"Fuction-ID:Set-Multi;"
/*视频码率设置命令*/
#define	 SET_RATE			"Fuction-ID:Set-Rate;"
/*音频码率设置命令*/
#define	 SET_AUDIO			"Fuction-ID:Set-Audio;"
/*编码格式设置命令*/
#define	 SET_RESOLUTION		"Fuction-ID:Set-Resolution;"
/*转码PID设置命令*/
#define	 SET_PID			"Fuction-ID:Set-PID;"
/*打包PID设置命令*/
#define	 SET_OUTPID		    "Fuction-ID:Set-OutPid;"
/*视频报警设置命令*/
#define	 SET_VIDEO			"Fuction-ID:Set-Video;"
/*音频报警设置命令*/
#define	 SET_ALERTAUDIO		"Fuction-ID:Set-AlertAudio;"
/*数据源模式设置命令*/
#define	 SET_TSMODE			"Fuction-ID:Set-Tsmode;"
/*原始流设置命令*/
#define	 SET_TSRAW			"Fuction-ID:Set-Tsraw;"
/*转码通道数设置命令*/
#define	 SET_CHANNEL		"Fuction-ID:Set-Chanel;"
/*获取所有数据命令  函数的第三个参数为NULL*/
#define  ASK_STATUS		    "Fuction-ID:Ask-Status;"
/*系统重启命令      函数的第三个参数为NULL*/
#define	 REBOOT				"Fuction-ID:Reboot;"

/*-----------------------------------------------------*/
// 本地网络参数
/*-----------------------------------------------------*/
typedef struct _tagNetConfig {
	char 	ipaddr[16];
	char 	netmask[16];
	char 	gateway[16];
}NetConfig_Obj;

/*-----------------------------------------------------*/
// 目标网络参数
/*-----------------------------------------------------*/
typedef struct _tagDstNetConfig {
	/*目标IP地址*/
	char	ipaddr[16];
	/*目标端口号*/
	int	    port;
	/*报警端口号*/
	int   	alarm_port;
}DstNetConfig_Obj;

/*-----------------------------------------------------*/
// 数据源网络参数
/*-----------------------------------------------------*/
typedef struct _tagMultiNetConfig {
	/*数据源IP*/
	char 	ipaddr[16];
	/*数据源端口号*/
	int		port;
}MultiNetConfig_Obj;

/*-----------------------------------------------------*/
// 视频码率参数:QCIF、CIF、D1、HD
/*-----------------------------------------------------*/
typedef struct _tagVideoBitrateConfig {
	/*QCIF格式视频码率*/
	int		bitrate_qcif;
	/*CIF格式视频码率*/
	int 	bitrate_cif;
	/*D1格式视频码率*/
	int		bitrate_d1;
	/*HD格式视频码率*/
	int		bitrate_hd;
}VideoBitrateConfig_Obj;

/*-----------------------------------------------------*/
// 音频码率参数:
/*-----------------------------------------------------*/
typedef struct _tagAudioBitrateConfig {
	int 	bitrate_audio;
}AudioBitrateConfig_Obj;

/*-----------------------------------------------------*/
// 编码格式参数:
/*-----------------------------------------------------*/
typedef struct _tagVideoSizeConfig {
/*共六个通道   每个通道的对应关系 0(CIF)  1(D1)  2(QCIF)  3(HD) */
	int 	videosize;
}VideoSizeConfig_Obj;

/*-----------------------------------------------------*/
// 转码PID和打包PID参数
/*-----------------------------------------------------*/
typedef struct _tagConfigPID
{
	int 	pmt_pid;
	int 	pcr_pid;
	int 	video_pid;
	int		audio_pid;
}ConfigPID_Obj; 


/*-----------------------------------------------------*/
// 视频报警门限参数
/*-----------------------------------------------------*/
typedef struct _tagAlertVideoRangeConfig {

	/*视频静帧*/
	int 	diffFrameThreshold;
	/*视频丢失*/
	int 	videoMissThreshold;
	/*视频黑场*/
	int 	video_black;
	/*视频彩条*/
	int 	lumaSliceNum;
	/*报警延时*/
	int     delaytime;
	/*视频检测抗噪声*/
	int     diffLumaThreshold;
	/*彩条检测灵敏度*/
	int     videoSliceThreshold;
	/*雪花检测灵敏度*/
	int     avgThreshold;

}AlertVideoConfig_Obj;

/*-----------------------------------------------------*/
// 音频报警门限参数
/*-----------------------------------------------------*/
typedef struct _tagAlertAudioRangeConfig {
	int 	audio_silence;
}AlertAudioConfig_Obj;

/*-----------------------------------------------------*/
// 是否发送原始流参数
/*-----------------------------------------------------*/
typedef struct _tagSendTsRaw
{
	int     tsraw;
}SendTsRaw_Obj;
/*-----------------------------------------------------*/
// 数据源模式参数
/*-----------------------------------------------------*/
typedef struct _tagSourceModeConfig {
	int mode;
}SourceModeConfig_Obj;

/*-----------------------------------------------------*/
// 转码通道数参数
/*-----------------------------------------------------*/
typedef struct _tagChanNum
{
	int channum;
}ChanNum_Obj;
/*-----------------------------------------------------*/
// 异态报警滚动条参数
/*-----------------------------------------------------*/

typedef struct _tagAlertstate
{
	/*视频静帧*/
	int   staticvideo[CHANTOTALNUM];
	/*视频劣播*/
	int   snowvideo[CHANTOTALNUM];
	/*视频黑场*/
	int	  videomiss[CHANTOTALNUM];
	/*视频彩条*/
	int	  colourslice[CHANTOTALNUM];
	/*视频信号丢失*/
	int   video_lost_lock[CHANTOTALNUM];
	/*有无音频信号*/
	int   audiostatus[CHANTOTALNUM];
} Alertstate_Obj;

/*-----------------------------------------------------*/
// 解包后的数据
/*-----------------------------------------------------*/
/*
typedef struct _tagUnPackagePara
{

	NetConfig_Obj 			   local_net;
	DstNetConfig_Obj		   dest_net;
	MultiNetConfig_Obj  	   src_net;
	VideoBitrateConfig_Obj     video_bitrate;
	AudioBitrateConfig_Obj     audio_bitrate;
	VideoSizeConfig_Obj        video_size;
	ConfigPID_Obj			   set_transpid;
	ConfigPID_Obj  			   set_codecpid;
	AlertVideoConfig_Obj       alert_video;
	AlertAudioConfig_Obj       alert_audio;
	SendTsRaw_Obj              send_tsraw;
	SourceModeConfig_Obj       source_mode;
	ChanNum_Obj                chan_num;
	Alertstate_Obj			   alert_state;

}UnPackagePara;
*/

/*-----------------------------------------------------*/
// 打包解包函数
/*-----------------------------------------------------*/
int DataPackage(char *str_sendbuf, char *command, void *data, int  chan_num);
int DataUnpackage(char *str_recvbuf, char *command, void *data, int  chan_num);

/*-----------------------------------------------------*/
// 网络发送接收函数
/*-----------------------------------------------------*/
void socket_func(char *msg_sendbuf, char *msg_recvbuf,int recvlength, int port);

#endif
