$(document).ready(
	function(  )
	{
		
		$("#tabs").tabs(  );
		
		var id_table=new Array("audio_alert","video_static","video_miss","video_black","video_colour","delay_time","diffluma","videoslice","avgthreshold","audio_rate","qcif_rate","cif_rate","d1_rate","hd_rate","ch0_pmt","ch0_pcr","ch0_video","ch0_audio","ch1_pmt","ch1_pcr","ch1_video","ch1_audio","ch2_pmt","ch2_pcr","ch2_video","ch2_audio","ch3_pmt","ch3_pcr","ch3_video","ch3_audio","pch0_pmt","pch0_pcr","pch0_video","pch0_audio","pch1_pmt","pch1_pcr","pch1_video","pch1_audio","pch2_pmt","pch2_pcr","pch2_video","pch2_audio","pch3_pmt","pch3_pcr","pch3_video","pch3_audio");
		
		for(var i=0; i<id_table.length; i++)
		{
			onlyNum(id_table[i]);
		}
		$("#zhuangtai1").hide();
		$("#audio_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("audio_alert");
			    jQuery.ajax({
                    url: "./cgi-bin/AudioAlert.cgi",
                    data: $('#audio_form').serialize(  ),
                    type: "POST",
					dateType: "json",
                    beforeSend: function( )
                    {
						if(check(text_id[0],event) == false)
						{
							return false;
						}
                    },
                    error: function(request)  			
					{
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
	
						var all_data = data.split(':');
						
						document.getElementById(text_id[0]).value = all_data[0];
                        alert("表单提交成功");
                    }
                });
				
                return false;
			}
				
		);
		
		$("#video_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("video_static","video_miss","video_black","video_colour","delay_time","diffluma","videoslice","avgthreshold");
				
			    jQuery.ajax({
                    url: "./cgi-bin/VideoAlert.cgi",
                    data: $('#video_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						for(var i=0; i<8; i++)
						{
							if(check(text_id[i],event) == false)
							{
								return false;
							}
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						var all_data = data.split(':');
						for(var i=0; i<8;i++)
						{
							document.getElementById(text_id[i]).value = all_data[i];
						}
						alert("表单提交成功");
                    }
                });
				
                return false;
			}
			
		);
		
		$("#source_form").submit
		(
			
			function(  )
			{
			    jQuery.ajax({
                    url: "./cgi-bin/SourceMode.cgi",
                    data: $('#source_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						
                    },
                    error: function(request)  			
					{   
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						var all_data = data.split(':');
						//alert(all_data[0]);
						if(all_data[0] == 0)
						{
							//alert(all_data[0]);
							//$("#mode_asi").attr("checked",'checked');
							//$("#mode_net").attr("checked",false);
							document.getElementById("mode_asi").checked = true;
							document.getElementById("mode_net").checked = false;
							
						}
						else if(all_data[0] == 1)
						{
							//alert(all_data[0]);
							//$("#mode_asi").attr("checked",false);
							//$("#mode_net").attr("checked",'checked');
							document.getElementById("mode_asi").checked = false;
							document.getElementById("mode_net").checked = true;
						}
						
						alert("表单提交成功");
                    }
                });

                return false;
			}

		);


		$("#channel_form").submit
		(
			
			function(  )
			{
			    jQuery.ajax({
                    url: "./cgi-bin/ChanNum.cgi",
                    data: $('#channel_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						var all_data = data.split(':');
						//alert(all_data[0]);
						var select = document.getElementById("chan_num");
						var ops = select.options;
						for(var i=0; i<7; i++)
						{
							
							if(all_data[0] == i)
							{
								ops[i-1].selected = true;
							}
						
						}
						alert("表单提交成功");
                    }
                });

                return false;
			}

		);
		
		
		$("#origin_form").submit
		(
			
			function(  )
			{
			    jQuery.ajax({
                    url: "./cgi-bin/OriginFlow.cgi",
                    data: $('#origin_form').serialize( ),
                    type: "POST",
					dateType: "json",		
                    beforeSend: function( )
                    {
						
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						var all_data = data.split(':');
						//alert(all_data[0]);
						if(all_data[0] == 0)
						{
							document.getElementById("origin_1").checked = false;
							document.getElementById("origin_2").checked = true;
							
						}
						else if(all_data[0] == 1)
						{
							document.getElementById("origin_1").checked = true;
							document.getElementById("origin_2").checked = false;
						}
						
						alert("表单提交成功");
                    }
                });

                return false;
			}

		);

		$("#videorate_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("qcif_rate","cif_rate","d1_rate","hd_rate");
				
			    jQuery.ajax({
                    url: "./cgi-bin/VideoBitRate.cgi",
                    data: $('#videorate_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						for(var i=0; i<4; i++)
						{
							if(check(text_id[i],event) == false)
							{
								return false;
							}
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						var all_data = data.split(':');
						for(var i=0; i<4;i++)
						{
							document.getElementById(text_id[i]).value = all_data[i];
						}
						alert("表单提交成功");
                    }
                });
				
                return false;
			}
			
		);
		
		$("#audiorate_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("audio_rate");
			    jQuery.ajax({
                    url: "./cgi-bin/AudioBitRate.cgi",
                    data: $('#audiorate_form').serialize(  ),
                    type: "POST",
					dateType: "json",
                    beforeSend: function( )
                    {
						if(check(text_id[0],event) == false)
						{
							return false;
						}
                    },
                    error: function(request)  			
					{
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						var all_data = data.split(':');
						
						document.getElementById(text_id[0]).value = all_data[0];
                        alert("表单提交成功");
                    }
                });
				
                return false;
			}
				
		);
				
		
		$("#videosize_form").submit
		(
			
			function(  )
			{
				var chan=new Array("ch0_qcif","ch0_cif","ch0_d1","ch1_hd","ch1_qcif","ch1_cif","ch1_d1","ch1_hd","ch2_qcif","ch2_cif","ch2_d1","ch2_hd","ch3_qcif","ch3_cif","ch3_d1","ch3_hd");
			    jQuery.ajax({
                    url: "./cgi-bin/VideoFormat.cgi",
                    data: $('#videosize_form').serialize( ),
                    type: "POST",
					dateType: "json",		
                    beforeSend: function( )
                    {
						
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{

						var all_data = data.split(':');
						for(var i=0; i<4; i++)
						{
							
							if(all_data[i] == 2)
							{
								for(var j=0; j<4; j++)
								{
									if(j == 0)
									{
										document.getElementById(chan[i*4+j]).checked = true;
									}
									else
									{
										document.getElementById(chan[i*4+j]).checked = false;
									}
								}
							}
							else if(all_data[i] == 0)
							{
								for(var j=0; j<4; j++)
								{
									if(j == 1)
									{
										document.getElementById(chan[i*4+j]).checked = true;
									}
									else
									{
										document.getElementById(chan[i*4+j]).checked = false;
									}
								}
							
							}
							else if(all_data[i] == 1)
							{
								for(var j=0; j<4; j++)
								{
									if(j == 2)
									{
										document.getElementById(chan[i*4+j]).checked = true;
									}
									else
									{
										document.getElementById(chan[i*4+j]).checked = false;
									}
								}
							}
							else if(all_data[i] == 3)
							{
								for(var j=0; j<4; j++)
								{
									if(j == 3)
									{
										document.getElementById(chan[i*4+j]).checked = true;
									}
									else
									{
										document.getElementById(chan[i*4+j]).checked = false;
									}
								}
							}
						
						}
						alert("表单提交成功");
					
                    }
                });

                return false;
			}

		);
		
		
		
		$("#transcode_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("ch0_pmt","ch0_pcr","ch0_video","ch0_audio","ch1_pmt","ch1_pcr","ch1_video","ch1_audio","ch2_pmt","ch2_pcr","ch2_video","ch2_audio","ch3_pmt","ch3_pcr","ch3_video","ch3_audio");
				
			    jQuery.ajax({
                    url: "./cgi-bin/TransCodeParam.cgi",
                    data: $('#transcode_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						for(var i=0; i<16; i++)
						{
							if(check(text_id[i],event) == false)
							{
								return false;
							}
						}
                    },
                    error: function(request)	
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data)
					{
						var all_data = data.split(':');
						for(var i=0; i<16; i++)
						{
							document.getElementById(text_id[i]).value = all_data[i];
						}
						alert("表单提交成功");
                    }
                });
				
                return false;
			}
			
		);
		
		
		$("#package_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("pch0_pmt","pch0_pcr","pch0_video","pch0_audio","pch1_pmt","pch1_pcr","pch1_video","pch1_audio","pch2_pmt","pch2_pcr","pch2_video","pch2_audio","pch3_pmt","pch3_pcr","pch3_video","pch3_audio");
				
			    jQuery.ajax({
                    url: "./cgi-bin/PacKageParam.cgi",
                    data: $('#package_form').serialize(  ), 
                    type: "POST",
					dateType: "json",			
                    beforeSend: function( )
                    {
						for(var i=0; i<16; i++)
						{
							if(check(text_id[i],event) == false)
							{
								return false;
							}
						}
                    },
                    error: function(request)	
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data)
					{
						var all_data = data.split(':');
						for(var i=0; i<16;i++)
						{
							document.getElementById(text_id[i]).value = all_data[i];
						}
						alert("表单提交成功");
                    }
                });
				
                return false;
			}
			
		);	

		$("#local_net_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("IP3","IP2","IP1","IP0",
	                                  "Mask3","Mask2","Mask1","Mask0",
									  "gateway3","gateway2","gateway1","gateway0");
				
			    jQuery.ajax({
                    url: "./cgi-bin/LocalNet.cgi",
                    data: $('#local_net_form').serialize(  ),
                    type: "POST",
					dateType: "json",	
                    beforeSend: function( )
                    {
/* 						for(var i=0; i<4; i++)
						{
							if(check(text_id[i],event) == false)
							{
								return false;
							}
						} */
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						var all_data = data.split(':');
						for(var i=0; i<12;i++)
						{
							document.getElementById(text_id[i]).value = all_data[i];
						}
						//alert(data);
						alert("表单提交成功");
                    }
                });
				
                return false;
			}
			
		);

		
		$("#dest_net_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("d_ch0_ip3","d_ch0_ip2","d_ch0_ip1","d_ch0_ip0",
									  "d_ch1_ip3","d_ch1_ip2","d_ch1_ip1","d_ch1_ip0",
									  "d_ch2_ip3","d_ch2_ip2","d_ch2_ip1","d_ch2_ip0",
									  "d_ch3_ip3","d_ch3_ip2","d_ch3_ip1","d_ch3_ip0",
									  "d_ch0_port","d_ch1_port","d_ch2_port","d_ch3_port",
									  "alert0_port","alert1_port","alert2_port","alert3_port");
				
			    jQuery.ajax({
                    url: "./cgi-bin/DestNet.cgi",
                    data: $('#dest_net_form').serialize(  ),
                    type: "POST",
					dateType: "json",
                    beforeSend: function( )
                    {
/* 						for(var i=0; i<4; i++)
						{
							if(check(text_id[i],event) == false)
							{
								return false;
							}
						} */
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data)
					{
						var all_data = data.split(':');
						//alert(all_data);
						for(var i=0; i<24;i++)
						{
							document.getElementById(text_id[i]).value = all_data[i];
						}
						//alert(data);
						alert("表单提交成功");
                    }
                });
				
                return false;
			}
			
		);
		
		$("#source_net_form").submit
		(
			
			function(  )
			{
				var text_id=new Array("ch0_ip3","ch0_ip2","ch0_ip1","ch0_ip0",
									  "ch1_ip3","ch1_ip2","ch1_ip1","ch1_ip0",
									  "ch2_ip3","ch2_ip2","ch2_ip1","ch2_ip0",
									  "ch3_ip3","ch3_ip2","ch3_ip1","ch3_ip0",
									  "chan0_port","chan1_port","chan2_port","chan3_port");
				
			    jQuery.ajax({
                    url: "./cgi-bin/SourceNet.cgi",
                    data: $('#source_net_form').serialize(  ),
                    type: "POST",
					dateType: "json",
                    beforeSend: function( )
                    {
/* 						for(var i=0; i<4; i++)
						{
							if(check(text_id[i],event) == false)
							{
								return false;
							}
						} */
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data)
					{
						var all_data = data.split(':');
						//alert(all_data);
						for(var i=0; i<20;i++)
						{
							document.getElementById(text_id[i]).value = all_data[i];
						}
						//alert(data);
						alert("表单提交成功");
                    }
                });
				
                return false;
			}
			
		);
		
		$("#jindutiao").hide();
		//**************************************************************************************//
		$("#user_list").submit
		(
			
			function(  )
			{
				var text_id=new Array("video_static","video_miss","video_black","video_colour","delay_time","diffluma","videoslice","avgthreshold");
				
			    jQuery.ajax({
                    url: "./cgi-bin/user_list.cgi",
                    data: $('#user_list').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("添加成功！");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		
	//**************************************************************************************//
		$("#set_ip").submit
		(
			
			function(  )
			{				
			    jQuery.ajax({
                    url: "./cgi-bin/set_ip.cgi",
                    data: $('#set_ip').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById("ip01").value>255||document.getElementById("ip01").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("ip01").value="";
							return false;
						}
						if(document.getElementById("ip02").value>255||document.getElementById("ip02").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("ip02").value="";
							return false;
						}
						if(document.getElementById("ip03").value>255||document.getElementById("ip03").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("ip03").value="";
							return false;
						}
						if(document.getElementById("ip04").value>255||document.getElementById("ip04").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("ip04").value="";
							return false;
						}
						if(document.getElementById("subnet01").value>255||document.getElementById("subnet01").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("subnet01").value="";
							return false;
						}
						if(document.getElementById("subnet02").value>255||document.getElementById("subnet02").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("subnet02").value="";
							return false;
						}
						if(document.getElementById("subnet03").value>255||document.getElementById("subnet03").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("subnet03").value="";
							return false;
						}
						if(document.getElementById("subnet04").value>255||document.getElementById("subnet04").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("subnet04").value="";
							return false;
						}
						if(document.getElementById("gateway01").value>255||document.getElementById("gateway01").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("gateway01").value="";
							return false;
						}
						if(document.getElementById("gateway02").value>255||document.getElementById("gateway02").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("gateway02").value="";
							return false;
						}
						if(document.getElementById("gateway03").value>255||document.getElementById("gateway03").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("gateway03").value="";
							return false;
						}
						if(document.getElementById("gateway04").value>255||document.getElementById("gateway04").value=="")
						{
							alert("请输0-255这个范围的值");
							document.getElementById("gateway04").value="";
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("配置成功！");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
				
		
		//**************************************************************************************//
		$("#port_reboot").submit
		(
			
			function(  )
			{
				var text_id=new Array("video_static","video_miss","video_black","video_colour","delay_time","diffluma","videoslice","avgthreshold");
				
			    jQuery.ajax({
                    url: "./cgi-bin/client.cgi",
                    data: $('#port_reboot').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {

                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("端口重启成功！");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		
		//**************************************************************************************//
		$("#safe").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/safe.cgi",
                    data: $('#safe').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						var name_buff = new Array("ip1","ip2","ip3","ip4","ip5","ip6","ip7","ip8","ip9","ip10","mask1","mask2","mask3","mask4","mask5","mask6","mask7","mask8","mask9","mask10");
						var data_num = 0;
						for(var i =0 ;i<20;i++)
						{
							if(document.getElementById(name_buff[i]).value != "")
							{
								var buff;
								var value_data;
								value_data = document.getElementById(name_buff[i]).value;
								buff = document.getElementById(name_buff[i]).value.split(".");
								for(var z=0;z<16;z++)
								{
									if(value_data[z]=='.')
									{
										data_num++;
									}	
								}
								if(data_num !=3)
								{
									alert("IP输入不合法");
									return false;
								}
								data_num = 0;
								if(buff[0]>255||buff[1]>255||buff[2]>255||buff[3]>255)
								{
									alert("IP输入不合法");
									return false;
								}
							}
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("提交成功");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		//**************************************************************************************//
		$("#port1_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port1.cgi",
                    data: $('#port1_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						
						if(document.getElementById('port_ip_id1').value==""||document.getElementById('port_bd_id1').value==""||document.getElementById('port_yc_id1').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id1').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id1').value<1025||document.getElementById('port_bd_id1').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id1').value<1025||document.getElementById('port_yc_id1').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id1').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		
		//**************************************************************************************//
		$("#port2_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port2.cgi",
                    data: $('#port2_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById('port_ip_id2').value==""||document.getElementById('port_bd_id2').value==""||document.getElementById('port_yc_id2').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id2').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id2').value<1025||document.getElementById('port_bd_id2').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id2').value<1025||document.getElementById('port_yc_id2').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id2').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		//**************************************************************************************//
		$("#port3_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port3.cgi",
                    data: $('#port3_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById('port_ip_id3').value==""||document.getElementById('port_bd_id3').value==""||document.getElementById('port_yc_id3').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id3').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id3').value<1025||document.getElementById('port_bd_id3').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id3').value<1025||document.getElementById('port_yc_id3').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id3').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						//alert(data);
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		
		//**************************************************************************************//
		$("#port4_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port4.cgi",
                    data: $('#port4_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById('port_ip_id4').value==""||document.getElementById('port_bd_id4').value==""||document.getElementById('port_yc_id4').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id4').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id4').value<1025||document.getElementById('port_bd_id4').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id4').value<1025||document.getElementById('port_yc_id4').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id4').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		//**************************************************************************************//
		$("#port5_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port5.cgi",
                    data: $('#port5_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById('port_ip_id5').value==""||document.getElementById('port_bd_id5').value==""||document.getElementById('port_yc_id5').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id5').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id5').value<1025||document.getElementById('port_bd_id5').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id5').value<1025||document.getElementById('port_yc_id5').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id5').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		//**************************************************************************************//
		$("#port6_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port6.cgi",
                    data: $('#port6_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById('port_ip_id6').value==""||document.getElementById('port_bd_id6').value==""||document.getElementById('port_yc_id6').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id6').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id6').value<1025||document.getElementById('port_bd_id6').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id6').value<1025||document.getElementById('port_yc_id6').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id6').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		//**************************************************************************************//
		$("#port7_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port7.cgi",
                    data: $('#port7_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById('port_ip_id7').value==""||document.getElementById('port_bd_id7').value==""||document.getElementById('port_yc_id7').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id7').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id7').value<1025||document.getElementById('port_bd_id7').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id7').value<1025||document.getElementById('port_yc_id7').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id7').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		//**************************************************************************************//
		$("#port8_form").submit
		(
			
			function(  )
			{			
			    jQuery.ajax({
                    url: "./cgi-bin/port8.cgi",
                    data: $('#port8_form').serialize(  ), 
                    type: "POST",    
					dateType: "json",			
                    beforeSend: function( )
                    {
						if(document.getElementById('port_ip_id8').value==""||document.getElementById('port_bd_id8').value==""||document.getElementById('port_yc_id8').value=="")
						{
							alert("输入为空，请重新输入");
							return false;
						}
						if(document.getElementById('port_bd_id8').value=="9999")
						{
							alert("不能设置为9999");
							return false;
						}
						if(document.getElementById('port_bd_id8').value<1025||document.getElementById('port_bd_id8').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						if(document.getElementById('port_yc_id8').value<1025||document.getElementById('port_yc_id8').value>65535)
						{
							alert("请输入1025-65535范围内的值");
							return false;							
						}
						var buff=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
						buff = document.getElementById('port_ip_id8').value;
						var section = 0;  //每一节的十进制值 
						var i=0;
						for(i=0;i<16;i++)
						{ 
							if(buff[i] == '.')
							{ 
								section++;
								if(section==1)
								{
									if(i>3)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==2)
								{
									if(i>7)
									{
										alert("IP不合法");
										return false;
									}
								}
								if(section==3)
								{
									if(i>11)
									{
										alert("IP不合法");
										return false;
									}
								}
							}
						}
						if(section!=3)
						{
							alert("IP输入不合法");
							return false;									
						}
						var all_data = buff.split('.');
						var num_1=Number(all_data[0]);
						var num_2=Number(all_data[1]);
						var num_3=Number(all_data[2]);
						var num_4=Number(all_data[3]);
						if(num_1>255||num_2>255||num_3>255||num_4>255)
						{
							alert("IP输入不合法");
							return false;
						}
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
					
                    success: function(data) 
					{
						
						alert("参数配置成功!");
                    }
                });
				
                return false;
			}
			
		);
		//****************************************************************************************//
		$("#reboot_form").submit
		(
			
			function(  )
			{
			    jQuery.ajax({
                    url: "./cgi-bin/SysReboot.cgi",
                    data: "name=1",
                    type: "POST",
					dateType: "json",
                    beforeSend: function( )
                    {
						
                    },
                    error: function(request)  			
					{     
                        alert("表单提交出错，请稍后再试");
                    },
                    success: function(shuju) 
					{
						//alert("重启成功");
					   document.getElementById("reboot_button").src="image/restar.png";
					   $("#jindutiao").show();
					   setTimeout("timedMsg()",4000) ;
                    }
                });

                return false;
			}

		);
		
	}
);

function onlyNum(id){
  $("#"+id).keydown(function(event)
     { var keyCode = event.which; 
        if (keyCode == 46 || keyCode == 8 || keyCode == 37 || keyCode == 39 || (keyCode >= 48 && keyCode <= 57) || 
(keyCode >= 96 && keyCode <= 105) ) 
             { return true; } 
             else { return false } }
             ).focus(function() 
                 { this.style.imeMode = 'disabled'; }
                 ); 
}

function check(id,event)
{
	var val = $("#"+id).val(  );
	if (val == "")
	{
		$("#"+id).focus(  );
		window.alert("阈值不能为空");
		return false;
	}
	else if(val > 9999)
	{
		alert(id+"输入的数字小于9999");
		return false;
	}

}

function timedMsg(  )
{
 
 document.getElementById("chongqiwenzi").innerHTML="系统已经重启！";
 $("#jindutiao").hide();
  document.getElementById("reboot_button").src="image/kaishi.png";
 
}

function UpdateUboot(  )
{
	if(document.getElementById("U_Update").value=="")
	{
		return false;
	}
	$("#loading")
	.ajaxStart(function(  ){
		$(this).show();
		$("#form_file").hide(  );
		$("#K_Form").hide(   );
		$("#U_Update").hide(   );
		$("#U_Button").hide(   );
		
		document.getElementById("show_state").value = "正在升级Uboot...";
	})
	.ajaxComplete(function(  ){
		$(this).hide();
		$("#form_file").show(  );
		$("#K_Form").show(  );
		$("#U_Update").show(  );
		$("#U_Button").show(   );
	});

	$.ajaxFileUpload
	(
		{
			url:'./cgi-bin/UpdateUboot.cgi',
			secureuri:false,
			fileElementId:'U_Update',
			dataType: 'json',
			data:{name:'logan', id:'id'},
			success: function (data, status)
			{
				document.getElementById("show_state").value = data.msg;
			},
			error: function (data, status, e)
			{
				alert(data.error);
			}
		}
	)
	return false;
}

function UpdateKernel(  )
{
	if(document.getElementById("K_Update").value=="")
	{
		return false;
	}
	$("#loading")
	.ajaxStart(function(  ){
		$(this).show();
		$("#form_file").hide(  );
		$("#K_Form").hide(   );
		$("#U_Update").hide(   );
		$("#U_Button").hide(   );
		
		document.getElementById("show_state").value = "正在升级内核...";
	})
	.ajaxComplete(function(  ){
		$(this).hide();
		$("#form_file").show(  );
		$("#K_Form").show(  );
		$("#U_Update").show(  );
		$("#U_Button").show(   );
	});

	$.ajaxFileUpload
	(
		{
			url:'./cgi-bin/UpdateKernel.cgi',
			secureuri:false,
			fileElementId:'K_Update',
			dataType: 'json',
			data:{name:'logan', id:'id'},
			success: function (data, status)
			{
				document.getElementById("show_state").value = data.msg;
			},
			error: function (data, status, e)
			{
				alert(data.error);
			}
		}
	)
	return false;
}

function UpdateRamdisk(  )
{
	if(document.getElementById("fileToUpload").value=="")
	{
		return false;
	}
	$("#loading")
	.ajaxStart(function(  ){
		$(this).show();
		$("#form_file").hide(  );
		$("#K_Form").hide(   );
		$("#U_Update").hide(   );
		$("#U_Button").hide(   );
		
		document.getElementById("show_state").value = "正在升级文件系统...";
	})
	.ajaxComplete(function(  ){
		$(this).hide();
		$("#form_file").show(  );
		$("#K_Form").show(  );
		$("#U_Update").show(  );
		$("#U_Button").show(   );
	});

	$.ajaxFileUpload
	(
		{
			url:'./cgi-bin/UpdateSystem.cgi',
			secureuri:false,
			fileElementId:'fileToUpload',
			dataType: 'json',
			data:{name:'logan', id:'id'},
			success: function (data, status)
			{
				document.getElementById("show_state").value = data.msg;
			},
			error: function (data, status, e)
			{
				alert(data.error);
			}
		}
	)
	return false;
}

